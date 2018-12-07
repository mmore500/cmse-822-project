import seaborn as sns
import pandas as pd
import matplotlib as mpl
from matplotlib import pyplot as plt

sns.set(rc={"lines.linewidth": 5})

data = [
    { 'Num Processors' : 1, 'Elapsed Updates' : 75503.8 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 70884.2 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 69155.4 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 72362.5 },
    { 'Num Processors' : 1, 'Elapsed Updates' : 71360.2 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 74485.1 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 78514 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 79216.2 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 86521.2 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 84863.4 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 78978.1 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 79975.3 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 79509.7 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 79440.6 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 79899.4 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 219037 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 221846 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 221677 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 218466 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 221168 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 567598 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 579957 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 560896 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 559802 },
    { 'Num Processors' : 256,  'Elapsed Updates' : 575893 },
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

next(plt.gca()._get_lines.prop_cycler)

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
plt.savefig("CharmStrong.pdf",figsize=(10,10))
