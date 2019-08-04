/*============================================================================*/
/* FetchOp.hpp -- Fetch                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class FetchOp : public Op
  {
  private:
    TablePtn*  m_pTablePtn;
    CrwBuffer* m_pCrwBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    FetchOp(Stz* szOpLabel, Digest* pDigest, Jobs::Job* pJob);
   ~FetchOp(void) noexcept(false);

    static Op* pNew(Digest* pDigest, Job_Dynamic* pJob, Stz* szLabel,
                    TokenVec* pTknVec, i8& i_Tkn_);
    static eOR eCall(void* aThis);
  
    eOR eFetchLoop(void);
    void vTablePtn(TablePtn* pTablePtn);
  
  private:
    Footer* aFooter(void);
  };

  FetchOp* _pFetchOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
