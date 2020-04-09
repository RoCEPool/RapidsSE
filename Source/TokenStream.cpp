/*============================================================================*/
/* TokenStream.cpp -- TokenStream Processor                                               */
/*============================================================================*/

#include "LexicalAnalysis.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TokenStream* LexicalAnalysis::_pTokenStream(void* aAddr)
{
  TokenStream* pTokenStream = reinterpret_cast<TokenStream*>(aAddr);
  
  RASSERT_TYPE(pTokenStream,CN_TOKENSTREAM);
  
  return pTokenStream;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TokenStream::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_TOKENSTREAM);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TokenStream::operator delete(void* aAddr)
{
  TokenStream* pTokenStream = reinterpret_cast<TokenStream*>(aAddr);
  
  HeapMem::vFree(pTokenStream);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TokenStream::TokenStream(void)
: Object(CN_TOKENSTREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  

  m_pTokeniser = new Tokeniser;
  
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPushParser();
  
  //if (gxArgs.m_bPrompt)
     m_pTokeniser->vUseStdin();
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);

    
  _checkInitialised_(CN_TOKENSTREAM);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TokenStream::~TokenStream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  delete m_pTokeniser;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* potentially execute a command file                                         */
/*----------------------------------------------------------------------------*/
bool TokenStream::bExecuteFile(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  if (! bTestFile(pTknVec_arg->pGet(0)->szTerm()))
    return false;
  
  bool b = bRunFile(pTknVec_arg);
  //ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool TokenStream::bRunFile(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
//  if ( ! bTokenCheck(1) )
//    if (! pTkn(1)->bSame(";"_Stz))
//      return false;
  
  vPushFile(pTknVec_arg->pGet(0)->szTerm());
  m_pTokeniser->vLineMode(true);
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void TokenStream::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPushParser();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool TokenStream::bTokenCheck(TokenVec* pTknVec_arg, i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  if (pTknVec_arg->pGet(nTokens)->bHasText()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for blank line                                                        */
/*----------------------------------------------------------------------------*/
bool TokenStream::bBlankLine(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  if (! pTknVec_arg->nTokens() ||
      0 == pTknVec_arg->pGet(0)->iLength())
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void TokenStream::vGetCommand(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  vGetTokens(pTknVec_arg);
    
  // if empty line just return
  if ( ! pTknVec_arg->nTokens() ||
       ! pTknVec_arg->pGet(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! pTknVec_arg->pGet(0)->bSame("/"_Stz) &&
      bTestFile(pTknVec_arg->pGet(0)->szTerm()))
    return;
  
  // collect till ;
  m_pTokeniser->vCollectStatement(pTknVec_arg);
  
  // stip off ;
  pTknVec_arg->vStripSemicolon();
  
  // stip off &
  pTknVec_arg->vStripAmpersand();
  
  // set no of tokens in token vector
  pTknVec_arg->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* get next token from input                                                  */
/*----------------------------------------------------------------------------*/
bool TokenStream::bGetToken(Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vNextToken(pTkn);
  
  eTN eToken = pTkn->eToken();
  
  if (TN_EOF == eToken)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* get command tokens from input                                              */
/*----------------------------------------------------------------------------*/
void TokenStream::vGetTokens(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  while (true) {
    
    pTknVec_arg->vReset();
    
    pTknVec_arg->vSource(m_pTokeniser->szSource());
    for (;;) {
      
      Token* pToken = pTknVec_arg->pGet(pTknVec_arg->nTokens(),true);
      
//      eTN eToken = m_pTokeniser->eNextToken((char*)pToken->szText());
//
//      if (TN_EOF == eToken)
//        break;
//
//      pToken->vToken(eToken);
      
      
      //      vTrace("vGetCommand : token %lld ]%s[\n"_Stz,
      //             pTknVec_inp()->nTokens(),
      //             pTkn(pTknVec_inp()->nTokens())->aText());
      
      if (! bGetToken(pToken))
        break;
      
      pTknVec_arg->vIncTokens();
      ASSERT(pTknVec_arg->nTokens() < C_MAX_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! pTknVec_arg->nTokens() && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPopParser();
      ASSERT(b);
      break;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool TokenStream::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  if (File::bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* tokenize string                                                            */
/*----------------------------------------------------------------------------*/
void TokenStream::vTokenise(Stz* aCommand, TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  s48K sCommand(aCommand);
  
  m_pTokeniser->vPushParser();
  m_pTokeniser->vUseString(sCommand);
  
  vGetCommand(pTknVec_arg);
  m_pTokeniser->bPopParser();
}

/*----------------------------------------------------------------------------*/
/* get tokeniser                                                              */
/*----------------------------------------------------------------------------*/
Tokeniser* TokenStream::pTokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  return m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void TokenStream::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vUseStdin();
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);
}

/*----------------------------------------------------------------------------*/
/* using stdin ?                                                              */
/*----------------------------------------------------------------------------*/
bool TokenStream::bStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);

  bool bIsStdIn = m_pTokeniser->bStdin();
  
  return bIsStdIn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(TokenStream);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
