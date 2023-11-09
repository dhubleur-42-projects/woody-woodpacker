bits 64
default rel
global _start

_start:
		; save registers
		push rax
		push rdi
		push rsi
		push rdx
		push rax
		push rbx

		; print message
        xor     eax, eax
        cdq
        mov     dl, 10
        inc     eax
        mov     edi, eax
        lea     rsi, [rel msg]
        syscall

		; uncipher
		mov rdi, 0x000000 			;to replace by the data addr
		mov rsi, 000000  				;to replace by the data len
		mov rdx, [rel key] 		;to replace by the key addr
		push rdx 				; save key pointer to reset it when end is reached
		jmp xor_loop

		xor_loop:
			cmp rsi, 0			; Check if size is 0
			je xor_end

			mov rax, [rdi]      ; Load current byte of data into al
			mov rbx, [rdx]      ; Load current byte of key into bl
			xor rax, rbx         ; XOR operation
			mov [rdi], rax      ; Store the result back in data

			inc rdi				; Increment data pointer
			inc rdx				; Increment key pointer

			dec rsi				; Decrement size

			cmp byte [rdx], 0  ; Check if the current byte of key is '\0'
			je reset_key

			jmp xor_loop

		reset_key:
			mov rdx, [rsp] 

			jmp xor_loop

		xor_end:
			pop rdx
			
			pop rbx
			pop rax
			pop rdx
			pop rsi
			pop rdi
			pop rax
			jmp 0x000000

; store the messaghe and the key
msg     db "..WOODY..",10
key     db "XXXXXXXXXXXXXXXX",0	;to replace by the key
