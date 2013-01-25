;; Problem Set 2, problem 1
;;  Execute the following:
;;    zz = aa*(bb+cc) - (dd*35);
;;    define: aa,bb,cc,dd in code area
;;            zz in data area

LDR r4, =bb	; Get address of bb
LDR r0, [r4]	; Store value of bb in r0
LDR r4, =cc	; Get address of cc
LDR r1, [r4]	; Store value of cc in r1
ADD r2, r0, r1	; Compute bb+cc, store to r2 (r2 = bb+cc)
LDR r4, =aa	; Get address of aa
LDR r0, [r4]	; Store value of aa in r0
MUL r2, r2, r0	; Compute aa*(bb+cc), store to r2

LDR r4, =dd	; Get address of dd
LDR r0, [r4]	; Store value of dd in r0
MUL r3, r0, #35	; Compute dd*35, store to r3

SUB r2, r2, r3	; Final Computation: r2 = r2 - r3

LDR r4, =zz	; Get address of zz
STR r2, [r4]	; Store result to address of zz ( [zz] <= r2 )

