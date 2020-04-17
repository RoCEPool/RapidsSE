/*============================================================================*/
/* Input.cpp -- Input : divide input stream into tokens               */
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
  
  m_pCharser   = new Charser(nullptr);
  
  m_eTknForm = UF_NONE;
  
  m_xAnnChar.vReset();
      
  m_pTknVec    = new TokenVec;
  m_pToken     = nullptr;
  
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
/* get current charser                                                        */
/*----------------------------------------------------------------------------*/
Charser* Input::pCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  return m_pCharser;
}

/*----------------------------------------------------------------------------*/
/* set normal cpp tokens                                                      */
/*----------------------------------------------------------------------------*/
void Input::vCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_eTknForm = UF_CPP_TOKENS;
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
         pCurTkn->szTerm(),
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
/* inspection                                                                 */
/*----------------------------------------------------------------------------*/
void Input::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vTrace("Input : %s\n"_Stz,
         aUF[m_eTknForm]
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
/* set single line mode                                                       */
/*----------------------------------------------------------------------------*/
void Input::vSingleLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  m_pCharser->vSingleLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* get SignleLine mode?                                                       */
/*----------------------------------------------------------------------------*/
bool Input::bSingleLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  bool bLineMode = m_pCharser->bSingleLineMode();
  
  return bLineMode;
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
/* push input file                                                            */
/*----------------------------------------------------------------------------*/
void Input::vPushFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  vPushCharser();
  
  bool b = bUseFile(szPath);
  ASSERT(b);
  
  vSingleLineMode(true);
}

/*----------------------------------------------------------------------------*/
/* read next command from token stream                                        */
/*----------------------------------------------------------------------------*/
void Input::vReadCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  ASSERT(m_pCharser->bUseBegun());
  
  vSingleLineMode(true);
  
  vReadTillEOF();
    
  // if empty line just return
  if ( ! m_pTknVec->nTokens() ||
       ! m_pTknVec->pGet(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! m_pTknVec->pGet(0)->bSame("/"_Stz) &&
     File::bExists(m_pTknVec->pGet(0)->szTerm()))
    return;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // switch to terminal mode
  vSingleLineMode(false);
  
  // read more till ;
  vReadTillSemicolon();
  
  // switch back to line mode
  vSingleLineMode(true);
  
  // stip off ;
  m_pTknVec->vStripSemicolon();
  
  // stip off &
  m_pTknVec->vStripAmpersand();
  
  // set no of tokens in token vector
  m_pTknVec->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* read tokens from input stream till EOF                                     */
/*----------------------------------------------------------------------------*/
void Input::vReadTillEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  while (true) {
    
    m_pTknVec->vReset();
    
    for (;;) {
      
      m_pTknVec->vAdvanceCurr();
      Token* pToken = m_pTknVec->pGet(m_pTknVec->nTokens(),true);
            
//      if (! bNextInputToken(pToken))
//        break;
      
      vReadToken_vec();
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
/* read tokens from input stream till ';'                                     */
/*----------------------------------------------------------------------------*/
void Input::vReadTillSemicolon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  // we should have a first token (e.g. create or drop)
  ASSERT(m_pTknVec->pGet(0)->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = m_pTknVec->iFindToken(";"_Stz);
  
  if (i_Token >= 0)
    return;
  
  i_Token = m_pTknVec->iCountTokens();
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_MAX_TOKENS; i_Token++) {
    
    Token* pToken = m_pTknVec->pGet(i_Token,true);
    
    vReadToken_vec();
    ASSERT(TN_EOF != pToken->eToken());
    
    // look for terminal
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz))
      break;

    m_pTknVec->vAdvanceCurr();
  }
  
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Input::vReadToken_nil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  
  m_pTknVec->vReset();

  m_pTknVec->vAdvanceCurr();
  Token* pToken = m_pTknVec->pCurrToken();;

  vReadToken_vec();
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
void Input::vReadToken_vec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(m_pCharser->bUseBegun());
  ASSERT(UF_CPP_TOKENS == m_eTknForm || UF_SEN_TOKENS == m_eTknForm);

  m_pToken = m_pTknVec->pCurrToken();;

  if (UF_CPP_TOKENS == m_eTknForm)
    vCollectCpp();
  else
    vCollectSen();
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
/* set sen tokens                                                             */
/*----------------------------------------------------------------------------*/
void Input::vSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
    
  m_eTknForm = UF_SEN_TOKENS;
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
Stz* Input::szToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);

  Token* pCurrToken = m_pTknVec->pCurrToken();
  
  return pCurrToken->szTerm();
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
  
  m_pCharser->vUseBegin();
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


