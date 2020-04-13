/*============================================================================*/
/* Tokeniser.cpp -- Tokeniser : divide input stream into tokens               */
/*============================================================================*/

#include "LexicalAnalysis.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(LexicalAnalysis,Tokeniser,CN_TOKENISER)
__aNewFunction(Tokeniser,CN_TOKENISER)
__vDeleteFunction(Tokeniser,CN_TOKENISER)
__aFooterFunction(Tokeniser)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tokeniser::Tokeniser(void)
: Object(CN_TOKENISER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser = nullptr;
  m_bMalloc  = false;
  m_bNormal  = false;
  
  m_xAnnChar.vReset();
      
  m_pTknVec  = new TokenVec;
  m_pToken   = nullptr;
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  while (m_pCharser)
    RASSERT(bPopCharser());
  
  delete m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/* tokenise given string                                                      */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTokenise(Stz* szString)
{
  _static_
  
  TokenVec* pTknVec = new TokenVec;
  
  vTokenise(pTknVec,szString);
  
  return pTknVec;
}

/*----------------------------------------------------------------------------*/
/* tokenise given string into given TokenVec                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vTokenise(TokenVec* pTknVec, Stz* szString)
{
  _static_
  
  pTknVec->vReset();
  
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPushCharser();
  pTokeniser->vUseString(szString);
  
  for (i8 i_Token = 0; ; i_Token++) {
    pTokeniser->vReadToken_nil();
    if (TN_EOF == pTokeniser->eToken())
      break;
    
    pTknVec->vIncTokens();
    
    Token* pToken = pTknVec->pGet(i_Token);
    pToken->vToken(pTokeniser->eToken());
    pToken->vCopy(pTokeniser->szToken());
    
  }
  
  delete pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* extract { EnameSet } from given tokenvec                                   */
/*----------------------------------------------------------------------------*/
void Tokeniser::vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec)
{
  _static_
  
  pEnameSet->vReset();

  i8 i_OpenToken  = pTknVec->iFindToken("{"_Stz);
  i8 i_CloseToken = pTknVec->iFindToken("}"_Stz);

  if (C_UNKNOWN == i_OpenToken || C_UNKNOWN == i_CloseToken)
    return;
  
  vToEnmSet(pEnameSet,pTknVec,i_OpenToken + 1,i_CloseToken - 1);
}

/*----------------------------------------------------------------------------*/
/* extract { EnameSet } from given tokenvec                                   */
/*----------------------------------------------------------------------------*/
void Tokeniser::vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, i8 i_From, i8 i_To)
{
  _static_
  
  pEnameSet->vReset();
  
  for (i8 i_Tkn = i_From; i_Tkn <= i_To; ) {
 
    i8 i_Name = i_Tkn++;
    
    if (pTknVec->pGet(i_Tkn)->bSame(":"_Stz) ||
        pTknVec->pGet(i_Tkn)->bSame("="_Stz))
      i_Tkn++;
    
    i8 i_Value = i_Tkn++;
    
    pEnameSet->vInsert(pTknVec->pGet(i_Name)->szTerm(),
                       pTknVec->pGet(i_Value)->szTerm());
    
    if (pTknVec->pGet(i_Tkn)->bSame(","_Stz))
      i_Tkn++;
  }
}

/*----------------------------------------------------------------------------*/
/* extract { EnameSet } from given tokenvec                                     */
/*----------------------------------------------------------------------------*/
void Tokeniser::vToEnmSet(EnameSet* pEnameSet, TokenVec* pTknVec, Stz* sDelimiter, i8 i_From, i8 i_To)
{
  _static_
  
  pEnameSet->vReset();
  
  for (i8 i_Tkn = i_From; i_Tkn <= i_To; ) {
    
    i8 i_Name = i_Tkn++;
    
    if (pTknVec->pGet(i_Tkn)->bSame(sDelimiter))
        i_Tkn++;
        
    i8 i_Value = i_Tkn++;
    
    pEnameSet->vInsert(pTknVec->pGet(i_Name)->szTerm(),
                       pTknVec->pGet(i_Value)->szTerm());
    
    if (pTknVec->pGet(i_Tkn)->bSame(","_Stz))
    i_Tkn++;
  }
}

