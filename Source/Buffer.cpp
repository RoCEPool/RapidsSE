/*============================================================================*/
/*  Buffer.cpp - head portion of a buffer in buffer pool                      */
/*============================================================================*/

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Buffer* Pool::_pBuffer(void* aAddr)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(aAddr);
  
  //RASSERT_TYPE(pTask,???);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Buffer::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Buffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Buffer::Buffer(eCN eClassNo)
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
Buffer::~Buffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(eClass() != CN_NONE);
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
}

/*----------------------------------------------------------------------------*/
/* get buffer that an object is within                                        */
/*----------------------------------------------------------------------------*/
Buffer* Buffer::pGetBuffer(Object* pObject)
{
  Buffer* pBuffer = _pBuffer(pBitAnd<Buffer*>(pObject,C_OBJECT_TO_BUFFER_MASK));
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* get buffer number                                                          */
/*----------------------------------------------------------------------------*/
bn Buffer::nBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* set buffer number                                                          */
/*----------------------------------------------------------------------------*/
void Buffer::vBufferNo(bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_nBufferNo = nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get address of data area in buffer                                         */
/*----------------------------------------------------------------------------*/
void* Buffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  // last byte in buffer
  void* aDataArea = aAdd<void*>((char*)this,sizeof(Buffer));
  
  return aDataArea;
}

/*----------------------------------------------------------------------------*/
/* get address of last usable byte in buffer                                  */
/*----------------------------------------------------------------------------*/
Stz* Buffer::aLastUsableByte(void)
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
bn Buffer::nOverflowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set overflow buffer number                                                 */
/*----------------------------------------------------------------------------*/
void Buffer::vOverflowBuffer(bn nOverflowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_nOverflowBuffer = nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Buffer::aFooter(void)
{
  // Note: cannot check _FOOTER_ here
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Footer* aFooter = aAdd<Footer*>(this,C_SIZE_POOL_BUFFER-sizeof(Footer));
  
  return aFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
