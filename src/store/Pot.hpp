/*============================================================================*/
/* Pot.hpp -- Working Pot of Buffers                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Pot : public Object
  {
  private:
  
    RowPix* m_pRowPix;    // condensed stream def

    DataBuffer*  m_paBufferChain[C_MAX_PTNSINGRID];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pot(void);
   ~Pot(void) noexcept(false);
  
  public:

    static void* aAllotRow(Pot* pPot, i8 iMixIn, char** aazStrArea);
    static void  vRepair(Object* pObject);

    DataBuffer* pBuffer(i8 i_Ptn);
    DataBuffer* pCompleted(i8 i_Ptn);
    DataBuffer* pDangling(i8 i_Ptn);
    Row* aNewDstRow(i8 i_Ptn, RowBuffer** ppRowBuffer, bool* pbNewBuffer);
  
    void vRowPix(RowPix* pRowPix);
  
  private:
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    Footer* aFooter(void);
  };

  Pot* _pPot(void* aAddr);
  void* aPot_AllotRow(Pot* pPot, i8 iMixIn, char** aazStrArea);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
