#include <iostream>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <set>
#include <mpi.h>
#include <fstream>
#include <sstream>
#include "matplotlibcpp.h"

#define MAX_ITERATIONS 100
#define CLUSTER_COUNT 3
#define MARKER_SIZE_CENTERS 100
#define MARKER_SIZE_DOTS 30
#define CENTER_COLOR "red"

namespace plt = matplotlibcpp;

using namespace std;

class Coordinate {
private:
    float coordX;
    float coordY;
public:
    Coordinate() : coordX(0), coordY(0) {}
    Coordinate(float x, float y) : coordX(x), coordY(y) {}
    
    float getCoordX() { return coordX; }
    float getCoordY() { return coordY; }
    void setCoordX(float x) { coordX = x; }
    void setCoordY(float y) { coordY = y; }
};

struct AlignedIndex {
    int clusterNum;
    char filler[64 - sizeof(int)];
};

class ParallelCluster {
private:
    int totalClusters;
    vector<Coordinate*> dataPoints;
    vector<Coordinate> clusterCenters;
    vector<int> assignments;
    int globalDataSize; // To track total data size across processes

public:
    ParallelCluster(int clusterQty, int dataSize, vector<vector<float>*>& inputData) {
        int processId, totalProcesses;
        MPI_Comm_rank(MPI_COMM_WORLD, &processId);
        MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);
        totalClusters = clusterQty;
        globalDataSize = dataSize;

        // Allocate data points for all processes
        dataPoints.resize(dataSize);

        // Root process populates dataPoints
        if (processId == 0) {
            for (int idx = 0; idx < dataSize; idx++) {
                dataPoints[idx] = new Coordinate(inputData[idx]->at(0), inputData[idx]->at(1));
            }
        }

        // Broadcast all data points to all processes to ensure consistency
        for (int i = 0; i < dataSize; i++) {
            Coordinate temp;
            if (processId == 0) temp = *dataPoints[i];
            MPI_Bcast(&temp, sizeof(Coordinate), MPI_BYTE, 0, MPI_COMM_WORLD);
            if (processId != 0) dataPoints[i] = new Coordinate(temp.getCoordX(), temp.getCoordY());
        }

