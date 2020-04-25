; Матвеев, Пылаев, Ушаков - БСБО-06-17
    
#include <p16F690.inc> ; Основная библиотека.

; Установки битов настойки.
__config (_INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _BOR_OFF & _IESO_OFF & _FCMEN_OFF)

; Переменные CL1 и CL2 равны 32 и 33 соответственно.
CL1 equ 0x20
CL2 equ 0x21
 
org 0 ; Начало обработки программы.

Start
    bsf STATUS,RP0 ; Установка бит управления банками (банк 1).
    clrf TRISC ; Отчистка регистра TRISC.
    bcf STATUS,RP0 ; Снимаем бит RP0 (доступен банк 0).
    MLOOP: clrf PORTC ; Метка MLOOP и отчистка порта C (все светодиоды гаснут).
		movlw b'00000011' ; Установка 0-го и 1-го светодиода.
		movwf PORTC ; Включение.
		call delay ; Задержка.
		bcf PORTC, 0 ; Выключение 0-го и...
		bcf PORTC, 1 ; ...1-го светодиодов. 
		movlw b'00001100' ; Установка 2-го и 3-го.
		movwf PORTC ; Включение.
		call delay
		bcf PORTC, 2 ; Выключение 2 и 3 светодиодов.
		bcf PORTC, 3
		bsf PORTC, 0 ; Включение 0-го.
		call delay
		bcf PORTC, 0
		bsf PORTC, 2
		call delay
		bcf PORTC, 2
		goto MLOOP ; Возвращение к началу метки MLOOP (бесконечный цикл).

delay
    movlw 0xff ; Пересылка значения ff в w.
    movf CL1,F ; Пересылка w в переменную CL1.
    loop1: decf CL1,F ; Уменьшение CL1 на 1.
		btfsc STATUS,2  ; Если CL1 = 0, то...
		goto NEXT ; ...переход в метку NEXT, иначе...
		movlw 0x5f ; ...значение переменной внутреннего цикла равно 5f.
		movf CL2,F ; Пересылка w в переменную CL2.
    loop2: decf CL2,F ; Уменьшение CL2 на 1.
		btfsc STATUS,2 ; Если CL2 равно 0, то...
		goto loop1 ; ...переход в начало метки loop1, иначе...
		goto loop2 ; ...переход в начало метки loop2.
    NEXT: return ; Выход из цикла. 

end ; Конец программы. 
