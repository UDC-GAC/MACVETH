'''
Filename: utils/merge_data.py

Created Date: Monday, October 19th 2020, 4:30:36 pm
Author: Marcos Horro

Copyright (c) 2020 Colorado State University
'''

#!/env/python3

from intrinsics_translation.spinner import Spinner
import intrinsics_translation.filter_uops as uops
import intrinsics_translation.intel_intrinsics_parser as intrin
import intrinsics_translation.macvethop_to_xed_iform as mvop
import pandas as pd
import numpy as np
import sys
import os


csv_sep = "|"

pref_to_width = {
    "": "XMMdq",
    "256": "YMMqq",
    "512": "ZMM"
}

pref_to_width_int = {
    "": 128,
    "256": 256,
    "512": 512
}

suff_to_dt = {
    "epi8": "B",
    "pi8": "B",
    "epi16": "W",
    "pi16": "W",
    "epi32": "D",
    "pi32": "D",
    "epi64": "Q",
    "epi64x": "Q",
    "ss": "SS",
    "ps": "SS",
    "ps1": "SS",
    "sd": "SD",
    "pd": "SD",
    "pd1": "SD"
}

suff_to_dt_int = {
    "epi8": 8,
    "pi8": 8,
    "epi16": 16,
    "pi16": 16,
    "epi32": 32,
    "pi32": 32,
    "epi64": 64,
    "epi64x": 64,
    "ss": 32,
    "ps": 32,
    "ps1": 32,
    "sd": 64,
    "pd": 64,
    "pd1": 64
}

# cols = ["intrinsics", "ASM_x", "XED_iform",
#         "ISA_x", "CPUID", "form", "ret", "op", "arch",
#         "string", "latency", "throughput", "uops", "ports"]

cols = ["MVOP", "intrinsics", "ASM_x", "XED_iform",
        "ISA_x", "CPUID", "latency", "throughput", "uops", "ports"]


def get_mem_op(dt):
    if dt != "SD" and dt != "SS":
        mem = "MEM" + dt.lower()
    elif dt == "SD":
        mem = "MEMq"
    else:
        mem = "MEMd"
    return mem


def get_xed_iform_from_asm(asm, dt, width, mem):
    if "VPINSR" in asm or "INSERT" in asm:
        return "%s%s_%s_%s_%s_IMMb" % (asm, dt, width, width, mem)
    return "%s%s_%s_%s" % (asm, dt, width, mem)


def get_xed_iform_from_intrin(intrin, asm):
    tok = intrin.split("_")
    width = pref_to_width[tok[1][2:]]
    dt = suff_to_dt[tok[3]]
    mem = get_mem_op(dt)
    xed_iform = get_xed_iform_from_asm(asm, dt, width, mem)
    return xed_iform


def set1_to_asm(s):
    return [get_xed_iform_from_intrin(s, "VBROADCAST")]


def set_to_asm(s):
    tok = s.split("_")
    width = pref_to_width_int[tok[1][2:]]
    dt = suff_to_dt_int[tok[3]]
    asm = "VMOV"
    s = s.replace("256", "")
    if dt > 16:
        tmp = get_xed_iform_from_intrin(s, asm)
    else:
        asm = "VPINSR"
        tmp = get_xed_iform_from_intrin(s, asm)
    if dt != "ss" and dt != "sd":
        l = [tmp, tmp]
    else:
        return [tmp]
    niter = (int(width/dt)) - 2
    if tok[3] == "ps":
        for vin in range(niter):  # for PS
            l += [get_xed_iform_from_intrin(s, "VINSERT")]
    elif tok[3] == "pd":
        l += [get_xed_iform_from_intrin(s, "VMOVH"),
              get_xed_iform_from_intrin(s, "VMOVH")]
    else:
        for vin in range(niter):  # for PS
            l += [get_xed_iform_from_intrin(s, asm)]
    return l


def find_xed_iform_and_replace(tmp, l):
    for (i, xed_list) in l:
        tmp.loc[tmp.intrinsics == i, "XED_iform"] = ""
        for xed in xed_list:
            tmp.loc[tmp.intrinsics == i, "XED_iform"] += xed + ";"
        tmp.loc[tmp.intrinsics == i,
                "XED_iform"] = tmp.loc[tmp.intrinsics == i, "XED_iform"].values[0][:-1]
    return tmp


def fill_mvops(isa, df):
    tmp = df.copy()
    tmp.loc[:, "MVOP"] = "NA"
    d = mvop.get_xed_iform_for_mvop(isa)
    tmp["MVOP"] = tmp.loc[:, "XED_iform"].apply(
        lambda x: d.get(x) if x in d.keys() else "NA")

    return tmp


def fill_missing_intrinsics(df):
    tmp = df.copy()
    mask_seq = (df.intrinsics != "NA") & (df.ASM_x == "seq")
    # set1
    mask_set1 = ((df.intrinsics.str.contains("set1")
                  ) | (df.intrinsics.str.contains("set.*p?1$")))
    list_set1_xed_iform = []
    for set1 in tmp.loc[mask_seq & mask_set1, "intrinsics"]:
        list_set1_xed_iform += [(set1, set1_to_asm(set1))]
    tmp = find_xed_iform_and_replace(tmp, list_set1_xed_iform)

    # set
    mask_set = df.intrinsics.str.contains("set[^1]*\_[^1]*")
    list_set_xed_iform = []
    for set_ins in tmp.loc[mask_seq & mask_set, "intrinsics"]:
        list_set_xed_iform += [(set_ins, set_to_asm(set_ins))]
    tmp = find_xed_iform_and_replace(tmp, list_set_xed_iform)
    return tmp


def generate_macveth_file(intrinsics_df, arch_df):
    # cleaning data
    df = intrinsics_df.merge(arch_df, on=["XED_iform"], how="outer")
    df.loc[df['ASM_x'].isnull(), 'ASM_x'] = df['ASM_y']
    df.drop("ASM_y", axis=1, inplace=True)
    df = df.replace("", "NA").replace(np.NaN, "NA")
    # fixing sets and set1
    df = fill_missing_intrinsics(df)
    return df


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('-a', '--arch',
                        action="store", dest="arch",
                        help="target architecture", default="broadwell")
    parser.add_argument('-i', '--isa',
                        action="store", dest="isa",
                        help="target ISA", default="AVX2")
    parser.add_argument('-o', '--output',
                        action="store", dest="csv_file",
                        help="name of the output file", default="macveth_output_arch_isa")
    args = parser.parse_args()
    arch = args.arch
    isa = args.isa.upper()
    csv_file = args.csv_file
    print("Retrieving all info from intrinsics for %s using %s ISA(s)" % (arch, isa))
    intrinsics_df = intrin.retrieve_intrinsics_as_csv(isa,
                                                      "intel_intrinsics_info.xml")
    print("Getting architectural latencies and so for " +
          arch + " with ISA " + isa + "...")
    arch_df = uops.get_csv(arch, isa, "instructions.xml")
    with Spinner("Generating MACVETH file... ", "MACVETH file generated properly"):
        df = generate_macveth_file(intrinsics_df, arch_df)
        df = fill_mvops(isa, df)
    df = df[cols]
    df.to_csv(csv_file, sep=csv_sep, index=False, header=False)

    if (isa != "all"):
        if (arch == "all"):
            arch = "default"
        arch = arch.capitalize()
        with Spinner("Copying file to MACVETH sources...", "Installed!"):
            os.system("cp %s ../src/Vectorization/SIMD/ISA/%s/%s" %
                      (csv_file, isa, arch))
