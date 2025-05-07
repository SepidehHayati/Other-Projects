# Exploratory Data Avalysis:

-- Step 1: View the cleaned dataset
select *
from layoff_staging2;

-- Step 2: Find the maximum number of layoffs and the maximum percentage laid off in a single entry
select max(total_laid_off), max(percentage_laid_off)
from layoff_staging2;

-- Step 3: Show companies that laid off 100% of their workforce, sorted by the number laid off
select *
from layoff_staging2
where percentage_laid_off = 1
order by total_laid_off desc;

-- Step 4: Total layoffs by company (which companies laid off the most people overall)
select company, sum(total_laid_off)
from layoff_staging2
group by company
order by 2 desc;

-- Step 5: Total layoffs by industry (which industries were hit the hardest)
select industry, sum(total_laid_off)
from layoff_staging2
group by industry
order by 2 desc;

-- Step 6: Total layoffs by country (which countries had the highest layoffs)
select country, sum(total_laid_off)
from layoff_staging2
group by country
order by 2 desc;

-- Step 7: Total layoffs by year (to analyze yearly trends)
select year(`date`), sum(total_laid_off)
from layoff_staging2
group by year(`date`)
order by 1 desc;

-- Step 8: Total layoffs by funding stage (e.g., Series A, Series B, etc.)
select stage, sum(total_laid_off)
from layoff_staging2
group by stage
order by 2 desc;

-- Step 9: Monthly layoffs with rolling total (to observe the trend over time)
with rolling_total as
(
select substring(`date`,1,7) as `month`, sum(total_laid_off) as total_off
from layoff_staging2
where substring(`date`,1,7) is not null
group by `month`
order by 1 asc
)
select `month`, total_off, sum(total_off) over(order by `month`) as rolling_total
from rolling_total;

-- Step 10: Total layoffs per company per year (detailed breakdown)
select company, year(`date`), sum(total_laid_off)
from layoff_staging2
group by company, year(`date`)
order by 3 desc;

-- Step 11: Top 5 companies by layoffs for each year
with company_year(company, years, total_laids_off) as
(
select company, year(`date`), sum(total_laid_off)
from layoff_staging2
group by company, year(`date`)
), company_year_rank as
(
select *, dense_rank() over(partition by years order by total_laids_off desc) as ranking
from company_year
where years is not null
order by years, ranking 
)
select * 
from company_year_rank
where ranking <= 5;





