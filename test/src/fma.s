# mark_description "Intel(R) C Intel(R) 64 Compiler for applications running on Intel(R) 64, Version 18.0.3.222 Build 20180410";
# mark_description "-O3 -march=native -S";
	.file "fma.c"
	.text
..TXTST0:
.L_2__routine_start_main_0:
# -- Begin  main
	.text
# mark_begin;
       .align    16,0x90
	.globl main
# --- main()
main:
..B1.1:                         # Preds ..B1.0
                                # Execution count [1.00e+00]
	.cfi_startproc
..___tag_value_main.1:
..L2:
                                                          #6.1
        pushq     %rbp                                          #6.1
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #6.1
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        andq      $-128, %rsp                                   #6.1
        subq      $24576, %rsp                                  #6.1
        xorl      %esi, %esi                                    #6.1
        movl      $3, %edi                                      #6.1
        call      __intel_new_feature_proc_init                 #6.1
                                # LOE rbx r12 r13 r14 r15
..B1.7:                         # Preds ..B1.1
                                # Execution count [1.00e+00]
        vstmxcsr  (%rsp)                                        #6.1
        xorl      %eax, %eax                                    #11.2
        orl       $32832, (%rsp)                                #6.1
        vldmxcsr  (%rsp)                                        #6.1
                                # LOE rax rbx r12 r13 r14 r15
..B1.2:                         # Preds ..B1.2 ..B1.7
                                # Execution count [1.02e+03]
        vmovupd   8192(%rsp,%rax,8), %ymm1                      #13.11
        lea       (,%rax,8), %rdx                               #13.3
        vmovupd   16384(%rsp,%rax,8), %ymm0                     #13.16
        vmovupd   8224(%rsp,%rax,8), %ymm3                      #13.11
        vmovupd   16416(%rsp,%rax,8), %ymm2                     #13.16
        vmovupd   8256(%rsp,%rax,8), %ymm5                      #13.11
        vmovupd   16448(%rsp,%rax,8), %ymm4                     #13.16
        vmovupd   8288(%rsp,%rax,8), %ymm7                      #13.11
        vmovupd   16480(%rsp,%rax,8), %ymm6                     #13.16
        vfmadd213pd 32(%rsp,%rax,8), %ymm2, %ymm3               #13.3
        vfmadd213pd (%rsp,%rax,8), %ymm0, %ymm1                 #13.3
        vfmadd213pd 64(%rsp,%rax,8), %ymm4, %ymm5               #13.3
        vfmadd213pd 96(%rsp,%rax,8), %ymm6, %ymm7               #13.3
        vmovupd   %ymm3, 32(%rsp,%rdx)                          #13.3
        vmovupd   %ymm1, (%rsp,%rdx)                            #13.3
        vmovupd   %ymm5, 64(%rsp,%rdx)                          #13.3
        vmovupd   %ymm7, 96(%rsp,%rdx)                          #13.3
        addq      $16, %rax                                     #11.2
        cmpq      $1024, %rax                                   #11.2
        jb        ..B1.2        # Prob 99%                      #11.2
                                # LOE rax rbx r12 r13 r14 r15
..B1.3:                         # Preds ..B1.2
                                # Execution count [1.00e+00]
        movl      $.L_2__STRING.0, %edi                         #16.2
        lea       (%rsp), %rsi                                  #16.2
        xorl      %eax, %eax                                    #16.2
        vzeroupper                                              #16.2
..___tag_value_main.6:
#       printf(const char *__restrict__, ...)
        call      printf                                        #16.2
..___tag_value_main.7:
                                # LOE
..B1.4:                         # Preds ..B1.3
                                # Execution count [1.00e+00]
        xorl      %edi, %edi                                    #17.2
#       exit(int)
        call      exit                                          #17.2
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	main,@function
	.size	main,.-main
..LNmain.0:
	.data
# -- End  main
	.section .rodata.str1.4, "aMS",@progbits,1
	.align 4
	.align 4
.L_2__STRING.0:
	.long	540876899
	.long	681509
	.type	.L_2__STRING.0,@object
	.size	.L_2__STRING.0,8
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
# End
