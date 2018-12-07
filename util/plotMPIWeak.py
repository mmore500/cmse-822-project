import seaborn as sns
import pandas as pd
import matplotlib as mpl
from matplotlib import pyplot as plt
from matplotlib.ticker import AutoMinorLocator

sns.set(rc={"lines.linewidth": 5})


data = [
    { 'Num Processors' : 1, 'Elapsed Updates' : 536105 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 533910 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 537994 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 539833 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 540541 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 473432 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 471381 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 473729 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 471516 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 476497 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 238084 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 238075 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 235873 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 240436 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 241715 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 176895 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 176128 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 162292 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 174221 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 169023 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 157577 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 166416 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 169028 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 165068 },
    { 'Num Processors' : 256,  'Elapsed Updates' : 165757 },
]

for d in data:
    d['Seconds per Update'] = 150 / d['Elapsed Updates']

df = pd.DataFrame(data)

plt.plot(
    [1,256],
    [df.loc[df['Num Processors'] == 1]['Seconds per Update'].mean(),
    df.loc[df['Num Processors'] == 1]['Seconds per Update'].mean()],
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

plt.xticks([4,16,64,256],[4,16,64,256])
ax.get_xaxis().set_minor_locator(mpl.ticker.AutoMinorLocator())
ax.grid(b=True, which='minor', color='w', linewidth=0.5)

plt.tight_layout()
plt.savefig('MPIWeak.pdf',figsize=(10,10))
