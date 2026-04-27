; Example source code for a spin lock.
;
; The source code is included as-is with no warranty.
;

; nasm directives to use x64 code and relative
; addressing
	use64
	default rel

	mov		ecx, 1			; the lock value
	xor		edx, edx		; number of retries
	lea		rsi, spinlock	; address of the spin lock

tryagain:
	xor		eax, eax		; comparison value
	lock cmpxchg [rsi], ecx	; atomic compare exchange
	jz		locked			; got the lock

	inc		edx				; can still retry?
	cmp		edx, 3
	jnl		longer			; no - take the long lock path

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
spinlock:
	dw 0	

