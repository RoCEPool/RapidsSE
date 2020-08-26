/*============================================================================*/
/* TokenVec.cpp -- vector of tokens                                           */
/*============================================================================*/

#include "LexicalAnalysis.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(LexicalAnalysis,TokenVec,CN_TOKENVEC)
__aNewFunction(TokenVec,CN_TOKENVEC)
__vDeleteFunction(TokenVec,CN_TOKENVEC)
__aFooterFunction(TokenVec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenVec::TokenVec(bool bNewTokens)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
      
  m_iCurTkn = -1;
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    pToken->vInitToken_restricted(i_Token,this);
  }
  
  _checkInitialised_(CN_TOKENVEC);
  
  _postcondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenVec::~TokenVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vSetAllTokens();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::pGetSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To)
{
  _static_
  
  TokenVec* pSubVec = new TokenVec;

  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Token;
  for (i_Token = i_From; i_Token < nTokens && i_Token <= i_To; i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    pSubVec->vAppendToken(pToken);
  }
  
  return pSubVec;
}

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::pGetSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd)
{
  _static_

  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Token;
  for (i_Token = 0; i_Token < nTokens; i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    if (pToken->bSame(szStart)) {
      break;
    }
  }

  if (i_Token >= nTokens)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pSubVec = new TokenVec;

  for (i_Token++; i_Token < nTokens; i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    if (pToken->bSame(szEnd))
      break;
    
    pSubVec->vAppendToken(pToken);
  }
  
  return pSubVec;
}

