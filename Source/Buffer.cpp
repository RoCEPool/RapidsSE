/*============================================================================*/
/*  Buffer.cpp - head portion of a buffer in buffer pool                      */
/*============================================================================*/

#include "Pool.h"

extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Buffer::operator new(size_t iSize)
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
  _precondition_(_MAGIC_, eClassNo);
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
  
  Memory::vCheckInitialised(this,sizeof(Buffer));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Buffer::~Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(eClass() != CN_NONE);
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
}

/*----------------------------------------------------------------------------*/
/* get buffer number                                                          */
/*----------------------------------------------------------------------------*/
bn Buffer::nBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get address of the byte after buffer                                       */
/*----------------------------------------------------------------------------*/
char* Buffer::pBufferLimit(void) const
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  char* pLimit = (char*)this + gpBufferPool->iBufferSize();
  
  return pPoolCheck<char*>(pLimit);
}

/*----------------------------------------------------------------------------*/
/* set buffer number                                                          */
/*----------------------------------------------------------------------------*/
void Buffer::vBufferNo(bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nBufferNo = nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get address of last byte in buffer                                         */
/*----------------------------------------------------------------------------*/
char* Buffer::pLastByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  char* pLastByte = reinterpret_cast<char*>(this) + 1_MB - 1;     // to be fixed
  
  return pLastByte;
}

/*----------------------------------------------------------------------------*/
/* get overflow buffer number                                                 */
/*----------------------------------------------------------------------------*/
bn Buffer::nOverflowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/* set overflow buffer number                                                 */
/*----------------------------------------------------------------------------*/
void Buffer::vOverflowBuffer(bn nOverflowBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nOverflowBuffer = nOverflowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Buffer::pFooter(void)
{
  // Note: cannot check _FOOTER_ here
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,C_SIZE_LARGE_BUFFER-sizeof(Footer)));
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/