        // Initialize cluster centers only on root and broadcast once
        srand(time(NULL) + processId);
        if (processId == 0) {
            vector<int> selectedIndices;
            int count = 0;
            while (count < clusterQty) {
                int randIdx = rand() % dataSize;
                if (find(selectedIndices.begin(), selectedIndices.end(), randIdx) == selectedIndices.end()) {
                    selectedIndices.push_back(randIdx);
                    clusterCenters.push_back(Coordinate(dataPoints[randIdx]->getCoordX(), 
                                                       dataPoints[randIdx]->getCoordY()));
                    count++;
                }
            }
        }
        clusterCenters.resize(clusterQty);
        MPI_Bcast(clusterCenters.data(), clusterQty * sizeof(Coordinate), MPI_BYTE, 0, MPI_COMM_WORLD);
    }

    vector<Coordinate> getStartingCenters() {
        return clusterCenters;
    }

    float computeDistance(Coordinate* pt1, Coordinate pt2) {
        float diffX = pt1->getCoordX() - pt2.getCoordX();
        float diffY = pt1->getCoordY() - pt2.getCoordY();
        return sqrt(diffX * diffX + diffY * diffY);
    }

    vector<AlignedIndex> findNearestCenters() {
        int processId, totalProcesses;
        MPI_Comm_rank(MPI_COMM_WORLD, &processId);
        MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);

        // Calculate local segment for this process
        int segmentSize = globalDataSize / totalProcesses;
        int remainder = globalDataSize % totalProcesses;
        int beginIdx = processId * segmentSize + min(processId, remainder);
        int endIdx = beginIdx + segmentSize + (processId < remainder ? 1 : 0);
        if (endIdx > globalDataSize) endIdx = globalDataSize;

        vector<AlignedIndex> localNearest(globalDataSize);
        vector<float> distances;

        // Compute nearest centers only for local segment
        for (int i = beginIdx; i < endIdx; i++) {
            distances.clear();
            for (int j = 0; j < clusterCenters.size(); j++) {
                distances.push_back(computeDistance(dataPoints[i], clusterCenters[j]));
            }
            auto minDist = min_element(distances.begin(), distances.end());
            localNearest[i].clusterNum = distance(distances.begin(), minDist);
        }

        // Gather all assignments efficiently
        vector<int> recvCounts(totalProcesses);
        vector<int> displs(totalProcesses);
        int offset = 0;
        for (int i = 0; i < totalProcesses; i++) {
            recvCounts[i] = segmentSize + (i < remainder ? 1 : 0);
            displs[i] = offset;
            offset += recvCounts[i];
        }

        MPI_Allgatherv(localNearest.data() + beginIdx, endIdx - beginIdx, MPI_BYTE,
                      localNearest.data(), recvCounts.data(), displs.data(), MPI_BYTE,
                      MPI_COMM_WORLD);

        return localNearest;
    }

    void updateCenters() {
        int processId, totalProcesses;
        MPI_Comm_rank(MPI_COMM_WORLD, &processId);
        MPI_Comm_size(MPI_COMM_WORLD, &totalProcesses);

        vector<AlignedIndex> currentAssignments = findNearestCenters();
        vector<int> clusterList(currentAssignments.size());
        for (int i = 0; i < currentAssignments.size(); i++) {
            clusterList[i] = currentAssignments[i].clusterNum;
        }

        // Compute local sums and counts for the full dataset
        vector<Coordinate> localCenters(totalClusters, Coordinate(0, 0));
        vector<int> localCounts(totalClusters, 0);

        // Each process computes only its local portion
        int segmentSize = globalDataSize / totalProcesses;
        int remainder = globalDataSize % totalProcesses;
        int beginIdx = processId * segmentSize + min(processId, remainder);
        int endIdx = beginIdx + segmentSize + (processId < remainder ? 1 : 0);
        if (endIdx > globalDataSize) endIdx = globalDataSize;

        for (int i = beginIdx; i < endIdx; i++) {
            int clusterIdx = clusterList[i];
            localCenters[clusterIdx].setCoordX(localCenters[clusterIdx].getCoordX() + dataPoints[i]->getCoordX());
            localCenters[clusterIdx].setCoordY(localCenters[clusterIdx].getCoordY() + dataPoints[i]->getCoordY());
            localCounts[clusterIdx]++;
        }

        // Reduce sums and counts globally across all processes
        vector<Coordinate> globalCenters(totalClusters, Coordinate(0, 0)); // Initialize to avoid garbage
        vector<int> globalCounts(totalClusters, 0);
        MPI_Allreduce(localCenters.data(), globalCenters.data(), totalClusters * 2, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(localCounts.data(), globalCounts.data(), totalClusters, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

        // Update centers on all processes
        for (int i = 0; i < totalClusters; i++) {
            if (globalCounts[i] > 0) {
                clusterCenters[i].setCoordX(globalCenters[i].getCoordX() / globalCounts[i]);
                clusterCenters[i].setCoordY(globalCenters[i].getCoordY() / globalCounts[i]);
            } else {
                // If no points assigned to a cluster, keep the old center to avoid NaN
                clusterCenters[i] = clusterCenters[i];
            }
        }

        assignments = clusterList;
    }

    vector<int> getAssignments() {
        return assignments;
    }

    int getPointAssignment(int idx) {
        return assignments[idx];
    }

    ~ParallelCluster() {
        for (int i = 0; i < dataPoints.size(); i++) {
            delete dataPoints[i];
        }
    }
};

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int processId;
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);

    double startTime = MPI_Wtime();

    vector<vector<float>*> inputValues;
    int totalDataPoints = 0;
    if (processId == 0) {
        string filePath = "./dataset/" + string(argv[1]);
        ifstream dataFile(filePath);
        if (!dataFile.is_open()) {
            cout << "Failed to open file " << filePath << endl;
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        string row;
        while (getline(dataFile, row)) {
            totalDataPoints++;
            stringstream parser(row);
            vector<float>* coords = new vector<float>;
            string num;
            while (getline(parser, num, ',')) {
                coords->push_back(stof(num));
            }
            inputValues.push_back(coords);
        }
        dataFile.close();
    }

    // Broadcast totalDataPoints to all processes
    MPI_Bcast(&totalDataPoints, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Create clustering object
    ParallelCluster* clusterAlgo = new ParallelCluster(CLUSTER_COUNT, totalDataPoints, inputValues);

    // Run k-means iterations
    for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
        clusterAlgo->updateCenters();
    }

    // Output results only from root process
    if (processId == 0) {
        vector<Coordinate> finalCenters = clusterAlgo->getStartingCenters();
        vector<float> centerXCoords;
        vector<float> centerYCoords;
        vector<float> dataXCoords(inputValues.size());
        vector<float> dataYCoords(inputValues.size());
        vector<int> dataAssignments(inputValues.size());

        for (int i = 0; i < finalCenters.size(); i++) {
            centerXCoords.push_back(finalCenters[i].getCoordX());
            centerYCoords.push_back(finalCenters[i].getCoordY());
        }

        for (int i = 0; i < inputValues.size(); i++) {
            dataXCoords[i] = inputValues[i]->at(0);
            dataYCoords[i] = inputValues[i]->at(1);
            dataAssignments[i] = clusterAlgo->getPointAssignment(i);
        }

        for (int i = 0; i < finalCenters.size(); i++) {
            cout << "(" << finalCenters[i].getCoordX() << "," << finalCenters[i].getCoordY() << ")\n";
        }

        plt::scatter_colored(dataXCoords, dataYCoords, dataAssignments, MARKER_SIZE_DOTS);
        plt::scatter(centerXCoords, centerYCoords, MARKER_SIZE_CENTERS, {{"color", CENTER_COLOR}});
        plt::save("output.png");
    }

    double endTime = MPI_Wtime();
    if (processId == 0) {
        cout << "Execution time = " << endTime - startTime << " sec\n";
    }

    // Clean up memory
    if (processId == 0) {
        for (int i = 0; i < totalDataPoints; i++) {
            delete inputValues[i];
        }
    }
    delete clusterAlgo;

    MPI_Finalize();
    return 0;
}
