/*============================================================================*/
/* FetchOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Fetch (
  CN_FETCHOP,         // class number
  OK_SOURCE,          // op kind
  "Fetch"_Stz,        // first word of prompt command
  "Fet"_Stz,          // prefix
                   
  true,               // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?

  false,              // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                   
  CN_NONE,            // what kind of output
                   
  FetchOp::pNew,      // function which parses the command and new's an op
  FetchOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "fetch <tableNm> <indexNm>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
FetchOp* DataOperators::_pFetchOp(void* aAddr)
{
  FetchOp* pFetchOp = reinterpret_cast<FetchOp*>(aAddr);
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* FetchOp::operator new(size_t)
{
  FetchOp* pFetchOp =
    static_cast<FetchOp*>(HeapMem::pAllocate(CN_FETCHOP));
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void FetchOp::operator delete(void* aAddress)
{
  FetchOp* pFetchOp = static_cast<FetchOp*>(aAddress);
  
  HeapMem::vFree(pFetchOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
FetchOp::FetchOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_FETCHOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  m_sTableNm   = ""_Stz;
  m_sIndexNm   = ""_Stz;
  m_pTable     = nullptr;
  m_pIndex     = nullptr;
  m_pTablePtn  = nullptr;
  m_pEntBuffer = nullptr;
  m_bRunPrelim = true;
  m_nLimit     = C_UNKNOWN;
  
  _checkInitialised_(CN_FETCHOP);

  m_fpCall = FetchOp::eCall;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
FetchOp::~FetchOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* FetchOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  FetchOp* pFetchOp = new FetchOp(pOpSpec,pPipeline);
  EnameSet* pEnameSet = pFetchOp->m_pOpSpec->pEnameSet();
  
  // get the table name
  pFetchOp->m_sTableNm = pEnameSet->szValue((i8)0);
  
  // get the index name
  pFetchOp->m_sIndexNm = pEnameSet->szValue((i8)1);
  
  // do we have a limit ?
  if (pEnameSet->nEnames() == 3)
    pFetchOp->m_nLimit = pEnameSet->iValue((i8)2);
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR FetchOp::eCall(void* aThisOp)
{  
  eOR eCallAgain = ((FetchOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR FetchOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  if (m_bRunPrelim)
    vPreliminary();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe the entry buffers
  
  if (m_pPipeline->bPulsed()) {
    vDebugPoint();
  }
  
  while (m_pEntBuffer) {
    
    if (bIsCancelling()) {
      vTrace("%s::!CANCELED :   after piping %lld buffers\n"_Stz,
             m_sName.szStz(),
             m_iWorkDone);
      break;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // exit early if limit
    if ( m_nLimit == C_UNKNOWN || m_iWorkDone < m_nLimit) {
      m_iWorkDone += m_pEntBuffer->nItems();
      vPipeOut(m_pEntBuffer);
      m_pEntBuffer = m_pEntBuffer->pNextBuffer();
    }
    else {
      m_pEntBuffer = nullptr;
    }
    
#ifdef WITH_PULSING
    if (bPause()) {
      AM::vGrabTaskLock(m_pTask);
      vPipeOut(new PauseMarker);
      return OR_PAUSE;
    }
#endif
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe end marker
  
  vPipeOut_endMarker();
  
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* preliminary                                                                */
/*----------------------------------------------------------------------------*/
void FetchOp::vPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  // reset runPreliminary
  m_bRunPrelim = false;
  
  // find the table in the session dictionary stack
  //m_pTable = Table::pFindTable(m_pPipeline->pSession()->pPathStack(), m_sTableNm);
  
  // find the index in the session dictionary stack
  //m_pIndex = Table::pFindIndex(m_pPipeline->pSession()->pPathStack(), m_sTableNm, m_sIndexNm);
  
  // get table ptn
  m_pTablePtn = m_pTable->pSlots()->pPartition(TablePtn::iPartitionToSlot(m_nPartition));
  
  // create the stream def from teh table
  m_pStreamDef = new StreamDef(m_pTable->pStreamDef());
  
  // pipe StreamDef
  vPipeOut(new StreamDef(m_pStreamDef));
  
  // get first buffer
  IndexTree* pIndexTree = m_pTablePtn->pIndexTree(m_pIndex->iIndexNo());
  m_pEntBuffer = pIndexTree->pFirstH0Buffer();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(FetchOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
