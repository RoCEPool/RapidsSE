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
Op::Op(eCN eClassNo, OpSpec* pOpSpec, Pipeline* pPipeline)
: Focal(eClassNo,AM::iNextOpId(),AM::iJobUniqueId(pPipeline->pJob()))
{
  _precondition_(_MAGIC_,eClassNo);
  
  RASSERT(pOpSpec);
  
  m_sName = pOpSpec->szName();

  m_pOpSpec       = pOpSpec;
  m_sNextLabel    = "<nil>"_Stz;

  m_pPipeline     = pPipeline;
  m_pJobPipeline  = pPipeline->pJob();
  m_pOpJob        = nullptr;
  m_pTask         = nullptr;
  
  m_bAckJobEnd    = true;
  m_bPulsed       = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_bAppendRight  = pOpSpec->bAppendRight();
  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  
  m_pOpOut        = nullptr;
  

  m_nNode            = C_UNKNOWN;
  m_nPartition       = C_UNKNOWN;
  m_nLeftEndMarkers  = 1;
  m_nRightEndMarkers = 0;
  
  m_eTopologyX = PX_NONE;
  m_iNextStage = C_UNKNOWN;
  m_iXmitNode  = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask = AM::pThisTLSTask();
  AM::vTaskBeginOp(pTask);
  
  ++ gixOp;
  AM::vLogOpr(SL_INF,this,OA_OP_CONSTRUCT,
              "Op construct : %p (%lld ops exist)"_Stz, //!format-LogOpr
              this,* gixOp);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pQueueLeft;
  delete m_pQueueRight;
  
  //delete m_pOpSpec;
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Task* pTask = AM::pExecutingTask(m_pJobPipeline);
  
  AM::vLogOpr(SL_INF,this,OA_OP_ACK,
              "Acking job task"_Stz); //!format-LogOpr
  
  AM::vAckWaitingTask(pTask,m_pTask);
}

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJobEnd(bool bAckJobEnd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bAckJobEnd = bAckJobEnd;
}

/*----------------------------------------------------------------------------*/
/* append operand to this op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vAppendLeft(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
    
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
/* dec the leftEndMarkers and return it                                       */
/*----------------------------------------------------------------------------*/
i8 Op::nDecLeftEndMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return (--m_nLeftEndMarkers);
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  AM::vDropTaskLock(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Op::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  AM::vGrabTaskLock(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* inc work done                                                              */
/*----------------------------------------------------------------------------*/
void Op::vIncWorkDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iWorkDone++;
}

/*----------------------------------------------------------------------------*/
/* is cancelling?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bIsCancelling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pPipeline->bCancelling();
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bLocked = AM::bTaskIsLocked(m_pTask);

  return bLocked;
}

/*----------------------------------------------------------------------------*/
/* set the job                                                                */
/*----------------------------------------------------------------------------*/
void Op::vJobPipeline(Job* pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pJobPipeline = pJob;
}

/*----------------------------------------------------------------------------*/
/* get the leftEndMarkers                                                     */
/*----------------------------------------------------------------------------*/
i8 Op::nLeftEndMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nLeftEndMarkers;
}

/*----------------------------------------------------------------------------*/
/* set the leftEndMarkers                                                                */
/*----------------------------------------------------------------------------*/
void Op::vLeftEndMarkers(i8 nEndMarkers)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nLeftEndMarkers = nEndMarkers;
}

