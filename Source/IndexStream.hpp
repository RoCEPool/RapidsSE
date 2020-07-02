/*============================================================================*/
/* IdxStream.hpp -- Index Stream                                              */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class IndexStream : public Object
  {
  private:
    TablePtn*  m_pTablePtn;
    CrwBuffer* m_pNextBuffer;
    i8         m_iLglBufferNo;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexStream(TablePtn* pTablePtn);
   ~IndexStream(void) noexcept(false);
  
    i8   iBufferNo(void);
    void vClose(void);
    bool bOpen(void);
    CrwBuffer* pRead(void);
    void vReset(void);
    bool bSeek(i8 iBufferNo);

  private:
    Footer* aFooter(void);
  };
  
  IndexStream* _pIndexStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
