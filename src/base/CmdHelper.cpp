/*============================================================================*/
/* CmdHelper.cpp -- a set of cmd helper functions                             */
/*============================================================================*/

#include "BASE.h"

#include "SP_TagSet.hpp"

#include "SP_TknVecStr.hpp"

/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get the cmd tag set of the job                                             */
/*----------------------------------------------------------------------------*/
TagSet* CmdHelper::pDefTagSet(Stz* szCmdNm)
{
  _static_

  // form the cmdTag name
  s32 sCmdTagNm(szCmdNm);
  StrLib::vConcat(sCmdTagNm.azChar(),(const char*)"_cmdTags"_Stz);
  
  // find the CmdTagNm word
  Word* pCmdWord = AM::DIC::pRoot()->pFindWord(sCmdTagNm);
  
  // get the CmdTag words tagSet
  TagSet* pCmdTagSet = pCmdWord->pWrdTagSet();
  
  return pCmdTagSet;
}

/*----------------------------------------------------------------------------*/
/* extract paths from token vector into tag set                               */
/*----------------------------------------------------------------------------*/
void CmdHelper::vExtractCmdPaths(TagSet* pDstCmdTagSet, TokenVec* pSrcCmdTknVec)
{
  _static_
    
  i8 i_Paths = pSrcCmdTknVec->iFindToken("paths"_Stz,TN_NAME);

  if (C_UNKNOWN == i_Paths)
    return;
  
  pSrcCmdTknVec->vRemoveToken(i_Paths);

  s32 sName;
  
  i8 nPaths;
  for (nPaths = 0; i_Paths < pSrcCmdTknVec->nTokens(); nPaths++) {
    sName.vSprint("<path%lld>"_Stz,nPaths);
    Token* pTkn = pSrcCmdTknVec->pGet(i_Paths);
    pDstCmdTagSet->vAppend(sName,pTkn->szChars());
    pSrcCmdTknVec->vRemoveToken(i_Paths);
  }
  
  pDstCmdTagSet->vAppend("<paths>"_Stz,nPaths);

}

/*----------------------------------------------------------------------------*/
/* extract tags                                                               */
/*----------------------------------------------------------------------------*/
bool CmdHelper::bExtractCmdTags(TagSet* pDstCmdTagSet, TokenVec* pSrcCmdTknVec)
{
  _static_
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract leading job control tags (such as /time or /nowait)
  
  Token* pTkn0 = pSrcCmdTknVec->pGet(0,true);
  
  s32 sTag;
  
  while ('/' == * pTkn0->azChars() ) {
    
    Token* pTkn1 = pSrcCmdTknVec->pGet(1,true);
    sTag.vSprint("<%s>"_Stz,pTkn1->szChars());
    
    pDstCmdTagSet->vInsert(sTag,1);
    
    pSrcCmdTknVec->vRemoveToken(0);
    pSrcCmdTknVec->vRemoveToken(0);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract trailing job supplemtntal tags ( such as <field_delimiter> = | )

  i8 i_TagBegin = pSrcCmdTknVec->iFirstTag2();
  if (C_UNKNOWN == i_TagBegin)
    return true;

  Token* pTkn_begin = pSrcCmdTknVec->pGet(i_TagBegin);
  UNUSED(pTkn_begin);
  
  i8 nTkns = pSrcCmdTknVec->nTokens();

  i8 i_Scan = i_TagBegin;
  while (i_Scan < nTkns) {
  
    Token* pScan = pSrcCmdTknVec->pGet(i_Scan++);        // <
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsLessThan())
      return false;
    
    Token* pTkn_name = pSrcCmdTknVec->pGet(i_Scan++);    // name
    if (i_Scan >= nTkns)
      return false;
    if (! pTkn_name->bIsName())
      return false;

    pScan = pSrcCmdTknVec->pGet(i_Scan++);               // >
    if (! pScan->bIsGreaterThan())
      return false;
    
    pScan = pSrcCmdTknVec->pGet(i_Scan++);               // =
    if (i_Scan >= nTkns)
      return false;
    if (! pScan->bIsAssign())
      return false;

    Token* pTkn_value = pSrcCmdTknVec->pGet(i_Scan++);   // value
    //if (! pTkn_value->bIsString())
    //  return false;

    s32 sName;
    sName.vSprint("<%s>"_Stz,pTkn_name->szChars());
    pDstCmdTagSet->vAppend(sName,pTkn_value->szChars());
  }
    
  pSrcCmdTknVec->vTrimTokens(i_TagBegin);
  
  return true;

}


