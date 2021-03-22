/*============================================================================*/
/* PtnStream.hpp -- Partition Stream                                          */
/*============================================================================*/
#pragma once

namespace STORE
{
  class PtnStream : public Object
  {
  private:
    TablePtn*  m_pTablePtn;

    union {
      CrwBuffer* m_pNextCrwBuffer;
      RowBuffer* m_pNextRowBuffer;
    };

    i8         m_iLglBufferNo;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnStream(TablePtn* pTablePtn);
   ~PtnStream(void) noexcept(false);
  
    i8   iBufferNo(void);
    void vClose(void);
    bool bOpen(void);
    DataBuffer* pRead(void);
    void vReset(void);
    bool bSeek(i8 iBufferNo);

  private:
    Footer* aFooter(void);
  };
  
  PtnStream* _pPtnStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
