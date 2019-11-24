/*============================================================================*/
/* QueueBuffer.cpp -- Queue PoolBuffer of Values                              */
/*============================================================================*/

#include "Structures.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
  QueueBuffer* pQueueBuffer = (QueueBuffer*)gpBufferPool->pAllocOtherBuffer();
  
  return pPoolCheck<void*>(pQueueBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void QueueBuffer::operator delete(void* pQueueBufferAddress)
{
  QueueBuffer* pQueueBuffer = static_cast<QueueBuffer*>(pQueueBufferAddress);
  
  gpBufferPool->vFree((PoolBuffer*)pQueueBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueueBuffer::QueueBuffer(void)
: PoolBuffer(CN_QUEUEBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  m_nPointers     = 0;
  m_ppTopObject = ppFirstPointer();
  m_ppNextFree  = m_ppTopObject;
  m_pNext       = nullptr;
  
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* attempt to append object pointer to this queue buffer                      */
/*----------------------------------------------------------------------------*/
bool QueueBuffer::bAppend(Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = sizeof(Object*);
  
  Stz* aFinalByteOfValue = aAdd<Stz*>(m_ppNextFree,nBytes-1);
  
  if (aFinalByteOfValue > PoolBuffer::aLastUsableByte())
    return false;
  
  * m_ppNextFree = pObject;
  
  m_ppNextFree = aAdd<Object**>(m_ppNextFree,nBytes);
  
  m_nPointers++;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get number of values in queue                                              */
/*----------------------------------------------------------------------------*/
i8 QueueBuffer::iCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  return m_nPointers;
}

/*----------------------------------------------------------------------------*/
/* dump this queue buffer                                                     */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vContentDump(Stz* aTab)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  vTrace("%sQueueBuffer: %p Count %lld TopObject %p NextFree %p\n"_Stz,
         aTab,
         this,
         m_nPointers,
         m_ppTopObject,
         m_ppNextFree);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void QueueBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  QueueBuffer* pQueueBuffer = _pQueueBuffer(pObject);
  
  vTrace("values %lld top %p next %p\n"_Stz,
         pQueueBuffer->m_nPointers,
         pQueueBuffer->m_ppTopObject,
         pQueueBuffer->m_ppNextFree);
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
Object* QueueBuffer::pPop(bool& bDiscardable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  bDiscardable = false;
  
  if (0 == m_nPointers) {
    //ASSERT(m_aTopValue == m_aNextFree);
    return nullptr;
  }
  
  i8 nBytes = sizeof(Object*);
  
  Object* pObject = * m_ppTopObject;
  
  m_ppTopObject = aAdd<Object**>(m_ppTopObject,nBytes);
  
  m_nPointers--;
  
  if (0 == m_nPointers) {
    
    // are the more queue buffers in the chain?
    if (m_pNext) {
      // yes, therefore have this top buffer discarded
      bDiscardable = true;
    }
    else {
      // no, therefore reuse this queue buffer
      m_ppTopObject = ppFirstPointer();
      m_ppNextFree  = m_ppTopObject;
    }
  }
  
  return pObject;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* first value in this qbuffer                                                */
/*----------------------------------------------------------------------------*/
Object** QueueBuffer::ppFirstPointer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  Object** ppFirstPointer = aAdd<Object**>(this,sizeof(QueueBuffer));
  
  return ppFirstPointer;
}

/*----------------------------------------------------------------------------*/
/* next value in this qbuffer                                                 */
/*----------------------------------------------------------------------------*/
Object** QueueBuffer::ppNextPointer(Object** ppPointer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUEUEBUFFER,_FOOTER_);
  
  i8 nBytes = sizeof(Object*);
  
  Object** ppNextPointer = aAdd<Object**>(ppPointer,nBytes);
  
  if (ppNextPointer < m_ppNextFree)
    return ppNextPointer;
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
  for (Object** ppObject = ppFirstPointer();
       ppObject;
       ppObject = ppNextPointer(ppObject))
    iCount++;
  
  ASSERT(iCount == m_nPointers);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
