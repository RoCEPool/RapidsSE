/*============================================================================*/
/* JobHelper.cpp -- a set of job helper functions                             */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
bool JobHelper::bParseCmd(EnameSet* pArgEnmSet,
                          TokenVec* pTknVec)
{
  Dic* pRootDic = AM::pRootDic();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Word* pCategory = pRootDic->pFindWord("opSyntax:"_Stz);
  
  Word* pSyntax = nullptr;
  
  while ( (pSyntax = pRootDic->pEachWord(pCategory,pSyntax)) ) {
    
    //vTrace("  opSyntax: %s\n"_Stz,pSyntax->szWordNm());
    
    if (JobHelper::bMatchesSyntax(pTknVec,pSyntax->pEnameSet())) {
      
      //vTrace("    matches\n"_Stz);
      
      pArgEnmSet->vAdd(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
      s32 sJobName(pSyntax->pEnameSet()->szValue("__Name__"_Stz));
      //StrLib::vRemoveSuffix(sJobName.azChar(),'_');
      pArgEnmSet->vAdd(C_TAG_JOBNM,sJobName);
      break;
    }
  }
  
  if (! pSyntax) {
    //vTrace("Unable to match job syntax\n"_Stz);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool b = bExtractArgs(pArgEnmSet,pSyntax->pEnameSet(),pTknVec);
  if (! b) {
    //vTrace("Unable to match job syntax\n"_Stz);
    return false;
  }
  
  // pArgEnmSet->vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse token vec of command and extract arguments                           */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractArgs(EnameSet* pArgEnmSet,
                             EnameSet* pSynEnmSet,
                             TokenVec* pTknVec
                             )
{
  i8 nTokens = pTknVec->nTokens();
  i8 nEnames = pSynEnmSet->nEnames();
  
//  if (nEnames != nTokens)
//    return false;
  
  i8 i_Syn = 0;
  i8 i_Tkn = 0;

  while (i_Tkn < nTokens && i_Syn < nEnames) {
    
    if (pSynEnmSet->szName(i_Syn)->bSame("__job__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynEnmSet->szName(i_Syn)->bSame("__name__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynEnmSet->szName(i_Syn)->bSame("__meta__"_Stz)) {
      if (! bExtractMeta(pArgEnmSet,pSynEnmSet,i_Syn,pTknVec,i_Tkn))
        return false;
      continue;
    }
   
    if (pSynEnmSet->szName(i_Syn)->bSame("__json__"_Stz)) {
      if (! bExtractJson(pArgEnmSet,pSynEnmSet,i_Syn,pTknVec,i_Tkn))
        return false;
      //pArgEnmSet->vPrint();
      continue;
    }
    
    if (pSynEnmSet->szName(i_Syn)->bSame("__etc__"_Stz)) {

      break;
    }
    
    i_Syn++;
    i_Tkn++;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractJson(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             )
{
  Token* pTkn = nullptr;

  ASSERT(pSynEnmSet->szName(i_Syn_)->bSame("__json__"_Stz));
  
  pTkn = pTknVec->pGet(i_Tkn_);
  ASSERT(pTkn->bSame("{"_Stz));

  pTkn = pTknVec->pGet(++i_Tkn_);
  while (! pTkn->bSame("}"_Stz)) {
    vTrace("  %s\n"_Stz,pTkn->szTerm());
    Token* pTkn_name = pTkn;
    
    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szTerm());

    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szTerm());
    Token* pTkn_value = pTkn;
    
    pArgEnmSet->vAdd(pTkn_name->szTerm(),
                     pTkn_value->szTerm());
    
    //pArgEnmSet->vPrint();


    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szTerm());
    
    if (pTkn->bSame(","_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn_);
      vTrace("  %s\n"_Stz,pTkn->szTerm());
    }
  }
  
  i_Syn_++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractMeta(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
                             i8&       i_Syn_,
                             TokenVec* pTknVec,
                             i8&       i_Tkn_
                             )
{
  Stz* szValue = pSynEnmSet->szValue(i_Syn_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn_);
  Stz* szTerm = pTkn->szTerm();
  
  pArgEnmSet->vAdd(szValue,szTerm);
  
  // pArgEnmSet->vPrint();
  
  i_Tkn_++;
  i_Syn_++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if the token vector matches the given job syntax                      */
/*----------------------------------------------------------------------------*/
bool JobHelper::bMatchesSyntax(TokenVec* pTknVec, EnameSet* pSynEnmSet)
{
  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Syn = 0;
  i8 i_Tkn = 0;
  
  while (i_Tkn < nTokens) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    if (pSynEnmSet->szName(i_Syn)->bEqual("__job__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynEnmSet->szName(i_Syn)->bEqual("__name__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynEnmSet->szName(i_Syn)->bEqual("__meta__"_Stz)) {
      //pSynEnmSet->vPrint();
      return true;
    }
    
    if (! pTkn->bSame((pSynEnmSet->szValue(i_Syn)))) {
      //vTrace("%s\n"_Stz, pTkn->szText());
      //pSynEnmSet->vPrint();
      return false;
    }
    
    i_Syn++;
    i_Tkn++;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
