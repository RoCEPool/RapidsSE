/*============================================================================*/
/* Job_Xmit.hpp -- Write Row Job                                          */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Xmit : public Job,
                       public JobExtractRow
  {
  public:
    i8         m_iNode;           // node id
    i8         m_iParentJobId;    // parent (InsertRow) job id
    Operand*   m_pOperand;        // operand to xmit
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Xmit(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
    ~Job_Xmit(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    static void vSpawnJobXmit(Session* pSession,
                              Operand* pOperand,
                              i8 iParentJobId,
                              i8 iNode);
    
  public:
    void vInsertRow(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Xmit* _pJob_Xmit(void* aAddr);
  
  Job* pJob_Xmit_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);
  Job* pJob_Xmit_new(JobClass* aJobClass);
  void vJob_Xmit_delete(Job* pJob);
  void vJob_Xmit_show(Job* pJob);
  
  JobClass* pJob_Xmit_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
