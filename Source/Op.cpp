/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"
#include "AM.hpp"

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
Op::Op(eCN eClassNo, Stz* szOpLabel, Job* pJob)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_sOpLabel      = szOpLabel;
  m_pJob          = pJob;
  m_pTask         = nullptr;
  
  m_bAckJobEnd    = true;
  m_pOperandQueue = new OperandQueue;
  m_iWorkQuanta   = C_UNKNOWN;
  m_pOpOut        = nullptr;
  m_pOpContinue   = nullptr;

  m_fpOpFun       = nullptr;
  m_fpDrain       = nullptr;
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
  Task* pTask = Task::pThisTLSTask();
  
  Op* pOp = pTask->pOp();
  
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
  
  eOR eCallAgain = (*m_fpOpFun)(this);
  
  switch (eCallAgain) {
   
    case OR_BIDE:
      ASSERT(bIsLocked());
      m_pTask->vTransition(TR_BIDE);
      return;
    
    case OR_EXIT:
      if (m_bAckJobEnd)
        vAckJob();
      m_pTask->vTransition(TR_EXIT);
      return;
    
    case OR_PAUSE:
      m_pTask->vTransition(TR_PAUSE);
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
  
  Task* pTask_JobQuery = m_pJob->m_pExecutingTask;
    
  IGNORE_RETURN(Scheduler::bAckWaitingTask(pTask_JobQuery,m_pTask));
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
  
  vTrace("%s::vAppendOperand : %p %s\n"_Stz,
         m_sOpLabel.aStz(),
         pOperand,
         aCN[pOperand->eClass()]);
  
  vGrabTaskLock();
  
  m_pOperandQueue->vAppendOperand(pOperand);
  
  if (TS_BIDING != m_pTask->eState()) {
    vDropTaskLock();
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // trigger this biding op
  Scheduler::vTriggerBidingTask(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  m_okLock.vDropWrite();
  
  m_pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabTaskLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  //m_okLock.vGrabWrite();
  
  m_pTask->vGrabLock();
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  bool bLocked = m_okLock.bIsLocked();

  bool bLocked = m_pTask->bIsLocked();

  return bLocked;
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
/* append operand to output op's operand queue                                */
/*----------------------------------------------------------------------------*/
void Op::vPipeOut(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s::vPipeOut : %p %s\n"_Stz,
         m_sOpLabel.aStz(),
         pOperand,
         aCN[pOperand->eClass()]);
  
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
    vTrace("%s::pPopOperand : %p %s\n"_Stz,
           m_sOpLabel.aStz(),
           pOperand,
           aCN[pOperand->eClass()]);
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
Tasks::Task* Op::pTask(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_pTask;
}

/*----------------------------------------------------------------------------*/
/* set task                                                                   */
/*----------------------------------------------------------------------------*/
void Op::vTask(Tasks::Task* pTask)
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
