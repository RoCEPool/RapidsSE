/*============================================================================*/
/* OutputOp.cpp -- Output                                                     */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OutputOp* PushDown::_pOutputOp(void* aAddr)
{
  OutputOp* pOutputOp = reinterpret_cast<OutputOp*>(aAddr);
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* OutputOp::operator new(size_t)
{
  OutputOp* pOutputOp =
    static_cast<OutputOp*>(HeapMem::pAllocate(CN_OUTPUTOP));
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void OutputOp::operator delete(void* aAddress)
{
  OutputOp* pOutputOp = static_cast<OutputOp*>(aAddress);
  
  HeapMem::vFree(pOutputOp);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
OutputOp::OutputOp(Stz* szOpLabel, Jobs::Job* pJob)
: Op(CN_OUTPUTOP,szOpLabel,pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  _checkInitialised_(CN_OUTPUTOP);

  m_fpOpFun = OutputOp::vCall;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
OutputOp::~OutputOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the output function                                                   */
/*----------------------------------------------------------------------------*/
void OutputOp::vCall(void* aThis)
{
  ((OutputOp*)aThis)->vOutput();
}

/*----------------------------------------------------------------------------*/
/* the output operator function                                               */
/*----------------------------------------------------------------------------*/
void OutputOp::vOutput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
    if (! pOperand) {
      Pthread::vSleepMsec(100,CN_OUTPUTOP);
      continue;
    }
    
    if (CN_ENDMARKER == pOperand->eClass()) {
      vAppendOut(pOperand);
      break;
    }
    
    vAppendOut(pOperand);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pTask->vTransition(TR_EXIT);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(OutputOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
