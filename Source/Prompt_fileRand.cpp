/*============================================================================*/
/* Prompt_fileRand.cpp -- file randomise                                      */
/*============================================================================*/

#include "Prompt.h"

PromptFun __Prompt_fileRand (
  PF_FILERAND,           // prompt function
  "Prompt_fileRand"_Stz, // name of the prompt function
  "comment"_Stz,         // comment about the function
  "fileRand"_Stz,        // the prompt command
  nullptr,               // the second word
  bPrompt_fileRand,      // prompt function
  true,                  // enabled

  true,                  // visible
  "fileRand"_Stz,        // help
  ""_Stz                 // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_fileRand(SePrompt* pPrompt)
{
  s256 sPathIn;
  s256 sPathOut;

  TokenVec* pInpTknVec = pPrompt->pInpTknVec();
  
  ASSERT(pInpTknVec->pGet(0)->bSame("fileRand"_Stz));
  
  sPathIn  = pInpTknVec->pGet(1)->szChars();
  sPathOut = pInpTknVec->pGet(2)->szChars();

  vTrace("fileRand %s into %s\n"_Stz,sPathIn.szStz(),sPathOut.szStz());
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
