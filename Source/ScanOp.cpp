/*============================================================================*/
/* ScanOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Scan (
  CN_SCANOP,          // class number
  OK_SOURCE,          // op kind
  "Scan"_Stz,         // first word of prompt command
  "Scn"_Stz,          // prefix
                  
  true,               // partitioned
  false,              // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  true,               // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  CN_NONE,            // what kind of output
                  
  ScanOp::pNew,       // function which parses the command and new's an op
  ScanOp::eCall,      // function which calls the op loop
  nullptr,            // function which calls the drain loop

  "scan <tableNm>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ScanOp* DataOperators::_pScanOp(void* aAddr)
{
  ScanOp* pScanOp = reinterpret_cast<ScanOp*>(aAddr);
  
  return pScanOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ScanOp::operator new(size_t)
{
  ScanOp* pScanOp =
    static_cast<ScanOp*>(HeapMem::pAllocate(CN_SCANOP));
  
  return pScanOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ScanOp::operator delete(void* aAddress)
{
  ScanOp* pScanOp = static_cast<ScanOp*>(aAddress);
  
  HeapMem::vFree(pScanOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ScanOp::ScanOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_SCANOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
  
  m_sTableNm   = ""_Stz;
  m_pTable     = nullptr;
  m_pTablePtn  = nullptr;
  m_pCrwBuffer = nullptr;
  m_bRunPrelim = true;
  
  _checkInitialised_(CN_SCANOP);

  m_fpCall = ScanOp::eCall;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ScanOp::~ScanOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ScanOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  ScanOp* pScanOp = new ScanOp(pOpSpec,pPipeline);
  
  pScanOp->m_sTableNm = pScanOp->m_pOpSpec->pEnameSet()->szValue((i8)0);
  
  return pScanOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ScanOp::eCall(void* aThisOp)
{  
  eOR eCallAgain = ((ScanOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ScanOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
  
  if (m_bRunPrelim)
    vPreliminary();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe the crw buffers
  
  if (m_pPipeline->bPulsed()) {
    vDebugPoint();
  }
  
  while (m_pCrwBuffer) {
    
    if (bIsCancelling()) {
      vTrace("%s::!CANCELED :   after piping %lld buffers\n"_Stz,
             m_sName.szStz(),
             m_iWorkDone);
      break;
    }
    
    vPipeOut(m_pCrwBuffer);
    
    vIncWorkDone();
    
    m_pCrwBuffer = m_pCrwBuffer->pNextBuffer();
    
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
void ScanOp::vPreliminary(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
  
  m_bRunPrelim = false;
  
  DicStack* pDicStack = new DicStack;
  AM::vJobDicStack(m_pJob, pDicStack);
  
  // find the table in the session dictionary stack
  m_pTable = Table::pFindTable(pDicStack, m_sTableNm);
  
  // get table ptn
  m_pTablePtn = m_pTable->pSlots()->pPartition(TablePtn::iPartitionToSlot(m_nPartition));
  
  // create a streamDef from teh table streamDef
  m_pStreamDef = new StreamDef(m_pTable->pStreamDef());
  
  // pipe StreamDef
  vPipeOut(new StreamDef(m_pStreamDef));
  
  // get first buffer
  m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ScanOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
