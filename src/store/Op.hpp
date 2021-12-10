/*============================================================================*/
/* Op.hpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/
#pragma once

#include "eYX.h"
#include "eYS.h"
#include "eXK.h"
#include "eZK.h"


#include "LifeGuard_t.hpp"

namespace BASE { class OperandQueue; }

namespace STORE { class PipelineSpec; class StageSpec; class OpSpec; }

namespace STORE
{
  class Op : public Operand,
             public LifeGuard_t<Op>
  {
  private:
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // operator

    // -- id, name and kind
    Gid_t<CN_OP> m_xOpId;     // op id in grid
    s32        m_sOpNm;     // name of this operator
    s256       m_sPath;     // operator path

    s4K        m_sExpr;     // expression for this filter
  
    TagSet     m_oOpTags;   // tags for the op
    bool       m_bTraceOp;  // tracing this operator
    bool       m_bTraceAck; // tracing ack of this operator

    // -- belongs to
    SMA<Stream,CN_STREAM> m_pStream;  // which stream (of stage of pipeline of job)
    i8          m_i_Op;     // is [m_i_Op] in m_pStream

    // -- refers to
    SMA<OpSpec,CN_OPSPEC>    m_pOpSpec;   // specification of this operator
    SMA<Pipeline,CN_PIPELINE>  m_p_Pipeline_op; // which pipeline

    // -- owns
    SMA<LogicUnit,CN_LOGICUNIT> m_pOpLogicUnit;  // logic unit of op

    // -- executed by
    SMA<Task,CN_TASK>      m_pTask;    // which task is currenty executing this op

    // can these go???
    bool m_bPulsedLimit;  // is this a pulsed operand?
    i8   m_iWorkQuanta;   // quanta of work between pulses
    i8   m_iWorkDone;     // work done
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // event handling

    eZK   m_eCurHandler;
    Word* m_paEventHandler[ZK_ENUMS];  // event handlers
    i8    m_naEventCount[ZK_ENUMS];    // counts of event handlers
  
    i8    m_eCurOperand;
    i8    m_naOperandCount[ZK_ENUMS];  // counts of operands piped-in

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // in-bound queues and outbound operand
  
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueLeft;   // left input queue
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueRight;  // right input queue
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueOoO;    // out-of-order queue

    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // move these in logic unit???
  
    i8   m_iSendNode;   // send node (for send)
    eXK  m_eSendCast;   // send kind
    i8   m_iRobin;      // send Round Robin partition/node/thread
    i8   m_iLocalRobin; // send Round Robin local ptn/node/thread
  
    // -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - -- - --
    // delete these???

    SMA<Operand,CN_NONE> m_pOutboundOperand;  // operand to be piped out

    SMA<Op,CN_OP>      m_pDownstreamOp;     // downstream op  DELETE
    
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
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    eYX  eCrossTopology(void);
    eZK  eCurHandler(void);
    Word* pCurWord(void);
    void vDotPrint(Stz* szText);
    Op*  pDownstreamOp(void);
    void vDownstreamOp(Op* pDownstreamOp);
    void vDropTaskLock(void);
    void vDump(void);
    Word* pEntryWord(void);
    Word* pEventHandler(void);
    void vGrabTaskLock(void);
    i8   iHandlerCount(i8 i_Handler);
    void vIncHandlerCount(eZK eEvent);
    void vIncOperandCount(eZK eEvent);
    void vIncWorkDone(void);
    void vInitOperandHandlers(void);
    bool bIsLocked(void);
    bool bIsOperandValid(i8 i_OperandKind);
    i8   iLocalRobinNext(void);
    Job* pJob(void);
    i8   iNextOp(void);
    Op*  pNextOp(void);
    i8   iNextStage(void);
    Stage* pNextStage(void);
    i8   iNode(void);
    i8   iOperandCount(i8 i_OperandKind);
    Word* pEventHandler(i8 i_OperandKind);
    void vEventHandler(i8 i_OperandKind, Word* pHandler);
    void vExpr(Stz* sExpr);
    Stz* szExpr(void);
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

    Stz* szPath(void);
    bool bPause(void);
    bool bPeekLeft(void);
    bool bPeekOoO(void);
    Pipeline* pPipeline_op(void);
    Job* pPipelineJob(void);
    void vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToThis(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToOutput(Operand* pOperand);
    Operand* pPopOoO(void);
    Operand* pPopOoOLeft(void);
    Operand* pPopRight(void);

    PtnId  tPtnId(void);

    Operand* pPopLeft(void);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    i8   nQueuedOperands(void);
    i8   iRobinNext(void);
    eXK  eSendCast(void);
    void vSendCast(eXK eSendCast);
    i8   iSendNode(void);
    void vSendNode(i8 iNde);
    void vSpawnOpTask(i8 iSessionId);
    void vSetTags(void);
    Stage* pStage(void);
    Stz* szStageName(void);
    Stream* pStream(void);
    eYS  eStreamTopology_op(void);
    Task* pTask(void);
    void vTask(Task* pTask);
    void vToCmdStr(Stz* szCmdStr);
    void vVerifyOpComposition(void);
    void vWalk(void* aArg);
    void vWalkLambda(
           i8 iDepth,
           std::function<void(Object* pObject, i8 iDepth)> f_vClosure);
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
