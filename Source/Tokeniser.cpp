/*============================================================================*/
/* Tokeniser.cpp -- Tokeniser : divide input stream into tokens               */
/*============================================================================*/

#include "Commands.h"
#include "StorageEngine.h"

extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Tokeniser* Commands::_pTokeniser(void* aAddr)
{
  Tokeniser* pTokeniser = reinterpret_cast<Tokeniser*>(aAddr);
  
  RASSERT_TYPE(pTokeniser,CN_TOKENISER);
  
  return pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Tokeniser::operator new(size_t)
{
  void* pTokeniser = HeapMem::pAllocate(CN_TOKENISER);
  
  return pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::operator delete(void* pTokeniserAddress)
{
  Tokeniser* pTokeniser = _pTokeniser(pTokeniserAddress);
  
  HeapMem::vFree(pTokeniser);
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
  
  m_sToken  = ""_Stz;
  m_eToken  = TN_NONE;
  
  
  i8 nNodes = gpAttachMap->m_pGrid->nNodes();
  Node* pNode = gpAttachMap->m_pGrid->pThisNode();
  i8  i_Node = pNode->iNodeNo();
  //bool bHasStarted = gpAttachMap->m_pGrid->m_bHasStarted;
  
  m_sPrompt.vSprint("[N%lld/%lld %s %lld%s%s] se %s%c"_Stz,
                    
                    i_Node,
                    nNodes,
                    pNode->aNodeName(),
                    Process::iGetPid(),
                    gpAttachMap->Arg.m_bActivityMap? " AM" : "",
#ifdef WITH_ENTRY_EXIT
                    " EE",
#else
                    "",
#endif
                    
                    RAPIDSSE_VERSION_STRING,
#ifdef _DEBUG
                    'd'
#else
                    'r'
#endif
                    );

  m_sContinue = "  > "_Stz;

  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_xTokenVec.vSetAllTokens();
  
  ASSERT(C_TOKENS == m_xTokenVec.nItems());
  delete m_pParser;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* tokenise given string                                                      */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTokenise(s4K& sString)
{
  TokenVec* pTokenVec = new TokenVec;
  
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPush();
  pTokeniser->vUseString(sString);
  
  for (i8 i_Token = 0; ; i_Token++) {
    pTokeniser->vNextToken();
    if (TN_NONE == pTokeniser->eToken())
      break;
    
    pTokenVec->vIncTokens();
    
    Token* pToken = pTokenVec->pGet(i_Token);
    pToken->vToken(pTokeniser->eToken());
    pToken->vCopy(pTokeniser->sToken().aStz());
    
  }
  
  delete pTokeniser;
  
  return pTokenVec;
}

/*----------------------------------------------------------------------------*/
/* tokenise given string                                                      */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTokenise(s48K& sString)
{
  TokenVec* pTokenVec = new TokenVec;
  
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPush();
  pTokeniser->vUseString(sString);
  
  for (i8 i_Token = 0; ; i_Token++) {
    pTokeniser->vNextToken();
    if (TN_NONE == pTokeniser->eToken())
      break;
    
    pTokenVec->vIncTokens();
    
    Token* pToken = pTokenVec->pGet(i_Token);
    pToken->vToken(pTokeniser->eToken());
    pToken->vCopy(pTokeniser->sToken().aStz());
    
  }
  
  delete pTokeniser;
  
  return pTokenVec;
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Tokeniser* pTokeniser = _pTokeniser(pObject);
  
  vTrace("parser %p %s\n"_Stz,
         pTokeniser->m_pParser,
         pTokeniser->m_bMalloc == true ? "malloc"_Stz : "pool"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(m_xTokenVec[0]->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = m_xTokenVec.iFindToken(";"_Stz);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_TOKENS);
    return;
  }
  
  i_Token = m_xTokenVec.iCountTokens();
  
  // switch to terminal mode
  vLineMode(false);
  
  vPrompt(m_sContinue);
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_TOKENS; i_Token++) {
    
    Token* pToken = m_xTokenVec[i_Token];
    eTN eToken = eNextToken((char*)pToken->aText());
    ASSERT(TN_NONE != eToken);
    pToken->vToken(eToken);
    
    // look for terminal
    if (TN_OTHER == eToken && pToken->bSame(";"_Stz))
      break;
  }
  
  // switch back to line mode
  vLineMode(true);
  vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
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
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bEOF = m_pParser->bEOF();
  
  return bEOF;
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
/* get LineMode?                                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bLineMode = m_pParser->bLineMode();
  
  return bLineMode;
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
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eNextToken(char* aToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_bNormal)
    m_eToken = eNextNormal(aToken);
  else
    m_eToken = eNextSpecial(aToken);
  
  return m_eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  eTN eToken = eNextToken(m_sToken.aChar());
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
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bPop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ( ! m_pParser )
    return false;
  
  Parser* pToBeDelete = m_pParser;
  m_pParser = m_pParser->pNext();
  
  ASSERT(m_pParser);
  
  delete pToBeDelete;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set prompt                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPrompt(s256& sPrompt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vPrompt(sPrompt);
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPush(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser = new(m_bMalloc) Parser(m_pParser);
  
  m_pParser->vPrompt(m_sPrompt);
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
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return &m_xTokenVec;
}

/*----------------------------------------------------------------------------*/
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_eToken;
}

/*----------------------------------------------------------------------------*/
/* get token string                                                           */
/*----------------------------------------------------------------------------*/
s48K& Tokeniser::sToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_sToken;
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
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bUseFile(s256& sPath)
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
void Tokeniser::vUseString(s4K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseString(s48K& sString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseString(sString);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse next normal token into given buffer                                  */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eNextNormal(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  *wToken = '\0';
  
  eTN eToken = eParseNormal(wToken);
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse next special token into given buffer                                 */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eNextSpecial(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  *wToken = '\0';

  eTN eToken = eParseSpecial(wToken);
  
  return eToken;
}


/*----------------------------------------------------------------------------*/
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse normal                                                               */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseNormal(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vGetNextStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *wToken = '\0';
    return TN_NONE;
  }
  
  // collect integer literal?
  if (bStartNormalInteger()) {
    eTN eLiteral = eParseNormalInteger(wToken);
    return eLiteral;
  }
  
  // collect string literal?
  if (bStartNormalString()) {
    eTN eToken = eParseNormalString(wToken);
    return eToken;
  }
  
  // collect name token?
  if (bStartNormalName()) {
    eTN eToken = eParseNormalName(wToken);
    return eToken;
  }
  
  eTN eToken = eParseNormalOther(wToken);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse normal integer literal                                               */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseNormalInteger(char* wToken)
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
  
  return TN_INTEGER;
}

/*----------------------------------------------------------------------------*/
/* parse normal name token                                                    */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseNormalName(char* wToken)
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
  
  return TN_NAME;
}

/*----------------------------------------------------------------------------*/
/* parse other token                                                          */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseNormalOther(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  ASSERT(bStartNormalOther());
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather .. as a string
  if (bIsDot()) {
    
    vGetNextStripped();
    
    if (bIsDot()) {
      *z++ = '.';
      *z++ = '.';
      
      // terminate the string token
      *z = '\0';
      
      return TN_STRING;
    }
    else {
      // push back the unconsumed char
      vUngetStrippedChar();
      
      // set last single character as .
      m_cParsedChar = '.';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather == as a token
  if (bIsEqual()) {
    vGetNextStripped();
    if (bIsEqual()) {
      *z++ = '=';
      *z++ = '=';
      *z = '\0';
      return TN_STRING;
    }
    else {
      // push back the unconsumed char
      vUngetStrippedChar();
      
      // set last single character as =
      m_cParsedChar = '=';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather <= as a token
  if (bIsLessThan()) {
    vGetNextStripped();
    if (bIsEqual()) {
      *z++ = '<';
      *z++ = '=';
      *z = '\0';
      return TN_STRING;
    }
    else {
      // push back the unconsumed char
      vUngetStrippedChar();
      
      // set last single character as <
      m_cParsedChar = '<';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather >= as a token
  if (bIsGreaterThan()) {
    vGetNextStripped();
    if (bIsEqual()) {
      *z++ = '>';
      *z++ = '=';
      *z = '\0';
      return TN_STRING;
    }
    else {
      // push back the unconsumed char
      vUngetStrippedChar();
      
      // set last single character as >
      m_cParsedChar = '>';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect a single character
  *z++ = m_cParsedChar;
  
  // terminate the other token
  *z = '\0';
  
  return TN_OTHER;
}

/*----------------------------------------------------------------------------*/
/* parse normal string literal                                                */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseNormalString(char* wToken)
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
      else if ('r' == m_cParsedChar)
        m_cParsedChar = '\r';
    }
    
    *z++ = m_cParsedChar;
    vGetNextStripped();
  }
  
  // terminate the string literal
  *z = '\0';
  
  return TN_STRING;
}

/*----------------------------------------------------------------------------*/
/* parse special                                                              */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseSpecial(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vGetNextStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *wToken = '\0';
    return TN_NONE;
  }
  
  // collect integer literal?
  if (bStartSpecialInteger()) {
    eTN eLiteral = eParseSpecialInteger(wToken);
    return eLiteral;
  }
  
  // collect string literal?
  if (bStartSpecialString()) {
    eTN eToken = eParseSpecialString(wToken);
    return eToken;
  }
  
  // collect name token
  eTN eToken = eParseSpecialName(wToken);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse special integer                                                      */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseSpecialInteger(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  i8 iNoDigits = 0;
  char *z = wToken;
  
  // we have seen a minus or a digit
  ASSERT(bStartSpecialInteger());
  
  // - start parsing an integer
  
  // collect leading minus
  if (bIsMinus()) {
    *z++ = m_cParsedChar;
    vGetNextStripped();
  }
  
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
  
  // - handle minus only
  if (! iNoDigits) {
    eTN eToken = eParseSpecialName(z);
    return eToken;    
  }
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if ( bIsWhite() || bStartSpecialString() ) {
    
    // push back the unconsumed char
    vUngetStrippedChar();
    
    // terminate an intlit
    *z = '\0';
    return TN_INTEGER;
  }
  
  // its actually a name; complete parsing name
  eTN eToken = eParseSpecialName(z);
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* parse special name                                                         */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseSpecialName(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  //ASSERT(bStartSpecialName());
  
  // collect the characters of the name
  while ( m_cParsedChar && !bIsWhite() && !bStartSpecialString() ) {
    *z++ = m_cParsedChar;
    vGetNextStripped();
  }
  
  // push back the unconsumed char
  vUngetStrippedChar();
  
  // terminate a namelit
  *z = '\0';

  return TN_NAME;
}

/*----------------------------------------------------------------------------*/
/* parse special string                                                       */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eParseSpecialString(char* wToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = wToken;
  
  ASSERT(bStartSpecialString());
  
  char cStartQuote = m_cParsedChar;
  
  // skip the leading '"' or '\''
  vGetNext();
  
  // collect the chars in the string
  //while ( ! bEndSpecialString() ) {
  while ( m_cParsedChar != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,m_cParsedChar);
    
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

  return TN_STRING;
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
  
  if (bIsMinus() || StrLib::bIsDigit(m_cParsedChar))
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
  
  if ('"' == m_cParsedChar || '\'' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of special string                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSpecialString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_cParsedChar || '\'' == m_cParsedChar)
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
/* test for a equal sign                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsEqual(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('=' == m_cParsedChar)
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
/* test for a less than sign                                                  */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsLessThan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('<' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a greater than sign                                               */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsGreaterThan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('>' == m_cParsedChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for a minus sign                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bIsMinus(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('-' == m_cParsedChar)
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
Footer* Tokeniser::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
