/*============================================================================*/
/* InputStream.cpp - Input Stream Handler                                     */
/*============================================================================*/

#include "Tasking.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* InputStream::operator new(size_t)
{
  vFatal("fatal: InputStream class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void InputStream::operator delete(void*)
{
  vFatal("fatal: InputStream class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
InputStream::InputStream(void)
: ObjVal(sizeof(InputStream))
{
  _poolAddress_(this);

  m_iDepth = 0;

  vReset();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
InputStream::~InputStream(void)
{
  _poolAddress_(this);

  vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* close current file                                                         */
/*----------------------------------------------------------------------------*/
void InputStream::vCloseFile(void)
{
  _poolAddress_(this);

  if (m_xInputFile.bIsOpen())
    m_xInputFile.vClose();
  
  m_bFromFile = false;
}

/*----------------------------------------------------------------------------*/
/* get depth of this input                                                    */
/*----------------------------------------------------------------------------*/
i8 InputStream::iDepth(void)
{
  _poolAddress_(this);
  
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* set depth of this input                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vDepth(i8 iDepth)
{
  _poolAddress_(this);
  
  m_iDepth = iDepth;
}

/*----------------------------------------------------------------------------*/
/* seen EOF?                                                                  */
/*----------------------------------------------------------------------------*/
bool InputStream::bEOF(void)
{
  _poolAddress_(this);
  
  return m_bEOF;
}

/*----------------------------------------------------------------------------*/
/* seen EOL?                                                                  */
/*----------------------------------------------------------------------------*/
bool InputStream::bEOL(void)
{
  _poolAddress_(this);
  
  return m_bEOL;
}

/*----------------------------------------------------------------------------*/
/* set EOL                                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vEOL(bool bEOL)
{
  _poolAddress_(this);
  
  m_bEOL = bEOL;
}

/*----------------------------------------------------------------------------*/
/* set liine oreinted mode                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vLineOriented(bool bLineOriented)
{
  _poolAddress_(this);
  
  m_bLineOriented = bLineOriented;
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
eLT InputStream::eNextLiteral(char* wLiteral)
{
  _poolAddress_(this);
  
  bool b;
  
  *wLiteral = '\0';
  
  // - skip any whitespace or comments before next literal

  while (true) {
    
    // get first/next char
    vGetChar();
    
    // end of input?
    if (bEndInput()) {
      m_eLiteralType = LT_NONE;
      *wLiteral = '\0';
      return m_eLiteralType;
    }
    
    // skip whitespace
    vSkipWhitespace();
    
    // did we get an eol?
    if ('\n' == m_cCurrChar) {
      m_eLiteralType = LT_NONE;
      *wLiteral = '\0';
      return m_eLiteralType;
    }
    
    // end of input?
    if (bEndInput()) {
      m_eLiteralType = LT_NONE;
      *wLiteral = '\0';
      return m_eLiteralType;
    }

    // collect any comment
    if (bStartComment()) {
      if (! bParseComment(wLiteral))
        break;
    }
    else {
      break;
    }
  }

  // - collect the literal
  
  // collect integer literal
  if (bStartInteger()) {
    b = bParseInteger(wLiteral);
    ASSERT(b);
    return m_eLiteralType;
  }
  
  // collect string literal
  if (bStartString()) {
    b = bParseString(wLiteral);
    ASSERT(b);
    return m_eLiteralType;
  }
  
  // collect name literal
  b = bParseName(wLiteral);
  ASSERT(b);
  
  return m_eLiteralType;  
}

/*----------------------------------------------------------------------------*/
/* open file for scanning                                                     */
/*----------------------------------------------------------------------------*/
bool InputStream::bOpenFile(const char* aFilePath)
{
  _poolAddress_(this);

  vCloseFile();
  
  bool bOpened = m_xInputFile.bOpen(aFilePath,C_OPEN_READ);
  
  if (bOpened)
    m_bFromFile = true;
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* prompt for input                                                           */
/*----------------------------------------------------------------------------*/
void InputStream::vPrintPrompt(void)
{
  _poolAddress_(this);
  
  printf("%s",m_aPrompt);
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void InputStream::vPrompt(char* aPrompt)
{
  _poolAddress_(this);
  
  m_aPrompt = aPrompt;
}

/*----------------------------------------------------------------------------*/
/* reset this input                                                           */
/*----------------------------------------------------------------------------*/
void InputStream::vReset(void)
{
  _poolAddress_(this);

  //m_iDepth -- never gets reset
  m_bFromFile    = false;
  m_xInputFile.vReset();
  m_yInputText   = nullptr;
  m_cCurrChar    = 0;
  m_eLiteralType = LT_NONE;
  m_bPrompt      = false;
  m_aPrompt      = "docol> ";
  m_bLineOriented = false;
  m_bEOF         = false;
}

/*----------------------------------------------------------------------------*/
/* use stdin as input file                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vUseStdin(void)
{
  _poolAddress_(this);
  
  vCloseFile();
  
  m_xInputFile.vSet(stdin,"stdin",C_OPEN_READ);
  
  m_bFromFile = true;
  m_bPrompt   = true;
}

/*----------------------------------------------------------------------------*/
/* use given text for scanning                                                */
/*----------------------------------------------------------------------------*/
void InputStream::vUseText(s& sScanText)
{
  _poolAddress_(this);
  
  vCloseFile();
  
  m_yInputText = sScanText.a();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to parse a comment                                                 */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseComment(char* wLiteral)
{
  char cOne;
  char cTwo;
  char *z = wLiteral;
  
  // have seen a '/'

  // lets look at the next character
  vGetChar();
  
  // test if we are looking at a line comment
  if ('/' == m_cCurrChar) {

    // it is a line comment
    // consume till end of input or end of line
    while (bMoreInput() && !bEndLine())
      vGetChar();
    
    // push back the unconsumed char
    if (bEndInput())
      vUngetChar();
    
    return true;
  }

  // test if we are looking at a block comment
  if ('*' == m_cCurrChar) {
    
    // it is a block comment
    
    // get first char
    vGetChar();
    if (bEndInput())
      return true;
    cOne = m_cCurrChar;
    
    // get second char
    vGetChar();
    if (bEndInput())
      return true;
    cTwo = m_cCurrChar;

    // find either end-of-comment or end-of-input
    while (true) {
    
      // end-of-comment ?
      if ('*' == cOne && '/' == cTwo)
        return true;
      
      // advance to next pair
      cOne = cTwo;
      vGetChar();
      if (bEndInput())
        return true;
      cTwo = m_cCurrChar;
    }

  }
  
  // it's actually a name ( e.g. /ugh )
  *z++ = '/';
  return bParseName(z);
}

/*----------------------------------------------------------------------------*/
/* attempt to parse integer                                                   */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseInteger(char* wLiteral)
{
  char *z = wLiteral;
  
  i8 iNoDigits = 0;
  
  // collect initial digits
  while (bIsDigit()) {
    *z++ = m_cCurrChar;
    iNoDigits++;
    vGetChar();
  }
  
  // end of input?
  if (bEndInput()) {
    
    // push back the unconsumed char
    if (bEndInput())
      vUngetChar();

    // terminate an intlit
    *z = '\0';
    m_eLiteralType = LT_INTLIT;
    return true;
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {
    
    // skip the 'x'
    vGetChar();
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cCurrChar;
      vGetChar();
    }
  }
  
  // end of collecting an integer?
  if ( bEndInput() || bIsWhite() || bStartString() ) {
    
    // push back the unconsumed char
    if (bEndInput())
      vUngetChar();

    // terminate an intlit
    *z = '\0';
    m_eLiteralType = LT_INTLIT;
    return true;
  }
  
  // its actually a name; complete parsing name
  bool b = bParseName(z);
  ASSERT(b);
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse name                                                      */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseName(char* wLiteral)
{
  char *z = wLiteral;

  // collect the characters of the name
  while ( bMoreInput() && !bIsWhite() && !bStartString() ) {
    *z++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back the unconsumed char
  if (bEndInput()) //&& m_xInputFile.bIsStdin())
    vUngetChar();
  
  // terminate a namelit
  *z = '\0';
  m_eLiteralType = LT_NAMELIT;
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse string                                                    */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseString(char* wLiteral)
{
  char *z = wLiteral;
  
  // skip the leading '"'
  vGetChar();
  
  // collect the chars in the string
  while ( bMoreInput() && !bEndString() ) {
    
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
  
  // push back a '\0'
  if (bEndInput() && '\0' == m_cCurrChar)
    vUngetChar();
  
  // terminate a strlit
  *z = '\0';
  m_eLiteralType = LT_STRLIT;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void InputStream::vGetChar(void)
{
  // - from input text
  
  if (! m_bFromFile) {
    // get next char from input text
    m_cCurrChar = *m_yInputText++;
    
    if ('\n' == m_cCurrChar || '\0' == m_cCurrChar)
      m_bEOL = true;
    
    return;
  }

  // - from input file
  
  // print prompt?
  if (m_bPrompt) {
    m_bPrompt = false;
    vPrintPrompt();
  }
  
  // get next char from input file
  m_cCurrChar = m_xInputFile.cGetChar();
  
  if (0 == m_cCurrChar) {
    m_bEOF = true;
    return;
  }
  
  // end of line processing
  if ('\n' == m_cCurrChar) {

    m_bEOL = true;
    // do we need to print the prompt again next time?
    if (m_xInputFile.bIsStdin())
      m_bPrompt = true;
    
    //m_cCurrChar = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* unget input character                                                      */
/*----------------------------------------------------------------------------*/
void InputStream::vUngetChar(void)
{
  if (m_bFromFile)
    m_xInputFile.vUngetChar(m_cCurrChar);
  else
    m_yInputText--;
  
  m_cCurrChar = 0;
}

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void InputStream::vSkipWhitespace(void)
{

  for (;;) {
    if (m_cCurrChar == ' ' || m_cCurrChar == '\t') {
      vGetChar();
      continue;
    }

    if (!m_bLineOriented && m_cCurrChar == '\n') {
      vGetChar();
      continue;
    }
    
    break;
  }
}

/*----------------------------------------------------------------------------*/
/* test for end input                                                         */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndInput(void)
{
  if ( ! m_cCurrChar ) // || (m_bLineOriented && '\n' == m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for more input                                                        */
/*----------------------------------------------------------------------------*/
bool InputStream::bMoreInput(void)
{
  if (m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for possible start of comment                                         */
/*----------------------------------------------------------------------------*/
bool InputStream::bStartComment(void)
{
  if ('/' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of integer                                                  */
/*----------------------------------------------------------------------------*/
bool InputStream::bStartInteger(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of string                                                   */
/*----------------------------------------------------------------------------*/
bool InputStream::bStartString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of line                                                       */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndLine(void)
{
  if ('\n' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of string                                                     */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndString(void)
{
  if ('"' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsDigit(void)
{
  if (String::bIsDigit(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsHex(void)
{
  if (String::bIsHex(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsWhite(void)
{
  if (String::bIsWhite(m_cCurrChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a 'x' or 'X'                                                      */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsX(void)
{
  if ('x' == m_cCurrChar || 'X' == m_cCurrChar)
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/