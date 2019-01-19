/*============================================================================*/
/* CarryOver.cpp - set of pointers to be restored on restart                  */
/*============================================================================*/

#include "StorageEngine.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* CarryOver::operator new(size_t)
{
  CarryOver* pCarryOver = (CarryOver*)gpBufferPool->pAllocOtherBuffer();
  
  return pPoolCheck<void*>(pCarryOver);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void CarryOver::operator delete(void* pCarryOverAddress)
{
  CarryOver* pCarryOver = static_cast<CarryOver*>(pCarryOverAddress);
  
  gpBufferPool->vFree((Buffer*)pCarryOver);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CarryOver::CarryOver(void)
: Buffer(CN_CARRYOVER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARRYOVER);
  
  // carry over must always be the second buffer in the buffer pool
  ASSERT(nBufferNo() == 2_bn);
    
  _checkInitialised_(CN_CARRYOVER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARRYOVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CarryOver::~CarryOver(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARRYOVER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* CarryOver::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARRYOVER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
