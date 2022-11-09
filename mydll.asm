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
	
	
	
	mov R8d,5
	
	;mov ala,RCX
	;mov rcx,5
	mov rax,[rsp+16+16+4]

	ret
MyProc1 endp
END