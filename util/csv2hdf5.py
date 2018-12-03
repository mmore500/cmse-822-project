# adapted from https://stackoverflow.com/questions/27203161/convert-large-csv-to-hdf5

import numpy as np
import pandas as pd
import sys

filename = '/tmp/test.h5'

df = pd.read_csv(sys.argv[1]+".csv",index_col=False,header=None)
print(df)

# Save to HDF5
df.to_hdf(sys.argv[1]+".h5", 'data', mode='w', format='fixed')
del df    # allow df to be garbage collected
