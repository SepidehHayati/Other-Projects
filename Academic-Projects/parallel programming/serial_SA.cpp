#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/resource.h> // For memory usage calculation on Unix/Linux
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace std;

// Constants
const int ITERATIONS = 100;        // Number of K-Means iterations
const int NO_OF_CLUSTERS = 3;      // Number of clusters
const int CENTROIDS_SIZE = 100;    // Size of centroid markers in plot
const int POINTS_SIZE = 30;        // Size of point markers in plot
const string CENTROIDS_COLOR = "red"; // Color of centroids in plot

// Point class to represent 2D coordinates
class Point {
private:
    float x, y;
public:
    Point(float x = 0, float y = 0) : x(x), y(y) {}
    float getX() const { return x; }
    float getY() const { return y; }
    void setX(float x) { this->x = x; }
    void setY(float y) { this->y = y; }
};

// KMeans class for clustering
class KMeans {
private:
    int k;                     // Number of clusters
    vector<Point> points;      // Data points
    vector<Point> centroids;   // Cluster centroids
    vector<int> assignments;   // Cluster assignment for each point

    // Calculate Euclidean distance between two points
    float distance(const Point& p1, const Point& p2) {
        float dx = p1.getX() - p2.getX();
        float dy = p1.getY() - p2.getY();
        return sqrt(dx * dx + dy * dy);
    }

public:
    KMeans(int k, const vector<vector<float>>& data) : k(k) {
        // Load points from input data
        points.reserve(data.size());
        for (const auto& row : data) {
            points.emplace_back(row[0], row[1]);
        }
        assignments.resize(points.size(), 0);

        // Randomly select initial centroids
        srand(static_cast<unsigned>(time(nullptr)));
        vector<int> centroid_indices;
        while (centroid_indices.size() < k) {
            int idx = rand() % points.size();
            if (find(centroid_indices.begin(), centroid_indices.end(), idx) == centroid_indices.end()) {
                centroid_indices.push_back(idx);
                centroids.push_back(points[idx]);
            }
        }
    }

    // Assign each point to the nearest centroid
    void assignClusters() {
        for (size_t i = 0; i < points.size(); ++i) {
            float min_dist = distance(points[i], centroids[0]);
            int cluster = 0;
            for (int j = 1; j < k; ++j) {
                float dist = distance(points[i], centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    cluster = j;
                }
            }
            assignments[i] = cluster;
        }
    }

    // Recalculate centroids based on current assignments
    void updateCentroids() {
        vector<Point> new_centroids(k, Point(0, 0));
        vector<int> counts(k, 0);

        // Sum coordinates of points in each cluster
        for (size_t i = 0; i < points.size(); ++i) {
            int cluster = assignments[i];
            new_centroids[cluster].setX(new_centroids[cluster].getX() + points[i].getX());
            new_centroids[cluster].setY(new_centroids[cluster].getY() + points[i].getY());
            counts[cluster]++;
        }

        // Compute new centroids by averaging
        for (int i = 0; i < k; ++i) {
            if (counts[i] > 0) {
                new_centroids[i].setX(new_centroids[i].getX() / counts[i]);
                new_centroids[i].setY(new_centroids[i].getY() / counts[i]);
            }
        }
        centroids = new_centroids;
    }

    // Run the K-Means algorithm
    void run() {
        for (int i = 0; i < ITERATIONS; ++i) {
            assignClusters();
            updateCentroids();
        }
    }

    // Getters for results
    vector<Point> getCentroids() const { return centroids; }
    vector<int> getAssignments() const { return assignments; }
    vector<Point> getPoints() const { return points; }
};

// Function to get memory usage (Unix systems)
long getMemoryUsage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory usage in kilobytes
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Please provide the dataset filename!" << endl;
        return 1;
    }

    // Record start time
    clock_t start_time = clock();

    // Load data from CSV file
    string filename = "./dataset/" + string(argv[1]);
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Cannot open file " << filename << "!" << endl;
        return 1;
    }

    vector<vector<float>> data;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        vector<float> row;
        string value;
        while (getline(ss, value, ',')) {
            row.push_back(stof(value));
        }
        data.push_back(row);
    }
    file.close();

    // Run K-Means clustering
    KMeans kmeans(NO_OF_CLUSTERS, data);
    kmeans.run();

    // Extract results
    vector<Point> centroids = kmeans.getCentroids();
    vector<int> assignments = kmeans.getAssignments();
    vector<Point> points = kmeans.getPoints();

    // Prepare data for plotting
    vector<float> x_points, y_points, x_centroids, y_centroids;
    for (const auto& p : points) {
        x_points.push_back(p.getX());
        y_points.push_back(p.getY());
    }
    for (const auto& c : centroids) {
        x_centroids.push_back(c.getX());
        y_centroids.push_back(c.getY());
    }

    // Print final centroids
    for (const auto& c : centroids) {
        cout << "(" << c.getX() << "," << c.getY() << ")\n";
    }

    // Plot the results
    plt::scatter_colored(x_points, y_points, assignments, POINTS_SIZE);
    plt::scatter(x_centroids, y_centroids, CENTROIDS_SIZE, {{"color", CENTROIDS_COLOR}});
    plt::save("output_serial.png");

    // Calculate execution time and memory usage
    clock_t end_time = clock();
    double elapsed_time = static_cast<double>(end_time - start_time) / CLOCKS_PER_SEC;
    long memory_usage = getMemoryUsage();

    cout << "Execution time: " << elapsed_time << " seconds" << endl;
    cout << "Memory usage: " << memory_usage << " KB" << endl;

    return 0;
}
