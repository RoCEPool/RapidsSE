/*============================================================================*/
/* PtnStm_cursor.hpp -- Partition Stream cursor                               */
/*============================================================================*/
#pragma once

namespace STORE
{
  class PtnStm_cursor : public Object
  {
  private:
    TablePtn*  m_pTablePtn;

    i8         m_iLglBufferNo;

    Bucket*    m_pCurBkt;
    DataBuffer* m_pCurBuffer;
    i8         m_iCurBktNo;
    PtnId      m_tCurPtnNo;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnStm_cursor(TablePtn* pTablePtn);
    ~PtnStm_cursor(void) noexcept(false);

    static void vRepair(Object* pObject, eRF eRepair);

    i8   iBufferNo(void);
    void vClose(void);
    bool bOpen(void);
    DataBuffer* pRead(void);
    void vReset(void);
    bool bSeek(i8 iBufferNo);

  private:
    Footer* aFooter(void);
  };
  
  PtnStm_cursor* _pPtnStm_cursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
