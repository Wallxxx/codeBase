name "CourseWork (old)" 

Assume CS:Code, DS:Code
Code Segment
    org 100h                 ; Program start point.
    
start:
    mov dx, offset inputMsg
    mov ah, 9h
    int 21h
    jmp main
    
main:
    mov ah, 01h              ; Enter char.
    int 21h
    cmp al, 1bh              ; If char == escape:
    je exit                  ; exit.
    cmp al, 0dh              ; If char == enter:
    je space                 ; go space
    
next: 
    cmp al, 73h              ; If char == 's':
    je this                  ; go this.
    cmp al, 53h              ; If char == 'S':
    je this                  ; go this.
    mov previous, al         ; previous char = this char.
    cmp bl, 0                ; If counter == 0:
    je main                  ; return main,
    cmp al, 40h              ; else if this char belongs...
    jb result                ; ...to an interval of... 
    cmp al, 7ah              ; ...letters:
    ja result                ; go result. 

this:
    cmp previous, 41h        ; If previous char - any letter:
    jae main                 ; this word does not fit, return main,
    add bl, 1                ; else: counter += 1
    jmp main                 ; return main.

result:
    mov ah, 02h           
    mov dl, 07h              ; 07h = beep.
    int 21h                  ; 21h -> use beep.
    mov bl, 0                ; counter = 0.
    jmp main                 ; return main.
    
space:
    mov dx, offset spaceMsg
    mov ah, 9h
    int 21h                  ; Line break.
    jmp next                 ; go next.
    
exit:
    int 20h                  ; End of the program.    
    
previous db 0                ; "char previous = 0;" (analogy with c/c++).
inputMsg db "Enter string: $"; Main message.
spaceMsg db 0dh, 0ah, "$"    ; Line break.
Code ENDS                    ; End "Code Segment".
END main                     ; End "main", end code.

; Test 1: "Six" (correct result: 1 beep)
; Test 2: "Mystery" (correct result: 0 beep)
; Test 3: "Mystery six" (correct result: 1 beep)
; Test 4: "This is mystery six" (correct result: 1 beep)
; Test 5: "six, six, six, six." (correct result: 4 beep)
; Test 6: "Hello! I am Jane. I am 21. I am a student. I consider 
;         it to be a special time in my life. Student life is 
;         always full of fun, new impressions and knowledge. I 
;         like being a student. Each day I learn something new 
;         and communicate with my friends. We meet almost every day. 
;         Sometimes we stay at the university after classes to 
;         prepare our homework for the next day or just to talk 
;         about our student life." 
;         (correct result: 8 beep) (Text from the internet)
