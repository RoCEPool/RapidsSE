/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "Pipelines.h"


Atomic gixOp;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op* Pipelines::_pOp(void* aAddr)
{
  Op* pOp = reinterpret_cast<Op*>(aAddr);
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op::operator new(size_t)
{
  Op* pOp =
  static_cast<Op*>(HeapMem::pAllocate(CN_OP));
  
  return pOp;
}

/*----------------------------------------------------------------------------*/
/* free back to the heap                                                      */
/*----------------------------------------------------------------------------*/
void Op::operator delete(void* aAddress)
{
  Object* pObject = static_cast<Object*>(aAddress);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op::Op(OpSpec* pOpSpec, Stage* pStage, Pipeline* pPipeline)
: Focal(CN_OP,AM::iNextOpId(),AM::iJobUniqueId(pPipeline->pJob()))

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(pOpSpec);
  
  m_sOpNm     = pOpSpec->szOpNm();
  m_pOpSpec   = pOpSpec;
  m_pStage    = pStage;
  m_pPipeline = pPipeline;

  m_oEnameSet.vCopy(pOpSpec->pEnameSet());
  m_oTopologyEnameSet.vCopy(pOpSpec->pTopologyEnameSet());
  
  m_pPipelineJob  = pPipeline->pJob();
  m_pOpJob        = nullptr;
  
  m_bPulsedLimit  = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_bAppendRight  = pOpSpec->bAppendRight();
  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;

  m_nNode            = C_UNKNOWN;
  m_nPartition       = C_UNKNOWN;
  m_nLeftEosMarkers  = 1;
  m_nRightEosMarkers = 0;
  
  m_eTopologyX = PX_NONE;
  m_iNextStage = C_UNKNOWN;
  m_iNextOp    = C_UNKNOWN;
  m_iXmitNode  = C_UNKNOWN;
  m_iRobin     = 0;
  
  vInitOperandHandlers();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask = AM::pThisTLSTask();
  AM::vTaskBeginOp(pTask);
  
  ++ gixOp;
  AM::vLogOpr(SL_INF,this,OA_OP_CONSTRUCT,
              "Op construct : %p (%lld ops exist)"_Stz, //!format-LogOpr
              this,* gixOp);
  
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  delete m_pQueueLeft;
  delete m_pQueueRight;
  
  -- gixOp;
  AM::vLogOpr(SL_INF,this,OA_OP_DESTRUCT,
              "Op destruct : %p (%lld ops exist)"_Stz, //!format-LogOpr
              this,* gixOp);
}

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump contents                                                              */
/*----------------------------------------------------------------------------*/
void Op::vDump(i8 i_Op)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  s256 sArgs;
  
  EnameSet* pEnameSet = this->pTopologyEnameSet();
  
  pEnameSet->vSprint(sArgs);
  
  // get the next stage
  i8 iNextStage = pEnameSet->iValue(C_TAG_NEXTSTAGE);
  Stage* pNextStage = m_pPipeline->pGet(iNextStage);
  
  // get the next op
  i8 iNextOp = pEnameSet->iValue(C_TAG_NEXTOP);
  Strand* pNextStrand = pNextStage->pGet(0);
  Op* pNextOp = pNextStrand->pGet(iNextOp);

  // get the cross topology
  ePX eTopology = (ePX)(pEnameSet->iValue(C_TAG_TOPOLOGY));
  
  // get the left EOS markers for consume op
  //! ALKA: right EOS markers ???
  i8 iConsumeXmits =  pEnameSet->iValue("<consumeXmits>"_Stz);
  i8 iLeftEosMarkers = iConsumeXmits ? iConsumeXmits : nLeftEosMarkers();
  
  vTrace("    [%03lld] (N%lld,P%02lld) Op %12s%-20s { EosMarkers(L%lld R%lld) :%-10s %s %s }\n"_Stz,
         i_Op,
         this->nNode(),
         this->nPartition(),
         this->szOpNm(),
         sArgs.szStz(),
         iLeftEosMarkers,
         this->nRightEosMarkers(),
         pNextOp->szOpNm(),
         this->bAppendRight() ? "appendRight"_Stz : "appendLeft "_Stz,
         aPX[eTopology]
        );
}

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Task* pTask = AM::pExecutingTask(m_pPipeline->pJob());
  
  AM::vLogOpr(SL_INF,this,OA_OP_ACK,
              "Acking job task"_Stz); //!format-LogOpr
  
  if (AM::bVerbose())
    vTrace("Op %s: acking pipeline job\n"_Stz, AM::szJobName(m_pOpJob));
  
  AM::vAckWaitingTask(pTask,AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* get ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAddWorkDone(i8 iWorkDone)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkDone += iWorkDone;
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendLeft(Operand* pOperand, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
//  vTrace("%s::vAppendLeft : %p %s\n"_Stz,
//         m_sOpNm.szStz(),
//         pOperand,
//         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  if (bSet) {
    while (pOperand) {
      pQueueLeft()->vAppendOperand(pOperand);
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      pOperand = pDataBuffer->pNextBuffer();
    }
  }
  else
    pQueueLeft()->vAppendOperand(pOperand);

  Task* pTask = AM::pExecutingTask(m_pOpJob);
  
  if (TS_BIDING != (eTS)AM::iTaskState(pTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendRight(Operand* pOperand, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  //  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
  //         m_sOpNm(),
  //         pOperand,
  //         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  if (bSet) {
    while (pOperand) {
      pQueueRight()->vAppendOperand(pOperand);
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      pOperand = pDataBuffer->pNextBuffer();
    }
  }
  pQueueRight()->vAppendOperand(pOperand);
  
  Task* pTask = AM::pExecutingTask(m_pOpJob);
  
  if (TS_BIDING != (eTS)AM::iTaskState(pTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(pTask);
}

/*----------------------------------------------------------------------------*/
/* append right ?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bAppendRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_bAppendRight;
}

/*----------------------------------------------------------------------------*/
/* dec the leftEosMarkers and return it                                       */
/*----------------------------------------------------------------------------*/
i8 Op::nDecLeftEosMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return (--m_nLeftEosMarkers);
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vDropTaskLock(AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* get the EnameSet                                                           */
/*----------------------------------------------------------------------------*/
EnameSet* Op::pEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return &m_oEnameSet;
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Op::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  AM::vGrabTaskLock(AM::pExecutingTask(m_pOpJob));
}

/*----------------------------------------------------------------------------*/
/* inc work done                                                              */
/*----------------------------------------------------------------------------*/
void Op::vIncWorkDone(void)
{
 _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkDone++;
}

/*----------------------------------------------------------------------------*/
/* init operand handlers                                                      */
/*----------------------------------------------------------------------------*/
void Op::vInitOperandHandlers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  Dic* pRootDic = AM::pRootDic();
  
  s32 sHandlerNm;
  
  for (i8 i_Kind = 1; i_Kind < ZK_ENUMS; i_Kind++) {
    
    sHandlerNm = aZK[i_Kind];
    sHandlerNm.vConcat("_handler"_Stz);
    
    //vTrace("Handler: %s\n"_Stz,sHandlerNm.szStz());
    
    Word* pHandler = pRootDic->pFindWord(sHandlerNm);
    if (! pHandler)
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "Unable to find operand handler for: %s"_Stz,
             sHandlerNm.szStz());
    
    m_paOperandHandler[i_Kind] = pHandler;
  }
  
  Correctness::vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* is cancelling?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bIsCancelling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pPipeline->bCancelling();
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bLocked = AM::bTaskIsLocked(AM::pExecutingTask(m_pOpJob));

  return bLocked;
}

/*----------------------------------------------------------------------------*/
/* set the job                                                                */
/*----------------------------------------------------------------------------*/
void Op::vJobPipeline(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pPipelineJob = pJob;
}

/*----------------------------------------------------------------------------*/
/* get the leftEosMarkers                                                     */
/*----------------------------------------------------------------------------*/
i8 Op::nLeftEosMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_nLeftEosMarkers;
}

/*----------------------------------------------------------------------------*/
/* set the leftEosMarkers                                                     */
/*----------------------------------------------------------------------------*/
void Op::vLeftEosMarkers(i8 nEosMarkers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_nLeftEosMarkers = nEosMarkers;
}

/*----------------------------------------------------------------------------*/
/* get next op                                                                */
/*----------------------------------------------------------------------------*/
i8 Op::iNextOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iNextOp;
}

