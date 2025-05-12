# Data Engineering Project with SQL 
This project demonstrates a complete data pipeline using SQL, including Data Warehousing, ETL/ELT processes, Exploratory Data Analysis, and Advanced Analytics to support business decision-making.

🚧 **Note:** This project is currently in progress and the repository is being updated step by step.  
📝 Project planning and progress tracking are available on [Notion](https://www.notion.so/Data-Warehouse-Project-1f18f6be5865804f801ed6acfc875abb?pvs=4).

## Part 1: Steps of Project:
Steps of the project:
### 1. Data Warehouse (Organize, Structure, Prepare)
This phase focuses on preparing and organizing raw data into a structured and optimized format suitable for analysis.

- **ETL/ELT Processing**: Extract, Transform, and Load data into a data warehouse.
- **Data Architecture**: Design a scalable and efficient data architecture.
- **Data Integration**: Combine data from multiple sources into a unified view.
- **Data Cleansing**: Remove inconsistencies and ensure data quality.
- **Data Load**: Load processed data into the warehouse.
- **Data Modeling**: Design schemas (star/snowflake) and define relationships.

### 2. Exploratory Data Analysis (Understand Data)
This phase is about discovering patterns and gaining initial insights into the data.

- **Basic Queries**: Perform simple `SELECT`, `WHERE`, `ORDER BY`, `LIMIT`, etc.
- **Data Profiling**: Understand data distributions, nulls, and types.
- **Simple Aggregations**: Use `COUNT`, `SUM`, `AVG`, `MIN`, `MAX`, and `GROUP BY`.
- **Subqueries**: Write nested queries for intermediate data processing.

### 3. Advanced Data Analytics (Answer Business Questions)
This phase is aimed at providing business insights using advanced SQL techniques.
- Use complex joins, window functions, and conditional logic.
- Develop reports and KPIs.
- Support decision-making through query-driven insights.
- window function
- CTE
- Subqueries
- Reports

## Part 2: Definition and Methods:

### What is ETL?
**ETL (Extract, Transform, Load)** is a data integration process that combines data from multiple sources into a single, consistent data store.

#### E: Extraction
- Extraction Methods:
   * Push Extraction
   * Pull Extraction ✅ (_Used in this project_)

- Extraction Types:
  * Full Extraction ✅ (_Used in this project_)
  * Incremental Extraction

- Extraction Techniques:
  * Manual Data Extraction
  * Database Querying
  * File Parsing ✅ (_Used in this project_)
  * API Calls
  * Event-Based Streaming
  * SDS
  * Web Scraping

#### T: Transformation Techniques:
- Data Enrichment
- Data Integration
- Derived Columns
- Data Cleansing:
  * Remove Duplicates
  * Data Filtering
  * Handling Missing Data
  * Handling Invalid Values
  * Trimming Spaces
  * Data Type Casting
  * Outlier Detection
- Data Normalization & Standardization
- Business Rules & Logic
- Data Aggregation

✅ _All types of transformation are applied in this project_

#### L: Load
- Processing Types (Batch processing, Streaming processing) ✅ (_Batch Processing is used in this project_)
- Load Method (Full, Incremental)
    * Full Load: Truncate and insert, Update and insert (Upsert), Drop-Create-Insert ✅ (_Full Load is Used in this project_)
    * Incremental Load: Upsert, Append, Merge(Update, Insert, Delete)
- Slowly Changing Dimention (SCD)
    * SDC 0: No Historization (we are no going to update anything)
    * SCD 1: Overwrite (we are updating the records with the new information from resources by overwriting the old value; Upsert: Update + Insert and we losse history) ✅ (*Used in this project*)
    * SCD 2: Historization (we insert new recorde without deleting and overwriting the old data)

## Part 3: Download the Materials:
This material is based on the **SQL Data Warehouse course by Baraa**.

🔗 [Download Material](https://www.datawithbaraa.com/sql-introduction/advanced-sql-project/#google_vignette)

## Part 4: Create Project Plan using Notion:
To better organize and track the progress of this project, I created a dedicated Notion workspace that includes tasks, timelines, and planning documentation.

🔗 [View the Notion Project Planning Page](https://www.notion.so/Data-Warehouse-Project-1f18f6be5865804f801ed6acfc875abb?pvs=4)


## Part 5: Analyzing Project Requirement:
### Building the DWH (Data Engineering)
#### objective
Developing a DWH using SQL Server 
#### Specification
- Data Sources: Import data from two source systems (ERP and CRM) provided as CSV files.
- Data Quality: Cleanse and resolve data quality issues before analysis.
- Integration: Combine both sources into a single, user-friendly data model designed for analytical queries.
- Scope: Focus on the latest dataset only; historization of data is not required.
- Documentation: Provide clear documentation of the data model to support both business stakeholders and analytics teams.
### BI: Analytics and Reporting
Developing SQL-based Analytics to deliver detailed insights

## Part 6: Choose Data Management Architecture
### Types 
In this project, different types of data management architectures were considered, and **Data Warehouse** was selected as the most appropriate solution for structured data.

1. Data Warehouse (structured data) ✅ (_Used in this project_)
2. Data Lake (semi-structured data)
3. Data Lakehouse (A hybrid approach combining the flexibility of data lakes with the structure and management features of data warehouses.)
4. Data Mesh (A decentralized architecture where different domains own and manage their data independently, following a product-based approach.)
   
## Part 7: How to build a Data Warehouse
Several well-known methodologies exist for building a Data Warehouse. In this project, the **Medallion Architecture** was chosen due to its layered approach and clear separation of raw, cleaned, and curated data.

1. Inmon
2. Kimball
3. Data Vault
4. Medallion Architecture ✅ (_Used in this project_)  
   A modern layered architecture commonly used in lakehouse environments. It organizes data into three progressive layers:

   - **Bronze Layer**  
     Raw data (e.g., full-load tables).  
     _Load method: As-is, no transformation._

   - **Silver Layer**  
     Cleaned and standardized data ready for analysis.  
     _Includes data transformation, standardization, aggregation, and integration._  
     _Load method: Full load with transformation._

   - **Gold Layer**  
     Curated data optimized for analytics and reporting.  
     _Stored as views or star schema tables for use in dashboards and business intelligence tools._
