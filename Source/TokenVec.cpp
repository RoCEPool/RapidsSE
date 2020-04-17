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
    
    i8 i_Find = pFindVec->iFindToken(pThisToken->szTerm());
    
    if (C_UNKNOWN == i_Find)
      continue;
    
    Token* pWithToken = pWithVec->pGet(i_Find);

    pThisToken->vCopy(pWithToken->szTerm());
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
    pExtractedVec->vAppendToken(pToken->szTerm());
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
    
    pExtractedVec->vAppendToken(pToken->szTerm());
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
void TokenVec::vAdvanceCurr(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  m_iCurTkn++;
}

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Stz* szTerm)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  pGet(nItems(),true)->vCopy(szTerm);

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
    vAppendToken(pToken->szTerm());
  }
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool TokenVec::bBlankLine(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  if (0 == nTokens() ||
      0 == pGet(0)->iLength())
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
    if ( ! pToken->bHasText() )
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
    
    pToken_dst->vCopy(pToken_src);
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
void TokenVec::vCutToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nItems();
  
  for (i8 i_Token = i_Tkn; i_Token < nTkns ; i_Token++) {
    
    Token* pTknDst = pGet(i_Token);
    Token* pTknSrc = pGet(i_Token + 1,true);
    
    pTknDst->vCopy(pTknSrc);
  }
  
  vDecItems();
}

/*----------------------------------------------------------------------------*/
/* dump token array                                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < nItems(); i_Token++) {
    
    Token* pToken = pGet(i_Token);

    if (! pToken->szTerm())
      break;
    
    vTrace("  (%2lld,%2lld) %s %s\n"_Stz,
           pToken->iLineNo(),
           pToken->iColNo(),
           aTN[pToken->eToken()],
           pToken->szTerm());
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* szTerm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = pGet(i_Token,true);
    if (szTerm->bSame(pToken->szTerm()))
      return i_Token;
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find token given text                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFindToken(Stz* szTerm, i8& i_Token)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i_Token++; i_Token < nTokens(); i_Token++) {
    Token* pToken = pGet(i_Token);
    if (pToken->szTerm()->bSame(szTerm))
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
/* insert a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vInsertToken(i8 i_Tkn, Stz* szTerm, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  // open up a token at _i_Tkn
  for (i8 i_Token = nItems() - 1; i_Token >= i_Tkn ; i_Token--) {
    
    Token* pTkn_dst = pGet(i_Token + 1,true);
    Token* pTkn_src = pGet(i_Token);
    
    pTkn_dst->vCopy(pTkn_src);
  }
  
  vIncItems();
  
  // - - - - - - - - - - - - - - - - -  - - - - - - - - - - - - - - - - - - - -
  // now copy the inserting token into place
  
  Token* pTkn_ins = pGet(i_Tkn);
  
  pTkn_ins->vCopy(szTerm);
  pTkn_ins->vToken(eToken);
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
/* print this token vector                                                    */
/*----------------------------------------------------------------------------*/
void TokenVec::vPrint(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < nItems(); i_Token++)
    vTrace("  %s\n"_Stz,pGet(i_Token)->szTerm());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_iCurTkn = -1;

  vItems(0);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++)
    pGet(i_Token,true)->vReset();
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
    if ( ! pGet(i_Token,true)->bHasText() )
      break;
  }
  
  vItems(i_Token);
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
/* tokenise given string into given TokenVec                                  */
/*----------------------------------------------------------------------------*/
void TokenVec::vTokenise(Stz* szString)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

  vReset();
  
  Input* pInput = new Input;
  pInput->vUseString(szString);
  pInput->vFormSenTokens();
  pInput->vUseBegin();

  for (i8 i_Token = 0; ; i_Token++) {
    pInput->vReadToken();
    if (TN_EOF == pInput->eToken())
      break;
    
    vIncTokens();
    
    Token* pToken = pGet(i_Token);
    pToken->vToken(pInput->eToken());
    pToken->vCopy(pInput->szToken());
    
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
        StrLib::vConcat(szDstStr->azChar(),pToken->szTerm()->azChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(szDstStr->azChar(),pToken->szTerm()->azChar());
        break;
        
      case TN_EOF:
        break;
        
      case TN_OTHER:
        StrLib::vConcat(szDstStr->azChar(),pToken->szTerm()->azChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(szDstStr->azChar(),"\"");
        StrLib::vConcat(szDstStr->azChar(),pToken->szTerm()->azChar());
        StrLib::vConcat(szDstStr->azChar(),"\"");
        break;
        
      case TN_TAG:
        StrLib::vConcat(szDstStr->azChar(),pToken->szTerm()->azChar());
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
