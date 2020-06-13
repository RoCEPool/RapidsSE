/*============================================================================*/
/* Clause_GroupBy.cpp -- GroupBy Clause operand                               */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_GroupBy,CN_CLAUSE_GROUPBY)
__aNewFunction(Clause_GroupBy,CN_CLAUSE_GROUPBY)
__vDeleteFunction(Clause_GroupBy,CN_CLAUSE_GROUPBY)
__aFooterFunction(Clause_GroupBy)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_GroupBy::Clause_GroupBy(void)
: Clause(CN_CLAUSE_GROUPBY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);

  m_sName = "groupBy"_Stz;
  
  _checkInitialised_(CN_CLAUSE_GROUPBY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_GroupBy::~Clause_GroupBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_GroupBy::bParse(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace(C_FMT_CLAUSE,"GROUPBY"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
