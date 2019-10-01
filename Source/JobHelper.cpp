/*============================================================================*/
/* JobHelper.cpp -- a set of job helper functions                             */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract arguments                            */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractArgs(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             TokenVec* pTknVec
                             )
{
  i8 nTokens = pTknVec->nTokens();
  i8 nEnames = pSynEnmSet->nEnm();
  
//  if (nEnames != nTokens)
//    return false;
  
  i8 i_Syn = 0;
  i8 i_Tkn = 0;

 while (i_Tkn < nTokens && i_Syn < nEnames) {
    
    if (pSynEnmSet->szName(i_Syn)->bSame("__meta__"_Stz)) {
      if (! bExtractMeta(pArgEnmSet,pSynEnmSet,i_Syn,pTknVec,i_Tkn))
        return false;
      continue;
    }
   
    if (pSynEnmSet->szName(i_Tkn)->bSame("__json__"_Stz)) {
      if (! bExtractJson(pArgEnmSet,pSynEnmSet,i_Syn,pTknVec,i_Tkn))
        return false;
      pArgEnmSet->vPrint();
      continue;
    }
   
    i_Syn++;
    i_Tkn++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractJson(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             )
{
  i8 nTokens = pTknVec->nTokens();
  i8 nEnames = pSynEnmSet->nEnm();

  ASSERT(pSynEnmSet->szValue(i_Syn_)->bSame("__json__"_Stz));
  
  Token* pTkn = pTknVec->pGet(i_Tkn_);
  
  ASSERT(pTkn->bSame("{"_Stz));
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractMeta(EnmSet*   pArgEnmSet,
                             EnmSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             )
{
  pArgEnmSet->vAdd(pSynEnmSet->szValue(i_Syn_),
                   pTknVec->pGet(i_Tkn_)->szText(),
                   TN_STRING);
  
  pArgEnmSet->vPrint();
  
  i_Tkn_++;
  i_Syn_++;
  
  return true;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