/*----------------------------------------------------------------------------*/
/* set next stage                                                             */
/*----------------------------------------------------------------------------*/
void Op::vNextOp(i8 iNextOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iNextOp = iNextOp;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iNextStage;
}

/*----------------------------------------------------------------------------*/
/* set next stage                                                             */
/*----------------------------------------------------------------------------*/
void Op::vNextStage(i8 iNextStage)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iNextStage = iNextStage;
}

/*----------------------------------------------------------------------------*/
/* get node                                                                   */
/*----------------------------------------------------------------------------*/
i8 Op::nNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_nNode;
}

/*----------------------------------------------------------------------------*/
/* set node                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vNode(i8 nNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_nNode = nNode;
}

/*----------------------------------------------------------------------------*/
/* get operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
Word* Op::pOperandHandler(i8 i_OperandKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Word* pHandler = m_paOperandHandler[i_OperandKind];
  
  return pHandler;
}

/*----------------------------------------------------------------------------*/
/* set operand handler for given operand kind                                 */
/*----------------------------------------------------------------------------*/
void Op::vOperandHandler(i8 i_OperandKind, Word* pHandler)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_paOperandHandler[i_OperandKind] = pHandler;
}

/*----------------------------------------------------------------------------*/
/* get out operator                                                           */
/*----------------------------------------------------------------------------*/
Job* Op::pOpJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpJob;
}

