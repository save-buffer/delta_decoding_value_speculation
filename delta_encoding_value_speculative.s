.global DeltaEncode_ValueSpeculative
.global DeltaDecode_ValueSpeculative

DeltaEncode_ValueSpeculative:
        testq   %rdx, %rdx
        je      .L1
        xorl    %eax, %eax
        xorl    %ecx, %ecx
.L3:
        movl    %ecx, %r9d
        movl    (%rdi,%rax,4), %ecx
        movl    %ecx, %r8d
        subl    %r9d, %r8d
        movl    %r8d, (%rsi,%rax,4)
        addq    $1, %rax
        cmpq    %rax, %rdx
        jne     .L3
.L1:
        ret
DeltaDecode_ValueSpeculative:
        xorl    %r8d, %r8d
        xorl    %eax, %eax
        testq   %rdx, %rdx
        je      .L23
.L24:
        addl    (%rdi,%r8,4), %eax
        cmpl    %ecx, %eax
        je      .L31
        movl    %eax, (%rsi,%r8,4)
        addq    $1, %r8
        cmpq    %r8, %rdx
        jne     .L24
        ret
.L31:
        movl    %eax, (%rsi,%r8,4)
        addq    $1, %r8
        movl    %eax, %ecx
        cmpq    %r8, %rdx
        jne     .L24
        ret
.L23:
        ret
