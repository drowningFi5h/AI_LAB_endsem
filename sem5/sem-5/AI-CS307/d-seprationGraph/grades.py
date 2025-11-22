import pandas as pd

# Read the data from the file into a DataFrame
df = pd.read_csv('data.txt', sep='\t')

# Display the first few rows of the DataFrame
print(df.head())

# Display information about the DataFrame
print(df.info())