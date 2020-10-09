	.section .text
.LNDBG_TX:
# mark_description "Intel(R) C Intel(R) 64 Compiler for applications running on Intel(R) 64, Version 18.0.3.222 Build 20180410";
# mark_description "-I/home/markoshorro/anaconda3/include/ -I . -L/home/markoshorro/anaconda3/lib/ -D__PURE_INTEL_C99_HEADERS__ ";
# mark_description "-O2 -g -xSKYLAKE -D_Float128=__float128 -mkl=sequential -qoverride-limits -vec-threshold0 -DEXTRALARGE_DATAS";
# mark_description "ET -DEXTRALARGE_DATASET -S -o GD98_a-d1compr.spf.S";
	.file "GD98_a-d1compr.spf.c"
	.text
..TXTST0:
.L_2__routine_start_kernel_spmv_fragment_0_0:
# -- Begin  kernel_spmv_fragment_0
	.text
# mark_begin;
       .align    16,0x90
	.globl kernel_spmv_fragment_0
# --- kernel_spmv_fragment_0(double *__restrict__, double *__restrict__, double *__restrict__)
kernel_spmv_fragment_0:
# parameter 1(A): %rdi
# parameter 2(x): %rsi
# parameter 3(y): %rdx
..B1.1:                         # Preds ..B1.0
                                # Execution count [1.00e+00]
..L26:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN0:
	.file   1 "GD98_a-d1compr.spf.c"
	.loc    1  61  is_stmt 1
..L25:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.111328
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.281250
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN1:
	.loc    1  58  is_stmt 1
..L24:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN2:
	.loc    1  55  is_stmt 1
..L23:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN3:
	.loc    1  52  is_stmt 1
..L22:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.880371
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.562500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN4:
	.loc    1  49  is_stmt 1
..L21:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.833496
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.687500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN5:
	.loc    1  46  is_stmt 1
..L20:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.833496
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.687500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN6:
	.loc    1  43  is_stmt 1
..L19:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN7:
	.loc    1  40  is_stmt 1
..L18:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN8:
	.loc    1  37  is_stmt 1
..L17:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN9:
	.loc    1  34  is_stmt 1
..L16:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN10:
	.loc    1  31  is_stmt 1
..L15:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN11:
	.loc    1  28  is_stmt 1
..L14:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN12:
	.loc    1  25  is_stmt 1
..L13:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN13:
	.loc    1  22  is_stmt 1
..L12:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN14:
	.loc    1  19  is_stmt 1
..L11:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L10:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.916992
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.562500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN15:
	.loc    1  16  is_stmt 1
..L9:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L8:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.686035
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN16:
	.loc    1  13  is_stmt 1
..L7:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L6:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.686035
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN17:
	.loc    1  10  is_stmt 1
..L5:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.865234
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 4
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN18:
	.loc    1  7  is_stmt 1
	.cfi_startproc
..___tag_value_kernel_spmv_fragment_0.2:
..L3:
                                                          #4.93
..LN19:
	.loc    1  4  prologue_end  is_stmt 1
..LN20:
	.loc    1  8  is_stmt 1
        vmovsd    32(%rsi), %xmm3                               #8.28
..LN21:
        vmovsd    144(%rsi), %xmm9                              #8.28
..LN22:
        vmovhpd   88(%rsi), %xmm3, %xmm10                       #8.28
..LN23:
        vmovhpd   200(%rsi), %xmm9, %xmm11                      #8.28
..LN24:
	.loc    1  11  is_stmt 1
        vmovsd    (%rdx), %xmm5                                 #11.5
..LN25:
	.loc    1  8  is_stmt 1
        vinsertf128 $1, %xmm11, %ymm10, %ymm12                  #8.28
..LN26:
        vmulpd    (%rdi), %ymm12, %ymm13                        #8.28
..LN27:
	.loc    1  14  is_stmt 1
        vxorpd    %xmm11, %xmm11, %xmm11                        #14.5
..LN28:
        vmovsd    64(%rsi), %xmm10                              #14.5
..LN29:
        vmovsd    96(%rsi), %xmm12                              #14.28
..LN30:
	.loc    1  8  is_stmt 1
        vextractf128 $1, %ymm13, %xmm14                         #8.5
..LN31:
        vaddpd    %xmm14, %xmm13, %xmm15                        #8.5
..LN32:
        vunpckhpd %xmm15, %xmm15, %xmm1                         #8.5
..LN33:
	.loc    1  14  is_stmt 1
        vmovhpd   136(%rsi), %xmm12, %xmm13                     #14.28
..LN34:
	.loc    1  8  is_stmt 1
        vaddsd    %xmm1, %xmm15, %xmm0                          #8.5
..LN35:
        vaddsd    72(%rdx), %xmm0, %xmm8                        #8.5
..LN36:
        vmovsd    %xmm8, 72(%rdx)                               #8.5
..LN37:
	.loc    1  11  is_stmt 1
        vmovhpd   48(%rsi), %xmm3, %xmm4                        #11.28
..LN38:
        vfmadd231pd 32(%rdi), %xmm4, %xmm5                      #11.5
..LN39:
	.loc    1  14  is_stmt 1
        vmovsd    176(%rsi), %xmm0                              #14.28
..LN40:
	.loc    1  17  is_stmt 1
        vmovupd   80(%rdi), %xmm3                               #17.18
..LN41:
	.loc    1  11  is_stmt 1
        vunpckhpd %xmm5, %xmm5, %xmm6                           #11.5
..LN42:
        vmovsd    24(%rsi), %xmm2                               #11.28
..LN43:
        vaddsd    %xmm6, %xmm5, %xmm9                           #11.5
..LN44:
        vmovhpd   40(%rsi), %xmm2, %xmm7                        #11.28
..LN45:
	.loc    1  14  is_stmt 1
        vfmadd231sd 48(%rdi), %xmm10, %xmm9                     #14.5
..LN46:
	.loc    1  17  is_stmt 1
        vmovsd    96(%rdi), %xmm6                               #17.18
..LN47:
	.loc    1  23  is_stmt 1
        vmovsd    120(%rsi), %xmm10                             #23.29
..LN48:
	.loc    1  14  is_stmt 1
        vmovsd    %xmm9, %xmm11, %xmm14                         #14.5
..LN49:
        vfmadd231pd 56(%rdi), %xmm13, %xmm14                    #14.5
..LN50:
	.loc    1  23  is_stmt 1
        vmovhpd   136(%rsi), %xmm10, %xmm11                     #23.29
..LN51:
        vmulpd    120(%rdi), %xmm11, %xmm12                     #23.29
..LN52:
	.loc    1  14  is_stmt 1
        vunpckhpd %xmm14, %xmm14, %xmm15                        #14.5
..LN53:
	.loc    1  23  is_stmt 1
        vunpckhpd %xmm12, %xmm12, %xmm13                        #23.5
..LN54:
	.loc    1  14  is_stmt 1
        vaddsd    %xmm15, %xmm14, %xmm1                         #14.5
..LN55:
        vfmadd231sd 72(%rdi), %xmm0, %xmm1                      #14.5
..LN56:
        vmovsd    %xmm1, (%rdx)                                 #14.5
..LN57:
	.loc    1  17  is_stmt 1
        vfmadd213pd (%rdx), %xmm7, %xmm3                        #17.5
..LN58:
        vmovsd    56(%rsi), %xmm7                               #17.29
..LN59:
        vfmadd213sd 16(%rdx), %xmm7, %xmm6                      #17.5
