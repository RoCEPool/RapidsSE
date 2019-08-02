/*============================================================================*/
/* FetchOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Fetch (
  CN_FETCHOP,         // class number
  OK_SIMPLE,          // op kind
  "Fetch"_Stz,        // first word of prompt command
                   
  FetchOp::pNew,      // function which parses the command and new's an op
  FetchOp::eCall,     // function which does the op
  nullptr,            // function which drains the operand queue

  "<help>"_Stz
);

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

  m_fpOpFun = FetchOp::eCall;
  
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* function which parses the command and new's an op                          */
/*----------------------------------------------------------------------------*/
Op* FetchOp::pNew(Job_Dynamic* pJob, TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  FetchOp* pFetchOp = new FetchOp("FetchOp:"_Stz,(Job*)pJob);
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* call the fetch loop function                                               */
/*----------------------------------------------------------------------------*/
eOR FetchOp::eCall(void* aThis)
{  
  eOR eCallAgain = ((FetchOp*)aThis)->eFetchLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the fetch operator loop function                                           */
/*----------------------------------------------------------------------------*/
eOR FetchOp::eFetchLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  for (m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
       m_pCrwBuffer;
       m_pCrwBuffer = m_pCrwBuffer->pNextBuffer()) {
    
    vTrace("%s::bFetch %p\n"_Stz,m_sOpLabel.aStz(),m_pCrwBuffer);
    
    vPipeOut(m_pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  vPipeOut_endMarker();
  
  return OR_EXIT;
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
