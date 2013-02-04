;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Brian Arnberg - ELEC6260           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Problem Set 3, Problem 1           ;;
;; Compute the following:                 ;;
;;  Z = (a0*x0) - (a1*x1)                 ;;
;;   Using a subroutine.                  ;;
;; The 'main' will call the subroutine    ;;
;;  twice, each time passing all 5        ;;
;;  variables with registers.             ;;
;; Z will be passed as an ADDRESS         ;;
;; x0,x1 are 32bit integers               ;;
;; a0,a1,Z using Q24.8 format             ;;
;;                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Define the CODE block
	AREA	RESET, CODE
	; Begin
	ENTRY
main	LDR r13, =StackEnd
	LDR r0, =a0_1		; get a0_1
	LDR r4, [r0]		;  r4 = a0_1
	LDR r0, =x0_1		; get x0_1
	LDR r5, [r0]    	;  r5 = x0_1
	LDR r0, =a1_1		; get a1_1
	LDR r6, [r0]    	;  r6 = a1_1
	LDR r0, =x1_1		; get x1_1
	LDR r7, [r0]    	;  r7 = x1_1
	LDR r8, =z1		; get address for z1
	; Load variables into stack
	STR r8, [r13, #-4]!	; PUSH =z1 to stack
	STR r7, [r13, #-4]!	; PUSH x1 to stack
	STR r6, [r13, #-4]!	; PUSH a1 to stack 
	STR r5, [r13, #-4]!	; PUSH x0 to stack
	STR r4, [r13, #-4]! 	; PUSH a0 to stack
	; Branch to sub-routine (for first run)
	BL subroutine	; Branch to subroutine
	; Return from subroutine, get new values
	LDR r0, =a0_2		; get a0_2
	LDR r4, [r0]		;  r4 = a0_2
	LDR r0, =x0_2		; get x0_2
	LDR r5, [r0]		;  r5 = x0_2
	LDR r0, =a1_2		; get a1_2
	LDR r6, [r0]		;  r6 = a1_2
	LDR r0, =x1_2		; get x0_2
	LDR r7, [r0]		;  r7 = x1_2
	LDR r8, =z2		; get address for z1
	; Load variables into stack
	STR r8, [r13, #-4]!	; PUSH =z1 to stack
	STR r7, [r13, #-4]!	; PUSH x1 to stack
	STR r6, [r13, #-4]!	; PUSH a1 to stack 
	STR r5, [r13, #-4]!	; PUSH x0 to stack
	STR r4, [r13, #-4]! 	; PUSH a0 to stack
	BL subroutine	; Branch to subroutine
halt	B halt		; branch to self, halt program
	; Define subroutine
subroutine
	LDR r4, [r13], #4	; PULL a0
	LDR r5, [r13], #4	; PULL x0
	LDR r6, [r13], #4   	; PULL a1
	LDR r7, [r13], #4	; PULL x1
	LDR r8, [r13], #4	; PULL =z
	LSL r5, #8       	; Shift x0 8 bits left
	LSL r7, #8		; Shift x1 8 bits left
	MUL r1, r4, r5		; a0 * x0(shifted)
	LSR r1, #8		; Shift the result 8 bits right
	MUL r2, r6, r7		; a1 * x1(shifted)	
	LSR r2, #8		; Shift the result 8 bits right
	SUB r0, r1, r2		; Find the difference
	STR r0, [r8]		; Store the result
	BX r14		; Return from subroutine
	; Define the variables
a0_1	DCD 0x0540	; 5.25
a0_2	DCD 0x0380	; 3.5
a1_1	DCD 0x06c0	; 6.75
a1_2	DCD 0x0420	; 4.125
x0_1	DCD 200
x0_2	DCD 300
x1_1	DCD 100
x1_2	DCD 200
	; Define the  DATA block
	AREA	data1, DATA
	; Set aside stack area
Stack 	SPACE 20		; Make stack 5 words long
StackEnd
	; Set space aside for variables
z1	SPACE 4
z2	SPACE 4
	END
