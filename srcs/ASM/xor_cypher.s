section .text
	global xor_cypher

; void xor_cypher(char *data, int size, char *key);
; xor_cypher(rdi data, rsi size, rdx key);
xor_cypher:
	push rdx 				; save key pointer to reset it when end is reached

	jmp xor_loop

    xor_loop:
        cmp rsi, 0			; Check if size is 0
        je xor_end

        mov al, [rdi]      ; Load current byte of data into al
        mov bl, [rdx]      ; Load current byte of key into bl
        xor al, bl         ; XOR operation
        mov [rdi], al      ; Store the result back in data

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

        ret