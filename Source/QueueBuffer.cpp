/*============================================================================*/
/* QueueBuffer.cpp - queue buffer of values                                       */
/*============================================================================*/

#include "Tasking.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* QueueBuffer::operator new(size_t)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void QueueBuffer::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueueBuffer::QueueBuffer(void)
: Buffer(CN_QUEUEBUFFER)
{
  _poolAddress_(this);
  _magic_();
  
  m_pNext     = nullptr;
  m_pTopValue = pFirstValue();
  m_pNextFree = m_pTopValue;
  m_iCount    = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueueBuffer::~QueueBuffer(void)
{
  _poolAddress_(this);
  _magic_();
}

/*----------------------------------------------------------------------------*/
/* append value to this queue                                                 */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bAppend(OnStack* pValue)
{
  _poolAddress_(this);
  
  i8 nBytes = pValue->iSize();
  
  char* pFinalByteOfValue = pcAdd(m_pNextFree,nBytes-1);
  
  if (pFinalByteOfValue > Buffer::pLastByte())
    return false;
  
  Memory::vCopy(m_pNextFree,pValue,nBytes);
  
  m_pNextFree = static_cast<OnStack*>(pvAdd(m_pNextFree,nBytes));
  
  m_iCount++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get number of values in queue                                              */
/*----------------------------------------------------------------------------*/
i8 QueueBuffer::iCount(void)
{
  _poolAddress_(this);
  
  return m_iCount;
}

/*----------------------------------------------------------------------------*/
/* dump this queue buffer                                                     */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vDump(void)
{
  _poolAddress_(this);
  
  vTrace("QueueBuffer: %p Count %lld TopValue %p NextFree %p\n",
         this,
         m_iCount,
         m_pTopValue,
         m_pNextFree);
}

/*----------------------------------------------------------------------------*/
/* get next qbuffer                                                           */
/*----------------------------------------------------------------------------*/
QueueBuffer* QueueBuffer::pNext(void)
{
  _poolAddress_(this);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next qbuffer                                                           */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vNext(QueueBuffer* pQbuffer)
{
  _poolAddress_(this);
  
  m_pNext = pQbuffer;
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bPop(ValWsp* pValWsp, bool& bDiscardable)
{
  _poolAddress_(this);

  bDiscardable = false;
  
  if (0 == m_iCount) {
    //ASSERT(m_pTopValue == m_pNextFree);
    return false;
  }
  
  i8 nBytes = m_pTopValue->iSize();

  Memory::vCopy(pValWsp,m_pTopValue,nBytes);
  
  //m_pTopValue = pNextValue(m_pTopValue);
  m_pTopValue = static_cast<OnStack*>(pvAdd(m_pTopValue,nBytes));
  
  m_iCount--;
  
  if (0 == m_iCount) {
    //ASSERT(!m_pTopValue);
    bDiscardable = true;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* first value in this qbuffer                                                */
/*----------------------------------------------------------------------------*/
OnStack* QueueBuffer::pFirstValue(void)
{
  _poolAddress_(this);
  
  OnStack* pFirstValue = static_cast<OnStack*>(pvAdd(this,sizeof(QueueBuffer)));
  
  return pFirstValue;
}

/*----------------------------------------------------------------------------*/
/* next value in this qbuffer                                                 */
/*----------------------------------------------------------------------------*/
OnStack* QueueBuffer::pNextValue(OnStack* pValue)
{
  _poolAddress_(this);
  
  i8 nBytes = pValue->iSize();
  
  OnStack* pNextValue = static_cast<OnStack*>(pvAdd(pValue,nBytes));
  
  if (pNextValue < m_pNextFree)
    return pNextValue;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* verify qbuffer                                                             */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vVerify(void)
{
  _poolAddress_(this);
  
  i8 iCount = 0;
  for (OnStack* pValue = pFirstValue(); pValue; pValue = pNextValue(pValue))
    iCount++;
  
  ASSERT(iCount == m_iCount);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/