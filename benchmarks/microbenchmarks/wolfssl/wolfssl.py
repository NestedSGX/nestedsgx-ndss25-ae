'''
NestedSGX:

wolfCrypt Benchmark (block bytes 1048576, min 1.0 sec each)                                                                              
RNG_HEALTH_TEST_CHECK_SIZE = 128                                                                                                         
sizeof(seedB_data)         = 128                                                                                                         
RNG_HEALTH_TEST_CHECK_SIZE = 128                                                                                                         
sizeof(seedB_data)         = 128                                                                                                         
RNG                        105 MiB took 1.003 seconds,  104.710 MiB/s                              
AES-128-CBC-enc            315 MiB took 1.005 seconds,  313.476 MiB/s
AES-128-CBC-dec            370 MiB took 1.005 seconds,  368.185 MiB/s
AES-192-CBC-enc            275 MiB took 1.014 seconds,  271.183 MiB/s
AES-192-CBC-dec            305 MiB took 1.001 seconds,  304.716 MiB/s
AES-256-CBC-enc            240 MiB took 1.017 seconds,  236.014 MiB/s
AES-256-CBC-dec            265 MiB took 1.005 seconds,  263.710 MiB/s
AES-128-GCM-enc             75 MiB took 1.065 seconds,   70.414 MiB/s
AES-128-GCM-dec             75 MiB took 1.058 seconds,   70.864 MiB/s
AES-192-GCM-enc             70 MiB took 1.029 seconds,   68.030 MiB/s
AES-192-GCM-dec             70 MiB took 1.057 seconds,   66.222 MiB/s
AES-256-GCM-enc             70 MiB took 1.058 seconds,   66.142 MiB/s
AES-256-GCM-dec             70 MiB took 1.055 seconds,   66.359 MiB/s
AES-128-GCM-enc-no_AAD      75 MiB took 1.057 seconds,   70.965 MiB/s
AES-128-GCM-dec-no_AAD      75 MiB took 1.054 seconds,   71.130 MiB/s
AES-192-GCM-enc-no_AAD      70 MiB took 1.020 seconds,   68.638 MiB/s
AES-192-GCM-dec-no_AAD      70 MiB took 1.022 seconds,   68.483 MiB/s
AES-256-GCM-enc-no_AAD      70 MiB took 1.056 seconds,   66.279 MiB/s
AES-256-GCM-dec-no_AAD      70 MiB took 1.055 seconds,   66.328 MiB/s
GMAC Default                97 MiB took 1.010 seconds,   96.076 MiB/s
3DES                        35 MiB took 1.065 seconds,   32.876 MiB/s
MD5                        690 MiB took 1.006 seconds,  686.106 MiB/s
SHA                        745 MiB took 1.005 seconds,  740.991 MiB/s
SHA-256                    255 MiB took 1.016 seconds,  251.005 MiB/s
HMAC-MD5                   685 MiB took 1.003 seconds,  683.244 MiB/s
HMAC-SHA                   740 MiB took 1.002 seconds,  738.217 MiB/s
HMAC-SHA256                250 MiB took 1.004 seconds,  249.093 MiB/s
PBKDF2                      30 KiB took 1.001 seconds,   30.194 KiB/s
RSA     2048   public      8200 ops took 1.004 sec, avg 0.122 ms, 8165.223 ops/sec
RSA     2048  private       200 ops took 1.365 sec, avg 6.823 ms, 146.563 ops/sec
DH      2048  key gen       349 ops took 1.001 sec, avg 2.868 ms, 348.719 ops/sec
DH      2048    agree       400 ops took 1.137 sec, avg 2.841 ms, 351.941 ops/sec
ECC   [      SECP256R1]   256  key gen     11400 ops took 1.003 sec, avg 0.088 ms, 11365.959 ops/sec
ECDHE [      SECP256R1]   256    agree      4200 ops took 1.013 sec, avg 0.241 ms, 4148.124 ops/sec
ECDSA [      SECP256R1]   256     sign      7500 ops took 1.007 sec, avg 0.134 ms, 7447.384 ops/sec
ECDSA [      SECP256R1]   256   verify      3800 ops took 1.014 sec, avg 0.267 ms, 3746.319 ops/sec


wolfCrypt Benchmark (block bytes 1048576, min 1.0 sec each)                                                                              
RNG_HEALTH_TEST_CHECK_SIZE = 128                                                                                                         
sizeof(seedB_data)         = 128                                                                                                         
RNG_HEALTH_TEST_CHECK_SIZE = 128                                                                                                         
sizeof(seedB_data)         = 128                                                                                                         
RNG                        105 MiB took 1.023 seconds,  102.667 MiB/s                              
AES-128-CBC-enc            315 MiB took 1.007 seconds,  312.758 MiB/s
AES-128-CBC-dec            370 MiB took 1.002 seconds,  369.164 MiB/s
AES-192-CBC-enc            270 MiB took 1.001 seconds,  269.798 MiB/s
AES-192-CBC-dec            310 MiB took 1.001 seconds,  309.697 MiB/s
AES-256-CBC-enc            240 MiB took 1.010 seconds,  237.607 MiB/s
AES-256-CBC-dec            270 MiB took 1.012 seconds,  266.819 MiB/s
AES-128-GCM-enc             75 MiB took 1.045 seconds,   71.799 MiB/s
AES-128-GCM-dec             75 MiB took 1.050 seconds,   71.416 MiB/s
AES-192-GCM-enc             70 MiB took 1.011 seconds,   69.212 MiB/s
AES-192-GCM-dec             70 MiB took 1.009 seconds,   69.351 MiB/s
AES-256-GCM-enc             70 MiB took 1.049 seconds,   66.720 MiB/s
AES-256-GCM-dec             70 MiB took 1.048 seconds,   66.798 MiB/s
AES-128-GCM-enc-no_AAD      75 MiB took 1.049 seconds,   71.473 MiB/s
AES-128-GCM-dec-no_AAD      75 MiB took 1.044 seconds,   71.838 MiB/s
AES-192-GCM-enc-no_AAD      70 MiB took 1.010 seconds,   69.327 MiB/s
AES-192-GCM-dec-no_AAD      70 MiB took 1.009 seconds,   69.380 MiB/s
AES-256-GCM-enc-no_AAD      70 MiB took 1.049 seconds,   66.703 MiB/s
AES-256-GCM-dec-no_AAD      70 MiB took 1.048 seconds,   66.764 MiB/s
GMAC Default                97 MiB took 1.005 seconds,   96.536 MiB/s
3DES                        35 MiB took 1.058 seconds,   33.083 MiB/s
MD5                        695 MiB took 1.004 seconds,  692.322 MiB/s
SHA                        750 MiB took 1.004 seconds,  746.955 MiB/s
SHA-256                    255 MiB took 1.010 seconds,  252.477 MiB/s
HMAC-MD5                   695 MiB took 1.006 seconds,  690.653 MiB/s
HMAC-SHA                   740 MiB took 1.002 seconds,  738.753 MiB/s
HMAC-SHA256                255 MiB took 1.010 seconds,  252.507 MiB/s
PBKDF2                      31 KiB took 1.001 seconds,   30.788 KiB/s
RSA     2048   public      8300 ops took 1.011 sec, avg 0.122 ms, 8207.145 ops/sec
RSA     2048  private       200 ops took 1.362 sec, avg 6.809 ms, 146.859 ops/sec
DH      2048  key gen       354 ops took 1.002 sec, avg 2.831 ms, 353.269 ops/sec
DH      2048    agree       400 ops took 1.146 sec, avg 2.865 ms, 349.051 ops/sec
ECC   [      SECP256R1]   256  key gen     11400 ops took 1.004 sec, avg 0.088 ms, 11352.455 ops/sec
ECDHE [      SECP256R1]   256    agree      4200 ops took 1.010 sec, avg 0.240 ms, 4158.684 ops/sec
ECDSA [      SECP256R1]   256     sign      7500 ops took 1.003 sec, avg 0.134 ms, 7475.234 ops/sec
ECDSA [      SECP256R1]   256   verify      3800 ops took 1.002 sec, avg 0.264 ms, 3792.994 ops/sec
'''

