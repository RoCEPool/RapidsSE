/*============================================================================*/
/* PoolBuffer.cpp - head portion of a buffer in buffer pool                   */
/*============================================================================*/

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PoolBuffer* Pool::_pPoolBuffer(void* aAddr)
{
  PoolBuffer* pBuffer = reinterpret_cast<PoolBuffer*>(aAddr);
  
  //RASSERT_TYPE(pTask,???);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* PoolBuffer::operator new(size_t)
{
  RASSERT(0);
  PoolBuffer* pBuffer = gpBufferPool->pAllocOtherBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void PoolBuffer::operator delete(void* pBufferAddress)
{
  RASSERT(0);
  PoolBuffer* pBuffer = static_cast<PoolBuffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PoolBuffer::PoolBuffer(eCN eClassNo)
: Object(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  m_aLastByte = aLastUsableByte();
  
  _setFooter_();
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
  
  _checkInitialised_(CN_FREEBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PoolBuffer::~PoolBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(eClass() != CN_NONE);
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
}

/*----------------------------------------------------------------------------*/
/* get buffer that an object is within                                        */
/*----------------------------------------------------------------------------*/
PoolBuffer* PoolBuffer::pGetPoolBuffer(Object* pObject)
{
  PoolBuffer* pBuffer =
    _pPoolBuffer(pBitAnd<PoolBuffer*>(pObject,C_OBJECT_TO_BUFFER_MASK));
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* get address of data area in buffer                                         */
/*----------------------------------------------------------------------------*/
void* PoolBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  // last byte in buffer
  void* aDataArea = aAdd<void*>((char*)this,sizeof(PoolBuffer));
  
  return aDataArea;
}

/*----------------------------------------------------------------------------*/
/* get address of last usable byte in buffer                                  */
/*----------------------------------------------------------------------------*/
Stz* PoolBuffer::aLastUsableByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  // last byte in buffer
  Stz* aLastByte = reinterpret_cast<Stz*>(this) + C_SIZE_POOL_BUFFER - 1;
  
  // last byte before footer
  Stz* aLastUsableByte = aLastByte - sizeof(C_FOOTER);
  
  return aLastUsableByte;
}

/*----------------------------------------------------------------------------*/
/* get overflow buffer number                                                 */
/*----------------------------------------------------------------------------*/
bn PoolBuffer::nOverflowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set overflow buffer number                                                 */
/*----------------------------------------------------------------------------*/
void PoolBuffer::vOverflowBuffer(bn nOverflowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_nOverflowBuffer = nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get buffer number                                                          */
/*----------------------------------------------------------------------------*/
bn PoolBuffer::nPoolBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* set buffer number                                                          */
/*----------------------------------------------------------------------------*/
void PoolBuffer::vPoolBufferNo(bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_nBufferNo = nBufferNo;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* PoolBuffer::aFooter(void)
{
  // Note: cannot check _FOOTER_ here
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Footer* aFooter = aAdd<Footer*>(this,C_SIZE_POOL_BUFFER-sizeof(Footer));
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
