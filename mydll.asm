.DATA

arrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
heightI DWORD 0			; the height of the image
widthI DWORD 0			; the width of the image

v1 DQ 1.0,2.0,1.0

.CODE

MyProc1 proc
	
	


	mov arrayStart, RCX
	mov readyArray, RDX
	mov heightI, R8d
	mov widthI, R9d
	
      

     

		 push    rbp
       mov     rbp, rsp
		  
		

		
		mov eax, DWORD PTR[rbp + 48]
		mov r15d,eax
		mov eax,DWORD PTR[rbp+56]
		mov r14d,eax
		mov eax,heightI
		

	mov r10, 0000FFFF0000FFFFh
	movq xmm10, r10	
	

	mov r10,000100020001h
	movq xmm0,r10

	mov r10,0FFFFFFF0000FFFFh
	pinsrq xmm0,r10,1



	
	
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