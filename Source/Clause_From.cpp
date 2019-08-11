/*============================================================================*/
/* Clause_From.cpp -- From Clause operand                                     */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Clause_From* CompilerStream::_pClause_From(void* aAddr)
{
  Clause_From* pOp = reinterpret_cast<Clause_From*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Clause_From::operator new(size_t)
{
  Clause_From* pClause_From =
    static_cast<Clause_From*>(HeapMem::pAllocate(CN_CLAUSE_FROM));
  
  return pClause_From;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Clause_From::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_From::Clause_From(TokenVec* pTknVec, i8& i_Tkn_)
: Clause(CN_CLAUSE_FROM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_FROM,_FOOTER_);
  
  i8 nTkn = pTknVec->nTokens();

  while (i_Tkn_ < nTkn) {
    Token* pTkn = pTknVec->pGet(i_Tkn_);
    
    if (StrLib::bOneOf((char*)szaClauseWords,pTkn->szText()->aChar()))
      break;
    
    m_pTokenVec->vAdd(new Token(pTkn));
    i_Tkn_++;
  }

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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Clause_From);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
