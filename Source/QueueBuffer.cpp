/*============================================================================*/
/* QueueBuffer.cpp - queue buffer of values                                       */
/*============================================================================*/

#include "Tasks.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* QueueBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  m_nValues   = 0;
  m_pTopValue = pFirstValue();
  m_pNextFree = m_pTopValue;
  m_pNext     = nullptr;
  
  _checkInitialised_(sizeof(QueueBuffer));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueueBuffer::~QueueBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append value to this queue                                                 */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bAppend(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = pValue->iSize();
  
  char* pFinalByteOfValue = pcAdd(m_pNextFree,nBytes-1);
  
  if (pFinalByteOfValue > Buffer::pLastUsableByte())
    return false;
  
  Memory::vCopy(m_pNextFree,pValue,nBytes);
  
  m_pNextFree = static_cast<Value*>(pvAdd(m_pNextFree,nBytes));
  
  m_nValues++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get number of values in queue                                              */
/*----------------------------------------------------------------------------*/
i8 QueueBuffer::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  return m_nValues;
}

/*----------------------------------------------------------------------------*/
/* dump this queue buffer                                                     */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vDump(const char* pTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  vTrace("%sQueueBuffer: %p\n", pTab, this);
  vTrace("%s  Count %lld\n", pTab, m_nValues);
  vTrace("%s  TopValue %p \n", pTab, m_pTopValue);
  vTrace("%s  NextFree %p\n\n", pTab, m_pNextFree);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_QUEUEBUFFER);
  ASSERT(pObject->eClass() == CN_QUEUEBUFFER);
  
  QueueBuffer* pQueueBuffer = reinterpret_cast<QueueBuffer*>(pObject);
  
  pQueueBuffer->vDump(pTab);
}

/*----------------------------------------------------------------------------*/
/* get next qbuffer                                                           */
/*----------------------------------------------------------------------------*/
QueueBuffer* QueueBuffer::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next qbuffer                                                           */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vNext(QueueBuffer* pQbuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  m_pNext = pQbuffer;
}

/*----------------------------------------------------------------------------*/
/* pop value from this queue (there may not be any)                           */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bPop(ValWsp* pValWsp, bool& bDiscardable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);

  bDiscardable = false;
  
  if (0 == m_nValues) {
    //ASSERT(m_pTopValue == m_pNextFree);
    return false;
  }
  
  i8 nBytes = m_pTopValue->iSize();

  Memory::vCopy(pValWsp,m_pTopValue,nBytes);
  
  //m_pTopValue = pNextValue(m_pTopValue);
  m_pTopValue = static_cast<Value*>(pvAdd(m_pTopValue,nBytes));
  
  m_nValues--;
  
  if (0 == m_nValues) {
    if (m_pNext) {
      bDiscardable = true;
    }
    else {
      m_pTopValue = pFirstValue();
      m_pNextFree = m_pTopValue;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop value set from this queue (there may not be any)                       */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bPop(ValSet* pValSet, bool& bDiscardable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
 
  bDiscardable = false;
  
  if (0 == m_nValues) {
    //ASSERT(m_pTopValue == m_pNextFree);
    return false;
  }
  
  i8 nBytes = m_pTopValue->iSize();
  
  Memory::vCopy(pValSet,m_pTopValue,nBytes);
  
  //m_pTopValue = pNextValue(m_pTopValue);
  m_pTopValue = static_cast<Value*>(pvAdd(m_pTopValue,nBytes));
  
  m_nValues--;
  
  if (0 == m_nValues) {
    if (m_pNext) {
      bDiscardable = true;
    }
    else {
      m_pTopValue = pFirstValue();
      m_pNextFree = m_pTopValue;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* first value in this qbuffer                                                */
/*----------------------------------------------------------------------------*/
Value* QueueBuffer::pFirstValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  Value* pFirstValue = static_cast<Value*>(pvAdd(this,sizeof(QueueBuffer)));
  
  return pFirstValue;
}

/*----------------------------------------------------------------------------*/
/* next value in this qbuffer                                                 */
/*----------------------------------------------------------------------------*/
Value* QueueBuffer::pNextValue(Value* pValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = pValue->iSize();
  
  Value* pNextValue = static_cast<Value*>(pvAdd(pValue,nBytes));
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 iCount = 0;
  for (Value* pValue = pFirstValue(); pValue; pValue = pNextValue(pValue))
    iCount++;
  
  ASSERT(iCount == m_nValues);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/