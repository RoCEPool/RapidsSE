/*============================================================================*/
/* Job_Dynamic.cpp -- Query Job                                                 */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Dynamic =
{
  CN_JOB_DYNAMIC,        // class number
  JK_LOCAL,              // job kind
  RS_DATA_ROWS,          // response kind
  "dynamic"_Stz,         // comment about the job
  "dynamic"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "dynamic_req"_Stz,     // the _req task seed
  nullptr,               // the _rcv task seed
  nullptr,               // the _exe task seed
  nullptr,               // the _end task seed
  nullptr,               // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  nullptr,               // prompt check function
  pJob_Dynamic_pre,      // prompt pre-process function
  vJob_Dynamic_post,     // prompt post-process function
  nullptr,               // job new function
  vJob_Dynamic_delete,   // job delete function
  nullptr,               // job pre-send function
  nullptr,               // job post-receive function
  vJob_Dynamic_show,     // job show function
  
  // help and example
  true,
  "dynamic"_Stz,
  "dynamic;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Dynamic_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Export_class;

  return &xJobClass_Dynamic;
}


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Dynamic* Jobs::_pJob_Dynamic(void* aAddress)
{
  Job_Dynamic* pJob_Dynamic = reinterpret_cast<Job_Dynamic*>(aAddress);
  
  RASSERT_TYPE(pJob_Dynamic,CN_JOB_DYNAMIC);
  
  return pJob_Dynamic;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Dynamic::operator new(size_t)
{
  Job_Dynamic* pJob_Dynamic =
    static_cast<Job_Dynamic*>(HeapMem::pAllocate(CN_JOB_DYNAMIC));
  
  return pJob_Dynamic;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Dynamic::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::operator delete(void* aAddress)
{
  Job_Dynamic* pJob_Dynamic = static_cast<Job_Dynamic*>(aAddress);
  
  HeapMem::vFree(pJob_Dynamic);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Dynamic::Job_Dynamic(ReqTuple* aReqTuple, Session* pSession)
: JobMark2(CN_JOB_DYNAMIC, pSession, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  //m_oOpVec
  
  m_pDigest      = new Digest(this);
  m_pBufferQueue = new Queue;
  m_pStreamDef   = nullptr;
  
  _checkInitialised_(CN_JOB_DYNAMIC);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Dynamic::~Job_Dynamic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
 
  delete m_pDigest;
  delete m_pBufferQueue;
  delete m_pStreamDef;
}


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pObject);

  pJob_Dynamic->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Dynamic::bPopulateJob(Session* pSession, TokenVec* pTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  i8 iNumTokens;
  
  ASSERT(pTokenVec->pGet(0)->bSame("dynamic"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - -
  // get no of tokens in token vector
  iNumTokens = pTokenVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be only 2 tokens
//  if (iNumTokens != 1) {
//    return nullptr;
//  }

  m_pDigest->m_pSession = pSession;
  m_pDigest->vParseCommand(pTokenVec);
  m_pDigest->vSetupJob();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query begin                                                                */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  i8 nOps = m_pDigest->m_oOpVec.nItems();
  AM::vSetAcksRmg(m_pExecutingTask,nOps);
  
  Session* pSession = AM::pThisTLSSession();

  for (i8 i_Op = m_pDigest->m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
   
    Op* pOp = m_pDigest->m_oOpVec.pGet(i_Op);
    bool bSpawned = AM::bSpawnOp(pSession,pOp);
    ASSERT(bSpawned);
  }
}

/*----------------------------------------------------------------------------*/
/* query end                                                                  */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query execute                                                              */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Dynamic::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Dynamic_pre(ReqTuple* aReqTuple, Session* pSession)
{
  Job_Dynamic* pJob_Dynamic = new Job_Dynamic(aReqTuple,pSession);
  //pJob->vDicPath(pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  bool bResult = pJob_Dynamic->bPopulateJob(pSession, pTokenVec);
  
  if ( !bResult) {
    pJob_Dynamic->vAddError(EC_INVALID_COMMAND);
  }
  
  return pJob_Dynamic;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Dynamic_post(Session* pSession, Job* pJob)
{
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJob);
  
//  if ( pJob_Dynamic->m_bHasError ) {
//
//    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
//           aEC[pJob_Dynamic->m_xResponse.m_eErrorCode]);
//
//    return;
//  }

  for (;;) {
    
    Operand* pOperand = _pOperand(pJob_Dynamic->m_pBufferQueue->pPop());
    
    if (! pOperand) {
      AM::pThisTLSPthread()->vSleepUsec(250,CN_JOB_DYNAMIC);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
        // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        pCrwBuffer->vPrintRows();
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        // print the responses received
        vTrace("success: %lld rows fetched\n"_Stz, *pJob_Dynamic->m_xResponse.m_ixRowsProcessed);
        delete (EndMarker*)pOperand;
        goto L_EXIT;
        
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pEntBuffer->nItems();
        pEntBuffer->vPrintRows();
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pIrwBuffer->nItems();
        pIrwBuffer->vPrintRows(pJob_Dynamic->m_pStreamDef->pColumnVec(0));
        delete pIrwBuffer;
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pSrtBuffer->nItems();
        pSrtBuffer->vPrintRows(pJob_Dynamic->m_pStreamDef);
        delete pSrtBuffer;
        break;
      }
        
        // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        pJob_Dynamic->m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
        if (AM::bMetadata()) {
          
          ColumnVec* pColVec = pJob_Dynamic->m_pStreamDef->m_oColVecAry.pGet(0);
          
          pJob_Dynamic->m_sMetadata.vCopy("metadata: ("_Stz);
          
          for (i8 i_Col = 0; i_Col < pColVec->nItems(); i_Col++) {
            if (i_Col != 0)
              pJob_Dynamic->m_sMetadata.vConcat(","_Stz);
            Column* pCol = pColVec->pGet(i_Col);
            pJob_Dynamic->m_sMetadata.vCatSprint("%s %s"_Stz, pCol->sColumnName().aChar(),
                                 aCT[pCol->eColumnType()]);
          }
          pJob_Dynamic->m_sMetadata.vConcat(")"_Stz);
          vTrace("%s\n"_Stz, pJob_Dynamic->m_sMetadata.aStz());
        }
        
        delete _pStreamDef(pOperand);
        break;
      }
        
        // -- -- -- -- -- --
        
      default:
        RASSERT(0);
        break;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return;
}

/*----------------------------------------------------------------------------*/
/* job delete function                                                        */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Dynamic_delete(Session* pSession, Job* pJob)
{
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJob);
  
  delete pJob_Dynamic;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Dynamic_show(Job* pJob)
{
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJob);

  pJob_Dynamic->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
