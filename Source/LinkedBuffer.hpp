/*============================================================================*/
/* LinkedBuffer.hpp -- Linked Buffer                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class LinkedBuffer : public AlignedObject
  {
  protected:
    LinkedBuffer* m_pNextBuffer;   // next buffer
    LinkedBuffer* m_pPrevBuffer;   // previous buffer
  
    LinkedBuffer* m_pHigherBuffer; // higher buffer
    LinkedBuffer* m_pLowerBuffer;  // lower buffer
  
  private:
    Word          m__Padding[3];   // padding to cache line

  public:
    LinkedBuffer(eCN eClass);
    ~LinkedBuffer(void) noexcept(false);

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
