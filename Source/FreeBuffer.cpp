/*============================================================================*/
/* FreeBuffer.cpp -- buffer when its in the buffer pool free chain            */
/*============================================================================*/

#include "Pool.h"
  
extern BufferPool* gpBufferPool;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Buffer* FreeBuffer::_pFreeBuffer(void* aAddr)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(aAddr);
  
  RASSERT( !pBuffer || CN_FREEBUFFER == pBuffer->eClass() );
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor -- never called                                                */
/*----------------------------------------------------------------------------*/
FreeBuffer::FreeBuffer(void)
: Buffer(CN_FREEBUFFER) // correct
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_FREEBUFFER);
  
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* destructor -- never called                                                 */
/*----------------------------------------------------------------------------*/
FreeBuffer::~FreeBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  ASSERT("Should never be reached");
}

/*----------------------------------------------------------------------------*/
/* get next free buffer                                                       */
/*----------------------------------------------------------------------------*/
bn FreeBuffer::nNextFree(void)
{
  if (CN_FREEBUFFER != eClass())
    int debug = 1;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  return m_nNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free buffer                                                       */
/*----------------------------------------------------------------------------*/
void FreeBuffer::vNextFree(bn nBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  m_nNextFree = nBuffer;
}

/*----------------------------------------------------------------------------*/
/* set footer                                                                 */
/*----------------------------------------------------------------------------*/
void FreeBuffer::vSetFooter(void)
{
  _setFooter_();
}

/*----------------------------------------------------------------------------*/
/* zero data                                                                  */
/*----------------------------------------------------------------------------*/
void FreeBuffer::vZeroData(void)
{
  char* pData  = pAdd<char*>(this,sizeof(FreeBuffer));
  i8    nBytes = C_SIZE_POOL_BUFFER - sizeof(FreeBuffer) - sizeof(Footer);
  
  Memory::vSet(pData,0xEF,nBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
