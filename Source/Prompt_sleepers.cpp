/*============================================================================*/
/* Prompt_sleepers.cpp -- sleepers statistics                                 */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_sleepers (
  PF_SLEEPERS,           // prompt function
  "Prompt_sleepers"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "sleepers"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_sleepers,      // prompt function
  true,                  // enabled

  true,                  // visible
  "sleepers"_Stz,        // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_sleepers(SePrompt* pPrompt)
{
  bool bReset = false;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();
  i8 iNumTokens = pTokenVec->nItems();
  if (3 == iNumTokens)
    bReset = pTokenVec->pGet(2)->bSame("reset"_Stz);

  Pthread::vSleepers();
  
  if (bReset)
    MemLib::vReset();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
