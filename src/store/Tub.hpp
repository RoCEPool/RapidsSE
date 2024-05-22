/*============================================================================*/
/* Tub.hpp -- Chain of Buffers with dangling support                            */
/*============================================================================*/
#pragma once

#include "CrwBuffer.hpp"

namespace STORE
{
  class Tub : public Object
  {
  private:
    PdsBufferQueue* m_pBufferAddrQueue;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Tub(void);
   ~Tub(void) noexcept(false);
    
    static void vRepair(Object* pObject);

		static void  vRowStrSize(Tub* pTub, i8 nRowStrSize);
    static void  vToCsv(StreamDef* pStreamDef, Tub* pTub, Pds* pPds);

  public:
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    bool        bIsEmpty(void);
    DataBuffer* pWorkingBuffer(void);

    Footer* aFooter(void);
  };
  
  Tub* _pTub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
