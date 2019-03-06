/*============================================================================*/
/* Prompt_uptime.cpp -- uptime prompt function                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_uptime (
  PF_UPTIME,             // prompt function
  "Prompt_uptime"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "uptime"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_uptime,        // prompt function
  true,                  // enabled

  true,                  // visible
  "uptime"_Stz           // help
);

extern AttachMap* gpAttachMap;
extern PromptCatalog gxPromptCatalog;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_uptime(SePrompt* pPrompt)
{
  Since oSince;
  oSince.vNow();
  
  i8 iUpTimeSsec = oSince.iSeconds() - gpAttachMap->m_oSince.iSeconds();

  vTrace("  %llds\n"_Stz,iUpTimeSsec);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
