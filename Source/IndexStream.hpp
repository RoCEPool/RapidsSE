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
    
    CrwBuffer* m_pNextBuffer;
    i8         m_iLglBufferNo;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    IndexStream(TablePtn* pTablePtn, Index* pIndex);
   ~IndexStream(void) noexcept(false);
  
    i8   iBufferNo(void);
    void vClose(void);
    bool bOpen(void);
    CrwBuffer* pRead(RowOnce* pRowOnce);
    void vReset(void);
    void vPosition(eRO eLowerRelnOp, Stz* szLowerKey, eRO eUpperRelnOp, Stz* szUpperKey);

  private:
    Footer* aFooter(void);
  };
  
  IndexStream* _pIndexStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