/*----------------------------------------------------------------------------*/
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szOpNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_sOpNm;
}

/*----------------------------------------------------------------------------*/
/* set partition in op                                                        */
/*----------------------------------------------------------------------------*/
void Op::vPartition(i8 nPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_nPartition = nPartition;
  
  // set the node tis partition belongs to 
  m_nNode = TablePtn::iPartitionToNode(m_nPartition);
}

/*----------------------------------------------------------------------------*/
/* get partition from op                                                      */
/*----------------------------------------------------------------------------*/
i8 Op::nPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_nPartition;
}

/*----------------------------------------------------------------------------*/
/* operator is to pause?                                                      */
/*----------------------------------------------------------------------------*/
bool Op::bPause(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (! m_bPulsedLimit)
    return false;
  
  if (0 == m_iWorkDone % m_iWorkQuanta)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* peek for OoO operand from operand queue                                    */
/*----------------------------------------------------------------------------*/
bool Op::bPeekQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  bool bOperandOoO;
  
  vGrabTaskLock();
  
  bOperandOoO = pQueueLeft()->bPeekQueueOoO();
  
  vDropTaskLock();
  
  return bOperandOoO;
}

/*----------------------------------------------------------------------------*/
/* get pipeline from op                                                       */
/*----------------------------------------------------------------------------*/
Pipeline* Op::pPipeline(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pPipeline;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut_nextOp(Operand* pOperand, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_nNode,
           m_nPartition,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           pOperand->eClass() == CN_MARKER ?
             aOM[(_pMarker(pOperand))->eOpMarker()] :
             aCN[pOperand->eClass()],
           pOperand);
  
  if (AM::szJobName(m_pOpJob)->bSame("xmitop"_Stz)) {
    vSendJobOperand(pOperand, bSet);
  }
  else {
    
      // get the next stage the xmit is supposed to xmit to
      Stage* pStage = m_pPipeline->pGet(m_iNextStage);
    
      // get the op to forward to depending on the cross topology
      Op* pOp = pStage->pPipeOutOp(m_eTopologyX,
                                   m_nNode,
                                   m_nPartition,
                                   m_iNextOp);
    if (m_bAppendRight)
      pOp->vAppendRight(pOperand, bSet);
    else
      pOp->vAppendLeft(pOperand, bSet);
    
    AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
                pOperand->eClass() == CN_MARKER ?
                aOM[(_pMarker(pOperand))->eOpMarker()] :
                aCN[pOperand->eClass()],
                m_bAppendRight ? "right"_Stz : "left"_Stz,
                aCN[pOp->eClass()]);
  }
  
}

