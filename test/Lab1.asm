#include <p16F690.inc> ; Файл с именами регистров. 

; Настройка битов конфигурации контроллера.
    
__config (_INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _BOR_OFF & _IESO_OFF & _FCMEN_OFF)

; Присвоение переменным адресов памяти.
    
DEST equ 0x20
CL1 equ 0x21
CL2 equ 0x22
CELL equ 0x23 

org 0 ; Начало обработки программы (с нулевой строки).

Start
    bsf DEST,0 ; Направление перехрда.
    bsf STATUS,RP0 ; Банк 1 активен.
    movlw 0xff 
    movwf TRISA ; Порт "A" на вход.
    clrf TRISC ; Порт "C" на выход.
    bcf STATUS,RP0 ; Банк 2 активен.
    bsf STATUS,RP1
    movlw 0xf7 ; Порт "A" на 3-й цифровой пин.
    movwf 0x1E ; Регистр уплавления цифра/аналог.
    bcf STATUS,RP0 ; Банк 0 активен.
    bcf STATUS,RP1 
    movlw 0b00000000 ; Все светодиоды выключены. 
    movwf CELL
    movf CELL,W
    movwf PORTC
    
LOOP: 
    btfss PORTA,3 ; Проверка нажатия кнопки.
    goto TOUCH ; Вызывается, если кнопка нажата. 
    goto LOOP ; Если кнопка не нажата. 
    
TOUCH:
    btfss CELL, 0 ; Если все 4 диода выключены...
    btfss CELL, 1
    btfss CELL, 2
    btfss CELL, 3
    goto FIRST ; Переходим к первой функции.
    btfsc CELL, 0 ; Если 0 и 3 диоды включены
    btfsc CELL, 3
    goto SECOND ; Переход ко второй функции
    btfsc CELL, 1 ; Если первый и второй включены
    btfsc CELL, 2
    goto THIRD ; Переход к третьей функции
    btfsc CELL, 0 ; Если 0 и 2 включены
    btfsc CELL, 2
    goto FOUR ; Переход к четвёртой функции
    btfss CELL, 0 ; Если 0, 1 и 2 включены
    btfss CELL, 1
    btfss CELL, 2
    goto SHUTDOWN ; Выключение всех

FIRST: ; Включение 1-го и 4-го.
    bcf STATUS,0
    clrf STATUS
    movlw 0b00001001
    movwf CELL
    movf CELL,W
    movwf PORTC
    goto LOOP
    
SECOND: ; Включение 2-го и 3-го.
    bcf STATUS,0
    clrf STATUS
    movlw 0b00000110
    movwf CELL
    movf CELL,W
    movwf PORTC
    goto LOOP
    
THIRD: ; Включение 1-го и 3-го.
    bcf STATUS,0
    clrf STATUS
    movlw 0b00000101
    movwf CELL
    movf CELL,W
    movwf PORTC
    goto LOOP

FOUR: ; Включение 1-го, 2-го и 3-го.
    bcf STATUS,0
    clrf STATUS
    movlw 0b00000111
    movwf CELL
    movf CELL,W
    movwf PORTC
    goto LOOP

SHUTDOWN: ; Выключение всех.
    bcf STATUS,0
    clrf STATUS
    movlw 0b00000000
    movwf CELL
    movf CELL,W
    movwf PORTC
    goto LOOP

    
delay ; Задержка.
    movlw 0xff
    movf CL1,F
loop1: 
    decf CL1,F
    btfsc STATUS,2
    goto NEXT
    movlw 0x2f
    movf CL2,F
loop2: 
    decf CL2,F
    btfsc STATUS,2
    goto loop1
    goto loop2
NEXT:
    return

end
