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
  m_pTokeniser->vPushCharser();
  
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
  
  m_pTokeniser->vPushCharser();
  
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
  
  m_pTokeniser->vReadCommand();
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
/* read next token from stream                                                */
/*----------------------------------------------------------------------------*/
TokenVec* TokenStream::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);

  TokenVec* pTknVec = m_pTokeniser->pTokenVec();
  
  return pTknVec;
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
