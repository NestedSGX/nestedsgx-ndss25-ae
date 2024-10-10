import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple

from common import Variant

SERIES1 = [
    "1",
    "2",
    "5",
    "10",
    "20",
    "30",
    "40",
    "50",
]

YLIM_AMD = 8500
YLIM_INTEL = 15000

def load_result(fname: str) -> List[Tuple[float, float]]:
    if not fname or not os.path.exists(fname):
        return []
    res = []
    with open(fname, "r") as f:
        reader = csv.reader(f)
        next(reader)
        # keep only the bufsize and the median
        res = [(float(row[0]) / 1000, float(row[-1])) for row in reader]
    res = np.array(res)[1:]  # skip 50K
    return res

def plot(amd_variants: List[Variant], xlabel: str, ylabel: str, figname: str):
    fig, axs = plt.subplots(1, 1, figsize=(6, 4))
    ax = axs
    ax.set_axisbelow(True)
    ax.grid(True, axis='y', linestyle='dashed', linewidth=0.5)
    variants = amd_variants
    legends = {}
    fontsize = 11
    aver_slowdown = 0.0

    ######## get legend, set color to black
    variants = amd_variants
    legends = {}
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines

######## A
    slowdowns = Variant.normalize(variants[0], variants)
    print("slow downs: ")
    print(slowdowns)
    for j, v in enumerate(variants):
        print(v)
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines
        if v.label == "NestedSGX" or v.label == "SGX":
            for data, sd in zip(v.data[1:], slowdowns[1].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
                text = "%.2f" % sd
        #        fontsize = 8
                xytext = (0, -12)
                ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
                            textcoords="offset points", ha='center', va='bottom')



    #ax.set_xlim(left=500, right=1000)
    # ax.set_ylim(top=180, bottom=60)
    #ax.text(830, 3000000, "NUM_LOOKUPS = 100", color='green', fontsize=fontsize)
    #ax.text(830, 9000000, "NUM_LOOKUPS = 250", color='indianred', fontsize=fontsize)
    #ax.text(5, 90, "Workload F", color='green', fontsize=fontsize)

    print("average slowdown: %s", aver_slowdown/8.0)

    labels = list(legends.keys())
    handlers = list(legends.values())
    if len(labels) >= 4:
        labels[1], labels[2], labels[3] = labels[3], labels[1], labels[2]
        handlers[1], handlers[2], handlers[3] = handlers[3], handlers[1], handlers[2]
    axs.legend(handlers, labels, loc='lower center', bbox_to_anchor=(0.5, 1),
                  ncol=3, columnspacing=1, handletextpad=0.5, borderaxespad=0, labelspacing=0.3, frameon=False)
    fig.supylabel(ylabel, x=0.01)
    fig.supxlabel(xlabel, x=0.55, y=0.05)

    plt.tight_layout()
  #  plt.subplots_adjust(hspace=0.2, top=0.70, bottom=0.11, left=0.14, right=0.1)
    plt.savefig(figname)
    plt.show()

if __name__ == "__main__":
    __file__ = os.getcwd()
    print("Plotting %s..." % __file__[:-3])

    bw_baremetal_lookup_250 = [[1, 843], [2, 844], [5, 880], [10, 885], [20, 905], [30, 865], [40, 878], [50, 885]]
    bw_sim_lookup_250 = [[1, 525], [2, 513], [5, 537], [10, 641], [20, 598], [30, 671], [40, 638], [50, 681]]
    bw_nsgx_lookup_250 = [[1, 522], [2, 482], [5, 510], [10, 498], [20, 510], [30, 513], [40, 517], [50, 510]]

    #colors = plt.get_cmap("Blues")
    variants = [
        [
#            Variant("Native VM", bw_baremetal_lookup_250, 'black', "s-"),
            Variant("Sim mode", bw_sim_lookup_250, 'royalblue', "x--"),  # indianred
            Variant("NestedSGX", bw_nsgx_lookup_250, 'indianred', "^:c"),
        ]
    
    ]
    print()
    for vv in variants:
        for v in vv:
            v.data = np.array(v.data)
            print(v)
        print()

    plot(variants[0], "Number of requesting threads", "Throughput", "tls.pdf")