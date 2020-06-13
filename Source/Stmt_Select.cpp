/*============================================================================*/
/* Stmt_Select.cpp -- Select Statement operand                                */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Stmt_Select,CN_STMT_SELECT)
__aNewFunction(Stmt_Select,CN_STMT_SELECT)
__vDeleteFunction(Stmt_Select,CN_STMT_SELECT)
__aFooterFunction(Stmt_Select)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stmt_Select::Stmt_Select(void)
: Operand(CN_STMT_SELECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
  
  m_pTokenVec = new TokenVec(false);
  
  m_pSubQry = nullptr;

  _checkInitialised_(CN_STMT_SELECT);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stmt_Select::~Stmt_Select(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
  
  delete m_pTokenVec;
  
  delete m_pSubQry;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse statement                                                            */
/*----------------------------------------------------------------------------*/
bool Stmt_Select::bParse(TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
  
  Descend* pDescend = new Descend(pTokenVec);
  
  while (pDescend->bIsDispatchable())
    if (! pDescend->bDispatch())
      break;
  
  delete pDescend;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Stmt_Select::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
  
  return m_pTokenVec;
}


/*----------------------------------------------------------------------------*/
/* get subQry                                                                 */
/*----------------------------------------------------------------------------*/
Clause_SubQry* Stmt_Select::pSubQry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);

  return m_pSubQry;
}

/*----------------------------------------------------------------------------*/
/* put subQry                                                                 */
/*----------------------------------------------------------------------------*/
void Stmt_Select::vSubQry(Clause_SubQry* pSubQry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);

  m_pSubQry = pSubQry;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
