/*============================================================================*/
/* TokenVec.cpp -- vector of tokens                                           */
/*============================================================================*/

#include "BASE.h"

#include "SP_Token.hpp"

#ifdef WITH_TOKENVEC_INTEGRITY
#define INTEGRITY_CHECK() (this->vIntegrityCheck())
#define INTEGRITY_CHECK_STATIC(obj) (obj->vIntegrityCheck())
#endif

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

  INTEGRITY_CHECK_STATIC(pSubVec);
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

  INTEGRITY_CHECK_STATIC(pSubVec);
  return pSubVec;
}

/*----------------------------------------------------------------------------*/
/* replace tokens (procedure params)                                          */
/*----------------------------------------------------------------------------*/
bool TokenVec::bReplace(TokenVec* pThisVec,
                        TokenVec* pFindVec, TokenVec* pWithVec)
{
  _static_
  INTEGRITY_CHECK_STATIC(pThisVec);

  TokenArrayIter xIter = pThisVec->m_pTokenArray->xIterator();
  Token* pThisToken;

  while ((pThisToken = xIter.pNext()) != nullptr) {

    i8 i_Find = pFindVec->iFindToken(pThisToken->szChars());
    
    if (C_UNKNOWN == i_Find)
      continue;

    if (pWithVec->nTokens() <= i_Find)
      return false;

    Token* pWithToken = pWithVec->pGet(i_Find);

    // SE-93 temp hack: to cater to certain names being "|"
    // eg create table | (...)
    if (pWithToken->bSame("|"_Stz))
      return false;

    pThisVec->vUpdateToken(xIter.iArrayIndex(), pThisToken, pWithToken->szChars());
    pThisToken->vToken(pWithToken->eToken());
  }

  INTEGRITY_CHECK_STATIC(pThisVec);
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
  INTEGRITY_CHECK();

  i8 nSize = szArgChars->nSize();
  Stz* szExtension = _szByteSpaceExtend(nSize);
  StrLib::vCopy(szExtension->azChar(), nSize, szArgChars->azChar());

  Token oToken{};
  oToken.vTokenNum(m_pTokenArray->nItems());
  oToken.vToken(eToken);
  oToken.vChars(szExtension);
  m_pTokenArray->vAdd(oToken);

  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Token* pToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  vAppendToken(pToken->szChars(),pToken->eToken());
  
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* append given token vector to this token vector                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendVector(TokenVec* pTknVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    vAppendToken(pToken);
  }
  
  INTEGRITY_CHECK();
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
/* Copy an external string for the last token and point the token to it       */
/*----------------------------------------------------------------------------*/
void TokenVec::szCopyStrForLastToken(Token* pLastToken, Stz* szSrc)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  ASSERT(pLastToken);
  ASSERT(szSrc);

  i8 iNumTokens = nTokens();
  ASSERT(iNumTokens > 0);
  ASSERT(pGet(iNumTokens - 1) == pLastToken);

  _vCharsAddLast(pLastToken, szSrc);
  INTEGRITY_CHECK();
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

  TokenArrayIter xIter = m_pTokenArray->xIterator(i_Token - 1);
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
  INTEGRITY_CHECK();

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

  INTEGRITY_CHECK();
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

  while ( (pTknA = xIter1.pNext()) != nullptr &&
          (pTknB = xIter2.pNext()) != nullptr ) {

    ASSERT(pTknA != pTknB);

    Stz* szCharsA = pTknA->szChars();
    Stz* szCharsB = pTknB->szChars();

    i8 nBytesA = szCharsA->nSize();
    
    if (szCharsB != aAdd<Stz*>(szCharsA,nBytesA)) {
      vTrace("Error at [%lld] - [%lld] %p\n"_Stz,
             xIter1.iArrayIndex(),
             xIter2.iArrayIndex(),
             aAdd<char*>(szCharsA,nBytesA)
             );
      vDumpVec();
    }
    
    ASSERT(szCharsB == aAdd<Stz*>(szCharsA,nBytesA));

    // The character at szCharsA is either the start of the bytespace, or
    // the character before it is a null terminator from a previous string.
    ASSERT(szCharsA == m_xyByteSpace.szArea() || *(aAdd<char*>(szCharsA, -1)) == '\0');
    ASSERT(*(aAdd<char*>(szCharsB, -1)) == '\0');
  }
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
  INTEGRITY_CHECK();

  i8 i_Semi = iFindToken(";"_Stz,TN_OTHER);
  
  if (C_UNKNOWN == i_Semi)
    return;
  
  vRemoveToken(i_Semi);
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* remove a token from token vector                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::vRemoveToken(i8 i_CutTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  i8 nTkns = nTokens();
  
  if (i_CutTkn < nTkns - 1) {
    _vTokenRemoveInterior(i_CutTkn);
  }
  else {
    _vTokenRemoveLast(i_CutTkn);
  }
    
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* rewind current                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vRewindCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  ASSERT(!m_xTokenIter.bHasNext()); // at the last element
  ASSERT(!m_xTokenIter.bIsBeforeFirst());
  INTEGRITY_CHECK();

  i8 nBytes = pCurrToken()->nChars() + 1;
  
  _vByteSpaceTrim(nBytes);

  i8 iIdx = m_xTokenIter.iArrayIndex();
  m_xTokenIter.pPrevious();

  m_pTokenArray->vRemove(iIdx);
  INTEGRITY_CHECK();
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
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* strip any leading/trailing single quotes from all tokens                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    pToken->vStripSingleQuotes();
  }
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* strip off ';'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;

  while ((pToken = xIter.pNext()) != nullptr) {
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      vRemoveToken(xIter.iArrayIndex());
      break;
    }
  }
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* trim / remove all subsequent tokens from given token index                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vTrimTokens(i8 i_Tkn)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  Token* pToken = pGet(i_Tkn);
  Stz* szTrimmedLatter = pToken->szChars();
  
  m_xyByteSpace.vTrim(szTrimmedLatter);
  m_pTokenArray->vTrimRight(i_Tkn);
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  return m_pTokenArray->nItems();
}

