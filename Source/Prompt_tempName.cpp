/*============================================================================*/
/* Prompt_tempName.cpp -- show tempName                                             */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*   gpAttachMap;

PromptNote mPromptNote_tempName (
  "Prompt_tempName"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "tempName"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_tempName,      // prompt function
  true,                  // enabled

  true,                  // visible
  "fun help"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tempName(SePrompt* pPrompt)
{
  s32 sName;
  
  gpAttachMap->m_oTempName.vNextName(sName);
  
  vTrace("%s\n"_Stz,sName.aStz());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
