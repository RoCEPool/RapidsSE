/*============================================================================*/
/* Op.cpp -- Operator                                                         */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"

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
  m_bAckJobEnd    = false;
  m_pOperandQueue = new OperandQueue;
  m_pOpOut        = nullptr;

  m_fpOpFun       = nullptr;
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
  
  bool bCallAgain = (*m_fpOpFun)(this);
  
  if (bCallAgain) {
    ASSERT(bIsLocked());
    m_pTask->vTransition(TR_BIDE);
    return;
  }

  if (m_bAckJobEnd)
    vAckJob();
  
  m_pTask->vTransition(TR_EXIT);
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
    
  bool bWasQueued = Scheduler::bAckTask(pTask_JobQuery,m_pTask);
  ASSERT(bWasQueued);
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
void Op::vAppendIn(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s::vAppendIn : %p %s\n"_Stz,
         m_sOpLabel.aStz(),
         pOperand,
         aCN[pOperand->eClass()]);
  
  vGrabLock();
  
  m_pOperandQueue->vAppendOperand(pOperand);
  
  if (TS_BIDING != m_pTask->eState()) {
    vDropLock();
    return;
  }
    
  // trigger this biding op
  Scheduler::vTriggerTask(m_pTask);
}

/*----------------------------------------------------------------------------*/
/* append operand to output op's operand queue                                */
/*----------------------------------------------------------------------------*/
void Op::vAppendOut(Operand* pOperand)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  vTrace("%s::vAppendOut : %p %s\n"_Stz,
         m_sOpLabel.aStz(),
         pOperand,
         aCN[pOperand->eClass()]);
  m_pOpOut->vAppendIn(pOperand);
}

/*----------------------------------------------------------------------------*/
/* append end-marker to output op's operand queue                             */
/*----------------------------------------------------------------------------*/
void Op::vAppendOut_endMarker(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  EndMarker* pEndMarker = new EndMarker;
  
  vAppendOut(pEndMarker);
}

/*----------------------------------------------------------------------------*/
/* bide/wait for operand to arrive                                            */
/*----------------------------------------------------------------------------*/
void Op::vBideOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(bIsLocked());
  
  Scheduler::vBideTask(m_pTask);
  
  ASSERT(bIsLocked());
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
//  m_okLock.vDropWrite();
  
  m_pTask->vDropLock();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabLock(void)
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
/* set op out                                                                 */
/*----------------------------------------------------------------------------*/
void Op::vOpOut(Op* pOpOut)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  m_pOpOut = pOpOut;
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
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  vGrabLock();
  
  pOperand = m_pOperandQueue->pPopOperand();
  
  if (pOperand)
    vDropLock();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (pOperand) {
    vTrace("%s::pPopOperand : %p %s\n"_Stz,
           m_sOpLabel.aStz(),
           pOperand,
           aCN[pOperand->eClass()]);
  }
  
  return pOperand;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
