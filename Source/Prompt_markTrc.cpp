/*============================================================================*/
/* Prompt_markTrc.cpp -- the mark trace option                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_markTrc (
  PF_NONE,                     // prompt function
  "Prompt_markTrc"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "markTrc"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_markTrc,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern AttachMap*    gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_markTrc(SePrompt* pPrompt)
{
  PromptHelper::vEnumFlags(gpAttachMap->m_oMarkTrace,
                           "Trc"_Stz,
                           aCN,
                           CN_ENUMS,
                           pPrompt->pTknVec());
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
