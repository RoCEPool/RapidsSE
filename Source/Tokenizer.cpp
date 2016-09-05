/*============================================================================*/
/* Tokenizer.cpp -- Tokenizer : divide input stream into tokens               */
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
{
  m_pParser = nullptr;
  m_bMalloc = false;
  m_bNormal = false;
  
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
  if (m_pParser)
    m_pParser->vPush();
  else
    m_pParser = new(m_bMalloc) Parser;
    
  m_pParser->vPrompt(m_sPrompt);
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bPop(void)
{
  if (m_pParser) {
    if (0 == m_pParser->iDepth()) {
      delete m_pParser;
      m_pParser = nullptr;
      return true;
    }
    else {
      bool bPopped = m_pParser->bPop();
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
  
  if (m_pParser)
    iParserDepth = m_pParser->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Tokenizer::vLineMode(bool bLineMode)
{
  m_pParser->vLineMode(bLineMode);
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
  
  m_pParser->vPrompt(m_sPrompt);
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bUseFile(s& sPath)
{
  bool bOpened = m_pParser->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUseStdin(void)
{
  m_pParser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUseString(s& sString)
{
  m_pParser->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* end                                           
 */
/*----------------------------------------------------------------------------*/
void Tokenizer::vEndTokenizing(void)
{
  m_pParser->vEndParsing();
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eNextToken(char* wToken)
{
  eTK eToken;
  
  if (m_bNormal)
    eToken = eNextNormal(wToken);
  else
    eToken = eNextSpecial(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokenizer::bEOF(void)
{
  bool bEOF = m_pParser->bEOF();
  
  return bEOF;
}



// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal token into given buffer                                  */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eNextNormal(char* wToken)
{
  *wToken = '\0';
  
  eTK eToken = eParseNormal(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next special token into given buffer                                 */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eNextSpecial(char* wToken)
{
  *wToken = '\0';

  eTK eToken = eParseSpecial(wToken);
  
  return eToken;
}


/*----------------------------------------------------------------------------*/
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse normal                                                               */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseNormal(char* wToken)
{
  vSkipWhitespace();
  
  return TK_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer                                                       */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseNormalInteger(char* wToken)
{
  return TK_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal name                                                          */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseNormalName(char* wToken)
{
  return TK_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse normal string                                                        */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseNormalString(char* wToken)
{
  return TK_NONE;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseSpecial(char* wToken)
{
  vGetChar();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *wToken = '\0';
    return TK_NONE;
  }
  
  // collect integer literal?
  if (bStartSpecialInteger()) {
    eTK eLiteral = eParseSpecialInteger(wToken);
    return eLiteral;
  }
  
  // collect string literal?
  if (bStartSpecialString()) {
    eTK eToken = eParseSpecialString(wToken);
    return eToken;
  }
  
  // collect name token
  eTK eToken = eParseSpecialName(wToken);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse special integer                                                      */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseSpecialInteger(char* wToken)
{
  i8 iNoDigits = 0;
  char *z = wToken;
  
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
    return TK_INTEGER;
  }
  
  // its actually a name; complete parsing name
  eTK eToken = eParseSpecialName(z);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse special name                                                         */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseSpecialName(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartSpecialName());
  
  // collect the characters of the name
  while ( m_cCurrChar && !bIsWhite() && !bStartSpecialString() ) {
    *z++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back the unconsumed char
  vUngetChar();
  
  // terminate a namelit
  *z = '\0';

  return TK_NAME;
}

/*----------------------------------------------------------------------------*/
/* parse special string                                                       */
/*----------------------------------------------------------------------------*/
eTK Tokenizer::eParseSpecialString(char* wToken)
{
  char *z = wToken;
  
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

  return TK_STRING;
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
  m_cCurrChar = m_pParser->cGet();
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokenizer::vUngetChar(void)
{
  m_pParser->vUnget(m_cCurrChar);
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