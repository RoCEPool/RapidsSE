/*============================================================================*/
/* InputStream.cpp - Input Stream Handler                                     */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* InputParser::operator new(size_t)
{
  vFatal("fatal: InputParser class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::operator delete(void*)
{
  vFatal("fatal: InputParser class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InputParser::InputParser(void)
: ObjVal(sizeof(InputParser))
{
  m_pCharred = nullptr;
  m_bMalloc  = false;
  m_bNormal  = false;
  
  //m_sPrompt
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InputParser::~InputParser(void)
{
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void InputParser::vPush(void)
{
  if (m_pCharred)
    m_pCharred->vPush();
  else
    m_pCharred = new(m_bMalloc) Charred;
    
  m_pCharred->vPrompt(m_sPrompt);
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool InputParser::bPop(void)
{
  if (m_pCharred) {
    if (1 == m_pCharred->iDepth()) {
      delete m_pCharred;
      m_pCharred = nullptr;
      return true;
    }
    else {
      bool bPopped = m_pCharred->bPop();
      return bPopped;
    }
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get depth of parser                                                        */
/*----------------------------------------------------------------------------*/
i8 InputParser::iDepth(void)
{
  i8 iParserDepth = 0;
  
  if (m_pCharred)
    iParserDepth = m_pCharred->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* set malloc flag                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::vMalloc(bool bMalloc)
{
  m_bMalloc = bMalloc;
}

/*----------------------------------------------------------------------------*/
/* set normal token flag                                                      */
/*----------------------------------------------------------------------------*/
void InputParser::vNormal(bool bNormal)
{
  m_bNormal = bNormal;
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void InputParser::vPrompt(const char* pPrompt)
{
  m_sPrompt = pPrompt;
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool InputParser::bUseFile(s& sPath)
{
  bool bOpened = m_pCharred->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void InputParser::vUseStdin(void)
{
  m_pCharred->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void InputParser::vUseString(s& sString)
{
  m_pCharred->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* begin                                                                      */
/*----------------------------------------------------------------------------*/
void InputParser::vBegin(void)
{
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextLiteral(char* wLiteral)
{
  eLT eLiteral;
  
  if (m_bNormal)
    eLiteral = eNextNormal(wLiteral);
  else
    eLiteral = eNextSpecial(wLiteral);
  
  return eLiteral;
}


// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal literal into given buffer                                */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextNormal(char* wLiteral)
{
  *wLiteral = '\0';
  
  eLT eLiteral = eParseNormal(wLiteral);
  
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* parse next special literal into given buffer                               */
/*----------------------------------------------------------------------------*/
eLT InputParser::eNextSpecial(char* wLiteral)
{
  *wLiteral = '\0';

  eLT eLiteral = eParseSpecial(wLiteral);
  
  return eLiteral;
}


/*----------------------------------------------------------------------------*/
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse normal                                                               */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormal(char* wLiteral)
{
  vSkipWhitespace();
  
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer                                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalInteger(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal name                                                          */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalName(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal string                                                        */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseNormalString(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecial(char* wLiteral)
{
  vGetChar();
  
  vSkipWhitespace();
  
  // end of input?
  if (! bEndInput()) {
    *wLiteral = '\0';
    return LT_NONE;
  }
  
  // collect integer literal?
  if (bStartSpecialInteger()) {
    eLT eLiteral = eParseSpecialInteger(wLiteral);
    return eLiteral;
  }
  
  // collect string literal?
  if (bStartSpecialString()) {
    eLT eLiteral = eParseSpecialString(wLiteral);
    return eLiteral;
  }
  
  // collect name literal
  eLT eLiteral = eParseSpecialName(wLiteral);
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* parse special integer                                                      */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialInteger(char* wLiteral)
{
  i8 iNoDigits = 0;
  char *z = wLiteral;
  
  // we have seen a digit
  ASSERT(bStartSpecialInteger());
  
  // - start parsing an integer
  
  // collect initial digits
  while (bIsDigit()) {
    *z++ = m_cCurrChar;
    iNoDigits++;
    vGetChar();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {

    // add the 'x'
    *z++ = m_cCurrChar;
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cCurrChar;
      vGetChar();
    }
  }
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if ( bIsWhite() || bStartSpecialString() ) {
    
    // push back the unconsumed char
    vUngetChar();
    
    // terminate an intlit
    *z = '\0';
    return LT_INTLIT;
  }
  
  // its actually a name; complete parsing name
  eLT eLiteral = eParseSpecialName(z);
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* parse special name                                                         */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialName(char* wLiteral)
{
  char *z = wLiteral;
  
  ASSERT(bStartSpecialName());
  
  // collect the characters of the name
  while ( !bIsWhite() && !bStartSpecialString() ) {
    *z++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back the unconsumed char
  vUngetChar();
  
  // terminate a namelit
  *z = '\0';

  return LT_NAMELIT;
}

/*----------------------------------------------------------------------------*/
/* parse special string                                                       */
/*----------------------------------------------------------------------------*/
eLT InputParser::eParseSpecialString(char* wLiteral)
{
  char *z = wLiteral;
  
  ASSERT(bStartSpecialString());
  
  // skip the leading '"'
  vGetChar();
  
  // collect the chars in the string
  while ( ! bEndSpecialString() ) {
    
    // test for \n
    if ('\\' == m_cCurrChar) {
      vGetChar();
      if ('n' == m_cCurrChar)
        m_cCurrChar = '\n';
      else {
        vUngetChar();
        m_cCurrChar = '\\';
      }
    }
    
    *z++ = m_cCurrChar;
    vGetChar();
  }
  
  // terminate the strlit
  *z = '\0';

  return LT_STRLIT;
}


/*----------------------------------------------------------------------------*/
/*-- character processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void InputParser::vSkipWhitespace(void)
{
  while ( m_cCurrChar && String::bIsWhite(m_cCurrChar) )
    vGetChar();
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalName(void)
{
  if (bStartNormalInteger() || bStartNormalString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartNormalString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool InputParser::bEndNormalString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special name                                             */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialName(void)
{
  if (bStartSpecialInteger() || bStartSpecialString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of special string                                           */
/*----------------------------------------------------------------------------*/
bool InputParser::bStartSpecialString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool InputParser::bEndSpecialString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool InputParser::bIsDigit(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool InputParser::bIsHex(void)
{
  if (String::bIsHex(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool InputParser::bIsX(void)
{
  if ('x' == m_cCurrChar || 'X' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool InputParser::bIsWhite(void)
{
  if (String::bIsWhite(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void InputParser::vGetChar(void)
{
  m_cCurrChar = m_pCharred->cGet();
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void InputParser::vUngetChar(void)
{
  m_pCharred->vUnget(m_cCurrChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool InputParser::bEndInput(void)
{
  if ('\0' == m_cCurrChar)
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/