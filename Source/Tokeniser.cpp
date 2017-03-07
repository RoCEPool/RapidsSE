/*============================================================================*/
/* Tokeniser.cpp -- Tokeniser : divide input stream into tokens               */
/*============================================================================*/

#include "Parsing.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Tokeniser* Tokeniser::_pTokeniser(void* aAddr)
{
  Tokeniser* pTokeniser = reinterpret_cast<Tokeniser*>(aAddr);
  
  RASSERT( !pTokeniser || CN_TOKENISER == pTokeniser->eClass() );
  
  return pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tokeniser::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pTokeniser = Heap::pAllocate(CN_TOKENISER);
  
  return pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::operator delete(void* pTokeniserAddress)
{
  RASSERT(pTokeniserAddress);
  Tokeniser* pTokeniser = _pTokeniser(pTokeniserAddress);
  
  Heap::vFree(pTokeniser);
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
  
  m_sToken  = ""_Str;
  m_eToken  = TK_NONE;
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  delete m_pParser;
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDump(eCN eClass, Object* pObject, const char* aTab)
{
  ASSERT(eClass == CN_TOKENISER);
  ASSERT(pObject->eClass() == CN_TOKENISER);
  
  Tokeniser* pTokeniser = reinterpret_cast<Tokeniser*>(pObject);
  
  vTrace("parser %p %s\n",
         pTokeniser->m_pParser,
         pTokeniser->m_bMalloc == true ? "malloc" : "pool");
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPush(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser = new(m_bMalloc) Parser(m_pParser);
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ( ! m_pParser )
    return false;
  
  Parser* pToBeDelete = m_pParser;
  m_pParser = m_pParser->pNext();
  
  delete pToBeDelete;
  
  return true;
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
void Tokeniser::vPrompt(StrVal& sPrompt)
{  
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vPrompt(sPrompt);
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bUseFile(StrVal& sPath)
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
void Tokeniser::vUseString(StrVal& sString)
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
  
  if (m_bNormal)
    m_eToken = eNextNormal(wToken);
  else
    m_eToken = eNextSpecial(wToken);
  
  return m_eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);

  eTK eToken = eNextToken(m_sToken.aStr());
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
    if (StrLib::bEqualTo(pLookFor,pToken[iToken],C_NO_CASE))
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
    if (StrLib::bEqualTo("",pToken[iToken]))
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


/*----------------------------------------------------------------------------*/
/* get toke type                                                              */
/*----------------------------------------------------------------------------*/
s48K& Tokeniser::sToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_sToken;
}

/*----------------------------------------------------------------------------*/
/* get toke type                                                              */
/*----------------------------------------------------------------------------*/
eTK Tokeniser::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_eToken;
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
    *z++ = m_cParsedChar;
    iNoDigits++;
    vGetNextStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {
    
    // add the 'x'
    *z++ = m_cParsedChar;
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cParsedChar;
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
  while ( StrLib::bIsAlphaDigitUnder(m_cParsedChar) ) {
    *z++ = m_cParsedChar;
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
  *z++ = m_cParsedChar;
  
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
    if ('\\' == m_cParsedChar) {
      vGetNextStripped();
      if ('n' == m_cParsedChar)
        m_cParsedChar = '\n';
      else {
        vUngetStrippedChar();
        m_cParsedChar = '\\';
      }
    }
*/
    if ('\\' == m_cParsedChar) {
      vGetNextStripped();
      // translate \n into new line
      if ('n' == m_cParsedChar)
        m_cParsedChar = '\n';
    }
    
    *z++ = m_cParsedChar;
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
    *z++ = m_cParsedChar;
    iNoDigits++;
    vGetNextStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && bIsX()) {

    // add the 'x'
    *z++ = m_cParsedChar;
    
    // collect the hex digits
    while (bIsHex()) {
      *z++ = m_cParsedChar;
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
  while ( m_cParsedChar && !bIsWhite() && !bStartSpecialString() ) {
    *z++ = m_cParsedChar;
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
  vGetNext();
  
  // collect the chars in the string
  while ( ! bEndSpecialString() ) {
    
/*
    // test for \n
    if ('\\' == m_cParsedChar) {
      vGetNextStripped();
      if ('n' == m_cParsedChar)
        m_cParsedChar = '\n';
      else {
        vUngetStrippedChar();
        m_cParsedChar = '\\';
      }
    }
*/
    
    if ('\\' == m_cParsedChar) {
      vGetNext();
      // translate \n into new line
      if ('n' == m_cParsedChar)
        m_cParsedChar = '\n';
    }    
    
    *z++ = m_cParsedChar;
    vGetNext();
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
  if (m_cParsedChar) {
    if (StrLib::bIsWhite(m_cParsedChar))
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
  
  if (StrLib::bIsDigit(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsAlphaUnder(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (! StrLib::bIsAlphaDigitUnder(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of special integer                                          */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSpecialInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsDigit(m_cParsedChar))
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
  
  if ('"' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSpecialString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-10 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsDigit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsDigit(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a dot                                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsDot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('.' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a base-16 digit                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsHex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsHex(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for an 'x' or 'X'                                                     */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsX(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('x' == m_cParsedChar || 'X' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a white character                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsWhite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsWhite(m_cParsedChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next input character                                                   */
/*----------------------------------------------------------------------------*/
void Tokeniser::vGetNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_cParsedChar = m_pParser->cGet(false);
}

/*----------------------------------------------------------------------------*/
/* get next input character (stripped of comments)                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vGetNextStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_cParsedChar = m_pParser->cGet(true);
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUngetStrippedChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUnget(m_cParsedChar);
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('\0' == m_cParsedChar)
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
