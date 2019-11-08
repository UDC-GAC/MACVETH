# mark_description "Intel(R) C++ Intel(R) 64 Compiler for applications running on Intel(R) 64, Version 18.0.3.222 Build 20180410";
# mark_description "";
# mark_description "-O2 -vec-threshold0 -S";
	.file "reduction.c"
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
	.cfi_personality 0x3,__gxx_personality_v0
..___tag_value_main.1:
..L2:
                                                          #11.1
        pushq     %rbp                                          #11.1
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp                                    #11.1
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        andq      $-128, %rsp                                   #11.1
        subq      $128, %rsp                                    #11.1
        xorl      %esi, %esi                                    #11.1
        movl      $3, %edi                                      #11.1
        call      __intel_new_feature_proc_init                 #11.1
                                # LOE rbx r12 r13 r14 r15
..B1.14:                        # Preds ..B1.1
                                # Execution count [1.00e+00]
        vstmxcsr  (%rsp)                                        #11.1
        movl      $_ZSt4cout, %edi                              #18.12
        movl      $.L_2__STRING.0, %esi                         #18.12
        orl       $32832, (%rsp)                                #11.1
        vldmxcsr  (%rsp)                                        #11.1
        vmovupd   indata(%rip), %ymm0                           #12.25
        vpermilpd $5, %ymm0, %ymm1                              #13.9
        vaddpd    %ymm1, %ymm0, %ymm2                           #14.9
        vmovupd   %ymm0, ymm0(%rip)                             #12.2
        vmovupd   %ymm1, ymm1(%rip)                             #13.2
        vmovupd   %ymm2, ymm2(%rip)                             #14.2
        vperm2f128 $1, %ymm2, %ymm2, %ymm3                      #15.9
        vaddpd    %ymm3, %ymm2, %ymm4                           #16.9
        vmovupd   %ymm3, ymm3(%rip)                             #15.2
        vmovupd   %ymm4, ymm4(%rip)                             #16.2
        vmovupd   %ymm4, outdata(%rip)                          #17.19
        vzeroupper                                              #18.12
..___tag_value_main.6:
#       std::operator<<<std::char_traits<char>>(std::basic_ostream<char, std::char_traits<char>> &, const char *)
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #18.12
..___tag_value_main.7:
                                # LOE rax rbx r12 r13 r14 r15
..B1.2:                         # Preds ..B1.14
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.30
        vmovsd    outdata(%rip), %xmm0                          #18.30
..___tag_value_main.8:
#       std::basic_ostream<char, std::char_traits<char>>::operator<<(std::basic_ostream<char, std::char_traits<char>> *, double)
        call      _ZNSolsEd                                     #18.30
..___tag_value_main.9:
                                # LOE rax rbx r12 r13 r14 r15
..B1.3:                         # Preds ..B1.2
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.44
        movl      $.L_2__STRING.1, %esi                         #18.44
..___tag_value_main.10:
#       std::operator<<<std::char_traits<char>>(std::basic_ostream<char, std::char_traits<char>> &, const char *)
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #18.44
..___tag_value_main.11:
                                # LOE rax rbx r12 r13 r14 r15
..B1.4:                         # Preds ..B1.3
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.51
        vmovsd    8+outdata(%rip), %xmm0                        #18.51
..___tag_value_main.12:
#       std::basic_ostream<char, std::char_traits<char>>::operator<<(std::basic_ostream<char, std::char_traits<char>> *, double)
        call      _ZNSolsEd                                     #18.51
..___tag_value_main.13:
                                # LOE rax rbx r12 r13 r14 r15
..B1.5:                         # Preds ..B1.4
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.65
        movl      $.L_2__STRING.1, %esi                         #18.65
..___tag_value_main.14:
#       std::operator<<<std::char_traits<char>>(std::basic_ostream<char, std::char_traits<char>> &, const char *)
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #18.65
..___tag_value_main.15:
                                # LOE rax rbx r12 r13 r14 r15
..B1.6:                         # Preds ..B1.5
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.72
        vmovsd    16+outdata(%rip), %xmm0                       #18.72
