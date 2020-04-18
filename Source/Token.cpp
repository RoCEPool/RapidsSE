/*============================================================================*/
/* Token.hpp -- Tokens                                                        */
/*============================================================================*/

#include "LexicalAnalysis.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(LexicalAnalysis,Token,CN_TOKEN)
__aNewFunction(Token,CN_TOKEN)
__vDeleteFunction(Token,CN_TOKEN)
__aFooterFunction(Token)


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
         pToken->azChars());
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the charsed characters of the token                                    */
/*----------------------------------------------------------------------------*/
char* Token::azChars(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_szChars->azChar();
}

/*----------------------------------------------------------------------------*/
/* get the charsed characters of the token                                    */
/*----------------------------------------------------------------------------*/
Stz* Token::szChars(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  return m_szChars;
}

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
  
  m_szChars->vConcat(aArg);
}

/*----------------------------------------------------------------------------*/
/* copy given chars into this token                                           */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Stz* szChars)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_szChars->vCopy(szChars);
}

/*----------------------------------------------------------------------------*/
/* copy given token into this token                                          */
/*----------------------------------------------------------------------------*/
void Token::vCopy(Token* pGivenToken)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_eToken = pGivenToken->m_eToken;
  m_szChars->vCopy(pGivenToken->szChars());
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bHasChars(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = *(char*)m_szChars ? true : false;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iLength()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 iLen = m_szChars->nLength();
  
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
  
  bool b = m_szChars->bIsNumeric();
  
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
  
  // tbd...
  m_szChars = m__sCharsBuff.szStz();
  

  m_eToken   = TN_NONE;
  *m_szChars->azChar() = '\0';
  
  m_iLineNo  = C_UNKNOWN;
  m_iColNo   = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
bool Token::bSame(Stz* aArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_szChars->bSame(aArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* reset this token set                                                       */
/*----------------------------------------------------------------------------*/
i8 Token::iToI8(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 i = m_szChars->iToI8();
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* strip single quotes from token                                             */
/*----------------------------------------------------------------------------*/
void Token::vStripSingleQuotes(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  StrLib::vRemove(m_szChars->azChar(),'\'');
  StrLib::vStrip(m_szChars->azChar(),'\'');
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
