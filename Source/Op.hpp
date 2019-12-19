/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage;
  class Op : public Focal
  {
  friend class Pipeline;
  
  public:
    s32            m_sName;          // first word of prompt command
    
    Pipeline*      m_pPipeline;      // op digest
    OpSpec*        m_pOpSpec;        // op specification
  
    s32            m_sNextLabel;     // label of this op
    
    Job*           m_pJobPipeline;   // pipeline job this operator belongs to
    Job*           m_pOpJob;         // job this operator belongs to
    Task*          m_pTask;          // task running this op
  
    bool           m_bAckJobEnd;     // ack job on end marker
    bool           m_bPulsed;        // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    OperandQueue*  m_pQueueLeft;     // the input queue being processed
    OperandQueue*  m_pQueueRight;    // the input queue being processed
  
    Op*            m_pOpOut;         // to which op the results get piped
    bool           m_bAppendRight;   // append to right queue?
  
    i8             m_nNode;               // node no
    i8             m_nPartition;          // partition no
    i8             m_nLeftEndMarkers;     // expected left end markers
    i8             m_nRightEndMarkers;    // expected left end markers
    
    ePX            m_eTopologyX;      // cross topology
    i8             m_iNextStage;      // next stage in the cross topology
    
    i8             m_iXmitNode;       // xmit node (for xmitOp)
    

    //! ALKA: should go
    eOR (*m_fpCall) (void* aThisOp);  // function which calls the op loop

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(eCN eClass, OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);
  
    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAckJobEnd(bool m_bAckJobEnd);
    void vAppendLeft(Operand* pOperand);
    void vAppendRight(Operand* pOperand);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    void vDropTaskLock(void);
    void vFocalOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    void vIncWorkDone(void);
    bool bIsCancelling(void);
    bool bIsLocked(void);
    void vJobPipeline(Job* pJob);
    void vLinkToStage(Stage* pStage);
    Stz* szName(void);
    Stz* szNextLabel(void);
    void vNextLabel(Stz* szNextLabel);
    i8   iOpId(void);
    void vOpOut(Op* pOpOut);
    void vPartition(i8 iPartition);
    bool bPause(void);
    void vPipeOut(Operand* pOperand);
    void vPipeToJob(Operand* pOperand);
    void vPipeToStart(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueue2(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    OperandQueue* pQueueLeft(void);
    OperandQueue* pQueueRight(void);
    bool bSpawnJob(i8 iSessionId);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vToCmdStr(Stz* szCmdStr);
    void vWorkQuanta(i8 iWorkQuanta);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
