/*============================================================================*/
/* ScanOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Scan (
  CN_SCANOP,          // class number
  OK_SIMPLE,          // op kind
  "Scan"_Stz,         // first word of prompt command
  "Scn"_Stz,          // prefix
                   
  ScanOp::pNew,      // function which parses the command and new's an op
  ScanOp::eCall,     // function which calls the op loop
  nullptr,           // function which calls the drain loop

  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ScanOp* PushDown::_pScanOp(void* aAddr)
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
ScanOp::ScanOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_SCANOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);
  
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
Op* ScanOp::pNew(Session* pSession, Stz* szLabel,
                 Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                 TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  ScanOp* pScanOp = new ScanOp(szLabel,pDigest,aOpDefn,bAppR);
  
  s32 sTableNm = pTknVec->pGet(i_Tkn_++)->aText();
  AM::vTableName((Job_Dynamic*)pDigest->m_pJob,sTableNm);
  
  pScanOp->m_sArgs = sTableNm;
  
  // find the table in the session dictionary stack
  pScanOp->m_pTable = Table::pFindTable(pSession->pPathStack(), sTableNm);
  
  // get table ptn
  pScanOp->m_pTablePtn = pScanOp->m_pTable->pSlots()->pPartition(0);
  
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
  
  if (m_pDigest->m_bPulsed) {
    vDebugPoint();
  }
  
  while (m_pCrwBuffer) {
    
    if (bIsCancelling()) {
      vTrace("%s::!CANCELED :   after piping %lld buffers\n"_Stz,
             m_sLabel.aStz(),
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
  
  // temp
//  if (! m_pTablePtn) {
//    m_pTablePtn = AM::pTable((Job_Dynamic*)m_pJob)->pSlots()->pPartition(0);
//  }
  
  // pipe StreamDef
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  vPipeOut(pStreamDef);
  
  // get first buffer
  m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
}

/*----------------------------------------------------------------------------*/
/* set table partition                                                        */
/*----------------------------------------------------------------------------*/
void ScanOp::vTablePtn(TablePtn* pTablePtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SCANOP,_FOOTER_);

  m_pTablePtn = pTablePtn;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(ScanOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
