/*============================================================================*/
/* TokenVec.cpp -- vector of tokens                                           */
/*============================================================================*/

#include "BASE.h"

#include "SP_Token.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__pCastFunction(BASE,TokenVec,CN_TOKENVEC)
__aNewFunction(TokenVec,CN_TOKENVEC)
__vDeleteFunction(TokenVec,CN_TOKENVEC)
__aFooterFunction(TokenVec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenVec::TokenVec(void)
: Object(CN_TOKENVEC),
  m_pTokenArray(new TokenArray),
  m_xTokenIter{m_pTokenArray}
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  _checkInitialised_(CN_TOKENVEC);
     
  _postcondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenVec::~TokenVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  delete m_pTokenArray;
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

  TokenArrayIter xIter(pTknVec->m_pTokenArray, i_From);
  Token* pToken = xIter.pCurrItem();
  i8 i_Token = i_From;

  if (pToken) {
    do {
      pSubVec->vAppendToken(pToken);
    } while ((pToken = xIter.pNext()) != nullptr && ++i_Token <= i_To);
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
/* replace tokens (procedure params)                                          */
/*----------------------------------------------------------------------------*/
bool TokenVec::bReplace(TokenVec* pThisVec,
                        TokenVec* pFindVec, TokenVec* pWithVec)
{
  _static_

  i8 nTokens = pThisVec->nTokens();
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pThisToken = pThisVec->pGet(i_Token);
    
    i8 i_Find = pFindVec->iFindToken(pThisToken->szChars());
    
    if (C_UNKNOWN == i_Find)
      continue;

    if (pWithVec->nItems() <= i_Find)
      return false;

    Token* pWithToken = pWithVec->pGet(i_Find);

    // SE-93 temp hack: to cater to certain names being "|"
    // eg create table | (...)
    if (pWithToken->bSame("|"_Stz))
      return false;

    pThisToken->vUpdateChars(pWithToken->szChars(),pThisVec);
    pThisToken->vToken(pWithToken->eToken());
  }

  return true;
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

  if (!m_xTokenIter.bHasNext()) {
    Token oToken;
    oToken.vTokenNum(m_pTokenArray->nItems());
    m_pTokenArray->vAdd(oToken);
  }
  m_xTokenIter.pNext();
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Stz* szArgChars, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nSize = szArgChars->nSize();
  Stz* szExtension = _szByteSpaceExtend(nSize);
  StrLib::vCopy(szExtension->azChar(), nSize, szArgChars->azChar());

  Token oToken{};
  oToken.vTokenNum(m_pTokenArray->nItems());
  oToken.vToken(eToken);
  oToken.vChars(szExtension);
  m_pTokenArray->vAdd(oToken);

  //vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vAppendToken(pToken->szChars(),pToken->eToken());
  
  //vIntegrityCheck();
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
  
  //vIntegrityCheck();
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
/* return current token                                                       */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pCurrToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  return m_xTokenIter.pCurrItem();
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into debug string                                        */
/*----------------------------------------------------------------------------*/
void TokenVec::vDbgStr(String* psDstStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  psDstStr->vReset();

  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    if (0 == i_Token % 8){
      psDstStr->vConcat("\n  "_Stz);
    }

    psDstStr->vCatSprint("[%lld]"_Stz, i_Token);

    Token* pToken = pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_BOOLEAN:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_DECIMAL:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_FLOAT:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_INTEGER:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_NAME:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      case TN_STRING:
        psDstStr->vConcat("\""_Stz);
        psDstStr->vConcat(pToken->szChars());
        psDstStr->vConcat("\""_Stz);
        break;
        
      case TN_TAG:
        psDstStr->vConcat(pToken->szChars());
        break;
        
      default:
        NEVER_REACH;
    }
    
    psDstStr->vConcat(" "_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* dump token vector                                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpVec()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  SP_Token spTkn;
  
  i8 nTkns = nTokens();

  if (nTkns)
    vTrace("TokenVec Dump (tokens %lld) %p:\n"_Stz,nTkns,this);
  else
    vTrace("TokenVec Dump (no tokens) %p:\n"_Stz,this);
  
  vTrace("  tokens %lld\n"_Stz,nTkns);
  vTrace("  curr token %lld\n"_Stz,m_xTokenIter.iArrayIndex());
  vTrace("  area %p\n"_Stz,m_xyByteSpace.szArea());
  vTrace("  size %lld\n"_Stz,m_xyByteSpace.nSize());
  vTrace("  boundary %p\n"_Stz,m_xyByteSpace.szBoundary());
  vTrace("  i_Latter %lld\n"_Stz,m_xyByteSpace.i_Latter());
  vTrace("  latter %p\n"_Stz,_szLaterChars());

  if (! nTkns) {
    vTrace("  **Empty TokenVec**\n"_Stz,""_Stz);
  }

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;
  while ((pToken = xIter.pNext()) != nullptr) {

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
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szArgChars)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    if (szArgChars->bSame(pToken->szChars()))
      return xIter.iArrayIndex();
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szArgChars, eTN eToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    if (eToken == pToken->eToken() &&
        szArgChars->bSame(pToken->szChars()))
      return xIter.iArrayIndex();
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find token given text                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFindToken(Stz* szArgChars, i8& i_Token)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator(i_Token);
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    if (szArgChars->bSame(pToken->szChars())) {
      return pToken;
    }
    ++i_Token;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find first tag                                                             */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFirstTag(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 i_TagBegin = iFindToken("<"_Stz,TN_OTHER);

  if (C_UNKNOWN != i_TagBegin)
    return i_TagBegin;
  
  i_TagBegin = iFindToken("<"_Stz,TN_TAG);
  
  if (C_UNKNOWN != i_TagBegin)
    return i_TagBegin;

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find first tag (by going backwards through the tokens)                     */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFirstTag2(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  //vDumpVec(0);
  
  i8 i_FirstTag = C_UNKNOWN;
  
  i8 i_Scan = iLastToken();
  Token* pScan = pGet(i_Scan);
  if (pScan->bIsSemicolon() || pScan->szChars()->bSame(";"_Stz))
    pScan = pGet(--i_Scan);
  
  while (i_Scan >= 4) {
    
    Token* pTkn_value = pGet(i_Scan--);   //            _^ value
    UNUSED(pTkn_value);

    Token* pTkn_assign = pGet(i_Scan--);  //          _^ = value
    if (! pTkn_assign->bIsAssign())
      break;
    
    Token* pTkn_greater = pGet(i_Scan--); //        _^ > = value
    if (! pTkn_greater->bIsGreaterThan())
      break;

    Token* pTkn_name = pGet(i_Scan--);    //   _^ name > = value
    if (! pTkn_name->bIsName())
      break;
    if (pTkn_name->szChars()->bSame("internal"_Stz))
      vDebugPoint();

    Token* pTkn_lesser = pGet(i_Scan--);  // _^ < name > = value
    if (! pTkn_lesser->bIsLessThan())
      break;

    // have seen the sequence: < name > = value
    i_FirstTag = i_Scan + 1;
  }
   
  return i_FirstTag;
}

/*----------------------------------------------------------------------------*/
/* return first token                                                         */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFirstToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  m_xTokenIter.vBeforeFirst();
  if (m_xTokenIter.bHasNext()) {
    return m_xTokenIter.pNext();
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* return token at a given 0-based index                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pGet(i8 iIdx)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  return m_pTokenArray->pGet(iIdx);
}

/*----------------------------------------------------------------------------*/
/* does this token vector have a ';'                                          */
/*----------------------------------------------------------------------------*/
bool TokenVec::bHasSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator();

  Token* pToken;
  while ((pToken = xIter.pNext()) != nullptr) {
    if (pToken->bIsSemicolon())
      return true;
  }

  return false;
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

  // Copy the token's string into the opened up by space area
  StrLib::vCopy(szInsAt->azChar(), nBytesToIns, szArgChars->azChar());

  // Insert the token at the given index. This will cause subsequent tokens
  // to ripple to the right.
  Token oToken = *pTkn_insAt;
  oToken.vTokenNum(i_At);
  oToken.vToken(eToken);
  m_pTokenArray->vInsert(oToken, i_At);

  // TEMPORARY, until a segmented packed string is implemented
  // Adjust the string pointers of all subsequent tokens by this same offset.
  // Also adjust the token numbers.
  TokenArrayIter xIter = m_pTokenArray->xIterator(i_At);

  Token* pToken;
  i8 iNum = i_At;
  while ((pToken = xIter.pNext()) != nullptr) {
    pToken->vTokenNum(++iNum);
    pToken->vChars(pToken->szChars() + nBytesToIns);
  }

//  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* step thru tokens checking the address of the next tokens chars             */
/*----------------------------------------------------------------------------*/
void TokenVec::vIntegrityCheck(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nTokens();

  if (nTkns < 2)
    return;

  TokenArrayIter xIter1 = m_pTokenArray->xIterator();
  TokenArrayIter xIter2 = m_pTokenArray->xIterator();
  xIter2.pNext();

  Token* pTknA;
  Token* pTknB;
  i8 i_Token = -1;

  while ( (pTknA = xIter1.pNext()) != nullptr &&
          (pTknB = xIter2.pNext()) != nullptr ) {

    ++i_Token;
    i8 i_TknA = pTknA->i_Token();
    i8 i_TknB = pTknB->i_Token();
    UNUSED(i_TknB);
    
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
      vDumpVec();
    }
    
    ASSERT(szCharsB == aAdd<Stz*>(szCharsA,nBytesA));
  }
}

/*----------------------------------------------------------------------------*/
/* return the number of tokens in the array                                   */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nItems(void) {
  _precondition_(_MAGIC_, CN_TOKENVEC, _FOOTER_);

  return m_pTokenArray->nItems();
}

/*----------------------------------------------------------------------------*/
/* return last token number/index                                             */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iLastToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 i_LastTkn = nTokens() - 1;
  
  return i_LastTkn;
}

/*----------------------------------------------------------------------------*/
/* return last token                                                          */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pLastToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  m_xTokenIter.vAfterLast();
  Token* pTkn = m_xTokenIter.pPrevious();
  return pTkn;
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

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;
  while ((pToken = xIter.pNext()) != nullptr) {
    vJot1(':',"  %s"_Stz,spTkn.sz(pToken));
  }
}

/*----------------------------------------------------------------------------*/
/* return next token                                                          */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pNextToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  Token* pToken = m_xTokenIter.pNext();
  return pToken;
}

