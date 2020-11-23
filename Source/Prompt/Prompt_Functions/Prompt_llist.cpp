/*============================================================================*/
/* Prompt_llist.cpp -- local list report                                      */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_llist (
  PF_OLIST,             // prompt function
  "Prompt_llist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "llist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_llist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "llist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_llist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Local List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  AM::pLocalJobs()->vGrabLock();
  AM::pLocalJobs()->vContentDump();
  AM::pLocalJobs()->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
