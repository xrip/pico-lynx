//
// Copyright (c) 2004 K. Wilkins
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//

//////////////////////////////////////////////////////////////////////////////
//                       Handy - An Atari Lynx Emulator                     //
//                          Copyright (c) 1996,1997                         //
//                                 K. Wilkins                               //
//////////////////////////////////////////////////////////////////////////////
// Mikey class header file                                                  //
//////////////////////////////////////////////////////////////////////////////
//                                                                          //
// This header file provides the interface definition and some of the code  //
// for the Mikey chip within the Lynx. The most crucial code is the         //
// Update() function which as you can probably guess updates all of the     //
// Mikey hardware counters and screen DMA from the prevous time it was      //
// called. Yes I know how to spell Mikey but I cant be bothered to change   //
// it everywhere.                                                           //
//                                                                          //
//    K. Wilkins                                                            //
// August 1997                                                              //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////
// Revision History:                                                        //
// -----------------                                                        //
//                                                                          //
// 01Aug1997 KW Document header added & class documented.                   //
//                                                                          //
//////////////////////////////////////////////////////////////////////////////

#ifndef MIKIE_H
#define MIKIE_H

//#define	TRACE_MIKIE

#ifdef TRACE_MIKIE

#define TRACE_MIKIE0(msg)					_RPT1(_CRT_WARN,"CMikie::"msg" (Time=%012d)\n",gSystemCycleCount)
#define TRACE_MIKIE1(msg,arg1)				_RPT2(_CRT_WARN,"CMikie::"msg" (Time=%012d)\n",arg1,gSystemCycleCount)
#define TRACE_MIKIE2(msg,arg1,arg2)			_RPT3(_CRT_WARN,"CMikie::"msg" (Time=%012d)\n",arg1,arg2,gSystemCycleCount)
#define TRACE_MIKIE3(msg,arg1,arg2,arg3)	_RPT4(_CRT_WARN,"CMikie::"msg" (Time=%012d)\n",arg1,arg2,arg3,gSystemCycleCount)

#else

#define TRACE_MIKIE0(msg)
#define TRACE_MIKIE1(msg,arg1)
#define TRACE_MIKIE2(msg,arg1,arg2)
#define TRACE_MIKIE3(msg,arg1,arg2,arg3)

#endif

class CSystem;

#define MIKIE_START	0xfd00
#define MIKIE_SIZE	0x100

#define DMA_RDWR_CYC	4

//
// Define counter types and defines
//

#define CTRL_A_IRQEN	0x80
#define CTRL_A_RTD		0x40
#define CTRL_A_RELOAD	0x10
#define CTRL_A_COUNT	0x08
#define CTRL_A_DIVIDE	0x07

#define CTRL_B_TDONE	0x08
#define CTRL_B_LASTCK	0x04
#define CTRL_B_CIN		0x02
#define CTRL_B_COUT		0x01

#define LINE_TIMER		0x00
#define SCREEN_TIMER	0x02

#define LINE_WIDTH		160
#define LINE_SIZE		80

#define UART_TX_INACTIVE	0x80000000
#define UART_RX_INACTIVE	0x80000000
#define UART_BREAK_CODE		0x00008000
#define UART_MAX_RX_QUEUE	32
#define UART_TX_TIME_PERIOD	(11)
#define UART_RX_TIME_PERIOD	(11)
#define UART_RX_NEXT_DELAY	(44)

typedef struct
{
   union
   {
      struct
      {
#ifdef MSB_FIRST
         UBYTE unused:8;
         UBYTE unused2:8;
         UBYTE unused3:4;
         UBYTE Blue:4;
         UBYTE Red:4;
         UBYTE Green:4;
#else
         UBYTE Green:4;
         UBYTE Red:4;
         UBYTE Blue:4;
#endif
      }Colours;
      ULONG2     Index;
   };
}TPALETTE;


//
// Enumerated types for possible mikie windows independant modes
//
enum
{
   MIKIE_BAD_MODE=0,
   MIKIE_NO_ROTATE,
   MIKIE_ROTATE_L,
   MIKIE_ROTATE_R
};

enum
{
   MIKIE_PIXEL_FORMAT_16BPP_565=0,
   MIKIE_PIXEL_FORMAT_16BPP_565_BE
};

