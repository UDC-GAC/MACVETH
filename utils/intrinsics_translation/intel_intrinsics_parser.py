'''
Filename: utils/intrinsics_translation/intel_intrinsics_parser.py

Created Date: Friday, October 16th 2020, 12:56:14 pm
Author: Marcos Horro

Copyright (c) 2020 Colorado State University
'''

#!/usr/bin/python3
import xml.etree.ElementTree as ET
import sys
import pandas as pd
import requests
import os
from tqdm import tqdm
import pathlib

# table
# =====
# intrinsics_name,ASM,XED_iform,ISA,CPUID[;CPUID],form,RET;OP1[;OP2]

csv_sep = "|"
url = "https://software.intel.com/sites/landingpage/IntrinsicsGuide/files/data-latest.xml"
intrin_file = "intel_intrinsics_info.xml"

# ordered list of ISAs
supported_isas = ["BASE", "MMX", "SSE", "SSE2",
                  "SSE3", "SSSE3", "SSE4", "AVX", "AVX2", "AVX512"]


def is_in_isa(isa, min_isa):
    if min_isa == "all":
        return True
    if isa not in supported_isas:
        return False
    return supported_isas.index(isa) <= supported_isas.index(min_isa)


def retrieve_intrinsics_as_csv(min_isa, download_file=""):
    f = pathlib.Path(intrin_file)
    if f.exists():
        root = ET.parse(intrin_file)
    else:
        resp = requests.get(url)
        root = ET.fromstring(resp.content)
        if (download_file != ""):
            with open(download_file, 'wb') as file:
                file.write(resp.content)
    cols = ["intrinsics", "ASM", "XED_iform",
            "ISA", "CPUID", "form", "ret", "op"]
    df = pd.DataFrame(columns=cols)
    for list_node in root.iter('intrinsics_list'):
        # This approach is ad-hoc: all children in intrinsics_list are intrinsics
        it = len(list_node.getchildren())
        with tqdm(total=it) as pbar:
            for intr_node in list_node.iter("intrinsic"):
                pbar.update(1)
                isa = intr_node.attrib["tech"].replace("-", "")
                if (isa == "SSE4.1") or (isa == "SSE4.2"):
                    isa = "SSE4"
                if not is_in_isa(isa, min_isa):
                    continue
                intrinsics_name = intr_node.attrib["name"]
                for r in intr_node.iter("return"):
                    ret = r.attrib["type"]
                op = ""
                for param in intr_node.iter("parameter"):
                    op += param.attrib["type"] + ","

                if "sequence" in intr_node.keys():
                    asm = xed_iform = "seq"
                else:
                    asm = xed_iform = ""

                for descr in intr_node.iter("description"):
                    if "does not generate any instructions" in descr.text or "zero latency" in descr.text:
                        asm = xed_iform = "none"

                form = ""
                for i in intr_node.iter("instruction"):
                    asm = i.attrib["name"]
                    if "xed" in i.attrib.keys():
                        xed_iform = i.attrib["xed"]
                    else:
                        xed_iform = asm
                    if "form" in i.attrib.keys():
                        form = i.attrib["form"]
                    else:
                        form = ""
                for cpuid in intr_node.iter("CPUID"):
                    cpuids = cpuid.text + ";"
                row = [intrinsics_name, asm, xed_iform,
                       isa, cpuids, form, ret, op]
                df = df.append(pd.DataFrame(
                    [row], columns=cols), ignore_index=True)
    return df


if __name__ == "__main__":
    download_file = ""
    if (len(sys.argv) > 1):
        download_file = sys.argv[1]
    intrin_file = intrin_file.split("/")[-1]
    df = retrieve_intrinsics_as_csv("AVX2", download_file)
    csv_file = "intrinsics.csv"
    df.to_csv(csv_file, sep=csv_sep)
