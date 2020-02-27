/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Op : public Focal
  {
  friend class Pipeline;
  
  private:
    s32            m_sOpNm;          // name of the operator
    OpSpec*        m_pOpSpec;        // specification of this operator
    Stage*         m_pStage;         // which stage of pipeline
    Pipeline*      m_pPipeline;      // which pipeline
  
    Job*           m_pJobPipeline;   // pipeline job this operator belongs to
    Job*           m_pOpJob;         // operator job this operator belongs to
  
    EnameSet       m_oEnameSet;         // ename set of cmd
    EnameSet       m_oTopologyEnameSet; // topology enameset
    
    bool           m_bPulsed;        // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    bool           m_bAppendRight;   // append to right queue?
    OperandQueue*  m_pQueueLeft;     // the input queue being processed
    OperandQueue*  m_pQueueRight;    // the input queue being processed
    
    i8             m_nNode;            // node no
    i8             m_nPartition;       // partition no
    i8             m_nLeftEosMarkers;  // expected left end markers
    i8             m_nRightEosMarkers; // expected left end markers
    
    ePX            m_eTopologyX;      // cross topology
    i8             m_iNextStage;      // next stage in the cross topology
    i8             m_iNextOp;         // next Op in the stage
    i8             m_iXmitNode;       // xmit node (for xmitOp)
    eXK            m_eXmitKind;       // xmit kind
    i8             m_iRobin;          // xmit Round Robin partition/node/thread
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(OpSpec* pOpSpec, Stage* pStage, Pipeline* pPipeline);
   ~Op(void) noexcept(false);

    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppendLeft(Operand* pOperand, bool bSet = false);
    void vAppendRight(Operand* pOperand, bool bSet = false);
    bool bAppendRight(void);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    i8   nDecLeftEosMarkers(void);
    void vDropTaskLock(void);
    EnameSet* pEnameSet(void);
    void vFocalOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    void vIncWorkDone(void);
    bool bIsCancelling(void);
    bool bIsLocked(void);
    void vJobPipeline(Job* pJob);
    i8   nLeftEosMarkers(void);
    void vLeftEosMarkers(i8 nEosMarkers);
    i8   iNextOp(void);
    void vNextOp(i8 iNextOp);
    i8   iNextStage(void);
    void vNextStage(i8 iNextStage);
    i8   nNode(void);
    void vNode(i8 nNode);
    Job* pOpJob(void);
    Stz* szOpNm(void);
    void vPartition(i8 iPartition);
    i8   nPartition(void);
    bool bPause(void);
    Pipeline* pPipeline(void);
    void vPipeOut(Operand* pOperand, bool bSet = false);
    void vPipeToJob(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    OperandQueue* pQueueLeft(void);
    OperandQueue* pQueueRight(void);
    i8   nRightEosMarkers(void);
    void vRightEosMarkers(i8 nEosMarkers);
    void vSendJobOperand(Operand* pOperand, bool bSet);
    bool bSpawnJob(i8 iSessionId);
    Task* pTask(void);
    void vToCmdStr(Stz* szCmdStr);
    ePX  eTopologyX(void);
    void vTopologyX(ePX eTopologyX);
    void vWorkQuanta(i8 iWorkQuanta);
    eXK  eXmitKind(void);
    void vXmitKind(eXK eXmitKind);
    i8   iXmitNode(void);
    void vXmitNode(i8 iNde);
    
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