/*----------------------------------------------------------------------------*/
/* replace tokens                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vReplace(TokenVec* pThisVec,
                        TokenVec* pFindVec, TokenVec* pWithVec)
{
  _static_
  
  i8 nTokens = pThisVec->nTokens();
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pThisToken = pThisVec->pGet(i_Token);
    
    i8 i_Find = pFindVec->iFindToken(pThisToken->szChars());
    
    if (C_UNKNOWN == i_Find)
      continue;
    
    Token* pWithToken = pWithVec->pGet(i_Find);

    pThisToken->vUpdateChars(pWithToken->szChars(),pThisVec);
    pThisToken->vToken(pWithToken->eToken());
  }
}

/*----------------------------------------------------------------------------*/
/* tokenise given string                                                      */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::pTokenise(Stz* szString)
{
  _static_
  
  TokenVec* pTknVec = new TokenVec;
  
  pTknVec->vTokenise(szString);
  
  return pTknVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* advance current token                                                      */
/*----------------------------------------------------------------------------*/
void TokenVec::vAdvanceCurr(bool bIncItems)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  m_iCurTkn++;
  
  if (bIncItems)
    vIncItems();
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Stz* szArgChars, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vIncItems();
  Token* pToken = pGet(nTokens()-1);
  
  i8 nSize = szArgChars->nSize();
  Stz* szExtension = _szByteSpaceExtend(nSize);
  
  pToken->vChars(szExtension);
  szExtension->vCopy(szArgChars);

  pToken->vToken(eToken);

  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vAppendToken(pToken->szChars(),pToken->eToken());
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* append given token vector to this token vector                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendVector(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    vAppendToken(pToken);
  }
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool TokenVec::bBlankLine(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  if (0 == nTokens() ||
      0 == pGet(0)->nChars())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* begin collecting a new token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vCollectBegin(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Stz* szLatter = _szLaterChars();
  
  * szLatter->azChar() = '\0';
  
  pToken->vChars(szLatter);
}

/*----------------------------------------------------------------------------*/
/* end collecting a token                                                     */
/*----------------------------------------------------------------------------*/
void TokenVec::vCollectEnd(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nChars = pToken->nChars();

  _vByteSpaceTake(nChars + 1);

  Stz* szLatter = _szLaterChars();
  * szLatter->azChar() = '\0';

  //SP_Token spTkn;
  //vTrace("vCollectEnd: %s\n"_Stz,spTkn.sz(pToken));
  
  //if (pToken->szChars()->bSame("catalog"_Stz))
  //if (pToken->szChars()->bSame(""_Stz))
  //  Correctness::vDebugPoint();
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* count tokens in token array                                         REMOVE */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iCountTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if ( ! pToken->bHasChars() )
      return i_Token;
  }
  
  ASSERT("Should never be reached"_Stz);
  return -1;
}

/*----------------------------------------------------------------------------*/
/* copy given token vector to this token vector                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vCopyVector(TokenVec* pTknVecSrc)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTknVecSrc->nTokens(); i_Token++) {
    
    Token* pToken_src = pTknVecSrc->pGet(i_Token);
    Token* pToken_dst = pGet(i_Token);
    
    pToken_dst->vCopyToken_restricted(pToken_src,this);
  }
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* return current token                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iCurrToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  return m_iCurTkn;
}

/*----------------------------------------------------------------------------*/
/* set current token                                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vCurrToken(i8 iCurTkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_iCurTkn = iCurTkn;
}

/*----------------------------------------------------------------------------*/
/* return current token                                                       */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pCurrToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  if (C_UNKNOWN == m_iCurTkn)
    return nullptr;
  
  Token* pTkn = pGet(m_iCurTkn,true);
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into debug string                                        */
/*----------------------------------------------------------------------------*/
void TokenVec::vDbgStr(Stz* szDstStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(szDstStr->azChar(),"");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    if (0 == i_Token % 8)
      StrLib::vConcat(szDstStr,"\n  "_Stz);

    StrLib::vSprintCat(szDstStr,"[%lld]"_Stz,i_Token);

    Token* pToken = pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_INTEGER:
        StrLib::vConcat(szDstStr,pToken->szChars());
        break;
        
      case TN_NAME:
        StrLib::vConcat(szDstStr,pToken->szChars());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(szDstStr,pToken->szChars());
        break;
        
      case TN_STRING:
        StrLib::vConcat(szDstStr,"\""_Stz);
        StrLib::vConcat(szDstStr,pToken->szChars());
        StrLib::vConcat(szDstStr,"\""_Stz);
        break;
        
      case TN_TAG:
        StrLib::vConcat(szDstStr,pToken->szChars());
        break;
        
      default:
        NEVER_REACH;
    }
    
    StrLib::vConcat(szDstStr," "_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpVec(i8 nPlusMore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  SP_Token spTkn;
  
  i8 nTkns = nTokens();
  i8 i_Limit = nTkns + nPlusMore;
  
  if (nTkns)
    vTrace("TokenVec Dump (tokens %lld) %p:\n"_Stz,nTkns,this);
  else
    vTrace("TokenVec Dump (no tokens) %p:\n"_Stz,this);
  
  vTrace("  tokens %lld\n"_Stz,nTkns);
  vTrace("  curr token %lld\n"_Stz,m_iCurTkn);
  vTrace("  area %p\n"_Stz,m_xyByteSpace.szArea());
  vTrace("  size %lld\n"_Stz,m_xyByteSpace.nSize());
  vTrace("  boundary %p\n"_Stz,m_xyByteSpace.szBoundary());
  vTrace("  i_Latter %lld\n"_Stz,m_xyByteSpace.i_Latter());
  vTrace("  latter %p\n"_Stz,_szLaterChars());

  if (! nTkns) {
    vTrace("  **Empty TokenVec**\n"_Stz,""_Stz);
  }
  
  for (i8 i_Token = 0; i_Token < i_Limit; i_Token++) {
    
    Token* pToken = pGet(i_Token,true);

    //if (! pToken->szChars())
    //  break;
    
    if (pToken->azChars()) {
      vTrace("  Chars %p : %s ; Size %lldB (next %p)\n"_Stz,
             pToken->azChars(),
             spTkn.sz(pToken),
             pToken->nChars() + 1,
             aAdd<char*>(pToken->azChars(),pToken->nChars() + 1)
             );
    }
    else {
      vTrace("  Chars %p\n"_Stz,
             pToken->azChars()
             );

    }
  }
}

/*----------------------------------------------------------------------------*/
/* duplcate entirely the given token vector                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vDuplicateVector(TokenVec* pSrcVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pSrcVec->nTokens(); i_Token++) {
    
    Token* pToken_src = pSrcVec->pGet(i_Token);
    
    vAppendToken(pToken_src);
  }
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szArgChars)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    Token* pToken = pGet(i_Token);
    if (szArgChars->bSame(pToken->szChars()))
      return i_Token;
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szArgChars, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    Token* pToken = pGet(i_Token);
    if (eToken == pToken->eToken() &&
        szArgChars->bSame(pToken->szChars()))
      return i_Token;
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find token given text                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFindToken(Stz* szArgChars, i8& i_Token)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();

  for (i_Token++; i_Token < nTkns; i_Token++) {
    Token* pToken = pGet(i_Token);
    if (pToken->szChars()->bSame(szArgChars))
      return pToken;
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return first token                                                         */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFirstToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  if (nTokens()) {
    m_iCurTkn = 0;
    Token* pTkn = pGet(m_iCurTkn);
    return pTkn;
  }
  
  m_iCurTkn = C_UNKNOWN;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* does this token vector have a ';'                                          */
/*----------------------------------------------------------------------------*/
bool TokenVec::bHasSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    Token* pToken = pGet(i_Token);
    
    if (pToken->bIsSemicolon())
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vIncItems();
}

/*----------------------------------------------------------------------------*/
/* insert a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vInsertToken(i8 i_At, Stz* szArgChars, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  // calc the action
  
  //vDumpVec(16);

  Token* pTkn_insAt = pGet(i_At);
  Stz* szInsAt = pTkn_insAt->szChars();
  i8 nBytesToIns = szArgChars->nSize();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open up byte space for new chars
  
  _vByteSpaceInsert(szInsAt,nBytesToIns);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // open a token
  
  _vVecRippleRight(i_At,nBytesToIns);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy the inserting token into place
  
  szInsAt->vCopy(szArgChars);
  pTkn_insAt->vToken(eToken);
  
  //vDumpVec(16);

  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* step thru tokens checking the address of the next tokens chars             */
/*----------------------------------------------------------------------------*/
void TokenVec::vIntegrityCheck(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();

  for (i8 i_Token = 0; i_Token < nTkns - 1; i_Token++) {
    Token* pTknA = pGet(i_Token);
    Token* pTknB = pGet(i_Token + 1);
    
    i8 i_TknA = pTknA->i_Token();
    i8 i_TknB = pTknB->i_Token();
    
    ASSERT(i_Token == i_TknA);
    ASSERT(i_TknB  == i_TknA + 1);

    Stz* szCharsA = pTknA->szChars();
    Stz* szCharsB = pTknB->szChars();

    i8 nBytesA = szCharsA->nSize();
    
    if (szCharsB != aAdd<Stz*>(szCharsA,nBytesA)) {
      vTrace("Error at [%lld] - [%lld] %p\n"_Stz,
             i_TknA,
             i_TknA + 1,
             aAdd<char*>(szCharsA,nBytesA)
             );
      vDumpVec(16);
    }
    
    ASSERT(szCharsB == aAdd<Stz*>(szCharsA,nBytesA));
  }
}

/*----------------------------------------------------------------------------*/
/* list tokens                                                                */
/*----------------------------------------------------------------------------*/
void TokenVec::vListTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  SP_Token spTkn;
  
  i8 nTkns = nTokens();
  
  if (! nTkns) {
    vJot1(':',"  **Empty TokenVec**"_Stz,""_Stz);
    return;
  }
  
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    
    Token* pToken = pGet(i_Token);

    //if (! pToken->szChars())
    //  break;
    
    vJot1(':',"  %s"_Stz,spTkn.sz(pToken));
  }
}

