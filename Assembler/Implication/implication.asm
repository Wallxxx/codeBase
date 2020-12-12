include "p16f84.inc"
__config _HS_OSC & _WDT_OFF & _PWRTE_ON

MAIN	
	clrf	PORTB			    ; Отчистка порта B.
	bsf 	STATUS, RP0		
	movlw	b'11111111'		; Все линии порта B - входные.
	movwf	TRISB			    ; Установка. 
	movlw	b'11110000'		; Линии с 0-й по 3-ю порта А - выходные. 
	movwf	TRISA 			  ; Установка. 
	bcf STATUS, RP0			; Возвращение в бланк 0. 
	
; Далее - бесконечный цикл. 
	
LOOP	
	clrw					; Отчистка регистра 'w'.
	; Далее читаем первый бит числа X и Y. Если первый бит числа X 
	; равен 1, то смотрим первый бит числа Y. Если и он равен единице, 
	; то в регистре 'w' на первый бит ставим единицу (первый диод
	; будет светиться). В противном случае пропускаем 26-ю строку
	; и диод гореть не будет. 
	; Если же первый бит числа X - ноль, то сразу переходим к 26-й
	; и включаем светодиод (исходя из таблицы истинности следования). 
	btfss 	PORTB, 0
	btfss 	PORTB, 7 
	addlw 	b'00000001'
	; Аналогично с другими битами числа X и Y. 
	btfss 	PORTB, 1
	btfss 	PORTB, 6
	addlw 	b'00000010'
	btfss 	PORTB, 2
	btfss 	PORTB, 5
	addlw 	b'00000100'
	btfss 	PORTB, 3
	btfss 	PORTB, 4
	addlw 	b'00001000'
	movwf	PORTA 			; Передаём данные из 'w' в PORTA (включаем диоды).
	goto LOOP 				; Возвращаемся в начало цикла. 
end
