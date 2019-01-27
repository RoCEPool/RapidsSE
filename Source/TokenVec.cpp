/*============================================================================*/
/* TokenVec.hpp -- vector of tokens                                           */
/*============================================================================*/

#include "Commands.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TokenVec* TokenVec::_pTokenSet(void* aAddr)
{
  TokenVec* pTokenSet = reinterpret_cast<TokenVec*>(aAddr);
  
  RASSERT_TYPE(pTokenSet,CN_TOKENVEC);
  
  return pTokenSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TokenVec::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_TOKENVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TokenVec::operator delete(void* pAddr)
{
  TokenVec* pTokenVec = reinterpret_cast<TokenVec*>(pAddr);
  
  HeapMem::vFree(pTokenVec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenVec::TokenVec(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token] = new Token;
  
  vReset();
  
  m_bDetachedRequest = false;
  
  _checkInitialised_(CN_TOKENVEC);
  
  _postcondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenVec::~TokenVec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* add a token                                                                */
/*----------------------------------------------------------------------------*/
void TokenVec::vAddToken(Stz* aStr)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_pItem[m_nItems++]->vCopy(aStr);
  
}

/*----------------------------------------------------------------------------*/
/* append given token vector to this token vector                             */
/*----------------------------------------------------------------------------*/
void TokenVec::vAppendVector(TokenVec* pTokenVec)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < pTokenVec->nTokens(); i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);
    vAddToken(pToken->aText());
  }
}

/*----------------------------------------------------------------------------*/
/* count tokens in token array                                                */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iCountTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if ( ! pToken->bHasText() )
      return i_Token;
  }
  
  ASSERT("Should never be reached"_Stz);
  return -1;
}

/*----------------------------------------------------------------------------*/
/* cut a token from token vector                                              */
/*----------------------------------------------------------------------------*/
void TokenVec::vCutToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  i8 nTkns = nItems();
  
  for (i8 i_Token = i_Tkn; i_Token < nTkns ; i_Token++) {
    
    Token* pTknA = m_pItem[i_Token];
    Token* pTknB = m_pItem[i_Token + 1];
    
    pTknA->vCopy(pTknB);
  }
  
  m_nItems--;
}

/*----------------------------------------------------------------------------*/
/* get detached request                                                       */
/*----------------------------------------------------------------------------*/
bool TokenVec::bDetachedRequest(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  return m_bDetachedRequest;
}

/*----------------------------------------------------------------------------*/
/* set detached request                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vDetachedRequest(bool bDetachedRequest)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_bDetachedRequest = bDetachedRequest;
}

/*----------------------------------------------------------------------------*/
/* dump token array                                                           */
/*----------------------------------------------------------------------------*/
void TokenVec::vDumpTokens(Stz* pTitle, i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  vTrace("%s\n"_Stz,pTitle);
  
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pToken = m_pItem[i_Token];

    if (! pToken->aText())
      break;
    
    vTrace("%s\n"_Stz, pToken->aText());
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 TokenVec::iFindToken(Stz* aLookFor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if (aLookFor->bSame(pToken->aText()))
      return i_Token;
  }
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* find token given text                                                      */
/*----------------------------------------------------------------------------*/
Token* TokenVec::pFindToken(Stz* aText, i8& i_Token)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i_Token++; i_Token < nTokens(); i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if (pToken->sText().bSame(aText))
      return pToken;
  }
  
  return nullptr;
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
/* inc number of tokens                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vIncTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_nItems++;
}

/*----------------------------------------------------------------------------*/
/* print this token vector                                                    */
/*----------------------------------------------------------------------------*/
void TokenVec::vPrint(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  for (i8 i_Token = 0; i_Token < nItems(); i_Token++)
    vTrace("  %s\n"_Stz,m_pItem[i_Token]->aText());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void TokenVec::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  m_nItems = 0;
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++)
    m_pItem[i_Token]->vReset();
  
  m_bDetachedRequest = false;
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetAllTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
    
  m_nItems = C_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* set the number of tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenVec::vSetTokens(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
   // get count of words in pWords
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
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
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if (TN_OTHER == pToken->eToken() && pToken->bSame("&"_Stz)) {
      m_bDetachedRequest = true;
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
  
  for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    Token* pToken = m_pItem[i_Token];
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz)) {
      pToken->vReset();
      break;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* concat all tokens into one string                                          */
/*----------------------------------------------------------------------------*/
void TokenVec::vToString(Stz* aBuff)
{
  _precondition_(_MAGIC_,CN_TOKENVEC,_FOOTER_);
  
  StrLib::vCopy(aBuff->aChar(),"");
  
  i8 nTokens = this->nTokens();
  
  // concat all the tokens together
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pToken = pGet(i_Token);
    
    eTN eToken = pToken->eToken();
    
    switch (eToken) {
        
      case TN_INTEGER:
        StrLib::vConcat(aBuff->aChar(),pToken->aText()->aChar());
        break;
        
      case TN_NAME:
        StrLib::vConcat(aBuff->aChar(),pToken->aText()->aChar());
        break;
        
      case TN_OTHER:
        StrLib::vConcat(aBuff->aChar(),pToken->aText()->aChar());
        break;
        
      case TN_STRING:
        StrLib::vConcat(aBuff->aChar(),"\"");
        StrLib::vConcat(aBuff->aChar(),pToken->aText()->aChar());
        StrLib::vConcat(aBuff->aChar(),"\"");
        break;
        
      default:
        NEVER_REACH;
    }
    
    StrLib::vConcat(aBuff->aChar()," ");
  }
  
  // concat a terminating semicolon (needed??)
  StrLib::vConcat(aBuff->aChar(),";");
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* TokenVec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TOKENVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
