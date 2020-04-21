/*============================================================================*/
/* Input.cpp -- Input : collect input stream into tokens                      */
/*============================================================================*/

#include "LexicalAnalysis.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(LexicalAnalysis,Input,CN_INPUT)
__aNewFunction(Input,CN_INPUT)
__vDeleteFunction(Input,CN_INPUT)
__aFooterFunction(Input)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Input::Input(void)
: Object(CN_INPUT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser = new Charser(nullptr);
  
  m_eTknForm = UF_NONE;
  
  m_xAnnChar.vReset();
      
  m_pTknVec  = new TokenVec;
  m_pToken   = nullptr;
  
  d_nUseBegin = 0;
  d_nCollectToken = 0;

  _checkInitialised_(CN_INPUT);
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Input::~Input(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  while (m_pCharser)
    RASSERT(bPopCharser());
  
  delete m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Input::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_

  Input* pInput = _pInput(pObject);
  
  vTrace("parser %p\n"_Stz,
         pInput->m_pCharser);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get chars of current token                                                 */
/*----------------------------------------------------------------------------*/
Stz* Input::szChars(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  Token* pCurrToken = m_pTknVec->pCurrToken();
  
  return pCurrToken->szChars();
}

/*----------------------------------------------------------------------------*/
/* get depth of charser                                                       */
/*----------------------------------------------------------------------------*/
i8 Input::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  i8 iCharserDepth = 0;
  
  if (m_pCharser)
    iCharserDepth = m_pCharser->iDepth();
  
  return iCharserDepth;
}

/*----------------------------------------------------------------------------*/
/* dump the tokeniser                                                         */
/*----------------------------------------------------------------------------*/
void Input::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Input::vDump\n"_Stz);
  vTrace("----------------\n"_Stz);

  Token* pCurTkn = m_pTknVec->pCurrToken();
  
  vTrace("Current Token: '%s' : %s @%lld,%lld\n"_Stz,
         pCurTkn->szChars(),
         aTN[pCurTkn->eToken()],
         pCurTkn->iLineNo(),
         pCurTkn->iColNo()
         );
  
  vTrace("Charser Stack:\n"_Stz);
         
  Charser* pCharser = m_pCharser;
  while (pCharser) {
    
    vTrace("[%lld] %s @%lld,%lld\n"_Stz,
           pCharser->iDepth(),
           pCharser->szPath(),
           pCharser->iLineRaw(),
           pCharser->iLineCol()
           );
    
    pCharser = pCharser->pNext();
  }
  
  vTrace("----------------\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* hard reset after fork                                                      */
/*----------------------------------------------------------------------------*/
void Input::vForkReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vForkReset();
  
  m_eTknForm = UF_SEN_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* set normal cpp tokens                                                      */
/*----------------------------------------------------------------------------*/
void Input::vFormCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_eTknForm = UF_CPP_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* set sen tokens                                                             */
/*----------------------------------------------------------------------------*/
void Input::vFormSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  m_eTknForm = UF_SEN_TOKENS;
}

/*----------------------------------------------------------------------------*/
/* inspection                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
//  vTrace("Input : %s\n"_Stz,
//         aUF[m_eTknForm]
//         );

  if (! m_pToken) {
    vTrace("Input #UseBegin.%lld #CollectToken.%lld : ]>NoToken<[\n"_Stz,
           d_nUseBegin,
           d_nCollectToken
           );
    m_pCharser->vInspect();
    return;
  }
  
  SP_Token spTkn;

  vTrace("Input #UseBegin.%lld #CollectToken.%lld : %s\n"_Stz,
         d_nUseBegin,
         d_nCollectToken,
         spTkn.sz(m_pToken)
         );

  m_pCharser->vInspect();
}

/*----------------------------------------------------------------------------*/
/* get stdin?                                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bIsStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bStdin = m_pCharser->bIsStdin();
  
  return bStdin;
}

/*----------------------------------------------------------------------------*/
/* get charser path                                                           */
/*----------------------------------------------------------------------------*/
Stz* Input::szPath(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Stz* szSource = m_pCharser->szPath();
  
  return szSource;
}

/*----------------------------------------------------------------------------*/
/* pop a parser                                                               */
/*----------------------------------------------------------------------------*/
bool Input::bPopCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (! m_pCharser)
    return false;
  
  Charser* pToBeDelete = m_pCharser;
  m_pCharser = m_pCharser->pNext();
  
  delete pToBeDelete;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Input::vPushCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Charser* pNewCharser = new Charser(m_pCharser);
  
  m_pCharser = pNewCharser;
}

