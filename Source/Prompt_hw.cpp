/*============================================================================*/
/* Prompt_hw.cpp -- print hardware related information                        */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_hw (
  PF_HW,                // prompt function
  "Prompt_hw"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "hw"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_hw,           // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hw(SePrompt* pPrompt)
{
  s32 sMem;
  Magnitudes::vByteMag(Hardware::iPhysicalMemory(),sMem);

  s32 sClock;
  Magnitudes::vHzMag(Hardware::iClockFrequency(),sClock);

  unsigned int x = 0x1;
  char *c = (char*) &x;
  
  vTrace("hw: %lld cores; %s memory; %s clock frequency; %s\n"_Stz,
         Hardware::iCoreCount(),
         sMem.aStz(),
         sClock.aStz(),
         *c == 0x1 ? "little endian"_Stz : "big endian"_Stz
         );
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
