python makerom.py
# generate the rom header
xxd -i -n rom_bin bin/rom.bin src/rom_bin.h
# hexdump the rom
hexdump -C bin/rom.bin
