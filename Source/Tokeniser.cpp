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
  
  m_pParser = nullptr;
  m_bMalloc = false;
  m_bNormal = false;
  
  m_xAnnChar.vReset();
    
  m_oToken.vReset();
  
  _checkInitialised_(CN_TOKENISER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tokeniser::~Tokeniser(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  while (m_pParser)
    RASSERT(bPopParser());
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* replace tokens                                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vReplace(TokenVec* pThisVec,
                              TokenVec* pFindVec, TokenVec* pWithVec)
{
  _static_
  
  i8 nTokens = pThisVec->nTokens();
  for (i8 i_Token = 0; i_Token < nTokens; i_Token++) {
    
    Token* pThisToken = pThisVec->pGet(i_Token);
    
    i8 i_Find = pFindVec->iFindToken(pThisToken->szTerm());
    
    if (C_UNKNOWN == i_Find)
      continue;
    
    Token* pWithToken = pWithVec->pGet(i_Find);

    pThisToken->vCopy(pWithToken->szTerm());
  }
}

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pSubVec(TokenVec* pTokenVec, i8 i_From, i8 i_To)
{
  _static_
  
  TokenVec* pExtractedVec = new TokenVec;

  i8 nTokens = pTokenVec->nTokens();
  
  i8 i_Token;
  for (i_Token = i_From; i_Token < nTokens && i_Token <= i_To; i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);
    pExtractedVec->vAppendToken(pToken->szTerm());
  }
  
  return pExtractedVec;
}

/*----------------------------------------------------------------------------*/
/* extract sub vector of tokens                                               */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pSubVec(TokenVec* pTokenVec, Stz* szStart, Stz* szEnd)
{
  _static_

  i8 nTokens = pTokenVec->nTokens();
  
  i8 i_Token;
  for (i_Token = 0; i_Token < nTokens; i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);
    if (pToken->bSame(szStart)) {
      break;
    }
  }

  if (i_Token >= nTokens)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  TokenVec* pExtractedVec = new TokenVec;

  for (i_Token++; i_Token < nTokens; i_Token++) {
    Token* pToken = pTokenVec->pGet(i_Token);
    if (pToken->bSame(szEnd))
      break;
    
    pExtractedVec->vAppendToken(pToken->szTerm());
  }
  
  return pExtractedVec;
}

/*----------------------------------------------------------------------------*/
/* tokenise given string                                                      */
/*----------------------------------------------------------------------------*/
TokenVec* Tokeniser::pTokenise(Stz* szString)
{
  _static_
  
  TokenVec* pTokenVec = new TokenVec;
  
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPushParser();
  pTokeniser->vUseString(szString);
  
  for (i8 i_Token = 0; ; i_Token++) {
    pTokeniser->vNextToken();
    if (TN_EOF == pTokeniser->eToken())
      break;
    
    pTokenVec->vIncTokens();
    
    Token* pToken = pTokenVec->pGet(i_Token);
    pToken->vToken(pTokeniser->eToken());
    pToken->vCopy(pTokeniser->szToken());
    
  }
  
  delete pTokeniser;
  
  return pTokenVec;
}

