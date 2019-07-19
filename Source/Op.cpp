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
Op::Op(eCN eClassNo, Job* pJob, Stz* szOpLabel)
: Object(eClassNo)
{
  _precondition_(_MAGIC_,eClassNo);
  
  m_sOpLabel      = szOpLabel;
  m_pJob          = pJob;
  m_bAckJobEnd    = false;
  m_fpOpFun       = nullptr;
  m_pOpOut        = nullptr;

  m_okLock.vInitLock(LK_OP);
  m_eOpState      = OZ_NONE;
  m_pOperandQueue = new OperandQueue;
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
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
void Op::vOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

//  while (bGetNextOperand()) {
//    (*m_fpOpFun)(this);
//  }
  
  m_okLock.vGrabWrite();
  m_eOpState = OZ_PROCESSING;
  m_okLock.vDropWrite();
  
  (*m_fpOpFun)(this);
  
  RASSERT(OZ_PROCESSING == m_eOpState);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set ack job on end marker                                                  */
/*----------------------------------------------------------------------------*/
void Op::vAckJobEnd(bool m_bAckJobEnd)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_bAckJobEnd = m_bAckJobEnd;
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
  
  m_okLock.vGrabWrite();
  
  m_pOperandQueue->vAppendOperand(pOperand);
  
  // trigger this op where the operand is being queued
  if (OZ_BIDING == m_eOpState)
    Scheduler::vTriggerOperand(m_pJob->m_pExecutingTask);
  
  m_okLock.vDropWrite();
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
  
  ASSERT(m_okLock.bIsLocked());
  
  Scheduler::vBideOperand(m_pJob->m_pExecutingTask);
  
  ASSERT(m_okLock.bIsLocked());
}

/*----------------------------------------------------------------------------*/
/* drop lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vDropLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* grab lock                                                                  */
/*----------------------------------------------------------------------------*/
void Op::vGrabLock(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* is locked?                                                                 */
/*----------------------------------------------------------------------------*/
bool Op::bIsLocked(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  bool bLocked = m_okLock.bIsLocked();
  
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
/* get op state                                                               */
/*----------------------------------------------------------------------------*/
eOZ Op::eOpState(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  return m_eOpState;
}

/*----------------------------------------------------------------------------*/
/* set op state                                                               */
/*----------------------------------------------------------------------------*/
void Op::vOpState(eOZ eOpState)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  m_eOpState = eOpState;
}

/*----------------------------------------------------------------------------*/
/* pop operand queue                                                          */
/*----------------------------------------------------------------------------*/
Operand* Op::pPopOperand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Operand* pOperand = nullptr;
  
  for (;;) {
    
    m_okLock.vGrabWrite();
  
    pOperand = m_pOperandQueue->pPopOperand();
  
    if (! pOperand) {
      
      vBideOperand();
      
      m_eOpState = OZ_BIDING;
      
      m_okLock.vDropWrite();
      break;
    }
  
    m_eOpState = OZ_PROCESSING;
    
    m_okLock.vDropWrite();
    break;
  }
  
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