..LN60:
        vmovupd   %xmm3, (%rdx)                                 #17.5
..LN61:
        vmovsd    %xmm6, 16(%rdx)                               #17.5
..LN62:
	.loc    1  20  is_stmt 1
        vmovsd    232(%rsi), %xmm1                              #20.29
..LN63:
        vmovhpd   248(%rsi), %xmm1, %xmm2                       #20.29
..LN64:
        vmulpd    104(%rdi), %xmm2, %xmm4                       #20.29
..LN65:
        vunpckhpd %xmm4, %xmm4, %xmm5                           #20.5
..LN66:
        vmovsd    224(%rsi), %xmm0                              #20.29
..LN67:
        vaddsd    %xmm5, %xmm4, %xmm7                           #20.5
..LN68:
	.loc    1  23  is_stmt 1
        vaddsd    %xmm13, %xmm12, %xmm5                         #23.5
..LN69:
	.loc    1  20  is_stmt 1
        vaddsd    %xmm7, %xmm3, %xmm9                           #20.5
..LN70:
	.loc    1  23  is_stmt 1
        vaddsd    %xmm5, %xmm8, %xmm8                           #23.5
..LN71:
	.loc    1  20  is_stmt 1
        vmovhpd   240(%rsi), %xmm0, %xmm14                      #20.29
..LN72:
	.loc    1  29  is_stmt 1
        vmovsd    192(%rsi), %xmm0                              #29.30
..LN73:
        vmovhpd   216(%rsi), %xmm0, %xmm3                       #29.30
..LN74:
        vmulpd    152(%rdi), %xmm3, %xmm2                       #29.30
..LN75:
	.loc    1  26  is_stmt 1
        vmulpd    136(%rdi), %xmm14, %xmm15                     #26.29
..LN76:
	.loc    1  29  is_stmt 1
        vunpckhpd %xmm2, %xmm2, %xmm7                           #29.5
..LN77:
	.loc    1  20  is_stmt 1
        vmovsd    %xmm9, (%rdx)                                 #20.5
..LN78:
	.loc    1  29  is_stmt 1
        vaddsd    %xmm7, %xmm2, %xmm9                           #29.5
..LN79:
	.loc    1  32  is_stmt 1
        vmovsd    72(%rsi), %xmm2                               #32.29
..LN80:
        vmovhpd   104(%rsi), %xmm2, %xmm13                      #32.29
..LN81:
        vmulpd    168(%rdi), %xmm13, %xmm10                     #32.29
..LN82:
	.loc    1  29  is_stmt 1
        vaddsd    184(%rdx), %xmm9, %xmm3                       #29.5
..LN83:
	.loc    1  32  is_stmt 1
        vunpckhpd %xmm10, %xmm10, %xmm11                        #32.5
..LN84:
	.loc    1  38  is_stmt 1
        vmovsd    8(%rsi), %xmm9                                #38.29
..LN85:
	.loc    1  32  is_stmt 1
        vaddsd    %xmm11, %xmm10, %xmm12                        #32.5
..LN86:
	.loc    1  38  is_stmt 1
        vmovhpd   48(%rsi), %xmm9, %xmm10                       #38.29
..LN87:
	.loc    1  32  is_stmt 1
        vaddsd    %xmm12, %xmm6, %xmm6                          #32.5
..LN88:
	.loc    1  38  is_stmt 1
        vmulpd    200(%rdi), %xmm10, %xmm11                     #38.29
..LN89:
	.loc    1  47  is_stmt 1
        vmovupd   248(%rdi), %xmm9                              #47.19
..LN90:
	.loc    1  26  is_stmt 1
        vunpckhpd %xmm15, %xmm15, %xmm1                         #26.5
..LN91:
	.loc    1  38  is_stmt 1
        vunpckhpd %xmm11, %xmm11, %xmm12                        #38.5
..LN92:
	.loc    1  26  is_stmt 1
        vaddsd    %xmm1, %xmm15, %xmm4                          #26.5
..LN93:
	.loc    1  35  is_stmt 1
        vmulpd    184(%rdi), %xmm13, %xmm1                      #35.30
..LN94:
	.loc    1  38  is_stmt 1
        vaddsd    %xmm12, %xmm11, %xmm14                        #38.5
..LN95:
	.loc    1  50  is_stmt 1
        vmovupd   264(%rdi), %xmm11                             #50.18
..LN96:
	.loc    1  35  is_stmt 1
        vunpckhpd %xmm1, %xmm1, %xmm0                           #35.5
..LN97:
	.loc    1  32  is_stmt 1
        vmovsd    %xmm6, 16(%rdx)                               #32.5
..LN98:
	.loc    1  35  is_stmt 1
        vaddsd    %xmm0, %xmm1, %xmm6                           #35.5
..LN99:
	.loc    1  26  is_stmt 1
        vaddsd    %xmm4, %xmm8, %xmm1                           #26.5
..LN100:
	.loc    1  35  is_stmt 1
        vaddsd    176(%rdx), %xmm6, %xmm7                       #35.5
..LN101:
	.loc    1  38  is_stmt 1
        vaddsd    %xmm14, %xmm1, %xmm0                          #38.5
..LN102:
	.loc    1  41  is_stmt 1
        vmovhpd   128(%rsi), %xmm2, %xmm1                       #41.30
..LN103:
        vmulpd    216(%rdi), %xmm1, %xmm2                       #41.30
..LN104:
        vunpckhpd %xmm2, %xmm2, %xmm4                           #41.5
..LN105:
	.loc    1  50  is_stmt 1
        vmovsd    176(%rsi), %xmm8                              #50.29
..LN106:
	.loc    1  41  is_stmt 1
        vaddsd    %xmm4, %xmm2, %xmm5                           #41.5
..LN107:
	.loc    1  50  is_stmt 1
        vmovhpd   208(%rsi), %xmm8, %xmm10                      #50.29
..LN108:
	.loc    1  38  is_stmt 1
        vmovsd    %xmm0, 72(%rdx)                               #38.5
..LN109:
	.loc    1  35  is_stmt 1
        vmovsd    %xmm7, 176(%rdx)                              #35.5
..LN110:
	.loc    1  44  is_stmt 1
        vmovupd   232(%rdi), %xmm7                              #44.18
..LN111:
	.loc    1  41  is_stmt 1
        vaddsd    80(%rdx), %xmm5, %xmm6                        #41.5
..LN112:
        vmovsd    %xmm6, 80(%rdx)                               #41.5
..LN113:
	.loc    1  50  is_stmt 1
        vfmadd213pd 72(%rdx), %xmm10, %xmm11                    #50.5
..LN114:
	.loc    1  59  is_stmt 1
        vmovddup  (%rsi), %xmm0                                 #59.30
..LN115:
	.loc    1  53  is_stmt 1
        vmovsd    256(%rdx), %xmm12                             #53.5
..LN116:
        vmovsd    264(%rsi), %xmm13                             #53.30
..LN117:
        vmovhpd   280(%rsi), %xmm13, %xmm15                     #53.30
..LN118:
	.loc    1  50  is_stmt 1
        vmovupd   %xmm11, 72(%rdx)                              #50.5
..LN119:
	.loc    1  44  is_stmt 1
        vfmadd213pd 8(%rdx), %xmm0, %xmm7                       #44.5
..LN120:
	.loc    1  59  is_stmt 1
        vmovsd    80(%rdx), %xmm5                               #59.5
..LN121:
        vmovhpd   112(%rdx), %xmm5, %xmm6                       #59.5
