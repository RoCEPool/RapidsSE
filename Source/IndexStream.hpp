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
    Index*     m_pIndex;
    EntBuffer* m_pEntBuffer;
    Ent*       m_aLowerEntry;
    Ent*       m_aUpperEntry;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexStream(TablePtn* pTablePtn, Index* pIndex);
   ~IndexStream(void) noexcept(false);
  
    void vClose(void);
    bool bDelete(void);
    bool bOpen(void);
    CrwBuffer* pRead(RowOnce* pRowOnce);
    void vReset(void);
    void vSeek(eRO eLowerRelnOp, Stz* szLowerKey, eRO eUpperRelnOp, Stz* szUpperKey);

  private:
    Footer* aFooter(void);
  };
  
  IndexStream* _pIndexStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
