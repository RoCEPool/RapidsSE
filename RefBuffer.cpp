/*============================================================================*/
/* RefBuffer.cpp -- Reference Buffer                                          */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RefBuffer* PushDown::_pRefBuffer(void* aAddress)
{
  RefBuffer* pRefBuffer = reinterpret_cast<RefBuffer*>(aAddress);
  
  return pRefBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RefBuffer::RefBuffer(void)
: DataBuffer(CN_REFBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_REFBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RefBuffer::~RefBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aAllotRefSet(i8 nRowRefs)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  byte* ayRefSet = ayAllotBytes(RefSet::nBytesReq(nRowRefs));
  
  RefSet* aRefSet = new(ayRefSet) RefSet(nRowRefs);
  
  return aRefSet;
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aFirstRefSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);
  
  RefSet* aRefSet = aFirstItem();
  
  return aRefSet;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
RefSet* RefBuffer::aNextRefSet(RefSet* aPriorRefSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER,_FOOTER_);

  RefSet* aRefSet = aAdjacentItem(aPriorRefSet);
  
  if (aRefSet && CN_REFSET == aRefSet->eClass())
    return aRefSet;

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RefBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_REFBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
