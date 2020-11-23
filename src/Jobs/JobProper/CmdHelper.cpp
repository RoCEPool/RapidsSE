/*============================================================================*/
/* CmdHelper.cpp -- a set of cmd helper functions                             */
/*============================================================================*/

#include "Jobs.h"

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get the cmd tag set of the job                                             */
/*----------------------------------------------------------------------------*/
TagSet* CmdHelper::pCmdTagSet(Stz* szJobNm)
{
  // form the cmdTag name
  s32 sCmdTagNm(szJobNm);
  StrLib::vConcat(sCmdTagNm.azChar(),(const char*)"_cmdTags"_Stz);
  
  // find the CmdTagNm word
  Word* pCmdWord = AM::pRootDic()->pFindWord(sCmdTagNm);
  
  // get the CmdTag words tagSet
  TagSet* pCmdTagSet = pCmdWord->pWrdTagSet();
  
  return pCmdTagSet;
}

/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
bool CmdHelper::bParseCmd(TokenVec* pTknVec, TagSet* pTagSet)
{
  Dic* pRootDic = AM::pRootDic();

  Word* pCategory = nullptr;
  Word* pSyntax   = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can we find a job which matches the syntax?
  
  pCategory = pRootDic->pFindWord("cmdSyntax:"_Stz);
  pSyntax   = nullptr;
  
  while ( (pSyntax = pRootDic->pEachWord(pSyntax,pCategory)) ) {
    
    //vTrace("  jobSyntax: %s\n"_Stz,pSyntax->szWordNm());
    //pSyntax->pTagSet()->vDumpSet();
    
    if (CmdHelper::bMatchesSyntax(pTknVec,pSyntax->pWrdTagSet())) {
      
      //vTrace("    matches\n"_Stz);
      
      s32 sJobName(pSyntax->pWrdTagSet()->szValue("__cmdNm__"_Stz));
      
      // get the CmdTag words tagSet
      TagSet* pCmdTagSet = CmdHelper::pCmdTagSet(sJobName);
      
      // copy the cmd tag set into the pTagSet
      pTagSet->vCopy(pCmdTagSet);
      
      goto L_Matched;
    }
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no match
  
  if (! pSyntax) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      vTrace("Unable to match command syntax:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pTknVec));
    }
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract arguments
  
  L_Matched:
  
  bool b = bExtractAllArgs(pTagSet,pSyntax->pWrdTagSet(),pTknVec);
  if (! b) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      SP_TagSet spTags;
      vTrace("Unable to extract arguments:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pTknVec));
      vTrace("  Syntax: %s\n"_Stz,spTags.sz(pSyntax->pWrdTagSet()));
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
bool CmdHelper::bExtractAllArgs(TagSet* pTagSet,
                                TagSet* pSynTags,
                                TokenVec* pTknVec)
{
  i8 nTags = pSynTags->nEnames();
  i8 nTokens = pTknVec->nTokens();
    
  i8 i_Syn = 0;
  i8 i_Tkn = 0;

  while (i_Tkn < nTokens && i_Syn < nTags) {
    
    // name of command
    if (pSynTags->szName(i_Syn)->bSame("__cmdNm__"_Stz)) {
      i_Syn++;
    }
      
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    else
    // command verb+noun
    if (pSynTags->szName(i_Syn)->bSame("__cmd__"_Stz)) {
      i_Syn++;
      i_Tkn++;
    }
    
    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    else
    // argument
    if (pSynTags->szName(i_Syn)->bSame("__arg__"_Stz)) {
      if (! bExtractArg(pTagSet,pSynTags,i_Syn,pTknVec,i_Tkn))
        return false;
    }
    else
    // json argument
    if (pSynTags->szName(i_Syn)->bSame("__json__"_Stz)) {
      if (! bExtractJson(pTagSet,pSynTags,i_Syn,pTknVec,i_Tkn))
        return false;
      //pArgTags->vPrint();
    }
    else
    // remainder
    if (pSynTags->szName(i_Syn)->bSame("__etc__"_Stz)) {
      break;
    }
    // unknown tag in syntax tag set
    else {
      vTrace("unknown tag in syntax tag set\n"_Stz);
      RASSERT(0);
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* parse tokenvec of command and extract argument                             */
/*----------------------------------------------------------------------------*/
bool CmdHelper::bExtractJson(TagSet*   pTagSet,
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
    
    pTagSet->vInsert(pTkn_name->szChars(),
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
bool CmdHelper::bExtractArg(TagSet*   pArgTags,
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
bool CmdHelper::bMatchesSyntax(TokenVec* pTknVec, TagSet* pSynTags)
{
  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Syn = 0;
  i8 i_Tkn = 0;
  
  while (i_Tkn < nTokens) {
    Token* pTkn = pTknVec->pGet(i_Tkn);
    
    if (pSynTags->szName(i_Syn)->bEqual("__cmdNm__"_Stz)) {
      i_Syn++;
    }
    
    else
    if (pSynTags->szName(i_Syn)->bEqual("__cmd__"_Stz)) {
      if (! pTkn->bSame((pSynTags->szValue(i_Syn)))) {
        return false;
      }
      i_Syn++;
      i_Tkn++;
    }
    
    else
    if (pSynTags->szName(i_Syn)->bEqual("__arg__"_Stz)) {
      i_Syn++;
      i_Tkn++;
    }
    
    else
    if (pSynTags->szName(i_Syn)->bEqual("__etc__"_Stz)) {
      return true;
    }
        
    // unknown tag in syntax tag set
    else {
      Stz* szTagNm = pSynTags->szName(i_Syn);
      
      //vTrace("unknown tag in syntax tag set\n"_Stz);
      return false;
    }

  }

  if (i_Syn != pSynTags->m_nEnames)
    return false;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
