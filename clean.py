import pandas as pd
import numpy as np

data = pd.read_csv('data/oldenburg_road-net.txt', sep=" ", header=None)
data.columns = ["row", "source", "dest", "weight"]

print(data.head(20))

del data['row']

print(data.head(20))

np.savetxt('data/oldenburg.txt', data.values, fmt='%g')