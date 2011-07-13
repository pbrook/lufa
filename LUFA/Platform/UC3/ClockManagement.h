/*
             LUFA Library
     Copyright (C) Dean Camera, 2011.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2011  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *  \brief Module Clock Driver for the AVR32 UC3 microcontrollers.
 *
 *  Clock management driver for the AVR32 UC3 microcontrollers. This driver allows for the configuration
 *  of the various clocks within the device to clock the various peripherals.
 */

/** \ingroup Group_PlatformDrivers
 *  \defgroup Group_PlatformDrivers_UC3Clocks UC3 Clock Management Driver - LUFA/Platform/UC3/ClockManagement.h
 *  \brief Module Clock Driver for the AVR32 UC3 microcontrollers.
 *
 *  \section Sec_Dependencies Module Source Dependencies
 *  The following files must be built with any user project that uses this module:
 *    - None
 *
 *  \section Sec_ModDescription Module Description
 *  Clock management driver for the AVR32 UC3 microcontrollers. This driver allows for the configuration
 *  of the various clocks within the device to clock the various peripherals.
 *
 *  Usage Example:
 *  \code
 *		#include <LUFA/Platform/UC3/ClockManagement.h>
 *
 *		void main(void)
 *		{
 *			// Start the master external oscillator which will be used as the main clock reference
 *			AVR32CLK_StartExternalOscillator(0, EXOSC_MODE_8MHZ_OR_MORE, EXOSC_START_0CLK);
 *			
 *			// Start the PLL for the CPU clock, switch CPU to it
 *			AVR32CLK_StartPLL(0, CLOCK_SRC_OSC0, 12000000, F_CPU);
 *			AVR32CLK_SetCPUClockSource(CLOCK_SRC_PLL0, F_CPU);
 *
 *			// Start the PLL for the USB Generic Clock module
 *			AVR32CLK_StartPLL(1, CLOCK_SRC_OSC0, 12000000, 48000000);
 *		}
 *  \endcode
 *
 *  @{
 */