/*----------------------------------------------------------------------------*/
/* return next token                                                          */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pNextToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_iCurTkn++;
  
  if (m_iCurTkn >= nTokens()) {
    m_iCurTkn = C_UNKNOWN;
    return nullptr;
  }
  
  Token* pTkn = pGet(m_iCurTkn);
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* move to next token                                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::vNextToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_iCurTkn++;
}

/*----------------------------------------------------------------------------*/
/* return previous token                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pPrevToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  if (m_iCurTkn > 0) {
    
    m_iCurTkn--;
    Token* pTkn = pGet(m_iCurTkn);
    return pTkn;
  }
  
  m_iCurTkn = C_UNKNOWN;
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* move to prev token                                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::vPrevToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_iCurTkn--;
}

/*----------------------------------------------------------------------------*/
/* remove the ';' token                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vRemoveSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  i8 i_Semi = iFindToken(";"_Stz,TN_OTHER);
  
  if (C_UNKNOWN == i_Semi)
    return;
  
  vRemoveToken(i_Semi);
}

/*----------------------------------------------------------------------------*/
/* remove a token from token vector                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::vRemoveToken(i8 i_CutTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  Token* pTknCut = pGet(i_CutTkn);

  if (i_CutTkn < nTkns - 1) {
    _vTokenRemoveInterior(pTknCut);
  }
  else {
    _vTokenRemoveLast(pTknCut);
  }
    
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* copy token                                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vReplaceToken(Token* pDstToken, Token* pSrcToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  // chars is not copied here. its copied in the caller
  
  pDstToken->vToken(pSrcToken->eToken());
  pDstToken->vLineNo(pSrcToken->iLineNo());
  pDstToken->vColNo(pSrcToken->iColNo());
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* rewind current                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vRewindCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nBytes = pCurrToken()->nChars() + 1;
  
  _vByteSpaceTrim(nBytes);

  m_iCurTkn--;
  
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetAllTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  vItems(C_MAX_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* completely reset this token vector                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::vSoftReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vReset();
  
  m_iCurTkn = -1;

  vItems(0);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    pToken->vReset();
  }
}

/*----------------------------------------------------------------------------*/
/* strip off '&'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripAmpersand(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (TN_OTHER == pToken->eToken() && pToken->bSame("&"_Stz)) {
      pToken->vReset();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* strip any leading/trailing single quotes from all tokens                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < nTokens(); i_Token++) {
    Token* pToken = pGet(i_Token);
    pToken->vStripSingleQuotes();
  }
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = this->nTokens();
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      vRemoveToken(i_Token);
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* trim / remove all tokens from given token                                  */
/*----------------------------------------------------------------------------*/
void TokenVec::vTrimTokens(i8 i_Tkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pToken = pGet(i_Tkn);
  Stz* szTrimmedLatter = pToken->szChars();
  
  i8 nHadTkns = nTokens();
  
  
  m_xyByteSpace.vTrim(szTrimmedLatter);
  
  for (i8 i_Reset = i_Tkn; i_Reset < nHadTkns; i_Reset++) {
    Token* pToken_reset = pGet(i_Reset);
    pToken_reset->vReset();
  }
  
  vItems(i_Tkn);
}

