/*============================================================================*/
/* Clause_OrderBy.cpp -- OrderBy Clause operand                               */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_OrderBy,CN_CLAUSE_ORDERBY)
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
bool Clause_OrderBy::bDescend(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("ORDERBY : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_OrderBy::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
