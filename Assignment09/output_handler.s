;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   Brian Arnberg                       ;;
;;   Problem Set #5 - Output Handler     ;;
;;    output_handler.s                   ;;
;;     - provides two functionalities    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   enable_leds - turns an LED on       ;;
;;      - assumes a single argument      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   clear_leds - turns an LED off       ;;
;;      - assumes a single argument      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

GPIOD	EQU 0x40020C00		;; general purpose I/O, port D
BSRRL	EQU 0x18		;; port D set
BSRRH	EQU 0x1A		;; port D reset

	AREA OUTPUT, CODE
	EXPORT enable_leds   
	EXPORT clear_leds

enable_leds
	MOV r2, #1		;; make r2 = 0x01
	MOV r2, r2, lsl r0	;; left shift the bit by the argument
	LDR r3, =GPIOD		;; get the address of GPIOD
	STR r2, [r3, #BSRRL]    ;; write the pattern in r2 to GPIOD->BSRRL
				;;  this sets the pin value (passed in the argument)
	BX r14			;; return from output handler

clear_leds	;; clear the LEDS
	MOV r2, #1		;; make r2 = 0x01
	MOV r2, r2, lsl r0	;; left shift the bit by the argument
	LDR r3, =GPIOD		;; get the address of GPIOD
	STR r2, [r3, #BSRRH]    ;; write the pattern in r2 to GPIOD->BSRRH
				;;  this clears the pin value (passed in the argument)
	BX r14			;; return from output handler


	END
