/*============================================================================*/
/* FetchOp.cpp -- Fetch                                                       */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Fetch (
  CN_FETCHOP,         // class number
  OK_SIMPLE,          // op kind
  "Fetch"_Stz,        // first word of prompt command
  "Fet"_Stz,          // prefix
                   
  FetchOp::pNew,      // function which parses the command and new's an op
  FetchOp::eCall,     // function which calls the op loop
  nullptr,            // function which calls the drain loop

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
FetchOp::FetchOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_FETCHOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  m_pTable     = nullptr;
  m_pIndex     = nullptr;
  m_pTablePtn  = nullptr;
  m_pEntBuffer = nullptr;
  m_bRunPrelim = true;
  
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
Op* FetchOp::pNew(Session* pSession, Stz* szLabel,
                  Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                  TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  FetchOp* pFetchOp = new FetchOp(szLabel,pDigest,aOpDefn,bAppR);
  
  s32 sTableNm = pTknVec->pGet(i_Tkn_++)->aText();
  s32 sIndexNm = pTknVec->pGet(i_Tkn_++)->aText();
  
  AM::vTableName((Job_Dynamic*)pDigest->m_pJob,sTableNm);
  AM::vIndexName((Job_Dynamic*)pDigest->m_pJob,sIndexNm);
  
  pFetchOp->m_sArgs = sTableNm;
  
  // find the table in the session dictionary stack
  pFetchOp->m_pTable = Table::pFindTable(pSession->pPathStack(), sTableNm);
  
  // find the index in the session dictionary stack
  pFetchOp->m_pIndex = Table::pFindIndex(pSession->pPathStack(), sTableNm, sIndexNm);
  
  // get table ptn
  pFetchOp->m_pTablePtn = pFetchOp->m_pTable->pSlots()->pPartition(0);
  
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
  // pipe the crw buffers
  
  if (m_pDigest->m_bPulsed) {
    vDebugPoint();
  }
  
  while (m_pEntBuffer) {
    
    if (bIsCancelling()) {
      vTrace("%s::!CANCELED :   after piping %lld buffers\n"_Stz,
             m_sLabel.aStz(),
             m_iWorkDone);
      break;
    }
    
    vPipeOut(m_pEntBuffer);
    
    vIncWorkDone();
    
    m_pEntBuffer = m_pEntBuffer->pNextBuffer();
    
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
  
  m_bRunPrelim = false;
  
  // temp
  //  if (! m_pTablePtn) {
  //    m_pTablePtn = AM::pTable((Job_Dynamic*)m_pJob)->pSlots()->pPartition(0);
  //  }
  
  // pipe StreamDef
  StreamDef* pStreamDef = m_pTable->pStreamDef();
  vPipeOut(pStreamDef);
  
  // get first buffer
  IndexTree* pIndexTree = m_pTablePtn->pIndexTree(m_pIndex->iIndexNo());
  
  m_pEntBuffer = pIndexTree->pFirstH0Buffer();
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
