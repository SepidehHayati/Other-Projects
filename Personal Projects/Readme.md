# Data Engineering Project with SQL 
This project demonstrates a complete data pipeline using SQL, including Data Warehousing, ETL/ELT processes, Exploratory Data Analysis, and Advanced Analytics to support business decision-making.

## 1. Data Warehouse (Organize, Structure, Prepare)

This phase focuses on preparing and organizing raw data into a structured and optimized format suitable for analysis.

- **ETL/ELT Processing**: Extract, Transform, and Load data into a data warehouse.
- **Data Architecture**: Design a scalable and efficient data architecture.
- **Data Integration**: Combine data from multiple sources into a unified view.
- **Data Cleansing**: Remove inconsistencies and ensure data quality.
- **Data Load**: Load processed data into the warehouse.
- **Data Modeling**: Design schemas (star/snowflake) and define relationships.

## 2. Exploratory Data Analysis (Understand Data)

This phase is about discovering patterns and gaining initial insights into the data.

- **Basic Queries**: Perform simple `SELECT`, `WHERE`, `ORDER BY`, `LIMIT`, etc.
- **Data Profiling**: Understand data distributions, nulls, and types.
- **Simple Aggregations**: Use `COUNT`, `SUM`, `AVG`, `MIN`, `MAX`, and `GROUP BY`.
- **Subqueries**: Write nested queries for intermediate data processing.

## 3. Advanced Data Analytics (Answer Business Questions)

This phase is aimed at providing business insights using advanced SQL techniques.

- Use complex joins, window functions, and conditional logic.
- Develop reports and KPIs.
- Support decision-making through query-driven insights.
- window function
- CTE
- Subqueries
- Reports

# Definitions related to the Project:
## What is ETL?
**ETL (Extract, Transform, Load)** is a data integration process that combines data from multiple sources into a single, consistent data store.


### Extraction (E)
**Extraction Methods**:
- Push Extraction
- Pull Extraction ✅ (Used in this project)

**Extraction Types**:
- Full Extraction ✅ (Used in this project)
- Incremental Extraction

**Extraction Techniques**:
- Manual Data Extraction
- Database Querying
- File Parsing ✅ (Used in this project)
- API Calls
- Event-Based Streaming
- SDS
- Web Scraping

**Transformation Techniques**:
- Data Enrichment
- Data Integration
- Derived Columns
- **Data Cleansing**:
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

✅ *All applied in this project*

### Load (L)
- Processing Types (Batch processing, Streaming processing) ✅ (*Batch Processing is used in this project*)
- Load Method (Full, Incremental)
    * Full Load: Truncate and insert, Update and insert (Upsert), Drop-Create-Insert ✅ (*Full Load is Used in this project*)
    * Incremental Load: Upsert, Append, Merge(Update, Insert, Delete)
- Slowly Changing Dimention (SCD)
    * SDC 0: No Historization (we are no going to update anything)
    * SCD 1: Overwrite (we are updating the records with the new information from resources by overwriting the old value; Upsert: Update + Insert and we losse history) ✅ (*Used in this project*)
    * SCD 2: Historization (we insert new recorde without deleting and overwriting the old data)



**Project requirements:**
Building the Datawarhouse (Data Engineering):
specifications:
1. Data sources
2. Data Quality
3. Integration
4. Scope
5. documentation
