.586
.model flat, stdcall
includelib ..\Debug\StandartLib.lib
includelib kernel32.lib
includelib libucrt.lib

ExitProcess PROTO : DWORD
copystr PROTO : DWORD, : DWORD
joinst PROTO : DWORD, : DWORD
outstr PROTO : DWORD
outint PROTO : SDWORD 
.stack 4096
.const
	overflow db 'ERROR: VARIABLE OVERFLOW', 0 
	null_division db 'ERROR: DIVISION BY ZERO', 0
	L1 SDWORD 3
	L2 SDWORD 1
	L3 SDWORD 9
	L4 BYTE "cool and unique string", 0
	L5 SDWORD 34
	L6 SDWORD 43
	L7 BYTE "right shift then left result", 0
	L10 BYTE "string concatenation by func", 0
	L11 BYTE " create", 0
	L12 BYTE " repository ", 0
	L13 BYTE "func with cycle result", 0
	L16 BYTE "string concatenation by operation plus", 0
	L17 BYTE " some ", 0
	L18 BYTE " string ", 0
	L19 BYTE "standart lib string copy", 0
	L20 BYTE "operation with oct ", 0
	L21 BYTE "operation with bin ", 0
	L23 SDWORD 0
.data
	Funcx SDWORD 0
	Funcprev SDWORD 0
	Funccur SDWORD 0
	Functemp SDWORD 0
	mainrez SDWORD 0
	mainother SDWORD 0
	mainoct SDWORD 0
	mainm BYTE 255 DUP(0)
	mainn BYTE 255 DUP(0)
	mainq BYTE 255 DUP(0)
	maine BYTE 255 DUP(0)
	mains BYTE 255 DUP(0)
	mainw SDWORD 0
	mainr SDWORD 0
	mainresr SDWORD 0
	mainresw SDWORD 0
	mainbin SDWORD 0
.code

Func_proc PROC, Funcp : SDWORD, Funcc : SDWORD
	push Funcp
	pop Funcprev
	push Funcc
	pop Funccur
	CYCLE: 
	push Funccur
	pop Functemp
	push Functemp
	push Funcprev
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop Funccur
	push Functemp
	pop Funcprev
	push Funcx
	push L2
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop Funcx
	cmp eax,L1
	jg NEXTL
	je NEXT
	loop CYCLE
	NEXTL:
	push L1
	NEXT:
	push Funccur
	pop eax
	ret 8


	jmp EXIT
	EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

	EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:
	pop eax
	ret 0

Func_proc ENDP

main PROC
	push L3
	pop mainoct
	push offset L4
	push offset mainm
	call copystr

	push L5
	pop mainw
	push L6
	pop mainr
	push offset L7
	call outstr

	push mainw
	push L1
	pop ebx
	pop eax
	mov cl, bl
	shr eax, cl
	push eax
	pop mainresw
	push mainr
	push L1
	pop ebx
	pop eax
	mov cl, bl
	shl eax, cl
	push eax
	pop mainresr
	push mainresw
	call outint

	push mainresr
	call outint

	push offset L10
	call outstr

	push offset L11
	push offset mainm
	call copystr

	push offset L12
	push offset mainn
	call copystr

	push offset mainm
	push offset mainn
	call joinst
	push eax
	push offset mainq
	call copystr

	push offset mainq
	call outstr

	push offset L13
	call outstr

	push L2
	push L2
	call Func_proc
	push eax
	pop mainrez
	push mainrez
	call outint

	push offset L16
	call outstr

	push offset L17
	push offset mainn
	call copystr

	push offset L18
	push offset mainm
	call copystr

	push offset mainm
	push offset mainn
	call joinst
	jo EXIT_OVERFLOW
	push eax
	push offset mainq
	call copystr

	push offset mainq
	call outstr

	push offset L19
	call outstr

	push offset mainm
	push offset maine
	call copystr
	push eax
	push offset mains
	call copystr

	push offset mains
	call outstr

	push offset L20
	call outstr

	push mainoct
	push mainrez
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop mainother
	push mainother
	call outint

	push offset L21
	call outstr

	push L1
	pop mainbin
	push mainbin
	push mainrez
	pop eax
	pop ebx
	add eax, ebx
	jo EXIT_OVERFLOW
	push eax
	pop mainother
	push mainother
	call outint

	push L23
	push 0
	call ExitProcess


	jmp EXIT
	EXIT_DIV_ON_NULL:
	push offset null_division
	call outstr
	push - 1
	call ExitProcess

	EXIT_OVERFLOW:
	push offset overflow
	call outstr
	push - 2
	call ExitProcess

	EXIT:
	push 0
	call ExitProcess

main ENDP
end main