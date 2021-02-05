/*============================================================================*/
/* BufferNo.hpp -- buffer number                                              */
/*============================================================================*/
#pragma once

namespace BASE { class LinkedBuffer; }

namespace BASE
{
  class BufferNo
  {
  public:
    i8 m_iBufferNo;
  
  public:
    BufferNo(void);
   ~BufferNo(void) noexcept(false);
  
    static i8            iToBufferNo(LinkedBuffer* pLinkedBuffer);
    static LinkedBuffer* pToLinkedBuffer(i8 iBufferNo);
  
    void operator =(i8 iBufferNo); // assignment
    void operator =(LinkedBuffer* pLinkedBuffer); // assignment
  
    i8 iGetBuffer(void);
    LinkedBuffer* pGetBuffer(void);


  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
//  inline CacheLine* _aCacheLine(void* aAddr)
//  {
//    CacheLine* aCacheLine = reinterpret_cast<CacheLine*>(aAddr);
//
//    return aCacheLine;
//  }
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
