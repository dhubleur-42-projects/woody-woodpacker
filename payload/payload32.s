bits 32
default rel
global _start

_start:
		; save registers
		push eax
		push ebx
		push ecx
		push edx

		; print message
		cdq
		mov dl, 10
		mov ebx, 1
		lea ecx, [rel msg]
		mov eax, 4
		int 0x80

		; uncipher
		lea eax, [ecx + (_start - msg)]         	; payload address
		sub eax, [ecx + (text_offset - msg)]    	; eax = .text address
		mov ebx, [rel data_len]                     ; ebx = .text size
		lea ecx, [rel key]                          ; ecx = key address

		push ecx
		jmp xor_loop

		xor_loop:
			cmp ebx, 0           ; Check if size is 0
			je xor_end

			mov al, [eax]        ; Load current byte of data
			mov bl, [ecx]        ; Load current byte of key
			xor al, bl           ; XOR operation
			mov [eax], al        ; Store the result back in data

			inc eax              ; Increment data pointer
			inc ecx              ; Increment key pointer

			dec ebx              ; Decrement size

			cmp byte [ecx], 0    ; Check if the current byte of key is '\0'
			je reset_key

			jmp xor_loop

		reset_key:
			mov ecx, [esp] 

			jmp xor_loop

		xor_end:
			pop ecx

			pop edx
			pop ecx
			pop ebx
			pop eax

			jmp 0x00000000


msg     	db "..WOODY..",10
key     	db "XXXXXXXXXXXXXXXX",0	
text_offset dd 0x00000000
data_len 	dd 0x00000000