/*============================================================================*/
/* Clause_From.cpp -- From Clause operand                                     */
/*============================================================================*/

#include "CompilerStream.h"

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
Clause_From::Clause_From(TokenVec* pTknVec, i8& i_Tkn_)
: Clause(CN_CLAUSE_FROM,pTknVec,i_Tkn_)
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
bool Clause_From::bDescend(Descend* pDescend)
{
  _static_
  
  Token* pCurTkn = pDescend->pCurr();
  
  vTrace("FROM : %s\n"_Stz,pCurTkn->szChars());
  pDescend->vNext();
  
  vCollect(pDescend);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* descend                                                                    */
/*----------------------------------------------------------------------------*/
bool Clause_From::bParse(Descend* pDescend)
{
  _static_
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
