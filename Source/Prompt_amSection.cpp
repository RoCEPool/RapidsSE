/*============================================================================*/
/* Prompt_amSection.cpp -- write a section mark into the activity map         */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_amSection (
  PF_AMSECTION,           // prompt function
  "Prompt_amSection"_Stz, // name of the prompt function
  "comment"_Stz,          // comment about the functopm
  "amSection"_Stz,        // the prompt command
  nullptr,                // the second word
  bPrompt_amSection,      // prompt function
  true,                   // enabled

  true,                   // visible
  "amSection"_Stz         // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_amSection(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  if (pTokenVec->nTokens() > 1) {

    Stz* szSectionNm = pTokenVec->pGet(1)->aText();
    ActivityMap::vMarkSection(szSectionNm);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