/*----------------------------------------------------------------------------*/
/* link this operator to the next stage                                       */
/*----------------------------------------------------------------------------*/
void Op::vLinkToStage(Stage* pStage)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(AM::eJobType(m_pOpJob) == JT_XMITOP);
  
  Op* pOpNext = nullptr;
  StageNde* pStageNde = nullptr;
  StagePtn* pStagePtn = nullptr;
  i8 i_Node = C_UNKNOWN;
  
  switch(m_eTopologyX) {
   
    case PX_ALL_NODES_TO_ORIGIN:
    case PX_ALL_PARTITIONS_TO_ORIGIN:
    case PX_ORIGIN_TO_ORIGIN: {
      i_Node = m_pPipeline->iOriginatingNode();
      RASSERT(pStage->pStageNdeVec()->nItems() == 1) ;
      pStageNde = pStage->pStageNde(i_Node);
      // get the first op of the stage nde
      pOpNext = pStageNde->pOpVec()->pGet(0);
      break;
    }
    
    case PX_ALL_PARTITIONS_TO_ALL_NODES: {
      RASSERT(0);
    }
      
    case PX_ALL_PARTITIONS_TO_ALL_PARTITIONS: {
      i_Node = TablePtn::iPartitionToNode(m_nPartition);
      pStagePtn = pStage->pStagePtn(m_nPartition);
      // get the first op of the stage ptn
      pOpNext = pStagePtn->pOpVec()->pGet(0);
      
      break;
    }
      
    default:
      RASSERT(0);
  }
  
  ASSERT( i_Node != C_UNKNOWN);
  
  // set the xmit node
  AM::vXmitNode(this,i_Node);
  
  // set the next label
  vNextLabel(pOpNext->m_sName);
  
  // set the op out
  vOpOut(pOpNext);
}

/*----------------------------------------------------------------------------*/
/* get op name                                                                */
/*----------------------------------------------------------------------------*/
Stz* Op::szName(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sName;
}

/*----------------------------------------------------------------------------*/
/* get next label                                                             */
/*----------------------------------------------------------------------------*/
Stz* Op::szNextLabel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sNextLabel;
}

/*----------------------------------------------------------------------------*/
/* set next label                                                             */
/*----------------------------------------------------------------------------*/
void Op::vNextLabel(Stz* szNextLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sNextLabel = szNextLabel;
}

/*----------------------------------------------------------------------------*/
/* get next stage                                                             */
/*----------------------------------------------------------------------------*/
i8 Op::iNextStage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iNextStage;
}

/*----------------------------------------------------------------------------*/
/* get operator id                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iOpId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iFocalId;
}

/*----------------------------------------------------------------------------*/
/* set out operator                                                           */
/*----------------------------------------------------------------------------*/
void Op::vOpOut(Op* pOpOut)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pOpOut = pOpOut;
}

/*----------------------------------------------------------------------------*/
/* set partition in op                                                        */
/*----------------------------------------------------------------------------*/
void Op::vPartition(i8 nPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nPartition = nPartition;
  
  // set the node tis partition belongs to 
  m_nNode = TablePtn::iPartitionToNode(m_nPartition);
}

/*----------------------------------------------------------------------------*/
/* get partition from op                                                      */
/*----------------------------------------------------------------------------*/
i8 Op::nPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nPartition;
}

/*----------------------------------------------------------------------------*/
/* operator is to pause?                                                      */
/*----------------------------------------------------------------------------*/
bool Op::bPause(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (! m_bPulsed)
    return false;
  
  if (0 == m_iWorkDone % m_iWorkQuanta)
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* append operand to next op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (AM::bVerbose())
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
           "vPipeOut :"_Stz,
           aCN[pOperand->eClass()],
           pOperand);

  switch(AM::eJobType(m_pOpJob)) {
    
    case JT_OUTPUTOP:
      vPipeToJob(pOperand);
      break;
      
    case JT_XMITOP:
      AM::vSendJobOperand(m_pTask,
                          pOperand,
                          m_pJobPipeline,
                          this);
      break;
  
    default:
      if (m_bAppendRight) {
        AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                    "PipeOut operand %s to right queue of %s"_Stz, //!format-LogOpr
                    aCN[pOperand->eClass()],
                    aCN[m_pOpOut->eClass()]);
        m_pOpOut->vAppendRight(pOperand);
      }
      else {
        AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                    "PipeOut operand %s to left queue of %s"_Stz, //!format-LogOpr
                    aCN[pOperand->eClass()],
                    aCN[m_pOpOut->eClass()]);
        m_pOpOut->vAppendLeft(pOperand);
      }
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline job's buffer queue                          */
/*----------------------------------------------------------------------------*/
void Op::vPipeToJob(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Queue* pBufferQueue = AM::pBufferQueue(m_pJobPipeline);
  
  if (AM::bVerbose())
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
           "vPipeToJob :"_Stz,
           aCN[pOperand->eClass()],
           pOperand);
  
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
              "vPipeToJob operand %s to queue of pipeline job"_Stz, //!format-LogOpr
              aCN[pOperand->eClass()]);
  
  pBufferQueue->vAppend(pOperand);
  
}

