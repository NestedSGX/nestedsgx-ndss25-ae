import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List

from common import Variant

parser = argparse.ArgumentParser()
parser.add_argument("--simulation", "-sim", type=str, help="Baseline result (*.res)")
parser.add_argument("--nestedsgx", "-nsgx", type=str, help="NestedSGX result (*.res)")
# parser.add_argument("--sgx_hw", "-sgx", type=str, help="SGX result (*.res)")
args = parser.parse_args()

SERIES = [
    "NUMERIC SORT",
    "STRING SORT",
    "BITFIELD",
    "FP EMULATION",
    "FOURIER",
    "ASSIGNMENT",
    "IDEA",
    "HUFFMAN",
    "NEURAL NET",
    "LU DECOMPOSITION",
]


def load_result(fname: str) -> List[float]:
    if not fname or not os.path.exists(fname):
        return []
    res = []
    with open(fname, "r") as f:
        for line in f.readlines():
            data = list(map(lambda s: s.strip(), line.split(":")))
            if len(data) == 4 and data[1] != "" and (data[0] in SERIES or data[0] == ""):
                res.append(float(data[1]))
    print(res)
    return res


def plot(amd_variants: List[Variant], ylabel: str, figname: str):
    fig, axs = plt.subplots(1, 1, figsize=(6, 4))

    total_width = 0.75
    width = total_width / len(amd_variants)
    x = np.arange(len(SERIES))

    legends = {}
    for variants in [amd_variants]:
        axs.set_axisbelow(True)
        axs.grid(True, axis="y", linestyle="dashed", linewidth=0.5)

        axs.set_yticks([i * 0.2 for i in range(0, 7)])
        axs.set_ylim(top=1.1)
        axs.set_xticks(np.arange(len(SERIES)) + 0.3)
        axs.set_xticklabels(SERIES, rotation=30, ha="right", fontsize=8)
        axs.tick_params(axis="x", length=0)
        axs.tick_params(axis="y", direction="in")

        variants = Variant.normalize(variants[0], variants)
        for j, v in enumerate(variants):
            if len(v.data) == 0:
                continue
            
            if j > 0:
                rects = axs.bar(x + j * width + (width - total_width) / 2,
                            v.data, width, label=v.label, ec = 'k', lw=0.8, color=v.color, hatch='.....')
            else:
                rects = axs.bar(x + j * width + (width - total_width) / 2,
                            v.data, width, label=v.label, ec = 'k', lw=0.8, color=v.color, hatch='//')  
            
            if not v.label in legends:
                legends[v.label] = rects
            
            if j > 0: # not baseline
                for k, rect in enumerate(rects):
                    text = "%.2f" % v.data[k]
                    xy = (rect.get_x() + rect.get_width() / 2, rect.get_height())
                    xytext = (0, 1 - (j - 1) * 12)
                    axs.annotate(text, xy=xy, xytext=xytext, fontsize=8,
                                textcoords="offset points", ha="center", va="bottom")

#    axs.text(3.9, 1.235, "AMD")
    axs.legend(legends.values(), legends.keys(), loc="lower center", bbox_to_anchor=(0.5, 1),
                  ncol=4, columnspacing=1, handletextpad=0.5, borderaxespad=0.2, frameon=False)
    #fig.supylabel(ylabel, x=0.01)
    axs.set_ylabel(ylabel)
    fig.tight_layout()

   # plt.subplots_adjust(hspace=0.6, top=0.93, bottom=0.17, right=0.98, left=0.12)
    plt.savefig(figname)
    plt.show()


if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    print("AMD baseline result: %s" % args.simulation)
    print("AMD NestedSGX result: %s" % args.nestedsgx)

    colors = plt.get_cmap("Blues")
    variants = [
        [
            Variant("Sim mode", load_result(args.simulation), 'white'),
            Variant("NestedSGX", load_result(args.nestedsgx), 'white'),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], "Iterations/sec (normalized)", "nbench.pdf")
