/*============================================================================*/
/* Prompt_indexEntries.cpp -- set index entries                               */
/*============================================================================*/

#include "StorageEngine.h"



PromptFun __Prompt_indexEntries (
  PF_INDEXENTRIES,            // prompt function
  "Prompt_indexEntries"_Stz,  // name of the prompt function
  "comment"_Stz,              // comment about the function
  "indexEntries"_Stz,         // the prompt command
  nullptr,                    // the second word
  bPrompt_indexEntries,       // prompt function
  true,                       // enabled

  true,                       // visible
  "indexEntries"_Stz,         // help
  ""_Stz                      // description
);

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_indexEntries(SePrompt* pPrompt)
{
  TokenVec* pInpTknVec = pPrompt->pInpTknVec();

  ASSERT(pInpTknVec->pGet(0)->bSame("indexEntries"_Stz));
  
  // get number of items in token vector
  i8 nTokens = pInpTknVec->nItems();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 1/2 tokens
  if (nTokens != 1 && nTokens != 2)
    return false;
  
  if (nTokens == 1) {
    vTrace("IndexEntries: %lld\n"_Stz, AM::nIndexEntries());
    vTrace("\nsuccess: 1 rows affected\n"_Stz);
    return true;
  }
  
  if (pInpTknVec->pGet(1)->eToken() != TN_INTEGER)
    return false;
  
  // get the no of indexEntries to set
  i8 iIndexEntries = pInpTknVec->pGet(1)->iToI8();
  
  AM::vIndexEntries(iIndexEntries);
  
  vTrace("\nsuccess: 0 rows affected\n"_Stz);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
