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
	mov eax,r8d
	inc eax
	mov r8d,eax

;liczenie indexu___________________________________
	mov eax,r14d		
	mov ecx,widthI
	mul ecx
	mov ecx,3
	mul ecx
	mov r10d,eax
	mov eax,r8d
	mul ecx
	mov ecx,eax
	mov eax,r10d
	add eax,ecx
	mov r10d,eax
	mov eax,r14d
	mov ecx,r12d
	mul ecx
	mov ecx,eax
	mov eax,r10d
	add ecx,eax
	mov r10d,eax
;______________________________________________________
	
	mov rax,arrayStart
	add rax,r10
	mov rcx,rax					;startarraay +index
	mov eax,widthI
	mov r11d,3
	mul r11d
	mov r11d,eax				;3*w
	movdqa xmm0,[rcx+rax]		;a[index + 3*w]
	mov eax,ecx
	sub eax,r11d
	movdqa xmm1,[rax]		;a[index - 3*w]


	movdqa xmm2,[rcx+r11-1]		;a[index - 1 + 3*w]

	movdqa xmm3,[r10d - 1 - r9d*2+rax]		; a[index - 1 - 3*w]
	;movdqa xmm4,[r10d + 1 + r9d*2+rax]		;a[index + 1 + 3*w]
	;movdqa xmm5,[r10d + 1 - r9d*2+rax]		;a[index + 1 - 3*w]

	;mulps xmm0,-2
	;mulps xmm1,2
	;mulps xmm2,-1
	;mulps xmm3,1
	;;mulps xmm4,-1
	;mulps xmm5,1

	;addps xmm0, xmm1
	;addps xmm2, xmm3
	;addps xmm4, xmm5

	;addps xmm0, xmm2
	;addps xmm0, xmm4

	;movaps [], xmm0
	;movaps	QWORD PTR [readyArray+r10d],xmm0

	jmp loopColums

increment:
	mov eax,r14d
	inc eax
	mov r14d,eax
	jmp loopRows

ending:
	
pop rbp
mov eax,0

ret
		
MyProc1 endp


END