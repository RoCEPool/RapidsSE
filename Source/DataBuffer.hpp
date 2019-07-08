/*============================================================================*/
/* DataBuffer.hpp -- Rows Buffer                                              */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class DataBuffer : public OperandBuffer
  {
  protected:
    Lock   m_okLock;        // lock
    i8     m_iBufferNo;     // this DataBuffer's buffer number
    i8     m_iPartitionNo;  // partition no this buffer belongs to
    i8     m_iBufferHeight; // buffer height in the tree
    Birth  m_oWhenNewed;    // when was this buffer created?

  public:
    DataBuffer(eCN eClass);
    ~DataBuffer(void) noexcept(false);

    i8          iAgeSec(void);

    void        vBufferHeight(i8 iBufferHeight);
    i8          iBufferHeight(void);
  
    void        vDropWrite(void);
    void        vGrabWrite(void);
  
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);
 
    Birth&      oWhenNewed(void);
  };
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
