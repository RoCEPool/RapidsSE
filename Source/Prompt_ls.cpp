/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_ls (
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
  Dir  oDir;
  s256 sDirPath;
  s256 sEntryName;
  i8 iEntryType;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("ls"_Stz));
  
  // get the current directory
  if (!Dir::bGetCurrent(sDirPath))
    return false;
  
  // open the current directory
  if (!oDir.bOpen(sDirPath))
    return false;
  
  // read the current directory and list contents
  while (oDir.bRead()) {
    sEntryName = oDir.sEntryName();
    iEntryType = oDir.iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sEntryName.aChar());
  }
  
  // close the current directory
  oDir.vClose();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