class CMikie : public CLynxBase
{
   public:
      CMikie(CSystem& parent, ULONG2 displayformat, ULONG2 samplerate);
      ~CMikie();

      bool	ContextSave(LSS_FILE *fp);
      bool	ContextLoad(LSS_FILE *fp);
      void	Reset(void);

      UBYTE	Peek(ULONG2 addr);
      void	Poke(ULONG2 addr, UBYTE data);
      ULONG2	ObjectSize(void) {return MIKIE_SIZE;};
      void	PresetForHomebrew(void);

      void	ComLynxCable(int status);
      void	ComLynxRxData(int data);
      void	ComLynxTxLoopback(int data);
      void	ComLynxTxCallback(void (*function)(int data, ULONG2 objref), ULONG2 objref);

      void	BuildPalette(void);

      void Update(void);
      void SetRotation(UBYTE data) {mDisplayRotate_Pending = data;};
      inline bool SwitchAudInDir(void){ return(mIODIR&0x10);};
      inline bool SwitchAudInValue(void){ return (mIODAT&0x10);};

   private:
      inline void UpdateSound(void);
      inline void UpdateCalcSound(void);
      inline void ResetDisplayPtr();
      ULONG2	DisplayRenderLine(void);
      void	BlowOut(void);

   private:
      CSystem		&mSystem;

      // Hardware storage

      ULONG2		mDisplayAddress;
      ULONG2		mAudioInputComparator;
      ULONG2		mTimerStatusFlags;
      ULONG2		mTimerInterruptMask;
      UBYTE    mDisplayRotate;
      UBYTE    mDisplayRotate_Pending;
   UWORD		mColourMap[4096];
      TPALETTE	mPalette[16];


      ULONG2		mIODAT;
      ULONG2		mIODIR;
      ULONG2		mIODAT_REST_SIGNAL;

      ULONG2		mDISPCTL_DMAEnable;
      ULONG2		mDISPCTL_Flip;
      ULONG2		mDISPCTL_FourColour;
      ULONG2		mDISPCTL_Colour;

      ULONG2		mTIM_0_BKUP;
      ULONG2		mTIM_0_ENABLE_RELOAD;
      ULONG2		mTIM_0_ENABLE_COUNT;
      ULONG2		mTIM_0_LINKING;
      ULONG2		mTIM_0_CURRENT;
      ULONG2		mTIM_0_TIMER_DONE;
      ULONG2		mTIM_0_LAST_CLOCK;
      ULONG2		mTIM_0_BORROW_IN;
      ULONG2		mTIM_0_BORROW_OUT;
      ULONG2		mTIM_0_LAST_LINK_CARRY;
      ULONG2		mTIM_0_LAST_COUNT;

      ULONG2		mTIM_1_BKUP;
      ULONG2		mTIM_1_ENABLE_RELOAD;
      ULONG2		mTIM_1_ENABLE_COUNT;
      ULONG2		mTIM_1_LINKING;
      ULONG2		mTIM_1_CURRENT;
      ULONG2		mTIM_1_TIMER_DONE;
      ULONG2		mTIM_1_LAST_CLOCK;
      ULONG2		mTIM_1_BORROW_IN;
      ULONG2		mTIM_1_BORROW_OUT;
      ULONG2		mTIM_1_LAST_LINK_CARRY;
      ULONG2		mTIM_1_LAST_COUNT;

      ULONG2		mTIM_2_BKUP;
      ULONG2		mTIM_2_ENABLE_RELOAD;
      ULONG2		mTIM_2_ENABLE_COUNT;
      ULONG2		mTIM_2_LINKING;
      ULONG2		mTIM_2_CURRENT;
      ULONG2		mTIM_2_TIMER_DONE;
      ULONG2		mTIM_2_LAST_CLOCK;
      ULONG2		mTIM_2_BORROW_IN;
      ULONG2		mTIM_2_BORROW_OUT;
      ULONG2		mTIM_2_LAST_LINK_CARRY;
      ULONG2		mTIM_2_LAST_COUNT;

