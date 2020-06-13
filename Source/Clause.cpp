/*============================================================================*/
/* Clause.cpp -- Clause                                                        */
/*============================================================================*/

#include "CompilerStream.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Clause* CompilerStream::_pClause(void* aAddr)
{
  Clause* pClause = reinterpret_cast<Clause*>(aAddr);
  
  return pClause;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Clause::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause::Clause(eCN eClassNo)
: Operand(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_pTokenVec = new TokenVec(false);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause::~Clause(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pTokenVec;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect                                                                    */
/*----------------------------------------------------------------------------*/
void Clause::vCollect(Descend* pDescend, Clause_SubQry* pSubQry)
{
  _static_
  
  bool bResult;
  bool bSubQry;
  
  while (pDescend->bMore()) {
    
    Token* pCurTkn = pDescend->pCurr();
    
    Stz* szChars = pCurTkn->szChars();
    
    if (szChars->bSame(")"_Stz)) {
      return;
    }
    else
    if (szChars->bSame("("_Stz)) {
      vTrace("  : %s\n"_Stz,szChars);
      pDescend->vNext();

      if (pDescend->bIsDescendParse(bSubQry)) {
        bResult = pDescend->bCallFunc(pSubQry);
      }
      else {
        vCollect(pDescend,pSubQry);
      }
      
      pCurTkn = pDescend->pCurr();
      szChars = pCurTkn->szChars();
      vTrace("  : %s\n"_Stz,szChars);
      pDescend->vNext();
    }
    else
    if (pDescend->bIsDescendParse(bSubQry)) {
      if (! pDescend->bCallFunc(pSubQry))
        break;
    }
    else {
      vTrace("  : %s\n"_Stz,szChars);
      pDescend->vNext();
    }
    
  } // while
  
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* Clause::pTokenVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pTokenVec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