import os
import argparse
import numpy as np
import matplotlib.pyplot as plt
from typing import List

from common import Variant

parser = argparse.ArgumentParser()
parser.add_argument("--NestedSGX", "-nsgx", type=str, help="NestedSGX result (*.res)")
parser.add_argument("--Sim", "-sim", type=str, help="Simulation result (*.res)")

args = parser.parse_args()

SERIES = [
    "RNG",
    "AES-128-CBC-enc", 
    "AES-128-CBC-dec", 
    "AES-192-CBC-enc", 
    "AES-192-CBC-dec", 
    "AES-256-CBC-enc", 
    "AES-256-CBC-dec",
    "AES-128-GCM-enc", 
    "AES-128-GCM-dec", 
    "AES-192-GCM-enc", 
    "AES-192-GCM-dec", 
    "AES-256-GCM-enc", 
    "AES-256-GCM-dec",
    "AES-128-GCM-enc-no_AAD", 
    "AES-128-GCM-dec-no_AAD",
    "AES-192-GCM-enc-no_AAD", 
    "AES-192-GCM-dec-no_AAD", 
    "AES-256-GCM-enc-no_AAD", 
    "AES-256-GCM-dec-no_AAD",
    "GMAC Default", 
    "3DES", 
    "MD5", 
    "SHA", 
    "SHA-256", 
    "HMAC-MD5",
    "HMAC-SHA", 
    "HMAC-SHA256", 
    "PBKDF2", 
    "RSA 2048 public", 
    "RSA 2048 private", 
    "DH 2048 key gen", 
    "DH 2048 agree", 
    "ECC [SECP256R1] 256 key gen",
    "ECDHE [SECP256R1] 256 agree",
    "ECDSA [SECP256R1] 256 sign",
    "ECDSA [SECP256R1] 256 verify",
]
"""

"""