/*----------------------------------------------------------------------------*/
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect normal token                                                       */
/*----------------------------------------------------------------------------*/
void Input::vCollectCpp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_CPP_TOKENS == m_eTknForm);

  char* azTermCrs = m_pToken->azTerm();
  m_pToken->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *azTermCrs = '\0';
    m_pToken->vToken(TN_EOF);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // collect integer literal?
  if (bStartCppInteger()) {
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
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());
  
  i8 iNoDigits = 0;
  
  // we have seen a digit
  ASSERT(bStartCppInteger());
  
  vTokenCollectBegin();
  
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
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp name                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartCppName());
  
  vTokenCollectBegin();
  
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
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect other cpp token                                                    */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartCppOther());
 
  vTokenCollectBegin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather ]] as a token
  if (m_xAnnChar.bIsCloseBracket()) {
    vGetAnnCharStripped();
    if (m_xAnnChar.bIsCloseBracket()) {
      *z++ = ']';
      *z++ = ']';
      *z = '\0';
      m_pToken->vToken(TN_STRING);
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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
      vTokenCollectEnd();
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

  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect normal cpp string literal                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectCppString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenString());
  
  vTokenCollectBegin();
  
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
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen token                                                          */
/*----------------------------------------------------------------------------*/
void Input::vCollectSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  ASSERT(UF_SEN_TOKENS == m_eTknForm);
  
  char* azTermCrs = m_pToken->azTerm();
  
  m_pToken->vReset();

  vGetAnnCharStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *azTermCrs = '\0';
    m_pToken->vToken(TN_EOF);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // collect integer literal?
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
    vTokenCollectBegin();
    vCollectSenName();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cross check valid SEN tokens
  switch (m_pToken->eToken()) {
      
    // the expected tokens
    case TN_EOF:
    case TN_INTEGER:
    case TN_NAME:
    case TN_STRING:
    case TN_TAG:
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
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  i8 iNoDigits = 0;
  
  // we have seen a minus or a digit
  ASSERT(bStartSenInteger());
  
  vTokenCollectBegin();

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
    vTokenCollectEnd();
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
    vTokenCollectEnd();
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
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());
  
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
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenString());
  
  vTokenCollectBegin();
  
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
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
void Input::vCollectSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenTag());
  
  vTokenCollectBegin();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_xAnnChar.m_cChar || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    // push back the unconsumed char
    vUngetAnnCharStripped();
    
    // terminate a namelit
    *z = '\0';

    m_pToken->vToken(TN_NAME);
    
    vTokenCollectEnd();

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
  
  vTokenCollectEnd();
  
  //vTrace("Recognised Sen Tag: %s\n"_Stz,pToken->szTerm());
  
  Correctness::vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- character processing ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  if (bSingleLineMode())
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
  
  m_pToken->vLineNo(m_xAnnChar.m_iLine);
  m_pToken->vColNo(m_xAnnChar.m_iCol);
}

/*----------------------------------------------------------------------------*/
/* end collecting a token                                                     */
/*----------------------------------------------------------------------------*/
void Input::vTokenCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Input::bEndInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INPUT,_FOOTER_);
  
  if (m_xAnnChar.bIsNull())
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
