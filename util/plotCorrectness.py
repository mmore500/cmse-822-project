from tqdm import tqdm
import numpy as np
import pandas as pd
import sys
import colorsys
import matplotlib
import matplotlib.patches as patches
from matplotlib import pyplot as plt

#arguments
# 1 chanel map
# 2 stockpiles

df = None
if "csv" in sys.argv[1]:
    df = pd.read_csv(sys.argv[1], sep=',', header=None)
elif "h5" in sys.argv[1]:
    df = pd.read_hdf(sys.argv[1])

ch = df.values

df = None
if "csv" in sys.argv[1]:
    df = pd.read_csv(sys.argv[2], sep=',', header=None)
elif "h5" in sys.argv[1]:
    df = pd.read_hdf(sys.argv[2])

sp = df.values

HEIGHT = sp.shape[0]
WIDTH = sp.shape[1]

cmat = [[
        (
            0.0 if sp[y][x] > 0 else 1.0,
            1.0 if sp[y][x] >= 0 else 0.0,
            1.0 if sp[y][x] == 0 else 0.0
    ) for y in tqdm(range(WIDTH))] for x in range(HEIGHT)]

# draw cells
plt.imshow(np.swapaxes(np.array(cmat), 1, 0),extent=(0,WIDTH,HEIGHT,0))


ch_pad = np.pad(ch, 1, mode='wrap')

print(ch_pad)

lines = [(
    patches.Polygon(
        [[x-1,y-1], [x,y-1]],
        linewidth=0.5,
        edgecolor=('white' if ch_pad[y-1][x] == ch_pad[y][x] else 'black'),
        alpha=(0.0 if ch_pad[y-1][x] == ch_pad[y][x] else 1.0),
        facecolor=None
        ),
    patches.Polygon(
        np.array([[x-1,y-1], [x-1,y]]),
        linewidth=0.5,
        edgecolor=('white' if ch_pad[y][x-1] == ch_pad[y][x] else 'black'),
        alpha=(0.0 if ch_pad[y][x-1] == ch_pad[y][x] else 1.0),
        facecolor=None
        )
    ) for y in tqdm(range(1,HEIGHT+1)) for x in range(1,WIDTH+1)]

flat_list = [item for sublist in lines for item in sublist]

# draw borders
for poly in flat_list:
    plt.gca().add_patch(poly)

plt.savefig(
        'correctness.pdf'
        , transparent=True
    )
