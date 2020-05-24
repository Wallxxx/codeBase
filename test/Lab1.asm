#include <p16F690.inc> ;подключаем файл с именами регистров

__config (_INTRC_OSC_NOCLKOUT & _WDT_OFF & _PWRTE_OFF & _MCLRE_OFF & _CP_OFF & _BOR_OFF & _IESO_OFF & _FCMEN_OFF)

DIOD equ 0x20 ;присваиваем переменным
CL1 equ 0x21 ;адреса памяти
CL2 equ 0x22
    org 0 ;начало обработки программы

Start
    bsf STATUS,RP0 ;банк1
    movlw 0xFF
    movwf TRISA ;весь ПОРТА на вход
    clrf TRISC ;весь ПОРТС на выход
    movlw 0x10 ;A/D - Fosc/8
    movwf ADCON1 ;регистр управления скоростью преобразования
    bcf STATUS,RP0 ;
    bsf STATUS,RP1 ;банк2
    movlw 0xF7 ;весь ПОРТА аналоговый, кроме пин3 (RA3)
    movwf ANSEL ;регистр управления - аналог/цифра
    bcf STATUS,RP0 ;банк0
    bcf STATUS,RP1
    movlw 0x01
    movwf ADCON0 ;включаем A/D
    movlw 0x08
    movwf DIOD
    
MAIN:
    movf DIOD,w ;вывод на св-диоды
    movwf PORTC
    nop ;ожидаем 5мкс для усиления A/D и
    nop ;зарядки конденсаторов
    nop ;
    nop ;
    nop ;
    bsf ADCON0,1 ;старт конвертации
    btfss ADCON0,1 ;проверка бита GO, если=0 то вып след команду
    goto $-1 ;декремент счетчика команд процессора
    movf ADRESH,w ;конец конвертации, помещаем старший байт
    addlw 0x01 ;прибавляем единицу к W
    movwf CL2 ;W в переменную задержки

AD_DelayLoop
    decfsz CL1,f ;декремент CL1, если STATUS(3)=1, то пропустить
    goto AD_DelayLoop ; The Inner loop takes 3 instr. per loop * 256 loopss = 768 instr.
    decfsz CL2,f ; The outer loop takes and additional 3 instructions per lap * 256 loops
    goto AD_DelayLoop ; (768+3) * 256 = 197376 instr. / 1M instr. per second = 0.197 sec.
    movlw .13 ;еще одна задержка на 10мс
    movwf CL2

TenmSdelay
    decfsz CL1,f
    goto TenmSdelay
    decfsz CL2,f
    goto TenmSdelay

Rotate
    
    btfss DIOD, 0 ; Если все 4 диода выключены...
    btfss DIOD, 1
    btfss DIOD, 2
    btfss DIOD, 3
    rrf DIOD, 0b00001001 ; Включаем первую комбинацию.
    btfsc DIOD, 0 ; Если 0 и 3 диоды включены
    btfsc DIOD, 3
    rrf DIOD, 0b00000110 ; Включаем вторую комбинацию.
    btfsc DIOD, 1 ; Если первый и второй включены
    btfsc DIOD, 2
    rrf DIOD, 0b00000101 ; Включаем третью комбинацию.
    btfsc DIOD, 0 ; Если 0 и 2 включены
    btfsc DIOD, 2
    rrf DIOD, 0b00000111 ; Включаем четвёртую комбинацию.
    btfss DIOD, 0 ; Если 0, 1 и 2 включены
    btfss DIOD, 1
    btfss DIOD, 2
    rrf DIOD, 0b00000000 ; Выключаем все.
    goto MAIN ;вернутся назад

end
