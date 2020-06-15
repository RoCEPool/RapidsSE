/*============================================================================*/
/* Job_Operand.hpp -- Xmit Job                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Operand : public Job
  {
  public:
    i8         m_iNode;           // node id
    i8         m_iPartition;      // partition
    ePX        m_eTopologyX;      // cross topology
    i8         m_iNextStage;      // next stage
    i8         m_iNextOp;         // nextOp in the nextStage
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Operand(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Operand(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    static Job_Operand* pNewJobOperand(Operand* pOperand, i8 iParentJobId, Op* pOp, i8 iNde, i8 iPtn);
    
  public:
    void vInsertRow(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Operand* _pJob_Operand(void* aAddr);
  
  Job* pJob_Operand_pre(JobClass* aJobClass, ReqTuple* aReqTuple, TagSet* pTagSet);
  Job* pJob_Operand_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Operand_delete(Job* pJob);
  void vJob_Operand_show(Job* pJob);
  
  JobClass* pJob_Operand_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
