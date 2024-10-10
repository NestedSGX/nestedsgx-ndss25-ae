import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple

from common import Variant

parser = argparse.ArgumentParser()
parser.add_argument("--tlsbase", "-tb", type=str,  help="tls baseline result")
parser.add_argument("--tlsnested", "-tn", type=str, help="tls Nested result")
args = parser.parse_args()


YLIM_AMD = 8500
YLIM_INTEL = 15000

def load_result(fname: str) -> List[Tuple[int, int]]:
    if not fname or not os.path.exists(fname):
        return []
    res = []
    with open(fname, "r") as f:
        for line in f.readlines():
            data = list(map(lambda s: s.strip(), line.split(":")))
            res.append([int(data[0]), int(data[1])])
    ret = np.array(res)
    return ret

def plot(amd_variants: List[Variant],
         xlabel: str, ylabel: str, figname: str):
    fig, ax = plt.subplots(1, 1, figsize=(6, 4))
    #plt.show()
    legends = {}
    variants = amd_variants
    ax.set_axisbelow(True)
    ax.grid(True, axis='y', linestyle='dashed', linewidth=0.5)

    ax.set_ylim(top=750, bottom=600)
    ax.set_yticks([i * 25 for i in range(24, 30)])
    ax.set_xlim(left=0, right=52)
    ax.set_xticks([i * 10 for i in range(1, 6)])
    ax.set_xticklabels([i * 10 for i in range(1, 6)])
    ax.set_yticklabels([i * 25 for i in range(24, 30)])
    
    for v in variants:
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines
    labels = list(legends.keys())
    handlers = list(legends.values())

    slowdowns = Variant.normalize(variants[0], variants)
    for data, sd in zip(variants[1].data, slowdowns[1].data):
        x, y, sd = data[0], data[1], sd[1]
        print("%s: bufsize=%s, throughput=%s, slowdown=%s" % (variants[1].label, x, y, sd))
        text = "%.2f" % sd
        fontsize = 8
        xytext = (0, 2)
        #ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=fontsize,
                    #textcoords="offset points", ha='center', va='bottom')

    ax.legend(handlers, labels, loc='lower center', bbox_to_anchor=(0.5, 1),
                  ncol=2, columnspacing=1, handletextpad=0.5, borderaxespad=0, labelspacing=0.3, frameon=False)
    fig.supylabel(ylabel, x=0.02)
    fig.supxlabel(xlabel, x=0.50, y=0.01)
    #plt.subplots_adjust(left=0.01, right=1.0) #right=1.0
    plt.tight_layout()
    # plt.subplots_adjust(hspace=0.2, top=0.90, bottom=0.12, left=0.16, right=0.97)
    plt.savefig(figname)
    plt.show()

if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    print("AMD baseline result: %s" % args.tlsbase)
    print("AMD NestedSGX result: %s" % args.tlsnested)

    colors_1 = plt.get_cmap("Blues")
    colors_2 = plt.get_cmap("Reds")
    variants = [
        [
            Variant("Sim mode", load_result(args.tlsbase), colors_1(0.8), "x--"),
            Variant("NestedSGX", load_result(args.tlsnested), colors_2(0.7), "^-"),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], "Number of requesting threads", "Throughput (handshakes/s)", "tls-final.pdf")