      ULONG2		mTIM_3_BKUP;
      ULONG2		mTIM_3_ENABLE_RELOAD;
      ULONG2		mTIM_3_ENABLE_COUNT;
      ULONG2		mTIM_3_LINKING;
      ULONG2		mTIM_3_CURRENT;
      ULONG2		mTIM_3_TIMER_DONE;
      ULONG2		mTIM_3_LAST_CLOCK;
      ULONG2		mTIM_3_BORROW_IN;
      ULONG2		mTIM_3_BORROW_OUT;
      ULONG2		mTIM_3_LAST_LINK_CARRY;
      ULONG2		mTIM_3_LAST_COUNT;

      ULONG2		mTIM_4_BKUP;
      ULONG2		mTIM_4_ENABLE_RELOAD;
      ULONG2		mTIM_4_ENABLE_COUNT;
      ULONG2		mTIM_4_LINKING;
      ULONG2		mTIM_4_CURRENT;
      ULONG2		mTIM_4_TIMER_DONE;
      ULONG2		mTIM_4_LAST_CLOCK;
      ULONG2		mTIM_4_BORROW_IN;
      ULONG2		mTIM_4_BORROW_OUT;
      ULONG2		mTIM_4_LAST_LINK_CARRY;
      ULONG2		mTIM_4_LAST_COUNT;

      ULONG2		mTIM_5_BKUP;
      ULONG2		mTIM_5_ENABLE_RELOAD;
      ULONG2		mTIM_5_ENABLE_COUNT;
      ULONG2		mTIM_5_LINKING;
      ULONG2		mTIM_5_CURRENT;
      ULONG2		mTIM_5_TIMER_DONE;
      ULONG2		mTIM_5_LAST_CLOCK;
      ULONG2		mTIM_5_BORROW_IN;
      ULONG2		mTIM_5_BORROW_OUT;
      ULONG2		mTIM_5_LAST_LINK_CARRY;
      ULONG2		mTIM_5_LAST_COUNT;

      ULONG2		mTIM_6_BKUP;
      ULONG2		mTIM_6_ENABLE_RELOAD;
      ULONG2		mTIM_6_ENABLE_COUNT;
      ULONG2		mTIM_6_LINKING;
      ULONG2		mTIM_6_CURRENT;
      ULONG2		mTIM_6_TIMER_DONE;
      ULONG2		mTIM_6_LAST_CLOCK;
      ULONG2		mTIM_6_BORROW_IN;
      ULONG2		mTIM_6_BORROW_OUT;
      ULONG2		mTIM_6_LAST_LINK_CARRY;
      ULONG2		mTIM_6_LAST_COUNT;

      ULONG2		mTIM_7_BKUP;
      ULONG2		mTIM_7_ENABLE_RELOAD;
      ULONG2		mTIM_7_ENABLE_COUNT;
      ULONG2		mTIM_7_LINKING;
      ULONG2		mTIM_7_CURRENT;
      ULONG2		mTIM_7_TIMER_DONE;
      ULONG2		mTIM_7_LAST_CLOCK;
      ULONG2		mTIM_7_BORROW_IN;
      ULONG2		mTIM_7_BORROW_OUT;
      ULONG2		mTIM_7_LAST_LINK_CARRY;
      ULONG2		mTIM_7_LAST_COUNT;

      ULONG2		mAUDIO_0_BKUP;
      ULONG2		mAUDIO_0_ENABLE_RELOAD;
      ULONG2		mAUDIO_0_ENABLE_COUNT;
      ULONG2		mAUDIO_0_LINKING;
      ULONG2		mAUDIO_0_CURRENT;
      ULONG2		mAUDIO_0_TIMER_DONE;
      ULONG2		mAUDIO_0_LAST_CLOCK;
      ULONG2		mAUDIO_0_BORROW_IN;
      ULONG2		mAUDIO_0_BORROW_OUT;
      ULONG2		mAUDIO_0_LAST_LINK_CARRY;
      ULONG2		mAUDIO_0_LAST_COUNT;
      SBYTE		mAUDIO_0_VOLUME;
      ULONG2		mAUDIO_0_INTEGRATE_ENABLE;
      ULONG2		mAUDIO_0_WAVESHAPER;