/*----------------------------------------------------------------------------*/
/* append operand to specific operand                                         */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut_thisOp(Operand* pOperand, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_nNode,
           m_nPartition,
           m_pPipelineJob,
           "vPipeOut :"_Stz,
           pOperand->eClass() == CN_MARKER ?
             aOM[(_pMarker(pOperand))->eOpMarker()] :
             aCN[pOperand->eClass()],
           pOperand);
  
  vAppendLeft(pOperand, bSet);
    
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
              "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
              pOperand->eClass() == CN_MARKER ?
              aOM[(_pMarker(pOperand))->eOpMarker()] :
              aCN[pOperand->eClass()],
              m_bAppendRight ? "right"_Stz : "left"_Stz,
              aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToJob(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Queue* pBufferQueue = AM::pBufferQueue(m_pPipelineJob);
  
  RASSERT(pBufferQueue);
  
  if (AM::bVerbose())
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_nNode,
           m_nPartition,
           m_pPipelineJob,
           "vPipeToJob :"_Stz,
           pOperand->eClass() == CN_MARKER ?
             aOM[(_pMarker(pOperand))->eOpMarker()] :
             aCN[pOperand->eClass()],
           pOperand);
  
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
              "vPipeToJob operand %s to queue of pipeline job"_Stz, //!format-LogOpr
              aCN[pOperand->eClass()]);
  
  pBufferQueue->vAppend(pOperand);
  
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
                pOperand->eClass() == CN_MARKER ?
                  aOM[(_pMarker(pOperand))->eOpMarker()] :
                  aCN[pOperand->eClass()]);
  else
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue no operand on left queue"_Stz); //!format-LogOpr
  
  if (pOperand)
    vDropTaskLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerbose() && pOperand) {
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_nNode,
           m_nPartition,
           m_pPipelineJob,
           "pPopQueue :"_Stz,
           pOperand->eClass() == CN_MARKER ?
             aOM[(_pMarker(pOperand))->eOpMarker()] :
             aCN[pOperand->eClass()],
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand OoO off the queue                                              */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueOoO(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueueOoO();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
                pOperand->eClass() == CN_MARKER ?
                aOM[(_pMarker(pOperand))->eOpMarker()] :
                aCN[pOperand->eClass()]);
  else
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue no operand on left queue"_Stz); //!format-LogOpr
  
  vDropTaskLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerbose() && pOperand) {
    vTrace("%12s (%10s) (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sOpNm.szStz(),
           m_pStage->szStageNm(),
           m_nNode,
           m_nPartition,
           m_pPipelineJob,
           "pPopQueue :"_Stz,
           pOperand->eClass() == CN_MARKER ?
           aOM[(_pMarker(pOperand))->eOpMarker()] :
           aCN[pOperand->eClass()],
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue A                                                        */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand)
    vDropTaskLock();
  
  if (pOperand) {
    if (AM::bVerbose())
      AM::vLogOpr(SL_INF,this,OA_OP_POP,
                  "PopQueueLeft operand %s from left queue"_Stz, //!format-LogOpr
                  pOperand->eClass() == CN_MARKER ?
                    aOM[(_pMarker(pOperand))->eOpMarker()] :
                    aCN[pOperand->eClass()]);
  }
  else {
    if (AM::bVerbose())
      AM::vLogOpr(SL_INF,this,OA_OP_POP,
                  "PopQueueLeft no operand on left queue"_Stz); //!format-LogOpr
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (AM::bVerbose())
      vTrace("%s::pPopQueueLeft : %s %p\n"_Stz,
             m_sOpNm.szStz(),
             pOperand->eClass() == CN_MARKER ?
               aOM[(_pMarker(pOperand))->eOpMarker()] :
               aCN[pOperand->eClass()],
             pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue B                                                        */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueRight()->pPopQueue();
  
  if (pOperand)
    vDropTaskLock();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueueRight operand %s from right queue"_Stz, //!format-LogOpr
                pOperand->eClass() == CN_MARKER ?
                  aOM[(_pMarker(pOperand))->eOpMarker()] :
                  aCN[pOperand->eClass()]);
  else
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueueRight no operand on right queue"_Stz); //!format-LogOpr
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (AM::bVerbose())
      vTrace("%s::pPopQueueRight : %s %p\n"_Stz,
             m_sOpNm.szStz(),
             pOperand->eClass() == CN_MARKER ?
               aOM[(_pMarker(pOperand))->eOpMarker()] :
               aCN[pOperand->eClass()],
             pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* is pulsed operator?                                                        */
/*----------------------------------------------------------------------------*/
bool Op::bPulsedLimit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* set pulsed operator                                                        */
/*----------------------------------------------------------------------------*/
void Op::vPulsedLimit(bool bPulsedLimit)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_bPulsedLimit = bPulsedLimit;
}

/*----------------------------------------------------------------------------*/
/* get left operand queue                                                     */
/*----------------------------------------------------------------------------*/
OperandQueue* Op::pQueueLeft(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pQueueLeft;
}

/*----------------------------------------------------------------------------*/
/* get right operand queue                                                    */
/*----------------------------------------------------------------------------*/
OperandQueue* Op::pQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pQueueRight;
}

/*----------------------------------------------------------------------------*/
/* get the rightEosMarkers                                                    */
/*----------------------------------------------------------------------------*/
i8 Op::nRightEosMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_nRightEosMarkers;
}

