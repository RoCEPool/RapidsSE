/*============================================================================*/
/* JobHelper.cpp -- a set of job helper functions                             */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
bool JobHelper::bParseCmd(TagSet* pArgTags, TokenVec* pTknVec)
{
  Dic* pRootDic = AM::pRootDic();

  Word* pCategory = nullptr;
  Word* pSyntax   = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can we find a job which matches the syntax?
  
  pCategory = pRootDic->pFindWord("jobSyntax:"_Stz);
  pSyntax   = nullptr;
  
  while ( (pSyntax = pRootDic->pEachWord(pCategory,pSyntax)) ) {
    
    //vTrace("  jobSyntax: %s\n"_Stz,pSyntax->szWordNm());
    //pSyntax->pTagSet()->vDumpSet();
    
    if (JobHelper::bMatchesSyntax(pTknVec,pSyntax->pWordTags())) {
      
      //vTrace("    matches\n"_Stz);
      
      pArgTags->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
      s32 sJobName(pSyntax->pWordTags()->szValue("__Name__"_Stz));
      //StrLib::vRemoveSuffix(sJobName.azChar(),'_');
      pArgTags->vInsert(C_TAG_JOBNM,sJobName);
      goto L_Matched;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can we find an operator which matches the syntax?

  pCategory = pRootDic->pFindWord("opSyntax:"_Stz);
  pSyntax   = nullptr;
  
  while ( (pSyntax = pRootDic->pEachWord(pCategory,pSyntax)) ) {
    
    //vTrace("  jobSyntax: %s\n"_Stz,pSyntax->szWordNm());
    //pSyntax->pTagSet()->vDumpSet();
    
    if (JobHelper::bMatchesSyntax(pTknVec,pSyntax->pWordTags())) {
      
      //vTrace("    matches\n"_Stz);
      
      pArgTags->vInsert(C_TAG_JOBCLASS,CN_JOB_UNIVERSAL);
      s32 sJobName(pSyntax->pWordTags()->szValue("__Name__"_Stz));
      //StrLib::vRemoveSuffix(sJobName.azChar(),'_');
      pArgTags->vInsert(C_TAG_JOBNM,sJobName);
      goto L_Matched;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no match
  
  if (! pSyntax) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      vTrace("Unable to match job syntax:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pTknVec));
    }
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract arguments
  
  L_Matched:
  
  bool b = bExtractAllArgs(pArgTags,pSyntax->pWordTags(),pTknVec);
  if (! b) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      SP_TagSet spTags;
      vTrace("Unable to extract arguments:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pTknVec));
      vTrace("  Syntax: %s\n"_Stz,spTags.sz(pSyntax->pWordTags()));
    }
    return false;
  }
  
  // pArgTags->vPrint();
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse token vec of command and extract arguments                           */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractAllArgs(TagSet* pArgTags,
                                TagSet* pSynTags,
                                TokenVec* pTknVec)
{
  i8 nTags = pSynTags->nEnames();
  i8 nTokens = pTknVec->nTokens();
    
  i8 i_Syn = 0;
  i8 i_Tkn = 0;

  while (i_Tkn < nTokens && i_Syn < nTags) {
    
    // job class
    if (pSynTags->szName(i_Syn)->bSame("__job__"_Stz)) {
      i_Syn++;
    }
    else
    // name of command
    if (pSynTags->szName(i_Syn)->bSame("__name__"_Stz)) {
      i_Syn++;
    }
    else
    // argument
    if (pSynTags->szName(i_Syn)->bSame("__meta__"_Stz)) {
      if (! bExtractArg(pArgTags,pSynTags,i_Syn,pTknVec,i_Tkn))
        return false;
    }
    else
    // json argument
    if (pSynTags->szName(i_Syn)->bSame("__json__"_Stz)) {
      if (! bExtractJson(pArgTags,pSynTags,i_Syn,pTknVec,i_Tkn))
        return false;
      //pArgTags->vPrint();
      continue;
    }
    else
    // remainder
    if (pSynTags->szName(i_Syn)->bSame("__etc__"_Stz)) {
      break;
    }
    // command noun/verb
    else {
      i_Syn++;
      i_Tkn++;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractJson(TagSet*   pArgTags,
                             TagSet*   pSynTags, i8& i_Syn_,
                             TokenVec* pTknVec,  i8& i_Tkn_)
{
  Token* pTkn = nullptr;

  ASSERT(pSynTags->szName(i_Syn_)->bSame("__json__"_Stz));
  
  pTkn = pTknVec->pGet(i_Tkn_);
  ASSERT(pTkn->bSame("{"_Stz));

  pTkn = pTknVec->pGet(++i_Tkn_);
  while (! pTkn->bSame("}"_Stz)) {
    vTrace("  %s\n"_Stz,pTkn->szChars());
    Token* pTkn_name = pTkn;
    
    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szChars());

    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szChars());
    Token* pTkn_value = pTkn;
    
    pArgTags->vInsert(pTkn_name->szChars(),
                     pTkn_value->szChars());
    
    //pArgTags->vPrint();


    pTkn = pTknVec->pGet(++i_Tkn_);
    vTrace("  %s\n"_Stz,pTkn->szChars());
    
    if (pTkn->bSame(","_Stz)) {
      pTkn = pTknVec->pGet(++i_Tkn_);
      vTrace("  %s\n"_Stz,pTkn->szChars());
    }
  }
  
  i_Syn_++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool JobHelper::bExtractArg(TagSet*   pArgTags,
                            TagSet*   pSynTags, i8& i_Syn_,
                            TokenVec* pTknVec,  i8& i_Tkn_)
{
  Stz* szValue = pSynTags->szValue(i_Syn_);
  
  Token* pTkn = pTknVec->pGet(i_Tkn_);
  Stz* szTerm = pTkn->szChars();
  
  pArgTags->vInsert(szValue,szTerm);
  
  // pArgTags->vPrint();
  
  i_Tkn_++;
  i_Syn_++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if the token vector matches the given job syntax                      */
/*----------------------------------------------------------------------------*/
bool JobHelper::bMatchesSyntax(TokenVec* pTknVec, TagSet* pSynTags)
{
  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Syn = 0;
  i8 i_Tkn = 0;
  
  while (i_Tkn < nTokens) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    if (pSynTags->szName(i_Syn)->bEqual("__job__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynTags->szName(i_Syn)->bEqual("__name__"_Stz)) {
      i_Syn++;
      continue;
    }
    
    if (pSynTags->szName(i_Syn)->bEqual("__meta__"_Stz)) {
      //pSynTags->vPrint();
      return true;
    }
    
    if (! pTkn->bSame((pSynTags->szValue(i_Syn)))) {
//      vTrace("%s\n"_Stz, pTkn->szChars());
//      pSynTags->vDumpSetDetailed();
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
