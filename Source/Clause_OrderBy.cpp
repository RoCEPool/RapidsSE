/*============================================================================*/
/* Clause_OrderBy.cpp -- OrderBy Clause operand                               */
/*============================================================================*/

#include "FrontEnd.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(FrontEnd,Clause_OrderBy,CN_CLAUSE_ORDERBY)
__aNewFunction(Clause_OrderBy,CN_CLAUSE_ORDERBY)
__vDeleteFunction(Clause_OrderBy,CN_CLAUSE_ORDERBY)
__aFooterFunction(Clause_OrderBy)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_OrderBy::Clause_OrderBy(void)
: Clause(CN_CLAUSE_ORDERBY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_ORDERBY,_FOOTER_);

  m_sName = "orderBy"_Stz;
  
  _checkInitialised_(CN_CLAUSE_ORDERBY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_ORDERBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_OrderBy::~Clause_OrderBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_ORDERBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_OrderBy::bParse(Descend* pDescend, Stmt_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace(C_FMT_CLAUSE,"ORDERBY"_Stz,Stz::szTab(pSubQry->iAtDepth()),pCurTkn->szChars());
  pDescend->vNextToken();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
