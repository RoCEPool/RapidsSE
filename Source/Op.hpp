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
  
  private:
    s32            m_sName;          // first word of prompt command
    
    Pipeline*      m_pPipeline;      // op digest
    EnameSet       m_oEnameSet;      // ename set of cmd
    
    Job*           m_pJobPipeline;   // pipeline job this operator belongs to
    Job*           m_pOpJob;         // operator job this operator belongs to
  
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

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op(void) noexcept(false);

    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppendLeft(Operand* pOperand);
    void vAppendRight(Operand* pOperand);
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
    Stz* szName(void);
    i8   iNextOp(void);
    void vNextOp(i8 iNextOp);
    i8   iNextStage(void);
    void vNextStage(i8 iNextStage);
    i8   nNode(void);
    void vNode(i8 nNode);
    Job* pOpJob(void);
    void vPartition(i8 iPartition);
    i8   nPartition(void);
    bool bPause(void);
    Pipeline* pPipeline(void);
    void vPipeOut(Operand* pOperand);
    void vPipeToJob(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    OperandQueue* pQueueLeft(void);
    OperandQueue* pQueueRight(void);
    i8   nRightEosMarkers(void);
    void vRightEosMarkers(i8 nEosMarkers);
    void vSendJobOperand(Operand* pOperand);
    bool bSpawnJob(i8 iSessionId);
    Task* pTask(void);
    void vToCmdStr(Stz* szCmdStr);
    ePX  eTopologyX(void);
    void vTopologyX(ePX eTopologyX);
    void vWorkQuanta(i8 iWorkQuanta);
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
