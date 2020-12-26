/*============================================================================*/
/* Prompt_version.cpp -- get SE version                                       */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_version (
  PF_VERSION,           // prompt function
  "Prompt_version"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the function
  "version"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_version,      // prompt function
  true,                 // enabled

  true,                 // visible
  "version"_Stz,        // help
  ""_Stz                // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_version(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  /*
  args.metadata if
    "(version VARCHAR)" metadata!
    "')" version "row: ('" "%v%v%v\n" str.format
    1 task::this task.session@ session.rowsProcessed
    else
      version
      then
   */
  ASSERT(pInpTknVec->pGet(0)->bSame("version"_Stz));
  
  if (AM::bMetadata()) {
    vTrace("\nmetadata: (version VARCHAR)\n\n"_Stz);
    vInfo("row: ('%s')\n"_Stz, (Stz*)RAPIDSSE_VERSION_STRING);
    vTrace("\nsuccess: 1 rows affected\n"_Stz);
  }
  else
    vInfo("%s\n"_Stz, (Stz*)RAPIDSSE_VERSION_STRING);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