..LN122:
	.loc    1  53  is_stmt 1
        vmovhpd   272(%rdx), %xmm12, %xmm2                      #53.5
..LN123:
	.loc    1  56  is_stmt 1
        vmovhpd   208(%rdx), %xmm3, %xmm4                       #56.5
..LN124:
	.loc    1  47  is_stmt 1
        vfmadd213pd 168(%rdx), %xmm0, %xmm9                     #47.5
..LN125:
	.loc    1  53  is_stmt 1
        vfmadd231pd 280(%rdi), %xmm15, %xmm2                    #53.5
..LN126:
	.loc    1  59  is_stmt 1
        vfmadd132pd 312(%rdi), %xmm6, %xmm0                     #59.5
..LN127:
	.loc    1  44  is_stmt 1
        vmovupd   %xmm7, 8(%rdx)                                #44.5
..LN128:
	.loc    1  53  is_stmt 1
        vmovsd    %xmm2, 256(%rdx)                              #53.5
..LN129:
        vmovhpd   %xmm2, 272(%rdx)                              #53.5
..LN130:
	.loc    1  59  is_stmt 1
        vmovsd    %xmm0, 80(%rdx)                               #59.5
..LN131:
        vmovhpd   %xmm0, 112(%rdx)                              #59.5
..LN132:
	.loc    1  56  is_stmt 1
        vmovsd    128(%rsi), %xmm2                              #56.30
..LN133:
	.loc    1  62  is_stmt 1
        vmovsd    8(%rdx), %xmm0                                #62.5
..LN134:
        vmovhpd   40(%rdx), %xmm0, %xmm7                        #62.5
..LN135:
	.loc    1  56  is_stmt 1
        vmovhpd   72(%rsi), %xmm2, %xmm3                        #56.30
..LN136:
        vfmadd231pd 296(%rdi), %xmm3, %xmm4                     #56.5
..LN137:
	.loc    1  62  is_stmt 1
        vfmadd231pd 328(%rdi), %xmm1, %xmm7                     #62.5
..LN138:
	.loc    1  47  is_stmt 1
        vmovupd   %xmm9, 168(%rdx)                              #47.5
..LN139:
	.loc    1  56  is_stmt 1
        vmovsd    %xmm4, 184(%rdx)                              #56.5
..LN140:
        vmovhpd   %xmm4, 208(%rdx)                              #56.5
..LN141:
	.loc    1  62  is_stmt 1
        vmovsd    %xmm7, 8(%rdx)                                #62.5
..LN142:
        vmovhpd   %xmm7, 40(%rdx)                               #62.5
..LN143:
	.loc    1  64  is_stmt 1
        vzeroupper                                              #64.1
..LN144:
	.loc    1  64  epilogue_begin  is_stmt 1
        ret                                                     #64.1
        .align    16,0x90
..LN145:
                                # LOE
..LN146:
	.cfi_endproc
# mark_end;
	.type	kernel_spmv_fragment_0,@function
	.size	kernel_spmv_fragment_0,.-kernel_spmv_fragment_0
..LNkernel_spmv_fragment_0.147:
.LNkernel_spmv_fragment_0:
	.data
# -- End  kernel_spmv_fragment_0
	.text
.L_2__routine_start_kernel_spmv_1:
# -- Begin  kernel_spmv
	.text
# mark_begin;
       .align    16,0x90
	.globl kernel_spmv
# --- kernel_spmv(double *__restrict__, double *__restrict__, double *__restrict__, long long, long long *, long long *)
kernel_spmv:
# parameter 1(A): %rdi
# parameter 2(x): %rsi
# parameter 3(y): %rdx
# parameter 4(n): %rcx
# parameter 5(indptr): %r8
# parameter 6(indices): %r9
..B2.1:                         # Preds ..B2.0
                                # Execution count [1.00e+00]
..L56:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 7
                # %s was not vectorized: vector dependence prevents vectorization%s
                # VECTOR TRIP COUNT IS KNOWN CONSTANT
                # COST MODEL DECISION WAS IGNORED
..LN148:
	.loc    1  71  is_stmt 1
..L55:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN149:
	.loc    1  61  is_stmt 1
..L54:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.111328
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.281250
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN150:
	.loc    1  58  is_stmt 1
..L53:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN151:
	.loc    1  55  is_stmt 1
..L52:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 1.158203
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.187500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN152:
	.loc    1  52  is_stmt 1
..L51:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.880371
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.562500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN153:
	.loc    1  49  is_stmt 1
..L50:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.833496
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.687500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN154:
	.loc    1  46  is_stmt 1
..L49:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.833496
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.687500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN155:
	.loc    1  43  is_stmt 1
..L48:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN156:
	.loc    1  40  is_stmt 1
..L47:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN157:
	.loc    1  37  is_stmt 1
..L46:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN158:
	.loc    1  34  is_stmt 1
..L45:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN159:
	.loc    1  31  is_stmt 1
..L44:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN160:
	.loc    1  28  is_stmt 1
..L43:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN161:
	.loc    1  25  is_stmt 1
..L42:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN162:
	.loc    1  22  is_stmt 1
..L41:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.592773
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN163:
	.loc    1  19  is_stmt 1
..L40:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L39:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.916992
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 0.562500
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN164:
	.loc    1  16  is_stmt 1
..L38:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L37:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.686035
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN165:
	.loc    1  13  is_stmt 1
..L36:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED BY 1
                # REMAINDER LOOP FOR VECTORIZATION
..L35:
                # optimization report
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.686035
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 2
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN166:
	.loc    1  10  is_stmt 1
..L34:
                # optimization report
                # LOOP WAS COMPLETELY UNROLLED
                # LOOP WAS VECTORIZED
                # VECTORIZATION HAS UNALIGNED MEMORY REFERENCES
                # VECTORIZATION SPEEDUP COEFFECIENT 0.865234
                # LOOP HAS ONE VECTOR ITERATION
                # VECTOR LENGTH 4
                # NORMALIZED VECTORIZATION OVERHEAD 1.500000
                # MAIN VECTOR TYPE: 64-bits floating point
                # COST MODEL DECISION WAS IGNORED
..LN167:
	.loc    1  7  is_stmt 1
	.cfi_startproc
..___tag_value_kernel_spmv.31:
..L32:
                                                         #66.137
..LN168:
	.loc    1  66  prologue_end  is_stmt 1
..LN169:
	.loc    1  8  is_stmt 1
        vmovsd    32(%rsi), %xmm3                               #8.28
..LN170:
        vmovsd    144(%rsi), %xmm9                              #8.28
..LN171:
        vmovhpd   88(%rsi), %xmm3, %xmm10                       #8.28
..LN172:
        vmovhpd   200(%rsi), %xmm9, %xmm11                      #8.28
..LN173:
	.loc    1  11  is_stmt 1
        vmovsd    (%rdx), %xmm5                                 #11.5
..LN174:
	.loc    1  72  is_stmt 1
        movq      (%r9), %rax                                   #72.35
..LN175:
        movq      (%r8), %r10                                   #72.7
..LN176:
        movq      8(%r9), %r11                                  #72.35
..LN177:
        movq      16(%r9), %rcx                                 #72.35
..LN178:
	.loc    1  8  is_stmt 1
        vinsertf128 $1, %xmm11, %ymm10, %ymm12                  #8.28
..LN179:
        vmulpd    (%rdi), %ymm12, %ymm13                        #8.28
