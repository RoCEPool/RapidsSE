/*============================================================================*/
/* Token.hpp -- Tokens                                                        */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,Token,CN_TOKEN)
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
  
  m_p_ExoTknVec = nullptr;
  m_i_Token     = C_UNKNOWN;
  
  vReset();
  
  _checkInitialised_(CN_TOKEN);
  
  _postcondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Token::Token(Token* pToCopy, TokenVec* pCaller)
: Object(CN_TOKEN)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_p_ExoTknVec = nullptr;
  m_i_Token     = C_UNKNOWN;

  vReset();
  
  vCopyToken_restricted(pToCopy,pCaller);
  
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
  UNUSED(eClass);
  UNUSED(aTab);

  vTrace("token %s %s\n"_Stz,
         aTN[pToken->m_eToken],
         pToken->azChars());
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool Token::bStartCppInteger(char cArg)
{
  _static_
  
  if (StrLib::bIsDigit(cArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Token::bStartCppName(char cArg)
{
  _static_

  //if ('$' == cArg)
  //  return true;
  
  if (StrLib::bIsNameStartChar(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Token::bStartCppOther(char cArg)
{
  _static_

  if (! StrLib::bIsNameBodyChar(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Token::bStartCppString(char cArg)
{
  _static_

  if (StrLib::bIsDoubleQuote(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Token::bEndCppString(char cArg)
{
  _static_

  if (StrLib::bIsDoubleQuote(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen integer                                              */
/*----------------------------------------------------------------------------*/
bool Token::bStartSenInteger(char cArg)
{
  _static_

  if (StrLib::bIsMinus(cArg) || StrLib::bIsDigit(cArg) || StrLib::bIsDot(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen name                                                 */
/*----------------------------------------------------------------------------*/
bool Token::bStartSenName(char cArg)
{
  _static_

  if (bStartSenInteger(cArg) || bStartSenString(cArg))
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen string                                               */
/*----------------------------------------------------------------------------*/
bool Token::bStartSenString(char cArg)
{
  _static_

  if (StrLib::bIsDoubleQuote(cArg))
    return true;
  
// 2020-Feb-26 RJH: I commented this out as I don't think we have 'string'
//                  in sen literals
  if (StrLib::bIsSingleQuote(cArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen tag                                                  */
/*----------------------------------------------------------------------------*/
bool Token::bStartSenTag(char cArg)
{
  _static_

  if (StrLib::bIsLessThan(cArg))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of Sen string                                                 */
/*----------------------------------------------------------------------------*/
bool Token::bEndSenString(char cArg)
{
  _static_

  if (StrLib::bIsDoubleQuote(cArg))
    return true;

  if (StrLib::bIsSingleQuote(cArg))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* collect Sen name                                                           */
/*----------------------------------------------------------------------------*/
eTN Token::eCollectSenName(char** ps, Stz* szString)
{
  _static_

  char *azChars = StrLib::zNullTerminator(szString->azChar());
  char *z = azChars;
  
  // collect the characters of the name
  while ( **ps && !StrLib::bIsWhite(**ps) && !bStartSenString(**ps) ) {
    *z++ = **ps;
    (*ps)++;
  }
  
  // terminate name
  *z = '\0';

  if (StrLib::bIsBoolean(szString->azChar()))
    return TN_BOOLEAN;
  else
    return TN_NAME;
}

/*----------------------------------------------------------------------------*/
/* collect Sen number (integer, decimal, float)                               */
/*----------------------------------------------------------------------------*/
eTN Token::eCollectSenNumber(char** ps, Stz* szString)
{
  _static_

  char *azChars = StrLib::zNullTerminator(szString->azChar());
  char *z = azChars;

  i8 nIntegerDigits  = 0;
  i8 nFractionDigits = 0;
  i8 nExponentDigits = 0;

  // we have seen a minus or a digit
  ASSERT(bStartSenInteger(cArg));

  // by default we are collectin an integer
  eTN eToken = TN_INTEGER;

  // - start parsing an integer
  
  // collect leading sign
  if (StrLib::bIsSign(**ps)) {
    *z++ = **ps;
    (*ps)++;
  }
  
  // collect initial digits of an integer
  while (StrLib::bIsDigit(**ps)) {
    *z++ = **ps;
    nIntegerDigits++;
    (*ps)++;
  }
  *z = '\0';

  // 0x hex number?
  if (1 == nIntegerDigits && StrLib::bIsX(**ps)) {

    // add the 'x'
    *z++ = **ps;
    
    // collect the hex digits
    (*ps)++;
    while (StrLib::bIsHex(**ps)) {
      *z++ = **ps;
      nIntegerDigits++;
      (*ps)++;
    }
    *z = '\0';
  }
  

  // - handle minus only
  if (! nIntegerDigits && ! StrLib::bIsDot(**ps)) {
    eToken = eCollectSenName(ps,szString);
    return eToken;
  }

  // . handle dot only
  if ( ! nIntegerDigits && StrLib::bIsDot(**ps)) {

    // collect next char
    (*ps)++;

    if ( ! StrLib::bIsDigit(**ps) ) {
      (*ps)--;
      eToken = eCollectSenName(ps,szString);
      return eToken;
    }
    (*ps)--;
  }

  // dot indicating fraction digits of decimal number follow
  if (StrLib::bIsDot(**ps)) {

    *z++ = **ps;
    (*ps)++;

    // collect digits after decimal
    while (StrLib::bIsDigit(**ps)) {
      *z++ = **ps;
      nFractionDigits++;
      (*ps)++;
    }


    // terminate the decimal literal
    *z = '\0';
    
    eToken = TN_DECIMAL;
  }
  

  // an e|E indicating a float number ?
  if ('e' == **ps || 'E' == **ps) {
    
    *z++ = **ps;
    (*ps)++;

    // collect optional sign after E
    if (StrLib::bIsSign(**ps)) {
      *z++ = **ps;
      (*ps)++;
    }


    // collect digits after E
    while (StrLib::bIsDigit(**ps)) {
      *z++ = **ps;
      nExponentDigits++;
      (*ps)++;
    }

    // terminate the float literal
    *z = '\0';
    
    if (! nExponentDigits) {
      eToken = eCollectSenName(ps,szString);
      return eToken;
    }
    
    eToken = TN_FLOAT;
  }
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if (StrLib::bIsNull(**ps) || StrLib::bIsWhite(**ps) || bStartSenString(**ps) ) {
    
    // push back the unconsumed char
    (*ps)--;
    
    // terminate
    *z = '\0';
    return eToken;
  }
  
  // its actually a name; complete parsing name
  eToken = eCollectSenName(ps,szString);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
eTN Token::eCollectSenString(char** ps, Stz* szString)
{
  _static_

  char *azChars = StrLib::zNullTerminator(szString->azChar());
  char *z = azChars;
    
  // skip the leading '"' or '\''
  char cStartQuote = **ps;
  (*ps)++;

  // collect the chars within the string
  while ( **ps && **ps != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,**ps);
        
    if (StrLib::bIsBackSlash(**ps)) {
      (*ps)++;
      // translate \n into new line
      if ('n' == **ps)
         *z++ = '\n';
      else
        *z++ = **ps;
      (*ps)++;
    }
    else {
      *z++ = **ps;
      (*ps)++;
    }
  }
 
  // terminate the strlit
  *z = '\0';
  
  return TN_STRING;
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
eTN Token::eCollectSenTag(char** ps, Stz* szString)
{
  _static_

  char *azChars = StrLib::zNullTerminator(szString->azChar());
  char *z = azChars;
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! **ps || StrLib::bIsWhite(**ps) || bStartSenString(**ps) ) {
    // push back the unconsumed char
    (*ps)--;

    // terminate name
    *z = '\0';
    
    return TN_NAME;
  }
     
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the characters of the name
  
  while ( **ps && !StrLib::bIsWhite(**ps) && !bStartSenString(**ps) ) {
    *z++ = **ps;
    (*ps)++;
  }
  
  // push back the unconsumed char
  (*ps)--;

  // terminate a namelit
  *z = '\0';

  return TN_TAG;
}

/*----------------------------------------------------------------------------*/
/* collect next sen token                                                     */
/*----------------------------------------------------------------------------*/
eTN Token::eCollectSenToken(char** ps, Stz* szString)
{
  _static_

  StrLib::vSkipSpaces(ps);
  if (! **ps)
    return TN_NONE;
  
  
  if (bStartSenInteger(**ps)) {
    //vCollectSenNumber();
    return TN_NONE;
  }
  
  if (bStartSenString(**ps)) {
    //vCollectSenString();
    return TN_STRING;
  }

  //vCollectSenName();

  return TN_NONE;
}


/*----------------------------------------------------------------------------*/
/*-- Restricted --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy given chars into this token                                           */
/*----------------------------------------------------------------------------*/
void Token::vUpdateChars(Stz* szArgChars, TokenVec* pExoTknVec)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  UNUSED(pExoTknVec);

  ASSERT(pExoTknVec == m_p_ExoTknVec);

  m_p_ExoTknVec->vUpdateToken(this,szArgChars);
}

/*----------------------------------------------------------------------------*/
/* copy given token into this token                                           */
/*----------------------------------------------------------------------------*/
void Token::vCopyToken_restricted(Token* pSrcToken, TokenVec* pExoTknVec)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  UNUSED(pExoTknVec);

  ASSERT(pExoTknVec == m_p_ExoTknVec);
  
  m_p_ExoTknVec->vReplaceToken(this,pSrcToken);
}

/*----------------------------------------------------------------------------*/
/* init this token                                                            */
/*----------------------------------------------------------------------------*/
void Token::vInitToken_restricted(i8 i_Token, TokenVec* pExoTknVec)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  ASSERT(! m_p_ExoTknVec);
  
  m_p_ExoTknVec = pExoTknVec;
  m_i_Token     = i_Token;
}

/*----------------------------------------------------------------------------*/
/* ripple copy given token into this token                                    */
/*----------------------------------------------------------------------------*/
void Token::vRippleCopy_restricted(Token* pSrcToken, i8 nAdjCharsBytes,
                                   TokenVec* pExoTknVec)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  UNUSED(pExoTknVec);

  ASSERT(pExoTknVec == m_p_ExoTknVec);
  
  m_eToken  = pSrcToken->m_eToken;
  m_szChars = aAdd<Stz*>(pSrcToken->m_szChars,nAdjCharsBytes);
  m_iLineNo = pSrcToken->m_iLineNo;
  m_iColNo  = pSrcToken->m_iColNo;
}

/*----------------------------------------------------------------------------*/
/* set chars of this token to given arg                                       */
/*----------------------------------------------------------------------------*/
void Token::vSetChars_restricted(Stz* szArg, TokenVec* pExoTknVec)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  UNUSED(pExoTknVec);

  ASSERT(pExoTknVec == m_p_ExoTknVec);
  
  m_szChars = szArg;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust this tokens m_szChars address by +/- the given number of bytes      */
/*----------------------------------------------------------------------------*/
void Token::vAdjustChars(i8 nBytes)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  Stz* szAdjustedChars = aAdd<Stz*>(m_szChars,nBytes);

  m_szChars = szAdjustedChars;
}

/*----------------------------------------------------------------------------*/
/* string length of chars                                                     */
/*----------------------------------------------------------------------------*/
i8 Token::nChars()
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  i8 nChars = m_szChars->nLength();
  
  return nChars;
}

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
/* set the charsed character area for this token                              */
/*----------------------------------------------------------------------------*/
void Token::vChars(Stz* szChars)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  m_szChars = szChars;
}

/*----------------------------------------------------------------------------*/
/* get the column number                                                      */
/*----------------------------------------------------------------------------*/
i8 Token::iColNo(void)
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
/* does this token have chars?                                                */
/*----------------------------------------------------------------------------*/
bool Token::bHasChars(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = *(char*)m_szChars ? true : false;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* is this token an arithmetic operator?                                      */
/*----------------------------------------------------------------------------*/
bool Token::bIsArithmetic(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  // arithmetic operators
  if (TN_NAME == m_eToken && bSame("+"_Stz))
    return true;

  if (TN_NAME == m_eToken && bSame("/"_Stz))
    return true;

  if (TN_NAME == m_eToken && bSame("%"_Stz))
    return true;

  if (TN_NAME == m_eToken && bSame("*"_Stz))
    return true;

  if (TN_NAME == m_eToken && bSame("-"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a = ?                                                        */
/*----------------------------------------------------------------------------*/
bool Token::bIsAssign(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (bSame("="_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a boolean value                                              */
/*----------------------------------------------------------------------------*/
bool Token::bIsBoolean(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  if (TN_BOOLEAN == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token decimal no?                                                  */
/*----------------------------------------------------------------------------*/
bool Token::bIsDecimal(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  //if (TN_NAME == m_eToken && StrLib::bIsDecimalNo((char*)m_szChars))
  if (TN_DECIMAL == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token float no?                                                    */
/*----------------------------------------------------------------------------*/
bool Token::bIsFloat(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  //if (TN_NAME == m_eToken && StrLib::bIsDecimalNo((char*)m_szChars))
  if (TN_FLOAT == m_eToken)
    return true;
  else
    return false;
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
/* is this token a < ?                                                        */
/*----------------------------------------------------------------------------*/
bool Token::bIsGreaterThan(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (bSame(">"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a > ?                                                        */
/*----------------------------------------------------------------------------*/
bool Token::bIsLessThan(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (bSame("<"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a logical operator ?                                         */
/*----------------------------------------------------------------------------*/
bool Token::bIsLogical(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  if (bSame("and"_Stz)  ||
      bSame("or"_Stz) ||
      bSame("not"_Stz) ||
      bSame("xor"_Stz))
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
/* is this token a \0 null?                                                   */
/*----------------------------------------------------------------------------*/
bool Token::bIsNull(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_NULL == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token an other?                                                    */
/*----------------------------------------------------------------------------*/
bool Token::bIsOther(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_OTHER == m_eToken)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a relational operator ?                                      */
/*----------------------------------------------------------------------------*/
bool Token::bIsRelational(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  if (bSame(">"_Stz)  ||
      bSame(">="_Stz) ||
      bSame("<"_Stz)  ||
      bSame("<="_Stz) ||
      bSame("=="_Stz) ||
      bSame("!="_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a semicol?                                                    */
/*----------------------------------------------------------------------------*/
bool Token::bIsSemicolon(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_OTHER == m_eToken && bSame(";"_Stz))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* is this token a string?                                                    */
/*----------------------------------------------------------------------------*/
bool Token::bIsString(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  if (TN_STRING == m_eToken)
    return true;
  else
    return false;
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
  
  m_szChars = nullptr;

  m_eToken  = TN_NONE;
  
  m_iLineNo = C_UNKNOWN;
  m_iColNo  = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* are the chars of this token the same as given arg?                         */
/*----------------------------------------------------------------------------*/
bool Token::bSame(Stz* szArg)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
  
  bool b = m_szChars->bSame(szArg);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* get which token                                                            */
/*----------------------------------------------------------------------------*/
i8 Token::i_Token(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);
    
  return m_i_Token;
}

/*----------------------------------------------------------------------------*/
/* get the i8 of the chars                                                    */
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
void Token::vSetNull(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  m_eToken = TN_NULL;
  *m_szChars->azChar() = '\0';
}

/*----------------------------------------------------------------------------*/
/* show token                                                                 */
/*----------------------------------------------------------------------------*/
void Token::vShow(void)
{
  _precondition_(_MAGIC_,CN_TOKEN,_FOOTER_);

  vTrace("[%lld] %s %s\n"_Stz
         ,m_i_Token
         ,aTN[m_eToken]
         ,m_szChars
         );
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
