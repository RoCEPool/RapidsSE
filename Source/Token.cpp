/*============================================================================*/
/* Token.hpp -- Tokens                                                        */
/*============================================================================*/

#include "LexicalAnalysis.h"

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
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Token::Token(Token* pToCopy)
: Object(CN_TOKEN)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  vReset();
  
  vCopy(pToCopy);
  
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
         pToken->m_sTerm.azChar());
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the column number                                                      */
/*----------------------------------------------------------------------------*/
i8 Token::iColNo()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_iColNo;
}

/*----------------------------------------------------------------------------*/
/* set the column number                                                      */
/*----------------------------------------------------------------------------*/
void Token::vColNo(i8 iColNo)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_iColNo = iColNo;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vConcat(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_sTerm.vConcat(aArg);
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_sTerm.vCopy(aArg);
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Token* pGivenToken)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken = pGivenToken->m_eToken;
  m_sTerm.vCopy(pGivenToken->szTerm());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bHasText(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = *(char*)m_sTerm.szStz() ? true : false;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iLength()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 iLen = m_sTerm.nLength();
  
  return iLen;
}

/*----------------------------------------------------------------------------*/
/* is this token an integer?                                                  */
/*----------------------------------------------------------------------------*/
bool Token::bIsInteger(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_INTEGER == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a name?                                                      */
/*----------------------------------------------------------------------------*/
bool Token::bIsName(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_NAME == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token numeric?                                                     */
/*----------------------------------------------------------------------------*/
bool Token::bIsNumeric(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_sTerm.bIsNumeric();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get the line number                                                        */
/*----------------------------------------------------------------------------*/
i8 Token::iLineNo()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_iLineNo;
}

/*----------------------------------------------------------------------------*/
/* set the line number                                                        */
/*----------------------------------------------------------------------------*/
void Token::vLineNo(i8 iLineNo)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_iLineNo = iLineNo;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
void Token::vReset(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken  = TN_NONE;
  m_sTerm   = ""_Stz;
  
  m_iLineNo = C_UNKNOWN;
  m_iColNo  = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_sTerm.bSame(aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get the az of term                                                         */
/*----------------------------------------------------------------------------*/
char* Token::azTerm(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_sTerm.azChar();
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
Stz* Token::szTerm(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_sTerm.szStz();
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iToI8(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 i = m_sTerm.iToI8();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* strip single quotes from token                                             */
/*----------------------------------------------------------------------------*/
void Token::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  StrLib::vRemove(m_sTerm.azChar(),'\'');
  StrLib::vStrip(m_sTerm.azChar(),'\'');
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
