/*============================================================================*/
/* LinkedBuffer.hpp -- Linked Buffer                                          */
/*============================================================================*/
#pragma once

namespace BASE
{
  class LinkedBuffer : public Operand
  {
  protected:
    LinkedBuffer* m_pNextBuffer;   // next buffer
    LinkedBuffer* m_pPrevBuffer;   // previous buffer
  
    LinkedBuffer* m_pHigherBuffer; // higher buffer
    LinkedBuffer* m_pLowerBuffer;  // lower buffer
  
    BufferNo m_iNextBuffer;   // next buffer
    BufferNo m_iPrevBuffer;   // previous buffer

    BufferNo m_iHigherBuffer; // higher buffer
    BufferNo m_iLowerBuffer;  // lower buffer

    //i8 m__padToCacheLine[4];

  private:
    //SingleWord    m__PadToCacheLine[4]; // padding to cache line

  public:
    LinkedBuffer(eCN eClass);
    ~LinkedBuffer(void) noexcept(false);
  
    void vLinkAsNext(LinkedBuffer* pArgBuffer);
    void vLinkAsPrev(LinkedBuffer* pArgBuffer);
    void vUnLink(void);

    LinkedBuffer* pHigherBuffer(void);
    void          vHigherBuffer(LinkedBuffer* pHigherBuffer);
  
    LinkedBuffer* pLowerBuffer(void);
    void          vLowerBuffer(LinkedBuffer* pLowerBuffer);
  
    LinkedBuffer* pNextBuffer(void);
    void          vNextBuffer(LinkedBuffer* pNextBuffer);
  
    LinkedBuffer* pPrevBuffer(void);
    void          vPrevBuffer(LinkedBuffer* pPrevBuffer);
  };
  
  LinkedBuffer* _pLinkedBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
