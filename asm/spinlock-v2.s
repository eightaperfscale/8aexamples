; Example source code of a spin lock code with some code alignment
; and performance improvements.
;
; The source code is included as-is with no warranty.
;

; nasm directives to use x64 code and relative
; addressing
	use64
	default rel

	mov		ecx, 1			; the lock value
	mov		dl, 3			; number of retries
	lea		rsi, spinlock	; address of the spin lock

	align(16)
tryagain:
	xor		eax, eax		; comparison value
	lock cmpxchg [rsi], ecx	; atomic compare exchange
	jz		locked			; got the lock

	sub		dl, 1			; can still retry?
	jz		longer			; no - take the long lock path

	pause					; wait a bit
	jmp		tryagain		; try the lock again

longer:
	call	longerway		; use the longer lock mechanism
	ret						; done!

locked:
	call	codeunderlock	; continue processing with lock held
	ret						; done!

; Dummy function calls
longerway:
codeunderlock:
	ret

; Spin lock value
align(64)
spinlock:
	dw 0	

