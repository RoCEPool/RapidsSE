/*============================================================================*/
/* Prompt_logNote.cpp -- write a note into the log                            */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_logNote (
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
  TokenVec* pTokenVec = pPrompt->pTokenVec();

  ASSERT((*pTokenVec)[0]->bSame("logNote"_Stz));
  
  if (pTokenVec->nTokens() > 1)
    AM::vLogPrm(SL_INF,pPrompt,PA_PRM_LOG_NOTE,
                "%s"_Stz, //!format-LogPrm
                pTokenVec->pGet(1)->szTerm());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
