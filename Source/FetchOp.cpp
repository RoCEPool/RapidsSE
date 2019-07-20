/*============================================================================*/
/* FetchOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "PushDown.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FetchOp* PushDown::_pFetchOp(void* aAddr)
{
  FetchOp* pFetchOp = reinterpret_cast<FetchOp*>(aAddr);
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* allocate a new entry buffer from heap                                      */
/*----------------------------------------------------------------------------*/
void* FetchOp::operator new(size_t)
{
  FetchOp* pFetchOp =
    static_cast<FetchOp*>(HeapMem::pAllocate(CN_FETCHOP));
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* free an entry buffer back to the heap                                      */
/*----------------------------------------------------------------------------*/
void FetchOp::operator delete(void* aAddress)
{
  FetchOp* pFetchOp = static_cast<FetchOp*>(aAddress);
  
  HeapMem::vFree(pFetchOp);
}


/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
FetchOp::FetchOp(Stz* szOpLabel, Jobs::Job* pJob)
: Op(CN_FETCHOP,szOpLabel,pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  m_pTablePtn = nullptr;
  m_pCrwBuffer = nullptr;

  _checkInitialised_(CN_FETCHOP);

  m_fpOpFun = FetchOp::vCall;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* initialize object                                                          */
/*----------------------------------------------------------------------------*/
FetchOp::~FetchOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* call the fetch function                                                    */
/*----------------------------------------------------------------------------*/
void FetchOp::vCall(void* aThis)
{  
  ((FetchOp*)aThis)->vFetch();
}

/*----------------------------------------------------------------------------*/
/* the fetch operator function                                                */
/*----------------------------------------------------------------------------*/
void FetchOp::vFetch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  for (m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
       m_pCrwBuffer;
       m_pCrwBuffer = m_pCrwBuffer->pNextBuffer()) {
    
    vTrace("%s::bFetch %p\n"_Stz,m_sOpLabel.aStz(),m_pCrwBuffer);
    
    vAppendOut(m_pCrwBuffer);
  }
  
  vAppendOut_endMarker();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Task* pTask_JobQuery = m_pJob->m_pExecutingTask;
  
  Pthread::vSleepSsec(3,CN_FETCHOP);
  
  bool bWasQueued = Scheduler::bAckTask(pTask_JobQuery,m_pTask);
  ASSERT(bWasQueued);
  
  m_pTask->vTransition(TR_EXIT);
}

/*----------------------------------------------------------------------------*/
/* set table partition                                                        */
/*----------------------------------------------------------------------------*/
void FetchOp::vTablePtn(TablePtn* pTablePtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);

  m_pTablePtn = pTablePtn;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(FetchOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