/*----------------------------------------------------------------------------*/
/* return previous token                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pPrevToken(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  Token* pToken = m_xTokenIter.pPrevious();
  return pToken;
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
  
  if (i_CutTkn < nTkns - 1) {
    _vTokenRemoveInterior(i_CutTkn);
  }
  else {
    _vTokenRemoveLast(i_CutTkn);
  }
    
//  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* rewind current                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vRewindCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  ASSERT(!m_xTokenIter.bHasNext()); // at the last element
  ASSERT(!m_xTokenIter.bIsBeforeFirst());

  i8 nBytes = pCurrToken()->nChars() + 1;
  
  _vByteSpaceTrim(nBytes);

  i8 iIdx = m_xTokenIter.iArrayIndex();
  m_xTokenIter.pPrevious();

  m_pTokenArray->vRemove(iIdx);
}

/*----------------------------------------------------------------------------*/
/* completely reset this token vector                                         */
/*----------------------------------------------------------------------------*/
void TokenVec::vSoftReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_xyByteSpace.vReset();

  m_xTokenIter.vBeforeFirst();
  m_pTokenArray->vReset();
}

/*----------------------------------------------------------------------------*/
/* strip any leading/trailing single quotes from all tokens                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    pToken->vStripSingleQuotes();
  }
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      vRemoveToken(xIter.iArrayIndex());
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* trim / remove all subsequent tokens from given token index                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vTrimTokens(i8 i_Tkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  // TODO: Add trim semantics to segmented array
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
//void TokenVec::vTokeniseSen(Stz* szTokenStr)
//{
//  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
//
//  vSoftReset();
//  vIntegrityCheck();
//
//  Input* pInput = new Input(nullptr);
//  pInput->vUseString(szTokenStr);
//  pInput->vFormSenTokens();
//  pInput->vUseBegin();
//
//  for (i8 i_Token = 0; ; i_Token++) {
//
//    pInput->vReadToken();
//    if (TN_NULL == pInput->eToken())
//      break;
//
//    vAppendToken(pInput->szChars(),pInput->eToken());
//  }
//
//  pInput->vUseEnd();
//  while (pInput)
//    pInput = pInput->pDeleteInput();
//}

/*----------------------------------------------------------------------------*/
/* tokenise given string into this token vector                               */
/*----------------------------------------------------------------------------*/
//void TokenVec::vTokeniseCpp(Stz* szTokenStr)
//{
//  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
//
//  vSoftReset();
//  vIntegrityCheck();
//
//  Input* pInput = new Input(nullptr);
//  pInput->vUseString(szTokenStr);
//  pInput->vFormCppTokens();
//  pInput->vUseBegin();
//
//  for (i8 i_Token = 0; ; i_Token++) {
//
//    pInput->vReadToken();
//    if (TN_NULL == pInput->eToken())
//      break;
//
//    vAppendToken(pInput->szChars(),pInput->eToken());
//  }
//
//  pInput->vUseEnd();
//  while (pInput)
//    pInput = pInput->pDeleteInput();
//}

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
  
