/*============================================================================*/
/* Prompt_tempName.cpp -- show tempName                                       */
/*============================================================================*/

#include "StorageEngine.h"


PromptFun __Prompt_tempName (
  PF_TEMPNAME,           // prompt function
  "Prompt_tempName"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "tempName"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_tempName,      // prompt function
  true,                  // enabled

  true,                  // visible
  "tempName"_Stz,        // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_tempName(SePrompt* pPrompt)
{
  s32 sName;
  
  for (i8 n = 0; n < 1000; n++) {
    AM::vNextTempName(sName);
    vTrace("%s\n"_Stz,sName.szStz());
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
