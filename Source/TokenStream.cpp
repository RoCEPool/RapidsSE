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
      
  vUseStdin();
    
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
/* input will be cpp tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenStream::vCppTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vCppTokens();
}

/*----------------------------------------------------------------------------*/
/* hard reset hard fork                                                       */
/*----------------------------------------------------------------------------*/
void TokenStream::vForkReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vForkReset();
}

/*----------------------------------------------------------------------------*/
/* inspect                                                                    */
/*----------------------------------------------------------------------------*/
void TokenStream::vInspect(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vInspect();
}

/*----------------------------------------------------------------------------*/
/* using stdin ?                                                              */
/*----------------------------------------------------------------------------*/
bool TokenStream::bIsStdIn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);

  bool bIsStdIn = m_pTokeniser->bIsStdin();
  
  return bIsStdIn;
}

/*----------------------------------------------------------------------------*/
/* set line mode                                                              */
/*----------------------------------------------------------------------------*/
void TokenStream::vLineMode(bool bLineMode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vLineMode(bLineMode);
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
//void TokenStream::vPushFile(Stz* szPath)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
//
//  m_pTokeniser->vPushCharser();
//
//  bool b = m_pTokeniser->bUseFile(szPath);
//  ASSERT(b);
//
//  m_pTokeniser->vLineMode(true);
//}
/*----------------------------------------------------------------------------*/
void TokenStream::vPushFile(Stz* szPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vPushFile(szPath);
}

/*----------------------------------------------------------------------------*/
/* read next command from token stream                                        */
/*----------------------------------------------------------------------------*/
void TokenStream::vReadCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vReadCommand();
}

/*----------------------------------------------------------------------------*/
/* read next token from stream                                                */
/*----------------------------------------------------------------------------*/
bool TokenStream::bReadToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  RASSERT(0); // called from vortex
  
//  m_pTokeniser->vReadToken_vec(pTknVec,pTkn);
//
//  eTN eToken = pTkn->eToken();
//
//  if (TN_EOF == eToken)
//    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* input will be sen tokens                                                   */
/*----------------------------------------------------------------------------*/
void TokenStream::vSenTokens(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vSenTokens();
}

/*----------------------------------------------------------------------------*/
/* read next token from stream                                                */
/*----------------------------------------------------------------------------*/
Tokeniser* TokenStream::pTokeniser(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);

  return m_pTokeniser;
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
/* use of token stream begins                                                 */
/*----------------------------------------------------------------------------*/
void TokenStream::vUseBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vUseBegin();
}

/*----------------------------------------------------------------------------*/
/* use of token stream ends                                                   */
/*----------------------------------------------------------------------------*/
void TokenStream::vUseEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* use stdin                                                                  */
/*----------------------------------------------------------------------------*/
void TokenStream::vUseStdin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOKENSTREAM,_FOOTER_);
  
  m_pTokeniser->vUseStdinX();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