/*----------------------------------------------------------------------------*/
/* set the rightEosMarkers                                                    */
/*----------------------------------------------------------------------------*/
void Op::vRightEosMarkers(i8 nEosMarkers)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_nRightEosMarkers = nEosMarkers;
}

/*----------------------------------------------------------------------------*/
/* set job operand                                                            */
/*----------------------------------------------------------------------------*/
void Op::vSendJobOperand(Operand* pOperand, bool bSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  // get next stage to xmit to
  ASSERT (m_iNextStage != C_UNKNOWN);
  ASSERT(m_iNextOp != C_UNKNOWN);
  Stage* pNextStage = m_pPipeline->pGet(m_iNextStage);
  RASSERT(pNextStage);
  bool bIsMarker;
  eXK eXmitKind = XK_NORMAL;
  i8 i_Ptn;
  i8 i_Nde;
  i8 i_Slt;
  i8 i_Strand;
  Strand* pStrand;
  
  Job_Operand* pJob_Operand = nullptr;

  switch(m_eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN:
    case PX_SPECIFIC_PARTITION_TO_ORIGIN: {
      
      //xmitNde is the originating node
      m_iXmitNode = m_pPipeline->iOriginatingNode();
      
      // new a operand job
      pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand, this, m_iXmitNode, C_UNKNOWN);
      
      // get the send node in the task
      Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
      
      if (bSet) {
        while (pOperand) {
          
          // set the operand in the job
          AM::vSetOperand(pJob_Operand, pOperand);
          
          // send the operand job across the sendNode
          AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
          DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
          pOperand = pDataBuffer->pNextBuffer();
        }
      }
      else {
        // send the operand job across the sendNode
        AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
      }
      
      // delete the operand job
      AM::vDeleteJobOperand(pJob_Operand);
      
      break;
    }
      
    case PX_ORIGIN_TO_ALL_PARTITIONS: {
      
      bIsMarker = pOperand->bIsMarker();
      if ( ! bIsMarker && pOperand->eClass() != CN_STREAMDEF) {
        eXmitKind = m_eXmitKind;
      }
      
      // check the XmitKind
      switch (eXmitKind) {
          
        case XK_HASHED : {
          DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
          i_Ptn = pDataBuffer->iPartitionNo();
          
          // xmitNode is the partition node
          m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
          
          //vTrace("Nde %lld; Ptn %lld;"_Stz, m_iXmitNode, i_Ptn);
          
          // new a operand job
          pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand, this, m_iXmitNode, i_Ptn);
          
          // get the send node in the task
          Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
          
          // send the operand job across the sendNode
          AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
          
          // delete the operand job
          AM::vDeleteJobOperand(pJob_Operand);
          
          break;
        }
          
        case XK_NORMAL: {
      
          for (i_Ptn = 0; i_Ptn < pNextStage->nItems(); i_Ptn++) {
            
            // xmitNode is the partition node
            m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
            
            // new a operand job
            pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
            
            // get the send node in the task
            Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
            
            // send the operand job across the sendNode
            AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
            
            // delete the operand job
            AM::vDeleteJobOperand(pJob_Operand);
          }
          break;
        }
        default:
          ASSERT(0);
      }
      break;
    }
    
    case PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN: {
      
      bIsMarker = pOperand->bIsMarker();
      if ( ! bIsMarker && pOperand->eClass() != CN_STREAMDEF) {
        eXmitKind = m_eXmitKind;
      }
      
      // check the XmitKind
      switch (eXmitKind) {
          
        case XK_ROBIN : {
          m_iRobin++;
          i_Slt = m_iRobin % AM::pGrid()->nSlotsPerNode();
          i_Ptn = TablePtn::iSlotToPartition(i_Slt);
          
          // xmitNode is the partition node
          m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
          ASSERT(m_iXmitNode == m_pPipeline->iOriginatingNode());
          
          // new a operand job
          pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
          
          // get the send node in the task
          Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
          
          // send the operand job across the sendNode
          AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
          
          // delete the operand job
          AM::vDeleteJobOperand(pJob_Operand);
          
          break;
        }
          
          
        case XK_NORMAL: {
          
          for (i_Strand = 0; i_Strand < pNextStage->nItems(); i_Strand++) {
            
            pStrand = pNextStage->pGet(i_Strand);
            i_Ptn = pStrand->i_Ptn();
            
            // xmitNode is the partition node
            m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
            
            // transmit only on partitions of originating node
            if ( m_iXmitNode == m_pPipeline->iOriginatingNode()) {
              // new a operand job
              pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
              
              // get the send node in the task
              Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
              
              // send the operand job across the sendNode
              AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
              
              // delete the operand job
              AM::vDeleteJobOperand(pJob_Operand);
            }
          }
        }
        break;
          
        default:
          ASSERT(0);
      }
      break;
    }
      
    case PX_ALL_PARTITIONS_ON_ORIGIN_TO_ALL_PARTITIONS: {
      
      bIsMarker = pOperand->bIsMarker();
      if ( ! bIsMarker && pOperand->eClass() != CN_STREAMDEF) {
        eXmitKind = m_eXmitKind;
      }
      
      // check the XmitKind
      switch (eXmitKind) {
          
        case XK_ROBIN : {
          m_iRobin++;
          if (m_eTopologyX == PX_ORIGIN_TO_ALL_PARTITIONS_ON_ORIGIN)
            i_Ptn = m_iRobin % AM::pGrid()->nSlotsPerNode();
          else
            i_Ptn = m_iRobin % AM::pGrid()->nPartitions();
          // xmitNode is the partition node
          m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
          
          // new a operand job
          pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
          
          // get the send node in the task
          Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
          
          // send the operand job across the sendNode
          AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
          
          // delete the operand job
          AM::vDeleteJobOperand(pJob_Operand);
          
          break;
        }
          
        case XK_HASHED : {
          DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
          i_Ptn = pDataBuffer->iPartitionNo();
          
          // xmitNode is the partition node
          m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
          
          //vTrace("Nde %lld; Ptn %lld;"_Stz, m_iXmitNode, i_Ptn);
          
          // new a operand job
          pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand, this, m_iXmitNode, i_Ptn);
          
          // get the send node in the task
          Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
          
          // send the operand job across the sendNode
          AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
          
          // delete the operand job
          AM::vDeleteJobOperand(pJob_Operand);
          
          break;
        }
          
        case XK_NORMAL: {
          
          //if ( m_nPartition == AM::pGrid()->nSlotsPerNode() - 1 ) {
            for (i_Ptn = 0; i_Ptn < pNextStage->nItems(); i_Ptn++) {
              
              // xmitNode is the partition node
              m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
              
              // new a operand job
              pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
              
              // get the send node in the task
              Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
              
              // send the operand job across the sendNode
              AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
              
              // delete the operand job
              AM::vDeleteJobOperand(pJob_Operand);
            }
          //}
          break;
        }
        default:
          ASSERT(0);
      }
      break;
    }
      
    case PX_ORIGIN_TO_SPECIFIC_PARTITION: {
      pStrand = pNextStage->pGetPartitionStrand(m_nPartition);
      i_Ptn = pStrand->i_Ptn();
      
      // xmitNode is the partition node
      m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
      
      // new a operand job
      pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, i_Ptn);
      
      // get the send node in the task
      Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
      
      // send the operand job across the sendNode
      AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
      
      // delete the operand job
      AM::vDeleteJobOperand(pJob_Operand);
      break;
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      RASSERT(0);
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      
      // xmitNode is the partitionNode
      m_iXmitNode = TablePtn::iPartitionToNode(m_nPartition);
      
      // new a operand job
      pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, m_nPartition);
      
      // get the send node in the task
      Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
      
      // send the operand job across the sendNode
      AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
      
      // delete the operand job
      AM::vDeleteJobOperand(pJob_Operand);
      
      break;
    }
      
    case PX_ORIGIN_TO_ALL_NODES: {
      
      for (i_Nde = 0; i_Nde < pNextStage->nItems(); i_Nde++) {
        
        // xmitNode is the partition node
        m_iXmitNode = i_Nde;
        
        // new a operand job
        pJob_Operand = AM::pNewJobOperand(m_pPipelineJob, pOperand,this, m_iXmitNode, C_UNKNOWN);
        
        // get the send node in the task
        Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
        
        // send the operand job across the sendNode
        AM::vSend(AM::pExecutingTask(m_pOpJob), pSendNode, pJob_Operand);
        
        // delete the operand job
        AM::vDeleteJobOperand(pJob_Operand);
      }
      break;
    }
      
    default:
      RASSERT(0);
  }
  
  // delete the operand
  AM::vDeleteOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* spawn a job for this operator                                              */
