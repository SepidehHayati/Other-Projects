-- Data Cleaning:
-- Plan of data cleaning:
-- 1. Remove duplicates
-- 2. Standardize the data
-- 3. Handle null or blank values
-- 4. Remove unnecessary columns

-- 1. Remove duplicates:

-- Step 1: View original data
SELECT * 
FROM layoffs;

-- Step 2: Create a copy of the original table to manipulate
CREATE TABLE layoff_staging LIKE layoffs;

-- Step 3: Insert data from original table into the staging table
INSERT INTO layoff_staging
SELECT * 
FROM layoffs;

-- Step 4: Check data in the staging table
SELECT * 
FROM layoff_staging;

-- Step 5: Identify duplicate rows using ROW_NUMBER()
with duplicate_cte as
(
select *, row_number() over(partition by company, location, industry, total_laid_off, percentage_laid_off, `date`, stage, country, funds_raised_millions) as row_num
from layoff_staging
)
select *
from duplicate_cte
where row_num > 1 ;

-- Step 6: Delete duplicates - This attempt does not work because DELETE cannot be done directly from a CTE
/*
with duplicate_cte as
(
select *, row_number() over(partition by company, location, industry, total_laid_off, percentage_laid_off, `date`, stage, country, funds_raised_millions) as row_num
from layoff_staging
)
delete 
from duplicate_cte
where row_num > 1 ;
*/

-- Step 7: Alternative way to delete duplicates by creating a new table
-- Create a new staging table with an extra 'row_num' column
CREATE TABLE `layoff_staging2` (
  `company` text,
  `location` text,
  `industry` text,
  `total_laid_off` int DEFAULT NULL,
  `percentage_laid_off` text,
  `date` text,
  `stage` text,
  `country` text,
  `funds_raised_millions` int DEFAULT NULL,
  `row_num` int
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

-- Insert data into the new staging table with row numbers to identify duplicates
INSERT INTO layoff_staging2
SELECT *, 
ROW_NUMBER() OVER(PARTITION BY company, location, industry, total_laid_off, percentage_laid_off, `date`, stage, country, funds_raised_millions) AS row_num
FROM layoff_staging;

-- Check the duplicate rows (row_num > 1)
select * 
from layoff_staging2
where row_num > 1;

-- Delete duplicate rows from the new staging table
delete 
from layoff_staging2
where row_num > 1;

-- Final check to see cleaned data
select * 
from layoff_staging2;

-- 2. Standardize the data (Finding issues in the data and fixing them):

-- Trim extra spaces from the 'company' column
select company, trim(company)
from layoff_staging2;

update layoff_staging2
set company = trim(company);

-- Standardize the 'industry' names starting with 'crypto' (e.g., 'Crypto', 'cryptocurrency', etc.)
select *
from layoff_staging2
where industry like 'crypto%';

update layoff_staging2
set industry = 'crypto'
where industry like 'crypto%';

-- Standardize country names starting with 'united state' (e.g., 'United States', 'United State', etc.)
update layoff_staging2
set country = 'united state'
where country like 'united state%';

-- Convert the 'date' column from text to proper DATE format
select `date`,
str_to_date(`date`, '%m/%d/%Y')
from layoff_staging2 ;

update layoff_staging2
set `date` = str_to_date(`date`, '%m/%d/%Y');

-- Modify the 'date' column data type to DATE
alter table layoff_staging2
modify column `date` date;

-- 3: Handle null or blank values

-- Identify rows where both 'total_laid_off' and 'percentage_laid_off' are null
select *
from layoff_staging2
where total_laid_off is null 
and percentage_laid_off is null;

-- Replace blank strings in 'industry' column with NULL
update layoff_staging2
set industry = null
where industry = '';

-- Verify all NULL or blank 'industry' values
select *
from layoff_staging2
where industry is null or industry = '';

-- Try to find missing industry values by matching with other rows that have the same company name
select t1.industry,t2.industry
from layoff_staging2 t1
join layoff_staging2 t2
	on t1.company = t2.company
where (t1.industry is null or t1.industry = '')
and t2.industry is not null;

-- Fill missing 'industry' values using matched rows
UPDATE layoff_staging2 t1
join layoff_staging2 t2
	on t1.company = t2.company
SET t1.industry = t2.industry
where t1.industry is null 
and t2.industry is not null;

-- Check data for a specific company
select *
from layoff_staging2
where company = 'Airbnb';

-- 4: Remove unnecessary columns or rows

-- Check again for rows where both layoff metrics are NULL
select *
from layoff_staging2
where total_laid_off is null 
and percentage_laid_off is null;

-- Delete those rows (they don't add value to the dataset)
delete 
from layoff_staging2
where total_laid_off is null 
and percentage_laid_off is null;

-- Drop the 'row_num' column used earlier for removing duplicates
alter table layoff_staging2
drop column row_num;

-- Final check to view cleaned dataset
select *
from layoff_staging2;