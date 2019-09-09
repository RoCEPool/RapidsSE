/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "Operators.h"

#include "AM.hpp"
#include "eTR.h"
#include "eTS.h"

Atomic gixOp;
extern OpCatalog gxOpCatalog;

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
/* is given class a valid op?                                                 */
/*----------------------------------------------------------------------------*/
bool Operators::_bIsValidOp(eCN eClass)
{
  if (eClass >= CN_OP_TOCLAUSE && eClass <= CN_OP_TOFILTER)
    return true;
//  else
//  if (eClass >= CN_OP_TOCLAUSE && eClass <= CN_OP_TOFILTER)
//    return true;
  else
    return false;
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
Op::Op(eCN eClassNo, OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR)
: Focal(eClassNo,AM::iNextOpId(),AM::iJobUniqueId(pDigest->m_pJob))
{
  _precondition_(_MAGIC_,eClassNo);
  
  RASSERT(pOpSpec);
  RASSERT(aOpDefn);
  
  ASSERT(pOpSpec->szLabel()->bSame(szOpLabel));
  
  m_sName = aOpDefn->m_szCommand;
  
  m_pOpSpec       = pOpSpec;
  
  m_sLabel        = pOpSpec->szLabel();
  m_sArgs         = ""_Stz;
  m_sNextLabel    = "<nil>"_Stz;
  
  m_aOpDefn       = aOpDefn;
  m_pDigest       = pDigest;
  m_pJob          = pDigest->m_pJob;
  m_pTask         = nullptr;
  
  m_bAckJobEnd    = true;
  m_bPulsed       = false;
  m_iWorkQuanta   = 1;
  m_iWorkDone     = 0;

  m_pQueueLeft    = new OperandQueue;
  m_pQueueRight   = new OperandQueue;
  
  m_pOpOut        = nullptr;
  m_bAppendRight  = bAppR;

  m_nSlot            = -1;
  m_nLeftEndMarkers  = 1;
  m_nRightEndMarkers = 0;
  m_pStreamDef       = nullptr;
  
  m_bFirstCall    = true;
  
  m_fpCall        = m_aOpDefn ? m_aOpDefn->m_fpCall  : nullptr;
  m_fpDrain       = m_aOpDefn ? m_aOpDefn->m_fpDrain : nullptr;
  
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
  
  if (m_pStreamDef)
    delete m_pStreamDef;
  
  delete m_pOpSpec;
  
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
/* call op loop                                                               */
/*----------------------------------------------------------------------------*/
void Op::vOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  eOR eCallAgain;
//  if (bIsCancelling() && m_fpDrain)
//    eCallAgain = (*m_fpDrain)(this);
//  else

  if (m_bFirstCall) {
    m_bFirstCall = false;
    
    m_aOpDefn = gxOpCatalog.aFindOp(m_sName);
    
    m_fpCall  = m_aOpDefn ? m_aOpDefn->m_fpCall  : nullptr;
    m_fpDrain = m_aOpDefn ? m_aOpDefn->m_fpDrain : nullptr;
  }
  
  AM::vLogOpr(SL_INF,this,OA_OP_CALL,
              "Calling operator"_Stz); //!format-LogOpr
  
  eCallAgain = (*m_fpCall)(this);

  AM::vLogOpr(SL_INF,this,OA_OP_RETURNED,
              "Returned from operator with %s"_Stz, //!format-LogOpr
             aOR[eCallAgain]);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  switch (eCallAgain) {
   
    case OR_BIDE:
      ASSERT(bIsLocked());
      AM::vTaskTransition(m_pTask,TR_BIDE);
      return;
    
    case OR_EXIT:
      if (m_bAckJobEnd)
        vAckJob();
      AM::vTaskTransition(m_pTask,TR_EXIT);
      return;
    
    case OR_PAUSE:
      AM::vTaskTransition(m_pTask,TR_PAUSE);
      return;
    
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Task* pTask = AM::pExecutingTask(m_pJob);
  
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
//         m_sLabel.aStz(),
//         pOperand,
//         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  m_pQueueLeft->vAppendOperand(pOperand);

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
  //         m_sLabel.aStz(),
  //         pOperand,
  //         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  m_pQueueRight->vAppendOperand(pOperand);
  
  if (TS_BIDING != (eTS)AM::iTaskState(m_pTask)) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  AM::vTriggerBidingTask(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* get args                                                                   */
/*----------------------------------------------------------------------------*/
Stz* Op::szArgs(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sArgs;
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
  
  return m_pDigest->m_bCancelling;
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
/* get label                                                                  */
/*----------------------------------------------------------------------------*/
Stz* Op::szLabel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_sLabel;
}

/*----------------------------------------------------------------------------*/
/* set label                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vLabel(Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_sLabel = szLabel;
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
/* get operator id                                                            */
/*----------------------------------------------------------------------------*/
i8 Op::iOpId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_iUniqueId;
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
/* append operand to output op's operand queue                                */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  if (AM::bVerbose())
    vTrace("%s::vPipeOut :    %s %p\n"_Stz,
           m_sLabel.aStz(),
           aCN[pOperand->eClass()],
           pOperand);

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
}

/*----------------------------------------------------------------------------*/
/* append operand to the dynamic job's buffer queue                           */
/*----------------------------------------------------------------------------*/
void Op::vPipeToJob(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Queue* pBufferQueue = AM::pBufferQueue(m_pJob);
  
  if (AM::bVerbose())
    vTrace("%s::vPipeToJob :    %s %p\n"_Stz,
           m_sLabel.aStz(),
           aCN[pOperand->eClass()],
           pOperand);
  
  AM::vLogOpr(SL_INF,this,OA_OP_APPEND,
              "vPipeToJob operand %s to queue of dynamic job"_Stz, //!format-LogOpr
              aCN[pOperand->eClass()]);
  
  pBufferQueue->vAppend(pOperand);
  
}

/*----------------------------------------------------------------------------*/
/* append end-marker to output op's operand queue                             */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut_endMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  EndMarker* pEndMarker = new EndMarker;
  
  vPipeOut(pEndMarker);
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = m_pQueueLeft->pPopQueue();
  
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
    vTrace("%s::pPopQueue : %s %p\n"_Stz,
           m_sLabel.aStz(),
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
  
  pOperand = m_pQueueLeft->pPopQueue();
  
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
             m_sLabel.aStz(),
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
  
  pOperand = m_pQueueRight->pPopQueue();
  
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
             m_sLabel.aStz(),
             aCN[pOperand->eClass()],
             pOperand);
  }
  
  return pOperand;
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
