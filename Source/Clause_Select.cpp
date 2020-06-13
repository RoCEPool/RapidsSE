/*============================================================================*/
/* Clause_Select.cpp -- Select Clause operand                                 */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Select,CN_CLAUSE_SELECT)
__aNewFunction(Clause_Select,CN_CLAUSE_SELECT)
__vDeleteFunction(Clause_Select,CN_CLAUSE_SELECT)
__aFooterFunction(Clause_Select)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Select::Clause_Select(void)
: Clause(CN_CLAUSE_SELECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SELECT,_FOOTER_);

  m_sName = "select"_Stz;
  
  _checkInitialised_(CN_CLAUSE_SELECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Select::~Clause_Select(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Select::bDescend(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_
  
  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("SELECT : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Select::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
