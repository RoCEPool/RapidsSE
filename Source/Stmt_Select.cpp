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
: Stmt(CN_STMT_SELECT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);
  
  m_pTokenVec = new TokenVec(false);
  
  m_pRootSubQry = nullptr;

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
  
  delete m_pRootSubQry;
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
  
  bool bResult = true;
  
  // create a descend for this statement
  Descend* pDescend = new Descend(pTokenVec,this);
  
  // descend and parse the statment
  bool bSubQry;
  if (pDescend->bIsDescendParse(bSubQry))
    bResult = pDescend->bCallFunc(m_pRootSubQry);
  
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
/* get root subQry                                                            */
/*----------------------------------------------------------------------------*/
Stmt_SubQry* Stmt_Select::pRootSubQry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);

  return m_pRootSubQry;
}

/*----------------------------------------------------------------------------*/
/* put subQry                                                                 */
/*----------------------------------------------------------------------------*/
void Stmt_Select::vRootSubQry(Stmt_SubQry* pRootSubQry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STMT_SELECT,_FOOTER_);

  m_pRootSubQry = pRootSubQry;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