//  vIntegrityCheck();
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
void TokenVec::vToTknString(String* psDstStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(psDstStr->azStr(), psDstStr->nAreaBytes(), "");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pGet(i_Token);
    
    if (i_Token)
      StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), ' ');
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_BOOLEAN:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_DECIMAL:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_FLOAT:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_INTEGER:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_TAG:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      default:
        NEVER_REACH;
    }
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into one string                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vVecToString(String* psDstStr, i8 iFromTkn, bool bAddSemicolon)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  bool bNoQuotes = false;

  StrLib::vCopy(psDstStr->azStr(), psDstStr->nAreaBytes(), "");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = iFromTkn; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_NONE:
        break;
        
      case TN_BOOLEAN:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_DECIMAL:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_FLOAT:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_INTEGER:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;

      case TN_LITERAL:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), '\'');
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), '\'');
        break;

      case TN_NAME:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_NULL:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      case TN_STRING:
        if (! bNoQuotes)
          StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), '"');
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        if (! bNoQuotes)
          StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), '"');
        bNoQuotes = false;
        break;
        
      case TN_TAG:
        StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), pToken->szChars()->azChar());
        break;
        
      default:
        NEVER_REACH;
    }
    
    StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), ' ');
  }
  
  // concat a terminating semicolon (needed??)
  if (bAddSemicolon) {
    StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), ';');
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
  
  Stz* szExtension = m_xyByteSpace.szLatter();
  
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
  
