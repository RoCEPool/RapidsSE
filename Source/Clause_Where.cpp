/*============================================================================*/
/* Clause_Where.cpp -- Where Clause operand                                   */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Where,CN_CLAUSE_WHERE)
__aNewFunction(Clause_Where,CN_CLAUSE_WHERE)
__vDeleteFunction(Clause_Where,CN_CLAUSE_WHERE)
__aFooterFunction(Clause_Where)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Where::Clause_Where(void)
: Clause(CN_CLAUSE_WHERE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_WHERE,_FOOTER_);

  m_sName = "where"_Stz;
  
  _checkInitialised_(CN_CLAUSE_WHERE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_WHERE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Where::~Clause_Where(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_WHERE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Where::bParse(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace(C_FMT_CLAUSE,"WHERE"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
