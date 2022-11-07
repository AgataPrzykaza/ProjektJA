.DATA

arrayStart QWORD 0		; pointer to the read array
readyArray QWORD 0			; pointer for the modified array
heightI DWORD 0			; the height of the image
widthI DWORD 0			; the width of the image

.CODE

MyProc1 proc

	mov arrayStart, RCX
	mov readyArray, RDX
	mov heightI, R8d
	mov widthI, R9d
	
	;mov r12d, DWORD PTR[rbp + 32]
	
	mov R8d,5
	
	;mov ala,RCX
	;mov rcx,5
	mov eax,5

	ret
MyProc1 endp
END