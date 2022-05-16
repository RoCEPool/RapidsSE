/*============================================================================*/
/* LinkedBuffer.cpp -- Linked Buffer                                          */
/*============================================================================*/

#include "BASE.h"



/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LinkedBuffer* BASE::_pLinkedBuffer(void* aAddress)
{
  LinkedBuffer* pLinkedBuffer = reinterpret_cast<LinkedBuffer*>(aAddress);
 
  if (pLinkedBuffer) {
    eCN eClassNo = pLinkedBuffer->eGetClassNo();
    UNUSED(eClassNo);
    
    ASSERT(CN_CRWBUFFER  == eClassNo ||
           CN_FREEPOOLBUFFER == eClassNo ||
           CN_ROWBUFFER  == eClassNo );
  }
  
  return pLinkedBuffer;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LinkedBuffer::LinkedBuffer(eCN eClassNo)
: Operand(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo);  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LinkedBuffer::~LinkedBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get buffer count                                                           */
/*----------------------------------------------------------------------------*/
i8 LinkedBuffer::nLinkedBufferCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  i8 nCount = 1;
  
  LinkedBuffer* pNext = pNextLinkedBuffer();
  while (pNext) {
    nCount++;
    pNext = pNext->pNextLinkedBuffer();
  }
  
  return nCount;
}

/*----------------------------------------------------------------------------*/
/* get buffer no of this buffer                                               */
/*----------------------------------------------------------------------------*/
BufferNo LinkedBuffer::tBufferNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  BufferNo tBufferNo;
  
  tBufferNo = this;

  return tBufferNo;
}

/*----------------------------------------------------------------------------*/
/* link given buffer after this buffer                                        */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLinkAsNext(LinkedBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  /*
      
     +-------------+         +-------------+
     | this buffer |         | next buffer |
     +-------------+         +-------------+
     |        next | (a) --> |        next |
     | prev        | <-- (b) | prev        |
     +-------------+         +-------------+
   
     +-------------+         +-------------+         +--------------+
     | this buffer |         | arg buffer  |         | next' buffer |
     +-------------+         +-------------+         +--------------+
     |        next | (c) --> |        next | (a) --> |         next |
     | prev        | <-- (d) | prev        | <-- (b) | prev         |
     +-------------+         +-------------+         +--------------+
   
  */
  
  pArgBuffer->vNextLinkedBuffer(m_tNextBuffer);      // (a)
  if (pNextLinkedBuffer())
    pNextLinkedBuffer()->vPrevLinkedBuffer(pArgBuffer->tBufferNo()); // (b)
  
  m_tNextBuffer = pArgBuffer;                  // (c)
  pArgBuffer->vPrevLinkedBuffer(this->tBufferNo());  // (d)
}

/*----------------------------------------------------------------------------*/
/* link given buffer before this buffer                                       */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLinkAsPrev(LinkedBuffer* pArgBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  /*
      
     +-------------+         +-------------+
     | prev buffer |         | this buffer |
     +-------------+         +-------------+
     |        next | (c) --> |        next |
     | prev        | <-- (d) | prev        |
     +-------------+         +-------------+
   
     +-------------+         +-------------+         +--------------+
     | prev buffer |         | arg buffer  |         | this buffer  |
     +-------------+         +-------------+         +--------------+
     |        next | (a) --> |        next | (c) --> |         next |
     | prev        | <-- (b) | prev        | <-- (d) | prev         |
     +-------------+         +-------------+         +--------------+
   
  */
  
  if (pPrevLinkedBuffer())
    pPrevLinkedBuffer()->vNextLinkedBuffer(pArgBuffer->tBufferNo());  // (a)
  pArgBuffer->vPrevLinkedBuffer(this->tBufferNo());             // (b)
  
  if (pNextLinkedBuffer())
    pNextLinkedBuffer()->vPrevLinkedBuffer(pArgBuffer->tBufferNo());  // (d)
  m_tNextBuffer = pArgBuffer;                             // (c)
}

/*----------------------------------------------------------------------------*/
/* unlink given buffer from the next and previous buffers                     */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vUnLink(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  /*
   
   +-------------+         +-------------+         +--------------+
   | prev buffer |         | this buffer  |        | next buffer  |
   +-------------+         +-------------+         +--------------+
   |        next | (a) --> |        next | (c) --> |         next |
   | prev        | <-- (b) | prev        | <-- (d) | prev         |
   +-------------+         +-------------+         +--------------+
   
   +-------------+         +-------------+
   | prev buffer |         | next buffer |
   +-------------+         +-------------+
   |        next | (a) --> |        next |
   | prev        | <-- (d) | prev        |
   +-------------+         +-------------+
   
   */
  
  //ASSERT( ! m_pNextBuffer )
  if (pPrevLinkedBuffer())
    pPrevLinkedBuffer()->vNextLinkedBuffer(m_tNextBuffer);    // (a)

  if (pNextLinkedBuffer())
    pNextLinkedBuffer()->m_tPrevBuffer = m_tPrevBuffer; // (d)
  
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get higher buffer                                                          */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pHigherLinkedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  LinkedBuffer* pHigherBuffer = m_tHigherBuffer.pGetLinkedBuffer();

  return pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherLinkedBuffer(BufferNo tHigherBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tHigherBuffer = tHigherBufferNo;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherLinkedBuffer(LinkedBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tHigherBuffer = pHigherBuffer;
  RASSERT(m_tHigherBuffer.pGetLinkedBuffer() == pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* get lower buffer                                                           */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pLowerLinkedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pLowerBuffer = m_tLowerBuffer.pGetLinkedBuffer();

  return pLowerBuffer;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerLinkedBuffer(LinkedBuffer* pLowerBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  m_tLowerBuffer = pLowerBuffer;
  RASSERT(m_tLowerBuffer.pGetLinkedBuffer() == pLowerBuffer);
}

/*----------------------------------------------------------------------------*/
/* set lower buffer                                                           */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerLinkedBuffer(BufferNo tLowerBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tLowerBuffer = tLowerBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pNextLinkedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pNextBuffer = m_tNextBuffer.pGetLinkedBuffer();

  return pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextLinkedBuffer(LinkedBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tNextBuffer = pNextBuffer;

  RASSERT(m_tNextBuffer.pGetLinkedBuffer() == pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextLinkedBuffer(BufferNo tNextBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tNextBuffer = tNextBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pPrevLinkedBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pPrevBuffer = m_tPrevBuffer.pGetLinkedBuffer();

  return pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevLinkedBuffer(LinkedBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //m_pPrevBuffer = pPrevBuffer;
  
  m_tPrevBuffer = pPrevBuffer;
  RASSERT(m_tPrevBuffer.pGetLinkedBuffer() == pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevLinkedBuffer(BufferNo tPrevBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tPrevBuffer = tPrevBufferNo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