/*----------------------------------------------------------------------------*/
/* read next command from input stream                                        */
/*----------------------------------------------------------------------------*/
void Input::vReadCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  ASSERT(m_pCharser->bUseBegun());
  
  m_pTknVec->vIntegrityCheck();
  vSingleLineCmd(true);
  
  m_pTknVec->vIntegrityCheck();
  vCollectVecFirstLine();
    
  // if empty line just return
  if ( ! m_pTknVec->nTokens() ||
      ! m_pTknVec->pGet(0)->bHasChars() ) {
    m_pTknVec->vIntegrityCheck();
    return;
  }
  
  // if its a command file just return
  if ( ! m_pTknVec->pGet(0)->bSame("/"_Stz) &&
      File::bExists(m_pTknVec->pGet(0)->szChars())) {
    m_pTknVec->vIntegrityCheck();
    return;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // switch to multi-line command
  m_pTknVec->vIntegrityCheck();
  vSingleLineCmd(false);
  
  // read more till ;
  m_pTknVec->vIntegrityCheck();
  vCollectVecTillSemicolon();
  
  // switch back to single line command
  m_pTknVec->vIntegrityCheck();
  vSingleLineCmd(true);
  
  // stip off ;
  m_pTknVec->vIntegrityCheck();
  m_pTknVec->vStripSemicolon();
  
  // stip off &
  m_pTknVec->vIntegrityCheck();
  m_pTknVec->vStripAmpersand();
  
  // set no of tokens in token vector
  m_pTknVec->vIntegrityCheck();
  m_pTknVec->vSetTokens();
  m_pTknVec->vIntegrityCheck();
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Input::vReadToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  m_pTknVec->vSoftReset();

  m_pTknVec->vAdvanceCurr();
  Token* pToken = m_pTknVec->pCurrToken();;

  vCollectToken();
}

/*----------------------------------------------------------------------------*/
/* seen EOF?                                                                  */
/*----------------------------------------------------------------------------*/
bool Input::bSeenEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bEOF = m_pCharser->bSeenEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/* set single line mode                                                       */
/*----------------------------------------------------------------------------*/
void Input::vSingleLineCmd(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vSingleLineCmd(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* get SignleLine mode?                                                       */
/*----------------------------------------------------------------------------*/
bool Input::bSingleLineCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bLineMode = m_pCharser->bSingleLineCmd();
  
  return bLineMode;
}

/*----------------------------------------------------------------------------*/
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Input::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  Token* pCurrToken = m_pTknVec->pCurrToken();
  
  eTN eToken = pCurrToken->eToken();
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get token string                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Input::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/* begin use of charser source                                                */
/*----------------------------------------------------------------------------*/
void Input::vUseBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  d_nUseBegin++;
  
  m_pCharser->vUseBegin();
  
  vInspect();
}

/*----------------------------------------------------------------------------*/
/* close use of charser source                                                */
/*----------------------------------------------------------------------------*/
void Input::vUseClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseClose();
}

/*----------------------------------------------------------------------------*/
/* end use of source                                                          */
/*----------------------------------------------------------------------------*/
void Input::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Input::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bOpened = m_pCharser->bUseFile(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Input::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUseString(szString);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


//
// collecting tokens ///////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* collect next token                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  ASSERT(UF_CPP_TOKENS == m_eTknForm || UF_SEN_TOKENS == m_eTknForm);

  d_nCollectToken++;

  m_pToken = m_pTknVec->pCurrToken();;

  if (UF_CPP_TOKENS == m_eTknForm)
    vCollectCpp();
  else
    vCollectSen();
}

/*----------------------------------------------------------------------------*/
/* collect one line line of tokens from input stream                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectVecFirstLine(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  while (true) {
    
    m_pTknVec->vSoftReset();
    
    for (;;) {
      
      m_pTknVec->vAdvanceCurr();
      Token* pToken = m_pTknVec->pGet(m_pTknVec->nTokens(),true);
            
//      if (! bNextInputToken(pToken))
//        break;
      
      vCollectToken();
      
      if (TN_EOF == pToken->eToken())
        break;

      m_pTknVec->vIncTokens();
      ASSERT(m_pTknVec->nTokens() < C_MAX_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! m_pTknVec->nTokens() && bSeenEOF()) {
      bool b = bPopCharser();
      ASSERT(b);
      break;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect vector of tokens from input stream till ';'                        */