//  vIntegrityCheck();

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

  StrLib::vCopy(szInsitui->azChar(), nInsSize, szArgChars->azChar());
  // * * * * * * * * * * * * * * * * * * * *

  
  //vTrace("--- --- --- --- --- ---\n"_Stz);
  //vDumpVec(16);

//  vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* copy chars into a last token                                               */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsCopyLast(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
//  vListTokens();
//  vIntegrityCheck();
  i8 nSize = szArgChars->nSize();
  
  Stz* szExtension = _szByteSpaceExtend(nSize);
  
  pDstToken->vChars(szExtension);

  StrLib::vCopy(szExtension->azChar(), nSize, szArgChars->azChar());

//  vListTokens();
//  vIntegrityCheck();
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
void TokenVec::_vTokenRemoveInterior(i8 iIdx)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  //vIntegrityCheck();

  Token* pTknCut = pGet(iIdx);
//  i8 i_CutTkn = pTknCut->i_Token();
//  SP_Token spTkn;
  //vTrace("Cutting:- %s %p\n"_Stz,spTkn.sz(pTknCut),pTknCut->azChars());

  
  // * * * * * * * * * * * * * * * * * * * * * *

  Stz* szCutAt  = pTknCut->szChars();
  char* azChars = szCutAt->azChar();
  UNUSED(azChars);

  i8 nBytesToCut = pTknCut->nChars() + 1;
  
  _vByteSpaceRemove(szCutAt,nBytesToCut);

  m_pTokenArray->vRemove(iIdx);

  // TEMPORARY, until a segmented packed string is implemented
  // adjust the string pointers of all subsequent tokens by this same offset.
  // Also adjust the token numbers.
  TokenArrayIter xIter = m_pTokenArray->xIterator(iIdx);
  Token* pToken = xIter.pCurrItem();
  i8 iNum = iIdx;

  do {
    pToken->vTokenNum(iNum++);
    pToken->vChars(pToken->szChars() - nBytesToCut);
  } while ((pToken = xIter.pNext()) != nullptr);

  // * * * * * * * * * * * * * * * * * * * * * *
  //vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* cut / erase the last token                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenRemoveLast(i8 iIdx)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  Token* pTknCut = pGet(iIdx);
  
  // reclaim the chars byte space
  i8 nBytes = pTknCut->nChars() + 1;
  _vByteSpaceTrim(nBytes);

  m_pTokenArray->vRemove(iIdx);
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
