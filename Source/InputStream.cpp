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
  
  if (bStdin())
    return;

  if (m_xInputFile.bIsOpen())
    m_xInputFile.vClose();
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
/* have we first char?                                                        */
/*----------------------------------------------------------------------------*/
bool InputStream::bFirstChar(void)
{
  _poolAddress_(this);
  
  return m_bFirstChar;
}

/*----------------------------------------------------------------------------*/
/* get a line                                                                 */
/*----------------------------------------------------------------------------*/
bool InputStream::bGetLine(s& sInputLine)
{
  _poolAddress_(this);
  
  char c;
  
  if (bStdin())
    vPrintPrompt();
  
  m_wInputLine = sInputLine.a();
  m_yInputCursor = sInputLine.a();
  
  do {
    
    // get next char from input file
    c = m_xInputFile.cGetChar();
    if ('\0' == c)
      break;
    
    *m_yInputCursor++ = c;
    
  } while ('\n' != c);
  
  // eof?
  if ('\0' == c && m_yInputCursor == m_wInputLine)
    return false;
  
  *m_yInputCursor++ = '\0';
  
  m_yInputCursor = m_wInputLine;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set line oreinted mode                                                     */
/*----------------------------------------------------------------------------*/
void InputStream::vLineOriented(bool bLineOriented)
{
  _poolAddress_(this);
  
  m_bLineOriented = bLineOriented;
}

/*----------------------------------------------------------------------------*/
/* get next line from input source                                            */
/*----------------------------------------------------------------------------*/
void InputStream::vNextLine(void)
{
  char c;
  bool bEOFdetected = false;
  
  m_yInputCursor = m_wInputLine;
  
  // end of apriori input?
  if (! m_bFromFile) {
    m_bEndInput= true;
    return;
  }
  
  // loop till we get a line
  for (;;) {
    
    // prompt the user
    if (m_xInputFile.bIsStdin())
      vPrintPrompt();
    
    // collect one line of input line
    for (;;) {
      
      // get next char from input file
      c = m_xInputFile.cGetChar();
      *m_yInputCursor++ = c;
      
      // test for eof
      if ('\0' == c) {
        bEOFdetected = true;
        break;
      }
      
      // test for eol
      if ('\n' == c) {
        *m_yInputCursor++ = '\0';
        break;
      }
    }
    
    // set the input cursor back to the beginning of input line
    m_yInputCursor = m_wInputLine;
    //vTrace("LINE %s",m_yInputCursor);
    
    // did we get a line?
    if ('\0' != *m_yInputCursor) {
      m_yInputCursor = m_wInputLine;
      m_cCurrChar  = *m_yInputCursor++;
      return;
    }
    
    // did user hit return without typing something in?
    if (m_xInputFile.bIsStdin())
      continue;
    
    // have we reached the end of the input file
    if (bEOFdetected)
      break;
  }
  
  m_bEndInput = true;
}

/*----------------------------------------------------------------------------*/
/* parse next literal into given buffer                                       */
/*----------------------------------------------------------------------------*/
eLT InputStream::eNextLiteral(char* wLiteral)
{
  _poolAddress_(this);
  
  bool b;
  
  m_eLiteralType = LT_NONE;
  *wLiteral = '\0';
  
  // - skip whitespace and comments
  
  while (true) {
    
    if (m_bFirstChar)
      m_bFirstChar = false;
    else
      vGetChar();
    
    // skip whitespace
    vSkipWhitespace();
    
    // end of input?
    if (m_bEndInput) {
      m_eLiteralType = LT_NONE;
      *wLiteral = '\0';
      //vTrace("NONE>><<\n");
      return m_eLiteralType;
    }
    
    // collect any comment
    if (bStartComment() && bParseComment(wLiteral)) {
      
      // did we parse a name like /ugh?
      if (LT_NONE != m_eLiteralType) {
        //vTrace("NAME >>%s<<\n",wLiteral);
        return m_eLiteralType;
      }
      
      // parsed a comment
      continue;
    }

    // we have a literal
    break;
  }
  
  // - collect the literal
  
  // collect integer literal?
  if (bStartInteger()) {
    b = bParseInteger(wLiteral);
    ASSERT(b);
    //vTrace("INT >>%s<<\n",wLiteral);
    return m_eLiteralType;
  }
  
  // collect string literal?
  if (bStartString()) {
    b = bParseString(wLiteral);
    ASSERT(b);
    //vTrace("STR >>%s<<\n",wLiteral);
    return m_eLiteralType;
  }
  
  // collect name literal
  b = bParseName(wLiteral);
  ASSERT(b);
  
  //vTrace("NAME >>%s<<\n",wLiteral);
  return m_eLiteralType;
}

/*----------------------------------------------------------------------------*/
/* open file for scanning                                                     */
/*----------------------------------------------------------------------------*/
bool InputStream::bOpenFile(s& sInputLine, const char* aFilePath)
{
  _poolAddress_(this);

  vCloseFile();
  
  bool bOpened = m_xInputFile.bOpen(aFilePath,C_OPEN_READ);
  if (bOpened)
    m_bFromFile = true;
 
  m_wInputLine  = sInputLine.a();
  vNextLine();
  m_bFirstChar  = true;
  
  return bOpened;
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
  m_wInputLine   = nullptr;
  m_yInputCursor   = nullptr;
  m_cCurrChar    = 0;
  m_eLiteralType = LT_NONE;
  m_aPrompt      = "docol> ";
  m_bEndInput    = false;
  m_bFirstChar   = false;
  
  m_bPrompt       = false;
  m_bLineOriented = false;
}

/*----------------------------------------------------------------------------*/
/* getting chars from stdin?                                                  */
/*----------------------------------------------------------------------------*/
bool InputStream::bStdin(void)
{
  _poolAddress_(this);
  
  bool b = m_xInputFile.bIsStdin();
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
void InputStream::vUseFile(s& sInputText)
{
  _poolAddress_(this);
  
  m_bFromFile  = true;
  m_wInputLine = nullptr;
  m_yInputCursor = sInputText.a();
  m_bEndInput  = false;
  
  m_bPrompt     = false;
  
  m_cCurrChar   = *m_yInputCursor++;
}

/*----------------------------------------------------------------------------*/
/* use stdin as input                                                         */
/*----------------------------------------------------------------------------*/
void InputStream::vUseStdin(s& sInputLine)
{
  _poolAddress_(this);
  
  if ( ! bStdin() ) {
    vCloseFile();
  
    // special case where stdin isnt opened,
    // so just set file as open opened for read
  
    m_xInputFile.vSet(stdin,"stdin",C_OPEN_READ);
  }
  
  // setup for reading from file
  m_bFromFile   = true;
  m_wInputLine  = sInputLine.a();
  m_yInputCursor  = m_wInputLine;
  *m_wInputLine = '\0';
  m_bEndInput   = false;
  m_bFirstChar   = false;
  
  m_bPrompt     = true;

  *m_yInputCursor = '\0';
}

/*----------------------------------------------------------------------------*/
/* use given text as input                                                    */
/*----------------------------------------------------------------------------*/
void InputStream::vUseText(s& sInputText)
{
  _poolAddress_(this);
  
//  if ( ! bStdin() )
//    vCloseFile();
  
  m_bFromFile  = false;
  m_wInputLine = nullptr;
  m_yInputCursor = sInputText.a();
  m_bEndInput  = false;
  
  m_bPrompt     = false;
  
  m_cCurrChar   = *m_yInputCursor++;
  m_bFirstChar  = true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void InputStream::vGetChar(void)
{
  m_cCurrChar = *m_yInputCursor++;
  
  if ('\0' != m_cCurrChar)
    return;

  // end of apriori text?
  if (! m_bFromFile) {
    ASSERT('\0' == m_cCurrChar)
    m_bEndInput = true;
    return;
  }
  
  if (m_bLineOriented) {
    ASSERT('\0' == m_cCurrChar)
    m_bEndInput = true;
    return;
  }
  
  vNextLine();
  
  if ('\0' != m_cCurrChar)
    return;
  
  ASSERT('\0' == m_cCurrChar)
  ASSERT( ! m_xInputFile.bIsStdin() );
  m_bEndInput = true;
}

/*----------------------------------------------------------------------------*/
/* unget input character                                                      */
/*----------------------------------------------------------------------------*/
void InputStream::vUngetChar(void)
{
  if (m_bFromFile)
    m_xInputFile.vUngetChar(m_cCurrChar);
  else
    m_yInputCursor--;
  
  m_cCurrChar = 0;
}

/*----------------------------------------------------------------------------*/
/* attempt to parse a comment                                                 */
/*----------------------------------------------------------------------------*/
bool InputStream::bParseComment(char* wLiteral)
{
  char cOne;
  char cTwo;
  char *z = wLiteral;
  
  // have seen a '/'
  ASSERT(bStartComment());
  
  // lets look at the next character
  vGetChar();
  
  // test for line comment, i.e. //...\n
  if ('/' == m_cCurrChar) {
    
    // consume till end of line
    while ('\n' != m_cCurrChar)
      vGetChar();
    
    vUngetChar();
    
    return true;
  }
  
  // test for block comment, i.e. /*...*/
  if ('*' == m_cCurrChar) {
    
    // get first char
    vGetChar();
    if (m_bEndInput)
      return true;
    cOne = m_cCurrChar;
    
    // get second char
    vGetChar();
    if (m_bEndInput)
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
      if (m_bEndInput)
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
  i8 iNoDigits = 0;
  char *z = wLiteral;

  // we have seen a digit
  ASSERT(bStartInteger());
  
  // - start parsing an integer
  
  // collect initial digits
  while (bIsDigit()) {
    *z++ = m_cCurrChar;
    iNoDigits++;
    vGetChar();
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

  // - handle the case where we have a name like 1_ugh

  // just a number?
  if ( bIsNull() || bIsWhite() || bStartComment() || bStartString() ) {

    // push back the unconsumed char
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
  
  ASSERT(bStartName());
  
  // collect the characters of the name
  while ( !bIsNull() && !bIsWhite() && !bStartString() ) {
    *z++ = m_cCurrChar;
    vGetChar();
  }
  
  // push back the unconsumed char
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
  while ( ! bEndString() ) {
    
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
  if ('\0' == m_cCurrChar)
    vUngetChar();
  
  // terminate the strlit
  *z = '\0';
  m_eLiteralType = LT_STRLIT;
  return true;
}

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void InputStream::vSkipWhitespace(void)
{
  while ( ! m_bEndInput && String::bIsWhite(m_cCurrChar) )
    vGetChar();
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
/* test for start of name                                                     */
/*----------------------------------------------------------------------------*/
bool InputStream::bStartName(void)
{
  if (bStartComment() || bStartInteger() || bStartString())
    return false;
  
  return true;
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
  if ('"' == m_cCurrChar || '\0' == m_cCurrChar)
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
/* test for '\0'                                                              */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsNull(void)
{
  if ('\0' == m_cCurrChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a '/'                                                             */
/*----------------------------------------------------------------------------*/
bool InputStream::bIsSlash(void)
{
  if ('/' == m_cCurrChar)
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

/*----------------------------------------------------------------------------*/
/* prompt for input                                                           */
/*----------------------------------------------------------------------------*/
void InputStream::vPrintPrompt(void)
{
  _poolAddress_(this);
  
  printf("%s",m_aPrompt);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/