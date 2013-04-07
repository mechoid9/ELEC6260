;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
;;   Brian Arnberg                       ;;
;;   Problem Set #7 - Input Handler      ;;
;;    input_handler.s                    ;;
;;     - tests the user button           ;;
;;     - sets a global variable          ;;
;;     - triggered by a button press     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     - global variables                ;;
;;        - state (represents a pattern) ;;
;;          1 - do nothing (initial)     ;;
;;          2 - counter-clockwise pattern;;
;;          3 - clockwise pattern        ;;
;;          4 - return to (1)            ;;
;;        - pressed (only set here)      ;;
;;          1 - it was pressed           ;;
;;          0 - it was not pressed       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GPIOA	EQU 0x40020000
IDR	EQU 0x10

	AREA INPUT, CODE
	EXPORT input_handler
	IMPORT state
	IMPORT pressed

input_handler		;; Test for user button
	;; if pressed, go on and leave
	LDR r3, =pressed	;; load address of pressed
	LDR r2, [r3] 		;; load pressed to r2
	CMP r2, #1	;; if (r2 == 1)
	BEQ exit	;; then exit (don't mess with stuff)
loop 	;; while button is pressed
	;; load value of button
	LDR r1, =GPIOA		;; load address of PORTA
	LDR r0, [r1, #IDR]	;; r0 = PORTA
	AND r0, r0, #0x01	;; only look at last bit
	;; cmp value to zero
	CMP r0, #1	;; if (r0 < 1) 
	BLT exit	;; then exit
	CMP r2, #1	;; if (pressed == 1), it was set, continue to loop
	BEQ loop
	;; else increment step
	LDR r1, =state	;; load the address of state
	LDR r0, [r1]	;; r0 = state
	ADD r0, r0, #1	;; increment state
	CMP r0, #4	;; if (r0 == 4)
	MOVEQ r0, #1	;; then r0 = 1	
	STR r0, [r1]	;; state = r0
	;; then indicate the button was pressed
	MOV r2, #1	;; set r0 to 1
	STR r2, [r3]	;; store 1 to pressed
	B loop		;; remain in while loop
exit
	BX r14		;; return from input_handler
	ALIGN
	END	