/*----------------------------------------------------------------------------*/
bool Op::bSpawnOperatorJob(i8 iSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  ReqTuple xReqTuple(m_sOpNm,iSessionId, false);

  m_pOpJob = AM::pNewJobOp(this,&xReqTuple);

  EnameSet* pOpJobEnameSet = AM::pEnameSet(m_pOpJob);
  EnameSet* pPipeLineJobEnameSet = AM::pEnameSet(m_pPipelineJob);

  pOpJobEnameSet->vInsert(C_TAG_PATH,
                          pPipeLineJobEnameSet->szValue(C_TAG_PATH));
  pOpJobEnameSet->vInsert(C_TAG_JOBNM,
                          m_sOpNm);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job* pJob_pipeline = AM::pThisTLSJob();
  EnameSet* pEnameSet_pipeline = AM::pEnameSet(pJob_pipeline);
 
  EnameSet* pEnameSet_op = AM::pEnameSet(m_pOpJob);

  pEnameSet_op->vCopy(pEnameSet_pipeline);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // form the command string using the parameter ename pairs
  s4K szCmdStr;
  vToCmdStr(szCmdStr);
  
  TokenVec* pTknVec = new TokenVec;
  Tokeniser::vTokenise(pTknVec, szCmdStr);
  
  //vTrace("Spawning job for %s\n"_Stz, szCmdStr.szStz());
  
  eEC eError = AM::ePopulateJob(m_pOpJob, pTknVec);
  
  // now append the hidden topology ename pairs to the job ename set
  AM::vAppendEnameSet(m_pOpJob, &m_oTopologyEnameSet);
  
  // delete the tokenVec
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  //AM::pEnameSet(m_pOpJob)->vAdd(C_TAG_API);
  //AM::pEnameSet(m_pOpJob)->vAdd(C_TAG_TAGS);

  bool bSpawned = AM::bSpawnJob(m_pOpJob);
  
  return bSpawned;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Op::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return AM::pExecutingTask(m_pOpJob);
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
//void Op::vTask(Task* pTask)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
//
//  m_pTask = pTask;
//}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  szCmdStr->vCopy(m_sOpNm);
  m_oEnameSet.vConcat(szCmdStr);
  //vTrace("%s\n"_Stz, szCmdStr);
}

