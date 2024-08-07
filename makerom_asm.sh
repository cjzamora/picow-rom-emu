# if bin directory does not exist, create it
if [ ! -d "bin" ]; then
  mkdir bin
fi

# check if there is .s param file passed
if [ -z "$1" ]; then
  echo "usage: $0 <asm_file>"
  exit 1
fi

# generate rom using vasm old style
# make sure to add .section code, "rx" at the beginning of the
# assembly file to automatically place the code in the correct
# memory region and add padding in between the code and data
vasm6502_oldstyle -Fbin -dotdir $1 -o bin/rom.bin
# generate the rom header
xxd -i -n rom_bin bin/rom.bin src/rom_bin.h
# hexdump the rom
hexdump -C bin/rom.bin
