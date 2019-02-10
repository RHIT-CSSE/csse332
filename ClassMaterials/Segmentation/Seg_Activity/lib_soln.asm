;lib.asm
;Michael Black, 2007

;lib.asm contains assembly functions that you can use in the shell

	.global _interrupt
    .global _change_stack_seg
    .global _change_data_seg

;int interrupt (int number, int AX, int BX, int CX, int DX)
_interrupt:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the interrupt number in AL
	push ds		;use self-modifying code to call the right interrupt
	mov bx,cs
	mov ds,bx
	mov si,#intr
	mov [si+1],al	;change the 00 below to the contents of AL
	pop ds
	mov ax,[bp+6]	;get the other parameters AX, BX, CX, and DX
	mov bx,[bp+8]
	mov cx,[bp+10]
	mov dx,[bp+12]

intr:	int #0x00	;call the interrupt (00 will be changed above)

	mov ah,#0	;we only want AL returned
	pop bp
	ret


;this is a hack
_change_stack_seg:
    mov dx,bp
    mov bp,sp
    mov ax,[bp+2]
    mov ss,ax
    mov bp,dx
    ret

_change_data_seg:
    mov dx,bp
    mov bp,sp
    mov ax,[bp+2]
    mov ds,ax
    mov bp,dx
    ret