/*----------------------------------------------------------------------------*/
void Input::vCollectVecTillSemicolon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  // we should have a first token (e.g. create or drop)
  ASSERT(m_pTknVec->pGet(0)->bHasChars());
  
  // have we already seen the ';'
  i8 i_Token = m_pTknVec->iFindToken(";"_Stz);
  
  if (i_Token >= 0)
    return;
  
  i_Token = m_pTknVec->iCountTokens();
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_MAX_TOKENS; i_Token++) {
    
    Token* pToken = m_pTknVec->pGet(i_Token,true);
    
    vCollectToken();
    ASSERT(TN_EOF != pToken->eToken());
    
    // look for terminal
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz))
      break;

    m_pTknVec->vAdvanceCurr();
  }
  
}

// collect cpp tokens //////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* collect normal token                                                       */
/*----------------------------------------------------------------------------*/
void Input::vCollectCpp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_CPP_TOKENS == m_eTknForm);

  char* azTermCrs = m_pToken->azChars();
  m_pToken->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTokenCollectBegin();

  // end of input?
  if (bHitEOF()) {
    *azTermCrs = '\0';
    m_pToken->vToken(TN_EOF);
  }
  
  // collect integer literal?
  else if (bStartCppInteger()) {
    vCollectCppInteger();
  }
  
  // collect string literal?
  else if (bStartCppString()) {
    vCollectCppString();
  }
  
  // collect name token?
  else if (bStartCppName()) {
    vCollectCppName();
  }
  
  // collect other
  else {
    vCollectCppOther();
  }
  
  vTokenCollectEnd();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  // cross check valid normal tokens
  switch (m_pToken->eToken()) {
     
    // the expected tokens
    case TN_EOF:
    case TN_INTEGER:
    case TN_NAME:
    case TN_STRING:
    case TN_OTHER:
      break;
     
    // unexpected tokens
    case TN_TAG:
      NEVER_REACH;
     
    default:
      NEVER_REACH;
   }
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp integer literal                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  i8 iNoDigits = 0;
  
  // we have seen a digit
  ASSERT(bStartCppInteger());
  
  // - start parsing an integer
  
  // collect initial digits
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar;
    iNoDigits++;
    vGetAnnCharStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && m_xAnnChar.bIsX()) {
    
    // add the 'x'
    *z++ = m_xAnnChar.m_cChar;
    
    // collect the hex digits
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar;
      vGetAnnCharStripped();
    }
  }
  
  // push back the unconsumed character
  vUngetAnnCharStripped();
    
  // terminate the integer literal
  *z = '\0';
  
  m_pToken->vToken(TN_INTEGER);
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp name                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  ASSERT(bStartCppName());
    
  // collect the characters of the name
  while ( StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar) ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate the name
  *z = '\0';
  
  m_pToken->vToken(TN_NAME);
}

