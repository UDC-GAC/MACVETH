'''
Filename: utils/intrinsics_translation/filter_uops.py

Created Date: Monday, October 19th 2020, 4:27:57 pm
Author: Marcos Horro

Copyright (c) 2020 Colorado State University
'''


#!/usr/bin/python3
import xml.etree.ElementTree as ET
from .spinner import Spinner
import sys
import pandas as pd
import requests
import time
import pathlib

supported_archs = {
    "sandybridge": "SNB",
    "haswell": "HSW",
    "broadwell": "BDW",
    "skylake": "SKL",
    "skylakex": "SKX",
    "kabylake": "KBL",
    "coffeelake": "CFL",
    "cannonlake": "CNL",
    "icelake": "ICL",
    "zen": "ZEN+",
    "zen2": "ZEN2"}

# inverse map
supported_archs_inv = {v: k for k, v in supported_archs.items()}

# ordered list of ISAs
supported_isas = ["BASE", "MMX", "SSE", "SSE2",
                  "SSE3", "SSSE3", "SSE4", "SSE42", "AVX", "AVX2", "AVX512"]

csv_sep = "|"
MAX_VAL = 1000000
MIN_VAL = 0

instr_file = "instructions.xml"


def is_in_isa(isa, min_isa):
    if min_isa == "all":
        return True
    if isa not in supported_isas:
        return False
    return supported_isas.index(isa) <= supported_isas.index(min_isa)


def check_support(arch, isa):
    if arch not in supported_archs.keys():
        print("Error: unsupported architecture " + arch)
        sys.exit(1)
    if isa == "all":
        return
    if isa not in supported_isas:
        print("Error: unsupported isa " + isa)
        sys.exit(1)


def get_info(instr_node, arch_node):
    string = instr_node.attrib['string']
    uops = 1
    th = 1
    lat = ""
    ports = ""
    for measure_node in arch_node.iter('measurement'):
        uops = measure_node.attrib['uops']
        th = measure_node.attrib['TP']
        if "ports" in measure_node.keys():
            ports = measure_node.attrib['ports']
        elif "ports_same_reg" in measure_node.keys():
            ports = measure_node.attrib['ports_same_reg']
        upperbound = MIN_VAL
        lowerbound = MAX_VAL
        addr_cyc = -1
        mem_cyc = -1
        cycles = -1
        for lat_node in measure_node:
            addr_cyc = -1
            mem_cyc = -1
            cycles = -1
            if "cycles" in lat_node.keys():
                cycles = int(lat_node.attrib['cycles'])
            else:
                if "cycles_addr" in lat_node.keys():
                    addr_cyc = int(lat_node.attrib['cycles_addr'])
                if "cycles_mem" in lat_node.keys():
                    mem_cyc = int(lat_node.attrib['cycles_mem'])

            if cycles != -1 and lowerbound > cycles:
                lowerbound = cycles
            if addr_cyc != -1 and lowerbound > addr_cyc:
                lowerbound = addr_cyc
            if mem_cyc != -1 and lowerbound > mem_cyc:
                lowerbound = mem_cyc

            if cycles != -1 and upperbound < cycles:
                upperbound = cycles
            if addr_cyc != -1 and upperbound < addr_cyc:
                upperbound = addr_cyc
            if mem_cyc != -1 and upperbound < mem_cyc:
                upperbound = mem_cyc
        if lowerbound == MAX_VAL and upperbound == MIN_VAL:
            lowerbound = upperbound
        if lowerbound == upperbound:
            lat = str(lowerbound)
        else:
            l = str(lowerbound)
            u = str(upperbound)
            if addr_cyc != -1 and mem_cyc != -1:
                u = "(" + str(addr_cyc) + "," + str(mem_cyc) + ")"
            lat = "[" + l + ";" + u + "]"
    asm = instr_node.attrib['asm']
    iform = instr_node.attrib['iform']
    return [asm, iform, string, lat, th, uops, ports]


def get_csv(arch, isa, download_file=""):
    check_support(arch, isa)
    cols = ["arch", "ISA", "ASM", "XED_iform",
            "string", "latency", "throughput", "uops", "ports"]
    df = pd.DataFrame(columns=cols)
    f = pathlib.Path(instr_file)
    if f.exists():
        with Spinner("Parsing uops file %s... " % (instr_file), "Parsed uops file properly!"):
            root = ET.parse(instr_file)
            root = root.getroot()
    else:
        url = "https://uops.info/instructions.xml"
        with Spinner("Downloading uops file... ", "UOPS file downloaded properly!"):
            resp = requests.get(url)
            root = ET.fromstring(resp.content)
            if (download_file != ""):
                with open(download_file, 'wb') as file:
                    file.write(resp.content)
    with Spinner("Parsing all instructions in uops file...", "All instructions parsed!"):
        for instr_node in root.iter('instruction'):
            # Future instruction set extensions
            if instr_node.attrib['extension'] in ['CLDEMOTE', 'ENQCMD', 'MCOMMIT', 'MOVDIR', 'PCONFIG', 'RDPRU', 'SERIALIZE', 'SNP', 'TSX_LDTRK', 'WAITPKG', 'WBNOINVD']:
                continue
            if any(x in instr_node.attrib['isa-set'] for x in ['BF16_', 'VP2INTERSECT']):
                continue
            if not is_in_isa(instr_node.attrib['extension'], isa):
                continue
            isa_str = instr_node.attrib['extension']
            found_arch = False
            for arch_node in instr_node.iter('architecture'):
                arch_name = arch_node.attrib['name']
                if arch_name == supported_archs[arch]:
                    found_arch = True
                    break
            found_arch |= (arch == "all")
            if not found_arch:
                continue

            if arch == "all":
                for arch_node in instr_node.iter('architecture'):
                    [asm, iform, string, lat, th, uops, ports] = get_info(
                        instr_node, arch_node)
                    arch = supported_archs_inv[arch_node.attrib['name']]
                    row = [arch, isa_str, asm, iform, string,
                           lat, th, uops, ports]
                    df = df.append(pd.DataFrame(row, columns=cols))
            else:
                [asm, iform, string, lat, th, uops, ports] = get_info(
                    instr_node, arch_node)
                row = [arch, isa_str, asm, iform, string,
                       lat, th, uops, ports]
                df = df.append(pd.DataFrame(
                    [row], columns=cols), ignore_index=True)
    return df


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Need an architecture and ISA")
        exit(1)

    arch = sys.argv[1]
    isa = sys.argv[2]
    download_file = ""
    instr_file = instr_file.split("/")[-1]
    if (len(sys.argv) > 3):
        download_file = sys.argv[3]
    df = get_csv(sys.argv[1], sys.argv[2], download_file)
    csv_file = "%s_%s.csv" % (arch, isa)
    df.to_csv(csv_file, sep=csv_sep)
