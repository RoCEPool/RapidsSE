/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

#include "Dir.hpp" //?

PromptFun __Prompt_ls (
  PF_LS,             // prompt function
  "Prompt_ls"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the function
  "ls"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_ls,        // prompt function
  true,              // enabled

  true,              // visible
  "ls"_Stz,          // help
  ""_Stz             // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ls(SePrompt* pPrompt)
{
  Dir* pDir;
  s256 sDirPath;
  s256 sEntryNm;
  i8 iEntryType;
  
  pDir = new Dir(pPrompt);
                 
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("ls"_Stz));
  
  // get the current directory
  if (!Dir::bGetCurrent(sDirPath))
    return false;
  
  // open the current directory
  if (!pDir->bOpen(sDirPath))
    return false;
  
  // read the current directory and list contents
  while (pDir->bRead()) {
    sEntryNm = pDir->szEntryNm();
    iEntryType = pDir->iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sEntryNm.azChar());
  }
  
  // close the current directory
  pDir->vClose();
  
  delete pDir;
                 
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
