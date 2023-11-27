bits 32
default rel
global _start

_start:
		; save registers
		push eax
		push edi
		push edx
		push ebx
		push ecx

		; print message
		xor eax, eax
		cdq
		mov dl, 10
		inc eax
		mov ebx, 1
		lea ecx, [rel msg]
		mov eax, 4
		int 0x80

		; uncipher
		lea edi, [ecx + (_start - msg)]         ; payload address
		sub edi, [ecx + (text_offset - msg)]    ; edi = .text address
		mov ecx, [data_len]                     ; ecx = .text size
		lea edx, [key]                          ; edx = key address

		push edx
		jmp xor_loop

		xor_loop:
			cmp ecx, 0           ; Check if size is 0
			je xor_end

			mov al, [edi]        ; Load current byte of data
			mov bl, [edx]        ; Load current byte of key
			xor al, bl           ; XOR operation
			mov [edi], al        ; Store the result back in data

			inc edi              ; Increment data pointer
			inc edx              ; Increment key pointer

			dec ecx              ; Decrement size

			cmp byte [edx], 0    ; Check if the current byte of key is '\0'
			je reset_key

			jmp xor_loop

		reset_key:
			pop edx

			jmp xor_loop

		xor_end:
			pop ebx

			pop ecx
			pop ebx
			pop edx
			pop edi
			pop eax

			jmp 0x00000000


msg     	db "..WOODY..",10
key     	db "XXXXXXXXXXXXXXXX",0	
text_offset dd 0x00000000
data_len 	dd 0x00000000