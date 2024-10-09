import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple

from common import Variant

parser = argparse.ArgumentParser()
parser.add_argument("--base-a", "-ba", type=str,  help="AMD baseline result (throughput.csv)")
parser.add_argument("--nsgx-a", "-na", type=str, help="AMD NestedSGX result (throughput.csv)")
parser.add_argument("--base-d", "-bd", type=str,  help="AMD baseline result workload D (throughput.csv)")
parser.add_argument("--nsgx-d", "-nd", type=str, help="AMD NestedSGX result workload D (throughput.csv)")
parser.add_argument("--base-f", "-bf", type=str,  help="AMD baseline result workload F (throughput.csv)")
parser.add_argument("--nsgx-f", "-nf", type=str, help="AMD NestedSGX result workload F (throughput.csv)")
# parser.add_argument("--native-a", "-an", type=str, help="AMD native result workload F (throughput.csv)")
# parser.add_argument("--native-d", "-bn", type=str, help="AMD native result workload F (throughput.csv)")
# parser.add_argument("--native-f", "-fn", type=str, help="AMD native result workload F (throughput.csv)")

args = parser.parse_args()


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


def plot(amd_variants: List[Variant], amd_variants_d: List[Variant], amd_variants_f: List[Variant],
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
    slowdowns = Variant.normalize(variants[1], variants)
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines
        if v.label == "NestedSGX" or v.label == "SGX":
            for data, sd in zip(v.data[1:], slowdowns[j].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
                text = "%.2f" % sd
        #        fontsize = 8
                xytext = (0, -12)
          #      ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
          #                  textcoords="offset points", ha='center', va='bottom')

######## D
    variants = amd_variants_d
    legends = {}

    slowdowns = Variant.normalize(variants[1], variants)
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines
        if v.label == "NestedSGX" or v.label == "SGX":
            for data, sd in zip(v.data[1:], slowdowns[j].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
                text = "%.2f" % sd
        #        fontsize = 8
                xytext = (0, -12)
         #       ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
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


######## F
    variants = amd_variants_f
    #legends = {}

    
    slowdowns = Variant.normalize(variants[1], variants)
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        #if not v.label in legends:
        #    legends[v.label] = lines
        if v.label == "NestedSGX" or v.label == "SGX":
            for data, sd in zip(v.data[1:], slowdowns[j].data[1:]):
                x, y, sd = data[0], data[1], sd[1]
                print("%s: rec_count=%s, throughput=%s, slowdown=%s" % (v.label, x, y, sd))
                aver_slowdown = aver_slowdown + sd
                text = "%.2f" % sd
        #        fontsize = 8
                xytext = (0, -12)
            #    ax.annotate(text, xy=(x, y), xytext=xytext, fontsize=8,
            #                textcoords="offset points", ha='center', va='bottom')
    print("average slowdown: %s", aver_slowdown/30.0)
    ax.set_xlim(left=0)
    ax.set_ylim(top=150, bottom=60)

    ax.text(5, 135, "Workload A", color='indianred', fontsize=fontsize)
    ax.text(5, 110, "Workload D", color='royalblue', fontsize=fontsize)
    ax.text(5, 90, "Workload F", color='green', fontsize=fontsize)


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
    print("Plotting %s..." % __file__[:-3])

    #colors = plt.get_cmap("Blues")
    variants = [
        [
#            Variant("Native VM", load_result(args.native_a), 'indianred', "s-"),
            Variant("Sim mode", load_result(args.base_a), 'indianred', "x--"),
            Variant("NestedSGX", load_result(args.nsgx_a), 'indianred', "^:c"),
        ],
        [
#            Variant("Native VM", load_result(args.native_d), 'royalblue', "s-"),
            Variant("Sim mode", load_result(args.base_d), 'royalblue', "x--"),
            Variant("NestedSGX", load_result(args.nsgx_d), 'royalblue', "^:c"),
        ],
        [
#            Variant("Native VM", load_result(args.native_f), 'green', "s-"),
            Variant("Sim mode", load_result(args.base_f), 'green', "x--"),
            Variant("NestedSGX", load_result(args.nsgx_f), 'green', "^:c"),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], variants[1], variants[2], "Number of records (x1000)", "Throughput (kop/s)", "sqlite.pdf")