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
    ; Set all bits of DDRB to output 1111 1111 (MSB->LSB)
    lda #%11111111
    sta DDRB
    ; Set the top 3 bits of DDRA to output 1110 0000  (MSB->LSB) (P7-P5)
    lda #%11100000
    sta DDRA

    ; Set 8-bit mode 2-line display (MSB->LSB)
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

    ; Load index 0 of message into x
    ldx #0

print:
    ; Load the character at index x of message into A
    lda message, x
    ; If the character is null go to reset
    beq reset
    ; Print the character
    jsr print_char
    ; Increment x
    inx
    ; Jump back to print
    jmp print

message: .asciiz "Hello, World!"

lcd_wait:
    pha
    ; Set PORTB to input
    lda #%00000000
    sta DDRB

lcd_busy:
    lda #RW
    sta PORTA
    lda #(RW | E)
    sta PORTA
    lda PORTB
    and #%10000000
    bne lcd_busy

    ; Set PORTB to output
    lda #%11111111
    sta DDRB
    pla
    rts

lcd_instruction:
    jsr lcd_wait

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
    jsr lcd_wait

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