def load_result(fname: str) -> List[float]:
    if not fname or not os.path.exists(fname):
        return []
    res = []
    with open(fname, "r") as f:
        for line in f.readlines():
            data = list(map(lambda s: s.strip(), line.split(":")))
            if len(data) == 2 and data[1] != "" and (data[0] in SERIES or data[0] == ""):
                res.append(float(data[1]))
    return res


def plot(amd_variants: List[Variant], ylabel: str, figname: str):
    fig, ax = plt.subplots(1, 1, figsize=(15, 5))
    print(fig)
    print(ax)
    print(ax.get_xbound())
    total_width = 0.75
    width = total_width / len(amd_variants)
    x = np.arange(len(SERIES))
    #intel_variants
    variants = amd_variants
    legends = {}
    ax.set_axisbelow(True)
    ax.grid(True, axis="y", linestyle="dashed", linewidth=0.5)

    ax.set_aspect(20)
    ax.set_yticks([i * 0.2 for i in range(0, 7)])
    ax.set_ylim(bottom=0.6, top=1.05)
    ax.set_xticks(np.arange(len(SERIES)) + 0.3)
    ax.set_xticklabels(SERIES, rotation=30, ha="right", fontsize=8)
    ax.tick_params(axis="x", length=0)
    ax.tick_params(axis="y", direction="in")

    variants = Variant.normalize(variants[0], variants)
    for j, v in enumerate(variants):
        if len(v.data) == 0:
            continue
        print(x + j * width + (width - total_width) / 2)
        rects = ax.bar(x + j * width + (width - total_width) / 2,
                        v.data, width, label=v.label, lw=0.8, edgecolor='k', color=v.color, hatch = v.marker)
        if not v.label in legends:
            legends[v.label] = rects
        if j > 0: # not baseline
            for k, rect in enumerate(rects):
                text = "%.2f" % v.data[k]
                xy = (rect.get_x() + rect.get_width() / 2, rect.get_height())
                xytext = (0, 1 - (j - 1) * 12)
                ax.annotate(text, xy=xy, xytext=xytext, fontsize=8,
                            textcoords="offset points", ha="center", va="bottom")

    ax.legend(legends.values(), legends.keys(), loc="lower center", bbox_to_anchor=(0.5, 1),
                  ncol=4, columnspacing=1, handletextpad=0.5, borderaxespad=0.2, frameon=False)
    fig.supylabel(ylabel, x=0.02)
    fig.tight_layout()

    plt.subplots_adjust(hspace=0.6, top=0.93, bottom=0.17, right=0.98, left=0.06)
    plt.savefig(figname)
    plt.show()


if __name__ == "__main__":
    print("Plotting %s..." % __file__[:-3])

    print("AMD Baseline result: %s" % args.Sim)
    print("AMD NestedSGX result: %s" % args.NestedSGX)

    # colors_1 = plt.get_cmap("Blues")
    # colors_2 = plt.get_cmap("Reds")
    variants = [
        [
            Variant("Sim mode", load_result(args.Sim), "white", '//'), # 
            Variant("NestedSGX", load_result(args.NestedSGX), "white", '.....'),
        ]
    ]
    print()
    for vv in variants:
        for v in vv:
            print(v)
        print()

    plot(variants[0], "Performance (normalized)", "wolfssl.pdf")