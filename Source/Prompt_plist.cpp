/*============================================================================*/
/* Prompt_plist.cpp -- participant list report                                */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_plist (
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

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_plist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Participant List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  gpAttachMap->m_pParticipatingJobs->vGrabLock();
  gpAttachMap->m_pParticipatingJobs->vContentDump();
  gpAttachMap->m_pParticipatingJobs->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
