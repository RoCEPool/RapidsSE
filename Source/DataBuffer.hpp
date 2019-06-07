/*============================================================================*/
/* DataBuffer.hpp -- Data Buffer                                              */
/*============================================================================*/
#pragma once

namespace Data
{
  class DataBuffer : public Object
  {
  protected:
    Lock   m_okLock;       // lock
    i8     m_iBufferNo;    // this DataBuffer's buffer number
    i8     m_iPartitionNo; // partition no this buffer belongs to
    i8     m_iBufferHeight;    // buffer height in the tree
    Since  m_oWhenNewed;   // when was this buffer created?

    DataBuffer* m_pNextBuffer;  // next buffer
    DataBuffer* m_pPrevBuffer;  // previous buffer
    DataBuffer* m_pHigherBuffer; // higher buffer

  public:
    DataBuffer(eCN eClass);
    ~DataBuffer(void) noexcept(false);

    i8          iAgeSsec(void);

    void        vBufferHeight(i8 iBufferHeight);
    i8          iBufferHeight(void);
  
    void        vDropWrite(void);
    void        vGrabWrite(void);

    DataBuffer* pHigherBuffer(void);
    void        vHigherBuffer(DataBuffer* pHigherBuffer);
  
    DataBuffer* pNextBuffer(void);
    void        vNextBuffer(DataBuffer* pNextBuffer);
  
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);

    DataBuffer* pPrevBuffer(void);
    void        vPrevBuffer(DataBuffer* pPrevBuffer);
 
    Since&      oWhenNewed(void);
  };
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
