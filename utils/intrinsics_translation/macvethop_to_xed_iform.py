'''
Filename: utils/intrinsics_translation/macvethop_to_xed_iform.py

Created Date: Friday, October 23rd 2020, 10:14:23 am
Author: Marcos Horro

Copyright (c) 2020 Colorado State University
'''


mvops_to_asm = {
    "LD": ["MOV", "XMM", "MEM"],
    "ST": ["MOV", "MEM", "XMM"],
    "ADD": ["ADD", "XMM", "XMM", "MEM"],
    "SUB": ["SUB", "XMM", "XMM", "MEM"],
    "MUL": ["MUL", "XMM", "XMM", "MEM"],
    "DIV": ["DIV", "XMM", "XMM", "MEM"]
}

type_to_asmsuffix = {
    "FLOAT": "SS",
    "DOUBLE": "SD",
    "INT": ""
}

type_to_asmpref = {
    "FLOAT": "V",
    "DOUBLE": "V",
    "INT": ""
}

type_to_memsuffix = {
    "FLOAT": "d",
    "DOUBLE": "q",
    "INT": ""
}

type_to_regsuffix = {
    "FLOAT": "dq",
    "DOUBLE": "dq",
    "INT": ""
}

dtypes = ["FLOAT", "DOUBLE"]


def get_xed_iform_for_mvop(isa):
    dict_mvop_xed_iform = {}
    for k in mvops_to_asm.keys():
        for data_type in dtypes:
            mvop = "%s_%s" % (k, data_type)
            asm = type_to_asmpref[data_type] + \
                mvops_to_asm[k][0] + type_to_asmsuffix[data_type]
            pos = 1
            for op in mvops_to_asm[k][1:]:
                if (op == "MEM"):
                    asm += "_" + op + type_to_memsuffix[data_type]
                else:
                    if (pos == len(mvops_to_asm[k])-1):
                        asm += "_" + op + type_to_memsuffix[data_type]
                    else:
                        asm += "_" + op + type_to_regsuffix[data_type]
                pos += 1
            dict_mvop_xed_iform[asm] = mvop
    return dict_mvop_xed_iform
