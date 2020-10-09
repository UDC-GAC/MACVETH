from __future__ import print_function

import sys
import re
import ipdb
from math import ceil

reg_lbyte = ["%al", "%bl", "%cl", "%dl"]
reg_byte = ["%r8b", "%r9b", "%r10b", "%r11b", "%r12b",
            "%r13b", "%r14b", "%r15b", "%sil", "%dil"]
reg_double = ["%r8d", "%r9d", "%r10d", "%r11d",
              "%r12d", "%r13d", "%r14d", "%r15d"]
reg_numbered = ["%r8"]
reg_32 = ["%rdi", "%rdx", "%rsi", "%rbp", "%rax", "%rip", "%rbx",
          "%rcx", "%eax", "%ecx", "%edx", "%ebx", "%esi", "%edi"]
reg_64 = ["%r9", "%r10", "%r11", "%r12", "%r13", "%r14", "%r15", "%rsp"]
reg_sse = ["%xmm0", "%xmm1", "%xmm2", "%xmm3", "%xmm4", "%xmm5", "%xmm6", "%xmm7", "%xmm8", "%xmm9", "%xmm10", "%xmm11", "%xmm12", "%xmm13", "%xmm14", "%xmm15",
           "%xmm16", "%xmm17", "%xmm18", "%xmm19", "%xmm20", "%xmm21", "%xmm22", "%xmm23", "%xmm24", "%xmm25", "%xmm26", "%xmm27", "%xmm28", "%xmm29", "%xmm30", "%xmm31"]
reg_avx = ["%ymm0", "%ymm1", "%ymm2", "%ymm3", "%ymm4", "%ymm5", "%ymm6", "%ymm7",
           "%ymm8", "%ymm9", "%ymm10", "%ymm11", "%ymm12", "%ymm13", "%ymm14", "%ymm15"]
reg_avx512 = ["%zmm26", "%zmm29"]


def find_noprefetch_region_labels(path):
    f = open(path, "r")

    labels = []
    for l in f:
        if l.strip() == "":
            continue
        words = l.strip().split()

        if words[0] in ["jb", "ja", "je", "jne", "jae", "jmp", "jl", "js", "jbe", "jg", "jle"]:
            labels.append(words[1])

    return labels


def asm_prefetch(path, outf=sys.stdout):

    noprefetch_region_labels = find_noprefetch_region_labels(path)
    f = open(path, "r")

    in_fragment = False
    offset = 0
    global_addr = 0
    label_addr = dict()
    jump_idx = dict()
    alignment_idx = dict()
    current_fragment_start = None
    in_hierarchical_cookie = False
    offsets = []

    for l in f:
        offsets.append(offset)
        if l.strip().startswith("#"):
            continue
        if not in_fragment:
            if l.strip().startswith("kernel_spmv_fragment_"):
                in_fragment = True
                current_fragment_start = l
                global_addr += offset
                offset = 0
            continue

        words = l.strip().split()

        if words[0].startswith(".cfi_endproc"):
            in_fragment = False
            continue

        if words[0].startswith(".align"):
            # This inserts a NOP to align the next instruction to a given boundary
            m = re.match("(.+),(.+)", words[1])
            alignment_factor = int(m.group(1))
            offset = int(ceil(float(offset)/alignment_factor)
                         * alignment_factor)
            alignment_idx[len(offsets)] = alignment_factor
            continue

        if words[0].startswith(".byte"):
            offset += 1
            continue

        if words[0].startswith("."):
            label = words[0][:-1]
            target_addr = global_addr + offset
            label_addr[label] = target_addr

            # Either forward or backwards jump.
            # FIXME: this needs to take into account multiple nested labels/jumps
            if words[0][:-1] in noprefetch_region_labels:
                if words[0][:-1] not in jump_idx:
                    # If the label comes before the jump (backwards), then we need to start a noprefetch region
                    in_hierarchical_cookie = True
                else:
                    # If the label comes after the jump (forward jump), then we need to end a noprefetch region
                    in_hierarchical_cookie = False

                    # Also, we need to check the size of the jump and to fix offsets if necessary
                    for idx in jump_idx[label]:
                        jump_distance = target_addr - \
                            global_addr - offsets[idx]
                        if jump_distance < -128 or jump_distance >= 128:
                            # Fix all offsets between the jump and the current position
                            delta = 4
                            for x in xrange(idx, len(offsets)):
                                if delta == 0:
                                    break
                                if x in alignment_idx:
                                    # If this was an align: realign and recompute all the following
                                    new_offset = int(
                                        ceil(float(offsets[x-1])/alignment_idx[x]) * alignment_idx[x])
                                    delta = new_offset - offsets[x]
                                    if delta == 0:
                                        break
