; FUNCIONES de C
  extern malloc
  extern free
  extern strcpy
  extern tdt_agregar
  extern tdt_borrar
  
; FUNCIONES
  global tdt_crear
  global tdt_recrear
  global tdt_cantidad
  global tdt_agregarBloque
  global tdt_agregarBloques
  global tdt_borrarBloque
  global tdt_borrarBloques
  global tdt_traducir
  global tdt_traducirBloque
  global tdt_traducirBloques
  global tdt_destruir

; /** defines offsets y size **/
  %define TDT_OFFSET_IDENTIFICACION   0
  %define TDT_OFFSET_PRIMERA          8
  %define TDT_OFFSET_CANTIDAD        16
  %define TDT_SIZE                   20
  %define bl_tam					 18
  %define bl_cl 					  0
  %define bl_sg     				  3

section .text

; =====================================
; tdt* tdt_crear(char* identificacion)
tdt_crear:
	push r12
	mov r12,rdi
	mov rdi, TDT_SIZE
	call malloc
	mov [rax], r12
	mov QWORD [rax+8], 0
	mov QWORD [rax+16],0 
	pop r12
	ret

; =====================================
; void tdt_recrear(tdt** tabla, char* identificacion)
tdt_recrear:
	push rdi
	xor rax,rax
	cmp [rsi], rax
	je .borrarsinid
	push rsi
	call tdt_destruir
	pop rsi
	mov rdi, rsi
	call tdt_crear
	pop rdi
	mov [rdi], rax
	ret
.borrarsinid:
	mov r8, [rdi]
	mov rsi, [r8]
	push rsi
	call tdt_destruir
	pop rsi
	mov rdi, rsi
	call tdt_crear
	pop rdi
	mov [rdi], rax
	ret
	

; =====================================
; uint32_t tdt_cantidad(tdt* tabla)
tdt_cantidad:
	xor rax, rax
	mov eax, [rdi+TDT_OFFSET_CANTIDAD]
	ret


; =====================================
; void tdt_agregarBloque(tdt* tabla, bloque* b)
tdt_agregarBloque:
	; en rdi tengo un puntero a la tabla, en rsi tengo un puntero al bloque. 
	mov rdx, [rsi+bl_sg]
	jmp tdt_agregar

; =====================================
; void tdt_agregarBloques(tdt* tabla, bloque** b)
tdt_agregarBloques:
  push rbx
  mov rbx, 0 
  .ciclo:
  push rsi
  cmp QWORD [rsi+rbx], 0
  je .fin
  mov rsi, [rsi+rbx]
  sub rsp, 8
  call tdt_agregarBloque
  add rsp, 8
  pop rsi
  add rbx, 8
  jmp .ciclo
  .fin:
  pop rsi
  pop rbx
  ret

; =====================================
; void tdt_borrarBloque(tdt* tabla, bloque* b)
tdt_borrarBloque:
	JMP tdt_borrar
		
; =====================================
; void tdt_borrarBloques(tdt* tabla, bloque** b)
tdt_borrarBloques:
  push rbx
	mov rbx, 0 
  .ciclo:
	push rsi
	cmp QWORD [rsi+rbx], 0
	je .fin
	mov rsi, [rsi+rbx]
	sub rsp, 8
	call tdt_borrarBloque
	add rsp, 8
	pop rsi
	add rbx, 8
	jmp .ciclo
	.fin:
  pop rsi
	pop rbx
	ret

		
; =====================================
; void tdt_traducir(tdt* tabla, uint8_t* clave, uint8_t* valor)
tdt_traducir:
	xor rax, rax
	mov al, [rsi]
	push rdx
	xor rdx, rdx
	mov dl, [rsi+1]
	xor rcx,rcx
	mov cl, [rsi+2]
	cmp QWORD [rdi+8], 0
	je .fin;chequeo si hay primera
	mov r10, [rdi+8]; pongo en r10 la dir de primera
	cmp QWORD [r10+rax], 0;chequeo si hay segunda
	je .fin
	mov r11, [r10+rax];pongo en r11 la dir de segunda
	cmp QWORD [r11+rdx], 0 ;chequeo si hay tercera
	je .fin
	mov r9, [r11+rdx]; pongo en r9 la dir de la tercera
	cmp QWORD [r12+rcx+16], 0; chequeo si hay valid
	je .fin
	pop rdx
	mov rdx,  [r12+rcx+16] ; pongo en rdx la dir del valor
	jmp .f
	.fin:
	pop rdx
	.f:
	ret
		
; =====================================
; void tdt_traducirBloque(tdt* tabla, bloque* b)
tdt_traducirBloque:
	mov rdx, [rsi+bl_sg]
	call tdt_traducir
	mov [rsi+bl_sg], rdx
	ret

; =====================================
; void tdt_traducirBloques(tdt* tabla, bloque** b)
tdt_traducirBloques:
  push rbx
  mov rbx, 0 
  .ciclo:
  push rsi
  cmp QWORD [rsi+rbx], 0
  je .fin
  mov rsi, [rsi+rbx]
  sub rsp, 8
  call tdt_traducirBloque
  add rsp, 8
  pop rsi
  add rbx, 8
  jmp .ciclo
  .fin:
  pop rsi
  pop rbx
  ret
		
; =====================================
; void tdt_destruir(tdt** tabla)
tdt_destruir:
	push rdi
	push r12
	push r13
	mov r10, [rdi]; pongo en r10 la dirección de la tabla
	mov r11, [r10+8]; pongo en r11 la direccíón de primera
	xor rcx, rcx;vacio rcx para usarlo de contador de primera
	xor rax, rax; vacio rax para usarlo de contador de segunda
	xor r12, r12;vacío r12 para usarlo para las direcciones que quizas tenga que liberar en primera
	xor r13, r13; vacío r13 para usarlo para las direcciones que quizas tenda que liberar en segunda
	cmp r11, 0
	JE .fin
	.cicloprim:
		xor rax, rax; limpio rax por si la tengo que usar en ciclo sec
		mov r12, [r11+rcx*8];paso a r12 la dirección de una segunda
		cmp r12, 0
		jne .ciclosec
	.limpiesec:
		push rcx
		push rax
		push r13
		push r12
		push r11
		push r10
		push rdi
		mov rdi, r12
		call free
		pop rdi
		pop r10
		pop r11
		pop r12
		pop r13
		pop rax
		pop rcx
		mov r12,0
		inc rcx
		cmp rcx, 256 
		jne .cicloprim
		mov rdi, r11
		call free
		mov r11, 0
		jmp .fin

	.ciclosec:
		mov r13, [r12+rax*8]
		cmp r13, 0
		jne .limpiarsec
	.vuelta:
		inc rax
		cmp rax, 256
		jne .ciclosec
		jmp .limpiesec
	.limpiarsec:
		push rcx
		push rax
		push r13
		push r12
		push r11
		push r10
		push rdi
		mov rdi, r13
		call free
		pop rdi
		pop r10
		pop r11
		pop r12
		pop r13
		pop rax
		pop rcx
		mov r13,0
		jmp .vuelta
	.fin:
		pop r13
		pop r12
		mov rdi, r10
		call free ;(sólo poner si en crear la str inicial la pongo en memoria flotante)
		pop rdi
		ret


