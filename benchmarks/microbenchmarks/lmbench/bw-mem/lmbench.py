import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List

from common import Variant

SERIES1 = [
    "bw-mem-rd",
    "bw-mem-wr",
    "bw-mem-rdwr",
    "bw-mem-cp",
    "bw-mem-fwr",
    "bw-mem-frd",
    "bw-mem-fcp",
    "bw-mem-bzero",
    "bw-mem-bcopy",
]

SERIES2 = [
    "drand48",
    "lrand48",
    "null",
    "read",
    "write",
    "stat",
    "fstat",
    "open",
]


def plot(amd_variants: List[Variant], intel_variants: List[Variant], ylabel: str, figname: str):
    fig, axs = plt.subplots(1, 1, figsize=(6, 2))

    total_width = 0.75
    width = total_width / len(amd_variants)
    

    legends = {}

    ###### first half
    ax = axs
    variants = amd_variants
    x = np.arange(len(SERIES1))

    ax.set_axisbelow(True)
    ax.grid(True, axis="y", linestyle="dashed", linewidth=0.5)

    #ax.set_yticks([i * 0.2 for i in range(0, 7)])
    ax.set_ylim(top=90000)
    ax.set_xticks(np.arange(len(SERIES1)) + 0.3)
    ax.set_xticklabels(SERIES1, rotation=30, ha="right", fontsize=8)
    ax.tick_params(axis="x", length=0)
    ax.tick_params(axis="y", direction="in")
    ax.set_ylabel("Memory bandwidth (MB/s)", fontsize = 8);

    variants_norm = Variant.normalize(variants[0], variants)
    for j, v in enumerate(variants):
        print(j)
        print(v)
        print(variants_norm[1].data)
        if len(v.data) == 0:
            continue

        if j > 0:
            rects = ax.bar(x + j * width + (width - total_width) / 2,
                        v.data, width, label=v.label, ec = 'k', color=v.color, hatch='.....')
        else:
            rects = ax.bar(x + j * width + (width - total_width) / 2,
                        v.data, width, label=v.label, ec = 'k', color=v.color, hatch='//')
            
        if not v.label in legends:
            legends[v.label] = rects
        if j > 1: # not baseline
            for k, rect in enumerate(rects):
                text = "%.2f" % variants_norm[2].data[k] #v.data[k]
                xy = (rect.get_x() + rect.get_width() / 2, rect.get_height() + 11000)
                xytext = (0, 1 - (j - 1) * 12)
                ax.annotate(text, xy=xy, xytext=xytext, fontsize=8,
                            textcoords="offset points", ha="center", va="bottom")

    #######################

   # axs[0].text(1.9, 1.235, "Lmbench (bw_mem)")
   # axs[1].text(1.9, 1.235, "Lmbench (syscall)")
    axs.legend(legends.values(), legends.keys(), loc="lower center", bbox_to_anchor=(0.5, 1),
                  ncol=4, columnspacing=1, handletextpad=0.5, borderaxespad=0.2, frameon=False)
   # fig.supylabel(ylabel, x=0.01)
    fig.tight_layout()

    #plt.subplots_adjust(hspace=0.6, top=0.93, bottom=0.10, right=0.98, left=0.14)
    plt.subplots_adjust(top=0.86, bottom=0.27)
    fig.align_ylabels(axs)

    plt.savefig(figname)
    plt.show()

def read_data(filename):
    with open(filename, 'r') as file:
        data = [int(line.strip()) for line in file]
    return data

if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    parser = argparse.ArgumentParser(description='parser')
    parser.add_argument('--nestedsgx', '-nsgx', type=str)
    parser.add_argument('--simulation', '-sim', type=str)

    args = parser.parse_args()
    nsgx = args.nestedsgx
    sim = args.simulation

    bw_nsgx = read_data(nsgx)
    bw_sim = read_data(sim)
    print(bw_nsgx)
    print(bw_sim)

    #colors = plt.get_cmap("Blues")
    variants = [
        [
            Variant("Sim mode", bw_sim, 'white'),
            Variant("NestedSGX", bw_nsgx, 'white'),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], variants[0], "normalized", "lmbench.pdf")