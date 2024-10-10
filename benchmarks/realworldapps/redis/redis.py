import os
import csv
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List, Tuple

from common import Variant

parser = argparse.ArgumentParser()
parser.add_argument("--simulation", "-sim", nargs=2, type=str, metavar=("THROUGHPUT", "LATENCY"),
                    help="AMD simulation result (throughput.csv latency.csv)")
parser.add_argument("--nestedsgx", "-nsgx", nargs=2, type=str, metavar=("THROUGHPUT", "LATENCY"),
                    help="AMD nestedsgx result (throughput.csv latency.csv)")
# parser.add_argument("--amd-hu", "-ah", nargs=2, type=str, metavar=("THROUGHPUT", "LATENCY"),
#                     help="AMD HU-Enclave result (throughput.csv latency.csv)")
# parser.add_argument("--intel-base", "-ib", nargs=2, type=str, metavar=("THROUGHPUT", "LATENCY"),
#                     help="Intel baseline result (throughput.csv latency.csv)")
# parser.add_argument("--intel-sgx", "-is", nargs=2, type=str, metavar=("THROUGHPUT", "LATENCY"),
#                     help="Intel SGX result (throughput.csv latency.csv)")
args = parser.parse_args()


def load_result(fnames: List[str]) -> List[Tuple[float, float]]:
    if not fnames:
        return []
    for fname in fnames:
        if not fname or not os.path.exists(fname):
            return []
    res = []
    for fname in fnames:
        with open(fname, "r") as f:
            reader = csv.reader(f)
            next(reader)
            res.append([float(row[-1]) / 1000 for row in reader])  # keep only the median
    res = np.array([(a, b) for a, b in zip(*res)])

    # remove very close points
    l = len(res)
    while l >= 2 and abs(res[l - 1][0] - res[l - 2][0]) < 1 and abs(res[l - 1][1] - res[l - 2][1]) < 0.05:
        l -= 1
    res = res[:l]
    return res


def plot(amd_variants: List[Variant],
         xlabel: str, ylabel: str, figname: str):
    fig, axs = plt.subplots(1, 1, figsize=(6, 4))

    legends = {}
    
    variants = amd_variants
    ax = axs
    ax.set_axisbelow(True)
    ax.grid(True, axis='y', linestyle='dashed', linewidth=0.5)


    #ax.text(4 + (36 - 4) * 0.47, 1.1 * 0.78 + 0.2, 'Intel')
    #ax.set_ylim(top=1.3, bottom=0.2)
    #ax.set_yticks([i * 0.2 for i in range(1, 7)])
   # ax.set_xlim(left=4, right=36)
    #ax.set_xticks(range(5, 36, 5))

   # print(variants)
    for v in variants:
        if len(v.data) == 0:
             continue
        lines, = ax.plot(v.data[:, 0], v.data[:, 1], v.marker, label=v.label, color=v.color, ms=4, lw=1)
        if not v.label in legends:
            legends[v.label] = lines

    labels = list(legends.keys())
    handlers = list(legends.values())
    if len(labels) == 4:
        labels[1], labels[2], labels[3] = labels[3], labels[1], labels[2]
        handlers[1], handlers[2], handlers[3] = handlers[3], handlers[1], handlers[2]
    axs.legend(handlers, labels, loc='lower center', bbox_to_anchor=(0.5, 1),
                  ncol=2, columnspacing=1, handletextpad=0.5, borderaxespad=0, labelspacing=0.3, frameon=False)
    fig.supylabel(ylabel, x=0.01)
    fig.supxlabel(xlabel, x=0.55, y=0)

    plt.tight_layout()
    plt.subplots_adjust(hspace=0.2, top=0.90, bottom=0.12, left=0.16, right=0.97)
    plt.savefig(figname)
    plt.show()


if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    # print("AMD baseline result: %s" % args.amd_base)
    # print("AMD GU-Enclave result: %s" % args.amd_gu)
    # print("AMD HU-Enclave result: %s" % args.amd_hu)
    # print("Intel baseline result: %s" % args.intel_base)
    # print("Intel SGX result: %s" % args.intel_sgx)

    colors = plt.get_cmap("Blues")
    variants = [
        # [
        #     Variant("Baseline", load_result(args.amd_base), colors(0.5), "x--"),
        #     Variant("GU-Enclave", load_result(args.amd_gu), colors(0.75), "s-"),
        #     Variant("HU-Enclave", load_result(args.amd_hu), colors(0.95), "o-"),
        # ],

        [
            Variant("Sim + Occlum", load_result(args.simulation), 'royalblue', "x--"),
            Variant("NestedSGX + Occlum", load_result(args.nestedsgx), 'indianred', "^-"),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], "Throughput (kop/s)", "Latency (ms)", "redis.pdf")

