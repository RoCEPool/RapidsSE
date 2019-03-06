/*============================================================================*/
/* Prompt_version.cpp -- get SE version                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_version (
  PF_NONE,                     // prompt function
  "Prompt_version"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "version"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_version,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_version(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();
  /*
  args.metadata if
    "(version VARCHAR)" metadata!
    "')" version "row: ('" "%v%v%v\n" str.format
    1 task::this task.session@ session.rowsProcessed
    else
      version
      then
   */
  ASSERT((*pTokenVec)[0]->bSame("version"_Stz));
  
  if (gpAttachMap->m_bMetadata) {
    vTrace("metadata: (version VARCHAR)\n"_Stz);
    vInfo("row: ('%s')\n"_Stz, (Stz*)RAPIDSSE_VERSION_STRING);
    vTrace("success: 1 rows affected\n"_Stz);
  }
  else
    vInfo("%s\n"_Stz, (Stz*)RAPIDSSE_VERSION_STRING);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
