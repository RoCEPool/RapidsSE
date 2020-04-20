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
    pExtractedVec->vAppendToken(pToken->szChars());
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
    
    pExtractedVec->vAppendToken(pToken->szChars());
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
/*-- Restricted --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy chars                                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vCopyCharsTV_restricted(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  //ASSERT(0);
  
  i8 nTkns = nTokens();
  
  if (pDstToken->i_Token() + 1 == nTkns) {
    
    Stz* szLatter = szLaterChars();
    * szLatter->azChar() = '\0';
    pDstToken->vChars(szLatter);

    szLatter->vCopy(szArgChars);
    
    i8 nLen = szLatter->nLength();
    vTakeCharsBytes(nLen + 1);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  ASSERT(0);

}

/*----------------------------------------------------------------------------*/
/* copy token                                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vCopyTokenTV_restricted(Token* pDstToken, Token* pSrcToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  // chars is not copied here. its copied in the caller
  
  pDstToken->vToken(pSrcToken->eToken());
  pDstToken->vLineNo(pSrcToken->iLineNo());
  pDstToken->vColNo(pSrcToken->iColNo());
}

/*----------------------------------------------------------------------------*/
/* insert a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vInsertToken_restricted(i8 i_Tkn, Stz* szArgChars, eTN eToken)
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
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* advance current token                                                      */
/*----------------------------------------------------------------------------*/
void TokenVec::vAdvanceCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  m_iCurTkn++;
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  Token* pToken = pGet(nTokens(),true);
  
  pToken->vCopyChars_restricted(szArgChars,this);

  vIncItems();
}

/*----------------------------------------------------------------------------*/
/* append given token vector to this token vector                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendVector(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    vAppendToken(pToken->szChars());
  }
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
/* cut a token from token vector                                              */
/*----------------------------------------------------------------------------*/
#ifndef __WITH_CHARS_SET
void TokenVec::vCutToken(i8 i_CutTkn)
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
void TokenVec::vCutToken(i8 i_CutTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();
  
  if (i_CutTkn < nTkns - 1) {
    _vTokenCutInterior(i_CutTkn);
  }
  else {
    _vTokenCutLast(i_CutTkn);
  }
  
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
//  m_xyCharsSet.vListAll();
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
//  m_xyCharsSet.vMoveLeft(szWhereToCut,nBytesToCut);
//  m_xyCharsSet.vListAll();
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
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vIncItems();
}

/*----------------------------------------------------------------------------*/
/* get latter of xyCharsSet                                                   */
/*----------------------------------------------------------------------------*/
Stz* TokenVec::szLaterChars(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Stz* szLater = m_xyCharsSet.szLatter();
  
  return szLater;
}

/*----------------------------------------------------------------------------*/
/* list tokens                                                                */
/*----------------------------------------------------------------------------*/
void TokenVec::vListTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  SP_Token spTkn;
  
  i8 nTkns = nTokens();
  for (i8 i_Token = 0; i_Token < nTkns; i_Token++) {
    
    Token* pToken = pGet(i_Token);

    //if (! pToken->szChars())
    //  break;
    
    vTrace("  %s\n"_Stz,spTkn.sz(pToken));
  }
  vTrace("\n"_Stz);
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
  
   // get count of words in pWords
  i8 i_Token;
  for (i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    if ( ! pGet(i_Token,true)->bHasChars() )
      break;
  }
  
  vItems(i_Token);
}

/*----------------------------------------------------------------------------*/
/* completely reset this token vector                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::vSoftReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyCharsSet.vReset();
  
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
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      pToken->vReset();
      //break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* take bytes (for token chars) from xyCharsSet                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vTakeCharsBytes(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyCharsSet.vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* tokenise given string into this token vector                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vTokenise(Stz* szTokenStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vSoftReset();
  
  Input* pInput = new Input;
  pInput->vUseString(szTokenStr);
  pInput->vFormSenTokens();
  pInput->vUseBegin();

  for (i8 i_Token = 0; ; i_Token++) {
    
    pInput->vReadToken();
    if (TN_EOF == pInput->eToken())
      break;
    
    vIncTokens();
    
    Token* pToken = pGet(i_Token);
    pToken->vToken(pInput->eToken());
    pToken->vCopyChars_restricted(pInput->szChars(),this);
    
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
        
      case TN_EOF:
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
/* insert byte space for chars, adjust all following tokens upwards to match  */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceInsert(i8 i_Tkn, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pTkn = pGet(i_Tkn);
    
  Stz* szInsAddr = pTkn->szChars();

  _vCharsMoveRight(szInsAddr,nBytes);
  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  i8 nTkns = nTokens();
//  for (i8 i_Follow = i_Tkn + 1; i_Follow < nTkns ; i_Follow++) {
//    Token* pTknFollowing = pGet(i_Follow);
//
//    pTknFollowing->vAdjustChars(+ nBytes); // observe plus sign
//  }
}

/*----------------------------------------------------------------------------*/
/* remove byte space for chars, adjust all following tokens downwards to match*/
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceRemove(i8 i_Tkn, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pTkn = pGet(i_Tkn);
  
  Stz* szRemAddr = pTkn->szChars();
  
  _vCharsMoveLeft(szRemAddr,nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
//  i8 nTkns = nTokens();
//  for (i8 i_Follow = i_Tkn + 1; i_Follow < nTkns ; i_Follow++) {
//    Token* pTknFollowing = pGet(i_Follow);
//
//    pTknFollowing->vAdjustChars(- nBytes); // observe minus sign
//  }
}

/*----------------------------------------------------------------------------*/
/* trim byte space at end of chars set                                        */
/*----------------------------------------------------------------------------*/
void TokenVec::_vByteSpaceTrim(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyCharsSet.vGive(nBytes);
}

/*----------------------------------------------------------------------------*/
/* move subset of chars leftwards                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsMoveLeft(Stz* szAtAddr, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  m_xyCharsSet.vMoveLeft(szAtAddr,nBytes);
}

/*----------------------------------------------------------------------------*/
/* move subset of chars rightwards                                            */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsMoveRight(Stz* szAtAddr, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyCharsSet.vMoveRight(szAtAddr,nBytes);
}

/*----------------------------------------------------------------------------*/
/* cut / erase a non-last token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenCutInterior(i8 i_CutTkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vTrace("_vTokenCutInterior::\n"_Stz);
  vListTokens();
  
  i8 nTkns = nTokens();
  
  Token* pTknCut = pGet(i_CutTkn);
  
  SP_Token spTkn;
  vTrace("Cutting:- %s\n"_Stz,spTkn.sz(pTknCut));

  char* azChars = pTknCut->azChars();
  Stz* szWhereToCut = pTknCut->szChars();
  i8 nBytesToCut = pTknCut->nChars() + 1;
  
  _vByteSpaceRemove(i_CutTkn,nBytesToCut);
  
  _vVecRippleLeft(i_CutTkn);
  
  vTrace("--- --- --- --- --- ---\n"_Stz);
  vListTokens();
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* cut / erase the last token                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenCutLast(i8 i_CutTkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  Token* pTknCut = pGet(i_CutTkn);
    
  // reclaim the chars byte space
  i8 nBytes = pTknCut->nChars() + 1;
  _vByteSpaceTrim(nBytes);
    
  // decrement te number of tokens
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* completely reset this token vector                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecHardReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyCharsSet.vReset();
  
  m_iCurTkn = -1;

  vItems(0);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    pToken->vReset();
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
