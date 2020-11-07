/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Op : public Focal
  {
  private:
    s32            m_sOpNm;          // name of the operator
    OpSpec*        m_pOpSpec;        // specification of this operator
    Stage*         m_pStage;         // which stage of pipeline
    Pipeline*      m_pPipeline;      // which pipeline
  
    Job*           m_pPipelineJob;   // pipeline job this operator belongs to
    Job*           m_pOpJob;         // operator job this operator belongs to
  
    TagSet         m_oOpTagSet;        // op tag set
    TagSet         m_oTopologyTagSet;  // topology tag set
    
    bool           m_bPulsedLimit;   // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    bool           m_bAppendRight;   // append to right queue?
    OperandQueue*  m_pQueueLeft;     // the input queue being processed
    OperandQueue*  m_pQueueRight;    // the input queue being processed
    
    i8             m_iNode;          // node no
    i8             m_tPtnNo;         // partition no (in grid)
    
    ePX            m_eTopologyX;     // cross topology
    i8             m_iNextStage;     // next stage in the cross topology
    i8             m_iNextOp;        // next Op in the stage
    i8             m_iXmitNode;      // xmit node (for xmit)
    eXK            m_eXmitKind;      // xmit kind
    i8             m_iRobin;         // xmit Round Robin partition/node/thread
    i8             m_iLocalRobin;    // xmit Round Robin local partition/node/thread
  
    Op*            m_pDownstreamOp;  // downstream op
  
    Word* m_paOperandHandler[ZK_ENUMS];  // array of operand handlers (by kind)
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(OpSpec* pOpSpec, Stage* pStage, Pipeline* pPipeline);
   ~Op(void) noexcept(false);
  
    void vDump(i8 i_Op);

    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppendLeft(Operand* pOperand, Task* pTriggeringTask, bool bSet = false);
    void vAppendRight(Operand* pOperand, Task* pTriggeringTask, bool bSet = false);
    bool bAppendRight(void);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    Op*  pDownstreamOp(void);
    void vDownstreamOp(Op* pDownstreamOp);
    void vDropTaskLock(void);
    void vDump(void);
    void vFocalOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    void vIncWorkDone(void);
    void vInitOperandHandlers(void);
    bool bIsLocked(void);
    void vJobPipeline(Job* pJob);
    i8   iNextOp(void);
    Op*  pNextOp(void);
    void vNextOp(i8 iNextOp);
    i8   iNextStage(void);
    void vNextStage(i8 iNextStage);
    i8   iNode(void);
    void vNode(i8 iNode);
    Word* pOperandHandler(i8 i_OperandKind);
    void vOperandHandler(i8 i_OperandKind, Word* pHandler);
    Job* pOpJob(void);
    Stz* szOpNm(void);
    TagSet* pOpTagSet(void);
    void vPtnNo(tPN tPtnNo);
    tPN  tPtnNo(void);
    bool bPause(void);
    bool bPeekQueue(void);
    bool bPeekQueueOoO(void);
    Pipeline* pPipeline(void);
    void vPipeOut_nextOp(Operand* pOperand, Task* pTriggeringTask, bool bSet = false);
    void vPipeOut_thisOp(Operand* pOperand, Task* pTriggeringTask, bool bSet = false);
    void vPipeToJob(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueueOoO(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    OperandQueue* pQueueLeft(void);
    i8   nQueueLength(void);
    OperandQueue* pQueueRight(void);
    void vSendXmitOperand(Operand* pOperand, Task* pTriggeringTask, bool bSet);
    void vSpawnOperatorJob(i8 iSessionId);
    Stage* pStage(void);
    Stz* szStageName(void);
    Task* pTask(void);
    void vToCmdStr(Stz* szCmdStr);
    TagSet* pTopologyTagSet(void);
    ePX  eTopologyX(void);
    void vTopologyX(ePX eTopologyX);
    void vWorkQuanta(i8 iWorkQuanta);
    eXK  eXmitKind(void);
    void vXmitKind(eXK eXmitKind);
    i8   iXmitNode(void);
    void vXmitNode(i8 iNde);
    
    Footer* aFooter(void);
    
  private:
    void vSend(Operand* pOperand, bool bSet, i8 i_Node, tPN tPtnNo);
    void vSendToAllNodes(Operand* pOperand, Task* pTriggeringTask,bool bSet, eXK eXmitKind);
    void vSendToAllOriginPartitions(Operand* pOperand, Task* pTriggeringTask,bool bSet, eXK eXmitKind);
    void vSendToAllPartitions(Operand* pOperand, Task* pTriggeringTask,bool bSet, eXK eXmitKind);
    void vSendToNode(Operand* pOperand, Task* pTriggeringTask,bool bSet, eXK eXmitKind,i8 i_Node);
    void vSendToOrigin(Operand* pOperand, Task* pTriggeringTask,bool bSet, eXK eXmitKind);
    void vSendToPartition(Operand* pOperand, Task* pTriggeringTask, bool bSet,
                          eXK eXmitKind, tPN tPtnNo);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
