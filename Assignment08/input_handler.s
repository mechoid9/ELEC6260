;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
;;   Brian Arnberg                       ;;
;;   Problem Set #5 - Input Handler      ;;
;;    input_handler.s                    ;;
;;     - tests the user button           ;;
;;     - sets a global variable          ;;
;;     - called each time the main       ;;
;;       loop is executed                ;;
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
;;                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GPIOA	EQU GPIOA_IDR

	AREA INPUT, CODE
	EXPORT input_handler

input_handler		;; Test for user button
	;; load value of button
	LDR r1, GPIOA	;; load address of PORTA
	LDR r0, [r1]	;; r0 = PORTA
	;; cmp value to zero
	TST r0, #0	;; if (r0 && 0) 
	BEQ exit	;; then exit
	;; if not zero (pressed), increment step
	LDR r1, =state	;; load the address of state
	LDR r0, [r1]	;; r0 = state
	ADD r0, r0, #1	;; increment state
	CMP r0, #4	;; if (r0 == 4)
	LDREQ r0, #1	;; then r0 = 1	
	STR r0, [r1]	;; state = r0
	;; then indicate the button was pressed
	LDR r1, =pressed;; load the address of pressed
	MOV r0, #1	;; set r0 to 1
	LDR r0, [r1]	;; store 1 to pressed
exit
	BX r14		;; return from input_handler
	END	