#                                    for y in xrange( x, len(offsets) ):
#                                        offsets[y] += delta
                                offsets[x] += delta
                            offset += delta  # And add +4 to the current offset
            continue

        if words[0] in "decq":
            if words[1] in reg_numbered or words[1] in reg_64 or words[1] in reg_32:
                offset += 3
                continue

        if words[0] in "negl":
            if words[1] in reg_32:
                offset += 2
                continue
            if words[1] in reg_double:
                offset += 3
                continue

        if words[0] in "negq":
            if words[1] in reg_32 or words[1] in reg_64 or words[1] in reg_numbered:
                offset += 3
                continue

        if words[0] in "testl":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 2
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_double:
                    offset += 3
                    continue

            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_32:
                    offset += 6
                    continue
                if words[2] in reg_double:
                    offset += 7
                    continue

        if words[0] in "testb":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_byte or words[2] in reg_lbyte:
                    offset += 3
                    continue
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue

        if words[0] in "imull":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2][:-1] in reg_32:
                    if words[3] in reg_32:
                        offset += 3
                        continue
                    if words[3] in reg_double:
                        offset += 4
                        continue

                if words[2][:-1] in reg_double:
                    if words[3] in reg_32 or words[3] in reg_double:
                        offset += 4
                        continue

                m = re.match("(.+)?\((.+)\)", words[2][:-1])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 4
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 5
                            continue
                        if words[3] in reg_double:
                            offset += 6
                            continue
                else:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 8
                            continue
                        if words[3] in reg_double:
                            offset += 9
                            continue
            else:
                if words[2][:-1] in reg_32:
                    if words[3] in reg_32:
                        offset += 6
                        continue
                    if words[3] in reg_double:
                        offset += 7
                        continue
                if words[2][:-1] in reg_double:
                    if words[3] in reg_32 or words[3] in reg_double:
                        offset += 7
                        continue
                m = re.match("(.+)?\((.+)\)", words[2][:-1])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 7
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 8
                            continue
                        if words[3] in reg_double:
                            offset += 9
                            continue
                else:
                    if m.group(2) in reg_64:
                        if words[3] in reg_32:
                            offset += 11
                            continue

        if words[0] in "imulq":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2][:-1] in reg_32:
                    if words[3] in reg_64 or words[3] in reg_numbered or words[3] in reg_32:
                        offset += 4
                        continue
                if words[2][:-1] in reg_numbered:
                    if words[3] in reg_numbered or words[3] in reg_64:
                        offset += 4
                        continue
                if words[2][:-1] in reg_64:
                    if words[3] in reg_64:
                        offset += 4
                        continue
            else:
                if words[2][:-1] in reg_32:
                    if words[3] in reg_32 or words[3] in reg_64:
                        offset += 7
                        continue
                if words[2][:-1] in reg_numbered:
                    if words[3] in reg_64:
                        offset += 7
                        continue
                if words[2][:-1] in reg_64:
                    if words[3] in reg_64:
                        offset += 7
                        continue

        if words[0] in "addb":
            if words[1][:-1] in reg_lbyte or words[1][:-1] in reg_byte:
                if words[2] in reg_byte or words[2] in reg_32 or words[2] in reg_lbyte:
                    offset += 3
                    continue

            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_lbyte:
                    offset += 3
                    continue
                if words[2] in reg_byte:
                    offset += 4
                    continue

        if words[0] in ["jb", "je", "jne", "ja", "jae", "jmp", "jl", "js", "jbe", "jg", "jle"]:
            label = words[1]
            try:
                target_addr = label_addr[label]
            except KeyError:
                # Assume by default that jump will be small (2 bytes offset)
                target_addr = global_addr + offset

            jump_distance = target_addr - global_addr - offset
            if jump_distance >= -126 and jump_distance < 120:
                # FIXME: heuristically, jumps >= 120 bytes or < -126 are generated using the long version. I do not know why.
                offset += 2
            else:
                offset += 6

            if words[1] in noprefetch_region_labels:
                if label in label_addr:  # Backwards jump
                    in_hierarchical_cookie = False
                else:  # Forward jump
                    in_hierarchical_cookie = True

            # Add index of this jump
            if not label in jump_idx:
                jump_idx[label] = [len(offsets)]
            else:
                jump_idx[label].append(len(offsets))
            continue

        if words[0] in "movzbl":
            if words[1][:-1] in reg_lbyte:
                if words[2] in reg_32:
                    offset += 3
                    continue
                if words[2] in reg_double:
                    offset += 4
                    continue
            if words[1][:-1] in reg_byte:
                if words[2] in reg_32 or words[2] in reg_double:
                    offset += 4
                    continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                    if words[2] in reg_double:
                        offset += 5
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 5
                        continue
                    if words[2] in reg_double:
                        offset += 6
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 7
                        continue
                    if words[2] in reg_double:
                        offset += 8
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 8
                        continue
                    if words[2] in reg_double:
                        offset += 9
                        continue

        if words[0] in ["cmovg", "cmovl", "cmovge", "cmovs", "cmovle", "cmovns"]:
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 3
                    continue
                if words[2] in reg_double:
                    offset += 4
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_32 or words[2] in reg_double:
                    offset += 4
                    continue

        if words[0] in "decl":
            if words[1] in reg_32:
                offset += 2
                continue
            if words[1] in reg_double:
                offset += 3
                continue

            m = re.match("(.+)?\((.+)\)", words[1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    offset += 3
                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    offset += 3
                    continue
                if m.group(2) in reg_64:
                    offset += 4
                    continue
            else:
                if m.group(2) in reg_32:
                    offset += 6
                    continue
                if m.group(2) in reg_64:
                    offset += 7
                    continue

        if words[0] in ["xorl", "xorb"]:
            if words[1][:-1] in reg_32 or words[1][:-1] in reg_lbyte:
                if words[2] in reg_32 or words[1][:-1] in reg_lbyte:
                    offset += 2
                    continue
            if words[1][:-1] in reg_double or words[1][:-1] in reg_byte:
                if words[1][:-1] in reg_double or words[1][:-1] in reg_byte:
                    offset += 3
                    continue

        if words[0] in ["pushq", "popq"]:
            if words[1] in reg_32:
                offset += 1
                continue
            if words[1] in reg_64:
                offset += 2
                continue

        if words[0] == "movl":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 2
                    continue
                if words[2] in reg_double:
                    offset += 3
                    continue
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 3
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 3
                        continue
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 6
                        continue
                    if m.group(2) in reg_64:
                        offset += 7
                        continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_double or words[2] in reg_32:
                    offset += 3
                    continue
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if words[2] in reg_32:
                        offset += 5
                        continue
                    if words[2] in reg_double:
                        offset += 6
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 7
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_64:
                            offset += 8
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 10
                            continue
                        if m.group(2) in reg_64:
                            offset += 11
                            continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 3
                        continue
                    if words[2] in reg_double:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 3
                        continue
                    if words[2] in reg_double:
                        offset += 4
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                    if words[2] in reg_double:
                        offset += 5
                        continue
            else:
                if words[2] in reg_32:
                    offset += 7
                    continue
                if words[2] in reg_double:
                    offset += 8
                    continue

        if words[0] == "subl":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 2
                    continue
                if words[2] in reg_double:
                    offset += 3
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_32 or words[2] in reg_double:
                    offset += 3
                    continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 3
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 6
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 7
                        continue

        if words[0] == "movq":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue
            if words[1][:-1] in reg_64:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue
            if words[1][:-1] in reg_numbered:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    m = re.match("(.+)?\((.+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 8
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        if m.group(2) in reg_64:
                            offset += 9
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 11
                            continue
                        if m.group(2) in reg_64:
                            offset += 12
                            continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 4
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 8
                        continue

        if words[0] == "movslq":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_64 or words[2] in reg_numbered or words[2] in reg_32:
                    offset += 3
                    continue
            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                    if words[2] in reg_numbered:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if words[2] in reg_32 or words[2] in reg_64:
                        offset += 4
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32 or words[2] in reg_64:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 8
                        continue

        if words[0] == "movb":
            if words[1][:-1] in reg_lbyte:
                if words[2] in reg_lbyte:
                    offset += 2
                    continue
                if words[2] in reg_byte:
                    offset += 3
                    continue
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 3
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 3
                        continue
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 6
                        continue
                    if m.group(2) in reg_64:
                        offset += 7
                        continue
            if words[1][:-1] in reg_byte:
                if words[2] in reg_lbyte or words[2] in reg_byte:
                    offset += 3
                    continue
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        offset += 4
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 7
                        continue
                    if m.group(2) in reg_64:
                        offset += 8
                        continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    m = re.match("(.+)?\((.+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 4
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 4
                            continue
                        if m.group(2) in reg_64:
                            offset += 5
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 7
                            continue
                        if m.group(2) in reg_64:
                            offset += 8
                            continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_lbyte:
                        offset += 3
                        continue
                    if words[2] in reg_byte:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if words[2] in reg_lbyte:
                        offset += 3
                        continue
                    if words[2] in reg_byte:
                        offset += 4
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_lbyte:
                        offset += 4
                        continue
                    if words[2] in reg_byte:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_lbyte:
                        offset += 6
                        continue
                    if words[2] in reg_byte:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_lbyte:
                        offset += 7
                        continue
                    if words[2] in reg_byte:
                        offset += 8
                        continue

        if words[0] in "prefetcht1":
            offset += 7
            continue

        if words[0] in "vmovsd":
            if words[1][:-1] in reg_sse:
                if words[2][:-1] in reg_sse:
                    if words[3] in reg_sse:
                        offset += 4
                        higher = 0
                        m = re.match("%xmm(\d+)", words[1][:-1])
                        if int(m.group(1)) > 7:
                            higher += 1
                        m = re.match("%xmm(\d+)", words[3])
                        if int(m.group(1)) > 7:
                            higher += 1
                        if higher > 1:
                            offset += 1
                        continue
                else:
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m is not None:
                        if m.group(1) is None:
                            if m.group(2) in reg_32:
                                offset += 4
                                continue
                            if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                                offset += 5
                                continue
                        elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                            if m.group(2) in reg_32:
                                offset += 5
                                continue
                            if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                                offset += 6
                                continue
                        else:
                            if m.group(2) in reg_32:
                                offset += 8
                                continue
                            if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                                offset += 9
                                continue
                    m = re.match(
                        "(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_32:
                            if m.group(3) in reg_32:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 5
                                    continue
                            if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 6
                                    continue
                        if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            if m.group(3) in reg_32:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 6
                                    continue
                            if m.group(3) in reg_64:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 6
                                    continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            if m.group(3) in reg_32:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 6
                                    continue
                            if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 7
                                    continue
                        if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            if m.group(3) in reg_32:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 7
                                    continue
                            if m.group(3) in reg_64:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 7
                                    continue
                    else:
                        if m.group(2) in reg_32:
                            if m.group(3) in reg_32:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 9
                                    continue
                            elif m.group(3) in reg_64 or m.group(3) in reg_numbered:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 10
                                    continue
                        elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                                if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                    offset += 10
                                    continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 4
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:  # byte
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 6
                            continue
                    elif m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                else:  # word
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 8
                            continue
                    elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 9
                            continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 5
                                continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 6
                                continue
                if m.group(2) in reg_numbered or m.group(2) in reg_64:
                    if m.group(3) in reg_64 or m.group(3) in reg_32 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 6
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None:
                            raise NotImplementedError
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                offset += 6
                                continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None:
                            raise NotImplementedError
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                offset += 7
                                continue
                if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                    if m.group(3) in reg_32 or m.group(3) in reg_numbered or m.group(3) in reg_64:
                        if m.group(4) is None:
                            raise NotImplementedError
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                offset += 7
                                continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None:
                            if words[2] in reg_sse:
                                offset += 9
                                continue
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                if words[2] in reg_sse:
                                    offset += 9
                                    continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                    if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None:
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                if words[2] in reg_sse:
                                    offset += 10
                                    continue

        if words[0] in "incl":
            if words[1] in reg_32:
                offset += 2
                continue
            elif words[1] in reg_double:
                offset += 3
                continue

            m = re.match("(.+)?\(([^,]+)\)", words[1])
            if m.group(1) is None:
                raise NotImplementedError
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    offset += 4
                    continue
            else:
                if m.group(2) in reg_32:
                    offset += 6
                    continue
                if m.group(2) in reg_64:
                    offset += 7
                    continue

        if words[0] in ["incq", "incb"]:
            if words[1] in reg_lbyte:
                offset += 2
                continue
            if words[1] in reg_32 or words[1] in reg_byte or words[1] in reg_64 or words[1] in reg_numbered:
                offset += 3
                continue

            m = re.match("(.+)?\(([^,]+)\)", words[1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    offset += 4
                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    offset += 5
                    continue
            else:
                if m.group(2) in reg_32:
                    offset += 7
                    continue
                if m.group(2) in reg_64:
                    offset += 8
                    continue

        if words[0] in "andl":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_32:
                    offset += 3
                    continue
                if words[2] in reg_double:
                    offset += 4
                    continue

        if words[0] == "cmpl":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 2
                    continue
                if words[2] in reg_double:
                    offset += 3
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_32 or words[2] in reg_double:
                    offset += 3
                    continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if words[2] in reg_32:
                        offset += 3
                        continue
                    if words[2] in reg_double:
                        offset += 4
                        continue
                    m2 = re.match("(.+)?\((.+)\)", words[2])
                    if m2.group(1) is None:
                        if m2.group(2) in reg_64:
                            offset += 4
                            continue
                    elif int(m2.group(1)) >= -128 and int(m2.group(1)) < 128:
                        if m2.group(2) in reg_64:
                            offset += 5
                            continue
                    else:
                        if m2.group(2) in reg_32:
                            offset += 7
                            continue
                        if m2.group(2) in reg_64:
                            offset += 8
                            continue
                else:
                    if words[2] in reg_32:
                        offset += 6
                        continue
                    if words[2] in reg_double:
                        offset += 7
                        continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 3
                        continue
                    if words[2] in reg_double:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                    if words[2] in reg_double:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 6
                        continue
                    if words[2] in reg_double:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 7
                        continue
                    if words[2] in reg_double:
                        offset += 8
                        continue

        if words[0] == "cmpq":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
            if words[1][:-1] in reg_64:
                if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_numbered:
                    offset += 3
                    continue
            if words[1][:-1] in reg_numbered:
                if words[2] in reg_64 or words[2] in reg_32:
                    offset += 3
                    continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_numbered:
                        offset += 4
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 5
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_64:
                            offset += 6
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        if m.group(2) in reg_64:
                            offset += 9
                            continueGL7d10-d1compr.spf.mv
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        raise NotImplementedError
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_64:
                            offset += 9
                            continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                        offset += 8
                        continue

        if words[0] == "cmpb":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_lbyte:
                    if words[2] == "%al":
                        offset += 2  # For some reason this one is different
                    else:
                        offset += 3
                    continue
                if words[2] in reg_byte:
                    offset += 4
                    continue

        if words[0] == "shlq":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_numbered:
                    offset += 4
                    continue

        if words[0] in ["shll", "shlb"]:
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_32 or words[2] in reg_lbyte:
                    offset += 3
                    continue
                elif words[2] in reg_byte or words[2] in reg_double:
                    offset += 4
                    continue

        if words[0] == "vmovhpd":
            if words[1][:-1] in reg_sse:
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m is not None:
                    if m.group(1) is None:
                        if m.group(2) in reg_numbered:
                            offset += 5
                            continue
                        else:
                            raise NotImplementedError
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 5
                            continue
                        if m.group(2) in reg_numbered:
                            offset += 6
                            continue
                        if m.group(2) in reg_64:
                            offset += 7
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        if m.group(2) in reg_numbered or m.group(2) in reg_64:
                            offset += 9
                            continue
                m = re.match("(.+)?\((.+),(.+),(.+)\)", words[2])
                if m.group(1) is None:
                    raise NotImplementedError
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 6
                                continue
                        if m.group(3) in reg_64:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 7
                                continue
                    if m.group(2) in reg_64:
                        if m.group(3) in reg_32:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 7
                                continue
                        if m.group(3) in reg_64:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 7
                                continue
                else:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 9
                                continue
                        if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 10
                                continue
                    if m.group(2) in reg_numbered or m.group(2) in reg_64:
                        if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                offset += 10
                                continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 4
                                continue
                    if m.group(2) in reg_numbered or m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 5
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 5
                                continue
                    if m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 6
                                continue
                    elif m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 7
                                continue
                else:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 8
                                continue
                    if m.group(2) in reg_numbered or m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 9
                                continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 5
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 9
                                    continue
                    elif m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue

        if words[0] in ["vxorpd", "vunpckhpd", "vaddsd", "vunpcklpd", "vmulsd"]:
            if words[1][:-1] in reg_sse:
                if words[2][:-1] in reg_sse:
                    if words[3] in reg_sse:
                        offset += 4
                        # For some reason, if more than one register higher than xmm7 are used, add 1 byte
                        higher = 0
                        m = re.match("%xmm(\d+)", words[1][:-1])
                        if int(m.group(1)) > 7:
                            offset += 1
                        continue
            elif words[1][:-1] in reg_avx:
                if words[2][:-1] in reg_avx:
                    if words[3] in reg_avx:
                        offset += 4
                        # For some reason, if more than one register higher than xmm7 are used, add 1 byte
                        higher = 0
                        m = re.match("%ymm(\d+)", words[1][:-1])
                        if int(m.group(1)) > 7:
                            offset += 1
                        continue

            m = re.match("(.+)?\(([^.]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                offset += 4
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 5
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                offset += 5
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 6
                                continue
                else:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                offset += 8
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 9
                                continue

            m = re.match("(.+)?\((.+),(.+),(.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 5
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_64:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered or m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 9
                                    continue
                    elif m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_numbered:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 10
                                    continue

        if words[0] == "vmulpd":
            if words[1][:-1] in reg_avx or words[1][:-1] in reg_sse:
                if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                    if words[3] in reg_avx or words[3] in reg_sse:
                        offset += 4
                        m = re.match("%(x|y)mm(\d+)", words[1][:-1])
                        # If destination register is larger than 7, +1 byte
                        if int(m.group(2)) > 7:
                            offset += 1
                        continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 4
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 5
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                offset += 5
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 6
                                continue
                else:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                offset += 8
                                continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                offset += 9
                                continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[2][:-1] in reg_sse:
                                    offset += 5
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(5) is None:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                    offset += 7
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 7
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 7
                                    continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 9
                                    continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 10
                                    continue
                if m.group(2) in reg_numbered or m.group(2) in reg_64:
                    if m.group(3) in reg_32 or m.group(3) in reg_numbered:
                        if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 10
                                    continue

        if words[0] in "vmovupd":
            if words[1][:-1] in reg_sse:
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m is not None:
                    if m.group(1) is None:
                        if m.group(2) in reg_32:
                            offset += 4
                            continue
                        if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 5
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 5
                            continue
                        if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 6
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 9
                            continue
                m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 5
                                continue
                    if m.group(2) in reg_64:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                    if m.group(2) in reg_numbered:
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                        if m.group(3) in reg_numbered:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                    if m.group(2) in reg_64:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                    if m.group(2) in reg_numbered:
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                else:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 9
                                continue
                        if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 10
                                continue
                    if m.group(2) in reg_numbered or m.group(2) in reg_64:
                        if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 10
                                continue
            elif words[1][:-1] in reg_avx:
                m = re.match("(.+)?\(([^,]+)\)", words[2])
                if m is not None:
                    if m.group(1) is None:
                        if m.group(2) in reg_32:
                            offset += 4
                            continue
                        elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 5
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 5
                            continue
                        elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 6
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        elif m.group(2) in reg_64 or m.group(2) in reg_numbered:
                            offset += 9
                            continue
                m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 5
                                continue
                        if m.group(3) in reg_numbered:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                    if m.group(2) in reg_numbered:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                    if m.group(2) in reg_64:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 6
                                continue
                        if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                    if m.group(2) in reg_numbered:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                    if m.group(2) in reg_64:
                        if m.group(3) in reg_32:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                        if m.group(3) in reg_64:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                offset += 7
                                continue
                else:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_32:
                            if m.group(4) is None:
                                raise NotImplementedError
                            else:
                                if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                    offset += 9
                                    continue
                        elif m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if m.group(4) is None:
                                raise NotImplementedError
                            else:
                                if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                    offset += 10
                                    continue
                    if m.group(2) in reg_numbered or m.group(2) in reg_64:
                        if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                            if m.group(4) is None:
                                offset += 10
                                continue
                            else:
                                if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                    offset += 10
                                    continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2] in reg_avx or words[2] in reg_sse:
                            offset += 4
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_avx or words[2] in reg_sse:
                            offset += 5
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 6
                            continue
                    if m.group(2) in reg_32:
                        if words[2] in reg_avx or words[2] in reg_sse:
                            offset += 5
                            continue
                else:
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_avx or words[2] in reg_sse:
                            offset += 9
                            continue
                    elif m.group(2) in reg_32:
                        if words[2] in reg_avx or words[2] in reg_sse:
                            offset += 8
                            continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 5
                                continue
                    if m.group(3) in reg_numbered or m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 6
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx or words[2] in reg_sse:
                                offset += 6
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 6
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 7
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32 or m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 7
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 7
                                continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse or words[2] in reg_avx:
                                offset += 9
                                continue
                    elif m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx or words[2] in reg_sse:
                                offset += 10
                                continue
                if m.group(2) in reg_numbered or m.group(2) in reg_64:
                    if m.group(3) in reg_32 or m.group(3) in reg_64 or m.group(3) in reg_numbered:
                        if m.group(4) is None:
                            if words[2] in reg_avx or words[2] in reg_sse:
                                offset += 10
                                continue
                        else:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                if words[2] in reg_avx or words[2] in reg_sse:
                                    offset += 10
                                    continue

        if words[0] in "vmovaps":
            if words[1][:-1] in reg_sse:
                if words[2] in reg_sse:
                    offset += 4
                    # For some reason, if two registers higher than xmm7 are used, add 1 byte
                    higher = 0
                    m = re.match("%xmm(\d+)", words[1][:-1])
                    if int(m.group(1)) > 7:
                        higher += 1
                    m = re.match("%xmm(\d+)", words[2])
                    if int(m.group(1)) > 7:
                        higher += 1
                    if higher == 2:
                        offset += 1
                    continue
            if words[1][:-1] in reg_avx:
                if words[2] in reg_avx:
                    offset += 4
                    # For some reason, if two registers higher than xmm7 are used, add 1 byte
                    higher = 0
                    m = re.match("%ymm(\d+)", words[1][:-1])
                    if int(m.group(1)) > 7:
                        higher += 1
                    m = re.match("%ymm(\d+)", words[2])
                    if int(m.group(1)) > 7:
                        higher += 1
                    if higher == 2:
                        offset += 1
                    continue

        if words[0].startswith("vfmadd"):
            if words[1][:-1] in reg_sse or words[1][:-1] in reg_avx:
                if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                    if words[3] in reg_sse or words[3] in reg_avx:
                        offset += 5
                        continue
            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 5
                                continue
                    if m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                if m.group(2) == "%rsp":
                                    offset += 6
                                else:
                                    offset += 5
                                continue
                    if m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                            if words[3] in reg_avx or words[3] in reg_sse:
                                offset += 5
                                continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 6
                                continue
                    if m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                if m.group(2) == "%rsp":
                                    offset += 7
                                else:
                                    offset += 6
                                continue
                    if m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 6
                                continue
                else:
                    if m.group(2) in reg_32:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 9
                                continue
                    if m.group(2) in reg_64:
                        if words[2][:-1] in reg_sse:
                            if words[3] in reg_sse:
                                if m.group(2) == "%rsp":
                                    offset += 10
                                else:
                                    offset += 9
                                continue
                        if words[2][:-1] in reg_avx:
                            if words[3] in reg_avx:
                                if m.group(2) == "%rsp":
                                    offset += 10
                                else:
                                    offset += 9
                                continue
                    if m.group(2) in reg_numbered:
                        if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                            if words[3] in reg_sse or words[3] in reg_avx:
                                offset += 9
                                continue

            m = re.match("(.+)?\(([^,]+)?,([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 6
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[2][:-1] in reg_avx:
                                    offset += 6
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx:
                                if words[3] in reg_avx:
                                    offset += 6
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 6
                                    continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 7
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse:
                                if words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 7
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_avx or words[2][:-1] in reg_sse:
                                if words[3] in reg_avx or words[3] in reg_sse:
                                    offset += 7
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 7
                                    continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                                if words[3] in reg_sse or words[3] in reg_avx:
                                    offset += 10
                                    continue

        if words[0] in "vextractf128":
            m = re.match("\$(\d+)", words[1][:-1])
            if int(m.group(1)) < 2:
                if words[2][:-1] in reg_avx:
                    if words[3] in reg_sse:
                        offset += 6
                        continue

                    m = re.match("(.+)?\((.+)\)", words[3])
                    if m.group(1) is None:
                        if m.group(2) in reg_32:
                            raise NotImplementedError(l)
                        elif m.group(2) in reg_64:
                            offset += 7
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            raise NotImplementedError(l)
                        elif m.group(2) in reg_64:
                            offset += 8
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 10
                            continue
                        elif m.group(2) in reg_64:
                            offset += 11
                            continue

        if words[0] in "vaddpd":
            if words[1][:-1] in reg_sse:
                if words[2][:-1] in reg_sse:
                    if words[3] in reg_sse:
                        offset += 4
                        # For some reason, if more than one register higher than xmm7 are used, add 1 byte
                        higher = 0
                        m = re.match("%xmm(\d+)", words[1][:-1])
                        if int(m.group(1)) > 7:
                            offset += 1
                        continue
            if words[1][:-1] in reg_avx:
                if words[2][:-1] in reg_avx:
                    if words[3] in reg_avx:
                        offset += 4
                        # For some reason, if more than one register higher than xmm7 are used, add 1 byte
                        higher = 0
                        m = re.match("%ymm(\d+)", words[1][:-1])
                        if int(m.group(1)) > 7:
                            offset += 1
                        continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    raise NotImplementedError(l)
                elif m.group(2) in reg_64:
                    if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                        if words[3] in reg_sse or words[3] in reg_avx:
                            offset += 5
                            continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    raise NotImplementedError(l)
                elif m.group(2) in reg_64:
                    if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                        if words[3] in reg_sse or words[3] in reg_avx:
                            offset += 6
                            continue
            else:
                if m.group(2) in reg_32:
                    if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                        if words[3] in reg_sse or words[3] in reg_avx:
                            offset += 8
                            continue
                elif m.group(2) in reg_64:
                    if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                        if words[3] in reg_sse or words[3] in reg_avx:
                            offset += 9
                            continue

        if words[0] in "vmovddup":
            if words[1][:-1] in reg_sse:
                if words[2] in reg_sse:
                    offset += 4
                    # For some reason, if more than one register higher than xmm7 are used, add 1 byte
                    higher = 0
                    m = re.match("%xmm(\d+)", words[1][:-1])
                    if int(m.group(1)) > 7:
                        offset += 1
                    continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 4
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 6
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_sse:
                            offset += 6
                            continue
                else:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 8
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 9
                            continue

            m = re.match("(.+)?\(([^,]+)?,([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 5
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 6
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 6
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 7
                                continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 9
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue

        if words[0] in "vzeroupper":
            offset += 3
            continue

        if words[0] in "retq":
            offset += 1
            continue

        if words[0] in "andb":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_lbyte:
                    offset += 2
                    continue
                if words[2] in reg_byte:
                    offset += 4
                    continue

        if words[0] in "andq":
            m = re.match("\$(.+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 4
                    continue

        if words[0] in "shrl":
            m = re.match("\$(\d+)", words[1][:-1])
            if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2] in reg_32:
                    offset += 3
                    continue
                if words[2] in reg_double:
                    offset += 4
                    continue

        if words[0] in "addl":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 2
                    continue
                if words[2] in reg_double:
                    offset += 3
                    continue
            if words[1][:-1] in reg_double:
                if words[2] in reg_double or words[2] in reg_32:
                    offset += 3
                    continue

            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if words[2] in reg_32:
                        offset += 3
                        continue
                    if words[2] in reg_double:
                        offset += 4
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 4
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 4
                            continue
                        if m.group(2) in reg_64:
                            offset += 5
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 7
                            continue
                        if m.group(2) in reg_64:
                            offset += 8
                            continue
                else:
                    if words[2] in reg_32:
                        offset += 6
                        continue
                    if words[2] in reg_double:
                        offset += 7
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        if m.group(2) in reg_64:
                            offset += 7
                            continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            offset += 7
                            continue
                        if m.group(2) in reg_64:
                            offset += 8
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 10
                            continue
                        if m.group(2) in reg_64:
                            offset += 11
                            continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 4
                        continue
                    if words[2] in reg_double:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_64:
                    if words[2] in reg_32:
                        offset += 7
                        continue
                    if words[2] in reg_double:
                        offset += 8
                        continue

        if words[0] in ["subq", "addq"]:
            if words[1][:-1] in reg_32:
                if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_numbered:
                    offset += 3
                    continue
            if words[1][:-1] in reg_64:
                if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                    offset += 3
                    continue
            if words[1][:-1] in reg_numbered:
                if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                    offset += 3
                    continue
            m = re.match("\$(.+)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    raise NotImplementedError(l)
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                        offset += 4
                        continue
                    m = re.match("(.+)?\(([^,]+)\)", words[2])
                    if m.group(1) is None:
                        raise NotImplementedError
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_64:
                            offset += 6
                            continue
                    else:
                        if m.group(2) in reg_32:
                            offset += 8
                            continue
                        if m.group(2) in reg_64:
                            offset += 9
                            continue
                else:
                    if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                        offset += 7
                        continue
                    m = re.match("(.+)?\(([^,]*)\)", words[2])
                    if m is not None:
                        if m.group(1) is None:
                            if m.group(2) in reg_64:
                                offset += 8
                                continue
                        elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                            if m.group(2) in reg_64:
                                offset += 9
                                continue
                        else:
                            if m.group(2) in reg_32:
                                offset += 11
                                continue
                            if m.group(2) in reg_64:
                                offset += 12
                                continue

            m = re.match("(.+)?\(([^,]*)\)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_64:
                        offset += 5
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_32:
                        offset += 7
                        continue
                if m.group(2) in reg_64:
                    if words[2] in reg_32 or words[2] in reg_64:
                        offset += 8
                        continue

        if words[0] == "lea":
            m = re.match("(.+)?\(([^,]*)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    raise NotImplementedError(l)
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2] in reg_32:
                            offset += 3
                            continue
                        if words[2] in reg_64 or words[2] in reg_double or words[2] in reg_numbered:
                            offset += 4
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_32 or words[2] in reg_double:
                            offset += 4
                            continue
                        if words[2] in reg_64:
                            offset += 5
                            continue
                else:
                    if m.group(2) in reg_32 or m.group(2) in reg_numbered:
                        if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64 or words[2] in reg_double:
                            offset += 7
                            continue
                    elif m.group(2) in reg_64:
                        if words[2] in reg_double:
                            offset += 7
                            continue
                        if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                            offset += 8
                            continue

            m = re.match("(.+)?\(([^,]+)?,([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) is None:
                    if m.group(3) in reg_32 or m.group(3) in reg_numbered or m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32:
                                offset += 7
                                continue
                            elif words[2] in reg_double or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 8
                                continue
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in words[2] in reg_32:
                                offset += 3
                                continue
                            if words[2] in reg_numbered or words[2] in reg_double or words[2] in reg_64:
                                offset += 4
                                continue
                    if m.group(3) in reg_numbered or m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 4
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_double or words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 4
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double or words[2] in reg_64:
                                offset += 4
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_double or words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 4
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_double or words[2] in reg_numbered:
                                offset += 4
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_double or words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 4
                                continue
                        else:
                            if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                                if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_double or words[2] in reg_numbered:
                                    offset += 4
                                    continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_double or words[2] in reg_numbered:
                                offset += 4
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) is None:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32:
                                offset += 7
                                continue
                            if words[2] in reg_double or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 8
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_double or words[2] in reg_32 or words[2] in reg_64:
                                offset += 8
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_double or words[2] in reg_64 or words[2] in reg_32:
                                offset += 8
                                continue
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 5
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_double or words[2] in reg_32:
                                offset += 5
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_double or words[2] in reg_numbered or words[2] in reg_32:
                                offset += 5
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double:
                                offset += 5
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 5
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64:
                                offset += 5
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_double:
                                offset += 5
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_double or words[2] in reg_64:
                                offset += 5
                                continue
            else:
                if m.group(2) is None:
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32:
                                offset += 8
                                continue
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_64:
                                offset += 8
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_double:
                                offset += 8
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_32 or words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 8
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_numbered or words[2] in reg_double:
                                offset += 8
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_32 or words[2] in reg_double:
                                offset += 8
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_numbered:
                                offset += 8
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64 or words[2] in reg_32:
                                offset += 8
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_64:
                                offset += 8
                                continue

        if words[0] in "leaveq" and not words[0] in "lea":
            offset += 1
            continue

        if words[0] in "vpermpd":
            m = re.match("\$(.+)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError(l)
            else:
                if words[2][:-1] in reg_avx:
                    if words[3] in reg_avx:
                        offset += 6
                        continue

                m = re.match("(.+)?\(([^,]*)\)", words[2][:-1])
                if m is not None:
                    if m.group(1) is None:
                        if m.group(2) in reg_32:
                            if words[3] in reg_avx:
                                offset += 6
                                continue
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        if m.group(2) in reg_32:
                            if words[3] in reg_avx:
                                offset += 7
                                continue
                        elif m.group(2) in reg_numbered:
                            if words[3] in reg_avx:
                                offset += 7
                                continue
                    else:
                        if m.group(2) in reg_32:
                            if words[3] in reg_avx:
                                offset += 10
                                continue
                        if m.group(2) in reg_64:
                            if words[3] in reg_avx:
                                offset += 10
                                continue

                m = re.match(
                    "(.+)?\(([^,]+)?,([^,]+)(,([^,]+))?\)", words[2][:-1])
                if m.group(1) is None:
                    raise NotImplementedError
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    raise NotImplementedError
                else:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_64:
                            if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                if words[3] in reg_avx:
                                    offset += 11
                                    continue

        if words[0] in "vshufpd":
            m = re.match("\$(.+)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2][:-1] in reg_avx:
                    if words[3][:-1] in reg_avx:
                        if words[4] in reg_avx:
                            offset += 5
                            m = re.match("%ymm(.+)", words[2][:-1])
                            if int(m.group(1)) > 7:
                                offset += 1
                            continue
            else:
                raise NotImplementedError

        if words[0] in ["vinsertf128", "vperm2f128", "vperm2i128", "vblendpd", "vpalignr"]:
            m = re.match("\$(.+)", words[1][:-1])
            if m.group(1) is None:
                if words[2][:-1] in reg_avx:
                    if words[3][:-1] in reg_avx:
                        if words[4][:-1] in reg_avx:
                            offset += 5
                            continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2][:-1] in reg_sse or words[2][:-1] in reg_avx:
                    if words[3][:-1] in reg_avx:
                        if words[4] in reg_avx:
                            offset += 6
                            continue
                else:
                    m = re.match("(.+)?\(([^,]+)\)", words[2][:-1])
                    if m is not None:
                        if m.group(1) is None:
                            if m.group(2) in reg_32:
                                raise NotImplementedError(l)
                            if m.group(2) in reg_64:
                                if words[3][:-1] in reg_avx:
                                    if words[4] in reg_avx:
                                        offset += 7
                                        continue
                        elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                            if m.group(2) in reg_32 or m.group(2) in reg_numbered:
                                if words[3][:-1] in reg_avx:
                                    if words[4] in reg_avx:
                                        offset += 7
                                        continue
                            if m.group(2) in reg_64:
                                if words[3][:-1] in reg_avx:
                                    if words[4] in reg_avx:
                                        offset += 8
                                        continue
                        else:
                            if m.group(2) in reg_32 or m.group(2) in reg_numbered:
                                if words[3][:-1] in reg_avx:
                                    if words[4] in reg_avx:
                                        offset += 10
                                        continue
                            if m.group(2) in reg_64:
                                if words[3][:-1] in reg_avx:
                                    if words[4] in reg_avx:
                                        offset += 11
                                        continue
                    m = re.match(
                        "(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[2][:-1])
                    if m.group(1) is None:
                        raise NotImplementedError
                    elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                        raise NotImplementedError
                    else:
                        if m.group(2) in reg_32:
                            if m.group(3) in reg_32:
                                if m.group(4) is None:
                                    if words[3][:-1] in reg_avx:
                                        if words[4] in reg_avx:
                                            offset += 11
                                            continue
                        if m.group(2) in reg_64:
                            if m.group(3) in reg_numbered:
                                if m.group(4) is None:
                                    raise NotImplementedError
                                else:
                                    if int(m.group(5)) >= -128 and int(m.group(5)) < 128:
                                        if words[3][:-1] in reg_avx:
                                            if words[4] in reg_avx:
                                                offset += 11
                                                continue
            else:
                raise NotImplementedError(l)

        if words[0] in "vmovapd":
            if words[1][:-1] in reg_sse or words[1][:-1] in reg_avx:
                if words[2] in reg_sse or words[2] in reg_avx:
                    offset += 4
                    higher = 0
                    m = re.match("%(x|y)mm(\d+)", words[1][:-1])
                    if int(m.group(2)) > 7:
                        higher += 1
                    m = re.match("%(x|y)mm(\d+)", words[2])
                    if int(m.group(2)) > 7:
                        higher += 1
                    if higher == 2:
                        offset += 1
                    continue
            if words[1][:-1] in reg_avx512:
                if words[2] in reg_avx512:
                    offset += 6
                    continue

        if words[0] in "vbroadcastsd":
            if words[1][:-1] in reg_sse:
                if words[2] in reg_avx:
                    offset += 5
                    continue
            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2] in reg_avx:
                            offset += 5
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_avx:
                            offset += 5
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_avx:
                            offset += 6
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2] in reg_avx:
                            offset += 6
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_avx:
                            offset += 6
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_avx:
                            offset += 7
                            continue
                else:
                    if m.group(2) in reg_32 or m.group(2) in reg_numbered:
                        if words[2] in reg_avx:
                            offset += 9
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_avx:
                            offset += 10
                            continue
            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 6
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 7
                                continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_64 or m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                if m.group(2) in reg_numbered:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                    if m.group(3) in reg_64:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_avx:
                                offset += 10
                                continue

        if words[0] in "vmovups":
            if words[1][:-1] in reg_avx or words[1][:-1] in reg_sse:
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        offset += 4
                        continue
                    if m.group(2) in reg_64:
                        offset += 5
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        offset += 5
                        continue
                    if m.group(2) in reg_64:
                        offset += 6
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 8
                        continue
                    elif m.group(2) in reg_64:
                        offset += 9
                        continue
            if words[1].startswith(".L"):
                # Uses label instead of offset. Quick hack: change label by whatever requiring a word to address
                words[1] = re.sub(".*(\(.*\))", "1024\\1", words[1])

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 4
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 5
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 5
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                        if words[2] in reg_avx:
                            offset += 6
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 6
                            continue
                else:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 8
                            continue
                    if m.group(2) in reg_64 or m.group(2) in reg_numbered:
                        if words[2] in reg_sse or words[2] in reg_avx:
                            offset += 9
                            continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 5
                                continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 6
                                continue
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 9
                                continue
                if m.group(2) in reg_64:
                    if m.group(3) in reg_numbered:
                        if m.group(4) is None or (int(m.group(5)) >= -128 and int(m.group(5)) < 128):
                            if words[2] in reg_sse:
                                offset += 10
                                continue

        if words[0] == "vmovq":
            if words[1][:-1] in reg_32:
                if words[2] in reg_32:
                    offset += 3
                    continue

            m = re.match("(.+)?\(([^,]+)\)", words[1][:-1])
            if m is not None:
                if m.group(1) is None:
                    if m.group(2) in reg_64:
                        if words[2] in reg_64 or words[2] in reg_numbered:
                            offset += 4
                            continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if words[2] in reg_sse:
                            offset += 5
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_64 or words[2] in reg_numbered:
                            offset += 5
                            continue
                        if words[2] in reg_sse:
                            offset += 7
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 6
                            continue
                else:
                    if m.group(2) in reg_32:
                        if words[2] in reg_32:
                            offset += 7
                            continue
                        elif words[2] in reg_sse:
                            offset += 8
                            continue
                    if m.group(2) in reg_numbered:
                        if words[2] in reg_sse:
                            offset += 9
                            continue
                    if m.group(2) in reg_64:
                        if words[2] in reg_sse:
                            offset += 9
                            continue

            m = re.match("(.+)?\(([^,]+),([^,]+)(,([^,]+))?\)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                raise NotImplementedError
            else:
                if m.group(2) in reg_32:
                    if m.group(3) in reg_32:
                        if m.group(4) is None:
                            if words[2] in reg_sse:
                                offset += 9
                                continue

        if words[0] in "vmovdqu":
            if words[1][:-1] in reg_avx:
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    if m.group(2) in reg_32:
                        raise NotImplementedException(l)
                    elif m.group(2) in reg_64:
                        offset += 5
                        continue
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64:
                        offset += 6
                        continue
                else:
                    if m.group(2) in reg_32:
                        offset += 8
                        continue
                    if m.group(2) in reg_64:
                        offset += 9
                        continue
            if words[1][:-1] in reg_sse:
                m = re.match("(.+)?\((.+)\)", words[2])
                if m.group(1) is None:
                    raise NotImplementedError
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_64:
                        offset += 6
                        continue
                else:
                    if m.group(2) in reg_64:
                        offset += 9
                        continue

            m = re.match("(.+)?\((.+)\)", words[1][:-1])
            if m.group(1) is None:
                if m.group(2) in reg_32:
                    raise NotImplementedError(l)
                elif m.group(2) in reg_64:
                    if words[2] in reg_avx:
                        offset += 5
                        continue
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if m.group(2) in reg_32:
                    raise NotImplementedError(l)
                elif m.group(2) in reg_64:
                    if words[2] in reg_avx:
                        offset += 6
                        continue
            else:
                if m.group(2) in reg_32:
                    if words[2] in reg_avx or words[2] in reg_sse:
                        offset += 8
                        continue
                elif m.group(2) in reg_64:
                    if words[2] in reg_avx or words[2] in reg_sse:
                        offset += 9
                        continue

        if words[0] in "vgatherdpd":
            if words[1][:-1] in reg_avx:
                m = re.match("(.+)?\((.+),(.+),(.+)\)", words[2][:-1])
                if m.group(1) is None:
                    raise NotImplementedError(l)
                elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_sse:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                if words[3] in reg_avx:
                                    offset += 7
                                    continue
                else:
                    if m.group(2) in reg_32:
                        if m.group(3) in reg_sse:
                            if int(m.group(4)) >= -128 and int(m.group(4)) < 128:
                                if words[3] in reg_avx:
                                    offset += 10
                                    continue

        if words[0] in "vpermilpd":
            m = re.match("\$(.+)", words[1][:-1])
            if m.group(1) is None:
                raise NotImplementedError(l)
            elif int(m.group(1)) >= -128 and int(m.group(1)) < 128:
                if words[2][:-1] in reg_avx:
                    if words[3] in reg_avx:
                        offset += 6
                        continue
            else:
                raise NotImplementedError(l)

        raise NotImplementedError(current_fragment_start + ":" + l)

    f.seek(0)
    offset = 0
    prev_byte = False
    for i, l in enumerate(f):
        if __debug__:
            print("<+%d>\t%s" % (offsets[i], l), end="", file=outf)
        else:
            cur_byte = l.strip().startswith(".byte")
            if offset >= 64 and not (prev_byte and cur_byte):
                print("\tprefetcht1 0x1000(%rip)", file=outf)
                offset = 7
            print(l, end="", file=outf)
            try:
                if offsets[i+1] - offsets[i] < 0:
                    offset = 0
                else:
                    offset += offsets[i+1] - offsets[i]
            except:
                pass
            prev_byte = cur_byte

    f.close()


def check_dump(fixed_path, dump_path, max_err=0.01):

    # fixed_path: path to the "fixed" file generated by fix_asm (with relative PCs on the left)
    # dump_path: output of objdump -d on the executable file

    ffix = open(fixed_path)
    fdump = open(dump_path)

    fragment = None
#    line_no = 0
#    fline_no = 0
    for l in fdump:
        #        line_no += 1
        if fragment is None:
            m = re.match("([0-9a-fA-F]+) <kernel_spmv_fragment_(\d+)>:", l)
            if not m is None:
                start_addr = int(m.group(1), 16)
                fragment = int(m.group(2))
        else:
            m = re.match("\s+([0-9a-fA-F]+):.+retq\s*$", l)
            if m is not None:
                match = "ret"
                end_addr = int(m.group(1), 16) - start_addr

                fl_fragment = False
                for fl in ffix:
                    #                    fline_no += 1
                    if not fl_fragment:
                        m = re.match(
                            "<\+\d+>\s+kernel_spmv_fragment_%d:$" % fragment, fl)
                        if not m is None:
                            fl_fragment = True
                    else:
                        m = re.match("<\+(\d+)>\s+ret\s+", fl)
                        if not m is None:
                            match_addr = int(m.group(1))
#                            if not match_addr == end_addr: raise ValueError()
                            rel_err = abs(
                                float(match_addr - end_addr) / min(match_addr, end_addr))
                            if rel_err > max_err:
                                raise ValueError()
                            print("Fragment %d: match == +%s (+/- %.2f%%)" %
                                  (fragment, hex(match_addr), rel_err*100))
                            fragment = None
                            break
                else:
                    raise ValueError()
            else:
                m = re.match("(\d+) <kernel_spmv_fragment_(\d+)>:", l)
                if m is not None:
                    ipdb.set_trace()

    ffix.close()
    fdump.close()


if __name__ == "__main__":
    try:
        path = sys.argv[1]
    except IndexError:
        print("Usage: fix_asm <path>")
        sys.exit(0)

    if len(sys.argv) > 2:
        outf = open(sys.argv[2], "w")
    else:
        outf = sys.stdout

    asm_prefetch(path, outf=outf)
