/*============================================================================*/
/* Prompt_olist.cpp -- originator list report                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_olist (
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

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_olist(SePrompt* pPrompt)
{
  vTrace("------------------------------------\n"_Stz);
  vTrace("Originator List\n"_Stz);
  vTrace("------------------------------------\n"_Stz);
  
  gpAttachMap->m_pOriginatingJobs->vGrabLock();
  gpAttachMap->m_pOriginatingJobs->vContentDump();
  gpAttachMap->m_pOriginatingJobs->vDropLock();

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