..LN180:
	.loc    1  14  is_stmt 1
        vxorpd    %xmm11, %xmm11, %xmm11                        #14.5
..LN181:
        vmovsd    64(%rsi), %xmm10                              #14.5
..LN182:
        vmovsd    96(%rsi), %xmm12                              #14.28
..LN183:
	.loc    1  8  is_stmt 1
        vextractf128 $1, %ymm13, %xmm14                         #8.5
..LN184:
        vaddpd    %xmm14, %xmm13, %xmm15                        #8.5
..LN185:
        vunpckhpd %xmm15, %xmm15, %xmm1                         #8.5
..LN186:
	.loc    1  14  is_stmt 1
        vmovhpd   136(%rsi), %xmm12, %xmm13                     #14.28
..LN187:
	.loc    1  8  is_stmt 1
        vaddsd    %xmm1, %xmm15, %xmm0                          #8.5
..LN188:
        vaddsd    72(%rdx), %xmm0, %xmm8                        #8.5
..LN189:
        vmovsd    %xmm8, 72(%rdx)                               #8.5
..LN190:
	.loc    1  11  is_stmt 1
        vmovhpd   48(%rsi), %xmm3, %xmm4                        #11.28
..LN191:
        vfmadd231pd 32(%rdi), %xmm4, %xmm5                      #11.5
..LN192:
	.loc    1  14  is_stmt 1
        vmovsd    176(%rsi), %xmm0                              #14.28
..LN193:
	.loc    1  17  is_stmt 1
        vmovupd   80(%rdi), %xmm3                               #17.18
..LN194:
	.loc    1  11  is_stmt 1
        vunpckhpd %xmm5, %xmm5, %xmm6                           #11.5
..LN195:
        vmovsd    24(%rsi), %xmm2                               #11.28
..LN196:
        vaddsd    %xmm6, %xmm5, %xmm9                           #11.5
..LN197:
        vmovhpd   40(%rsi), %xmm2, %xmm7                        #11.28
..LN198:
	.loc    1  14  is_stmt 1
        vfmadd231sd 48(%rdi), %xmm10, %xmm9                     #14.5
..LN199:
	.loc    1  17  is_stmt 1
        vmovsd    96(%rdi), %xmm6                               #17.18
..LN200:
	.loc    1  23  is_stmt 1
        vmovsd    120(%rsi), %xmm10                             #23.29
..LN201:
	.loc    1  14  is_stmt 1
        vmovsd    %xmm9, %xmm11, %xmm14                         #14.5
..LN202:
        vfmadd231pd 56(%rdi), %xmm13, %xmm14                    #14.5
..LN203:
	.loc    1  23  is_stmt 1
        vmovhpd   136(%rsi), %xmm10, %xmm11                     #23.29
..LN204:
        vmulpd    120(%rdi), %xmm11, %xmm12                     #23.29
..LN205:
	.loc    1  14  is_stmt 1
        vunpckhpd %xmm14, %xmm14, %xmm15                        #14.5
..LN206:
	.loc    1  23  is_stmt 1
        vunpckhpd %xmm12, %xmm12, %xmm13                        #23.5
..LN207:
	.loc    1  14  is_stmt 1
        vaddsd    %xmm15, %xmm14, %xmm1                         #14.5
..LN208:
        vfmadd231sd 72(%rdi), %xmm0, %xmm1                      #14.5
..LN209:
        vmovsd    %xmm1, (%rdx)                                 #14.5
..LN210:
	.loc    1  17  is_stmt 1
        vfmadd213pd (%rdx), %xmm7, %xmm3                        #17.5
..LN211:
        vmovsd    56(%rsi), %xmm7                               #17.29
..LN212:
        vfmadd213sd 16(%rdx), %xmm7, %xmm6                      #17.5
..LN213:
        vmovupd   %xmm3, (%rdx)                                 #17.5
..LN214:
        vmovsd    %xmm6, 16(%rdx)                               #17.5
..LN215:
	.loc    1  20  is_stmt 1
        vmovsd    232(%rsi), %xmm1                              #20.29
..LN216:
        vmovhpd   248(%rsi), %xmm1, %xmm2                       #20.29
..LN217:
        vmulpd    104(%rdi), %xmm2, %xmm4                       #20.29
..LN218:
        vunpckhpd %xmm4, %xmm4, %xmm5                           #20.5
..LN219:
        vmovsd    224(%rsi), %xmm0                              #20.29
..LN220:
        vaddsd    %xmm5, %xmm4, %xmm7                           #20.5
..LN221:
	.loc    1  23  is_stmt 1
        vaddsd    %xmm13, %xmm12, %xmm5                         #23.5
..LN222:
	.loc    1  20  is_stmt 1
        vaddsd    %xmm7, %xmm3, %xmm9                           #20.5
..LN223:
	.loc    1  23  is_stmt 1
        vaddsd    %xmm5, %xmm8, %xmm8                           #23.5
..LN224:
	.loc    1  20  is_stmt 1
        vmovhpd   240(%rsi), %xmm0, %xmm14                      #20.29
..LN225:
	.loc    1  29  is_stmt 1
        vmovsd    192(%rsi), %xmm0                              #29.30
..LN226:
        vmovhpd   216(%rsi), %xmm0, %xmm3                       #29.30
..LN227:
        vmulpd    152(%rdi), %xmm3, %xmm2                       #29.30
..LN228:
	.loc    1  26  is_stmt 1
        vmulpd    136(%rdi), %xmm14, %xmm15                     #26.29
..LN229:
	.loc    1  29  is_stmt 1
        vunpckhpd %xmm2, %xmm2, %xmm7                           #29.5
..LN230:
	.loc    1  20  is_stmt 1
        vmovsd    %xmm9, (%rdx)                                 #20.5
..LN231:
	.loc    1  29  is_stmt 1
        vaddsd    %xmm7, %xmm2, %xmm9                           #29.5
..LN232:
	.loc    1  32  is_stmt 1
        vmovsd    72(%rsi), %xmm2                               #32.29
..LN233:
        vmovhpd   104(%rsi), %xmm2, %xmm13                      #32.29
..LN234:
        vmulpd    168(%rdi), %xmm13, %xmm10                     #32.29
..LN235:
	.loc    1  29  is_stmt 1
        vaddsd    184(%rdx), %xmm9, %xmm3                       #29.5
..LN236:
	.loc    1  32  is_stmt 1
        vunpckhpd %xmm10, %xmm10, %xmm11                        #32.5
..LN237:
	.loc    1  38  is_stmt 1
        vmovsd    8(%rsi), %xmm9                                #38.29
..LN238:
	.loc    1  32  is_stmt 1
        vaddsd    %xmm11, %xmm10, %xmm12                        #32.5
..LN239:
	.loc    1  38  is_stmt 1
        vmovhpd   48(%rsi), %xmm9, %xmm10                       #38.29
..LN240:
	.loc    1  32  is_stmt 1
        vaddsd    %xmm12, %xmm6, %xmm6                          #32.5
..LN241:
	.loc    1  38  is_stmt 1
        vmulpd    200(%rdi), %xmm10, %xmm11                     #38.29
..LN242:
	.loc    1  47  is_stmt 1
        vmovupd   248(%rdi), %xmm9                              #47.19
..LN243:
	.loc    1  26  is_stmt 1
        vunpckhpd %xmm15, %xmm15, %xmm1                         #26.5
..LN244:
	.loc    1  38  is_stmt 1
        vunpckhpd %xmm11, %xmm11, %xmm12                        #38.5
