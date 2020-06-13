/*============================================================================*/
/* Clause_Limit.cpp -- Limit Clause operand                                   */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Limit,CN_CLAUSE_LIMIT)
__aNewFunction(Clause_Limit,CN_CLAUSE_LIMIT)
__vDeleteFunction(Clause_Limit,CN_CLAUSE_LIMIT)
__aFooterFunction(Clause_Limit)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Limit::Clause_Limit(void)
: Clause(CN_CLAUSE_LIMIT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_LIMIT,_FOOTER_);

  m_sName = "limit"_Stz;
  
  _checkInitialised_(CN_CLAUSE_LIMIT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_LIMIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Limit::~Clause_Limit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_LIMIT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Limit::bDescend(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("LIMIT : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Limit::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
