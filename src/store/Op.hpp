/*============================================================================*/
/* Op.hpp -- Operator of Stream of Stage of Pipeline                          */
/*============================================================================*/
#pragma once

#include "ePX.h"
#include "eXK.h"
#include "eZK.h"

namespace STORE { class PipelineSpec; class StageSpec; class OpSpec; }

namespace STORE
{
  class Op : public Object
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name within pipeline
    s32  m_sStreamNm;   // stream name within stream within pipeline
    s32  m_sOpNm;       // name of this operator
    i8   m_i_Op;        // [op] in stream
  
    bool m_bTraceOp;    // tracing this operator

    OpSpec*   m_pOpSpec;   // specification of this operator
    Stream*   m_pStream;   // which stream of stage of pipleine
    Stage*    m_pStage;    // which stage of pipeline
    Pipeline* m_pPipeline; // which pipeline
  
    i8   m_tPtnNo; // this op processes m_tPtnNo in grid
    i8   m_i_Node; // this op processes m_i_Node in grid
    
    // -- -- -- -- -- --
  
    Job*           m_pPipelineJob;   // pipeline job this operator belongs to
    Job*           m_pOpJob;         // operator job of this operator
  
    TagSet         m_oOpTagSet;        // op tag set
    TagSet         m_oTopologyTagSet;  // topology tag set
    
    bool           m_bPulsedLimit;   // is this a pulsed operand?
    i8             m_iWorkQuanta;    // quanta of work between pulses
    i8             m_iWorkDone;      // work done

    bool           m_bAppendRight;   // append to right queue?
    OperandQueue*  m_pQueueLeft;     // the input queue being processed
    OperandQueue*  m_pQueueRight;    // the input queue being processed
    
    ePX            m_eCrossTopo;     // cross topology
    i8             m_iNextStage;     // next stage in the cross topology
    i8             m_iNextOp;        // next Op in the next stage
    i8             m_iOtherStage;    // other stage in the cross topology
    i8             m_iOtherOp;       // next Op in the other stage
    i8             m_iSendNode;      // send node (for send)
    eXK            m_eSendKind;      // send kind
    i8             m_iRobin;         // send Round Robin partition/node/thread
    i8             m_iLocalRobin;    // send Round Robin local ptn/node/thread
  
    Op*            m_pDownstreamOp;  // downstream op
    Operand*       m_pOutboundOperand;  // operand to be piped out

    Word* m_paOperandHandler[ZK_ENUMS];  // array of operand handlers (by kind)
    i8    m_iOperandCount[ZK_ENUMS];     // array of operand counts of operands recceived
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op(OpSpec* pOpSpec, Stream* pStream);
    Op(Stz* szOpNm);
   ~Op(void) noexcept(false);
  
    void vDump(i8 i_Op);

    // - - - - - - - - - - - - - - - -
    void vAckJob(void);
    void vAddWorkDone(i8 iWorkDone);
    void vAppend(Operand* pOperand, Task* pTriggeringTask);
    void vAppendLeft(Operand* pOperand, Task* pTriggeringTask);
    void vAppendRight(Operand* pOperand, Task* pTriggeringTask);
    bool bAppendRight(void);
    void vAppendToJob(Operand* pOperand);
    Stz* szArgs(void);
    ePX  eCrossTopo(void);
    void vCrossTopo(ePX eCrossTopo);
    Op*  pDownstreamOp(void);
    void vDownstreamOp(Op* pDownstreamOp);
    void vDropTaskLock(void);
    void vDump(void);
    void vOutline(s4K& sOutline);
    void vGrabTaskLock(void);
    void vIncWorkDone(void);
    void vInitOperandHandlers(void);
    bool bIsLocked(void);
    bool bIsOperandValid(i8 i_OperandKind);
    void vJobPipeline(Job* pJob);
    i8   iLocalRobin(void);
    void vLocalRobin(i8 iLocalRobin);
    i8   iNextOp(void);
    Op*  pNextOp(void);
    void vNextOp(i8 iNextOp);
    i8   iNextStage(void);
    i8   iOtherStage(void);
    void vNextStage(i8 iNextStage);
    i8   iNode(void);
    void vNode(i8 iNode);
    Word* pOperandHandler(i8 i_OperandKind);
    void vOperandHandler(i8 i_OperandKind, Word* pHandler);
    void vIncOperandCount(i8 i_OperandKind);
    Job* pOpJob(void);
    void vOpJob(Job* pOpJob);
    Stz* szOpNm(void);
    TagSet* pOpTagSet(void);
    void vOpTrace(Operand* pOperand, Stz* szFunActNm,
                  Stz* szSupplemental=""_Stz);
    i8   iOtherOp(void);
    Op*  pOtherOp(void);
    void vOtherOp(i8 iNextOp);
    void vOtherStage(i8 iNextStage);
    Operand* pOutboundOperand(void);
    void vOutboundOperand(Operand* pOutboundOperand);
    void vPtnNo(tPN tPtnNo);
    tPN  tPtnNo(void);
    bool bPause(void);
    bool bPeekQueue(void);
    bool bPeekQueueOoO(void);
    Pipeline* pPipeline(void);
    Job* pPipelineJob(void);
    void vPipeToNextOp(Operand* pOperand, Task* pTriggeringTask);
    void vPipeToOutput(Operand* pOperand);
    Operand* pPopQueue(void);
    Operand* pPopQueueOoO(void);
    Operand* pPopQueueLeft(void);
    Operand* pPopQueueRight(void);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    OperandQueue* pQueueLeft(void);
    i8   nQueueLength(void);
    OperandQueue* pQueueRight(void);
    i8   iRobin(void);
    void vRobin(i8 iRobin);
    void vSpawnOperatorJob(i8 iSessionId);
    Stage* pStage(void);
    Stz* szStageName(void);
    Stream* pStream(void);
    Task* pTask(void);
    void vToCmdStr(Stz* szCmdStr);
    TagSet* pTopologyTagSet(void);
    void vWorkQuanta(i8 iWorkQuanta);
    eXK  eSendKind(void);
    void vSendKind(eXK eSendKind);
    i8   iSendNode(void);
    void vSendNode(i8 iNde);
    
  private:
    Footer* aFooter(void);
    
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Op* _pOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
