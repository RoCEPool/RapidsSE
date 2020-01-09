/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "Operators.h"

#include "AM.hpp"
#include "eSA.h"
#include "eTS.h"

Atomic gixOp;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op* Operators::_pOp(void* aAddr)
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
Op::Op(OpSpec* pOpSpec, Pipeline* pPipeline)
: Focal(CN_OP,AM::iNextOpId(),AM::iJobUniqueId(pPipeline->pJob()))

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  RASSERT(pOpSpec);
  
  m_sName = pOpSpec->szName();

  m_oEnameSet.vCopy(pOpSpec->pEnameSet());

  m_pPipeline     = pPipeline;
  m_pJobPipeline  = pPipeline->pJob();
  m_pOpJob        = nullptr;
  m_pTask         = nullptr;
  
  m_bPulsed       = false;
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
/* this executing / tls job                                                   */
/*----------------------------------------------------------------------------*/
Op* Op::pThisTLSOp(void)
{
  Op* pOp = AM::pThisTLSOp();
  
  return pOp;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
    vTrace("Op %s: acking pipeline job\n"_Stz, aJT[AM::eJobType(m_pOpJob)]);
  
  AM::vAckWaitingTask(pTask,m_pTask);
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
void Op::vAppendLeft(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
//  vTrace("%s::vAppendLeft : %p %s\n"_Stz,
//         m_sName.szStz(),
//         pOperand,
//         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  pQueueLeft()->vAppendOperand(pOperand);

  if (TS_BIDING != (eTS)AM::iTaskState(m_pTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendRight(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  //  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
  //         m_sName(),
  //         pOperand,
  //         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  pQueueRight()->vAppendOperand(pOperand);
  
  if (TS_BIDING != (eTS)AM::iTaskState(m_pTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(m_pTask);
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
  
  AM::vDropTaskLock(m_pTask);
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
  
  AM::vGrabTaskLock(m_pTask);
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
  
  bool bLocked = AM::bTaskIsLocked(m_pTask);

  return bLocked;
}

/*----------------------------------------------------------------------------*/
/* set the job                                                                */
/*----------------------------------------------------------------------------*/
void Op::vJobPipeline(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pJobPipeline = pJob;
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
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_sName;
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
/* get out operator                                                           */
/*----------------------------------------------------------------------------*/
Job* Op::pOpJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pOpJob;
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
  
  if (! m_bPulsed)
    return false;
  
  if (0 == m_iWorkDone % m_iWorkQuanta)
    return true;
  
  return false;
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
void Op::vPipeOut(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
           "vPipeOut :"_Stz,
           pOperand->eClass() == CN_MARKER ?
             aOM[(_pMarker(pOperand))->eOpMarker()] :
             aCN[pOperand->eClass()],
           pOperand);

  eJT eJobType = AM::eJobType(m_pOpJob);
  
  switch(eJobType) {
      
    case JT_XMITOP:
      vSendJobOperand(pOperand);
      break;
      
    default:
        
        // get the next stage the xmit is supposed to xmit to
        Stage* pStage = m_pPipeline->pStageVec()->pGet(m_iNextStage);
        
        // get the op to forward to depending on the cross topology
        Op* pOp = pStage->pPipeOutOp(m_eTopologyX,
                                     m_nNode,
                                     m_nPartition,
                                     m_iNextOp);                
      if (m_bAppendRight)
        pOp->vAppendRight(pOperand);
      else
        pOp->vAppendLeft(pOperand);
      
      AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                  "PipeOut operand %s to %s queue of %s"_Stz, //!format-LogOpr
                  pOperand->eClass() == CN_MARKER ?
                  aOM[(_pMarker(pOperand))->eOpMarker()] :
                  aCN[pOperand->eClass()],
                  m_bAppendRight ? "right"_Stz : "left"_Stz,
                  aCN[pOp->eClass()]);
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToJob(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  Queue* pBufferQueue = AM::pBufferQueue(m_pJobPipeline);
  
  if (AM::bVerbose())
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
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
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
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
             m_sName.szStz(),
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
             m_sName.szStz(),
             pOperand->eClass() == CN_MARKER ?
               aOM[(_pMarker(pOperand))->eOpMarker()] :
               aCN[pOperand->eClass()],
             pOperand);
  }
  
  return pOperand;
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
void Op::vSendJobOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);

  // get next stage to xmit to
  ASSERT (m_iNextStage != C_UNKNOWN);
  ASSERT(m_iNextOp != C_UNKNOWN);
  Stage* pNextStage = m_pPipeline->pStageVec()->pGet(m_iNextStage);
  RASSERT(pNextStage);
  
  Job_Operand* pJob_Operand = nullptr;
  
  switch(m_eTopologyX) {
      
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN: {
      
      //xmitNde is the originating node
      m_iXmitNode = m_pPipeline->iOriginatingNode();
      
      // new a operand job
      pJob_Operand = AM::pNewJobOperand(m_pJobPipeline, pOperand,this, m_iXmitNode, C_UNKNOWN);
      
      // get the send node in the task
      Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
      
      // send the operand job across the sendNode
      AM::vSend(m_pTask, pSendNode, pJob_Operand);
      
      // delete the operand job
      AM::vDeleteJobOperand(pJob_Operand);
      
      break;
    }
      
    case PX_ORIGIN_TO_ALL_PARTITIONS: {
      for (i8 i_Ptn = 0; i_Ptn < pNextStage->pStagePtnVec()->nItems(); i_Ptn++) {
        
        // xmitNode is the partition node
        m_iXmitNode = TablePtn::iPartitionToNode(i_Ptn);
        
        // new a operand job
        pJob_Operand = AM::pNewJobOperand(m_pJobPipeline, pOperand,this, m_iXmitNode, i_Ptn);
        
        // get the send node in the task
        Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
        
        // send the operand job across the sendNode
        AM::vSend(m_pTask, pSendNode, pJob_Operand);
        
        // delete the operand job
        AM::vDeleteJobOperand(pJob_Operand);
      }
      break;
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      RASSERT(0);
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      
      // xmitNode is the partitionNode
      m_iXmitNode = TablePtn::iPartitionToNode(m_nPartition);
      
      // new a operand job
      pJob_Operand = AM::pNewJobOperand(m_pJobPipeline, pOperand,this, m_iXmitNode, m_nPartition);
      
      // get the send node in the task
      Node* pSendNode = AM::pGrid()->pGetNode(m_iXmitNode);
      
      // send the operand job across the sendNode
      AM::vSend(m_pTask, pSendNode, pJob_Operand);
      
      // delete the operand job
      AM::vDeleteJobOperand(pJob_Operand);
      
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
bool Op::bSpawnJob(i8 iSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  ReqTuple xReqTuple(m_sName,iSessionId, false);

  m_pOpJob = AM::pNewJobUniversal(m_pJobPipeline, this, &xReqTuple);
  
  s4K szCmdStr;
  vToCmdStr(szCmdStr);
  
  TokenVec* pTknVec = new TokenVec;
  Tokeniser::vTokenise(pTknVec, szCmdStr);
  
  //vTrace("Spawning job for %s\n"_Stz, szCmdStr.szStz());
  
  eEC eError = AM::ePopulateJob(m_pOpJob, pTknVec);
  
  // delete the tokenVec
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  bool bSpawned = AM::bSpawnJob(m_pOpJob);
  
  m_pTask = AM::pExecutingTask(m_pOpJob);
  
  return bSpawned;
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
Task* Op::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
  
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP,_FOOTER_);
    
  szCmdStr->vCopy(m_sName);
  m_oEnameSet.vAppend(szCmdStr);
  //vTrace("%s\n"_Stz, szCmdStr);
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
