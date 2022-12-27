.DATA

arrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
heightI DWORD 0			; the height of the image
widthI DWORD 0			; the width of the image
index DWORD 0


.CODE

MyProc1 proc

;get data ----------------------------------------------------

	push rbp
	mov  rbp, rsp


	mov arrayStart, RCX
	mov readyArray, RDX
	mov heightI, R8d
	mov widthI, R9d
	
	mov eax, DWORD PTR[rbp + 48]
	mov r15d,eax				;   Number of rows
	mov eax,DWORD PTR[rbp+56]
	mov r14d,eax				;	Position is stored 
	
	push r11
	push r10
	push r15
	push r14
	push r13
	push r12
;-----------------------------------------------------------	
padding:
	mov eax, widthI		;get w
	mov ecx,3
	mul ecx				;w*3
	mov ecx,4			
	div ecx
	mov ecx,edx			;(w*3)%4
	mov eax,4
	sub eax,ecx			;(4 - (w * 3) % 4)
	mov ecx,4
	div ecx
	mov eax,edx			;(4 - (w * 3) % 4) % 4
	mov r12d,eax		;padding

	mov r10d,0			;index

	mov eax,r14d
	mov ecx,r15d
	add eax,ecx
	mov r15d,eax		;position+ileBit

	mov eax,widthI
	add eax,widthI
	add eax,widthI
	mov r13d,eax		;iloscWiersza

loopRows:
	mov ecx,r14d		;y
	mov edx,r15d
	cmp ecx,edx
	jae ending
	mov eax,heightI
	dec eax
	mov edx,r14d
	cmp eax,edx
	je ending

	mov r8d,1			;x

loopColums:
	mov eax,widthI
	dec eax
	cmp r8d,eax
	jae increment		;next row


;liczenie indexu___________________________________
	mov eax,r14d		
	mul r13d
	mov r10d,eax
	mov eax,r8d
	mov ecx,3
	mul ecx
	mov ecx,eax
	mov eax,r10d
	add eax,ecx
	mov r10d,eax
	mov eax,r14d
	mul r12d
	mov ecx,eax
	mov eax,r10d
	add ecx,eax
	mov r10d,eax
;______________________________________________________
	
	mov rax,arrayStart
	add rax,r10
	mov rcx,rax					;startarraay +index
	mov eax,widthI
	
	movdqu xmm0,[rcx+r13-1]				;a[index + 3*w]
	movdqu xmm4,[rcx+r13+1-1]		;a[index + 1 + 3*w]
	movdqu xmm2,[rcx+r13-1-1]		;a[index - 1 + 3*w]
	mov rax,rcx
	sub rax,r13
	movdqu xmm1,[rax-1]					;a[index - 3*w]
	movdqu xmm3,[rax - 1-1]				; a[index - 1 - 3*w]
	movdqu xmm5,[rax + 1-1]		;a[index + 1 - 3*w]

	paddd xmm0,xmm0
	movdqu xmm10,xmm0
	psubb xmm0,xmm10
	psubb xmm0,xmm10		;*-2
	psubb xmm0,xmm4			;-1
	psubb xmm0,xmm2			;-1

	paddd xmm1,xmm1
	paddd xmm1,xmm3
	paddd xmm1,xmm5
	paddd xmm0,xmm1


	
	mov rax,readyArray
	add rax,r10
	movdqu [rax-1],xmm0

	mov eax,r8d
	;inc eax
	mov ecx,6
	add eax,ecx
	mov r8d,eax
	jmp loopColums

increment:
	mov eax,r14d
	inc eax
	mov r14d,eax
	jmp loopRows

ending:

pop rbp
pop r11
pop r10

pop r15
pop r14
pop r13
pop r12



ret
		
MyProc1 endp


END