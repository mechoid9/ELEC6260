;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Brian Arnberg                                    ;;
;; Midterm - Problem 3                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; for (I=0;I<300;I++)                              ;;
;;   M[I+1] = (N[I]*23.25) + M[I];                  ;;
;; ; all numbers are Q28.4 format                   ;;
;; ; M and N are already populated                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	AREA variables,DATA
M	SPACE 4*300	;300 32-bit words
N 	SPACE 4*300	;300 32-bit words
I	SPACE 4		;1 32-bit word
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	AREA for_loop,CODE,READONLY
	LDR r8, =M		;; initial address for M
	LDR r9, =N		;; initial address for N
	LDR r7, =I		;; address for I 
	MOV r2, #0		;; initialize I
loop
	LDR r4, [r8], #4	;; M: load r4 from r8, then index r8
	LDR r5, [r9], #4	;; N: load r5 from r9, then index r9
	MOV r3, #0x174		;; 23.25 = 0x174 in Q28.4
	MUL r5, r5, r3		;; N[I]*23.5
	LSR r5, #4		;; shift right 4 bits
	AND r4, r5		;; calculate result
	STR r4, [r8]		;; store to M[I+1]
	ADD r2, r2, #1		;; I = I + 1
	CMP r2, 300		;; if (I < 300)
	BLT loop		;; branch to ``loop''
	STR r2, [r7]		;; store I
h	B h			;; end
	END
