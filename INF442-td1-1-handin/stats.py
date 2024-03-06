import pandas as pd
pd.set_option('display.max_columns', 100)
pd.set_option('display.max_rows', 100)
pd.set_option('display.precision', 2)

data = pd.read_csv('csv/wines.csv', index_col=0)

print(data)