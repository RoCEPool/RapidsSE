/*============================================================================*/
/* LinkedBuffer.hpp -- Linked Buffer                                          */
/*============================================================================*/
#pragma once

namespace BASE
{
  class LinkedBuffer : public Operand
  {
  private:
    BufferNo m_tNextBuffer;   // next buffer
    BufferNo m_tPrevBuffer;   // previous buffer

    BufferNo m_tHigherBuffer; // higher buffer
    BufferNo m_tLowerBuffer;  // lower buffer

  private:
    SingleWord    m__PadToCacheLine[4]; // padding to cache line

  public:
    LinkedBuffer(eCN eClass);
    ~LinkedBuffer(void) noexcept(false);
  
    BufferNo tBufferNo(void);   // buffer no
  
    void vLinkAsNext(LinkedBuffer* pArgBuffer);
    void vLinkAsPrev(LinkedBuffer* pArgBuffer);
    void vUnLink(void);

    LinkedBuffer* pHigherBuffer(void);
    void          vHigherBuffer(BufferNo tHigherBufferNo);
    void          vHigherBuffer(LinkedBuffer* pHigherBuffer);

    LinkedBuffer* pLowerBuffer(void);
    void          vLowerBuffer(BufferNo tLowerBufferNo);
    void          vLowerBuffer(LinkedBuffer* pLowerBuffer);
  
    LinkedBuffer* pNextBuffer(void);
    void          vNextBuffer(BufferNo tNextBufferNo);
    void          vNextBuffer(LinkedBuffer* pNextBuffer);
  
    LinkedBuffer* pPrevBuffer(void);
    void          vPrevBuffer(BufferNo tPrevBufferNo);
    void          vPrevBuffer(LinkedBuffer* pPrevBuffer);
  };
  
  LinkedBuffer* _pLinkedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
