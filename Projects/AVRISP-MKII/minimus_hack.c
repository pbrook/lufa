#include "AVRISP-MKII.h"
#include <LUFA/Drivers/Peripheral/Serial.h>
#include <LUFA/Drivers/Board/Buttons.h>

#include "minimus_rom.h"

#define AUDIO_SCALE 12

#define BEEP_MASK (1 << 7)
void
LogByte(const uint8_t Byte)
{
  static int count;

  if (count) {
      count--;

      if (Byte & 1)
	  PORTB |= BEEP_MASK;
      else
	  PORTB &= ~(BEEP_MASK);
      return;
  }
  count = AUDIO_SCALE;
}

void foo(uint16_t val)
{
  int i;
  static const char hex_chars[] = "0123456789abcdef";
  for (i = 0; i < 4; i++) {
    Serial_SendByte(hex_chars[val >> 12]);
    val <<= 4;
  }
  Serial_SendByte('\n');
}

static uint8_t
do_cmd(const uint32_t cmd)
{
  ISPTarget_SendByte(cmd >> 24);
  ISPTarget_SendByte((cmd >> 16) & 0xff);
  ISPTarget_SendByte((cmd >> 8) & 0xff);
  return ISPTarget_TransferByte(cmd & 0xff);
}

static void del(void)
{
  uint32_t id;
  for (id = 0; id < 16000ul * 60; id++)
    asm volatile("");
}

enum fuse_id {
    FUSE_LOW,
    FUSE_HIGH,
    FUSE_EXT
};
static void
mm_SetFuse(enum fuse_id id, uint8_t val)
{
  uint8_t old;
  uint32_t read_cmd = 0;
  uint32_t write_cmd = 0;
  switch (id) {
    case FUSE_LOW:
      read_cmd = 0x5000;
      write_cmd = 0xaca0;
      break;
    case FUSE_HIGH:
      read_cmd = 0x5808;
      write_cmd = 0xaca8;
      break;
    case FUSE_EXT:
      read_cmd = 0x5008;
      write_cmd = 0xaca4;
      break;
  }
  read_cmd <<= 16;
  write_cmd <<= 16;

  old = do_cmd(read_cmd);
  if (old == val)
    return;
  do_cmd(write_cmd | val);
  do {
      old = do_cmd(read_cmd);
  } while (old != val);
}

static void
mm_SetLockBits(uint8_t val)
{
  uint8_t old;

  old = do_cmd(0x58000000ul) & 0x3f;
  if (old == val)
    return;
  do_cmd(0xace000c0ul | val);
  do {
      old = do_cmd(0x58000000ul) & 0x3f;
  } while (old != val);
}

static void
mm_EraseChip(void)
{
  do_cmd(0xac977f00ul);
}

static bool
mm_StartISP(void)
{
  uint8_t id;
  ISPTarget_ChangeTargetResetLine(false);
  ISPTarget_EnableTargetISP();
  del();
  ISPTarget_ChangeTargetResetLine(true);
  del();
  ISPTarget_SendByte(0xac);
  ISPTarget_SendByte(0x53);
  id = ISPTarget_TransferByte(0);
  ISPTarget_SendByte(0);
  return id != 0x53;
}

static bool
mm_VerifyID(void)
{
  uint32_t id;
  int i;
  id = 0;
  for (i = 0; i < 3; i++)
    id = (id << 8) | do_cmd(0x30000000 | (i << 8));
  return (id != 0x1e958aul);
}

static void
mm_ProgramFlash(void)
{
  uint16_t i;
  uint8_t c;
  uint32_t cmd;

  cmd = 0x40000000ul;
  cmd |= ((uint32_t)mm_rom_start << 7) & 0xffff00ul;
  i = 0;
  while (i < mm_rom_len) {
      c = pgm_read_byte(&mm_rom[i]);
      do_cmd(cmd | c);
      i++;
      cmd ^= 0x08000000ul;
      if ((i & 0x3f) == 0) {
	  do_cmd(0x4c000000ul | (cmd & 0x00ffc000ul));
	  while((do_cmd(0xf0000000ul) & 1) == 1)
	    /* No-op */;
      }
      if ((i & 1) == 0)
	cmd += 0x100;
  }
}

static bool
mm_button(void)
{
  return (Buttons_GetStatus() & BUTTONS_BUTTON1) != 0;
}

bool
program_minimus(void)
{
  static bool done_init;
  int i;

  if (!done_init) {
      Buttons_Init();
      DDRB |= BEEP_MASK;
      PORTB |= BEEP_MASK;
      //Serial_Init(9600, false);
      done_init = true;
  }

  if (ISPActive)
    return false;

  if (!mm_button())
    return false;
  for (i = 0; i < 10; i++) {
      if (!mm_button())
	return true;
      LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED2);
      Delay_MS(100);
      if (!mm_button())
	return true;
      LEDs_SetAllLEDs(0);
      Delay_MS(100);
  }
  LEDs_SetAllLEDs(LEDS_LED1 | LEDS_LED2);
  while (mm_button())
    Delay_MS(1);

  if (mm_StartISP())
    goto fail;
  if (mm_VerifyID())
    goto fail;
  mm_EraseChip();
  if (mm_StartISP())
    goto fail;
  mm_SetLockBits(0x3f);
  mm_SetFuse(FUSE_EXT, 0xcb);
  mm_SetFuse(FUSE_HIGH, 0xd8);
  mm_SetFuse(FUSE_LOW, 0xff);
  mm_StartISP();
  mm_ProgramFlash();
  mm_SetLockBits(0x2f);
  ISPTarget_ChangeTargetResetLine(false);
  ISPTarget_DisableTargetISP();
  //Serial_SendString("DONE\n");
  return true;
fail:
  //Serial_SendString("FAIL\n");
  return true;
}
