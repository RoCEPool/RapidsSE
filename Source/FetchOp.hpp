/*============================================================================*/
/* FetchOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class FetchOp : public Op
  {
  private:
    Table*     m_pTable;
    Index*     m_pIndex;
    TablePtn*  m_pTablePtn;
    EntBuffer* m_pEntBuffer;
  
    bool       m_bRunPrelim;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FetchOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~FetchOp(void) noexcept(false);

    static Op* pNew(Session* pSession, Stz* szLabel,
                    Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
    void vPreliminary(void);

    void vTablePtn(TablePtn* pTablePtn);
  
  private:
    Footer* aFooter(void);
  };

  FetchOp* _pFetchOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
