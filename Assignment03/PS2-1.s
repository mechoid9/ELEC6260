;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Brian Arnberg - ELEC6260           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Problem Set 2, problem 1               ;;
;;  Execute the following:                ;;
;;    zz = aa*(bb+cc) - (dd*35);          ;;
;;    define: aa,bb,cc,dd in code area    ;;
;;            zz in data area             ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	AREA	ps2p1, CODE
			; Name this CODE block, ps2p1
	ENTRY		; Mark first instruction to execute
start			; aa*(bb+cc)
	LDR r4, =bb0	; Get address of bb0
	LDR r0, [r4]	; Store value of bb0 in r0
	LDR r4, =cc0	; Get address of cc0
	LDR r1, [r4]	; Store value of cc0 in r1
	ADD r2, r0, r1	; Compute bb0+cc0, store to r2 (r2 = bb0+cc0)
	LDR r4, =aa0	; Get address of aa0
	LDR r0, [r4]	; Store value of aa0 in r0
	MUL r2, r2, r0	; Compute aa0*(bb0+cc0), store to r2
next			; (dd*35)
	LDR r4, =dd0	; Get address of dd0
	LDR r0, [r4]	; Store value of dd0 in r0
	MUL r3, r0, #35	; Compute dd0*35, store to r3
final			; aa*(bb+cc) - (dd*35)
	SUB r2, r2, r3	; Final Computation: r2 = r2 - r3
store			; store to address of zz
	LDR r4, =zz0	; Get address of zz0
	STR r2, [r4]	; Store result to address of zz0 ( [zz0] <= r2 )
variables		; Declare variable values in code block
	aa0 dcd #1	; Declare value of aa0 
	bb0 dcd #2	; Declare value of bb0
	cc0 dcd #2	; Declare value of cc0
	dd0 dcd #2	; Declare value of dd0
	
	AREA 	data1, DATA
			; Name this DATA block, data1
	zz0 space 4	; Set space aside for variable zz0