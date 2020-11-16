; Выполнено 24.03.2020

Assume CS: Code, DS: Code
Code Segment
    str db 21								; Исходная строка (последняя позиция для символа конца строки)
    strlen equ 20							; Длина строки
    org 100h

Start:    
    mov ah, 09h								; Вывод сообщения, где сообщается о необходимости ввести строку
    mov dx, offset inputStringMessage
    add dx, 2								; Первые два байта зарезервированны
    int 21h
    
    mov ah, 0ah								; Ввод строки
    mov dx, offset str
    int 21h
    
    mov ah, 09h								; Переход на новую строку в консили
    mov dx, offset paragraph
    add dx, 2
    int 21h     
           
    mov ah, 09h								; Вывод сообщения, где сообщается о необходимости ввести символ, на который делать замену
    mov dx, offset inputSymbolMessage
    add dx, 2
    int 21h
    
    mov ah, 01h								; Ввод символа...
    int 21h
    mov bl, al								; ...и перемещение его в bl, а в...
    mov al, 61h								; ...al храним букву "a" (61 hex)
    
    jmp findAndReplace						; Переходим части кода, отвечающей за поиск и замену буквы в строке

findAndReplace: 
    cld										; Установка флага DF в 0, таким образом scasb будет идти от...
    mov cx, strlen							; ...strlen к 0
    lea di, str
    repne scasb								; Ищем букву. Scasb сравнивает каждый символ строки с символом в al
    cmp cx, 0								; Если прошли всю строку...
    je show									; Выводим результат. Иначе: значит...
    dec di									; ...всю строку не прошли и нашли нужный символ для замены
    mov byte ptr[di], bl					; Заменяем его и...
    jmp findAndReplace  					; Ищем ещё

show: ; Тут возникли проблемы с выводом, пришлось костылём делать. 
    mov byte ptr[di - 9], '$'				; Ставим в "конце" строки символ конца 

    mov ah, 09h								; Переходим на новую строку в консоли
    mov dx, offset paragraph
    add dx, 2
    int 21h
    
    mov ah, 09h								; Выводим сообщение "Result: " и переходим на новую строку в консоли
    mov dx, offset resultMessage
    add dx, 2
    int 21h
    
; 	Отчистка экрана консоли (не пригодилось):
;    mov ah, 0
;    mov al, 2
;    int 10h
     
    mov ah,09h								; Выводим уже изменённую строку
    mov dx, offset str
    add dx, 2
    int 21h
    jmp Quit								; И выходим из программы, вызывая 20-е прерывание
    
Quit: int 20h

inputStringMessage db 'Enter string: ', '$'	; Строки для вывода
inputSymbolMessage db 'Enter symbol: ', '$'
resultMessage db 'Result: ', 0Dh, 0Ah, '$'
paragraph db 0Dh, 0Ah, '$'

Code ENDS
END Start
