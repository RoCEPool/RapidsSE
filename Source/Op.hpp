/*============================================================================*/
/* Op.hpp -- Operator                                                         */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Op : public Focal
  {
  friend class Digest;
  
  protected:  
    s32            m_sName;          // first word of prompt command
  
    OpSpec*        m_pOpSpec;        // op specification
  
    s32            m_sLabel;         // label of this op
    s256           m_sArgs;          // args of this op   (initial dev only)
    s32            m_sNextLabel;     // label of this op
  
    OpDefn*        m_aOpDefn;        // op definition
    Digest*        m_pDigest;        // op digest
    Job*           m_pJob;           // job this operator belongs to
    Task*          m_pTask;          // task running this op
  
    bool           m_bAckJobEnd;     // ack job on end marker
    bool           m_bPulsed;        // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    OperandQueue*  m_pQueueLeft;     // the input queue being processed
    OperandQueue*  m_pQueueRight;    // the input queue being processed
  
    Op*            m_pOpOut;         // to which op the results get piped
    bool           m_bAppendRight;   // append to right queue?
  
    i8             m_nSlot;               // slot no
    i8             m_nLeftEndMarkers;     // expected left end markers
    i8             m_nRightEndMarkers;    // expected left end markers
    
    StreamDef*     m_pStreamDef;
  
    bool           m_bFirstCall;      // first time being called?
    
    eOR (*m_fpCall) (void* aThisOp);  // function which calls the op loop
    eOR (*m_fpDrain)(void* aThisOp);  // function which calls the drain loop

  public:
    void  operator delete(void*);
  
    Op(eCN eClass, OpDefn* aOpDefn, Digest* pDigest, Stz* szOpLabel, bool bAppR, OpSpec* pOpSpec);
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
    Stz* szLabel(void);
    void vLabel(Stz* szLabel);
    Stz* szNextLabel(void);
    void vNextLabel(Stz* szNextLabel);
    i8   iOpId(void);
    void vOpOut(Op* pOpOut);
    bool bPause(void);
    void vPipeOut(Operand* pOperand);
    void vPipeOut_endMarker(void);
    void vPipeToJob(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
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
