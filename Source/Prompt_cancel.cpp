/*============================================================================*/
/* Prompt_cancel.cpp -- cancel a pipeline job                                 */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_cancel (
  PF_CANCEL,            // prompt function
  "Prompt_cancel"_Stz, // name of the prompt function
  "comment"_Stz,       // comment about the function
  "cancel"_Stz,        // the prompt command
  nullptr,             // the second word
  bPrompt_cancel,      // prompt function
  true,                // enabled

  true,              // visible
  "cancel"_Stz,      // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_cancel(SePrompt* pPrompt)
{
  s256 sFile;

  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("cancel"_Stz));
  
  i8 iJobId = pTokenVec->pGet(1)->iToI8();

  vTrace("Cancelling job %lld\n"_Stz, iJobId);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