..LN245:
	.loc    1  26  is_stmt 1
        vaddsd    %xmm1, %xmm15, %xmm4                          #26.5
..LN246:
	.loc    1  35  is_stmt 1
        vmulpd    184(%rdi), %xmm13, %xmm1                      #35.30
..LN247:
	.loc    1  38  is_stmt 1
        vaddsd    %xmm12, %xmm11, %xmm14                        #38.5
..LN248:
	.loc    1  50  is_stmt 1
        vmovupd   264(%rdi), %xmm11                             #50.18
..LN249:
	.loc    1  35  is_stmt 1
        vunpckhpd %xmm1, %xmm1, %xmm0                           #35.5
..LN250:
	.loc    1  32  is_stmt 1
        vmovsd    %xmm6, 16(%rdx)                               #32.5
..LN251:
	.loc    1  35  is_stmt 1
        vaddsd    %xmm0, %xmm1, %xmm6                           #35.5
..LN252:
	.loc    1  26  is_stmt 1
        vaddsd    %xmm4, %xmm8, %xmm1                           #26.5
..LN253:
	.loc    1  35  is_stmt 1
        vaddsd    176(%rdx), %xmm6, %xmm7                       #35.5
..LN254:
	.loc    1  38  is_stmt 1
        vaddsd    %xmm14, %xmm1, %xmm0                          #38.5
..LN255:
	.loc    1  41  is_stmt 1
        vmovhpd   128(%rsi), %xmm2, %xmm1                       #41.30
..LN256:
        vmulpd    216(%rdi), %xmm1, %xmm2                       #41.30
..LN257:
        vunpckhpd %xmm2, %xmm2, %xmm4                           #41.5
..LN258:
	.loc    1  50  is_stmt 1
        vmovsd    176(%rsi), %xmm8                              #50.29
..LN259:
	.loc    1  41  is_stmt 1
        vaddsd    %xmm4, %xmm2, %xmm5                           #41.5
..LN260:
	.loc    1  50  is_stmt 1
        vmovhpd   208(%rsi), %xmm8, %xmm10                      #50.29
..LN261:
	.loc    1  38  is_stmt 1
        vmovsd    %xmm0, 72(%rdx)                               #38.5
..LN262:
	.loc    1  35  is_stmt 1
        vmovsd    %xmm7, 176(%rdx)                              #35.5
..LN263:
	.loc    1  44  is_stmt 1
        vmovupd   232(%rdi), %xmm7                              #44.18
..LN264:
	.loc    1  41  is_stmt 1
        vaddsd    80(%rdx), %xmm5, %xmm6                        #41.5
..LN265:
        vmovsd    %xmm6, 80(%rdx)                               #41.5
..LN266:
	.loc    1  50  is_stmt 1
        vfmadd213pd 72(%rdx), %xmm10, %xmm11                    #50.5
..LN267:
	.loc    1  59  is_stmt 1
        vmovddup  (%rsi), %xmm0                                 #59.30
..LN268:
	.loc    1  53  is_stmt 1
        vmovsd    256(%rdx), %xmm12                             #53.5
..LN269:
        vmovsd    264(%rsi), %xmm13                             #53.30
..LN270:
        vmovhpd   280(%rsi), %xmm13, %xmm15                     #53.30
..LN271:
	.loc    1  50  is_stmt 1
        vmovupd   %xmm11, 72(%rdx)                              #50.5
..LN272:
	.loc    1  44  is_stmt 1
        vfmadd213pd 8(%rdx), %xmm0, %xmm7                       #44.5
..LN273:
	.loc    1  59  is_stmt 1
        vmovsd    80(%rdx), %xmm5                               #59.5
..LN274:
        vmovhpd   112(%rdx), %xmm5, %xmm6                       #59.5
..LN275:
	.loc    1  53  is_stmt 1
        vmovhpd   272(%rdx), %xmm12, %xmm2                      #53.5
..LN276:
	.loc    1  56  is_stmt 1
        vmovhpd   208(%rdx), %xmm3, %xmm4                       #56.5
..LN277:
	.loc    1  47  is_stmt 1
        vfmadd213pd 168(%rdx), %xmm0, %xmm9                     #47.5
..LN278:
	.loc    1  53  is_stmt 1
        vfmadd231pd 280(%rdi), %xmm15, %xmm2                    #53.5
..LN279:
	.loc    1  59  is_stmt 1
        vfmadd132pd 312(%rdi), %xmm6, %xmm0                     #59.5
..LN280:
	.loc    1  44  is_stmt 1
        vmovupd   %xmm7, 8(%rdx)                                #44.5
..LN281:
	.loc    1  53  is_stmt 1
        vmovsd    %xmm2, 256(%rdx)                              #53.5
..LN282:
        vmovhpd   %xmm2, 272(%rdx)                              #53.5
..LN283:
	.loc    1  59  is_stmt 1
        vmovsd    %xmm0, 80(%rdx)                               #59.5
..LN284:
        vmovhpd   %xmm0, 112(%rdx)                              #59.5
..LN285:
	.loc    1  56  is_stmt 1
        vmovsd    128(%rsi), %xmm2                              #56.30
..LN286:
	.loc    1  62  is_stmt 1
        vmovsd    8(%rdx), %xmm0                                #62.5
..LN287:
        vmovhpd   40(%rdx), %xmm0, %xmm7                        #62.5
..LN288:
	.loc    1  56  is_stmt 1
        vmovhpd   72(%rsi), %xmm2, %xmm3                        #56.30
..LN289:
        vfmadd231pd 296(%rdi), %xmm3, %xmm4                     #56.5
..LN290:
	.loc    1  62  is_stmt 1
        vfmadd231pd 328(%rdi), %xmm1, %xmm7                     #62.5
..LN291:
	.loc    1  47  is_stmt 1
        vmovupd   %xmm9, 168(%rdx)                              #47.5
..LN292:
	.loc    1  56  is_stmt 1
        vmovsd    %xmm4, 184(%rdx)                              #56.5
..LN293:
        vmovhpd   %xmm4, 208(%rdx)                              #56.5
..LN294:
	.loc    1  62  is_stmt 1
        vmovsd    %xmm7, 8(%rdx)                                #62.5
..LN295:
        vmovhpd   %xmm7, 40(%rdx)                               #62.5
..LN296:
	.loc    1  72  is_stmt 1
        vmovsd    344(%rdi), %xmm8                              #72.21
..LN297:
        vmovsd    (%rsi,%rax,8), %xmm1                          #72.33
..LN298:
        vfmadd213sd (%rdx,%r10,8), %xmm1, %xmm8                 #72.5
..LN299:
        vmovsd    352(%rdi), %xmm10                             #72.21
..LN300:
        vmovsd    (%rsi,%r11,8), %xmm9                          #72.33
..LN301:
        vmovsd    360(%rdi), %xmm12                             #72.21
..LN302:
        vmovsd    %xmm8, (%rdx,%r10,8)                          #72.5
..LN303:
        movq      8(%r8), %rax                                  #72.7
..LN304:
        movq      16(%r8), %r10                                 #72.7
..LN305:
        vmovsd    (%rsi,%rcx,8), %xmm11                         #72.33
..LN306:
        vfmadd213sd (%rdx,%rax,8), %xmm9, %xmm10                #72.5
..LN307:
        vmovsd    368(%rdi), %xmm14                             #72.21
..LN308:
        vmovsd    376(%rdi), %xmm0                              #72.21
