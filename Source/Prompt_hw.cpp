/*============================================================================*/
/* Prompt_hw.cpp -- print hardware related information                        */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_hw (
  PF_HW,                // prompt function
  "Prompt_hw"_Stz,      // name of the prompt function
  "comment"_Stz,        // comment about the function
  "hw"_Stz,             // the prompt command
  nullptr,              // the second word
  bPrompt_hw,           // prompt function
  true,                 // enabled

  true,                 // visible
  "hw"_Stz,             // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_hw(SePrompt* pPrompt)
{
  s32 sMem;
  MagLib::vByteMag(Hardware::iPhysicalMemory(),sMem);

  s32 sCpuFreq;
  MagLib::vHzMag(Hardware::iCpuFrequency(),sCpuFreq);

  unsigned int x = 0x1;
  char *c = (char*) &x;
  
  vTrace("hw: %lld cores; %s memory; %s cpu frequency; %s\n"_Stz,
         Hardware::iCoreCount(),
         sMem.szStz(),
         sCpuFreq.szStz(),
         *c == 0x1 ? "little endian"_Stz : "big endian"_Stz
         );
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
