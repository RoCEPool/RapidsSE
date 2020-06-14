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
bool Clause_Select::bParse(Descend* pDescend, Stmt_SubQry* pParentSubQry)
{
  _static_
  
  // begin new sub query
  Stmt_SubQry* pSubQry = new Stmt_SubQry;
  if (pParentSubQry) {
    Stz* szQryNbr = pParentSubQry->szSubQryNbr();
    pSubQry->vParentSubQry(pParentSubQry);
    i8 iSubDepth = pParentSubQry->iAtDepth() + 1;
    pSubQry->vAtDepth(iSubDepth);
  }
  else {
    pSubQry->vAtDepth(0);
  }
  
  // does the statement have a root sub-query?
  Stmt_Select* pSelectStmt = _pStmt_Select(pDescend->pStmt());
  if (! pSelectStmt->pRootSubQry()) {
    pSelectStmt->vRootSubQry(pSubQry);
  }
  
  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace("%s) SELECT : %s\n"_Stz,pSubQry->szSubQryNbr(),pCurTkn->szChars());
  pDescend->vNextToken();
  
  Clause_Project::bParse(pDescend,pSubQry);

  bool bSubQry;
  while (pDescend->bMoreTokens() && pDescend->bIsDescendParse(bSubQry))
    if (! pDescend->bCallFunc(pSubQry))
      break;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
