/*============================================================================*/
/* InputStream.cpp - Input Stream Handler                                     */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tokenizer::operator new(size_t)
{
  vFatal("fatal: Tokenizer class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tokenizer::operator delete(void*)
{
  vFatal("fatal: Tokenizer class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tokenizer::Tokenizer(void)
: ObjVal(sizeof(Tokenizer))
{
  m_pCharred = nullptr;
  m_bMalloc  = false;
  m_bNormal  = false;
  
  //m_sPrompt
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokenizer::~Tokenizer(void)
{
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokenizer::vPush(void)
{
  if (m_pCharred)
    m_pCharred->vPush();
  else
    m_pCharred = new(m_bMalloc) Parser;
    
  m_pCharred->vPrompt(m_sPrompt);
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bPop(void)
{
  if (m_pCharred) {
    if (0 == m_pCharred->iDepth()) {
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
i8 Tokenizer::iDepth(void)
{
  i8 iParserDepth = 0;
  
  if (m_pCharred)
    iParserDepth = m_pCharred->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Tokenizer::vLineMode(bool bLineMode)
{
  m_pCharred->vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* set malloc flag                                                            */
/*----------------------------------------------------------------------------*/
void Tokenizer::vMalloc(bool bMalloc)
{
  m_bMalloc = bMalloc;
}

/*----------------------------------------------------------------------------*/
/* set normal token flag                                                      */
/*----------------------------------------------------------------------------*/
void Tokenizer::vNormal(bool bNormal)
{
  m_bNormal = bNormal;
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Tokenizer::vPrompt(const char* pPrompt)
{
  m_sPrompt = pPrompt;
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bUseFile(s& sPath)
{
  bool bOpened = m_pCharred->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUseStdin(void)
{
  m_pCharred->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUseString(s& sString)
{
  m_pCharred->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* begin                                                                      */
/*----------------------------------------------------------------------------*/
void Tokenizer::vBegin(void)
{
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eNextLiteral(char* wLiteral)
{
  eLT eLiteral;
  
  if (m_bNormal)
    eLiteral = eNextNormal(wLiteral);
  else
    eLiteral = eNextSpecial(wLiteral);
  
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bEOF(void)
{
  bool bEOF = m_pCharred->bEOF();
  
  return bEOF;
}



// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal literal into given buffer                                */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eNextNormal(char* wLiteral)
{
  *wLiteral = '\0';
  
  eLT eLiteral = eParseNormal(wLiteral);
  
  return eLiteral;
}

/*----------------------------------------------------------------------------*/
/* parse next special literal into given buffer                               */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eNextSpecial(char* wLiteral)
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
eLT Tokenizer::eParseNormal(char* wLiteral)
{
  vSkipWhitespace();
  
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer                                                       */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eParseNormalInteger(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal name                                                          */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eParseNormalName(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal string                                                        */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eParseNormalString(char* wLiteral)
{
  return LT_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eLT Tokenizer::eParseSpecial(char* wLiteral)
{
  vGetChar();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
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
eLT Tokenizer::eParseSpecialInteger(char* wLiteral)
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
eLT Tokenizer::eParseSpecialName(char* wLiteral)
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
eLT Tokenizer::eParseSpecialString(char* wLiteral)
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
void Tokenizer::vSkipWhitespace(void)
{
  while ( m_cCurrChar && String::bIsWhite(m_cCurrChar) )
    vGetChar();
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartNormalInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartNormalName(void)
{
  if (bStartNormalInteger() || bStartNormalString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartNormalString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bEndNormalString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartSpecialInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special name                                             */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartSpecialName(void)
{
  if (bStartSpecialInteger() || bStartSpecialString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of special string                                           */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bStartSpecialString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bEndSpecialString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bIsDigit(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bIsHex(void)
{
  if (String::bIsHex(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bIsX(void)
{
  if ('x' == m_cCurrChar || 'X' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bIsWhite(void)
{
  if (String::bIsWhite(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void Tokenizer::vGetChar(void)
{
  m_cCurrChar = m_pCharred->cGet();
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUngetChar(void)
{
  m_pCharred->vUnget(m_cCurrChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bEndInput(void)
{
  if ('\0' == m_cCurrChar)
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/