/*----------------------------------------------------------------------------*/
/* dump tokeniser                                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_

  Tokeniser* pTokeniser = _pTokeniser(pObject);
  
  vTrace("parser %p %s\n"_Stz,
         pTokeniser->m_pCharser,
         pTokeniser->m_bMalloc == true ? "malloc"_Stz : "pool"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current charser                                                        */
/*----------------------------------------------------------------------------*/
Charser* Tokeniser::pCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_pCharser;
}

/*----------------------------------------------------------------------------*/
/* get depth of parser                                                        */
/*----------------------------------------------------------------------------*/
i8 Tokeniser::iDepth(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  i8 iParserDepth = 0;
  
  if (m_pCharser)
    iParserDepth = m_pCharser->iDepth();
  
  return iParserDepth;
}

/*----------------------------------------------------------------------------*/
/* dump the tokeniser                                                         */
/*----------------------------------------------------------------------------*/
void Tokeniser::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Tokeniser::vDump\n"_Stz);
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
           pCharser->szSource(),
           pCharser->iLineRaw(),
           pCharser->iLineCol()
           );
    
    pCharser = pCharser->pNext();
  }
  
  vTrace("----------------\n"_Stz);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bEOF = m_pCharser->bEOF();
  
  return bEOF;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* get LineMode?                                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bLineMode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bLineMode = m_pCharser->bLineMode();
  
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
bool Tokeniser::bPopCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
void Tokeniser::vPushCharser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser = new(m_bMalloc) Charser(m_pCharser);
}

/*----------------------------------------------------------------------------*/
/* read next command from token stream                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReadCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
    
  vReadTillEOF();
    
  // if empty line just return
  if ( ! m_pTknVec->nTokens() ||
       ! m_pTknVec->pGet(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! m_pTknVec->pGet(0)->bSame("/"_Stz) &&
     File::bExists(m_pTknVec->pGet(0)->szTerm()))
    return;
  
  // read more till ;
  vReadTillSemicolon();
  
  // stip off ;
  m_pTknVec->vStripSemicolon();
  
  // stip off &
  m_pTknVec->vStripAmpersand();
  
  // set no of tokens in token vector
  m_pTknVec->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* read tokens from stream till EOF                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReadTillEOF(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
    if (! m_pTknVec->nTokens() && bEOF()) {
      bool b = bPopCharser();
      ASSERT(b);
      break;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReadTillSemicolon(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(m_pTknVec->pGet(0)->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = m_pTknVec->iFindToken(";"_Stz);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_MAX_TOKENS);
    return;
  }
  
  i_Token = m_pTknVec->iCountTokens();
  
  // switch to terminal mode
  vLineMode(false);
  
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
  
  // switch back to line mode
  vLineMode(true);
  //vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Request : "_Stz,m_xTokenVec,C_MAX_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReadToken_nil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pTknVec->vReset();

  m_pTknVec->vAdvanceCurr();
  Token* pToken = m_pTknVec->pCurrToken();;

  vReadToken_vec();
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReadToken_vec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pToken = m_pTknVec->pCurrToken();;

  if (m_bNormal)
    vCollectNormal();
  else
    vCollectSen();
}

/*----------------------------------------------------------------------------*/
/* get EOF?                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Tokeniser::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  Stz* szSource = m_pCharser->szSource();
  
  return szSource;
}

/*----------------------------------------------------------------------------*/
/* get stdin?                                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bStdin = m_pCharser->bStdin();
  
  return bStdin;
}

/*----------------------------------------------------------------------------*/
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  Token* pCurrToken = m_pTknVec->pCurrToken();
  
  eTN eToken = pCurrToken->eToken();
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get token string                                                           */
/*----------------------------------------------------------------------------*/
Stz* Tokeniser::szToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);

  Token* pCurrToken = m_pTknVec->pCurrToken();
  
  return pCurrToken->szTerm();
}

/*----------------------------------------------------------------------------*/
/* get token string                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_pTknVec;
}

/*----------------------------------------------------------------------------*/
/* end use of source                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* use file                                                                   */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bOpened = m_pCharser->bUseFile(szPath);
  
  return bOpened;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vUseStdin();
}

