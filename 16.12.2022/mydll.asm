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
	mov ebx,3
	mul ebx				;w*3
	mov ebx,4			
	div ebx
	mov ebx,edx			;(w*3)%4
	mov eax,4
	sub eax,ebx			;(4 - (w * 3) % 4)
	mov ebx,4
	div ebx
	mov eax,edx			;(4 - (w * 3) % 4) % 4
	mov r12d,eax		;padding

	mov r10d,0			;index

	mov eax,r14d
	mov ebx,r15d
	add eax,ebx
	mov r15d,eax		;position+ileBit

loopRows:
	mov ecx,r14d		;y
	mov edx,r15d
	cmp ecx,edx
	jae ending
	mov eax,heightI
	dec eax
	mov ebx,r14d
	cmp eax,ebx
	je ending

loopColums:
	mov r8d,1			;x
	mov eax,widthI
	dec eax
	cmp r8d,eax
	jae increment		;next row
	mov eax,r8d
	inc eax

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