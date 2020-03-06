/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
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
  
  if (bNewTokens) {
    for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++)
      m_pItem[i_Token] = new Token;
    vReset();
  }
  else {
    for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++)
      m_pItem[i_Token] = nullptr;
  }
  
  m_iCurTkn = C_UNKNOWN;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendToken(Stz* szTerm)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_pItem[m_nItems++]->vCopy(szTerm);
}

/*----------------------------------------------------------------------------*/
/* append given token vector to this token vector                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendVector(TokenVec* pTokenVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTokenVec->nTokens(); i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);
    vAppendToken(pToken->szTerm());
  }
}

/*----------------------------------------------------------------------------*/
/* count tokens in token array                                                */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iCountTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if ( ! pToken->bHasText() )
      return i_Token;
  }
  
  ASSERT("Should never be reached"_Stz);
  return -1;
}

/*----------------------------------------------------------------------------*/
/* copy given token vector to this token vector                               */
/*----------------------------------------------------------------------------*/
void TokenVec::vCopyVector(TokenVec* pTokenVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTokenVec->nTokens(); i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);

    Token* pNewToken = new Token(pToken);
    
    vAdd(pNewToken);
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
  
  Token* pTkn = pGet(m_iCurTkn);
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
    
    Token* pTknDst = m_pItem[i_Token];
    Token* pTknSrc = m_pItem[i_Token + 1];
    
    pTknDst->vCopy(pTknSrc);
  }
  
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* dump token array                                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < nItems(); i_Token++) {
    
    Token* pToken = m_pItem[i_Token];

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
    Token* pToken = m_pItem[i_Token];
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
    Token* pToken = m_pItem[i_Token];
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
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* insert a token                                                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vInsertToken(i8 i_Tkn, Stz* szTerm, eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
      
  for (i8 i_Token = m_nItems - 1; i_Token >= i_Tkn ; i_Token--) {
    
    Token* pTknDst = m_pItem[i_Token + 1];
    if (! pTknDst) {
      m_pItem[i_Token + 1] = new Token;
      pTknDst = m_pItem[i_Token + 1];
    }
    
    Token* pTknSrc = m_pItem[i_Token];
    
    pTknDst->vCopy(pTknSrc);
  }
  
  m_nItems++;
  
  Token* pTknIns = m_pItem[i_Tkn];
  
  pTknIns->vCopy(szTerm);
  pTknIns->vToken(eToken);
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
    vTrace("  %s\n"_Stz,m_pItem[i_Token]->szTerm());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++)
    m_pItem[i_Token]->vReset();
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetAllTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  m_nItems = C_MAX_TOKENS;
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
    if ( ! m_pItem[i_Token]->bHasText() )
      break;
  }
  
  m_nItems = i_Token;
}

/*----------------------------------------------------------------------------*/
/* strip off '&'                                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vStripAmpersand(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_MAX_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
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
  for (i8 i_Token = 0; i_Token < nTokens(); i_Token++) {
    Token* pToken = m_pItem[i_Token];
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
    Token* pToken = m_pItem[i_Token];
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      pToken->vReset();
      //break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get number of tokens                                                       */
/*----------------------------------------------------------------------------*/
i8 TokenVec::nTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  return m_nItems;
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
