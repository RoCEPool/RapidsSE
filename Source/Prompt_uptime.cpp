/*============================================================================*/
/* Prompt_uptime.cpp -- uptime prompt function                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_uptime (
  PF_UPTIME,             // prompt function
  "Prompt_uptime"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "uptime"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_uptime,        // prompt function
  true,                  // enabled

  true,                  // visible
  "uptime"_Stz,          // help
  ""_Stz                 // description
);

extern AttachMap* gpAttachMap;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_uptime(SePrompt* pPrompt)
{
  u8 uUpTimeSsec = gpAttachMap->m_oBirth.uSecondsElapsed();

  vTrace("  %llds\n"_Stz,uUpTimeSsec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
