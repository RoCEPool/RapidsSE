/*============================================================================*/
/* Prompt_indexEntries.cpp -- set index entries                               */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

PromptNote mPromptNote_indexEntries (
  " Prompt_indexEntries"_Stz, // name of the prompt function
  "comment"_Stz,              // comment about the functopm
  "indexEntries"_Stz,         // the prompt command
  nullptr,                    // the second word
  bPrompt_indexEntries,       // prompt function
  true,                       // enabled

  true,                       // visible
  "fun help"_Stz              // help
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_indexEntries(SePrompt* pPrompt)
{
  TokenVec* pTokenVec = pPrompt->pTknVec();

  ASSERT((*pTokenVec)[0]->bSame("indexEntries"_Stz));
  
  // get number of items in token vector
  i8 iNumTokens = pTokenVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1/2 tokens
  if (iNumTokens != 1 && iNumTokens != 2)
    return false;
  
  if (iNumTokens == 1) {
    vTrace("IndexEntries: %lld\n"_Stz, gpAttachMap->m_nIndexEntries);
    return true;
  }
  
  if (pTokenVec->pGet(1)->eToken() != TN_INTEGER)
    return false;
  
  // get the no of indexEntries to set
  i8 iIndexEntries = pTokenVec->pGet(1)->iToI8();
  gpAttachMap->m_nIndexEntries = iIndexEntries;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
