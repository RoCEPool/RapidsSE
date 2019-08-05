/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "PushDown.h"

#include "AM.hpp"
#include "eTR.h"
#include "eTS.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op* PushDown::_pOp(void* aAddr)
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
Op::Op(eCN eClassNo, Stz* szLabel, Digest* pDigest, OpDefn* aOpDefn)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_sLabel        = szLabel;
  m_sArgs         = ""_Stz;
  m_sNextLabel    = "<nil>"_Stz;
  
  m_aOpDefn       = aOpDefn;
  m_pDigest       = pDigest;
  m_pJob          = pDigest->m_pJob;
  m_pTask         = nullptr;
  
  m_bAckJobEnd    = true;
  m_bPulsed       = false;
  m_iWorkQuanta   = C_UNKNOWN;
  m_iWorkDone     = 0;

  m_pOperandQueue = new OperandQueue;
  m_pOperandQueue2 = nullptr;
  m_pOpOut        = nullptr;
  m_pOpContinue   = nullptr;

  m_fpCall        = m_aOpDefn ? m_aOpDefn->m_fpCall  : nullptr;
  m_fpDrain       = m_aOpDefn ? m_aOpDefn->m_fpDrain : nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op::~Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  delete m_pOperandQueue;
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
void Op::vAppendOperand(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
//         m_sLabel.aStz(),
//         pOperand,
//         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  m_pOperandQueue->vAppendOperand(pOperand);
  
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
/* is cancelling?                                                             */
/*----------------------------------------------------------------------------*/
bool Op::bIsCancelling(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //  bool bLocked = m_okLock.bIsLocked();
  
  return m_pDigest->m_bCancelling;
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  bool bLocked = m_okLock.bIsLocked();

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
  
  vTrace("%s::vPipeOut :    %s %p\n"_Stz,
         m_sLabel.aStz(),
         aCN[pOperand->eClass()],
         pOperand);

  m_pOpOut->vAppendOperand(pOperand);
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
/* append operand to self op's operand queue                                  */
/*----------------------------------------------------------------------------*/
void Op::vPipeSelf(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s::vPipeSelf : %p %s\n"_Stz,
         m_sLabel.aStz(),
         pOperand,
         aCN[pOperand->eClass()]);
  
  m_pOperandQueue->vAppendOperand(pOperand);
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabTaskLock();
  
  pOperand = m_pOperandQueue->pPopOperand();
  
  if (pOperand)
    vDropTaskLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (pOperand) {
    vTrace("%s::pPopOperand : %s %p\n"_Stz,
           m_sLabel.aStz(),
           aCN[pOperand->eClass()],
           pOperand);
  }
  
  return pOperand;
}

/*----------------------------------------------------------------------------*/
/* standard drain                                                             */
/*----------------------------------------------------------------------------*/
void Op::vStdDrain(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // to be coded...
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
/* inc work done                                                              */
/*----------------------------------------------------------------------------*/
void Op::vIncWorkDone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_iWorkDone++;
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
