    radix dec
    include "p16f690.inc"

    extern STK00

UDL_delay udata
delay0 res 1
delay1 res 1
delay_arg0 res 1
delay_arg1 res 1

code_delay code

; Clock rate: 8 MHz
; Cycles per ms: (rate / 4 / 1000) = 2000
; Cycles per us: (rate / 4 / 1000000) = 2

    global _delay
_delay:
    banksel delay0

    movwf delay_arg1
    movf STK00, w
    movwf delay_arg0

    incf delay_arg0, f
    incf delay_arg1, f
    goto _delay_loop_enter

; Every iteration takes 2000 cycles. Every 256th iteration it takes 2002 cycles.
; Considering the input is arbitrary, it can't really be made much more accurate...
_delay_loop:
    call _delay_ms
_delay_loop_enter:
    decfsz delay_arg0, f
    goto _delay_loop
    decfsz delay_arg1, f
    goto _delay_loop

    return

; Delay a milisecond
_delay_ms:
    movlw 223               ; 1
    movwf delay0            ; 1
    movlw 3                 ; 1
    movwf delay1            ; 1

_delay_ms_loop:
    decfsz delay0, f        ; 1 (2)
    goto _delay_ms_loop     ; 2
    decfsz delay1, f        ; 1 (2)
    goto _delay_ms_loop     ; 2

    return                  ; 2

; Cycles for delay0 loop (where 0 < delay0 <= 256):
;   (delay0 - 1) * (1 + 2) + 2
;   simplified: delay0 * 3 - 1
; Cycles for delay1 loop (where 1 < delay1 <= 256):
;   ((delay0 * 3 - 1) + 1 + 2) + (delay1 - 2) * ((256 * 3 - 1) + 1 + 2) + ((256 * 3 - 1) + 2)
;   simplified: delay0 * 3 + delay1 * 770 - 771

; For delay0 = 150 and delay1 = 3
;   150 * 3 + 3 * 770 - 771 = 1989 cycles
; Cycles total (including prologue and epilogue): 1989 + 4 + 2 = 1995
; If you count the cycles used by `call` (2), `decfsz` (1) and `goto` (2),
;   it becomes 2000, making it suitable for our purpose.

    end
