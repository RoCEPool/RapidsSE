/*============================================================================*/
/* Tub.hpp -- Chain of Buffers with dangling support                            */
/*============================================================================*/
#pragma once

#include "CrwBuffer.hpp"
#include "RowBuffer.hpp"

namespace STORE
{
  class Tub : public Object
  {
  private:
    DataBuffer*  m_pBufferChain;
  
    CrwBuffer*   m_pInCrwBuffer;
    RowBuffer*   m_pInRowBuffer;

    RowPix*      m_pRowPix;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Tub(void);
   ~Tub(void) noexcept(false);
    
    static void vRepair(Object* pObject);

    static void* aAllotRow(Tub* pTub, char** aazStrArea);
    static void  vDiscardRow(Tub* pTub);
    static void  vRowStrSize(Tub* pTub, i8 nRowStrSize);

  public:
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    CrwBuffer*  pInCrwBuffer(void);
    void        vInCrwBuffer(CrwBuffer* pCrwBuffer);
    RowBuffer*  pInRowBuffer(void);
    void        vInRowBuffer(RowBuffer* pRowBuffer);
    bool        bIsEmpty(void);
    RowPix*     pRowPix(void);
    void        vRowPix(RowPix* pRowPix);
    DataBuffer* pWorkingBuffer(void);

    Footer* aFooter(void);
  };
  
  Tub* _pTub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
