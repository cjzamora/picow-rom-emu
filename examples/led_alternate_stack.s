    .section code, "rx"
    .org $8000

reset:
    lda #$ff    
    sta $6002 

loop:
    jsr blink_a
    jsr blink_b

    jmp loop

blink_a:
    lda #$55
    sta $6000
    rts

blink_b:
    lda #$aa
    sta $6000
    rts

    .org    $fffc
    .word   reset
    .word   $0000