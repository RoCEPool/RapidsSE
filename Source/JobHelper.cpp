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
  
  Word* pCategory = pRootDic->pFindWord("jobSyntax:"_Stz);
  
  Word* pSyntax = nullptr;
  
  while ( (pSyntax = pRootDic->pEachWord(pCategory,pSyntax)) ) {
    vTrace("  jobSyntax: %s\n"_Stz,pSyntax->sName().aStz());
    
    if (JobHelper::bMatchesSyntax(pTknVec,pSyntax->pEnameSet())) {
      vTrace("    matches\n"_Stz);
      
      pArgEnmSet->vAdd("<jobClass>"_Stz,CN_JOB_UNIVERSAL,TN_INTEGER);
      s32 sJobName(pSyntax->pEnameSet()->szValue("__Name__"_Stz));
      //StrLib::vRemoveSuffix(sJobName.azChar(),'_');
      pArgEnmSet->vAdd("<jobNm>"_Stz,sJobName,TN_STRING);
      break;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  bool b = bExtractArgs(pArgEnmSet,pSyntax->pEnameSet(),pTknVec);
  if (! b) {
    vTrace("Unable to match job syntax\n"_Stz);
    return EC_INVALID_COMMAND;
  }
  
  pArgEnmSet->vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract arguments                            */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractArgs(EnameSet*   pArgEnmSet,
                             EnameSet*   pSynEnmSet,
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
    vTrace("  %s\n"_Stz,pTkn->szText());
    Token* pTkn_name = pTkn;
    
    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szText());

    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szText());
    Token* pTkn_value = pTkn;
    
    pArgEnmSet->vAdd(pTkn_name->szText(),
                     pTkn_value->szText(),
                     TN_STRING);
    
    pArgEnmSet->vPrint();


    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szText());
    
    if (pTkn->bSame(","_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn_);
      vTrace("  %s\n"_Stz,pTkn->szText());
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
  pArgEnmSet->vAdd(pSynEnmSet->szValue(i_Syn_),
                   pTknVec->pGet(i_Tkn_)->szText(),
                   TN_STRING);
  
  pArgEnmSet->vPrint();
  
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
    
    if (pSynEnmSet->szName(i_Syn)->bEqual("__meta__"_Stz))
      return true;
    
    if (! pTkn->szText()->bEqual((pSynEnmSet->szValue(i_Syn))))
      return false;
    
    i_Syn++;
    i_Tkn++;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
