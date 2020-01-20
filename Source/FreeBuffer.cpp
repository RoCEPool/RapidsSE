/*============================================================================*/
/* FreeBuffer.cpp -- buffer when its in the buffer pool free chain            */
/*============================================================================*/

#include "Pool.h"
  
/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PoolBuffer* FreeBuffer::_pFreeBuffer(void* aAddr)
{
  PoolBuffer* pBuffer = reinterpret_cast<PoolBuffer*>(aAddr);
  
  RASSERT_TYPE(pBuffer,CN_FREEBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor -- never called                                                */
/*----------------------------------------------------------------------------*/
FreeBuffer::FreeBuffer(void)
: PoolBuffer(CN_FREEBUFFER) // correct
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_FREEBUFFER);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* destructor -- never called                                                 */
/*----------------------------------------------------------------------------*/
FreeBuffer::~FreeBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get next free buffer                                                       */
/*----------------------------------------------------------------------------*/
i8 FreeBuffer::iNextFree(void)
{
  if (CN_FREEBUFFER != eClass())
    int debug = 1;
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  return m_iNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free buffer                                                       */
/*----------------------------------------------------------------------------*/
void FreeBuffer::vNextFree(i8 iNextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FREEBUFFER,_FOOTER_);
  
  m_iNextFree = iNextFree;
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
  Stz* aData  = aAdd<Stz*>(this,sizeof(FreeBuffer));
  i8   nBytes = C_SIZE_POOL_BUFFER - sizeof(FreeBuffer) - sizeof(Footer);
  
  MemLib::vSet(aData,0x00,nBytes,CN_FREEBUFFER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
