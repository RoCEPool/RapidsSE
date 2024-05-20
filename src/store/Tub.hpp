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

    RowPix*      m_pRowPix;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Tub(void);
   ~Tub(void) noexcept(false);
    
    static void vRepair(Object* pObject);

    static void* aAllotRowPds(Tub* pTub, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode);
		static Row*  pAllotRow(Tub* pTub, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode);
    static void  vRowStrSize(Tub* pTub, i8 nRowStrSize);
    static void  vSetRowPix(Tub* pTub, RowPix* pRowPix);
    static void  vToCsv(StreamDef* pStreamDef, Tub* pTub, Pds* pPds);

  public:
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    bool        bIsEmpty(void);
    RowPix*     pRowPix(void);
    void        vRowPix(RowPix* pRowPix);
    DataBuffer* pWorkingBuffer(void);

    Footer* aFooter(void);
  };
  
  Tub* _pTub(void* aAddr);
  void  vTub_RowStrSize(void* pTub, i8 nRowStrSize);
  void vTub_RowPix(void* pTub, void* aRowPix);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