/*----------------------------------------------------------------------------*/
/* collect other cpp token                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  ASSERT(bStartCppOther());
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather ]] as a token
  if (m_xAnnChar.bIsCloseBracket()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsCloseBracket()) {
      *z++ = ']';
      *z++ = ']';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as ]
      m_xAnnChar.m_cChar = ']';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather :: as a string
  if (m_xAnnChar.bIsColon()) {

    vGetAnnCharStripped();

    if (m_xAnnChar.bIsColon()) {
      *z++ = ':';
      *z++ = ':';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();

      // set last single character as :
      m_xAnnChar.m_cChar = ':';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather .. as a string
  if (m_xAnnChar.bIsDot()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsDot()) {
      *z++ = '.';
      *z++ = '.';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as .
      m_xAnnChar.m_cChar = '.';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather == as a token
  if (m_xAnnChar.bIsEqual()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '=';
      *z++ = '=';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as =
      m_xAnnChar.m_cChar = '=';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather >= as a token
  if (m_xAnnChar.bIsGreaterThan()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '>';
      *z++ = '=';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as >
      m_xAnnChar.m_cChar = '>';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather <= as a token
  if (m_xAnnChar.bIsLessThan()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsEqual()) {
      *z++ = '<';
      *z++ = '=';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as <
      m_xAnnChar.m_cChar = '<';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather [[ as a token
  if (m_xAnnChar.bIsOpenBracket()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsOpenBracket()) {
      *z++ = '[';
      *z++ = '[';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as [
      m_xAnnChar.m_cChar = '[';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather || as a string
  if (m_xAnnChar.bIsPipe()) {
    
    vGetAnnCharStripped();
    
    if (m_xAnnChar.bIsPipe()) {
      *z++ = '|';
      *z++ = '|';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      return;
    }
    else {
      // push back the unconsumed char
      vUngetAnnCharStripped();
      
      // set last single character as .
      m_xAnnChar.m_cChar = '|';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect a single character
  *z++ = m_xAnnChar.m_cChar;
  
  // terminate the other token
  *z = '\0';
  m_pToken->vToken(TN_OTHER);
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp string literal                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  ASSERT(bStartSenString());
    
  // skip the leading '"'
  vGetAnnCharStripped();
  
  // collect the characters within the string
  while ( ! bEndCppString() ) {
    
    if (m_xAnnChar.bIsBackSlash()) {
      vGetAnnCharStripped();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
      else if ('r' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\r';
    }
    
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // terminate the string literal
  *z = '\0';
  
  m_pToken->vToken(TN_STRING);
}

//
// collect sen tokens //////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* collect Sen token                                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_SEN_TOKENS == m_eTknForm);
  
  char* azTermCrs = m_pToken->azChars();
  
  m_pToken->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTokenCollectBegin();

  // end of input?
  if (bHitEOF()) {
    *azTermCrs = '\0';
    m_pToken->vToken(TN_EOF);
  }
  
  // collect integer literal?
  else
  if (bStartSenInteger()) {
    vCollectSenInteger();
  }
  
  // collect string literal?
  else if (bStartSenString()) {
    vCollectSenString();
  }

#ifdef WITH_SEN_TAGS
  // collect sen tag?
  else if (bStartSenTag()) {
    vCollectSenTag();
  }
#endif
  
  // collect name token
  else {
    vCollectSenName();
  }
  
  vTokenCollectEnd();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cross check valid SEN tokens
  switch (m_pToken->eToken()) {
      
    // the expected tokens
    case TN_INTEGER:
    case TN_NAME:
    case TN_TAG:
      if (m_pToken->szChars()->bSame(""_Stz)) {
        vInspect();
        Correctness::vDebugPoint();
      }
      break;
      
    case TN_EOF:
    case TN_STRING:
      break;

    // unexpected tokens
    case TN_OTHER:
      NEVER_REACH;
      
    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/* collect Sen integer                                                        */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  i8 iNoDigits = 0;
  
  // we have seen a minus or a digit
  ASSERT(bStartSenInteger());
  
  // - start parsing an integer
  
  // collect leading minus
  if (m_xAnnChar.bIsMinus()) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // collect initial digits
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar;
    iNoDigits++;
    vGetAnnCharStripped();
  }
  *z = '\0';

  // 0x hex number?
  if (1 == iNoDigits && m_xAnnChar.bIsX()) {

    // add the 'x'
    *z++ = m_xAnnChar.m_cChar;
    
    // collect the hex digits
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar;
      vGetAnnCharStripped();
    }
  }
  
  // - handle minus only
  if (! iNoDigits) {
    vCollectSenName();
    return;
  }
  
  // - handle the case where we have a name like 1_ugh
  
  // just a number?
  if (m_xAnnChar.bIsWhite() || bStartSenString() ) {
    
    // push back the unconsumed char
    vUngetAnnCharStripped();
    
    // terminate an intlit
    *z = '\0';
    m_pToken->vToken(TN_INTEGER);
    return;
  }
  
  // its actually a name; complete parsing name
  vCollectSenName();
}

/*----------------------------------------------------------------------------*/
/* collect Sen name                                                           */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  //ASSERT(bStartSenName());
  
  // collect the characters of the name
  while ( m_xAnnChar.m_cChar && !m_xAnnChar.bIsWhite() && !bStartSenString() ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate a namelit
  *z = '\0';

  m_pToken->vToken(TN_NAME);
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  ASSERT(bStartSenString());
    
  char cStartQuote = m_xAnnChar.m_cChar;
  
  // skip the leading '"' or '\''
  vGetAnnChar();
  
  // collect the chars in the string
  //while ( ! bEndSenString() ) {
  while ( m_xAnnChar.m_cChar != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,m_xAnnChar.m_cChar);
        
    if (m_xAnnChar.bIsBackSlash()) {
      vGetAnnChar();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
    }    
    
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnChar();
  }
 
  // terminate the strlit
  *z = '\0';

  m_pToken->vToken(TN_STRING);
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *azChars = StrLib::zNullTerminator(m_pToken->azChars());
  char *z = azChars;

  ASSERT(bStartSenTag());
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_xAnnChar.m_cChar || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    // push back the unconsumed char
    vUngetAnnCharStripped();
    
    // terminate a namelit
    *z = '\0';

    m_pToken->vToken(TN_NAME);
    
    return;
  }
     
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // collect the characters of the name
  
  while ( m_xAnnChar.m_cChar && !m_xAnnChar.bIsWhite() && !bStartSenString() ) {
    *z++ = m_xAnnChar.m_cChar;
    vGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vUngetAnnCharStripped();
  
  // terminate a namelit
  *z = '\0';

  m_pToken->vToken(TN_TAG);
}


