/*============================================================================*/
/* Token.hpp -- Tokens                                                        */
/*============================================================================*/

#include "Commands.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Token* Token::_pToken(void* aAddr)
{
  Token* pToken = reinterpret_cast<Token*>(aAddr);
  
  RASSERT_TYPE(pToken,CN_TOKEN);
  
  return pToken;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Token::operator new(size_t)
{
  void* pToken = HeapMem::pAllocate(CN_TOKEN);
  
  return pToken;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Token::operator delete(void* pTokenAddress)
{
  Token* pToken = _pToken(pTokenAddress);
  
  HeapMem::vFree(pToken);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Token::Token(void)
: Object(CN_TOKEN)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  vReset();
  
  _checkInitialised_(CN_TOKEN);
  
  _postcondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Token::~Token(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Token::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Token* pToken = _pToken(pObject);
  
  vTrace("token %s %s\n"_Stz,
         aTN[pToken->m_eToken],
         pToken->m_sText.aChar());
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN Token::eToken(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_eToken;
}

/*----------------------------------------------------------------------------*/
/* set token kind                                                             */
/*----------------------------------------------------------------------------*/
void Token::vToken(eTN eToken)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken = eToken;
}

/*----------------------------------------------------------------------------*/
/* get text                                                                   */
/*----------------------------------------------------------------------------*/
s256& Token::sText(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_sText;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken = TN_NONE;
  m_sText = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bIsNumeric(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_sText.bIsNumeric();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_sText.bSame(aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bHasText(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = *(char*)m_sText.aStz() ? true : false;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_sText.vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Token* pGivenToken)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken = pGivenToken->m_eToken;
  m_sText.vCopy(pGivenToken->aText());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vConcat(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_sText.vConcat(aArg);
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iToI8()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 i = m_sText.iToI8();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iLength()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 iLen = m_sText.iLength();
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
Stz* Token::aText(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_sText.aStz();
}

/*----------------------------------------------------------------------------*/
/* strip single quotes from token                                             */
/*----------------------------------------------------------------------------*/
void Token::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  StrLib::vRemove(m_sText.aChar(),'\'');
  StrLib::vStrip(m_sText.aChar(),'\'');
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Token::aFooter(void)
{
  _precondition_(_MAGIC_,CN_TOKEN);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
