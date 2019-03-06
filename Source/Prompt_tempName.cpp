/*============================================================================*/
/* Prompt_tempName.cpp -- show tempName                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*   gpAttachMap;

PromptNote mPromptNote_tempName (
  PF_TEMPNAME,                     // prompt function
  "Prompt_tempName"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the functopm
  "tempName"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_tempName,      // prompt function
  true,                  // enabled

  true,                  // visible
  "tempName"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tempName(SePrompt* pPrompt)
{
  s32 sName;
  
  for (i8 n = 0; n < 1000; n++) {
    gpAttachMap->m_oTempName.vNextName(sName);
    vTrace("%s\n"_Stz,sName.aStz());
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
