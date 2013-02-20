;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   Brian Arnberg                       ;;
;;   Problem Set #5 - Output Handler     ;;
;;    output_handler.s                   ;;
;;     - writes patterns to the LEDS     ;;
;;     - called by system tick timer     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     - takes no arguments              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     - global variables                ;;
;;        - state (represents a pattern) ;;
;;          1 - do nothing (initial)     ;;
;;          2 - counter-clockwise pattern;;
;;          3 - clockwise pattern        ;;
;;          4 - return to (1)            ;;
;;        - pressed (only reset here)    ;;
;;          1 - it was pressed           ;;
;;          0 - it was not pressed       ;;
;;        - step ( 0,4,8,12,16)          ;;
;;           indicates what to do        ;;
;;                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GPIOD	EQU 0x40020C00		;; general purpose I/O, port D
BSRRL	EQU 0x18		;; port D set
BSRRH	EQU 0x1A		;; port D reset

	AREA OUTPUT, CODE
	EXPORT output_handler

output_handler
	LDR r3, =step		;; get address of step
	LDR r2, [r3]		;; r2 = step
	LDR r1, =pressed	;; get address of pressed
	LDR r0, [r1]		;; r0 = pressed
	CMP r0, #1		;; if (pressed != 1)
	BNE step_check	
	MOV r2, #0		;; then reset step
	MOV r0, #0		;; then reset pressed (to leave it alone)
	MOV r0, [r1]		;; and store the value of pressed

step_check	;; check the step
	CMP r2, #16		;; if (step == 5)
	MOVEQ r2, #0		;;  reset step
	BEQ clear_leds		;;  clear_leds
				;; else
state_check	;; check the state
	LDR r1, =state		;; get address of state
	LDR r0, [r1]		;; r0 = state 
	CMP r0, #2		;; compare state to #2
	BEQ counter_clockwise	;; if (r0 == 2), counter_clockwise
	BGT clockwise 		;; else if (r0 > 2), clockwise
				;; else, clear_leds
clear_leds	;; clear the LEDS
	MOV r4, #1		;; make r4 = 0x0001
	MOV r5, #12		;; make r5 = 0x000C	
loop_clear
	MOV r4, r4, lsl r5	;; bit shift r4 by r5
	LDR r6, =GPIOD		;; address of GPIOD
	STR r4, [r6, #BSRRH]	;; reset the pin
	MOV r4, r4, lsl #1	;; bit shift r4 once
	CMP r4, #15		;; if (r4 <= 15) 
	BLE loop_clear		;; then branch to loop_clear
	B exit			;; branch to exit

counter_clockwise
	MOV r4, #1		;; make r4 = 0x0001
	LDR r7, =ccw		;; r7 points to ccw
	LDR r5, [r7, r2]	;; r5 is the step value for ccw	
	MOV r4, r4, lsl r5	;; bit shift r4 by r5
	LDR r6, =GPIOD		;; address of GPIOD
	STR r4, [r6, #BSRRL]	;; set the pin
	ADD r2, r2, #4		;; increment step
	B exit			;; branch to exit

clockwise
	MOV r4, #1		;; make r4 = 0x0001
	LDR r7, =cw		;; r7 points to ccw
	LDR r5, [r7, r2]	;; r5 is the step value for ccw	
	MOV r4, r4, lsl r5	;; bit shift r4 by r5
	LDR r6, =GPIOD		;; address of GPIOD
	STR r4, [r6, #BSRRL]	;; set the pin
	ADD r2, r2, #4		;; increment step
	B exit			;; branch to exit

exit
	STR r2, [r3]		;; store the final step value
	BX r14			;; return from output handler
	;; define steps
ccw	DCD	13, 12, 15, 14	;; steps for counter-clockwise
cw	DCD	13, 14, 15, 12	;; steps for clockwise
	;; store variables
	AREA data1, DATA
step	SPACE 4			;; set space aside to store step
	END
