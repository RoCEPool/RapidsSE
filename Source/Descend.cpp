/*============================================================================*/
/* Descend.cpp -- Token Vector Descender                                      */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Descend,CN_DESCEND)
__aNewFunction(Descend,CN_DESCEND)
__vDeleteFunction(Descend,CN_DESCEND)
__aFooterFunction(Descend)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Descend::Descend(TokenVec* pArgVec)
: Object(CN_DESCEND)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_pTokenVec = pArgVec;
    
  _checkInitialised_(CN_DESCEND);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Descend::~Descend(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  //delete m_pTokenVec;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
i8 Descend::iCurr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  i8 iCurr = m_pTokenVec->iCurrToken();
  
  return iCurr;
}

/*----------------------------------------------------------------------------*/
/* get current token                                                          */
/*----------------------------------------------------------------------------*/
Token* Descend::pCurr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  Token* pCurr = m_pTokenVec->pCurrToken();
  
  return pCurr;
}

/*----------------------------------------------------------------------------*/
/* set to and get first token                                                 */
/*----------------------------------------------------------------------------*/
Token* Descend::pFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  vFirst();
  
  Token* pFirst = m_pTokenVec->pCurrToken();
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* set to first token                                                         */
/*----------------------------------------------------------------------------*/
void Descend::vFirst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  m_pTokenVec->vCurrToken(0);
}

/*----------------------------------------------------------------------------*/
/* set to and get last token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  vLast();
  
  Token* pLast = m_pTokenVec->pCurrToken();
  
  return pLast;
}

/*----------------------------------------------------------------------------*/
/* set to last token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);

  i8 nTokens = m_pTokenVec->nTokens();
  
  m_pTokenVec->vCurrToken(nTokens - 1);
}

/*----------------------------------------------------------------------------*/
/* set to and get next token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pNext = m_pTokenVec->pCurrToken();
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* set to next token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_pTokenVec->vNextToken();
}

/*----------------------------------------------------------------------------*/
/* set to and get next token                                                  */
/*----------------------------------------------------------------------------*/
Token* Descend::pPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  Token* pPrev = m_pTokenVec->pPrevToken();
  
  return pPrev;
}

/*----------------------------------------------------------------------------*/
/* set to next token                                                          */
/*----------------------------------------------------------------------------*/
void Descend::vPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DESCEND,_FOOTER_);
  
  m_pTokenVec->vPrevToken();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
