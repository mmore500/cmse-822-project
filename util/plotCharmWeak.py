import seaborn as sns
import pandas as pd
import matplotlib as mpl
from matplotlib import pyplot as plt


sns.set(rc={"lines.linewidth": 5})

data = [
    { 'Num Processors' : 4, 'Elapsed Updates' : 3.66711e+06 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 3.32939e+06 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 3.61596e+06 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 3.50188e+06 },
    { 'Num Processors' : 4, 'Elapsed Updates' : 3.57392e+06 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 2.56541e+06 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 2.37968e+06 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 2.55071e+06 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 2.32547e+06 },
    { 'Num Processors' : 16, 'Elapsed Updates' : 2.64267e+06 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 833929 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 843235 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 771975 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 821814 },
    { 'Num Processors' : 64, 'Elapsed Updates' : 783164 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 567598 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 579957 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 560896 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 559802 },
    { 'Num Processors' : 256, 'Elapsed Updates' : 575893 },
]

for d in data:
    d['Seconds per Update'] = 150 / d['Elapsed Updates']

df = pd.DataFrame(data)

next(plt.gca()._get_lines.prop_cycler)

plt.plot(
    [4,256],
    [df.loc[df['Num Processors'] == 4]['Seconds per Update'].mean(),
    df.loc[df['Num Processors'] == 4]['Seconds per Update'].mean()],
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
plt.savefig("CharmWeak.pdf",figsize=(10,10))
