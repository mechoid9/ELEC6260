;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Brian Arnberg                                    ;;
;; Midterm - Problem 4                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; 8-bit: STATUS 0xE0000001                         ;;
;;        DATA   0xE0000000                         ;;
;;        ARRAY  0x40000000 (200 bytes long)        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
DATA 	EQU 0xE0000000
STATUS 	EQU 0xE0000001
ARRAY	EQU 0x40000000

	AREA CONVERT,CODE
	EXPORT convert
convert
	LDR r0, =STATUS		;; address of STATUS byte
	LDR r1, =DATA		;; address of DATA byte
	LDR r2, =ARRAY		;; initial address of ARRAY
	LDR r5, =ARRAY
	ADD r5, r5, #200	;; calculate end of ARRAY

again	LDRB r3, [r2, #1]!	;; load byte value from ARRAY, increment
	STRB r3, [r1]		;; store byte to DATA for conversion
loop	LDRB r4, [r0]		;; load STATUS into r4
	TST r4, #0x80		;; test leftmost bit of STATUS
	BEQ loop		;; if leftmost bit of STATUS is 1, loop
	CMP r2, r5		;; if (r2 < r5) ; (at the end of the array
	BLT again		;; loop back to ``again''
	BX r14			;; return from subroutine
	END