/*----------------------------------------------------------------------------*/
/* append operand to the pipeline startOp                                     */
/*----------------------------------------------------------------------------*/
void Op::vPipeToStart(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
                "PipeOut operand %s to left queue of %s"_Stz, //!format-LogOpr
                aCN[pOperand->eClass()],
                aCN[m_pOpOut->eClass()]);
  m_pOpOut->vAppendLeft(pOperand);  
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
               aCN[pOperand->eClass()]);
  else
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue no operand on left queue"_Stz); //!format-LogOpr

  //if (pOperand)
    vDropTaskLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (AM::bVerbose() && pOperand) {
    vTrace("%12s (N%lld) (P%02lld) (job:%p)::%12s %12s %p\n"_Stz,
           m_sName.szStz(),
           m_nNode,
           m_nPartition,
           m_pJobPipeline,
           "pPopQueue :"_Stz,
           aCN[pOperand->eClass()],
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueue2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueue operand %s from left queue"_Stz, //!format-LogOpr
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueLeft()->pPopQueue();
  
  if (pOperand)
    vDropTaskLock();
  
  if (pOperand) {
    if (AM::bVerbose())
      AM::vLogOpr(SL_INF,this,OA_OP_POP,
                  "PopQueueLeft operand %s from left queue"_Stz, //!format-LogOpr
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = pQueueRight()->pPopQueue();
  
  if (pOperand)
    vDropTaskLock();
  
  if (pOperand)
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueueRight operand %s from right queue"_Stz, //!format-LogOpr
               aCN[pOperand->eClass()]);
  else
    AM::vLogOpr(SL_INF,this,OA_OP_POP,
                "PopQueueRight no operand on right queue"_Stz); //!format-LogOpr
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    if (AM::bVerbose())
      vTrace("%s::pPopQueueRight : %s %p\n"_Stz,
             m_sName.szStz(),
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pQueueLeft;
}

/*----------------------------------------------------------------------------*/
/* get right operand queue                                                    */
/*----------------------------------------------------------------------------*/
OperandQueue* Op::pQueueRight(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pQueueRight;
}

/*----------------------------------------------------------------------------*/
/* get the rightEndMarkers                                                    */
/*----------------------------------------------------------------------------*/
i8 Op::nRightEndMarkers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_nRightEndMarkers;
}

/*----------------------------------------------------------------------------*/
/* set the rightEndMarkers                                                    */
/*----------------------------------------------------------------------------*/
void Op::vRightEndMarkers(i8 nEndMarkers)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_nRightEndMarkers = nEndMarkers;
}

/*----------------------------------------------------------------------------*/
/* spawn a job for this operator                                              */
/*----------------------------------------------------------------------------*/
bool Op::bSpawnJob(i8 iSessionId)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ReqTuple xReqTuple(m_sName,iSessionId, false);

  m_pOpJob = AM::pNewJobUniversal(this, &xReqTuple,
                                  this->m_iFocalId);
  
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
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vTask(Task* pTask)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_pTask = pTask;
}

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void Op::vToCmdStr(Stz* szCmdStr)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  szCmdStr->vCopy(m_sName);
  szCmdStr->vConcat(" "_Stz);
  m_pOpSpec->pEnameSet()->vAppend(szCmdStr);
  //vTrace("%s\n"_Stz, szCmdStr);
}

/*----------------------------------------------------------------------------*/
/* get cross topology                                                         */
/*----------------------------------------------------------------------------*/
ePX Op::eTopologyX(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eTopologyX;
}


/*----------------------------------------------------------------------------*/
/* set work quanta                                                            */
/*----------------------------------------------------------------------------*/
void Op::vWorkQuanta(i8 iWorkQuanta)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iWorkQuanta = iWorkQuanta;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
