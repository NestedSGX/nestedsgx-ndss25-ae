import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List

from common import Variant
import sys

SERIES1 = [
    "seq-read",
    "seq-write",
    "seq-rdwr",
    "rand-read",
    "rand-write",
    "rand-rdwr",
]

parser = argparse.ArgumentParser()
parser.add_argument("--simulation", "-sim", type=str, help="Baseline result (*.res)")
parser.add_argument("--nestedsgx", "-nsgx", type=str, help="NestedSGX result (*.res)")
# parser.add_argument("--sgx_hw", "-sgx", type=str, help="SGX result (*.res)")
args = parser.parse_args()

def load(filename):
    with open(filename, 'r') as file:
        text = file.read()

    bw_values = []
    for line in text.split('\n'):
        parts = line.split('bw=')
        if len(parts) > 1:
            bw = parts[1].split('MiB/s')[0]
            bw_values.append(float(bw))
    return bw_values

def plot(amd_variants: List[Variant], ylabel: str, figname: str):
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
    ax.set_ylim(top=700)
    ax.set_xticks(np.arange(len(SERIES1)) ) # + 0.3
    ax.set_xticklabels(SERIES1, ha="center", fontsize=8) # rotation=30, 
    ax.tick_params(axis="x", length=0)
    ax.tick_params(axis="y", direction="in")
    ax.set_ylabel("Bandwidth (MB/s)", fontsize = 8);

    for j, v in enumerate(variants):
        print(j)
        print(v)
        if len(v.data) == 0:
            continue
        
        
        if j == 1:
            rects = ax.bar(x + j * width + (width - total_width) / 2,
                        v.data[0], width, label=v.label, color='white', ec = 'k', lw=0.8, hatch='.....', bottom = 0)
        else:
            rects = ax.bar(x + j * width + (width - total_width) / 2,
                        v.data[0], width, label=v.label, color='white', ec = 'k', lw=0.8, hatch='//', bottom = 0)

        # if j == 1:
        #     ax.bar(x + j * width + (width - total_width) / 2,
        #                 v.data[1], width, label=v.label, color='white', ec = 'k', lw=0.8, hatch='.....', bottom = v.data[0])
        # else:
        #     ax.bar(x + j * width + (width - total_width) / 2,
        #                 v.data[1], width, label=v.label, color='white', ec = 'k', lw=0.8, hatch='//', bottom = v.data[0])
         
        if not v.label in legends:
            legends[v.label] = rects

    
    #######################

   # axs[0].text(1.9, 1.235, "Lmbench (bw_mem)")
   # axs[1].text(1.9, 1.235, "Lmbench (syscall)")
    axs.legend(legends.values(), legends.keys(), loc="lower center", bbox_to_anchor=(0.5, 1),
                  ncol=4, columnspacing=1, handletextpad=0.5, borderaxespad=0.2, frameon=False)
   # fig.supylabel(ylabel, x=0.01)
    fig.tight_layout()

 #   plt.subplots_adjust(hspace=0.6, top=0.93, bottom=0.17, right=0.98, left=0.1)
    fig.align_ylabels(axs)

    plt.savefig(figname)
    plt.show()


if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    #bw_nsgx = [193, 54, 53.5, 182, 57.2, 58.3]
    # bw_nsgx = [428, 54, 53.5, 381, 57.2, 58.3] # hot-calls
    bw_nsgx_h = [0, 0, 34.6, 0, 0, 38.8]
    # bw_sim = [655, 54.4, 53.6, 625, 43.6, 61.2]
    bw_sim_h = [0, 0, 35.6, 0, 0, 41.8]
    nsgx = args.nestedsgx
    sim = args.simulation

    bw_nsgx = load(nsgx)
    bw_sim = load(sim)
    
    print(bw_nsgx)
    print(bw_sim)
    #colors = plt.get_cmap("Blues")
    variants = [
        [
#            Variant("Native VM", bw_baremetal, 'lightgrey'),
            Variant("Sim mode + Occlum", [bw_sim], 'lightcyan', "x--"),
            Variant("NestedSGX + Occlum", [bw_nsgx], 'indianred', "^:c"),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], "normalized", "fio.pdf")
