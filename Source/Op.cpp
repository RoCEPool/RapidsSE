/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "Operators.h"

#include "AM.hpp"
#include "eTR.h"
#include "eTS.h"

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
Op::Op(eCN eClassNo, Stz* szLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_iOpId         = AM::iNextOpId();
  
  m_sLabel        = szLabel;
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
  
  m_fpCall        = m_aOpDefn ? m_aOpDefn->m_fpCall  : nullptr;
  m_fpDrain       = m_aOpDefn ? m_aOpDefn->m_fpDrain : nullptr;
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
    eCallAgain = (*m_fpCall)(this);

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
  
  Task* pTask_JobQuery = AM::pExecutingTask(m_pJob);
    
  AM::vAckWaitingTask(pTask_JobQuery,m_pTask);
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
  
  return m_iOpId;
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

  if (m_bAppendRight)
    m_pOpOut->vAppendRight(pOperand);
  else
    m_pOpOut->vAppendLeft(pOperand);
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
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