/*----------------------------------------------------------------------------*/
/* use string                                                                 */
/*----------------------------------------------------------------------------*/
void Tokeniser::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
void Tokeniser::vCollectNormal(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char* azTermCrs = m_pToken->azTerm();
  m_pToken->vReset();

  vParserGetAnnCharStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *azTermCrs = '\0';
    m_pToken->vToken(TN_EOF);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // collect integer literal?
  if (bStartNormalInteger()) {
    vCollectNormalInteger();
  }
  
  // collect string literal?
  else if (bStartNormalString()) {
    vCollectNormalString();
  }
  
  // collect name token?
  else if (bStartNormalName()) {
    vCollectNormalName();
  }
  
  // collect other
  else {
    vCollectNormalOther();
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
/* collect normal integer literal                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());
  
  i8 iNoDigits = 0;
  
  // we have seen a digit
  ASSERT(bStartNormalInteger());
  
  vTokenCollectBegin();
  
  // - start parsing an integer
  
  // collect initial digits
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar;
    iNoDigits++;
    vParserGetAnnCharStripped();
  }
  
  // 0x hex number?
  if (1 == iNoDigits && m_xAnnChar.bIsX()) {
    
    // add the 'x'
    *z++ = m_xAnnChar.m_cChar;
    
    // collect the hex digits
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar;
      vParserGetAnnCharStripped();
    }
  }
  
  // push back the unconsumed character
  vParserUngetAnnCharStripped();
    
  // terminate the integer literal
  *z = '\0';
  
  m_pToken->vToken(TN_INTEGER);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect normal name                                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartNormalName());
  
  vTokenCollectBegin();
  
  // collect the characters of the name
  while ( StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar) ) {
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vParserUngetAnnCharStripped();
  
  // terminate the name
  *z = '\0';
  m_pToken->vToken(TN_NAME);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect other token                                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartNormalOther());
 
  vTokenCollectBegin();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather ]] as a token
  if (m_xAnnChar.bIsCloseBracket()) {
    vParserGetAnnCharStripped();
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
      vParserUngetAnnCharStripped();
      
      // set last single character as ]
      m_xAnnChar.m_cChar = ']';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather :: as a string
  if (m_xAnnChar.bIsColon()) {

    vParserGetAnnCharStripped();

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
      vParserUngetAnnCharStripped();

      // set last single character as :
      m_xAnnChar.m_cChar = ':';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather .. as a string
  if (m_xAnnChar.bIsDot()) {
    
    vParserGetAnnCharStripped();
    
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
      vParserUngetAnnCharStripped();
      
      // set last single character as .
      m_xAnnChar.m_cChar = '.';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather == as a token
  if (m_xAnnChar.bIsEqual()) {
    vParserGetAnnCharStripped();
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
      vParserUngetAnnCharStripped();
      
      // set last single character as =
      m_xAnnChar.m_cChar = '=';
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather >= as a token
  if (m_xAnnChar.bIsGreaterThan()) {
    vParserGetAnnCharStripped();
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
      vParserUngetAnnCharStripped();
      
      // set last single character as >
      m_xAnnChar.m_cChar = '>';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather <= as a token
  if (m_xAnnChar.bIsLessThan()) {
    vParserGetAnnCharStripped();
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
      vParserUngetAnnCharStripped();
      
      // set last single character as <
      m_xAnnChar.m_cChar = '<';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather [[ as a token
  if (m_xAnnChar.bIsOpenBracket()) {
    vParserGetAnnCharStripped();
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
      vParserUngetAnnCharStripped();
      
      // set last single character as [
      m_xAnnChar.m_cChar = '[';
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather || as a string
  if (m_xAnnChar.bIsPipe()) {
    
    vParserGetAnnCharStripped();
    
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
      vParserUngetAnnCharStripped();
      
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
/* collect normal string literal                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenString());
  
  vTokenCollectBegin();
  
  // skip the leading '"'
  vParserGetAnnCharStripped();
  
  // collect the characters within the string
  while ( ! bEndNormalString() ) {
    
    if (m_xAnnChar.bIsBackSlash()) {
      vParserGetAnnCharStripped();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
      else if ('r' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\r';
    }
    
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnCharStripped();
  }
  
  // terminate the string literal
  *z = '\0';
  
  m_pToken->vToken(TN_STRING);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen token                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSen(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char* azTermCrs = m_pToken->azTerm();
  
  m_pToken->vReset();

  vParserGetAnnCharStripped();
  
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
void Tokeniser::vCollectSenInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  i8 iNoDigits = 0;
  
  // we have seen a minus or a digit
  ASSERT(bStartSenInteger());
  
  vTokenCollectBegin();

  // - start parsing an integer
  
  // collect leading minus
  if (m_xAnnChar.bIsMinus()) {
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnCharStripped();
  }
  
  // collect initial digits
  while (m_xAnnChar.bIsDigit()) {
    *z++ = m_xAnnChar.m_cChar;
    iNoDigits++;
    vParserGetAnnCharStripped();
  }
  *z = '\0';

  // 0x hex number?
  if (1 == iNoDigits && m_xAnnChar.bIsX()) {

    // add the 'x'
    *z++ = m_xAnnChar.m_cChar;
    
    // collect the hex digits
    while (m_xAnnChar.bIsHex()) {
      *z++ = m_xAnnChar.m_cChar;
      vParserGetAnnCharStripped();
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
    vParserUngetAnnCharStripped();
    
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
void Tokeniser::vCollectSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());
  
  //ASSERT(bStartSenName());
  
  // collect the characters of the name
  while ( m_xAnnChar.m_cChar && !m_xAnnChar.bIsWhite() && !bStartSenString() ) {
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vParserUngetAnnCharStripped();
  
  // terminate a namelit
  *z = '\0';

  m_pToken->vToken(TN_NAME);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenString());
  
  vTokenCollectBegin();
  
  char cStartQuote = m_xAnnChar.m_cChar;
  
  // skip the leading '"' or '\''
  vParserGetAnnChar();
  
  // collect the chars in the string
  //while ( ! bEndSenString() ) {
  while ( m_xAnnChar.m_cChar != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,m_xAnnChar.m_cChar);
        
    if (m_xAnnChar.bIsBackSlash()) {
      vParserGetAnnChar();
      // translate \n into new line
      if ('n' == m_xAnnChar.m_cChar)
        m_xAnnChar.m_cChar = '\n';
    }    
    
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnChar();
  }
 
  // terminate the strlit
  *z = '\0';

  m_pToken->vToken(TN_STRING);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = StrLib::zNullTerminator(m_pToken->azTerm());

  ASSERT(bStartSenTag());
  
  vTokenCollectBegin();
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (! m_xAnnChar.m_cChar || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    // push back the unconsumed char
    vParserUngetAnnCharStripped();
    
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
    vParserGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vParserUngetAnnCharStripped();
  
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
void Tokeniser::vSkipWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  while (bHaveWhitespace())
    vParserGetAnnCharStripped();
}

/*----------------------------------------------------------------------------*/
/* have whitespace                                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bHaveWhitespace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  // do we have a current parsed character, which is a white char?
  if (m_xAnnChar.m_cChar) {
    if (StrLib::bIsWhite(m_xAnnChar.m_cChar))
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
  
  if (StrLib::bIsDigit(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal name                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (StrLib::bIsNameStartChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal other                                             */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalOther(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (! StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of normal string                                            */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_xAnnChar.bIsDoubleQuote())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen integer                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSenInteger(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_xAnnChar.bIsMinus() || StrLib::bIsDigit(m_xAnnChar.m_cChar))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen name                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSenName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (bStartSenInteger() || bStartSenString())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen string                                               */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
bool Tokeniser::bStartSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  //if (m_xAnnChar.bIsTilde())
  if (m_xAnnChar.bIsLessThan())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of Sen string                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
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
void Tokeniser::vParserGetAnnChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vGet(m_xAnnChar,false);
  m_xAnnChar.m_cChar = m_xAnnChar.m_cChar;
}

/*----------------------------------------------------------------------------*/
/* get next input character (stripped of comments)                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vParserGetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vGet(m_xAnnChar,true);
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vParserUngetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pCharser->vUnget(m_xAnnChar);
}

/*----------------------------------------------------------------------------*/
/* begin collecting a new token                                               */
/*----------------------------------------------------------------------------*/
void Tokeniser::vTokenCollectBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pToken->vLineNo(m_xAnnChar.m_iLine);
  m_pToken->vColNo(m_xAnnChar.m_iCol);
}

/*----------------------------------------------------------------------------*/
/* end collecting a token                                                     */
/*----------------------------------------------------------------------------*/
void Tokeniser::vTokenCollectEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* test for end of input                                                      */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_xAnnChar.bIsNull())
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
