/*============================================================================*/
/* Prompt_olist.cpp -- originator list report                                 */
/*============================================================================*/

#include "Prompt.h"


PromptFun __Prompt_olist (
  PF_OLIST,             // prompt function
  "Prompt_olist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "olist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_olist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "olist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_olist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Originator List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  AM::pOriginatingJobs()->vGrabLock();
  AM::pOriginatingJobs()->vContentDump();
  AM::pOriginatingJobs()->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
