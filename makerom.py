import os

# 6502 instructions
code = bytearray([
    0xa9, 0xff,         # lda #$ff
    0x8d, 0x02, 0x60,   # sta $6002

    0xa9, 0x55,         # lda #$55
    0x8d, 0x00, 0x60,   # sta $6000

    0xa9, 0xaa,         # lda #$aa
    0x8d, 0x00, 0x60,   # sta $6000

    0x4c, 0x05, 0x80    # jmp $8005
])

# generate 65k rom file with 0xea (noop) instructions
rom = bytearray([0xea] * 0x10000)

# copy code to 0x8000
for i in range(len(code)):
    rom[0x8000 + i] = code[i]

# set reset vector to 0x8000
rom[0xfffc] = 0x00
rom[0xfffd] = 0x80

# create bin directory if it doesn't exist
os.makedirs("bin", exist_ok=True)

# write rom to bin/rom.bin
with open("bin/rom.bin", "wb") as out_file:
    out_file.write(rom)