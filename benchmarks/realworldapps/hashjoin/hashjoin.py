import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple

from common import Variant

SERIES1 = [
    "500,000",
    "600,000",
    "700,000",
    "800,000",
    "900,000",
    "1000,000",
]

YLIM_AMD = 8500
YLIM_INTEL = 15000

parser = argparse.ArgumentParser()
parser.add_argument("--simulation", "-sim", type=str, help="Baseline result (*.res)")
parser.add_argument("--nestedsgx", "-nsgx", type=str, help="NestedSGX result (*.res)")
# parser.add_argument("--sgx_hw", "-sgx", type=str, help="SGX result (*.res)")
args = parser.parse_args()

def load_result(fname: str):
    y_data = []
    with open(fname, 'r') as file:
        y_data = [int(line.strip()) for line in file]
    
    y_part1 = y_data[:len(y_data) // 2]
    y_part2 = y_data[len(y_data) // 2:]
    x_data = [500, 600, 700, 800, 900, 1000]

    return list(zip(x_data, y_part1)), list(zip(x_data, y_part2))

def plot(amd_variants: List[Variant], amd_variants_f: List[Variant],
         xlabel: str, ylabel: str, figname: str):
    fig, axs = plt.subplots(1, 1, figsize=(6, 4))
    ax = axs
    ax.set_axisbelow(True)
    ax.grid(True, axis='y', linestyle='dashed', linewidth=0.5)
    variants = amd_variants
    legends = {}
    fontsize = 11
    aver_slowdown = 0.0

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
            for data, sd in zip(v.data[1:], slowdowns[0].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
        #        text = "%.2f" % sd
        #        fontsize = 8
        #        xytext = (0, -12)
        #        ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
         #                   textcoords="offset points", ha='center', va='bottom')

######## get legend, set color to black
    variants = amd_variants_f
    legends = {}
    slowdowns = Variant.normalize(variants[1], variants)
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color='black', ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines

    ax.set_xlim(left=500, right=1000)
    # ax.set_ylim(top=180, bottom=60)
    ax.text(830, 3000000, "NUM_LOOKUPS = 100", color='green', fontsize=fontsize)
    ax.text(830, 9000000, "NUM_LOOKUPS = 250", color='indianred', fontsize=fontsize)
    #ax.text(5, 90, "Workload F", color='green', fontsize=fontsize)

    ######## F
    variants = amd_variants_f
    slowdowns = Variant.normalize(variants[1], variants)
    for j, v in enumerate(variants):
        print(v)
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines
        if v.label == "NestedSGX" or v.label == "SGX":
            for data, sd in zip(v.data[1:], slowdowns[0].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
        #        text = "%.2f" % sd
        #        fontsize = 8
        #        xytext = (0, -12)
        #        ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
        #                    textcoords="offset points", ha='center', va='bottom')
    print("average slowdown: %s", aver_slowdown/10.0)

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
  #  plt.subplots_adjust(bottom=0.04)
    plt.savefig(figname)
    plt.show()

if __name__ == "__main__":
    __file__ = os.getcwd()
    print("Plotting %s..." % __file__[:-3])

    # bw_baremetal_lookup_250 = [[500, 5864120], [600, 6935142], [700, 8028968], [800, 9690856], [900, 10944795], [1000, 11669584]]
    sim = args.simulation
    nsgx = args.nestedsgx

    bw_sim_lookup_100, bw_sim_lookup_250 = load_result(sim)
    bw_nsgx_lookup_100, bw_nsgx_lookup_250 = load_result(nsgx)
    
    #bw_sim_lookup_250 = [[500, 5997980], [600, 6957838], [700, 8196488], [800, 9253976], [900, 10475207], [1000, 11362629]]
    #bw_nsgx_lookup_250 = [[500, 5717393], [600, 6883975], [700, 8122268], [800, 9063341], [900, 10232093], [1000, 10681615]]

    # bw_baremetal_lookup_100 = [[500, 2417647], [600, 2887102], [700, 3309905], [800, 3790219], [900, 4300297], [1000, 4717994]]
    #bw_sim_lookup_100 = [[500, 2449722], [600, 2855068], [700, 3694711], [800, 3928755], [900, 4329057], [1000, 4789221]]
    #bw_nsgx_lookup_100 = [[500, 2258244], [600, 2824897], [700, 3337129], [800, 3736814], [900, 4281932], [1000, 4774343]]

    #colors = plt.get_cmap("Blues")
    variants = [
        [
#            Variant("Native VM", bw_baremetal_lookup_250, 'indianred', "s-"),
            Variant("Sim mode", bw_sim_lookup_250, 'indianred', "x--"),
            Variant("NestedSGX", bw_nsgx_lookup_250, 'indianred', "^:c"),
        ],
         [
#            Variant("Native VM", bw_baremetal_lookup_100, 'green', "s-"),
            Variant("Sim mode", bw_sim_lookup_100, 'green', "x--"),
            Variant("NestedSGX", bw_nsgx_lookup_100, 'green', "^:c"),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            v.data = np.array(v.data)
            print(v)
        print()

    plot(variants[0], variants[1], "Hash table size (x1000)", "Latency (us)", "hashjoin.pdf")