#ifndef _UC3_CLOCK_MANAGEMENT_H_
#define _UC3_CLOCK_MANAGEMENT_H_

	/* Includes: */
		#include <avr32/io.h>
		#include <stdbool.h>
		#include <stdint.h>

		#include <LUFA/Common/Common.h>

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Enum for the possible external oscillator types. */
			enum Extern_OSC_ClockTypes_t
			{
				EXOSC_MODE_CLOCK         = 0, /**< External clock (non-crystal) mode. */
				EXOSC_MODE_900KHZ_MAX    = 1, /**< External crystal oscillator equal to or slower than 900KHz. */
				EXOSC_MODE_3MHZ_MAX      = 2, /**< External crystal oscillator equal to or slower than 3MHz. */
				EXOSC_MODE_8MHZ_MAX      = 3, /**< External crystal oscillator equal to or slower than 8MHz. */
				EXOSC_MODE_8MHZ_OR_MORE  = 4, /**< External crystal oscillator equal to or faster than 8MHz. */		
			};

			/** Enum for the possible external oscillator statup times. */
			enum Extern_OSC_ClockStartup_t
			{
				EXOSC_START_0CLK         = 0, /**< Immediate startup, no delay. */
				EXOSC_START_64CLK        = 1, /**< Wait 64 clock cyles before startup for stability. */
				EXOSC_START_128CLK       = 2, /**< Wait 128 clock cyles before startup for stability. */
				EXOSC_START_2048CLK      = 3, /**< Wait 2048 clock cyles before startup for stability. */
				EXOSC_START_4096CLK      = 4, /**< Wait 4096 clock cyles before startup for stability. */
				EXOSC_START_8192CLK      = 5, /**< Wait 8192 clock cyles before startup for stability. */
				EXOSC_START_16384CLK     = 6, /**< Wait 16384 clock cyles before startup for stability. */
			};
			
			/** Enum for the possible module clock sources. */
			enum System_ClockSource_t
			{
				CLOCK_SRC_SLOW_CLK       = 0, /**< Clock sourced from the internal slow clock. */
				CLOCK_SRC_OSC0           = 1, /**< Clock sourced from the Oscillator 0 clock. */
				CLOCK_SRC_OSC1           = 2, /**< Clock sourced from the Oscillator 1 clock. */
				CLOCK_SRC_PLL0           = 3, /**< Clock sourced from the PLL 0 clock. */
				CLOCK_SRC_PLL1           = 4, /**< Clock sourced from the PLL 1 clock. */
			};

		/* Inline Functions: */
			/** Starts the given external oscillator of the UC3 microcontroller, with the given options. This routine blocks until
			 *  the oscillator is ready for use.
			 *
			 *  \param[in] Channel  Index of the external oscillator to start.
			 *  \param[in] Type     Type of clock attached to the given oscillator channel, a value from \ref Extern_OSC_ClockTypes_t.
			 *  \param[in] Startup  Statup time of the external oscillator, a value from \ref Extern_OSC_ClockStartup_t.
			 *
			 *  \return Boolean \c true if the external oscillator was successfully started, \c false if invalid parameters specified.
			 */
			static inline void AVR32CLK_StartExternalOscillator(const uint8_t Channel,
			                                                    const uint8_t Type,
			                                                    const uint8_t Startup) ATTR_ALWAYS_INLINE;
			static inline void AVR32CLK_StartExternalOscillator(const uint8_t Channel,
			                                                    const uint8_t Type,
			                                                    const uint8_t Startup)
			{
				switch (Channel)
				{
					case 0:
						AVR32_PM.OSCCTRL0.startup = Startup;
						AVR32_PM.OSCCTRL0.mode    = Type;
						break;
					case 1:
						AVR32_PM.OSCCTRL1.startup = Startup;
						AVR32_PM.OSCCTRL1.mode    = Type;
						break;
					default:
						return false;
				}

				AVR32_PM.mcctrl |= (1 << (AVR32_PM_MCCTRL_OSC0EN_OFFSET + Channel));

				while (!(AVR32_PM.poscsr & (1 << (AVR32_PM_POSCSR_OSC0RDY_OFFSET + Channel))));
				return true;
			}

			/** Stops the given external oscillator of the UC3 microcontroller.
			 *
			 *  \param[in] Channel  Index of the external oscillator to stop.
			 */
			static inline void AVR32CLK_StopExternalOscillator(const uint8_t Channel) ATTR_ALWAYS_INLINE;
			static inline void AVR32CLK_StopExternalOscillator(const uint8_t Channel)
			{
				AVR32_PM.mcctrl &= ~(1 << (AVR32_PM_MCCTRL_OSC0EN_OFFSET + Channel));
			}

			/** Starts the given PLL of the UC3 microcontroller, with the given options. This routine blocks until the PLL is ready for use.
			 *
			 *  \param[in] Channel     Index of the PLL to start.
			 *  \param[in] Source      Clock source for the PLL, a value from \ref System_ClockSource_t.
			 *  \param[in] SourceFreq  Frequency of the PLL's clock source, in Hz.
			 *  \param[in] Frequency   Target frequency of the PLL's output.
			 *
			 *  \return Boolean \c true if the PLL was successfully started, \c false if invalid parameters specified.
			 */
			static inline bool AVR32CLK_StartPLL(const uint8_t Channel,
			                                     const uint8_t Source,
			                                     const uint32_t SourceFreq,
			                                     const uint32_t Frequency) ATTR_ALWAYS_INLINE;
			static inline bool AVR32CLK_StartPLL(const uint8_t Channel,
			                                     const uint8_t Source,
			                                     const uint32_t SourceFreq,
			                                     const uint32_t Frequency)
			{
				switch (Source)
				{
					case CLOCK_SRC_OSC0:
						AVR32_PM.PLL[Channel].pllosc = 0;
						break;
					case CLOCK_SRC_OSC1:
						AVR32_PM.PLL[Channel].pllosc = 1;
						break;
					default:
						return false;
				}

				AVR32_PM.PLL[Channel].pllmul = (Frequency / SourceFreq) ? (((Frequency / SourceFreq) - 1) / 2) : 0;
				AVR32_PM.PLL[Channel].plldiv = 0;
				AVR32_PM.PLL[Channel].pllen  = true;

				while (!(AVR32_PM.poscsr & (1 << (AVR32_PM_POSCSR_LOCK0_OFFSET + Channel))));
				return true;
			}

			/** Stops the given PLL of the UC3 microcontroller.
			 *
			 *  \param[in] Channel  Index of the PLL to stop.
			 */
			static inline void AVR32CLK_StopPLL(const uint8_t Channel) ATTR_ALWAYS_INLINE;
			static inline void AVR32CLK_StopPLL(const uint8_t Channel)
			{
				AVR32_PM.PLL[Channel].pllen = false;
			}
			
			/** Starts the given Generic Clock of the UC3 microcontroller, with the given options.
			 *
			 *  \param[in] Channel     Index of the Generic Clock to start.
			 *  \param[in] Source      Clock source for the Generic Clock, a value from \ref System_ClockSource_t.
			 *  \param[in] SourceFreq  Frequency of the Generic Clock's clock source, in Hz.
			 *  \param[in] Frequency   Target frequency of the Generic Clock's output.
			 *
			 *  \return Boolean \c true if the Generic Clock was successfully started, \c false if invalid parameters specified.
			 */
			static inline bool AVR32CLK_StartGenericClock(const uint8_t Channel,
			                                              const uint8_t Source,
			                                              const uint32_t SourceFreq,
			                                              const uint32_t Frequency) ATTR_ALWAYS_INLINE;
			static inline bool AVR32CLK_StartGenericClock(const uint8_t Channel,
			                                              const uint8_t Source,
			                                              const uint32_t SourceFreq,
			                                              const uint32_t Frequency)
			{
				switch (Source)
				{
					case CLOCK_SRC_OSC0:
						AVR32_PM.GCCTRL[Channel].pllsel = 0;
						AVR32_PM.GCCTRL[Channel].oscsel = 0;
						break;
					case CLOCK_SRC_OSC1:
						AVR32_PM.GCCTRL[Channel].pllsel = 0;
						AVR32_PM.GCCTRL[Channel].oscsel = 1;
						break;
					case CLOCK_SRC_PLL0:
						AVR32_PM.GCCTRL[Channel].pllsel = 1;
						AVR32_PM.GCCTRL[Channel].oscsel = 0;
						break;
					case CLOCK_SRC_PLL1:
						AVR32_PM.GCCTRL[Channel].pllsel = 1;
						AVR32_PM.GCCTRL[Channel].oscsel = 1;
						break;
					default:
						return false;
				}
				
				if (SourceFreq < Frequency)
				  return false;

				AVR32_PM.GCCTRL[Channel].diven = (SourceFreq > Frequency) ? true : false;
				AVR32_PM.GCCTRL[Channel].div   = (((SourceFreq / Frequency) - 1) / 2);
				AVR32_PM.GCCTRL[Channel].cen   = true;
				
				return true;
			}
			
			/** Stops the given generic clock of the UC3 microcontroller.
			 *
			 *  \param[in] Channel  Index of the generic clock to stop.
			 */
			static inline void AVR32CLK_StopGenericClock(const uint8_t Channel) ATTR_ALWAYS_INLINE;
			static inline void AVR32CLK_StopGenericClock(const uint8_t Channel)
			{
				AVR32_PM.GCCTRL[Channel].cen = false;
			}
			
			/** Sets the clock source for the main microcontroller core. The given clock source should be configured
			 *  and ready for use before this function is called.
			 *
			 *  This function will configure the FLASH controller's wait states automatically to suit the given clock source.
			 *
			 *  \param[in] Source      Clock source for the CPU core, a value from \ref System_ClockSource_t.
			 *  \param[in] SourceFreq  Frequency of the CPU core's clock source, in Hz.
			 *
			 *  \return Boolean \c true if the CPU core clock was sucessfully altered, \c false if invalid parameters specified.
			 */
			static inline bool AVR32CLK_SetCPUClockSource(const uint8_t Source,
			                                              const uint32_t SourceFreq) ATTR_ALWAYS_INLINE;
			static inline bool AVR32CLK_SetCPUClockSource(const uint8_t Source,
			                                              const uint32_t SourceFreq)
			{
				AVR32_FLASHC.FCR.fws = (SourceFreq > 30000000) ? true : false;

				switch (Source)
				{
					case CLOCK_SRC_SLOW_CLK:
						AVR32_PM.MCCTRL.mcsel = 0;
						break;
					case CLOCK_SRC_OSC0:
						AVR32_PM.MCCTRL.mcsel = 1;
						break;
					case CLOCK_SRC_PLL0:
						AVR32_PM.MCCTRL.mcsel = 2;
						break;
					default:
						return false;
				}
				
				return true;
			}

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */
