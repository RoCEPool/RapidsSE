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

    LinkedBuffer* pHigherLinkedBuffer(void);
    void          vHigherLinkedBuffer(BufferNo tHigherBufferNo);
    void          vHigherLinkedBuffer(LinkedBuffer* pHigherBuffer);

    LinkedBuffer* pLowerLinkedBuffer(void);
    void          vLowerLinkedBuffer(BufferNo tLowerBufferNo);
    void          vLowerLinkedBuffer(LinkedBuffer* pLowerBuffer);
  
    LinkedBuffer* pNextLinkedBuffer(void);
    void          vNextLinkedBuffer(BufferNo tNextBufferNo);
    void          vNextLinkedBuffer(LinkedBuffer* pNextBuffer);
  
    LinkedBuffer* pPrevBuffer(void);
    void          vPrevBuffer(BufferNo tPrevBufferNo);
    void          vPrevBuffer(LinkedBuffer* pPrevBuffer);
  };
  
  LinkedBuffer* _pLinkedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
