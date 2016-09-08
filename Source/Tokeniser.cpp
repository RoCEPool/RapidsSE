/*============================================================================*/
/* Tokeniser.cpp -- Tokeniser : divide input stream into tokens               */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tokeniser::operator new(size_t)
{
  vFatal("fatal: Tokeniser class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::operator delete(void*)
{
  vFatal("fatal: Tokeniser class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tokeniser::Tokeniser(void)
: Object(CN_TOKENISER)
{
  m_pParser = nullptr;
  m_bMalloc = false;
  m_bNormal = false;
  
  Memory::vCheckInitialised(this,sizeof(Tokeniser));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void)
{
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPush(void)
{
  if (m_pParser)
    m_pParser->vPush();
  else
    m_pParser = new(m_bMalloc) Parser;
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bPop(void)
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
i8 Tokeniser::iDepth(void)
{
  i8 iParserDepth = 0;
  
  if (m_pParser)
    iParserDepth = m_pParser->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vLineMode(bool bLineMode)
{
  m_pParser->vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* set malloc flag                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vMalloc(bool bMalloc)
{
  m_bMalloc = bMalloc;
}

/*----------------------------------------------------------------------------*/
/* set normal token flag                                                      */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNormal(bool bNormal)
{
  m_bNormal = bNormal;
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPrompt(s& sPrompt)
{  
  m_pParser->vPrompt(sPrompt);
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bUseFile(s& sPath)
{
  bool bOpened = m_pParser->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseStdin(void)
{
  m_pParser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseString(s& sString)
{
  m_pParser->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* end use of source                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseEnd(void)
{
  m_pParser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextToken(char* wToken)
{
  eTK eToken;
  
  if (m_bNormal)
    eToken = eNextNormal(wToken);
  else
    eToken = eNextSpecial(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* reset token array                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vResetTokens(char* pToken[], i8 nTokens)
{
  for (i8 iToken = 0; iToken < nTokens; iToken++)
    *pToken[iToken] = '\0';
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 Tokeniser::iFindToken(const char* pLookFor, char* pToken[], i8 nTokens)
{
  for (i8 iToken = 0; iToken < nTokens; iToken++)
    if (String::bEqualTo(pLookFor,pToken[iToken],C_NO_CASE))
      return iToken;
  
  return -1;
}

/*----------------------------------------------------------------------------*/
/* count tokens in token array                                                */
/*----------------------------------------------------------------------------*/
i8 Tokeniser::iCountTokens(char* pToken[], i8 nTokens)
{
  for (i8 iToken = 0; iToken < nTokens; iToken++)
    if (String::bEqualTo("",pToken[iToken]))
      return iToken;
  
  ASSERT("Should never be reached");
  return -1;
}

/*----------------------------------------------------------------------------*/
/* dump token array                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDumpTokens(const char* pTitle, char* pToken[], i8 nTokens)
{
  vTrace("%s",pTitle);
  
  for (i8 iToken = 0; iToken < nTokens; iToken++) {
    
    if (! *pToken[iToken])
      break;
    
    vTrace("%s ", pToken[iToken]);
  }
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEOF(void)
{
  bool bEOF = m_pParser->bEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/* get LineMode?                                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bLineMode(void)
{
  bool bLineMode = m_pParser->bLineMode();
  
  return bLineMode;
}

/*----------------------------------------------------------------------------*/
/* get stdin?                                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStdin(void)
{
  bool bStdin = m_pParser->bStdin();
  
  return bStdin;
}



// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal token into given buffer                                  */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextNormal(char* wToken)
{
  *wToken = '\0';
  
  eTK eToken = eParseNormal(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next special token into given buffer                                 */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextSpecial(char* wToken)
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
eTK Tokeniser::eParseNormal(char* wToken)
{
  vGetNextStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *wToken = '\0';
    return TK_NONE;
  }
  
  // collect integer literal?
  if (bStartNormalInteger()) {
    eTK eLiteral = eParseNormalInteger(wToken);
    return eLiteral;
  }
  
  // collect string literal?
  if (bStartNormalString()) {
    eTK eToken = eParseNormalString(wToken);
    return eToken;
  }
  
  // collect name token?
  if (bStartNormalName()) {
    eTK eToken = eParseNormalName(wToken);
    return eToken;
  }
  
  eTK eToken = eParseNormalOther(wToken);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer literal                                               */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseNormalInteger(char* wToken)
{
  i8 iNoDigits = 0;
  char *z = wToken;
  
  // we have seen a digit
  ASSERT(bStartNormalInteger());
  
  // - start parsing an integer
  
  // collect initial digits
  while (bIsDigit()) {
    *z++ = m_cStrippedChar;
    iNoDigits++;
    vGetNextStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {
    
    // add the 'x'
    *z++ = m_cStrippedChar;
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cStrippedChar;
      vGetNextStripped();
    }
  }
  
  // push back the unconsumed character
  vUngetStrippedChar();
    
  // terminate the integer literal
  *z = '\0';
  
  return TK_INTEGER;
}

/*----------------------------------------------------------------------------*/
/* parse normal name token                                                    */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseNormalName(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartNormalName());
  
  // collect the characters of the name
  while ( String::bIsAlphaDigitUnder(m_cStrippedChar) ) {
    *z++ = m_cStrippedChar;
    vGetNextStripped();
  }
  
  // push back the unconsumed char
  vUngetStrippedChar();
  
  // terminate the name
  *z = '\0';
  
  return TK_NAME;
}

/*----------------------------------------------------------------------------*/
/* parse other token                                                          */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseNormalOther(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartNormalOther());
  
  // collect a single character
  *z++ = m_cStrippedChar;
  
  // terminate the other token
  *z = '\0';
  
  return TK_OTHER;
}

/*----------------------------------------------------------------------------*/
/* parse normal string literal                                                */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseNormalString(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartSpecialString());
  
  // skip the leading '"'
  vGetNextStripped();
  
  // collect the characters within the string
  while ( ! bEndNormalString() ) {
    
    // test for \n
    if ('\\' == m_cStrippedChar) {
      vGetNextStripped();
      if ('n' == m_cStrippedChar)
        m_cStrippedChar = '\n';
      else {
        vUngetStrippedChar();
        m_cStrippedChar = '\\';
      }
    }
    
    *z++ = m_cStrippedChar;
    vGetNextStripped();
  }
  
  // terminate the string literal
  *z = '\0';
  
  return TK_STRING;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseSpecial(char* wToken)
{
  vGetNextStripped();
  
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
eTK Tokeniser::eParseSpecialInteger(char* wToken)
{
  i8 iNoDigits = 0;
  char *z = wToken;
  
  // we have seen a digit
  ASSERT(bStartSpecialInteger());
  
  // - start parsing an integer
  
  // collect initial digits
  while (bIsDigit()) {
    *z++ = m_cStrippedChar;
    iNoDigits++;
    vGetNextStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {

    // add the 'x'
    *z++ = m_cStrippedChar;
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cStrippedChar;
      vGetNextStripped();
    }
  }
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if ( bIsWhite() || bStartSpecialString() ) {
    
    // push back the unconsumed char
    vUngetStrippedChar();
    
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
eTK Tokeniser::eParseSpecialName(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartSpecialName());
  
  // collect the characters of the name
  while ( m_cStrippedChar && !bIsWhite() && !bStartSpecialString() ) {
    *z++ = m_cStrippedChar;
    vGetNextStripped();
  }
  
  // push back the unconsumed char
  vUngetStrippedChar();
  
  // terminate a namelit
  *z = '\0';

  return TK_NAME;
}

/*----------------------------------------------------------------------------*/
/* parse special string                                                       */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eParseSpecialString(char* wToken)
{
  char *z = wToken;
  
  ASSERT(bStartSpecialString());
  
  // skip the leading '"'
  vGetNextStripped();
  
  // collect the chars in the string
  while ( ! bEndSpecialString() ) {
    
    // test for \n
    if ('\\' == m_cStrippedChar) {
      vGetNextStripped();
      if ('n' == m_cStrippedChar)
        m_cStrippedChar = '\n';
      else {
        vUngetStrippedChar();
        m_cStrippedChar = '\\';
      }
    }
    
    *z++ = m_cStrippedChar;
    vGetNextStripped();
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
void Tokeniser::vSkipWhitespace(void)
{
  while (bHaveWhitespace())
    vGetNextStripped();
}

/*----------------------------------------------------------------------------*/
/* have whitespace                                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bHaveWhitespace(void)
{
  // do we have a current parsed character, which is a white char?
  if (m_cStrippedChar) {
    if (String::bIsWhite(m_cStrippedChar))
      return true;
    else
      return false;
  }

  // - no current parsed character
  
  // a '\0' in line mode indicates end of line; not considered whitespace
  if (bLineMode())
    return false;
  
  // a '\0' at eof is the eof marker; not considered whitespace
  if (bEOF())
    return false;

  // a '\0' is otherwise whitespace
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalInteger(void)
{
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalName(void)
{
  if (String::bIsAlphaUnder(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalOther(void)
{
  if (! String::bIsAlphaDigitUnder(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalString(void)
{
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndNormalString(void)
{
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialInteger(void)
{
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special name                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialName(void)
{
  if (bStartSpecialInteger() || bStartSpecialString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of special string                                           */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialString(void)
{
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSpecialString(void)
{
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsDigit(void)
{
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsHex(void)
{
  if (String::bIsHex(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsX(void)
{
  if ('x' == m_cStrippedChar || 'X' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsWhite(void)
{
  if (String::bIsWhite(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void Tokeniser::vGetNextStripped(void)
{
  m_cStrippedChar = m_pParser->cGet();
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUngetStrippedChar(void)
{
  m_pParser->vUnget(m_cStrippedChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndInput(void)
{
  if ('\0' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Tokeniser::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/