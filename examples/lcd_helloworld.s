; 6522 Data Lines
PORTB = $6000
PORTA = $6001

; 6522 Data Direction Register Select
; 10 = DDRB
; 11 = DDRA
DDRB = $6002
DDRA = $6003

; Hitachi HD44780 LCD
; Enable = Enable (E)
; RW = Read/Write (R/W)
; RS = Register Select (RS)
E = %10000000
RW = %01000000
RS = %00100000

    .section code, "rx"
    .org $8000

reset:
    ; Set all bits of DDRB to output 1111 1111 (LSB->MSB)
    lda #%11111111
    sta DDRB
    ; Set the top 3 bits of DDRA to output 1110 0000
    lda #%11100000
    sta DDRA

    ; Set 8-bit mode 2-line display
    lda #%00111000
    jsr lcd_instruction
    ; Set display on, cursor off, blink on
    lda #%00001110
    jsr lcd_instruction
    ; Increment and shift cursor, no display shift
    lda #%00000110
    jsr lcd_instruction
    ; Clear display
    lda #%00000001
    jsr lcd_instruction

    ; Print "HELLO WORLD!"
    lda #"H"
    jsr print_char
    
    lda #"E"
    jsr print_char

    lda #"L"
    jsr print_char

    lda #"L"
    jsr print_char

    lda #"O"
    jsr print_char

    lda #" "
    jsr print_char

    lda #"W"
    jsr print_char

    lda #"O"
    jsr print_char

    lda #"R"
    jsr print_char

    lda #"L"
    jsr print_char

    lda #"D"
    jsr print_char

    lda #"!"
    jsr print_char

loop:
    jmp reset

lcd_instruction:
    sta PORTB
    
    ; Clear RS/RW/E bits
    lda #0
    sta PORTA
    ; Toggle E bit
    lda #E
    sta PORTA
    ; Clear RS/RW/E bits
    lda #0
    sta PORTA
    ; Return to subroutine
    rts

print_char:
    sta PORTB
    
    ; Clear RS/RW/E bits
    lda #RS
    sta PORTA
    ; Toggle RS | E bit
    lda #(RS | E)
    sta PORTA
    ; Clear RS/RW/E bits
    lda #RS
    sta PORTA
    ; Return to subroutine
    rts

    .org    $fffc
    .word   reset
    .word   $0000