/*----------------------------------------------------------------------------*/
/* tokenise given string into this token vector                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vTokenise(Stz* szTokenStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vSoftReset();
  vIntegrityCheck();

  Input* pInput = new Input;
  pInput->vUseString(szTokenStr);
  pInput->vFormSenTokens();
  pInput->vUseBegin();

  for (i8 i_Token = 0; ; i_Token++) {
    
    pInput->vReadToken();
    if (TN_NULL == pInput->eToken())
      break;
      
    vAppendToken(pInput->szChars(),pInput->eToken());
  }
  
  pInput->vUseEnd();
  delete pInput;
}


/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTokens = nItems();
  
  return nTokens;
}

/*----------------------------------------------------------------------------*/
/* copy chars                                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vUpdateToken(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  if (_bIsLastToken(pDstToken))
    _vCharsCopyLast(pDstToken,szArgChars);
  else
    _vCharsCopyInterior(pDstToken,szArgChars);
  
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* copy chars                                                                 */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nUsedByteSpace(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nUsed = m_xyByteSpace.i_Latter();
  
  return nUsed;
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into one string                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vToTknString(Stz* szDstStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(szDstStr->azChar(),"");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pGet(i_Token);
    
    if (i_Token)
      StrLib::vConcat(szDstStr->azChar()," ");
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_INTEGER:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_TAG:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      default:
        NEVER_REACH;
    }
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into one string                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vVecToString(Stz* szDstStr, i8 iFromTkn, bool bAddSemicolon)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(szDstStr->azChar(),"");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = iFromTkn; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_INTEGER:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(szDstStr->azChar(),"\"");
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        StrLib::vConcat(szDstStr->azChar(),"\"");
        break;
        
      case TN_TAG:
        StrLib::vConcat(szDstStr->azChar(),pToken->szChars()->azChar());
        break;
        
      default:
        NEVER_REACH;
    }
    
    StrLib::vConcat(szDstStr->azChar()," ");
  }
  
  // concat a terminating semicolon (needed??)
  if (bAddSemicolon) {
    StrLib::vConcat(szDstStr->azChar(),";");
  }
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy chars into a last token                                               */
/*----------------------------------------------------------------------------*/
Stz* TokenVec::_szByteSpaceExtend(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Stz* szExtension = m_xyByteSpace.szLatter();;
  
  * szExtension->azChar() = '\0';

  _vByteSpaceTake(nBytes);

  return szExtension;
}

