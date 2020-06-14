/*============================================================================*/
/* Clause_From.cpp -- From Clause operand                                     */
/*============================================================================*/

#include "FrontEnd.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_From,CN_CLAUSE_FROM)
__aNewFunction(Clause_From,CN_CLAUSE_FROM)
__vDeleteFunction(Clause_From,CN_CLAUSE_FROM)
__aFooterFunction(Clause_From)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_From::Clause_From(void)
: Clause(CN_CLAUSE_FROM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_FROM,_FOOTER_);
  
  m_sName = "from"_Stz;
    
  _checkInitialised_(CN_CLAUSE_FROM);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_FROM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_From::~Clause_From(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_FROM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_From::bParse(Descend* pDescend, Stmt_SubQry* pSubQry)
{
  _static_
  
  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace(C_FMT_CLAUSE,"FROM"_Stz,Stz::szTab(pSubQry->iAtDepth()),pCurTkn->szChars());
  pDescend->vNextToken();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
