/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_ls (
  "Prompt_ls"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "ls"_Stz,          // the prompt command
  nullptr,           // the second word
  bPrompt_ls,        // prompt function
  true,              // enabled

  true,              // visible
  "fun help"_Stz     // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_ls(SePrompt* pPrompt)
{
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("ls"_Stz));
  
  // get the current directory
  if (!xDir.bGetCurrent(sDir)) {
    return false;
  }
  
  // open the current directory
  if (!xDir.bOpen(sDir)) {
    return false;
  }
  
  // read the current directory
  while (xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.aChar());
  }
  
  // close the current directory
  xDir.vClose();
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
