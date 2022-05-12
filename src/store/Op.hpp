/*============================================================================*/
/* Op.hpp -- Operator (of Stream of Stage of Deck of Vertex of Pipeline)      */
/*============================================================================*/
#pragma once

#include "eYX.h"
#include "eYS.h"
#include "eXK.h"
#include "eEV.h"

#include "EvMgr.hpp"
#include "OperandWatch.hpp"
#include "PipeIn.hpp"

namespace BASE { class OperandQueue; }

namespace STORE { class Deck;
                  class OpSpec;
                  class VertexSpec; class Vertex;
                  class StageSpec; }

namespace STORE
{
  class Op : public Operand
  {
  private:
  
    SMA<Stream,CN_STREAM> m_p_Stream;  // which stream (of stage of pipeline of job)
    SMA<OpSpec,CN_OPSPEC> m_p_OpSpec;  // specification of this operator

    SMA<Task,CN_TASK>     m_pTask;     // task of op

    Gid_t<CN_OP> m_xOpId;     // op id in grid
    s256         m_sSpot;     // operator grid position/path

    TagSet*  m_pOpTags;   // tags for the op
    bool     m_bTraceOp;  // tracing this operator
    bool     m_bTraceAck; // tracing ack of this operator

    EvMgr*   m_pEvMgr;    // event handlers
         
    PipeIn*  m_pPipeIn;   // in-bound queues
 
    // -- - -- - -- - -- - -- - -- - --
    // move these in logic unit???
  
    i8   m_iSendNode;   // send node (for send)
    eXK  m_eSendCast;   // send kind
    i8   m_iRobin;      // send Round Robin partition/node/thread
    i8   m_iLocalRobin; // send Round Robin local ptn/node/thread
      

    // can these go???
    bool m_bPulsedLimit;  // is this a pulsed operand?
    i8   m_iWorkQuanta;   // quanta of work between pulses
    i8   m_iWorkDone;     // work done

    i8   m_iMetricsQueued;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(Stream* pStream,
       OpSpec* pOpSpec);
   ~Op(void) noexcept(false);
  
    // - - - - - - - - - - - - - - - -
    void vAckInvokingTask(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppendLeft(Operand* pOperand, Task* pTriggeringTask);
    void vAppendOoO(Operand* pOperand, Task* pTriggeringTask);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
  
    void vCreateTaskForOp(void);
    eYX  eCrossTopology(void);
    eEV  eCurEvent(void);
    Word* pCurWord(void);
  
    Deck* pDeck(void);
    void vDotPrint(Stz* szText);
    void vDropTaskLock(void);
    void vDump(void);
  
    Word* pEntryWord(void);
    i8    nEventCount(eEV eEvent);
    void vEventCountInc(eEV eEvent);
    Word* pEventHandler(void);
    Word* pEventHandler(i8 i_OperandKind);
    void vEventHandler(i8 i_OperandKind, Word* pHandler);
    EvMgr* pEvMgr(void);

    void vGrabTaskLock(void);
  
    void vIncWorkDone(void);
    bool bIsLocked(void);
    bool bIsEventValid(i8 i_OperandKind);
  
    i8   iLocalRobinNext(void);
  
    Job* pJobOfOp(void);
  
    i8   iMetricsQueued(void);
    void vMetricsQueued(i8 iMetricsQueued);
  
    i8   iNextOp(void);
    Op*  pNextOp(void);
    i8   iNextStage(void);
    Stage* pNextStage(void);
    i8   iNodeId(void);
    
    i8   i_Op(void);
    OperandWatch* pOperandWatch(void);
    LogicUnit* pOpLogicUnit(void);
    Stz* szOpNm(void);
    OpSpec* pOpSpec(void);
    TagSet* pOpTags(void);
    void vOpTags(TagSet* pOpTags);
    void vOpTrace(Operand* pOperand
                  ,Stz* szFunNm
                  ,bool bHandler=true
                  ,Stz* szSupplemental=""_Stz
                  );
    i8   iOtherOp(void);
    i8   iOtherStage(void);
    Stage* pOtherStage(void);

    Stz* szSpot(void);
    bool bPause(void);
    bool bPeekLeft(void);
    bool bPeekOoO(void);
    Op*  pPipeDstOp(void);
    Pipeline* pPipeline(void);
    void vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToThis(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToOutput(Operand* pOperand);
    Operand* pPopOoO(void);
    Operand* pPopOoOLeft(void);
    Operand* pPopRight(void);
    LglPtn  tLglPtn(void);
    Operand* pPopLeft(void);
    i8   nPotSize(void);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
  
    i8   nQueuedOperands(void);
  
    i8   iRobinNext(void);
  
    eXK  eSendCast(void);
    void vSendCast(eXK eSendCast);
    i8   iSendNode(void);
    void vSendNode(i8 iNde);
    void vSpawnTask(void);
    void vSetInvoker(Task* pInvoker);
    void vSetTags(void);
    Stage* pStage(void);
    Stz* szStageName(void);
    Stream* pStream(void);
    eYS  eStreamTopology_op(void);

    Task* pTask(void);
    void vTask(Task* pTask);
    i8   nTmpIndexSize(void);
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