/*----------------------------------------------------------------------------*/
/* insert byte space for chars, adjust all following tokens upwards to match  */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceInsert(Stz* szAtChars, i8 nBytesIns)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vMoveRight(szAtChars,nBytesIns);
}

/*----------------------------------------------------------------------------*/
/* remove byte space for chars, adjust all following tokens downwards to match*/
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceRemove(Stz* szAtChars, i8 nBytesRem)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  m_xyByteSpace.vMoveLeft(szAtChars,nBytesRem);
}

/*----------------------------------------------------------------------------*/
/* take bytes (for token chars) from xyCharsSet                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceTake(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* trim byte space at end of chars set                                        */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceTrim(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* copy chars into a non-last token                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsCopyInterior(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vIntegrityCheck();

  //vTrace("_vCharsCopyInterior\n"_Stz);
  //vDumpVec(16);

  SP_Token spTkn;
  //vTrace("Copying:- %s <= %s\n"_Stz,spTkn.sz(pDstToken),szArgChars);

  
  // * * * * * * * * * * * * * * * * * * * *
  i8 i_Tkn = pDstToken->i_Token();
  Stz* szInsitui = pDstToken->szChars();
  
  i8 nRemSize = pDstToken->nChars() + 1;
  _vByteSpaceRemove(szInsitui,nRemSize);
  _vVecRippleLeft(i_Tkn,(- nRemSize)); // observe minus sign

  //vTrace("--- --- --- --- --- ---\n"_Stz);

  i8 nInsSize = szArgChars->nSize();
  _vByteSpaceInsert(szInsitui,nInsSize);
  _vVecRippleRight(i_Tkn,nInsSize);

  szInsitui->vCopy(szArgChars);
  // * * * * * * * * * * * * * * * * * * * *

  
  //vTrace("--- --- --- --- --- ---\n"_Stz);
  //vDumpVec(16);

  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* copy chars into a last token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsCopyLast(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vListTokens();
  vIntegrityCheck();
  i8 nSize = szArgChars->nSize();
  
  Stz* szExtension = _szByteSpaceExtend(nSize);
  
  pDstToken->vChars(szExtension);

  szExtension->vCopy(szArgChars);
  vListTokens();
  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* completely reset this token vector                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::_vHardReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vReset();
  
  m_iCurTkn = -1;

  vItems(0);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    pToken->vReset();
  }
}

/*----------------------------------------------------------------------------*/
/* test if given token is the last token                                      */
/*----------------------------------------------------------------------------*/
bool TokenVec::_bIsLastToken(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();

  if (pToken->i_Token() + 1 == nTkns)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get latter of xyCharsSet                                                   */
/*----------------------------------------------------------------------------*/
Stz* TokenVec::_szLaterChars(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Stz* szLater = m_xyByteSpace.szLatter();
  
  return szLater;
}

/*----------------------------------------------------------------------------*/
/* cut / erase a non-last token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenRemoveInterior(Token* pTknCut)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  //vTrace("_vTokenRemoveInterior::\n"_Stz);
  //vDumpVec(16);
  
  //vIntegrityCheck();

  i8 nTkns = nTokens();
  i8 i_CutTkn = pTknCut->i_Token();
  SP_Token spTkn;
  //vTrace("Cutting:- %s %p\n"_Stz,spTkn.sz(pTknCut),pTknCut->azChars());

  
  // * * * * * * * * * * * * * * * * * * * * * *

  Stz* szCutAt  = pTknCut->szChars();
  char* azChars = szCutAt->azChar();
  
  i8 nBytesToCut = pTknCut->nChars() + 1;
  
  _vByteSpaceRemove(szCutAt,nBytesToCut);

  _vVecRippleLeft(i_CutTkn,(- nBytesToCut)); // observe minus sign
    
  // * * * * * * * * * * * * * * * * * * * * * *

  
  //vTrace("--- --- --- --- --- ---\n"_Stz);
  //vDumpVec(16);

  vIntegrityCheck();

  //Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* cut / erase the last token                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenRemoveLast(Token* pTknCut)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  // reclaim the chars byte space
  i8 nBytes = pTknCut->nChars() + 1;
  _vByteSpaceTrim(nBytes);
    
  pTknCut->vReset();
  
  // decrement te number of tokens
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* step thru tokens adjusting location of each chars by given +/- bytes       */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecAdjustChars(i8 i_Tkn, i8 nByBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();

  for (i8 i_Token = i_Tkn; i_Token < nTkns; i_Token++) {
    Token* pTknAdj = pGet(i_Token);

    pTknAdj->vAdjustChars(nByBytes);
  }
}

/*----------------------------------------------------------------------------*/
/* ripple the tokens leftward, overwriting the i_Tkn, dec no of tokens        */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecRippleLeft(i8 i_Tkn, i8 nAdjCharsBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pTknSrc = nullptr;
  Token* pTknDst = nullptr;
  
  i8 i_Limit = nTokens();

  for (i8 i_Ripple = i_Tkn + 1; i_Ripple < i_Limit ; i_Ripple++) {
    
    pTknSrc = pGet(i_Ripple);
    pTknDst = pGet(i_Ripple - 1);
 
    pTknDst->vRippleCopy_restricted(pTknSrc,nAdjCharsBytes,this);
  }

  if (pTknSrc)
    pTknSrc->vReset();
  
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* inc no tokens, ripple tokens rightward from i_Tkn, leaving i_Tkn for reuse */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecRippleRight(i8 i_Tkn, i8 nAdjCharsByte)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pTknSrc = nullptr;
  Token* pTknDst = nullptr;
  
  i8 i_Start = nTokens() - 1;

  vIncItems();
  
  for (i8 i_Ripple = i_Start; i_Ripple >= i_Tkn; i_Ripple--) {
    
    pTknSrc = pGet(i_Ripple);
    pTknDst = pGet(i_Ripple + 1);
    
    pTknDst->vRippleCopy_restricted(pTknSrc,nAdjCharsByte,this);
  }
  
  pTknSrc = pGet(i_Start);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
