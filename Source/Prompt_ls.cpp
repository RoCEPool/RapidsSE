/*============================================================================*/
/* Prompt_ls.cpp -- list directory contents                                   */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_ls (
  "Prompt_ls"_Stz,   // name of the prompt function
  "comment"_Stz,     // comment about the functopm
  "ls"_Stz,          // the prompt command
  nullptr,           // the second word
  vPrompt_ls,        // prompt post-process function
  true               // enabled
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
void Prompt::vPrompt_ls(SePrompt* pPrompt)
{
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  TokenVec* pTokenVec = pPrompt->pTokenVec();
  
  ASSERT((*pTokenVec)[0]->bSame("ls"_Stz));
  
  // get the current directory
  if (!xDir.bGetCurrent(sDir)) {
    //m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return;
  }
  
  // open the current directory
  if (!xDir.bOpen()) {
    //m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return;
  }
  
  // read the current directory
  while (xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.aChar());
  }
  
  // close the current directory
  xDir.vClose();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
