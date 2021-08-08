/*============================================================================*/
/* Op.hpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/
#pragma once

#include "ePX.h"
#include "eXK.h"
#include "eZK.h"
#include "eSS.h"

namespace STORE { class PipelineSpec; class StageSpec; class OpSpec; }

namespace STORE
{
  class Op : public Operand
  {
  private:
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
  
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name within pipeline
    s32  m_sStreamNm;   // stream name within stream within pipeline
    s32  m_sOpNm;       // name of this operator
    i8   m_i_Op;        // [op] in stream
  
    OpSpec*   m_pOpSpec;   // specification of this operator
    Stream*   m_pStream;   // which stream of stage of pipleine
    Stage*    m_pStage;    // which stage of pipeline
    Pipeline* m_p_Pipeline_op; // which pipeline
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
    TagSet    m_oOpTags; // tags for the op
    Task*     m_pTask;   // which task
    eSS       m_eSeed;   // seed to spawn

    LogicUnit* m_pOpLogicUnit;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // working variables. these are not copies of variables from opSpec
  
    bool m_bTraceOp;    // tracing this operator

    bool           m_bPulsedLimit;   // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    OperandQueue*  m_pQueueLeft;     // left input queue
    OperandQueue*  m_pQueueRight;    // right input queue
    OperandQueue*  m_pQueueOoO;      // out-of-order queue

    i8             m_iSendNode;      // send node (for send)
    eXK            m_eSendKind;      // send kind
    i8             m_iRobin;         // send Round Robin partition/node/thread
    i8             m_iLocalRobin;    // send Round Robin local ptn/node/thread
  
    Op*            m_pDownstreamOp;  // downstream op
    Operand*       m_pOutboundOperand;  // operand to be piped out

    i8    m_i_CurHandler;
    i8    m_i_CurOperand;
    Word* m_paOperandHandler[ZK_ENUMS];  // array of operand handlers (by kind)
    i8    m_iHandlerCount[ZK_ENUMS];     // array of operand counts of handlers
    i8    m_iOperandCount[ZK_ENUMS];     // array of operand counts of operands recceived
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(OpSpec* pOpSpec, Stream* pStream);
    Op(Stz* szOpNm);
   ~Op(void) noexcept(false);
  
    // - - - - - - - - - - - - - - - -
    void vAckPipelineJob(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppendLeft(Operand* pOperand, Task* pTriggeringTask);
    void vAppendOoO(Operand* pOperand, Task* pTriggeringTask);
    void vAppendRight(Operand* pOperand, Task* pTriggeringTask);
    bool bAppendRight(void);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    ePX  eCrossTopo(void);
    i8   iCurHandler(void);
    Word* pCurWord(void);
    void vDotPrint(Stz* szText);
    Op*  pDownstreamOp(void);
    void vDownstreamOp(Op* pDownstreamOp);
    void vDropTaskLock(void);
    void vDump(void);
    Word* pEventHandler(void);
    void vGrabTaskLock(void);
    i8   iHandlerCount(i8 i_Handler);
    void vIncHandlerCount(i8 i_Handler);
    void vIncOperandCount(i8 i_OperandKind);
    void vIncWorkDone(void);
    void vInitOperandHandlers(void);
    bool bIsLocked(void);
    bool bIsOperandValid(i8 i_OperandKind);
    i8   iLocalRobinNext(void);
    i8   iNextOp(void);
    Op*  pNextOp(void);
    i8   iNextStage(void);
    Stage* pNextStage(void);
    i8   iNode(void);
    i8   iOperandCount(i8 i_OperandKind);
    Word* pOperandHandler(i8 i_OperandKind);
    void vOperandHandler(i8 i_OperandKind, Word* pHandler);
    i8   iOp(void);
    LogicUnit* pOpLogicUnit(void);
    Stz* szOpNm(void);
    OpSpec* pOpSpec(void);
    TagSet* pOpTags(void);
    void vOpTrace(Operand* pOperand
                  ,Stz* szFunNm
                  ,bool bHandler=true
                  ,Stz* szSupplemental=""_Stz
                  );
    i8   iOtherOp(void);
    i8   iOtherStage(void);
    Operand* pOutboundOperand(void);
    void vOutboundOperand(Operand* pOutboundOperand);

    bool bPause(void);
    bool bPeekLeft(void);
    bool bPeekOoO(void);
    Pipeline* pPipeline_op(void);
    Job* pPipelineJob(void);
    void vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToSelf(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToOutput(Operand* pOperand);
    Operand* pPopOoO(void);
    Operand* pPopOoOLeft(void);
    Operand* pPopRight(void);
    tPN  tPtnNo(void);
    Operand* pPopLeft(void);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    i8   nQueuedOperands(void);
    i8   iRobinNext(void);
    Stz* szSeed(void);
    eXK  eSendKind(void);
    void vSendKind(eXK eSendKind);
    i8   iSendNode(void);
    void vSendNode(i8 iNde);
    void vSpawnTaskForOp(i8 iSessionId);
    Stage* pStage(void);
    Stz* szStageName(void);
    Stream* pStream(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vToCmdStr(Stz* szCmdStr);
    void vVerifyOpComposition(void);
    void vWorkQuanta(i8 iWorkQuanta);
    
  private:
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
