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
  
    static Object*     pToAllocatedObject(i8 iBufferNo);
  
    static i8          iToBufferNo(Object* pObject);

    static HeapBuffer* pToHeapBuffer(i8 iBufferNo);
  
    static LinkedBuffer* pToLinkedBuffer(i8 iBufferNo);
  
    static PoolBuffer* pToPoolBuffer(i8 iBufferNo);

    static BufferNo tUNKNOWN(void);

    void operator =(i8 iBufferNo); // assignment
    void operator =(Object* pObject); // assignment
  
    i8 iGetBuffer(void);
    LinkedBuffer* pGetLinkedBuffer(void);
    PoolBuffer* pGetPoolBuffer(void);


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
