/*============================================================================*/
/* TokenStream.cpp -- Token Stream Processor                                  */
/*============================================================================*/

#include "LexicalAnalysis.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(LexicalAnalysis,TokenStream,CN_TOKENSTREAM)
__aNewFunction(TokenStream,CN_TOKENSTREAM)
__vDeleteFunction(TokenStream,CN_TOKENSTREAM)
__aFooterFunction(TokenStream)

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
/* using stdin ?                                                              */
/*----------------------------------------------------------------------------*/
bool TokenStream::bIsStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);

  bool bIsStdIn = m_pTokeniser->bStdin();
  
  return bIsStdIn;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void TokenStream::vPushFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vPushParser();
  
  bool b = m_pTokeniser->bUseFile(szPath);
  ASSERT(b);
  
  m_pTokeniser->vLineMode(true);
}

/*----------------------------------------------------------------------------*/
/* read next command from token stream                                        */
/*----------------------------------------------------------------------------*/
void TokenStream::vReadCommand(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  vReadTillEOF(pTknVec);
    
  // if empty line just return
  if ( ! pTknVec->nTokens() ||
       ! pTknVec->pGet(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! pTknVec->pGet(0)->bSame("/"_Stz) &&
     File::bExists(pTknVec->pGet(0)->szTerm()))
    return;
  
  // read more till ;
  vReadTillSemicolon(pTknVec);
  
  // stip off ;
  pTknVec->vStripSemicolon();
  
  // stip off &
  pTknVec->vStripAmpersand();
  
  // set no of tokens in token vector
  pTknVec->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* read tokens from stream till EOF                                           */
/*----------------------------------------------------------------------------*/
void TokenStream::vReadTillEOF(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  while (true) {
    
    pTknVec->vReset();
    
    pTknVec->vSource(m_pTokeniser->szSource());
    for (;;) {
      
      pTknVec->vAdvanceCurr();
      Token* pToken = pTknVec->pGet(pTknVec->nTokens(),true);
            
//      if (! bNextInputToken(pTknVec,pToken))
//        break;
      
      m_pTokeniser->vReadToken_vec(pTknVec,pToken);
      if (TN_EOF == pToken->eToken())
        break;

      pTknVec->vIncTokens();
      ASSERT(pTknVec->nTokens() < C_MAX_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! pTknVec->nTokens() && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPopParser();
      ASSERT(b);
      break;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void TokenStream::vReadTillSemicolon(TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(pTknVec->pGet(0)->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = pTknVec->iFindToken(";"_Stz);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_MAX_TOKENS);
    return;
  }
  
  i_Token = pTknVec->iCountTokens();
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_MAX_TOKENS; i_Token++) {
    
    Token* pToken = pTknVec->pGet(i_Token,true);
    
    m_pTokeniser->vReadToken_vec(pTknVec,pToken);
    ASSERT(TN_EOF != pToken->eToken());
    
    // look for terminal
    if (TN_OTHER == pToken->eToken() && pToken->bSame(";"_Stz))
      break;

    pTknVec->vAdvanceCurr();
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  //vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Request : "_Stz,m_xTokenVec,C_MAX_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* read next token from stream                                                */
/*----------------------------------------------------------------------------*/
bool TokenStream::bReadToken(TokenVec* pTknVec, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vReadToken_vec(pTknVec,pTkn);
  
  eTN eToken = pTkn->eToken();
  
  if (TN_EOF == eToken)
    return false;

  return true;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
