.DATA

arrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
heightI DWORD 0			; the height of the image
widthI DWORD 0			; the width of the image
index DWORD 0
v dd 1,2,1

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
	mov r15d,eax				;   Number of bits
	mov eax,DWORD PTR[rbp+56]
	mov r14d,eax				;	Position is stored 
	
;-----------------------------------------------------------		  
		

; Initialisation


	mov rsi, arrayStart
	mov rdi, readyArray

	mov ecx, r14d				; Counter for loop

	;Mask-------------------------------------

	;-----------------------------------------

	mov r11d, heightI
	dec r11d					; r11d = imageHeight-1

	mov r12d, widthI
	dec r12d;					; r12d = imageWidth-1

	mov r9d, ecx				; use r9 for end of loop
	add r9d, r15d				; position + ilebit

	mov eax,r14d				;get position
	lea eax,[2*eax+eax]			; positon*3
	mov index,eax				; store in index
	
	

	;Calculate the current row and column
	mov eax, ecx	; save the cunter into eax
	xor edx,edx		; clean edx for division
	div widthI		; div to get modulo in edx.
	mov eax, ecx	; save the counter again
	sub eax, edx
	mov r10d, edx	; save the modulo for later
	xor edx,edx		; clean edx for division
	div widthI		; finally row is saved in eax =(i-i%w)/w


	

	; eax - row counter
	; r10d - column counter



	cmp eax, 0					;if(row ==0)
	jnz Initial
	mov ebx, widthI
	sub ebx, r10d
	inc ebx
	add ecx, ebx				; ecx += width-column+1
	mov r10d, 1					; column = 1
	inc eax						; row++

Initial:
	cmp r10d, 0
	jnz forloop
	inc r10d					;if(col==0) col=1

forloop:

	cmp r10d, r12d		
	jnz Check					; col != imageWidth-1 -> jump
	add ecx, 2
	mov r10d, 1
	inc eax						;ecx+=2, col = 1, row++

Check:

	cmp eax, r11d
	;jge FunctionEnd				; row >= imageHeight-1

	cmp ecx, r9d			
	;jge FunctionEnd				;i >= bytesToCalculate

	cmp r10d, r13d		
	jnz CalculationStart		; col != imageWidth-2 -> jump
	dec ecx
	dec r10d


CalculationStart:
		
		
		



	;mov r10, 0000FFFF0000FFFFh
	;movq xmm10, r10	
	

	;mov r10,000100020001h
	;movq xmm0,r10

	;mov r10,0FFFFFFF0000FFFFh
	;pinsrq xmm0,r10,1

	;mov r8d,1000
	;mov  rax, rcx
	;movd xmm1,QWORD PTR[rax]
	;movd xmm1, DWORD PTR[rcx+r8]
	
	;movss xmm0,[v]


	;pinsrd xmm0,[v],1
	;movss xmm10,[v] 
	;pinsrd xmm0,[v],2
	


		pop     rbp
        ret
		
MyProc1 endp

Macierz proc

	ret
Macierz endp

		;mov     rax, rdx
        ;mov     QWORD PTR [rax],0

        ;add     rax, 1
        ;mov     QWORD PTR [rax], 255

		;add     rax, 1
        ;mov     QWORD PTR [rax], 255

END