/*----------------------------------------------------------------------------*/
/* copy token with new string                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vUpdateToken(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  i8 numTokens = nTokens();

  // Get the index in the array of this token. We'll do a reverse search
  // because it's more likely to be at the end.
  i8 iIdx = m_pTokenArray->iRevFind(pDstToken);
  ASSERT(iIdx >= 0);
  ASSERT(iIdx < numTokens);

  if (iIdx == numTokens - 1) // last item
    _vCharsReplaceLast(iIdx, pDstToken, szArgChars);
  else
    _vCharsReplaceInterior(iIdx, pDstToken, szArgChars);
  
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* copy token with new string                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::vUpdateToken(i8 iIdx, Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  ASSERT(m_pTokenArray->pGet(iIdx) == pDstToken);
  INTEGRITY_CHECK();

  i8 numTokens = nTokens();

  if (iIdx == numTokens - 1) // last item
    _vCharsReplaceLast(iIdx, pDstToken, szArgChars);
  else
    _vCharsReplaceInterior(iIdx, pDstToken, szArgChars);

  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into one string                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vToTknString(String* psDstStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(psDstStr->azStr(), psDstStr->nAreaBytes(), "");
  
  // concat all the tokens together
  TokenArrayIter xIter = m_pTokenArray->xIterator();
  Token* pToken;
  bool bFirst = true;

  while ((pToken = xIter.pNext()) != nullptr) {

    if (bFirst) {
      bFirst = false;
    } else {
      StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), ' ');
    }
    
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

  TokenArrayIter xIter = m_pTokenArray->xIterator(iFromTkn - 1);
  Token* pToken;
  bool bFirst = true;

  // concat all the tokens together
  while ((pToken = xIter.pNext()) != nullptr) {

    if (bFirst) {
      bFirst = false;
    } else {
      StrLib::vConcat(psDstStr->azStr(), psDstStr->nAreaBytes(), ' ');
    }

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
/* add chars for a last token, extending the bytespace                        */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsAddLast(Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  // Assumes that the Tokens string is not currently pointing to some bytespace area
  ASSERT( !(pDstToken->szChars() >= m_xyByteSpace.szArea() &&
            pDstToken->szChars() <= m_xyByteSpace.szBoundary() ));

//  vListTokens();
  i8 nSize = szArgChars->nSize();

  Stz* szExtension = _szByteSpaceExtend(nSize);

  pDstToken->vChars(szExtension);

  StrLib::vCopy(szExtension->azChar(), nSize, szArgChars->azChar());

//  vListTokens();
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* copy chars into a non-last token                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsReplaceInterior(i8 iIdx, Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  INTEGRITY_CHECK();

//  SP_Token spTkn;
  //vTrace("Copying:- %s <= %s\n"_Stz,spTkn.sz(pDstToken),szArgChars);

  ASSERT(iIdx >= 0);
  ASSERT(iIdx < nTokens() - 1); // not the last item

  // Note: This assumes that the token's string pointer is already pointing
  //       into the bytespace area. Hence this needs to be removed first.
  ASSERT(pDstToken->szChars() >= m_xyByteSpace.szArea());
  ASSERT(pDstToken->szChars() <= m_xyByteSpace.szBoundary());
  
  Stz* szInsitui = pDstToken->szChars();
  i8 nOldSize = pDstToken->nChars() + 1;
  i8 nNewSize = szArgChars->nSize();
  i8 nSizeDiff = nNewSize - nOldSize;

  if (nSizeDiff > 0) {
    // insert space into the bytespace
    _vByteSpaceInsert(szInsitui,nSizeDiff);

  } else if (nSizeDiff < 0) {
    // remove space from the bytespace
    _vByteSpaceRemove(szInsitui,-nSizeDiff);
  }

  // Now we can copy the new string into the byte space
  StrLib::vCopy(szInsitui->azChar(), nNewSize, szArgChars->azChar());
  _vVecAdjustChars(iIdx + 1, nSizeDiff);

  // * * * * * * * * * * * * * * * * * * * *

  
  //vTrace("--- --- --- --- --- ---\n"_Stz);
  //vDumpVec(16);

  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* replace chars for a last token. Token already has chars in bytespace       */
/*----------------------------------------------------------------------------*/
void TokenVec::_vCharsReplaceLast(i8 iIdx, Token* pDstToken, Stz* szArgChars)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  ASSERT(iIdx == nTokens() - 1);
  UNUSED(iIdx);

  ASSERT(pDstToken->szChars() >= m_xyByteSpace.szArea());
  ASSERT(pDstToken->szChars() <= m_xyByteSpace.szBoundary());

//  vListTokens();
  INTEGRITY_CHECK();
  i8 nOldSize = pDstToken->szChars()->nSize();
  i8 nNewSize = szArgChars->nSize();
  i8 iSizeDiff = nNewSize - nOldSize;

  if (iSizeDiff > 0) {
    _vByteSpaceTake(iSizeDiff);
  } else if (iSizeDiff < 0) {
    _vByteSpaceTrim(-iSizeDiff);
  }

  StrLib::vCopy(pDstToken->azChars(), nNewSize, szArgChars->azChar());

//  vListTokens();
  INTEGRITY_CHECK();
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
  
  INTEGRITY_CHECK();

  Token* pTknCut = pGet(iIdx);
//  i8 i_CutTkn = pTknCut->i_Token();
//  SP_Token spTkn;
  //vTrace("Cutting:- %s %p\n"_Stz,spTkn.sz(pTknCut),pTknCut->azChars());

  //vTrace("\n\n\n"_Stz);
  //vDumpVec();
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
    pToken->vChars(aAdd<Stz*>(pToken->szChars(), -nBytesToCut));
  } while ((pToken = xIter.pNext()) != nullptr);

  // * * * * * * * * * * * * * * * * * * * * * *
  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* cut / erase the last token                                                 */
/*----------------------------------------------------------------------------*/
void TokenVec::_vTokenRemoveLast(i8 iIdx)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  INTEGRITY_CHECK();

  Token* pTknCut = pGet(iIdx);
  
  // reclaim the chars byte space
  i8 nBytes = pTknCut->nChars() + 1;
  _vByteSpaceTrim(nBytes);

  m_pTokenArray->vRemove(iIdx);

  INTEGRITY_CHECK();
}

/*----------------------------------------------------------------------------*/
/* step thru tokens adjusting location of each chars by given +/- bytes       */
/*----------------------------------------------------------------------------*/
void TokenVec::_vVecAdjustChars(i8 i_Tkn, i8 nByBytes)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  TokenArrayIter xIter = m_pTokenArray->xIterator(i_Tkn - 1);
  Token* pTknAdj;

  while ((pTknAdj = xIter.pNext()) != nullptr) {
    pTknAdj->vAdjustChars(nByBytes);
  }

  INTEGRITY_CHECK();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
