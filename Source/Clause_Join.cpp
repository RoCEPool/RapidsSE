/*============================================================================*/
/* Clause_Join.cpp -- Join Clause operand                                     */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompilerStream,Clause_Join,CN_CLAUSE_JOIN)
__aNewFunction(Clause_Join,CN_CLAUSE_JOIN)
__vDeleteFunction(Clause_Join,CN_CLAUSE_JOIN)
__aFooterFunction(Clause_Join)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_Join::Clause_Join(TokenVec* pTknVec, i8& i_Tkn_)
: Clause(CN_CLAUSE_JOIN,pTknVec,i_Tkn_)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);

  m_sName = "join"_Stz;
  
  _checkInitialised_(CN_CLAUSE_JOIN);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_Join::~Clause_Join(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_JOIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Join::bDescend(Descend* pDescend)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("JOIN : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Join::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
