/*============================================================================*/
/* ObjBuffer.cpp -- Index Entry Buffer                                        */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ObjBuffer* BASE::_pObjBuffer(void* aAddress)
{
  ObjBuffer* pObjBuffer = reinterpret_cast<ObjBuffer*>(aAddress);
  
  RASSERT_TYPE(pObjBuffer,CN_ENTBUFFER);
  
  return pObjBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* ObjBuffer::operator new(size_t)
{
  ObjBuffer* pObjBuffer =
    static_cast<ObjBuffer*>(Heaps::pAllocate(CN_ENTBUFFER));
  
  return pObjBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of entry buffer for given address                              */
/*----------------------------------------------------------------------------*/
void* ObjBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void ObjBuffer::operator delete(void* aAddress)
{
  ObjBuffer* pObjBuffer = static_cast<ObjBuffer*>(aAddress);
  
  Heaps::vFree(pObjBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ObjBuffer::ObjBuffer(void)
: DataBuffer(CN_ENTBUFFER, true)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  ASSERT(CacheLine::bOnCacheLineBoundary(this));
    
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ObjBuffer::~ObjBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ObjBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
