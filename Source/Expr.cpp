/*============================================================================*/
/* Expr.cpp -- Expression Evaluator                                           */
/*============================================================================*/

#include "Expressions.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Expressions,Expr,CN_EXPR)
__aNewFunction(Expr,CN_EXPR)
__vDeleteFunction(Expr,CN_EXPR)
__aFooterFunction(Expr)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Expr::Expr(void)
: Object(CN_EXPR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  m_eKind = UK_NONE;
  
  // m_sSource
  m_pDynFun = nullptr;
  
  m_pCalculator = new Calculator;
  
  _checkInitialised_(CN_EXPR);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Expr::~Expr(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  delete m_pDynFun;
  
  delete m_pCalculator;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Expr::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  TokenVec* pTknVec = TokenVec::pTokenise(m_sSource);
  
  bool b = m_pCalculator->bCompileTest(pTknVec);
  
  delete pTknVec;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* evaluate                                                                    */
/*----------------------------------------------------------------------------*/
i8 Expr::iEvaluate(Rsb* aRsb, RowSlab* pRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
    
  i8 i = m_pCalculator->iEvaluate(aRsb,pRowSlab);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* get function kind                                                          */
/*----------------------------------------------------------------------------*/
eUK Expr::eKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  return m_eKind;
}

/*----------------------------------------------------------------------------*/
/* set function kind                                                          */
/*----------------------------------------------------------------------------*/
void Expr::vKind(eUK eKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  m_eKind = eKind;
}

/*----------------------------------------------------------------------------*/
/* get source                                                                 */
/*----------------------------------------------------------------------------*/
Stz* Expr::szSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  return m_sSource;
}

/*----------------------------------------------------------------------------*/
/* set source                                                                 */
/*----------------------------------------------------------------------------*/
void Expr::vSource(Stz* szSource)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  m_sSource = szSource;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
