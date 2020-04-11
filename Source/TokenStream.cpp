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
void TokenStream::vReadCommand(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  vReadTillEOF(pTknVec_arg);
    
  // if empty line just return
  if ( ! pTknVec_arg->nTokens() ||
       ! pTknVec_arg->pGet(0)->bHasText() )
    return;
  
  // if its a command file just return
  if ( ! pTknVec_arg->pGet(0)->bSame("/"_Stz) &&
     File::bExists(pTknVec_arg->pGet(0)->szTerm()))
    return;
  
  // collect till ;
  m_pTokeniser->vReadTillSemicolon(pTknVec_arg);
  
  // stip off ;
  pTknVec_arg->vStripSemicolon();
  
  // stip off &
  pTknVec_arg->vStripAmpersand();
  
  // set no of tokens in token vector
  pTknVec_arg->vSetTokens();
}

/*----------------------------------------------------------------------------*/
/* read tokens from stream till EOF                                           */
/*----------------------------------------------------------------------------*/
void TokenStream::vReadTillEOF(TokenVec* pTknVec_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  while (true) {
    
    pTknVec_arg->vReset();
    
    pTknVec_arg->vSource(m_pTokeniser->szSource());
    for (;;) {
      
      pTknVec_arg->vAdvanceCurr();
      Token* pToken = pTknVec_arg->pGet(pTknVec_arg->nTokens(),true);
            
//      if (! bNextInputToken(pTknVec_arg,pToken))
//        break;
      
      m_pTokeniser->vNextToken_z(pTknVec_arg,pToken);
      if (TN_EOF == pToken->eToken())
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
/* read next token from stream                                                */
/*----------------------------------------------------------------------------*/
bool TokenStream::bReadToken(TokenVec* pTknVec_arg, Token* pTkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vNextToken_z(pTknVec_arg,pTkn);
  
  eTN eToken = pTkn->eToken();
  
  if (TN_EOF == eToken)
    return false;

  return true;
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
