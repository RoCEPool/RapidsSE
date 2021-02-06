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
  
    ASSERT(CN_CRWBUFFER  == eClassNo ||
           CN_FREEBUFFER == eClassNo ||
           CN_ROWBUFFER  == eClassNo);
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
  
  ASSERT(CacheLine::bMultipleCacheLines(sizeof(LinkedBuffer)));

  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  
  m_pHigherBuffer = nullptr;
  m_pLowerBuffer  = nullptr;
  
//  m_tNextBuffer   = C_UNKNOWN;
//  m_tPrevBuffer   = C_UNKNOWN;
//
//  m_tHigherBuffer = C_UNKNOWN;
//  m_tLowerBuffer  = C_UNKNOWN;

  //MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),eClassNo);

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
  
  pArgBuffer->vNextBuffer(m_tNextBuffer);      // (a)
  if (m_pNextBuffer)
    m_pNextBuffer->vPrevBuffer(pArgBuffer->tBufferNo()); // (b)
  
  m_tNextBuffer = pArgBuffer;                  // (c)
  pArgBuffer->vPrevBuffer(this->tBufferNo());  // (d)
}
/*----------------------------------------------------------------------------*/
//void LinkedBuffer::vLinkAsNext(LinkedBuffer* pArgBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_);
//
//  /*
//
//     +-------------+         +-------------+
//     | this buffer |         | next buffer |
//     +-------------+         +-------------+
//     |        next | (a) --> |        next |
//     | prev        | <-- (b) | prev        |
//     +-------------+         +-------------+
//
//     +-------------+         +-------------+         +--------------+
//     | this buffer |         | arg buffer  |         | next' buffer |
//     +-------------+         +-------------+         +--------------+
//     |        next | (c) --> |        next | (a) --> |         next |
//     | prev        | <-- (d) | prev        | <-- (b) | prev         |
//     +-------------+         +-------------+         +--------------+
//
//  */
//
//  pArgBuffer->vNextBuffer(m_pNextBuffer);    // (a)
//  if (m_pNextBuffer)
//    m_pNextBuffer->vPrevBuffer(pArgBuffer);  // (b)
//
//  m_pNextBuffer = pArgBuffer;                // (c)
//  pArgBuffer->vPrevBuffer(this);             // (d)
//}

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
  
  if (m_pPrevBuffer)
    m_pPrevBuffer->vNextBuffer(pArgBuffer->tBufferNo());  // (a)
  pArgBuffer->vPrevBuffer(this->tBufferNo());             // (b)
  
  if (m_pNextBuffer)
    m_pNextBuffer->vPrevBuffer(pArgBuffer->tBufferNo());  // (d)
  m_pNextBuffer = pArgBuffer;                             // (c)
}
/*----------------------------------------------------------------------------*/
//void LinkedBuffer::vLinkAsPrev(LinkedBuffer* pArgBuffer)
//{
//  _precondition_(_ADDRESS_,_MAGIC_);
//
//  /*
//
//     +-------------+         +-------------+
//     | prev buffer |         | this buffer |
//     +-------------+         +-------------+
//     |        next | (c) --> |        next |
//     | prev        | <-- (d) | prev        |
//     +-------------+         +-------------+
//
//     +-------------+         +-------------+         +--------------+
//     | prev buffer |         | arg buffer  |         | this buffer  |
//     +-------------+         +-------------+         +--------------+
//     |        next | (a) --> |        next | (c) --> |         next |
//     | prev        | <-- (b) | prev        | <-- (d) | prev         |
//     +-------------+         +-------------+         +--------------+
//
//  */
//
//  if (m_pPrevBuffer)
//    m_pPrevBuffer->vNextBuffer(pArgBuffer);  // (a)
//  pArgBuffer->vPrevBuffer(this);             // (b)
//
//  if (m_pNextBuffer)
//    m_pNextBuffer->vPrevBuffer(pArgBuffer);  // (d)
//  m_pNextBuffer = pArgBuffer;                // (c)
//}

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
  if (m_pPrevBuffer)
    m_pPrevBuffer->vNextBuffer(m_pNextBuffer);    // (a)

  if (m_pNextBuffer)
    m_pNextBuffer->m_pPrevBuffer = m_pPrevBuffer; // (d)
  
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get higher buffer                                                          */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  LinkedBuffer* pHigherBuffer = _pLinkedBuffer(m_pHigherBuffer);

  //RASSERT(m_tHigherBuffer.pGetBuffer() == pHigherBuffer);

  return pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherBuffer(BufferNo tHigherBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tHigherBuffer = tHigherBufferNo;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherBuffer(LinkedBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pHigherBuffer = pHigherBuffer;
  
  m_tHigherBuffer = pHigherBuffer;
  RASSERT(m_tHigherBuffer.pGetLinkedBuffer() == pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* get lower buffer                                                           */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pLowerBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pLowerBuffer = _pLinkedBuffer(m_pLowerBuffer);

  RASSERT(m_tLowerBuffer.pGetLinkedBuffer() == pLowerBuffer);

  return pLowerBuffer;
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerBuffer(LinkedBuffer* pLowerBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pLowerBuffer = pLowerBuffer;
  
  m_tLowerBuffer = pLowerBuffer;
  RASSERT(m_tLowerBuffer.pGetLinkedBuffer() == pLowerBuffer);
}

/*----------------------------------------------------------------------------*/
/* set lower buffer                                                           */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerBuffer(BufferNo tLowerBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tLowerBuffer = tLowerBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pNextBuffer = _pLinkedBuffer(m_pNextBuffer);

  RASSERT(m_tNextBuffer.pGetLinkedBuffer() == pNextBuffer);

  return pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextBuffer(LinkedBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pNextBuffer = pNextBuffer;
  
  m_tNextBuffer = pNextBuffer;
    
  RASSERT(m_tNextBuffer.pGetLinkedBuffer() == pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextBuffer(BufferNo tNextBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tNextBuffer = tNextBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  LinkedBuffer* pPrevBuffer = _pLinkedBuffer(m_pPrevBuffer);

  RASSERT(m_tPrevBuffer.pGetLinkedBuffer() == pPrevBuffer);

  return pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevBuffer(LinkedBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pPrevBuffer = pPrevBuffer;
  
  m_tPrevBuffer = pPrevBuffer;
  RASSERT(m_tPrevBuffer.pGetLinkedBuffer() == pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevBuffer(BufferNo tPrevBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_tPrevBuffer = tPrevBufferNo;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
