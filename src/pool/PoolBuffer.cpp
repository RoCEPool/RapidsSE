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
PoolBuffer* Resources::_pPoolBuffer(void* aAddr)
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
    
  _setFooter_();
  
  ASSERT(m_iBufferNo >= gpBufferPool->iBufferFirst());
  ASSERT(m_iBufferNo <= gpBufferPool->iBufferLast());
  
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
  
  ASSERT(m_iBufferNo >= gpBufferPool->iBufferFirst());
  ASSERT(m_iBufferNo <= gpBufferPool->iBufferLast());
}

/*----------------------------------------------------------------------------*/
/* get buffer number                                                          */
/*----------------------------------------------------------------------------*/
i8 PoolBuffer::iBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_iBufferNo;
}

/*----------------------------------------------------------------------------*/
/* set buffer number                                                          */
/*----------------------------------------------------------------------------*/
void PoolBuffer::vBufferNo(i8 iBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_iBufferNo = iBufferNo;
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