//
// character processing ////////////////////////////////////////////////////////
//

/*----------------------------------------------------------------------------*/
/* skip whitespace                                                            */
/*----------------------------------------------------------------------------*/
void Input::vSkipWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  while (bHaveWhitespace())
    vGetAnnCharStripped();
}

/*----------------------------------------------------------------------------*/
/* have whitespace                                                            */
/*----------------------------------------------------------------------------*/
bool Input::bHaveWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  // do we have a current parsed character, which is a white char?
  if (m_xAnnChar.m_cChar) {
    if (StrLib::bIsWhite(m_xAnnChar.m_cChar))
      return true;
    else
      return false;
  }

  // - no current parsed character
  
  // a '\0' in line mode indicates end of line; not considered whitespace
  if (bSingleLineCmd())
    return false;
  
  // a '\0' at eof is the eof marker; not considered whitespace
  if (bSeenEOF())
    return false;

  // a '\0' is otherwise whitespace
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal integer                                           */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (StrLib::bIsDigit(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if ('$' == m_xAnnChar.m_cChar)
    return true;
  
  if (StrLib::bIsNameStartChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (! StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Input::bStartCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Input::bEndCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen integer                                              */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsMinus() || StrLib::bIsDigit(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen name                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (bStartSenInteger() || bStartSenString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen string                                               */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
// 2020-Feb-26 RJH: I commented this out as I don't think we have 'string'
//                  in sen literals
  if (m_xAnnChar.bIsSingleQuote())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen tag                                                  */
/*----------------------------------------------------------------------------*/
bool Input::bStartSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  //if (m_xAnnChar.bIsTilde())
  if (m_xAnnChar.bIsLessThan())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of Sen string                                                 */
/*----------------------------------------------------------------------------*/
bool Input::bEndSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;

// 2020-Feb-26 RJH: I commented this out as I don't think we have 'string'
//                  in sen literals
  if (m_xAnnChar.bIsSingleQuote())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get next character from parser input                                       */
/*----------------------------------------------------------------------------*/
void Input::vGetAnnChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vGetAnnChar(m_xAnnChar,false);
  m_xAnnChar.m_cChar = m_xAnnChar.m_cChar;
}

/*----------------------------------------------------------------------------*/
/* get next input character (stripped of comments)                            */
/*----------------------------------------------------------------------------*/
void Input::vGetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vGetAnnChar(m_xAnnChar,true);
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Input::vUngetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vUngetAnnChar(m_xAnnChar);
}

/*----------------------------------------------------------------------------*/
/* begin collecting a new token                                               */
/*----------------------------------------------------------------------------*/
void Input::vTokenCollectBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
#ifdef __WITH_CHARS_SET
  
//  Stz* szLatter = m_pTknVec->szLaterChars();
//
//  * szLatter->azChar() = '\0';
//
//  m_pToken->vChars(szLatter);
  
  m_pTknVec->vCollectBegin(m_pToken);
  
#endif
  
  m_pToken->vLineNo(m_xAnnChar.m_iLine);
  m_pToken->vColNo(m_xAnnChar.m_iCol);
}

/*----------------------------------------------------------------------------*/
/* end collecting a token                                                     */
/*----------------------------------------------------------------------------*/
void Input::vTokenCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

#ifdef __WITH_CHARS_SET
  
//  i8 nChars = m_pToken->nChars();
//
//  m_pTknVec->vTakeCharsBytes(nChars + 1);
//
//  Stz* szLatter = m_pTknVec->szLaterChars();
//  * szLatter->azChar() = '\0';
//
//  vTrace("Chars: %s\n"_Stz,m_pToken->szChars());
  
  m_pTknVec->vCollectEnd(m_pToken);

#endif
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Input::bHitEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsNull())
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
