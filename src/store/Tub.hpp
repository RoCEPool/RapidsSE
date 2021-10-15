/*============================================================================*/
/* Tub.hpp -- Chain of Buffers with dangling support                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Tub : public Object
  {
  private:
    DataBuffer*  m_pBufferChain;
  
    CrwBuffer*   m_pInCrwBuffer;
    RowBuffer*   m_pInRowBuffer;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Tub(void);
   ~Tub(void) noexcept(false);
    
    static void vRepair(Object* pObject);

    static void vFun0(Tub* pTub);
    static void vFun1(Tub* pTub);
    static void vFun2(Tub* pTub);

  public:
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    CrwBuffer*  pInCrwBuffer(void);
    void        vInCrwBuffer(CrwBuffer* pCrwBuffer);
    RowBuffer*  pInRowBuffer(void);
    void        vInRowBuffer(RowBuffer* pRowBuffer);
    bool        bIsEmpty(void);
    DataBuffer* pWorkingBuffer(void);

    Footer* aFooter(void);
  };
  
  Tub* _pTub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
