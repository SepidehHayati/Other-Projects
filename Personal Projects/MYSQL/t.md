# Layoff Trends Analysis & Data Cleaning in SQL

This project involves analyzing real-world layoff data using SQL. The objective is to clean the dataset and extract meaningful insights by identifying trends across various dimensions such as company, industry, country, funding stage, and time.

## Data Cleaning Steps

- Removed duplicate records using `ROW_NUMBER()`
- Standardized inconsistent text values (e.g., whitespace, case sensitivity)
- Converted date formats to SQL `DATE` type
- Handled null and blank values by referencing similar entries
- Dropped irrelevant or empty rows and columns

## Exploratory Data Analysis (EDA)

The following analyses were performed:

- Companies with the highest number of layoffs  
- Industries most affected by layoffs  
- Layoffs by country  
- Layoffs by funding stage  
- Year-over-year trends  
- Monthly rolling total of layoffs  
- Top 5 companies with the most layoffs each year  

## Key Skills Demonstrated

- SQL: Joins, CTEs, Window Functions, Aggregations  
- Data Cleaning and Standardization  
- Trend Analysis & Business Insight Extraction  

## Dataset

The dataset used in this project is publicly available and can be found [here](https://github.com/AlexTheAnalyst/MySQL-YouTube-Series/blob/main/layoffs.csv).  
It contains layoff records from companies in the tech industry.
