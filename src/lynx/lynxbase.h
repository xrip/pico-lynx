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

//
// Generic Lynx base class.
//

#ifndef LYNXBASE_H
#define LYNXBASE_H

// bank0	- Cartridge bank 0
// bank1	- Cartridge bank 1
// ram		- all ram
// cpu		- system memory as viewed by the cpu

enum EMMODE {bank0,bank1,ram,cpu};

class CLynxBase
{
   // Function members

   public:
      virtual ~CLynxBase() {};

   public:
      virtual void	Reset(void) {};
      virtual bool	ContextLoad(FILE *fp) { return 0; };
      virtual bool	ContextSave(FILE *fp) { return 0; };

      virtual void	Poke(ULONG2 addr, UBYTE data)=0;
      virtual UBYTE	Peek(ULONG2 addr)=0;
      virtual void	PokeW(ULONG2 addr, UWORD data) {};	// ONLY mSystem overloads these, they are never use by the clients
      virtual UWORD	PeekW(ULONG2 addr) {return 0;};
      virtual void	BankSelect(EMMODE newbank){};
      virtual ULONG2	ObjectSize(void) {return 1;};
      virtual ULONG2	ReadCycle(void) {return 5;};
      virtual ULONG2	WriteCycle(void) {return 5;};

};

#endif
