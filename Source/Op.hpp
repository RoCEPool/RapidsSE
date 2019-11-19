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
    OpDefn*        m_aOpDefn;        // op definition
    OpSpec*        m_pOpSpec;        // op specification
  
    s32            m_sNextLabel;     // label of this op
    
    //! ALKA: These should go into OpObjectName
    s32            m_sObjectName;    // object name (e.g. schema, table etc)
    s32            m_sIndexName;     // index name
    Table*         m_pTable;         // table 
    
    Job*           m_pJobPipeline;   // pipeline job this operator belongs to

    Job*           m_pJobUniversal;  // universal job this operator belongs to
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
    
    StreamDef*     m_pStreamDef;
  
    bool           m_bFirstCall;      // first time being called?
    
    ePX            m_eTopologyX;      // cross topology
    i8             m_iNextStage;      // next stage in the cross topology
    
    eOR (*m_fpCall) (void* aThisOp);  // function which calls the op loop
    eOR (*m_fpDrain)(void* aThisOp);  // function which calls the drain loop

  public:
    void  operator delete(void*);
  
    Op(eCN eClass, OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op(void) noexcept(false);
  
    static Op* pThisTLSOp(void);

    // - - - - - - - - - - - - - - - -
    void vOpLoop(void);
  
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
    bool bPause(void);
    void vPipeOut(Operand* pOperand);
    void vPipeOut_endMarker(void);
    void vPipeToJob(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueue2(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    OperandQueue* pQueueLeft(void);
    OperandQueue* pQueueRight(void);
    bool bSpawnJob(i8 iSessionId);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vWorkQuanta(i8 iWorkQuanta);
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
  bool _bIsValidOp(eCN eClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
