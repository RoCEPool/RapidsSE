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

    pThisToken->vCopyChars_restricted(pWithToken->szChars(),pThisVec);
  }
}

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::pSubVec(TokenVec* pTknVec, i8 i_From, i8 i_To)
{
  _static_
  
  TokenVec* pExtractedVec = new TokenVec;

  i8 nTokens = pTknVec->nTokens();
  
  i8 i_Token;
  for (i_Token = i_From; i_Token < nTokens && i_Token <= i_To; i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    pExtractedVec->vAppendToken(pToken);
  }
  
  return pExtractedVec;
}

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::pSubVec(TokenVec* pTknVec, Stz* szStart, Stz* szEnd)
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
  
  TokenVec* pExtractedVec = new TokenVec;

  for (i_Token++; i_Token < nTokens; i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    if (pToken->bSame(szEnd))
      break;
    
    pExtractedVec->vAppendToken(pToken);
  }
  
  return pExtractedVec;
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
#ifndef __WITH_CHARS_SET
void TokenVec::vAppendToken(Stz* szArgChars, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  Token* pToken = pGet(nTokens(),true);
  
  pToken->vCopyChars_restricted(szArgChars,this);
  pToken->vToken(eToken);

  vIncItems();
  
}
#else
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
#endif

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vAppendToken(pToken->szChars(),pToken->eToken());
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
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
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
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

  _vBytesSpaceTake(nChars + 1);

  Stz* szLatter = _szLaterChars();
  * szLatter->azChar() = '\0';

  //SP_Token spTkn;
  //vTrace("vCollectEnd: %s\n"_Stz,spTkn.sz(pToken));
  
  //if (pToken->szChars()->bSame("catalog"_Stz))
  if (pToken->szChars()->bSame(""_Stz))
    Correctness::vDebugPoint();
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
}

