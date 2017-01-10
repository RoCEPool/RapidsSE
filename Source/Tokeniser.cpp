/*============================================================================*/
/* Tokeniser.cpp -- Tokeniser : divide input stream into tokens               */
/*============================================================================*/

#include "Tasks.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tokeniser::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Tokeniser class cannot be new'd");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::operator delete(void*)
{
  vFatal("fatal: Tokeniser class cannot be deleted");
  
  RASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tokeniser::Tokeniser(void)
: Object(CN_TOKENISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser = nullptr;
  m_bMalloc = false;
  m_bNormal = false;
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
}

Parser* m_pParser; // parser
bool    m_bMalloc; // malloc or pool heap?
bool    m_bNormal; // normal tokens?
char    m_cStrippedChar; // character from parser, stripped of comments

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TOKENISER);
  ASSERT(pObject->eClass() == CN_TOKENISER);
  
  Tokeniser* pTokeniser = reinterpret_cast<Tokeniser*>(pObject);
  
  vTrace("%sTokeniser %p:\n", pTab, pTokeniser);
  vTrace("%s  parser %p\n", pTab, pTokeniser->m_pParser);
  vTrace("%s  %s %p\n\n", pTab, pTokeniser->m_bMalloc == true ? "malloc" : "pool");
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPush(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* set malloc flag                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vMalloc(bool bMalloc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_bMalloc = bMalloc;
}

/*----------------------------------------------------------------------------*/
/* set normal token flag                                                      */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNormal(bool bNormal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_bNormal = bNormal;
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPrompt(s& sPrompt)
{  
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vPrompt(sPrompt);
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bUseFile(s& sPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bOpened = m_pParser->bUseFile(sPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseString(s& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* end use of source                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextToken(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  for (i8 iToken = 0; iToken < nTokens; iToken++)
    *pToken[iToken] = '\0';
}

/*----------------------------------------------------------------------------*/
/* find token in token array                                                  */
/*----------------------------------------------------------------------------*/
i8 Tokeniser::iFindToken(const char* pLookFor, char* pToken[], i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bEOF = m_pParser->bEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/* get LineMode?                                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bLineMode = m_pParser->bLineMode();
  
  return bLineMode;
}

/*----------------------------------------------------------------------------*/
/* get stdin?                                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bStdin = m_pParser->bStdin();
  
  return bStdin;
}



// private: ////////////////////////////////////////////////////////////////////


/*----------------------------------------------------------------------------*/
/* parse next normal token into given buffer                                  */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextNormal(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  *wToken = '\0';
  
  eTK eToken = eParseNormal(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next special token into given buffer                                 */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eNextSpecial(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  ASSERT(bStartNormalOther());
 
  // gather .. as a string
  if (bIsDot()) {
    
    vGetNextStripped();
    
    if (bIsDot()) {
      *z++ = '.';
      *z++ = '.';
      
      // terminate the string token
      *z = '\0';
      
      return TK_STRING;
    }
    else {
      // push back the unconsumed char
      vUngetStrippedChar();
    }
  }
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  ASSERT(bStartSpecialString());
  
  // skip the leading '"'
  vGetNextStripped();
  
  // collect the characters within the string
  while ( ! bEndNormalString() ) {
    
/*
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
*/
    if ('\\' == m_cStrippedChar) {
      vGetNextStripped();
      // translate \n into new line
      if ('n' == m_cStrippedChar)
        m_cStrippedChar = '\n';
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  ASSERT(bStartSpecialString());
  
  // skip the leading '"'
  vGetNextStripped();
  
  // collect the chars in the string
  while ( ! bEndSpecialString() ) {
    
/*
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
*/
    
    if ('\\' == m_cStrippedChar) {
      vGetNextStripped();
      // translate \n into new line
      if ('n' == m_cStrippedChar)
        m_cStrippedChar = '\n';
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  while (bHaveWhitespace())
    vGetNextStripped();
}

/*----------------------------------------------------------------------------*/
/* have whitespace                                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bHaveWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsAlphaUnder(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (! String::bIsAlphaDigitUnder(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special name                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (bStartSpecialInteger() || bStartSpecialString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of special string                                           */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSpecialString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsDigit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsDigit(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a dot                                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsDot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('.' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsHex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsHex(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsX(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('x' == m_cStrippedChar || 'X' == m_cStrippedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsWhite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (String::bIsWhite(m_cStrippedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void Tokeniser::vGetNextStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_cStrippedChar = m_pParser->cGet();
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUngetStrippedChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUnget(m_cStrippedChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
