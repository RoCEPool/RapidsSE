/*============================================================================*/
/* QueueBuffer.cpp -- Queue Buffer of Values                                  */
/*============================================================================*/

#include "Structures.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
QueueBuffer* Structures::_pQueueBuffer(void* aAddr)
{
  QueueBuffer* pQueueBuffer = reinterpret_cast<QueueBuffer*>(aAddr);
  
  RASSERT_TYPE(pQueueBuffer,CN_QUEUEBUFFER);
  
  return pQueueBuffer;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* QueueBuffer::operator new(size_t)
{
  QueueBuffer* pQueueBuffer = (QueueBuffer*)gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pQueueBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void QueueBuffer::operator delete(void* pQueueBufferAddress)
{
  QueueBuffer* pQueueBuffer = static_cast<QueueBuffer*>(pQueueBufferAddress);
  
  gpBufferPool->vFree((Buffer*)pQueueBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueueBuffer::QueueBuffer(void)
: Buffer(CN_QUEUEBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  m_nValues   = 0;
  m_aTopValue = aFirstValue();
  m_aNextFree = m_aTopValue;
  m_pNext     = nullptr;
  
  _checkInitialised_(CN_QUEUEBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueueBuffer::~QueueBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* append value to this queue                                                 */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bAppend(Value* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = aValue->nSize();
  
  Stz* aFinalByteOfValue = aAdd<Stz*>(m_aNextFree,nBytes-1);
  
  if (aFinalByteOfValue > Buffer::aLastUsableByte())
    return false;
  
  MemLib::vCopy(m_aNextFree,aValue,nBytes,CN_QUEUEBUFFER);
  
  m_aNextFree = aAdd<Value*>(m_aNextFree,nBytes);
  
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
void QueueBuffer::vContentDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  vTrace("%sQueueBuffer: %p Count %lld TopValue %p NextFree %p\n"_Stz,
         aTab,
         this,
         m_nValues,
         m_aTopValue,
         m_aNextFree);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  QueueBuffer* pQueueBuffer = _pQueueBuffer(pObject);
  
  vTrace("values %lld top %p next %p\n"_Stz,
         pQueueBuffer->m_nValues,
         pQueueBuffer->m_aTopValue,
         pQueueBuffer->m_aNextFree);
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
bool QueueBuffer::bPop(Forth::GenVar* pValWrk, bool& bDiscardable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);

  bDiscardable = false;
  
  if (0 == m_nValues) {
    //ASSERT(m_aTopValue == m_aNextFree);
    return false;
  }
  
  i8 nBytes = m_aTopValue->nSize();

  MemLib::vCopy(pValWrk->aNestedValue(),m_aTopValue,nBytes,CN_QUEUEBUFFER);
  
  //m_aTopValue = pNextValue(m_aTopValue);
  m_aTopValue = aAdd<Value*>(m_aTopValue,nBytes);
  
  m_nValues--;
  
  if (0 == m_nValues) {
    if (m_pNext) {
      bDiscardable = true;
    }
    else {
      m_aTopValue = aFirstValue();
      m_aNextFree = m_aTopValue;
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
Value* QueueBuffer::aFirstValue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  Value* aFirstValue = pAdd<Value*>(this,sizeof(QueueBuffer));
  
  return aFirstValue;
}

/*----------------------------------------------------------------------------*/
/* next value in this qbuffer                                                 */
/*----------------------------------------------------------------------------*/
Value* QueueBuffer::aNextValue(Value* aValue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = aValue->nSize();
  
  Value* aNextValue = aAdd<Value*>(aValue,nBytes);
  
  if (aNextValue < m_aNextFree)
    return aNextValue;
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
  for (Value* aValue = aFirstValue(); aValue; aValue = aNextValue(aValue))
    iCount++;
  
  ASSERT(iCount == m_nValues);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
