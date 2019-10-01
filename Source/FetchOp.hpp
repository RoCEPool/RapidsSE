/*============================================================================*/
/* FetchOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class FetchOp : public Op
  {
  private:
    s256       m_sTableNm;
    s256       m_sIndexNm;
    Table*     m_pTable;
    Index*     m_pIndex;
    TablePtn*  m_pTablePtn;
    EntBuffer* m_pEntBuffer;
    i8         m_nLimit;     
    
    bool       m_bRunPrelim;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FetchOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~FetchOp(void) noexcept(false);

    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
    void vPreliminary(void);
  
  private:
    Footer* aFooter(void);
  };

  FetchOp* _pFetchOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
