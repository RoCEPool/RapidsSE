/*============================================================================*/
/* Prompt_logNote.cpp -- write a note into the log                            */
/*============================================================================*/

#include "Prompt.h"

PromptDesc __Prompt_logNote (
  PF_LOGNOTE,           // prompt function
  "Prompt_logNote"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "logNote"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_logNote,      // prompt function
  true,                 // enabled

  true,                 // visible
  "logNote"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_logNote(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("logNote"_Stz));
  
  if (pTokenVec->nTokens() > 1)
    Log::vNote(pTokenVec->pGet(1)->szText());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
