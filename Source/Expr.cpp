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
  
  RegSet* pRegSet = AM::pThisTLSRegSet();

  m_pCalculator = pRegSet->pCalculator();
  
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
  
  //delete m_pCalculator;
}

/*----------------------------------------------------------------------------*/
/* aggregate                                                                   */
/*----------------------------------------------------------------------------*/
i8 Expr::iAggregate(RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  i8 i = m_pCalculator->iAggregate(pDstRowSlab,aSrcRsb,pSrcRowSlab);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* group by aggregate                                                         */
/*----------------------------------------------------------------------------*/
i8 Expr::iAggregateGby(IndexTree* pIndexTree, RowSlab* pDstRowSlab, Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  i8 i = m_pCalculator->iAggregateGby(pIndexTree,pDstRowSlab,aSrcRsb,pSrcRowSlab);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
void Expr::vGenerateAggGby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  m_pCalculator->vGenerateAggGby();
}

/*----------------------------------------------------------------------------*/
/* compile                                                                    */
/*----------------------------------------------------------------------------*/
bool Expr::bCompile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  TokenVec* pTknVec = TokenVec::pTokenise(m_pCalculator->szSource());
  
  bool b = m_pCalculator->bCompile(pTknVec);
  
  delete pTknVec;
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* evaluate                                                                   */
/*----------------------------------------------------------------------------*/
i8 Expr::iFilter(Rsb* aRsb, RowSlab* pRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
    
  i8 i = m_pCalculator->iFilter(aRsb,pRowSlab);
  
  return i;
}

/*----------------------------------------------------------------------------*/
/* evaluate                                                                   */
/*----------------------------------------------------------------------------*/
i8 Expr::iProject(Pot* pPot, RowSlab* pDstRowSlab,
                  Rsb* aSrcRsb, RowSlab* pSrcRowSlab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
    
  i8 i = m_pCalculator->iProject(pPot,pDstRowSlab,aSrcRsb,pSrcRowSlab);
  
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
  
  return m_pCalculator->szSource();
}

/*----------------------------------------------------------------------------*/
/* set source                                                                 */
/*----------------------------------------------------------------------------*/
void Expr::vSource(Stz* szSource)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_EXPR,_FOOTER_);
  
  m_pCalculator->vSource(szSource);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
