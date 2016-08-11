/*============================================================================*/
/* Input.cpp - input handler                                                  */
/*============================================================================*/

#include "Tasking.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Input::operator new(size_t iSize)
{
  vFatal("fatal: Input class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Input::operator delete(void* pAddress)
{
  vFatal("fatal: Input class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input::Input(void)
: ObjVal(sizeof(Input))
{
  _poolAddress_(this);

  m_iDepth = 0;

  vReset();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input::~Input(void)
{
  _poolAddress_(this);

  vCloseFile();
}

/*----------------------------------------------------------------------------*/
/* get depth of this input                                                    */
/*----------------------------------------------------------------------------*/
i8 Input::iDepth(void)
{
  _poolAddress_(this);
  
  return m_iDepth;
}

/*----------------------------------------------------------------------------*/
/* set depth of this input                                                    */
/*----------------------------------------------------------------------------*/
void Input::vDepth(i8 iDepth)
{
  _poolAddress_(this);
  
  m_iDepth = iDepth;
}

/*----------------------------------------------------------------------------*/
/* get type of literal just scanned                                           */
/*----------------------------------------------------------------------------*/
eLT Input::eLiteralType(void)
{
  _poolAddress_(this);
  
  return m_eLiteralType;
}

/*----------------------------------------------------------------------------*/
/* close current file                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCloseFile(void)
{
  _poolAddress_(this);

  if (m_xInputFile.bIsOpen())
    m_xInputFile.vClose();
  
  m_bFromFile = false;
}

/*----------------------------------------------------------------------------*/
/* discard remainder of line                                                  */
/*----------------------------------------------------------------------------*/
void Input::vDiscardLine(void)
{
  _poolAddress_(this);

  do {
    vGetChar();
  } while ('\n' != m_cChar);
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
void Input::vNextLiteral(char* wWorkspace)
{
  _poolAddress_(this);
  
  bool b;
  
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
bool Input::bOpenFile(const char* aFilePath)
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
void Input::vPrompt(void)
{
  _poolAddress_(this);
  
  printf("docol> ");
}

/*----------------------------------------------------------------------------*/
/* reset this input                                                           */
/*----------------------------------------------------------------------------*/
void Input::vReset(void)
{
  _poolAddress_(this);

  m_bFromFile    = false;
  m_yScanText    = nullptr;
  m_eLiteralType = LT_NONE;
  m_cChar        = 0;
  m_bPrompt      = false;
  
  m_xInputFile.vReset();
}

/*----------------------------------------------------------------------------*/
/* use text for scanning                                                      */
/*----------------------------------------------------------------------------*/
void Input::vScanText(void)
{
  _poolAddress_(this);
  
  vCloseFile();
  
  m_yScanText = nullptr;
}

/*----------------------------------------------------------------------------*/
/* use text for scanning                                                      */
/*----------------------------------------------------------------------------*/
void Input::vScanText(s& sScanText)
{
  _poolAddress_(this);
  
  vCloseFile();

  m_yScanText = sScanText.a();
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Input::vSetStdin(void)
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
bool Input::bParseBlock(char* wWorkspace)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse integer                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bParseInteger(char* yWorkspace)
{
  char *y = yWorkspace;
  
  i8 iNoDigits = 0;
  
  // collect initial digits
  while (bIsDigit()) {
    *y++ = m_cChar;
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
      *y++ = m_cChar;
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
bool Input::bParseLine(char* wWorkspace)
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse name                                                      */
/*----------------------------------------------------------------------------*/
bool Input::bParseName(char* yWorkspace)
{
  char *y = yWorkspace;

  // collect the characters of the name
  while ( bMoreInput() && !bIsWhite() && !bStartString() ) {
    *y++ = m_cChar;
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
bool Input::bParseString(char* yWorkspace)
{
  char *y = yWorkspace;
  
  // skip the leading '"'
  vGetChar();
  
  // collect the chars in the string
  while ( bMoreInput() && !bEndString() ) {
    *y++ = m_cChar;
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
void Input::vGetChar(void)
{
  if (m_bFromFile) {
    
    if (m_bPrompt) {
      m_bPrompt = false;
      vPrompt();
    }
    
    m_cChar = m_xInputFile.cGetChar();
    
    // print the prompt again next time?
    if (m_xInputFile.bIsStdin() && '\n' == m_cChar)
      m_bPrompt = true;
  }
  else {
    m_cChar = *m_yScanText++;
  }
}

/*----------------------------------------------------------------------------*/
/* unget input character                                                      */
/*----------------------------------------------------------------------------*/
void Input::vUngetChar(void)
{
  if (m_bFromFile) {
    m_xInputFile.vUngetChar(m_cChar);
  }
  else {
    m_yScanText--;
  }
  
  m_cChar = 0;
}

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void Input::vSkipWhitespace(void)
{
  while (String::bIsWhite(m_cChar))
    vGetChar();
}

/*----------------------------------------------------------------------------*/
/* test for end input                                                         */
/*----------------------------------------------------------------------------*/
bool Input::bEndInput(void)
{
  if ( ! m_cChar )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for more input                                                        */
/*----------------------------------------------------------------------------*/
bool Input::bMoreInput(void)
{
  if (m_cChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of integer                                                  */
/*----------------------------------------------------------------------------*/
bool Input::bStartInteger(void)
{
  if (String::bIsDigit(m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of string                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bStartString(void)
{
  if (m_cChar == '"')
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of string                                                     */
/*----------------------------------------------------------------------------*/
bool Input::bEndString(void)
{
  if (m_cChar == '"')
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bIsDigit(void)
{
  if (String::bIsDigit(m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bIsHex(void)
{
  if (String::bIsHex(m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bIsWhite(void)
{
  if (String::bIsWhite(m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a 'x' or 'X'                                                      */
/*----------------------------------------------------------------------------*/
bool Input::bIsX(void)
{
  if ('x' == m_cChar || 'X' == m_cChar)
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/