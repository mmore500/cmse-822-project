import seaborn as sns
import pandas as pd
import matplotlib as mpl
from matplotlib import pyplot as plt

sns.set(rc={"lines.linewidth": 5})

data = [
    { 'Num Processors' : 1, 'Elapsed Updates' : 21809 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 22669 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 22631 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 22643 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 22618 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 87360 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 86476 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 85956 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 85903 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 86188 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 368035 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 367723 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 362917 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 367880 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 365513 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 1180370 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 1128421 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 1118295 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 1115493 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 1060351 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 1370275 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 1265382 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 1200989 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 1366065 },
    { 'Num Processors' : 256,  'Elapsed Updates' : 1141848 },
]

for d in data:
    d['Seconds per Update'] = 150 / d['Elapsed Updates']

df = pd.DataFrame(data)

plt.plot(
    [1,256],
    [df.loc[df['Num Processors'] == 1]['Seconds per Update'].mean(),
    df.loc[df['Num Processors'] == 1]['Seconds per Update'].mean()/256],
    linestyle='dashed',
    color='black',
    linewidth=2
    )


ax = sns.lineplot(
        x="Num Processors",
        y="Seconds per Update",
        markers=True,
        dashes=False,
        data=df,
        marker='o',
        legend=False,
        ci='sd',
        markersize=10
    )

ax.set_xscale("log")
ax.set_yscale("log")

plt.xticks([1,4,16,64,256],[1,4,16,64,256])
ax.get_xaxis().set_minor_locator(mpl.ticker.AutoMinorLocator())
ax.grid(b=True, which='minor', color='w', linewidth=0.5)

plt.tight_layout()
plt.savefig('MPIStrong.pdf',figsize=(10,10))
