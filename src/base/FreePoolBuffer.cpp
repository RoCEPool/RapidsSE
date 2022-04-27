/*============================================================================*/
/* FreePoolBuffer.cpp -- buffer when its in the buffer pool free chain            */
/*============================================================================*/

#include "BASE.h"
  
/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
PoolBuffer* FreePoolBuffer::_pFreePoolBuffer(void* aAddr)
{
  PoolBuffer* pBuffer = reinterpret_cast<PoolBuffer*>(aAddr);
  
  RASSERT_TYPE(pBuffer, CN_FREEPOOLBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor -- never called                                                */
/*----------------------------------------------------------------------------*/
FreePoolBuffer::FreePoolBuffer(void)
: PoolBuffer(CN_FREEPOOLBUFFER) // correct
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FREEPOOLBUFFER, _FOOTER_);
  
  _checkInitialised_(CN_FREEPOOLBUFFER);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* destructor -- never called                                                 */
/*----------------------------------------------------------------------------*/
FreePoolBuffer::~FreePoolBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FREEPOOLBUFFER, _FOOTER_);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get next free buffer                                                       */
/*----------------------------------------------------------------------------*/
i8 FreePoolBuffer::iNextFree(void)
{  
  _precondition_(_ADDRESS_, _MAGIC_, CN_FREEPOOLBUFFER, _FOOTER_);
  
  return m_iNextFree;
}

/*----------------------------------------------------------------------------*/
/* set next free buffer                                                       */
/*----------------------------------------------------------------------------*/
void FreePoolBuffer::vNextFree(i8 iNextFree)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_FREEPOOLBUFFER, _FOOTER_);
  
  m_iNextFree = iNextFree;
}

/*----------------------------------------------------------------------------*/
/* set footer                                                                 */
/*----------------------------------------------------------------------------*/
void FreePoolBuffer::vSetFooter(void)
{
  _setFooter_();
}

/*----------------------------------------------------------------------------*/
/* zero data                                                                  */
/*----------------------------------------------------------------------------*/
void FreePoolBuffer::vZeroData(void)
{
  Stz* aData  = aAdd<Stz*>(this,sizeof(FreePoolBuffer));
  i8   nBytes = C_SIZE_POOL_BUFFER - sizeof(FreePoolBuffer) - sizeof(Footer);
  
  MemLib::vSet(aData, 0x00, nBytes, CN_FREEPOOLBUFFER);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
