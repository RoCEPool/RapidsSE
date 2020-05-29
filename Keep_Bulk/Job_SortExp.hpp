/*============================================================================*/
/* Job_SortExp.hpp -- Sort Job                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_SortExp : public JobMark2
  {
  public:

    i8          m_iBuffers;
    Digest*     m_pDigest;     // op digest
  
    GenerateOp* m_pGenerateOp; // generate
    SortOp*     m_pSortOp;     // sort
    MergeOp*    m_pMergeOp;    // merge
    OutputOp*   m_pOutputOp;   // output
    ZedOp*      m_pZedOp;      // sink

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SortExp(Session* pSession);
    ~Job_SortExp(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
    void vBegin(void);
    void vEnd(void);
    void vExecute(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SortExp* _pJob_SortExp(void* aAddr);
  
  Job* pJob_SortExp_pre(Session* pSession);
  void vJob_SortExp_delete(Session* pSession, Job* pJob);
  void vJob_SortExp_show(Job* pJob);
  
  JobClass* pJob_SortExp_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
