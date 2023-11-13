bits 64
default rel
global _start

_start:
		; save registers
		push rax
		push rdi
		push rsi
		push rdx
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
		mov rdi, rsp
		add rdi, text_offset		;rdi = .text address
		mov rsi, [rel data_len]		;rsi = .data size
		mov rdx, [rel key]			;rdx = key address
		push rdx 
		jmp xor_loop

		xor_loop:
			cmp rsi, 0			; Check if size is 0
			je xor_end

			mov rax, [rdi]      ; Load current byte of data into al
			mov rbx, [rdx]      ; Load current byte of key into bl
			xor rax, rbx        ; XOR operation
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
			pop rdx
			pop rsi
			pop rdi
			pop rax
			
			jmp 0x000000


msg     	db "..WOODY..",10
key     	db "XXXXXXXXXXXXXXXX",0	
text_offset dq 424242424242424242
data_len 	dq 212121212121212121