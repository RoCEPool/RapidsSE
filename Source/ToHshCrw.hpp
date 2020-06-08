/*============================================================================*/
/* ToHshCrw.hpp -- Convert ptn nst rsb buffers to hashed crw buffers          */
/*============================================================================*/
#pragma once

namespace Load
{
  class ToHshCrw : public Object
  {
  private:
  
    Table*     m_pTable;
    StreamDef* m_pStreamDef;
    CrwBuffer* m_pCrwBuffer;

    HashSet    m_oHashSet;
  
    RowCompactor* m_paRowCompactor[C_MAX_PARTITIONS]; // row compactors

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ToHshCrw(StreamDef* pStreamDef);
   ~ToHshCrw(void) noexcept(false);
  
    void vConvert(RsbBuffer* pRsbBuffer);
  
    CrwBuffer* pCompleted(i8 i_Ptn);
    CrwBuffer* pDangling(i8 i_Ptn);
  
  private:
    void vAddNst(RsbBuffer* pRsbBuffer);
    void vAddRsbRows(i8 i_Ptn, RsbBuffer* pRsbBuffer);
    CrwBuffer* pNewCrwBuffer(i8 i_Ptn);

    Footer* aFooter(void);
  };

  ToHshCrw* _pToHshCrw(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
