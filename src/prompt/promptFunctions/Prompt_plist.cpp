/*============================================================================*/
/* Prompt_plist.cpp -- participant list report                                */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_plist (
  PF_PLIST,             // prompt function
  "Prompt_plist"_Stz,   // name of the prompt function
  "comment"_Stz,        // comment about the function
  "plist"_Stz,          // the prompt command
  nullptr,              // the second word
  bPrompt_plist,        // prompt function
  true,                 // enabled

  true,                 // visible
  "plist"_Stz,          // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_plist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Participant List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  AM::pParticipatingJobs()->vGrabLock();
  AM::pParticipatingJobs()->vContentDump();
  AM::pParticipatingJobs()->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
