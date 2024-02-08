/*============================================================================*/
/* Pot.hpp -- Working Pot of Buffers                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  typedef struct {
    i8 iLglPtn;
    i8 iSynthKey;
    DataBuffer* pDataBuffer;
  } PotPdsItem;

  class Pot : public Object
  {
  private:
  
    RowPix* m_pRowPix;    // condensed stream def

    i8                      m_iSynthKey;     // Synthetic key used to append buffers to the Pds structure
    Pds_t<PotPdsItem>*      m_pPdsOfBuffers; // Pds of buffers

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pot(void);
   ~Pot(void) noexcept(false);
  
  public:

    static void* aAllotRowPds(Pot* pPot, i8 iKeyHsh, char** aazStrArea, i8* pnMaxStrAreaLen, i8* piErrorCode);
    static void  vRowStrSize(Pot* pPot, i8 iKeyHsh, i8 nRowStrSize);
    static void  vRepair(Object* pObject);
    static void  vSetRowPix(Pot* pPot, RowPix* pRowPix);

    DataBuffer* pBuffer(i8 i_Ptn);
    DataBuffer* pCompleted(i8 i_Ptn);
    DataBuffer* pDangling(i8 i_Ptn);
    Row* aNewDstRow(i8 i_Ptn, Pds** ppPdsRowBuffer, bool* pbNewBuffer);
  
    void vRowPix(RowPix* pRowPix);
  
    i8 nBuffers(void);
  
  private:
    void vAddBuffer(i8 i_Ptn, DataBuffer* pDataBuffer);

    Footer* aFooter(void);
  };

  static i8 iPotPdsItemKeyCompare(void* aItem_l, void* aItem_r);

  Pot* _pPot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