..___tag_value_main.16:
#       std::basic_ostream<char, std::char_traits<char>>::operator<<(std::basic_ostream<char, std::char_traits<char>> *, double)
        call      _ZNSolsEd                                     #18.72
..___tag_value_main.17:
                                # LOE rax rbx r12 r13 r14 r15
..B1.7:                         # Preds ..B1.6
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.86
        movl      $.L_2__STRING.1, %esi                         #18.86
..___tag_value_main.18:
#       std::operator<<<std::char_traits<char>>(std::basic_ostream<char, std::char_traits<char>> &, const char *)
        call      _ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc #18.86
..___tag_value_main.19:
                                # LOE rax rbx r12 r13 r14 r15
..B1.8:                         # Preds ..B1.7
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.93
        vmovsd    24+outdata(%rip), %xmm0                       #18.93
..___tag_value_main.20:
#       std::basic_ostream<char, std::char_traits<char>>::operator<<(std::basic_ostream<char, std::char_traits<char>> *, double)
        call      _ZNSolsEd                                     #18.93
..___tag_value_main.21:
                                # LOE rax rbx r12 r13 r14 r15
..B1.9:                         # Preds ..B1.8
                                # Execution count [1.00e+00]
        movq      %rax, %rdi                                    #18.107
        movl      $_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_, %esi #18.107
..___tag_value_main.22:
#       std::basic_ostream<char, std::char_traits<char>>::operator<<(std::basic_ostream<char, std::char_traits<char>> *, std::basic_ostream<char, std::char_traits<char>>::__ostream_type &(*)(std::basic_ostream<char, std::char_traits<char>>::__ostream_type &))
        call      _ZNSolsEPFRSoS_E                              #18.107
..___tag_value_main.23:
                                # LOE rbx r12 r13 r14 r15
..B1.10:                        # Preds ..B1.9
                                # Execution count [1.00e+00]
        xorl      %eax, %eax                                    #19.1
        movq      %rbp, %rsp                                    #19.1
        popq      %rbp                                          #19.1
	.cfi_def_cfa 7, 8
	.cfi_restore 6
        ret                                                     #19.1
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	main,@function
	.size	main,.-main
..LNmain.0:
	.data
# -- End  main
	.section .text._ZNSt11char_traitsIcE6lengthEPKc, "xaG",@progbits,_ZNSt11char_traitsIcE6lengthEPKc,comdat
..TXTST1:
.L_2__routine_start__ZNSt11char_traitsIcE6lengthEPKc_1:
# -- Begin  _ZNSt11char_traitsIcE6lengthEPKc
	.section .text._ZNSt11char_traitsIcE6lengthEPKc, "xaG",@progbits,_ZNSt11char_traitsIcE6lengthEPKc,comdat
# mark_begin;
       .align    16,0x90
	.weak _ZNSt11char_traitsIcE6lengthEPKc
# --- std::char_traits<char>::length(const std::char_traits<char>::char_type *)
_ZNSt11char_traitsIcE6lengthEPKc:
# parameter 1: %rdi
..B2.1:                         # Preds ..B2.0
                                # Execution count [1.00e+00]
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
..___tag_value__ZNSt11char_traitsIcE6lengthEPKc.27:
..L28:
                                                         #317.7
        pushq     %rsi                                          #317.7
	.cfi_def_cfa_offset 16
        movq      %rdi, %rdx                                    #322.9
        movq      %rdx, %rcx                                    #322.9
        andq      $-16, %rdx                                    #322.9
        pxor      %xmm0, %xmm0                                  #322.9
        pcmpeqb   (%rdx), %xmm0                                 #322.9
        pmovmskb  %xmm0, %eax                                   #322.9
        andl      $15, %ecx                                     #322.9
        shrl      %cl, %eax                                     #322.9
        bsf       %eax, %eax                                    #322.9
        jne       ..L30         # Prob 60%                      #322.9
        movq      %rdx, %rax                                    #322.9
        addq      %rcx, %rdx                                    #322.9
        call      *__intel_sse2_strlen@GOTPCREL(%rip)           #322.9
..L30:                                                          #
                                # LOE rax rbx rbp r12 r13 r14 r15