/*----------------------------------------------------------------------------*/
/* count tokens in token array                                                */
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
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
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
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpVec(i8 nPlusMore)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  SP_Token spTkn;
  
  i8 nTkns = nTokens();
  i8 i_Limit = nTkns + nPlusMore;
  
  vTrace("TokenVec %p:\n"_Stz,this);
  vTrace("  items %lld\n"_Stz,nItems());
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
    
    vTrace("  Chars %p : %s ; Size %lldB (next %p)\n"_Stz,
           pToken->azChars(),
           spTkn.sz(pToken),
           pToken->nChars() + 1,
           aAdd<char*>(pToken->azChars(),pToken->nChars() + 1)
           );
  }
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szArgChars)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (szArgChars->bSame(pToken->szChars()))
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
  
  for (i_Token++; i_Token < nTokens(); i_Token++) {
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
void TokenVec::vInsertToken(i8 i_Tkn, Stz* szArgChars, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  // open up a token at _i_Tkn
  for (i8 i_Token = nTokens() - 1; i_Token >= i_Tkn ; i_Token--) {
    
    Token* pTkn_dst = pGet(i_Token + 1,true);
    Token* pTkn_src = pGet(i_Token);
    
    pTkn_dst->vCopyToken_restricted(pTkn_src,this);
  }
  
  vIncItems();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // now copy the inserting token into place
  
  Token* pTkn_ins = pGet(i_Tkn);
  
  pTkn_ins->vCopyChars_restricted(szArgChars,this);
  pTkn_ins->vToken(eToken);
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
}

/*----------------------------------------------------------------------------*/
/* step thru tokens checking the address of the next tokens chars             */
/*----------------------------------------------------------------------------*/
void TokenVec::vIntegrityCheck(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
#ifdef __WITH_CHARS_SET

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
#endif
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
/* remove the ';' token                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vRemoveSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  i8 i_Semi = iFindToken(";"_Stz);
  
  if (C_UNKNOWN == i_Semi)
    return;
  
  vRemoveToken(i_Semi);
}

/*----------------------------------------------------------------------------*/
/* remove a token from token vector                                           */
/*----------------------------------------------------------------------------*/
#ifndef __WITH_CHARS_SET
void TokenVec::vRemoveToken(i8 i_CutTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  if (i_CutTkn >= nTkns - 1) {
    vDecItems();
    return;
  }
  
  Token* pTknCut = pGet(i_CutTkn);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  for (i8 i_Token = i_CutTkn; i_Token < nTkns ; i_Token++) {
    
    Token* pTknDst = pGet(i_Token);
    Token* pTknSrc = pGet(i_Token + 1,true);
    
    pTknDst->vCopyToken_restricted(pTknSrc,this);
  }
  
  vDecItems();
  
}
#else
/*----------------------------------------------------------------------------*/
void TokenVec::vRemoveToken(i8 i_CutTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  Token* pTknCut = pGet(i_CutTkn);

  if (i_CutTkn < nTkns - 1) {
    _vTokenCutInterior(pTknCut);
  }
  else {
    _vTokenCutLast(pTknCut);
  }
  
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
}
/*----------------------------------------------------------------------------*/
//void TokenVec::vCutToken(i8 i_CutTkn)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
//
//  i8 nTkns = nTokens();
//
//  if (i_CutTkn >= nTkns - 1) {
//    vDecItems();
//    return;
//  }
//
//  Token* pTknCut = pGet(i_CutTkn);
//
//// ----
//  char* azChars = pTknCut->azChars();
//  Stz* szWhereToCut = pTknCut->szChars();
//  i8 nBytesToCut = pTknCut->nChars() + 1;
//
//  m_xyByteSpace.vListAll();
//
//  Correctness::vDebugPoint();
//// ----
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  for (i8 i_Token = i_CutTkn; i_Token < nTkns ; i_Token++) {
//
//    Token* pTknDst = pGet(i_Token);
//    Token* pTknSrc = pGet(i_Token + 1,true);
//
//    pTknDst->vCopyToken_restricted(pTknSrc,this);
//  }
//
//  vDecItems();
//
//// ----
//
//  m_xyByteSpace.vMoveLeft(szWhereToCut,nBytesToCut);
//  m_xyByteSpace.vListAll();
//
//  nTkns = nTokens();
//
//  for (i8 i_Token = i_CutTkn; i_Token < nTkns ; i_Token++) {
//    Token* pTknAdj = pGet(i_Token);
//
//    pTknAdj->vAdjustChars(- nBytesToCut); // observe minus sign
//  }
//
//
//  Correctness::vDebugPoint();
//// ----
//
//}
#endif

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
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
}

/*----------------------------------------------------------------------------*/
/* rewind current                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vRewindCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
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
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
#ifndef __WITH_CHARS_SET
   // get count of words in pWords
  i8 i_Token;
  for (i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    if ( ! pGet(i_Token,true)->bHasChars() )
      break;
  }
  
  vItems(i_Token);
#endif
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
#ifndef __WITH_CHARS_SET
void TokenVec::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      pToken->vReset();
      //break;
    }
  }
}
#else
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
#endif
/*----------------------------------------------------------------------------*/
/* tokenise given string into this token vector                               */
/*----------------------------------------------------------------------------*/
#ifndef __WITH_CHARS_SET
void TokenVec::vTokenise(Stz* szTokenStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vSoftReset();
  #ifdef __WITH_CHARS_SET
    vIntegrityCheck();
  #endif

  Input* pInput = new Input;
  pInput->vUseString(szTokenStr);
  pInput->vFormSenTokens();
  pInput->vUseBegin();

  for (i8 i_Token = 0; ; i_Token++) {
    
    pInput->vReadToken();
    if (TN_NULL == pInput->eToken())
      break;
    
    vIncTokens();
    
    Token* pToken = pGet(i_Token);
    pToken->vToken(pInput->eToken());
    
    #ifdef __WITH_CHARS_SET
      vIntegrityCheck();
    #endif

    pToken->vCopyChars_restricted(pInput->szChars(),this);
    
  }
  
  pInput->vUseEnd();
  delete pInput;
}
#else
/*----------------------------------------------------------------------------*/
void TokenVec::vTokenise(Stz* szTokenStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vSoftReset();
  #ifdef __WITH_CHARS_SET
    vIntegrityCheck();
  #endif

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
#endif

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
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
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

  _vBytesSpaceTake(nBytes);

  return szExtension;
}

/*----------------------------------------------------------------------------*/
/* insert byte space for chars, adjust all following tokens upwards to match  */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceInsert(Stz* szAtChars, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vMoveRight(szAtChars,nBytes);
}

/*----------------------------------------------------------------------------*/
/* remove byte space for chars, adjust all following tokens downwards to match*/
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceRemove(Stz* szAtChars, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  m_xyByteSpace.vMoveLeft(szAtChars,nBytes);
}

/*----------------------------------------------------------------------------*/
/* take bytes (for token chars) from xyCharsSet                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vBytesSpaceTake(i8 nBytes)
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
  #ifdef __WITH_CHARS_SET
    vIntegrityCheck();
  #endif

  vTrace("_vCharsCopyInterior::\n"_Stz);
  vListTokens();

  SP_Token spTkn;
  vTrace("Copying:- %s <= %s\n"_Stz,spTkn.sz(pDstToken),szArgChars);

  
  // * * * * * * * * * * * * * * * * * * * *
  i8 i_Tkn = pDstToken->i_Token();
  Stz* szInsitui = pDstToken->szChars();
  
  i8 nRemSize = pDstToken->nChars() + 1;
  _vByteSpaceRemove(szInsitui,nRemSize);
  _vVecAdjustChars(i_Tkn + 1,(- nRemSize)); // observe the minus sign
  
  vTrace("--- --- --- --- --- ---\n"_Stz);
  vListTokens();

  i8 nInsSize = szArgChars->nSize();
  _vByteSpaceInsert(szInsitui,nInsSize);
  _vVecAdjustChars(i_Tkn + 1,nInsSize);

  
  szInsitui->vCopy(szArgChars);
  // * * * * * * * * * * * * * * * * * * * *

  
  vTrace("--- --- --- --- --- ---\n"_Stz);
  vListTokens();
  
#ifdef __WITH_CHARS_SET
  vIntegrityCheck();
#endif
}

/*----------------------------------------------------------------------------*/
/* copy chars into a last token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsCopyLast(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  #ifdef __WITH_CHARS_SET
    vListTokens();
    vIntegrityCheck();
  #endif
  i8 nSize = szArgChars->nSize();
  
  Stz* szExtension = _szByteSpaceExtend(nSize);
  
  pDstToken->vChars(szExtension);

  szExtension->vCopy(szArgChars);
#ifdef __WITH_CHARS_SET
  vListTokens();
  vIntegrityCheck();
#endif
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
void TokenVec::_vTokenCutInterior(Token* pTknCut)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vTrace("_vTokenCutInterior::\n"_Stz);
  vListTokens();
  
  vIntegrityCheck();

  i8 nTkns = nTokens();
  i8 i_CutTkn = pTknCut->i_Token();
  SP_Token spTkn;
  vTrace("Cutting:- %s\n"_Stz,spTkn.sz(pTknCut));

  
  // * * * * * * * * * * * * * * * * * * * * * *

  Stz* szCutAt  = pTknCut->szChars();
  char* azChars = szCutAt->azChar();
  
  i8 nBytesToCut = pTknCut->nChars() + 1;
  
  _vByteSpaceRemove(szCutAt,nBytesToCut);
  _vVecRippleLeft(i_CutTkn);
    
  // * * * * * * * * * * * * * * * * * * * * * *

  
  vTrace("--- --- --- --- --- ---\n"_Stz);
  vListTokens();

  vIntegrityCheck();

  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* cut / erase the last token                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenCutLast(Token* pTknCut)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  // reclaim the chars byte space
  i8 nBytes = pTknCut->nChars() + 1;
  _vByteSpaceTrim(nBytes);
    
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
void TokenVec::_vVecRippleLeft(i8 i_Tkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 i_Limit = nTokens();

  for (i8 i_Ripple = i_Tkn + 1; i_Ripple < i_Limit ; i_Ripple++) {
    
    Token* pTknSrc = pGet(i_Ripple);
    Token* pTknDst = pGet(i_Ripple - 1);
 
    pTknDst->vRippleCopy_restricted(pTknSrc,this);
  }

  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* inc no tokens, ripple tokens rightward from i_Tkn, leaving i_Tkn for reuse */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecRippleRight(i8 i_Tkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 i_Start = nTokens() - 1;

  vIncItems();
  
  for (i8 i_Ripple = i_Start; i_Ripple >= i_Tkn; i_Ripple--) {
    
    Token* pTknSrc = pGet(i_Ripple);
    Token* pTknDst = pGet(i_Ripple + 1);
    
    pTknDst->vRippleCopy_restricted(pTknSrc,this);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
