;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;   Brian Arnberg                       ;;
;;   Problem Set #4 - Problem 1          ;;
;;    IIR direct form type II filter     ;;
;;      TMS 32C5x DSP                    ;;
;; y(n) = b3*v3 + b2*v2 + b1*v1 + b0*v0  ;;
;;   v0 = x(n) - a3*v3 - a2*v2 - a1*v1   ;;
;;    A3 = -a3   B3 = b3   B0 = b0       ;;
;;    A2 = -a2   B2 = b2                 ;;
;;    A1 = -a1   B1 = b1                 ;;
;;                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	IN X0, PA0	; read x(n) from PA0, set as vn
	ZAP		; clear A and P
	MAC A3, v3	; a3*v3
	MAC A2, v2	; +a2*v2
	MAC A1, v1	; +a1*v1 
	APAC		; A = A+P
	ADD X0		; A = A + X0
	SACH v0		; save LSB to V0
	ZAP		; clear A and P
	MAC B3, v3	; b3*v3
	MACD B2, v2	; b2*v2, move v2 to v3
	MACD B1, v1	; b1*v1, move v1 to v2
	MACD B0, v0	; b0*v0, move v0 to v1
	APAC     	; A=A+P
	SACH Yn 	; save LSB
	OUT Yn,PA1 	; write y(n) to PA1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                       ;;
;;   Program Memory   |   Data Memory    ;;
;;____________________|__________________;;
;;    A3 = -a3        |    X0            ;;
;;    A2 = -a2        |    V0   (low)    ;;
;;    A1 = -a1        |    V1            ;;
;;    B0 =  b0        |    V2            ;;
;;    B1 =  b1        |    V3   (high)   ;;
;;    B2 =  b2        |                  ;;
;;    B3 =  b3        |                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
