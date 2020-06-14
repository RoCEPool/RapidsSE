/*============================================================================*/
/* Clause_Project.cpp -- From Clause operand                                  */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Project,CN_CLAUSE_PROJECT)
__aNewFunction(Clause_Project,CN_CLAUSE_PROJECT)
__vDeleteFunction(Clause_Project,CN_CLAUSE_PROJECT)
__aFooterFunction(Clause_Project)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Project::Clause_Project(void)
: Clause(CN_CLAUSE_PROJECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_PROJECT,_FOOTER_);
  
  m_sName = "project"_Stz;
    
  _checkInitialised_(CN_CLAUSE_PROJECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Project::~Clause_Project(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_PROJECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Project::bParse(Descend* pDescend, Stmt_SubQry* pSubQry)
{
  _static_
  
  Token* pCurTkn = pDescend->pCurrToken();
  
  vTrace(C_FMT_CLAUSE,"PROJECT"_Stz,pCurTkn->szChars());
  pDescend->vNextToken();
  
  vCollect(pDescend,pSubQry);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
