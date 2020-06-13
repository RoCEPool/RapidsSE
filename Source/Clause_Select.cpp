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
bool Clause_Select::bParse(Descend* pDescend, Clause_SubQry* pParentSubQry)
{
  _static_
  
  Clause_SubQry* pSubQry = new Clause_SubQry;

  Stmt_Select* pSelectStmt = _pStmt_Select(pDescend->pStmt());
  if (! pSelectStmt->pSubQry()) {
    pSelectStmt->vSubQry(pSubQry);
  }
  
  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("%s SELECT : %s\n"_Stz,pSubQry->szSubQryNbr(),pCurTkn->szChars());
  pDescend->vNext();
  
  Clause_Project::bParse(pDescend,pSubQry);

  bool bSubQry;
  while (pDescend->bMore() && pDescend->bIsDescendParse(bSubQry))
    if (! pDescend->bCallFunc(pSubQry))
      break;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
