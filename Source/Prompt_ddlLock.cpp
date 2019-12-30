/*============================================================================*/
/* Prompt_ddlLock.cpp -- ddlLock inspection                                   */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_ddlLock (
  PF_DDLLOCK,            // prompt function
  "Prompt_ddlLock"_Stz,  // name of the prompt function
  "comment"_Stz,         // comment about the function
  "ddlLock"_Stz,         // the prompt command
  nullptr,               // the second word
  bPrompt_ddlLock,       // prompt function
  true,                  // enabled

  true,                  // visible
  "ddlLock"_Stz,         // help
  ""_Stz                 // description
);


/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ddlLock(SePrompt* pPrompt)
{
  vTrace("  DdlLock : %016x\n"_Stz,AM::pDdlLock()->uLockWord());

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
