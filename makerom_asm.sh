# generate rom using vasm old style
# make sure to add .section code, "rx" at the beginning of the
# assembly file to automatically place the code in the correct
# memory region and add padding in between the code and data
vasm6502_oldstyle -Fbin -dotdir src/rom.s -o bin/rom.bin
# generate the rom header
xxd -i -n rom_bin bin/rom.bin src/rom_bin.h
# hexdump the rom
hexdump -C bin/rom.bin
