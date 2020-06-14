/*============================================================================*/
/* Clause_Join.cpp -- Join Clause operand                                     */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Join,CN_CLAUSE_JOIN)
__aNewFunction(Clause_Join,CN_CLAUSE_JOIN)
__vDeleteFunction(Clause_Join,CN_CLAUSE_JOIN)
__aFooterFunction(Clause_Join)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Join::Clause_Join(void)
: Clause(CN_CLAUSE_JOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);

  m_sName = "join"_Stz;
  
  _checkInitialised_(CN_CLAUSE_JOIN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Join::~Clause_Join(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Join::bParse(Descend* pDescend, Stmt_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace(C_FMT_CLAUSE,"JOIN"_Stz,Stz::szTab(pSubQry->iAtDepth()),pCurTkn->szChars());
  pDescend->vNextToken();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