/*----------------------------------------------------------------------------*/
/* parse command                                                              */
/*----------------------------------------------------------------------------*/
bool CmdHelper::bParseCmd(TokenVec* pCmdTknVec, TagSet* pCmdTagSet)
{
  _static_

  Dic* pDicRoot = AM::DIC::pRoot();

  Word* pCategory = nullptr;
  Word* pSyntax   = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // can we find a job which matches the syntax?
  
  pCategory = pDicRoot->pFindWord("cmdSyntax:"_Stz);
  pSyntax   = nullptr;
  
  while ( (pSyntax = pDicRoot->pEachWord(pSyntax,pCategory)) ) {
    
    //vTrace("  jobSyntax: %s\n"_Stz,pSyntax->szWordNm());
    //pSyntax->pTagSet()->vDumpSet();
    
    if (CmdHelper::bMatchesSyntax(pCmdTknVec,pSyntax->pWrdTagSet())) {
      
      //vTrace("    matches\n"_Stz);
      
      s32 sCmdName(pSyntax->pWrdTagSet()->szValue("__cmdNm__"_Stz));
      
      // get the default set of tags for the comman
      TagSet* pDefTagSet = CmdHelper::pDefTagSet(sCmdName);
      
      // copy the cmd tag set into the pTagSet
      pCmdTagSet->vCopy(pDefTagSet);
      
      goto L_Matched;
    }
  }
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no match
  
  if (! pSyntax) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      vTrace("Unable to match command syntax:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pCmdTknVec));
    }
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // extract arguments
  
  L_Matched:
  
  bool b = bExtractAllArgs(pCmdTagSet,pSyntax->pWrdTagSet(),pCmdTknVec);
  if (! b) {
    if (AM::bDeveloper()) {
      SP_TknVecStr spTvs;
      SP_TagSet spTags;
      vTrace("Unable to extract arguments:-\n"_Stz);
      vTrace("  Tokens: %s\n"_Stz,spTvs.sz(pCmdTknVec));
      vTrace("  Syntax: %s\n"_Stz,spTags.sz(pSyntax->pWrdTagSet()));
    }
    return false;
  }
  
  // pArgTags->vPrint();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* populate tagset from given token vector                                    */
/*----------------------------------------------------------------------------*/
void CmdHelper::vPopulateTagSet(TagSet* pTagSet, TokenVec* pTknVec)
{
  _static_
  
  i8 nTkns = pTknVec->nTokens();

  i8 i_Scan = 0;
  while (i_Scan < nTkns) {
    
    Token* pTkn_name = pTknVec->pGet(i_Scan++);    // <tagNm>
    if (i_Scan >= nTkns)
      break;
    if (TN_TAG != pTkn_name->eToken())
      break;

    Token* pTkn_equals = pTknVec->pGet(i_Scan++);  // =
    UNUSED(pTkn_equals);
    
    if (i_Scan >= nTkns)
      break;
    //if (! pTkn_equals->bIsAssign())
    //  break;

    Token* pTkn_value = pTknVec->pGet(i_Scan++);   // value
    
    pTagSet->vReplace(pTkn_name->szChars(),pTkn_value->szChars());
  }

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
  _static_

  i8 nTags = pSynTags->nItems();
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
  _static_

  Token* pTkn = nullptr;

  ASSERT(pSynTags->szName(i_Syn_)->bSame("__json__"_Stz));
  UNUSED(pSynTags);
  
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
  _static_

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
  _static_

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
      UNUSED(szTagNm);

      //vTrace("unknown tag in syntax tag set\n"_Stz);
      return false;
    }

  }

  if (i_Syn != pSynTags->m_nItems)
    return false;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
