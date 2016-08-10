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
void Input::vNextLiteral(char* aWorkspace)
{
  _poolAddress_(this);
  
  // reset
  m_eLiteralType = LT_NONE;
  *aWorkspace = '\0';
  char *a = aWorkspace;
  
  // get first/next char
  vGetChar();
  if ( ! m_cChar )
    return;
  
  // skip whitespace
  while (String::bIsWhite(m_cChar))
    vGetChar();
  
  if ( ! m_cChar)
    return;

  // collect int literal
  if (String::bIsDigit(m_cChar)) {
    
    i8 iNoDigits = 0;
    
    for ( ; m_cChar && String::bIsDigit(m_cChar); vGetChar()) {
      *a++ = m_cChar;
      iNoDigits++;
    }
    
    if ( ! m_cChar ) {
      *a = '\0';
      m_eLiteralType = LT_NAMELIT;
      return;
    }
    
    if (('x' == m_cChar || 'X' == m_cChar) && 1 == iNoDigits) {
      vGetChar();
      *a++ = m_cChar;
      for ( ; m_cChar && String::bIsHex(m_cChar); vGetChar())
        *a++ = m_cChar;
    }
    
    if ( ! m_cChar || String::bIsWhite(m_cChar) || '"' == m_cChar) {
      *a = '\0';
      m_eLiteralType = LT_INTLIT;
      if ('"' == m_cChar)
        vUngetChar();
      return;
    }
    
    for ( ; m_cChar && !String::bIsWhite(m_cChar) && '"' != m_cChar; vGetChar())
      *a++ = m_cChar;
    *a = '\0';
    m_eLiteralType = LT_NAMELIT;
    if ('"' == m_cChar)
      vUngetChar();
    return;
  }
  
  // collect str literal
  if (m_cChar == '"') {
    
    vGetChar();
    for ( ; m_cChar && '"' != m_cChar; vGetChar())
      *a++ = m_cChar;
    
    *a = '\0';
    m_eLiteralType = LT_STRLIT;
    return;
  }
  
  // collect name literal
  for ( ; m_cChar && !String::bIsWhite(m_cChar) && '"' != m_cChar; vGetChar())
    *a++ = m_cChar;
  *a = '\0';
  m_eLiteralType = LT_NAMELIT;
  
  if (! m_cChar || '"' == m_cChar)
    vUngetChar();
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/