/*----------------------------------------------------------------------------*/
/* get the topology EnameSet                                                  */
/*----------------------------------------------------------------------------*/
EnameSet* Op::pTopologyEnameSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return &m_oTopologyEnameSet;
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
ePX Op::eTopologyX(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_eTopologyX;
}

/*----------------------------------------------------------------------------*/
/* set cross topology                                                         */
/*----------------------------------------------------------------------------*/
void Op::vTopologyX(ePX eTopologyX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_eTopologyX = eTopologyX;
}

/*----------------------------------------------------------------------------*/
/* set work quanta                                                            */
/*----------------------------------------------------------------------------*/
void Op::vWorkQuanta(i8 iWorkQuanta)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iWorkQuanta = iWorkQuanta;
}

/*----------------------------------------------------------------------------*/
/* get xmit kind                                                              */
/*----------------------------------------------------------------------------*/
eXK Op::eXmitKind(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_eXmitKind;
}

/*----------------------------------------------------------------------------*/
/* set xmit kind                                                              */
/*----------------------------------------------------------------------------*/
void Op::vXmitKind(eXK eXmitKind)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_eXmitKind = eXmitKind;
}

/*----------------------------------------------------------------------------*/
/* get xmit node                                                              */
/*----------------------------------------------------------------------------*/
i8 Op::iXmitNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_iXmitNode;
}

/*----------------------------------------------------------------------------*/
/* set xmit node                                                              */
/*----------------------------------------------------------------------------*/
void Op::vXmitNode(i8 iXmitNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_iXmitNode = iXmitNode;
}

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
__aFooterFunction(Op);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
