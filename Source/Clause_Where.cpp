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
Clause_Where::Clause_Where(TokenVec* pTknVec, i8& i_Tkn_)
: Clause(CN_CLAUSE_WHERE,pTknVec,i_Tkn_)
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
bool Clause_Where::bDescend(Descend* pDescend)
{
  _static_

  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("WHERE : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_Where::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