      ULONG2		mAUDIO_1_BKUP;
      ULONG2		mAUDIO_1_ENABLE_RELOAD;
      ULONG2		mAUDIO_1_ENABLE_COUNT;
      ULONG2		mAUDIO_1_LINKING;
      ULONG2		mAUDIO_1_CURRENT;
      ULONG2		mAUDIO_1_TIMER_DONE;
      ULONG2		mAUDIO_1_LAST_CLOCK;
      ULONG2		mAUDIO_1_BORROW_IN;
      ULONG2		mAUDIO_1_BORROW_OUT;
      ULONG2		mAUDIO_1_LAST_LINK_CARRY;
      ULONG2		mAUDIO_1_LAST_COUNT;
      SBYTE		mAUDIO_1_VOLUME;
      ULONG2		mAUDIO_1_INTEGRATE_ENABLE;
      ULONG2		mAUDIO_1_WAVESHAPER;

      ULONG2		mAUDIO_2_BKUP;
      ULONG2		mAUDIO_2_ENABLE_RELOAD;
      ULONG2		mAUDIO_2_ENABLE_COUNT;
      ULONG2		mAUDIO_2_LINKING;
      ULONG2		mAUDIO_2_CURRENT;
      ULONG2		mAUDIO_2_TIMER_DONE;
      ULONG2		mAUDIO_2_LAST_CLOCK;
      ULONG2		mAUDIO_2_BORROW_IN;
      ULONG2		mAUDIO_2_BORROW_OUT;
      ULONG2		mAUDIO_2_LAST_LINK_CARRY;
      ULONG2		mAUDIO_2_LAST_COUNT;
      SBYTE		mAUDIO_2_VOLUME;
      ULONG2		mAUDIO_2_INTEGRATE_ENABLE;
      ULONG2		mAUDIO_2_WAVESHAPER;

      ULONG2		mAUDIO_3_BKUP;
      ULONG2		mAUDIO_3_ENABLE_RELOAD;
      ULONG2		mAUDIO_3_ENABLE_COUNT;
      ULONG2		mAUDIO_3_LINKING;
      ULONG2		mAUDIO_3_CURRENT;
      ULONG2		mAUDIO_3_TIMER_DONE;
      ULONG2		mAUDIO_3_LAST_CLOCK;
      ULONG2		mAUDIO_3_BORROW_IN;
      ULONG2		mAUDIO_3_BORROW_OUT;
      ULONG2		mAUDIO_3_LAST_LINK_CARRY;
      ULONG2		mAUDIO_3_LAST_COUNT;
      SBYTE		mAUDIO_3_VOLUME;
      ULONG2		mAUDIO_3_INTEGRATE_ENABLE;
      ULONG2		mAUDIO_3_WAVESHAPER;

      SBYTE		mAUDIO_OUTPUT[4];
      UBYTE    mAUDIO_ATTEN[4];
      ULONG2		mSTEREO;
      ULONG2		mPAN;

      //
      // Serial related variables
      //
      ULONG2		mUART_RX_IRQ_ENABLE;
      ULONG2		mUART_TX_IRQ_ENABLE;

      ULONG2		mUART_RX_COUNTDOWN;
      ULONG2		mUART_TX_COUNTDOWN;

      ULONG2		mUART_SENDBREAK;
      ULONG2		mUART_TX_DATA;
      ULONG2		mUART_RX_DATA;
      ULONG2		mUART_RX_READY;

      ULONG2		mUART_PARITY_ENABLE;
      ULONG2		mUART_PARITY_EVEN;

      SLONG		mUART_CABLE_PRESENT;
      void		(*mpUART_TX_CALLBACK)(int data, ULONG2 objref);
      ULONG2		mUART_TX_CALLBACK_OBJECT;

      SLONG		mUART_Rx_input_queue[UART_MAX_RX_QUEUE];
      ULONG2 	mUART_Rx_input_ptr;
      ULONG2 	mUART_Rx_output_ptr;
      SLONG		mUART_Rx_waiting;
      SLONG		mUART_Rx_framing_error;
      SLONG		mUART_Rx_overun_error;

      //
      // Screen related
      //

      UBYTE		*mpDisplayCurrent;
      UBYTE		*mpRamPointer;
      ULONG2		mLynxLine;
      ULONG2		mLynxLineDMACounter;
      ULONG2		mLynxAddr;

      ULONG2		mAudioSampleRate;
      ULONG2		mDisplayFormat;
      ULONG2		mDisplayPitch;
};


#endif
