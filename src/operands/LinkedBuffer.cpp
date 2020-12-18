/*============================================================================*/
/* LinkedBuffer.cpp -- Linked Buffer                                          */
/*============================================================================*/

#include "Chunks.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LinkedBuffer* Chunks::_pLinkedBuffer(void* aAddress)
{
  LinkedBuffer* pLinkedBuffer = reinterpret_cast<LinkedBuffer*>(aAddress);
  
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
  
  MemLib::vZero(m__PadToCacheLine,sizeof(m__PadToCacheLine),eClassNo);

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
  
  pArgBuffer->vNextBuffer(m_pNextBuffer);    // (a)
  if (m_pNextBuffer)
    m_pNextBuffer->vPrevBuffer(pArgBuffer);  // (b)
  
  m_pNextBuffer = pArgBuffer;                // (c)
  pArgBuffer->vPrevBuffer(this);             // (d)
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
  
  if (m_pPrevBuffer)
    m_pPrevBuffer->vNextBuffer(pArgBuffer);  // (a)
  pArgBuffer->vPrevBuffer(this);             // (b)
  
  if (m_pNextBuffer)
    m_pNextBuffer->vPrevBuffer(pArgBuffer);  // (d)
  m_pNextBuffer = pArgBuffer;                // (c)
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
  if (m_pPrevBuffer)
    m_pPrevBuffer->vNextBuffer(m_pNextBuffer);    // (a)

  if (m_pNextBuffer)
    m_pNextBuffer->m_pPrevBuffer = m_pPrevBuffer; // (d)
  
}


/*----------------------------------------------------------------------------*/
/* get higher buffer                                                          */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pHigherBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pHigherBuffer);
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vHigherBuffer(LinkedBuffer* pHigherBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pHigherBuffer = pHigherBuffer;
}

/*----------------------------------------------------------------------------*/
/* get lower buffer                                                           */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pLowerBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pLowerBuffer);
}

/*----------------------------------------------------------------------------*/
/* set higher buffer                                                          */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vLowerBuffer(LinkedBuffer* pLowerBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pLowerBuffer = pLowerBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vNextBuffer(LinkedBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
LinkedBuffer* LinkedBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return _pLinkedBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void LinkedBuffer::vPrevBuffer(LinkedBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