/*----------------------------------------------------------------------------*/
/* tokenise given string into given TokenVec                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vTokenise(TokenVec* pTokenVec, Stz* szString)
{
  _static_
  
  pTokenVec->vReset();
  
  Tokeniser* pTokeniser = new Tokeniser;
  pTokeniser->vPushParser();
  pTokeniser->vUseString(szString);
  
  for (i8 i_Token = 0; ; i_Token++) {
    pTokeniser->vNextToken();
    if (TN_EOF == pTokeniser->eToken())
      break;
    
    pTokenVec->vIncTokens();
    
    Token* pToken = pTokenVec->pGet(i_Token);
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
    
    pEnameSet->vAdd(pTknVec->pGet(i_Name)->szTerm(),
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
    
    pEnameSet->vAdd(pTknVec->pGet(i_Name)->szTerm(),
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
         pTokeniser->m_pParser,
         pTokeniser->m_bMalloc == true ? "malloc"_Stz : "pool"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectStatement(TokenVec* pTokenVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(pTokenVec_arg->pGet(0)->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = pTokenVec_arg->iFindToken(";"_Stz);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_MAX_TOKENS);
    return;
  }
  
  i_Token = pTokenVec_arg->iCountTokens();
  
  // switch to terminal mode
  vLineMode(false);
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_MAX_TOKENS; i_Token++) {
    
    Token* pToken = (*pTokenVec_arg)[i_Token];
    vNextToken(pToken);
    ASSERT(TN_EOF != pToken->eToken());
    
    // look for terminal
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz))
      break;
  }
  
  // switch back to line mode
  vLineMode(true);
  //vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Request : "_Stz,m_xTokenVec,C_MAX_TOKENS);
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
/* parse next token                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vNextToken(&m_oToken);
}

/*----------------------------------------------------------------------------*/
/* parse next token into given buffer                                         */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextToken(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (m_bNormal)
    vNextNormal(pToken);
  else
    vNextSen(pToken);
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
bool Tokeniser::bPopParser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if (! m_pParser)
    return false;
  
  Characterise* pToBeDelete = m_pParser;
  m_pParser = m_pParser->pNext();
  
  delete pToBeDelete;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push a parser                                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vPushParser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser = new(m_bMalloc) Characterise(m_pParser);
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
/* get token type                                                             */
/*----------------------------------------------------------------------------*/
eTN Tokeniser::eToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  eTN eToken = m_oToken.eToken();
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get token string                                                           */
/*----------------------------------------------------------------------------*/
Stz* Tokeniser::szToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  return m_oToken.szTerm();
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
bool Tokeniser::bUseFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  bool bOpened = m_pParser->bUseFile(szPath);
  
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
void Tokeniser::vUseString(Stz* szString)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUseString(szString);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse next normal token into given buffer                                  */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextNormal(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  pToken->vReset();
  
  vCollectNormal(pToken,pToken->azTerm());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cross check valid normal tokens
  switch (pToken->eToken()) {
      
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
/* parse next Sen token into given buffer                                     */
/*----------------------------------------------------------------------------*/
void Tokeniser::vNextSen(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  pToken->vReset();

  vCollectSen(pToken,pToken->azTerm());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // cross check valid SEN tokens
  switch (pToken->eToken()) {
      
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
/*-- token processing --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect normal token                                                       */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormal(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vParserGetAnnCharStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *azTermCrs = '\0';
    pToken->vToken(TN_EOF);
    return;
  }
  
  // collect integer literal?
  if (bStartNormalInteger()) {
    vCollectNormalInteger(pToken,azTermCrs);
    return;
  }
  
  // collect string literal?
  if (bStartNormalString()) {
    vCollectNormalString(pToken,azTermCrs);
    return;
  }
  
  // collect name token?
  if (bStartNormalName()) {
    vCollectNormalName(pToken,azTermCrs);
    return;
  }
  
  vCollectNormalOther(pToken,azTermCrs);
}

/*----------------------------------------------------------------------------*/
/* collect normal integer literal                                             */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalInteger(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  i8 iNoDigits = 0;
  char *z = azTermCrs;
  
  // we have seen a digit
  ASSERT(bStartNormalInteger());
  
  vTokenCollectBegin(pToken);
  
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
  
  pToken->vToken(TN_INTEGER);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect normal name                                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalName(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
  ASSERT(bStartNormalName());
  
  vTokenCollectBegin(pToken);
  
  // collect the characters of the name
  while ( StrLib::bIsNameBodyChar(m_xAnnChar.m_cChar) ) {
    *z++ = m_xAnnChar.m_cChar;
    vParserGetAnnCharStripped();
  }
  
  // push back the unconsumed char
  vParserUngetAnnCharStripped();
  
  // terminate the name
  *z = '\0';
  pToken->vToken(TN_NAME);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect other token                                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalOther(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
  ASSERT(bStartNormalOther());
 
  vTokenCollectBegin(pToken);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // gather ]] as a token
  if (m_xAnnChar.bIsCloseBracket()) {
    vParserGetAnnCharStripped();
    if (m_xAnnChar.bIsCloseBracket()) {
      *z++ = ']';
      *z++ = ']';
      *z = '\0';
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
      pToken->vToken(TN_STRING);
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
  pToken->vToken(TN_OTHER);

  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect normal string literal                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectNormalString(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
  ASSERT(bStartSenString());
  
  vTokenCollectBegin(pToken);
  
  // skip the leading '"'
  vParserGetAnnCharStripped();
  
  // collect the characters within the string
  while ( ! bEndNormalString() ) {
    
    if ('\\' == m_xAnnChar.m_cChar) {
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
  
  pToken->vToken(TN_STRING);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen token                                                          */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSen(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  vParserGetAnnCharStripped();
  
  vSkipWhitespace();
  
  // end of input?
  if (bEndInput()) {
    *azTermCrs = '\0';
    pToken->vToken(TN_EOF);
    return;
  }
  
  // collect integer literal?
  if (bStartSenInteger()) {
    vCollectSenInteger(pToken,azTermCrs);
    return;
  }
  
  // collect string literal?
  if (bStartSenString()) {
    vCollectSenString(pToken,azTermCrs);
    return;
  }
  
  // collect name token
  vTokenCollectBegin(pToken);
  vCollectSenName(pToken,azTermCrs);
}

/*----------------------------------------------------------------------------*/
/* collect Sen integer                                                        */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenInteger(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  i8 iNoDigits = 0;
  char *z = azTermCrs;
  
  // we have seen a minus or a digit
  ASSERT(bStartSenInteger());
  
  vTokenCollectBegin(pToken);

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
    vCollectSenName(pToken,z);
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
    pToken->vToken(TN_INTEGER);
    vTokenCollectEnd();
    return;
  }
  
  // its actually a name; complete parsing name
  vCollectSenName(pToken,z);
}

/*----------------------------------------------------------------------------*/
/* collect Sen name                                                           */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenName(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
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

  pToken->vToken(TN_NAME);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen string                                                         */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenString(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
  ASSERT(bStartSenString());
  
  vTokenCollectBegin(pToken);
  
  char cStartQuote = m_xAnnChar.m_cChar;
  
  // skip the leading '"' or '\''
  vParserGetAnnChar();
  
  // collect the chars in the string
  //while ( ! bEndSenString() ) {
  while ( m_xAnnChar.m_cChar != cStartQuote ) {
  
    //vTrace(">>%c<<\n"_Stz,m_xAnnChar.m_cChar);
        
    if ('\\' == m_xAnnChar.m_cChar) {
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

  pToken->vToken(TN_STRING);
  
  vTokenCollectEnd();
}

/*----------------------------------------------------------------------------*/
/* collect Sen tag                                                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vCollectSenTag(Token* pToken, char* azTermCrs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  char *z = azTermCrs;
  
  ASSERT(bStartSenString());
  
  vTokenCollectBegin(pToken);
  
  char cStartOpenAngle = m_xAnnChar.m_cChar;

  // skip the leading <
  vParserGetAnnChar();
  
  if (! m_xAnnChar.m_cChar || m_xAnnChar.bIsWhite() || bStartSenString() ) {
    // push back the unconsumed char
    vParserUngetAnnCharStripped();
    
    // terminate a namelit
    *z = '\0';

    pToken->vToken(TN_NAME);
    
    vTokenCollectEnd();

    return;
  }
     
  // terminate the tag
  *z = '\0';

  pToken->vToken(TN_NAME);
  
  vTokenCollectEnd();
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
  
  if ('"' == m_xAnnChar.m_cChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of normal string                                              */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndNormalString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_xAnnChar.m_cChar)
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
  
  if ('"' == m_xAnnChar.m_cChar || '\'' == m_xAnnChar.m_cChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for start of Sen tag                                                  */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bStartSenTag(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('<' == m_xAnnChar.m_cChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* test for end of Sen string                                                 */
/*----------------------------------------------------------------------------*/
bool Tokeniser::bEndSenString(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  if ('"' == m_xAnnChar.m_cChar || '\'' == m_xAnnChar.m_cChar)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get next character from parser input                                       */
/*----------------------------------------------------------------------------*/
void Tokeniser::vParserGetAnnChar(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vGet(m_xAnnChar,false);
  m_xAnnChar.m_cChar = m_xAnnChar.m_cChar;
}

/*----------------------------------------------------------------------------*/
/* get next input character (stripped of comments)                            */
/*----------------------------------------------------------------------------*/
void Tokeniser::vParserGetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vGet(m_xAnnChar,true);
}

/*----------------------------------------------------------------------------*/
/* unget current input character                                              */
/*----------------------------------------------------------------------------*/
void Tokeniser::vParserUngetAnnCharStripped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  m_pParser->vUnget(m_xAnnChar);
}

/*----------------------------------------------------------------------------*/
/* begin collecting a new token                                               */
/*----------------------------------------------------------------------------*/
void Tokeniser::vTokenCollectBegin(Token* pToken)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENISER,_FOOTER_);
  
  pToken->vLineNo(m_xAnnChar.m_iLine);
  pToken->vColNo(m_xAnnChar.m_iCol);
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
  
  if ('\0' == m_xAnnChar.m_cChar)
    return true;
  
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
