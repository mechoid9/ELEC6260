;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Brian Arnberg - ELEC6260           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Problem Set 2, problem 2           ;;
;; Compute the following:                 ;;
;;	if ((mm - nn) < 15) {             ;;
;;		kk = jj - 5;              ;;
;;		xx = 0;                   ;;
;;	} else {                          ;; 
;;		kk = cc + 18;             ;;
;;		xx = 1;                   ;;
;;	}                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Test both cases (TRUE vs FALSE)        ;;
;;     cc = 2, jj = 10                    ;;
;;  TRUE                                  ;;
;;    mm = 30, nn = 20                    ;;
;;     kk = 5, xx = 0                     ;;
;;  FALSE                                 ;;
;;    mm = 30, nn = 10                    ;;
;;     kk = 20, xx = 1                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Define the CODE block
	AREA	RESET, CODE
	; Begin
	ENTRY		; Define ENTRY location
	; Test the condition
	LDR r4, =mm0	; Get Address for mm0
	LDR r0, [r4]	; Store value of mm0 to r0
	LDR r4, =nn0	; Get Address for nn0
	LDR r1, [r4]	; Store value of nn0 to r1
	SUB r0, r0, r1  ; Compute mm-nn, store to r0
	MOV r1, #15	; Put #15 in r1
	CMP r0, r1	; Compare (mm-nn) < 15
	BLT true	; if true, branch to true
false	; False Block
	LDR r4, =cc	; Get Address for cc0
	LDR r0, [r4]	; Store to r0
	MOV r1, #18	; Set r1 to #18
	ADD r0, r0, r1	; r0 = cc + 18
	LDR r4, =kk0	; Get address of kk
	STR r0, [r4] 	; Store r0 to kk0
	MOV r1, #1	; Put #1 in r1
	LDR r4, =xx0	; Get address of xx0
	STR r1, [r4] 	; Store r1 to xx0
	B m		; Branch to m (skip over true block)
true	; True Block
	LDR r4, =jj0	; Get address for jj0
	LDR r0, [r4]	; Store value to r0
	MOV r1, #5	; Put #5 in r1
	SUB r0, r0, r1	; r0 = jj - #5
	LDR r4, =kk0	; Get address of kk
	STR r0, [r4] 	; Store r0 to kk0
	MOV r1, #0	; Put #0 in r1
	LDR r4, =xx0	; Get address of xx0
	STR r1, [r4] 	; Store r1 to xx0
m	B m  		; Branch to m, never end
	; Define the value of the 4 input variables
mm0	DCD 0x1E	; mm = 30
nn0	DCD 0x14	; nn = 20
jj0	DCD 0x0A	; jj = 10
cc0	DCD 0x02	; cc = 2

	; Define the DATA block
	AREA	data1, DATA
	; Set space for variables
kk0	SPACE 4		; Set 4 bytes aside for kk
xx0	SPACE 4		; Set 4 bytes aside for xx

	END		; End the program
