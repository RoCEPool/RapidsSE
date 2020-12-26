/*============================================================================*/
/* Prompt_logNote.cpp -- write a note into the log                            */
/*============================================================================*/

#include "StorageEngine.h"

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
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("logNote"_Stz));
  
  if (pInpTknVec->nTokens() > 1)
    AM::vLogPrm(SL_INF,pPrompt,PA_PRM_LOG_NOTE,
                "%s"_Stz, //!format-LogPrm
                pInpTknVec->pGet(1)->szChars());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
