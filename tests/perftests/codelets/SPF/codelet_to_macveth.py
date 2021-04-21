#!/env/python3

import sys


def place_pragmas_in_codelet(f):
    open_file = open(f, "r")
    ext = ".c"
    if ".cpp" in f:
        ext = ".cpp"
    macveth_file = f.split(ext)[0] + ".mv" + ext
    outf = open(macveth_file, "w+")
    opened_bracket = -1
    reg_decl = False
    begin_func = False
    pragma_decl = False
    i_vars = []
    for l in open_file:
        if "kernel_spmv_fragment" in l:
            begin_func = True
        if not begin_func:
            outf.write(l)
            continue
        if "{" in l:
            opened_bracket += 1
        if "}" in l:
            opened_bracket -= 1

        if opened_bracket == 0 and reg_decl and not pragma_decl:
            pragma_decl = True
            s = "#pragma macveth "
            for v in i_vars:
                s += v + " full "
            outf.write(s + "\n")
        if "register int" in l:
            reg_decl = True
            tmp = l.split("register int")[1][:-2]
            i_vars = tmp.replace(" ", "").split(",")
        if opened_bracket == -1 and reg_decl:
            outf.write("#pragma endmacveth\n")
            begin_func = False
            reg_decl = False
            pragma_decl = False
        outf.write(l)
    outf.close()


if __name__ == "__main__":
    place_pragmas_in_codelet(sys.argv[1])