..LN309:
        vmovsd    384(%rdi), %xmm2                              #72.21
..LN310:
        vmovsd    %xmm10, (%rdx,%rax,8)                         #72.5
..LN311:
        vfmadd213sd (%rdx,%r10,8), %xmm11, %xmm12               #72.5
..LN312:
        vmovsd    392(%rdi), %xmm4                              #72.21
..LN313:
        vmovsd    %xmm12, (%rdx,%r10,8)                         #72.5
..LN314:
        movq      24(%r9), %r11                                 #72.35
..LN315:
        movq      24(%r8), %rax                                 #72.7
..LN316:
        movq      32(%r9), %rcx                                 #72.35
..LN317:
        vmovsd    (%rsi,%r11,8), %xmm13                         #72.33
..LN318:
        vfmadd213sd (%rdx,%rax,8), %xmm13, %xmm14               #72.5
..LN319:
        vmovsd    (%rsi,%rcx,8), %xmm15                         #72.33
..LN320:
        vmovsd    %xmm14, (%rdx,%rax,8)                         #72.5
..LN321:
        movq      32(%r8), %r10                                 #72.7
..LN322:
        movq      40(%r9), %r11                                 #72.35
..LN323:
        movq      40(%r8), %rax                                 #72.7
..LN324:
        vfmadd213sd (%rdx,%r10,8), %xmm15, %xmm0                #72.5
..LN325:
        vmovsd    (%rsi,%r11,8), %xmm1                          #72.33
..LN326:
        vmovsd    %xmm0, (%rdx,%r10,8)                          #72.5
..LN327:
        vfmadd213sd (%rdx,%rax,8), %xmm1, %xmm2                 #72.5
..LN328:
        vmovsd    %xmm2, (%rdx,%rax,8)                          #72.5
..LN329:
        movq      48(%r9), %rdi                                 #72.35
..LN330:
        movq      48(%r8), %r8                                  #72.7
..LN331:
        vmovsd    (%rsi,%rdi,8), %xmm3                          #72.33
..LN332:
        vfmadd213sd (%rdx,%r8,8), %xmm3, %xmm4                  #72.5
..LN333:
        vmovsd    %xmm4, (%rdx,%r8,8)                           #72.5
..LN334:
	.loc    1  74  is_stmt 1
        vzeroupper                                              #74.1
..LN335:
	.loc    1  74  epilogue_begin  is_stmt 1
        ret                                                     #74.1
        .align    16,0x90
..LN336:
                                # LOE
..LN337:
	.cfi_endproc
# mark_end;
	.type	kernel_spmv,@function
	.size	kernel_spmv,.-kernel_spmv
..LNkernel_spmv.338:
.LNkernel_spmv:
	.data
# -- End  kernel_spmv
	.data
	.section .debug_opt_report, ""
..L60:
	.ascii ".itt_notify_tab\0"
	.word	258
	.word	48
	.long	46
	.long	..L61 - ..L60
	.long	48
	.long	..L62 - ..L60
	.long	818
	.long	0x00000008,0x00000000
	.long	0
	.long	0
	.long	0
	.long	1
	.quad	..L26
	.long	28
	.long	5
	.quad	..L25
	.long	28
	.long	24
	.quad	..L24
	.long	28
	.long	43
	.quad	..L23
	.long	28
	.long	62
	.quad	..L22
	.long	28
	.long	81
	.quad	..L21
	.long	28
	.long	100
	.quad	..L20
	.long	28
	.long	119
	.quad	..L19
	.long	28
	.long	138
	.quad	..L18
	.long	28
	.long	157
	.quad	..L17
	.long	28
	.long	176
	.quad	..L16
	.long	28
	.long	195
	.quad	..L15
	.long	28
	.long	214
	.quad	..L14
	.long	28
	.long	233
	.quad	..L13
	.long	28
	.long	252
	.quad	..L12
	.long	28
	.long	271
	.quad	..L11
	.long	28
	.long	290
	.quad	..L10
	.long	28
	.long	302
	.quad	..L9
	.long	28
	.long	321
	.quad	..L8
	.long	28
	.long	333
	.quad	..L7
	.long	28
	.long	352
	.quad	..L6
	.long	28
	.long	364
	.quad	..L5
	.long	28
	.long	383
	.quad	..L56
	.long	28
	.long	402
	.quad	..L55
	.long	28
	.long	421
	.quad	..L54
	.long	28
	.long	440
	.quad	..L53
	.long	28
	.long	459
	.quad	..L52
	.long	28
	.long	478
	.quad	..L51
	.long	28
	.long	497
	.quad	..L50
	.long	28
	.long	516
	.quad	..L49
	.long	28
	.long	535
	.quad	..L48
	.long	28
	.long	554
	.quad	..L47
	.long	28
	.long	573
	.quad	..L46
	.long	28
	.long	592
	.quad	..L45
	.long	28
	.long	611
	.quad	..L44
	.long	28
	.long	630
	.quad	..L43
	.long	28
	.long	649
	.quad	..L42
	.long	28
	.long	668
	.quad	..L41
	.long	28
	.long	687
	.quad	..L40
	.long	28
	.long	706
	.quad	..L39
	.long	28
	.long	718
	.quad	..L38
	.long	28
	.long	737
	.quad	..L37
	.long	28
	.long	749
	.quad	..L36
	.long	28
	.long	768
	.quad	..L35
	.long	28
	.long	780
	.quad	..L34
	.long	28
	.long	799
..L61:
	.long	1769238639
	.long	1635412333
	.long	1852795252
	.long	1885696607
	.long	1601466991
	.long	1936876918
	.long	7237481
	.long	1769238639
	.long	1635412333
	.long	1852795252
	.long	1885696607
	.long	7631471
..L62:
	.long	-2079325440
	.long	-2146430462
	.long	-1065320320
	.long	-2139062144
	.long	-896499070
	.long	316244935
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-943222654
	.long	303225507
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084058480
	.long	269619929
	.long	-2139062144
	.long	-2139062080
	.long	-1870495104
	.long	-645675062
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1327988094
	.long	316449735
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-945104766
	.long	303226083
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-473453864
	.long	269619932
	.long	-2139062144
	.long	-2139062080
	.long	-259882368
	.long	-545011829
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1947172222
	.long	316638151
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-947130238
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084074512
	.long	269619935
	.long	-2139062144
	.long	-2139062080
	.long	-259882368
	.long	-545011829
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1947172222
	.long	316638151
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-947130238
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084074512
	.long	269161183
	.long	-2139062144
	.long	-2139061568
	.long	269617280
	.long	-2139062144
	.long	-2139062144
	.long	-1333624192
	.long	-593246283
	.long	-2146432238
	.long	-1065320320
	.long	-2139062142
	.long	-2146430456
	.long	-2139062144
	.long	-2139062144
	.long	-1746369918
	.long	316638151
	.long	-2139090933
	.long	-2101313408
	.long	142639232
	.long	-2139090926
	.long	-2139062144
	.long	-2105507712
	.long	-946345854
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2050511136
	.long	269619935
	.long	-2139062144
	.long	-2139058496
	.long	-2119139200
	.long	-2138996608
	.long	-2146430448
	.long	-1065320320
	.long	-2139062144
	.long	-896499070
	.long	316244935
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-943222654
	.long	303225507
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084058480
	.long	269619929
	.long	-2139062144
	.long	-2139062080
	.long	-1870495104
	.long	-645675062
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1327988094
	.long	316449735
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-945104766
	.long	303226083
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-473453864
	.long	269619932
	.long	-2139062144
	.long	-2139062080
	.long	-259882368
	.long	-545011829
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1947172222
	.long	316638151
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-947130238
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084074512
	.long	269619935
	.long	-2139062144
	.long	-2139062080
	.long	-259882368
	.long	-545011829
	.long	-2146430446
	.long	-1065320320
	.long	-2139062144
	.long	-1947172222
	.long	316638151
	.long	-2139090926
	.long	-2134867840
	.long	-2105507712
	.long	-947130238
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2084074512
	.long	269161183
	.long	-2139062144
	.long	-2139061568
	.long	269617280
	.long	-2139062144
	.long	-2139062144
	.long	-1333624192
	.long	-593246283
	.long	-2146432238
	.long	-1065320320
	.long	-2139062142
	.long	-2146430456
	.long	-2139062144
	.long	-2139062144
	.long	-1746369918
	.long	316638151
	.long	-2139090933
	.long	-2101313408
	.long	142639232
	.long	-2139090926
	.long	-2139062144
	.long	-2105507712
	.long	-946345854
	.long	303226755
	.long	-2139062256
	.long	-2139045760
	.long	-2105376640
	.long	-2050511136
	.word	4831
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .debug_info
	.section .debug_info
