section .data
    c21 dd -2
    c22 dd 14
    c31 dd 2
    c32 dd 6
section .text
global f1
f1:
    push ebp
    mov ebp, esp

    finit
    fldln2
    fld qword[ebp+8]
    fyl2x

    leave
    ret

global df1
df1:
    push ebp
    mov ebp, esp

    finit
    fld qword[ebp+8]
    fld1
    fdivp

    leave
    ret

global f2
f2:
    push ebp
    mov ebp, esp

    finit
    fild dword[c21]
    fld  qword[ebp+8]
    fmulp
    fild dword[c22]
    faddp

    leave
    ret

global df2
df2:
    push ebp
    mov ebp, esp

    finit
    fild dword[c21]

    leave
    ret
global f3
f3:
    push ebp
    mov ebp, esp


    finit
    fld1
    fild dword[c31]
    fld qword[ebp+8]
    fsubp

    fdivp
    fild dword[c32]
    faddp

    leave
    ret

global df3
df3:
    push ebp
    mov ebp, esp

    finit
    fld1
    fild dword[c31]
    fld qword[ebp+8]
    fsubp
    fld st0
    fmulp
    fdivp

    leave
    ret