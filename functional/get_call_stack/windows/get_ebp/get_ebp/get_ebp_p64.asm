

.CODE
ALIGN 16

; https://en.wikipedia.org/wiki/X86_calling_conventions#Microsoft_x64_calling_convention
; has 32 byte shadow space
asm_func PROC
	mov rax, rsp
	ret
asm_func ENDP

END