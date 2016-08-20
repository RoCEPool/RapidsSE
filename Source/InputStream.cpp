/*============================================================================*/
/* InputStream.cpp - input stream handler                                     */
/*============================================================================*/

#include "Tasking.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* InputStream::operator new(size_t iSize)
{
  vFatal("fatal: InputStream class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void InputStream::operator delete(void* pAddress)
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
/* get type of literal just scanned                                           */
/*----------------------------------------------------------------------------*/
eLT InputStream::eLiteralType(void)
{
  _poolAddress_(this);
  
  return m_eLiteralType;
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
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
void InputStream::vNextLiteral(char* wWorkspace)
{
  _poolAddress_(this);
  
  bool b;
  
  *wWorkspace = '\0';
  
  while (true) {
    
    // get first/next char
    vGetChar();
    
    // end of input?
    if (bEndInput()) {
      m_eLiteralType = LT_NONE;
      *wWorkspace = '\0';
      return;
    }
    
    // skip whitespace
    vSkipWhitespace();
    
    // end of input?
    if (bEndInput()) {
      m_eLiteralType = LT_NONE;
      *wWorkspace = '\0';
      return;
    }

    // collect any comment
    if (bStartComment()) {
      if (!bParseComment(wWorkspace))
        break;
    }
    else {
      break;
    }
  }
  
  // collect integer literal
  if (bStartInteger()) {
    b = bParseInteger(wWorkspace);
    ASSERT(b);
    return;
  }
  
  // collect string literal
  if (bStartString()) {
    b = bParseString(wWorkspace);
    ASSERT(b);
    return;
  }
  
  // collect name literal
  b = bParseName(wWorkspace);
  ASSERT(b);
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
  
  printf("%s",m_wPrompt);
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void InputStream::vPrompt(char* wPrompt)
{
  _poolAddress_(this);
  
  m_wPrompt = wPrompt;
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
  m_wPrompt      = "docol>";
}

/*----------------------------------------------------------------------------*/
/* use text for scanning                                                      */
/*----------------------------------------------------------------------------*/
void InputStream::vInputText(void)
{
  _poolAddress_(this);
  
  vCloseFile();
  
  m_yInputText = nullptr;
}

/*----------------------------------------------------------------------------*/
/* use text for scanning                                                      */
/*----------------------------------------------------------------------------*/
void InputStream::vInputText(s& sScanText)
{
  _poolAddress_(this);
  
  vCloseFile();

  m_yInputText = sScanText.a();
}

/*----------------------------------------------------------------------------*/
/* use stdin as input file                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vSetStdin(void)
{
  _poolAddress_(this);
  
  vCloseFile();
  
  m_xInputFile.vSet(stdin,"stdin",C_OPEN_READ);
  
  m_bFromFile = true;
  m_bPrompt   = true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to parse block comment                                             */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseBlock(char* wWorkspace)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse a comment                                                 */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseComment(char* yWorkspace)
{
  char cOne;
  char cTwo;
  char *y = yWorkspace;
  
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
  *y++ = '/';
  return bParseName(y);
}

/*----------------------------------------------------------------------------*/
/* attempt to parse integer                                                   */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseInteger(char* yWorkspace)
{
  char *y = yWorkspace;
  
  i8 iNoDigits = 0;
  
  // collect initial digits
  while (bIsDigit()) {
    *y++ = m_cCurrChar;
    iNoDigits++;
    vGetChar();
  }
  
  // end of input?
  if (bEndInput()) {
    
    // push back the unconsumed char
    if (bEndInput())
      vUngetChar();

    // terminate an intlit
    *y = '\0';
    m_eLiteralType = LT_INTLIT;
    return true;
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {
    
    // skip the 'x'
    vGetChar();
    
    // collect the hex digits
    while (bIsHex()) {
      *y++ = m_cCurrChar;
      vGetChar();
    }
  }
  
  // end of collecting an integer?
  if ( bEndInput() || bIsWhite() || bStartString() ) {
    
    // push back the unconsumed char
    if (bEndInput())
      vUngetChar();

    // terminate an intlit
    *y = '\0';
    m_eLiteralType = LT_INTLIT;
    return true;
  }
  
  // its actually a name; complete parsing name
  bool b = bParseName(y);
  ASSERT(b);
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse line comment                                              */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseLine(char* wWorkspace)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse name                                                      */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseName(char* yWorkspace)
{
  char *y = yWorkspace;

  // collect the characters of the name
  while ( bMoreInput() && !bIsWhite() && !bStartString() ) {
    *y++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back the unconsumed char
  if (bEndInput())
    vUngetChar();
  
  // terminate a namelit
  *y = '\0';
  m_eLiteralType = LT_NAMELIT;
  return true;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse string                                                    */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseString(char* yWorkspace)
{
  char *y = yWorkspace;
  
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
        
    *y++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back a '\0'
  if (bEndInput())
    vUngetChar();
  
  // terminate a strlit
  *y = '\0';
  m_eLiteralType = LT_STRLIT;
  return true;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void InputStream::vGetChar(void)
{
  if (m_bFromFile) {
    
    // print prompt?
    if (m_bPrompt) {
      m_bPrompt = false;
      vPrintPrompt();
    }
    
    // get next char from input file
    m_cCurrChar = m_xInputFile.cGetChar();
    
    // do we need to print the prompt again next time?
    if (m_xInputFile.bIsStdin() && '\n' == m_cCurrChar)
      m_bPrompt = true;
  }
  else {
    // get next char from input file
    m_cCurrChar = *m_yInputText++;
  }
}

/*----------------------------------------------------------------------------*/
/* unget input character                                                      */
/*----------------------------------------------------------------------------*/
void InputStream::vUngetChar(void)
{
  if (m_bFromFile) {
    m_xInputFile.vUngetChar(m_cCurrChar);
  }
  else {
    m_yInputText--;
  }
  
  m_cCurrChar = 0;
}

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void InputStream::vSkipWhitespace(void)
{
  while (String::bIsWhite(m_cCurrChar))
    vGetChar();
}

/*----------------------------------------------------------------------------*/
/* test for end input                                                         */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndInput(void)
{
  if ( ! m_cCurrChar )
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
  if (m_cCurrChar == '/')
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
  if (m_cCurrChar == '"')
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of line                                                       */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndLine(void)
{
  if (m_cCurrChar == '\n')
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of string                                                     */
/*----------------------------------------------------------------------------*/
bool InputStream::bEndString(void)
{
  if (m_cCurrChar == '"')
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