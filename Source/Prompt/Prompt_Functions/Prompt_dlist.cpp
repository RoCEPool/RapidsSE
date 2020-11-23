/*============================================================================*/
/* Prompt_dlist.cpp -- demon list report                                      */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_dlist (
  PF_OLIST,             // prompt function
  "Prompt_dlist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "dlist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_dlist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "dlist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_dlist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Demon List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  AM::pDemonJobs()->vGrabLock();
  AM::pDemonJobs()->vContentDump();
  AM::pDemonJobs()->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
