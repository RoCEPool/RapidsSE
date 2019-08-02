/*============================================================================*/
/* Prompt_logSection.cpp -- write a section line into the log                 */
/*============================================================================*/

#include "Prompt.h"

PromptDesc mPromptNote_logSection (
  PF_LOGSECTION,           // prompt function
  "Prompt_logSection"_Stz, // name of the prompt function
  "comment"_Stz,           // comment about the function
  "logSection"_Stz,        // the prompt command
  nullptr,                 // the second word
  bPrompt_logSection,      // prompt function
  true,                    // enabled

  true,                    // visible
  "logSection"_Stz,        // help
  ""_Stz                   // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_logSection(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  if (pTokenVec->nTokens() > 1) {

    Stz* szSectionNm = pTokenVec->pGet(1)->aText();
    Log::vNoteSection(szSectionNm);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
