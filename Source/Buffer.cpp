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
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  pFooter()->vSetFooter();
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
  
  Memory::vCheckInitialised(this,sizeof(Buffer));

  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Buffer::~Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  ASSERT(eClass() != CN_NONE);
  
  ASSERT(m_nBufferNo >= gpBufferPool->nBufferFirst());
  ASSERT(m_nBufferNo <= gpBufferPool->nBufferLast());
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
/* get address of the byte after buffer                                       */
/*----------------------------------------------------------------------------*/
char* Buffer::pBufferLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  char* pLimit = (char*)this + gpBufferPool->iBufferSize();
  
  return pPoolCheck<char*>(pLimit);
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
/* get address of last usable byte in buffer                                  */
/*----------------------------------------------------------------------------*/
char* Buffer::pLastByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  char* pLastByte = reinterpret_cast<char*>(this) + C_SIZE_LARGE_BUFFER - 1;
  
  pLastByte -= sizeof(FOOTER);
  
  return pLastByte;
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
Footer* Buffer::pFooter(void)
{
  // Note: cannot check _FOOTER_ here
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Footer* pFooter = static_cast<Footer*>(pvAdd(this,C_SIZE_LARGE_BUFFER-sizeof(Footer)));
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/