.debug_info_seg:
	.align 1
	.4byte 0x000001df
	.2byte 0x0004
	.4byte .debug_abbrev_seg
	.byte 0x08
//	DW_TAG_compile_unit:
	.byte 0x01
//	DW_AT_comp_dir:
	.4byte .debug_str
//	DW_AT_name:
	.4byte .debug_str+0x49
//	DW_AT_producer:
	.4byte .debug_str+0x5e
	.4byte .debug_str+0xca
//	DW_AT_language:
	.byte 0x01
//	DW_AT_use_UTF8:
	.byte 0x01
//	DW_AT_low_pc:
	.8byte 0x0000000000000000
//	DW_AT_ranges:
	.4byte .debug_ranges_seg+0x40
//	DW_AT_stmt_list:
	.4byte .debug_line_seg
//	DW_TAG_subprogram:
	.byte 0x02
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_prototyped:
	.byte 0x01
//	DW_AT_name:
	.4byte .debug_str+0x1d5
	.4byte .debug_str+0x1d5
//	DW_AT_low_pc:
	.8byte .L_2__routine_start_kernel_spmv_fragment_0_0
//	DW_AT_high_pc:
	.8byte ..LNkernel_spmv_fragment_0.147-.L_2__routine_start_kernel_spmv_fragment_0_0
//	DW_AT_external:
	.byte 0x01
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0041
//	DW_AT_location:
	.2byte 0x5501
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0078
//	DW_AT_location:
	.2byte 0x5401
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0079
//	DW_AT_location:
	.2byte 0x5101
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3069
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3169
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3269
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3369
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3469
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3569
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3669
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
	.byte 0x00
//	DW_TAG_restrict_type:
	.byte 0x05
//	DW_AT_type:
	.4byte 0x000000b8
//	DW_TAG_pointer_type:
	.byte 0x06
//	DW_AT_type:
	.4byte 0x000000bd
//	DW_TAG_base_type:
	.byte 0x07
//	DW_AT_byte_size:
	.byte 0x08
//	DW_AT_encoding:
	.byte 0x04
//	DW_AT_name:
	.4byte .debug_str+0x1ec
//	DW_TAG_base_type:
	.byte 0x08
//	DW_AT_byte_size:
	.byte 0x04
//	DW_AT_encoding:
	.byte 0x05
//	DW_AT_name:
	.4byte 0x00746e69
//	DW_TAG_subprogram:
	.byte 0x02
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_prototyped:
	.byte 0x01
//	DW_AT_name:
	.4byte .debug_str+0x1f3
	.4byte .debug_str+0x1f3
//	DW_AT_low_pc:
	.8byte .L_2__routine_start_kernel_spmv_1
//	DW_AT_high_pc:
	.8byte ..LNkernel_spmv.338-.L_2__routine_start_kernel_spmv_1
//	DW_AT_external:
	.byte 0x01
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0041
//	DW_AT_location:
	.2byte 0x5501
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0078
//	DW_AT_location:
	.2byte 0x5401
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0079
//	DW_AT_location:
	.2byte 0x5101
//	DW_TAG_formal_parameter:
	.byte 0x03
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000001b7
//	DW_AT_name:
	.2byte 0x006e
//	DW_AT_location:
	.2byte 0x5201
//	DW_TAG_formal_parameter:
	.byte 0x09
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000001be
//	DW_AT_name:
	.4byte .debug_str+0x209
//	DW_AT_location:
	.2byte 0x5801
//	DW_TAG_formal_parameter:
	.byte 0x09
//	DW_AT_decl_line:
	.byte 0x42
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000001be
//	DW_AT_name:
	.4byte .debug_str+0x210
//	DW_AT_location:
	.2byte 0x5901
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x43
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x0069
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x43
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x006a
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_inlined_subroutine:
	.byte 0x0a
//	DW_AT_ranges:
	.4byte .debug_ranges_seg
//	DW_AT_entry_pc:
	.8byte ..LN149
//	DW_AT_abstract_origin:
	.4byte 0x000001c3
//	DW_AT_call_line:
	.byte 0x45
//	DW_AT_call_column:
	.byte 0x03
//	DW_AT_call_file:
	.byte 0x01
//	DW_TAG_formal_parameter:
	.byte 0x0b
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0041
//	DW_TAG_formal_parameter:
	.byte 0x0b
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0078
//	DW_TAG_formal_parameter:
	.byte 0x0b
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_AT_name:
	.2byte 0x0079
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3069
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3169
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3269
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3369
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3469
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3569
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
//	DW_TAG_variable:
	.byte 0x04
//	DW_AT_decl_line:
	.byte 0x05
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_name:
	.2byte 0x3669
	.byte 0x00
//	DW_AT_type:
	.4byte 0x000000c4
	.byte 0x00
	.byte 0x00
//	DW_TAG_base_type:
	.byte 0x07
//	DW_AT_byte_size:
	.byte 0x08
//	DW_AT_encoding:
	.byte 0x05
//	DW_AT_name:
	.4byte .debug_str+0x1ff
//	DW_TAG_pointer_type:
	.byte 0x06
//	DW_AT_type:
	.4byte 0x000001b7
//	DW_TAG_subprogram:
	.byte 0x0c
//	DW_AT_decl_line:
	.byte 0x04
//	DW_AT_decl_file:
	.byte 0x01
//	DW_AT_declaration:
	.byte 0x01
//	DW_AT_prototyped:
	.byte 0x01
//	DW_AT_name:
	.4byte .debug_str+0x1d5
	.4byte .debug_str+0x1d5
//	DW_AT_external:
	.byte 0x01
//	DW_AT_inline:
	.byte 0x01
//	DW_TAG_formal_parameter:
	.byte 0x0d
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_TAG_formal_parameter:
	.byte 0x0d
//	DW_AT_type:
	.4byte 0x000000b3
//	DW_TAG_formal_parameter:
	.byte 0x0d
//	DW_AT_type:
	.4byte 0x000000b3
	.byte 0x00
	.byte 0x00
// -- Begin DWARF2 SEGMENT .debug_line
	.section .debug_line
.debug_line_seg:
	.align 1
// -- Begin DWARF2 SEGMENT .debug_abbrev
	.section .debug_abbrev
