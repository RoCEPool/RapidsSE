/*============================================================================*/
/* DataBuffer.hpp -- Rows Buffer                                              */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class DataBuffer : public LinkedBuffer
  {
  protected:
    Lock   m_okLock;        // lock
    i8     m_iBufferNo;     // this DataBuffer's buffer number
    i8     m_iPartitionNo;  // partition no this buffer belongs to
    i8     m_iBufferHeight; // buffer height in the tree
    Birth  m_oWhenNewed;    // when was this buffer created?
    b8     m_bIsPermanent;  // is buffer permanent ?

  public:
    DataBuffer(eCN eClass, bool bIsPermanent);
   ~DataBuffer(void) noexcept(false);

    static DataBuffer* pRefdDataBuffer(Rrf& xRef);
    
    i8          iAgeSec(void);

    i8          iBufferHeight(void);
    void        vBufferHeight(i8 iBufferHeight);
  
    void        vDropWrite(void);
    void        vGrabWrite(void);
  
    void        vPartitionNo(i8 iPartitionNo);
    i8          iPartitionNo(void);
    
    Birth&      oWhenNewed(void);
  
    // - - - - - - - - - - - - - -
    bool bIsPermanent(void);
    void vIsPermanent(bool bIsPermanent);
    i8   iBufferNo(void);
  };
  
  DataBuffer* _pDataBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
