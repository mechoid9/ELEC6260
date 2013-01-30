;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Brian Arnberg - ELEC6260           ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;     Problem Set 2, problem 2           ;;
;; Implement the following:               ;;
;;   for(i=0; i<15; i++)                  ;;
;;       zz[i] = aa[i] - bb[i] + 5;       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   aa = 10*i                            ;;
;;   bb = 1*i                             ;;
;;                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Define the CODE block
	AREA	RESET, CODE
	; Begin
	ENTRY		; Define ENTRY location
	; Initialize loop
	MOV r0, #0	; r0 = I; initialize to 0
	MOV r1, #15	; r1 = 15 (use for end condition)
	LDR r2, =zz0	; r2 will point to zz[i]
	LDR r3, =aa0	; r3 points to aa[i]
	LDR r5, =bb0	; r5 points to bb[i]	
	; Do the loop       
loop
	LDR r4, [r3], #4 ; load aa[i] into r4, then point to next value
	LDR r6, [r5], #4 ; load bb[i] into r6, then point to next value
	MOV r7, #5	; move #5 into r7
	SUB r4, r4, r6	; r4 = aa[i] - bb[i]
	ADD r4, r4, r7	; r4 = r4 + #5
	STR r4, [r2], #4 ; store the result to zz[i], then point to next value
	ADD r0, r0, #1	; increment i
	CMP r0, r1	; Compare (r0 - 15)
	BLT loop	; if r0 < 15, branch to loop
halt 	B halt		; branch to self
	; Define the value of the 2 input variables
aa0	DCD  10,20,30,40,50,60,70,80,90,100,110,120,130,140,150
bb0	DCD  1,2,3,4,5,6,7,8,9,10,11,12,13,14,15
	; Define the DATA block
	AREA	data1, DATA
	; Set space for variables
zz0	SPACE 60	; Set 60 bytes aside for zz

	END		; End the program