..B2.4:                         # Preds ..B2.1
                                # Execution count [1.00e+00]
        popq      %rcx                                          #322.9
	.cfi_def_cfa_offset 8
        ret                                                     #322.9
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	_ZNSt11char_traitsIcE6lengthEPKc,@function
	.size	_ZNSt11char_traitsIcE6lengthEPKc,.-_ZNSt11char_traitsIcE6lengthEPKc
..LN_ZNSt11char_traitsIcE6lengthEPKc.1:
	.data
# -- End  _ZNSt11char_traitsIcE6lengthEPKc
	.text
.L_2__routine_start___sti__$E_2:
# -- Begin  __sti__$E
	.text
# mark_begin;
       .align    16,0x90
# --- __sti__$E()
__sti__$E:
..B3.1:                         # Preds ..B3.0
                                # Execution count [1.00e+00]
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
..___tag_value___sti__$E.33:
..L34:
                                                         #
        pushq     %rsi                                          #
	.cfi_def_cfa_offset 16
        movl      $_ZN17_INTERNALf276cea9St8__ioinitE, %edi     #74.25
..___tag_value___sti__$E.36:
#       std::ios_base::Init::Init(std::ios_base::Init *)
        call      _ZNSt8ios_base4InitC1Ev                       #74.25
..___tag_value___sti__$E.37:
                                # LOE rbx rbp r12 r13 r14 r15
..B3.2:                         # Preds ..B3.1
                                # Execution count [1.00e+00]
        movl      $_ZNSt8ios_base4InitD1Ev, %edi                #74.25
        movl      $_ZN17_INTERNALf276cea9St8__ioinitE, %esi     #74.25
        movl      $__dso_handle, %edx                           #74.25
        addq      $8, %rsp                                      #74.25
	.cfi_def_cfa_offset 8
#       __cxa_atexit(void (*)(void *), void *, void *)
        jmp       __cxa_atexit                                  #74.25
        .align    16,0x90
                                # LOE
	.cfi_endproc
# mark_end;
	.type	__sti__$E,@function
	.size	__sti__$E,.-__sti__$E
..LN__sti__$E.2:
	.data
# -- End  __sti__$E
	.bss
	.align 32
	.align 32
	.globl ymm0
ymm0:
	.type	ymm0,@object
	.size	ymm0,32
	.space 32	# pad
	.align 32
	.globl ymm1
ymm1:
	.type	ymm1,@object
	.size	ymm1,32
	.space 32	# pad
	.align 32
	.globl ymm2
ymm2:
	.type	ymm2,@object
	.size	ymm2,32
	.space 32	# pad
	.align 32
	.globl ymm3
ymm3:
	.type	ymm3,@object
	.size	ymm3,32
	.space 32	# pad
	.align 32
	.globl ymm4
ymm4:
	.type	ymm4,@object
	.size	ymm4,32
	.space 32	# pad
	.align 8
	.globl outdata
outdata:
	.type	outdata,@object
	.size	outdata,32
	.space 32	# pad
	.align 1
_ZN17_INTERNALf276cea9St8__ioinitE:
	.type	_ZN17_INTERNALf276cea9St8__ioinitE,@object
	.size	_ZN17_INTERNALf276cea9St8__ioinitE,1
	.space 1	# pad
	.data
	.align 8
	.align 8
	.globl indata
indata:
	.long	0x00000000,0x40000000
	.long	0x00000000,0x40080000
	.long	0x00000000,0x40000000
	.long	0x00000000,0x40140000
	.type	indata,@object
	.size	indata,32
	.section .rodata.str1.4, "aMS",@progbits,1
	.align 4
	.align 4
.L_2__STRING.0:
	.long	544503119
	.long	1952671094
	.long	540701295
	.byte	0
	.type	.L_2__STRING.0,@object
	.size	.L_2__STRING.0,13
	.space 3, 0x00 	# pad
	.align 4
.L_2__STRING.1:
	.word	32
	.type	.L_2__STRING.1,@object
	.size	.L_2__STRING.1,2
	.section .ctors, "wa"
	.align 8
__init_0:
	.type	__init_0,@object
	.size	__init_0,8
	.quad	__sti__$E
	.data
	.hidden __dso_handle
# mark_proc_addr_taken __sti__$E;
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 8
# End
