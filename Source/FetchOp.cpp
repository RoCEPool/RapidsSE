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
FetchOp::FetchOp(Stz* szOpLabel, Digest* pDigest)
: Op(CN_FETCHOP,&__Op_Fetch,szOpLabel,pDigest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  m_pTablePtn = nullptr;
  m_pCrwBuffer = nullptr;

  _checkInitialised_(CN_FETCHOP);

  m_fpOpFun = FetchOp::eCall;
  
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
/* parse & new function                                                       */
/*----------------------------------------------------------------------------*/
Op* FetchOp::pNew(Digest* pDigest, Job_Dynamic* pJob, Stz* szLabel,
                  TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  FetchOp* pFetchOp = new FetchOp(szLabel,pDigest);
  
  s32 sTableNm = pTknVec->pGet(i_Tkn_++)->aText();
  AM::vTableName(pJob,sTableNm);
  
  pFetchOp->m_sArgs = sTableNm;
  
  return pFetchOp;
}

/*----------------------------------------------------------------------------*/
/* call loop function                                                         */
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
/* loop function                                                              */
/*----------------------------------------------------------------------------*/
eOR FetchOp::eFetchLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_FETCHOP,_FOOTER_);
  
  // temp
  if (! m_pTablePtn) {
    m_pTablePtn = AM::pTable((Job_Dynamic*)m_pJob)->pSlots()->pPartition(0);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe StreamDef
  
  StreamDef* pStreamDef = AM::pTable((Job_Dynamic*)m_pJob)->pStreamDef();
  vPipeOut(pStreamDef);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe the crw buffers
  
  for (m_pCrwBuffer = m_pTablePtn->pRootCrwBuffer();
       m_pCrwBuffer;
       m_pCrwBuffer = m_pCrwBuffer->pNextBuffer()) {
    
    vTrace("%s::bFetch %p\n"_Stz,m_sLabel.aStz(),m_pCrwBuffer);
    
    vPipeOut(m_pCrwBuffer);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // pipe end marker
  
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
