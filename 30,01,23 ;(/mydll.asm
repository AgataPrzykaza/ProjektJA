.DATA

arrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
heightI DWORD 0			; the height of the image
widthI DWORD 0			; the width of the image



.CODE

MyProc1 proc

;get data ----------------------------------------------------

	push rbp
	mov  rbp, rsp


	mov arrayStart, RCX			;tablica poczatkowa
	mov readyArray, RDX			;tablica wyjsciowa
	mov heightI, R8d			;wysokosc
	mov widthI, R9d				;szerokosc
	
	mov eax, DWORD PTR[rbp + 48]
	mov r15d,eax				;   Number of rows
	mov eax,DWORD PTR[rbp+56]
	mov r14d,eax				;	Position is stored 
	
	push r11
	push r10
	push r15
	push r14
	push r13
	push r12					;zapamietanie rejestrow
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

	mov eax,r14d		;pozycja
	mov ecx,r15d		;ile wierszy
	add eax,ecx
	mov r15d,eax		;position+ilewierszy

	mov eax,widthI
	add eax,widthI
	add eax,widthI
	mov r13d,eax		;w*3

loopRows:
	mov ecx,r14d		;y
	mov edx,r15d		;position+ileBit
	cmp ecx,edx			;czy koniec wierszy
	jae ending			;next koniec
	mov eax,heightI
	dec eax
	mov edx,r14d
	cmp eax,edx			;czy ostatni wiersz
	je ending			;next koniec



	mov r8d,1			;x


loopColums:
	mov eax,widthI
	dec eax
	cmp r8d,eax			;czy koniec kolumny
	jae increment		;next row


;liczenie indexu___________________________________
	mov eax,r14d		
	mul r13d
	mov r10d,eax		;y*iloscWiersza
	mov eax,r8d
	mov ecx,3
	mul ecx
	mov ecx,eax
	mov eax,r10d
	add eax,ecx
	mov r10d,eax		;y * iloscWiersza + x * 3
	mov eax,r14d
	mul r12d
	mov ecx,eax
	mov eax,r10d
	add ecx,eax
	mov r10d,eax		; r10=y * iloscWiersza + x * 3 +padding*y
;______________________________________________________
	
	mov rax,arrayStart
	add rax,r10
	mov rcx,rax					;startarraay +index
	

	pmovzxbw xmm0,[rcx+r13]		;a[index + 3*w]
	pmovzxbw xmm10,[rcx + r13]
	pmovzxbw xmm4,[rcx+r13+3]		;a[index + 1 + 3*w]
	pmovzxbw xmm2,[rcx+r13-3]		;a[index - 1 + 3*w]
	mov rax,rcx
	sub rax,r13
	pmovzxbw xmm1,[rax]			;a[index - 3*w]
	
	pmovzxbw xmm3,[rax +3]		; a[index - 1 - 3*w]
	pmovzxbw xmm5,[rax -3]		;a[index + 1 - 3*w]


	PADDW 	 xmm0,xmm0				;a[index + 3*w]*2
	

	PADDW 	 xmm10,xmm10				

	PSUBW  xmm0,xmm10		
	PSUBW  xmm0,xmm10		;*-2
	PSUBW  xmm0,xmm4			;-1
	PSUBW  xmm0,xmm2			;-1

	PADDW 	 xmm1,xmm1			;*2
	PADDW 	 xmm1,xmm3			;1
	PADDW 	 xmm1,xmm5			;1
	PADDW 	 xmm0,xmm1			;wynik=ujemne+dodatnie

	
	
	
	packuswb xmm0, xmm0
	;packuswb xmm0, xmm0
	
	mov rax,readyArray
	add rax,r10


	movdqu [rax],xmm0	;zapis do tablicy wyjsciowej

	mov eax,r8d
	inc eax				
	
	mov r8d,eax			;zapis kolumny
	jmp loopColums

increment:
	mov eax,r14d		
	inc eax
	
	mov r14d,eax		;zwiekszenie wiersza
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