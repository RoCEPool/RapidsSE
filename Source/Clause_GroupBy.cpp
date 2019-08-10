/*============================================================================*/
/* Clause_GroupBy.cpp -- GroupBy Clause operand                               */
/*============================================================================*/

#include "CompilerStream.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Clause_GroupBy* CompilerStream::_pClause_GroupBy(void* aAddr)
{
  Clause_GroupBy* pOp = reinterpret_cast<Clause_GroupBy*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* allocate from heap                                                         */
/*----------------------------------------------------------------------------*/
void* Clause_GroupBy::operator new(size_t)
{
  Clause_GroupBy* pClause_GroupBy =
    static_cast<Clause_GroupBy*>(HeapMem::pAllocate(CN_CLAUSE_GROUPBY));
  
  return pClause_GroupBy;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Clause_GroupBy::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Clause_GroupBy::Clause_GroupBy(void)
: Operand(CN_CLAUSE_GROUPBY)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);

  _checkInitialised_(CN_CLAUSE_GROUPBY);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Clause_GroupBy::~Clause_GroupBy(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CLAUSE_GROUPBY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Clause_GroupBy);


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