.debug_abbrev_seg:
	.align 1
	.byte 0x01
	.byte 0x11
	.byte 0x01
	.byte 0x1b
	.byte 0x0e
	.byte 0x03
	.byte 0x0e
	.byte 0x25
	.byte 0x0e
	.2byte 0x7681
	.byte 0x0e
	.byte 0x13
	.byte 0x0b
	.byte 0x53
	.byte 0x0c
	.byte 0x11
	.byte 0x01
	.byte 0x55
	.byte 0x17
	.byte 0x10
	.byte 0x17
	.2byte 0x0000
	.byte 0x02
	.byte 0x2e
	.byte 0x01
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x27
	.byte 0x0c
	.byte 0x03
	.byte 0x0e
	.2byte 0x4087
	.byte 0x0e
	.byte 0x11
	.byte 0x01
	.byte 0x12
	.byte 0x07
	.byte 0x3f
	.byte 0x0c
	.2byte 0x0000
	.byte 0x03
	.byte 0x05
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x49
	.byte 0x13
	.byte 0x03
	.byte 0x08
	.byte 0x02
	.byte 0x18
	.2byte 0x0000
	.byte 0x04
	.byte 0x34
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x03
	.byte 0x08
	.byte 0x49
	.byte 0x13
	.2byte 0x0000
	.byte 0x05
	.byte 0x37
	.byte 0x00
	.byte 0x49
	.byte 0x13
	.2byte 0x0000
	.byte 0x06
	.byte 0x0f
	.byte 0x00
	.byte 0x49
	.byte 0x13
	.2byte 0x0000
	.byte 0x07
	.byte 0x24
	.byte 0x00
	.byte 0x0b
	.byte 0x0b
	.byte 0x3e
	.byte 0x0b
	.byte 0x03
	.byte 0x0e
	.2byte 0x0000
	.byte 0x08
	.byte 0x24
	.byte 0x00
	.byte 0x0b
	.byte 0x0b
	.byte 0x3e
	.byte 0x0b
	.byte 0x03
	.byte 0x08
	.2byte 0x0000
	.byte 0x09
	.byte 0x05
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x49
	.byte 0x13
	.byte 0x03
	.byte 0x0e
	.byte 0x02
	.byte 0x18
	.2byte 0x0000
	.byte 0x0a
	.byte 0x1d
	.byte 0x01
	.byte 0x55
	.byte 0x17
	.byte 0x52
	.byte 0x01
	.byte 0x31
	.byte 0x13
	.byte 0x59
	.byte 0x0b
	.byte 0x57
	.byte 0x0b
	.byte 0x58
	.byte 0x0b
	.2byte 0x0000
	.byte 0x0b
	.byte 0x05
	.byte 0x00
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x49
	.byte 0x13
	.byte 0x03
	.byte 0x08
	.2byte 0x0000
	.byte 0x0c
	.byte 0x2e
	.byte 0x01
	.byte 0x3b
	.byte 0x0b
	.byte 0x3a
	.byte 0x0b
	.byte 0x3c
	.byte 0x0c
	.byte 0x27
	.byte 0x0c
	.byte 0x03
	.byte 0x0e
	.2byte 0x4087
	.byte 0x0e
	.byte 0x3f
	.byte 0x0c
	.byte 0x20
	.byte 0x0b
	.2byte 0x0000
	.byte 0x0d
	.byte 0x05
	.byte 0x00
	.byte 0x49
	.byte 0x13
	.2byte 0x0000
	.byte 0x00
// -- Begin DWARF2 SEGMENT .debug_frame
	.section .debug_frame
.debug_frame_seg:
	.align 1
// -- Begin DWARF2 SEGMENT .debug_str
	.section .debug_str,"MS",@progbits,1
.debug_str_seg:
	.align 1
	.8byte 0x616d2f656d6f682f
	.8byte 0x72726f68736f6b72
	.8byte 0x70736b726f772f6f
	.8byte 0x5643414d2f656361
	.8byte 0x747365742f485445
	.8byte 0x745f667265702f73
	.8byte 0x65646f632f747365
	.8byte 0x4650532f7374656c
	.8byte 0x67726f5f4650532f
	.byte 0x00
	.8byte 0x642d615f38394447
	.8byte 0x732e72706d6f6331
	.4byte 0x632e6670
	.byte 0x00
	.8byte 0x2952286c65746e49
	.8byte 0x6c65746e49204320
	.8byte 0x4320343620295228
	.8byte 0x2072656c69706d6f
	.8byte 0x6c70706120726f66
	.8byte 0x736e6f6974616369
	.8byte 0x676e696e6e757220
	.8byte 0x65746e49206e6f20
	.8byte 0x2c3436202952286c
	.8byte 0x6e6f697372655620
	.8byte 0x2e332e302e383120
	.8byte 0x6c69754220323232
	.8byte 0x3430383130322064
	.4byte 0x000a3031
	.8byte 0x2f656d6f682f492d
	.8byte 0x6f68736f6b72616d
	.8byte 0x63616e612f6f7272
	.8byte 0x6e692f3361646e6f
	.8byte 0x2d202f6564756c63
	.8byte 0x682f4c2d202e2049
	.8byte 0x6b72616d2f656d6f
	.8byte 0x2f6f72726f68736f
	.8byte 0x61646e6f63616e61
	.8byte 0x2d202f62696c2f33
	.8byte 0x5f455255505f5f44
	.8byte 0x39435f4c45544e49
	.8byte 0x5245444145485f39
	.8byte 0x20324f2d205f5f53
	.8byte 0x594b53782d20672d
	.8byte 0x5f442d20454b414c
	.8byte 0x38323174616f6c46
	.8byte 0x74616f6c665f5f3d
	.8byte 0x6c6b6d2d20383231
	.8byte 0x746e65757165733d
	.8byte 0x766f712d206c6169
	.8byte 0x6c2d656469727265
	.8byte 0x762d207374696d69
	.8byte 0x73657268742d6365
	.8byte 0x442d2030646c6f68
	.8byte 0x52414c4152545845
	.8byte 0x53415441445f4547
	.8byte 0x545845442d205445
	.8byte 0x5f454752414c4152
	.8byte 0x2054455341544144
	.8byte 0x4447206f2d20532d
	.8byte 0x6331642d615f3839
	.8byte 0x6670732e72706d6f
	.2byte 0x532e
	.byte 0x00
	.8byte 0x735f6c656e72656b
	.8byte 0x676172665f766d70
	.4byte 0x746e656d
	.2byte 0x305f
	.byte 0x00
	.4byte 0x62756f64
	.2byte 0x656c
	.byte 0x00
	.8byte 0x735f6c656e72656b
	.4byte 0x00766d70
	.8byte 0x6e6f6c20676e6f6c
	.2byte 0x0067
	.4byte 0x70646e69
	.2byte 0x7274
	.byte 0x00
	.8byte 0x0073656369646e69
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
// -- Begin DWARF2 SEGMENT .debug_ranges
	.section .debug_ranges
.debug_ranges_seg:
	.align 1
	.8byte ..LN149
	.8byte ..LN168
	.8byte ..LN169
	.8byte ..LN174
	.8byte ..LN178
	.8byte ..LN296
	.8byte 0x0000000000000000
	.8byte 0x0000000000000000
	.8byte .L_2__routine_start_kernel_spmv_fragment_0_0
	.8byte ..LNkernel_spmv.338
	.8byte 0x0000000000000000
	.8byte 0x0000000000000000
	.section .text
.LNDBG_TXe:
# End
