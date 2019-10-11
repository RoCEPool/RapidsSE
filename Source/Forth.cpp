/*============================================================================*/
/* ForthClump.hpp -- Forth Clump                                              */
/*============================================================================*/

#include "SenVM.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ForthClump* SenVM::_pForthClump(void* aAddr)
{
  ForthClump* pForthClump = reinterpret_cast<ForthClump*>(aAddr);
  
  RASSERT_TYPE(pForthClump,CN_FORTH);
  
  return pForthClump;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ForthClump::operator new(size_t)
{
  ForthClump* pForthClump = (ForthClump*)(HeapMem::pAllocate(CN_FORTH));
  
  return pForthClump;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ForthClump::operator delete(void* aAddress)
{
  ForthClump* pForthClump = reinterpret_cast<ForthClump*>(aAddress);
  
  HeapMem::vFree(pForthClump);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ForthClump::ForthClump(Task* pTask)
: Object(CN_FORTH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
  
  m_pTask        = pTask;
  
  m_pLocalsFrame = nullptr;
  m_pDataStack   = nullptr;
  m_pReturnStack = nullptr;
  m_pAuxStack    = nullptr;
  
 _checkInitialised_(CN_FORTH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ForthClump::~ForthClump(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FORTH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ForthClump);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
