/*============================================================================*/
/* JobHelper.cpp -- a set of job helper functions                             */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractArgs(EnmSet*   pArgEnmSet,
                             TokenVec* pTknVec,
                             EnmSet*   pSynEnmSet)
{
  i8 nTokens = pTknVec->nTokens();
  i8 nEnames = pSynEnmSet->nEnm();
  
  if (nEnames != nTokens)
    return false;
  
  for (i8 i_Tkn = 0; i_Tkn < nTokens; i_Tkn++) {
    
    if (pSynEnmSet->szName(i_Tkn)->bSame("__meta__"_Stz)) {
      pArgEnmSet->vAdd(pSynEnmSet->szValue(i_Tkn),
                       pTknVec->pGet(i_Tkn)->szText(),
                       TN_STRING);
      pArgEnmSet->vPrint();
    }
    else {
      
    }
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
