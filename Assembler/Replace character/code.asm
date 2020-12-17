name 'HomeWork'

include 'emu8086.inc'   ; For 'putc'

Assume CS:Code, DS:Code
Code Segment
    org 100h            ; Program start point.
    
main:
    mov ah, 0           ; Enter char...
    int 16h             ; ...but not print.
    cmp al, 1bh         ; If char = escape...
    je exit             ; ...exit.
    cmp al, 0f1h        ; If char = {russian "e" with dots}...
    je swap             ; ...replaced by space.
    cmp al, 08h         ; If char != backspace...
    jne print           ; ...print.
    jcxz main           ; If there is nothing to delete - go main.
    dec cx              ; Line lenght - 1.
    dec di
    putc 08h            ; Print backspace.
    putc ' '            ; Print space.
    putc 08h            ; Print backspace.
    jmp main            ; Go main.
    
print:    
    mov ah, 0eh         ; Print char.
    int 10h
    jmp main            ; Return main.
    
swap:
    mov al, 20h         ; Replace.
    jmp print           ; Print.
    
exit:
    int 20h             ; Exit. 
    
Code ENDS
END main
