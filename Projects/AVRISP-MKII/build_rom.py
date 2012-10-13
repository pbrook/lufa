#! /usr/bin/python

import io

line = 0
fin = io.open("minimus.rom", "rt")
fout = io.open("minimus_rom.h", "wt")
fout.write(u"static const uint8_t mm_rom[] PROGMEM = {\n");
first_addr = -1;
rom_len = 0;
for s in fin:
  line += 1
  s = s.strip();
  if s[0] != ':':
    raise Exception("Bad character at line %d" % line);
  n = int(s[1:3], 16);
  addr = int(s[3:7], 16);
  flags = int(s[7:9], 16);
  if first_addr == -1:
    first_addr = addr;
  elif first_addr + rom_len != addr:
    raise Exception("Bad address 0x%x (expected 0x%x) at line %d" % (addr, next_addr, line))
  for i in range(0, n):
    b = int(s[9 + i * 2: 11 + i * 2], 16)
    fout.write(u"0x%02x," % b)
  rom_len += n
  fout.write(u"\n")

while (rom_len & 127) != 0:
  fout.write(u"0xff,")
  rom_len += 1
fout.write(u"\n};\n")
if (first_addr & 127) != 0:
  raise Exception("Image no page aligned")
fout.write(u"#define mm_rom_len 0x%xu\n" % rom_len);
fout.write(u"#define mm_rom_start 0x%xu\n" % first_addr);
fout.close();
fin.close();
