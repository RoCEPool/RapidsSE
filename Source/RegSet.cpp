/*============================================================================*/
/* RegSet.hpp -- RegSet Values                                                */
/*============================================================================*/

#include "Registers.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RegSet* Registers::_pRegSet(void* aAddr)
{
  RegSet* pRegSet = reinterpret_cast<RegSet*>(aAddr);
  
  RASSERT_TYPE(pRegSet,CN_REGSET);
  
  return pRegSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RegSet::operator new(size_t)
{
  RegSet* pRegSet = (RegSet*)(HeapMem::pAllocate(CN_REGSET));
  
  return pRegSet;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RegSet::operator delete(void* aAddress)
{
  RegSet* pRegSet = reinterpret_cast<RegSet*>(aAddress);
  
  HeapMem::vFree(pRegSet);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RegSet::RegSet(Task* pTask)
: Object(CN_REGSET)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  vTrace("  new RegSet %p for Task %p\n"_Stz,this,pTask);
  
  m_pTask = pTask;
  
  m_pOperandQueueLeft  = nullptr;
  m_pOperandQueueRight = nullptr;

 _checkInitialised_(CN_REGSET);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RegSet::~RegSet(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REGSET,_FOOTER_);
  
  delete m_pOperandQueueLeft;
  delete m_pOperandQueueRight;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(RegSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
