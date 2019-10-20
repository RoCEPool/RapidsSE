/*============================================================================*/
/* Job_Dynamic.cpp -- Dynamic Job                                             */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Dynamic =
{
  CN_JOB_DYNAMIC,        // class number
  JK_DIST2PC,            // job kind
  RS_DATA_ROWS,          // response kind
  "dynamic"_Stz,         // comment about the job
  "dynamic"_Stz,         // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "dynamic_req"_Stz,     // the _req task seed
  "dynamic_beg"_Stz,     // the _rcv task seed
  "dynamic_exe"_Stz,     // the _exe task seed
  "dynamic_end"_Stz,     // the _end task seed
  "dynamic_can"_Stz,     // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Dynamic_pre,      // prompt pre-process function
  vJob_Dynamic_post,     // prompt post-process function
  nullptr,               // job new function
  vJob_Dynamic_delete,   // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
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
Job_Dynamic::Job_Dynamic(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
: JobMark2(aJobClass, aReqTuple, pSession)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  //m_oOpVec
  
  m_pPipeline     = nullptr;
  m_pPipelineSpec = nullptr;
  m_pStreamDef    = nullptr;
  m_pBufferQueue  = new Queue;
  m_sRequest      = ""_Stz;
  
  _checkInitialised_(CN_JOB_DYNAMIC);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Dynamic::~Job_Dynamic(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
 
  //delete m_pPipeline;  // ALKA: ????
  delete m_pPipelineSpec;
  delete m_pStreamDef;
  delete m_pBufferQueue;
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
bool Job_Dynamic::bPopulateJob(Session* pSession, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  // first token should be "dynamic"
  ASSERT(pTknVec->pGet(0)->bSame("dynamic"_Stz));
  
  // second token should be "pipeline"
  if ( ! pTknVec->pGet(1)->bSame("pipeline"_Stz) )
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Compile the token vector
  
  Dynamo* pDynamo = new Dynamo(pTknVec);
  bool bSuccess = pDynamo->bCompile();
  if ( ! bSuccess ) {
    delete pDynamo;
    return false;
  }
  
  PipelineSpec* pPipelineSpec = pDynamo->pPipelineSpec();
  StageSpec*   pStageSpec     = pPipelineSpec->pStageSpec(0);
  
  s4K sPipe("dynamic "_Stz);
  pStageSpec->vToPipeStr(sPipe.szStz());

#ifdef WITH_TRACE_DYNAMO
  vTrace("Restated stage 0: ]]%s[[\n"_Stz,sPipe.szStz());
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
#endif
  
  Tokeniser::vTokenise(pTknVec,sPipe);
  
  if (AM::bVerbose()) {
    SP_TknVecStr spTknVecStr;
    vTrace("TknVecStr ]]%s[[\n"_Stz,spTknVecStr.sz(pTknVec));
    vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set the pipeline
  
  m_pPipeline->vSession(pSession);
  m_pPipeline->vPopulateOpVec(pPipelineSpec);
  m_pPipeline->vSetupJob();
  
  delete pDynamo;
         
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
/* query begin (participant job executes this code)                           */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  m_pPipeline    = new Pipeline(this);
  m_pBufferQueue = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the pipeline
  
  Tokeniser* pTokenizer = new Tokeniser;
  TokenVec* pTokenVec = new TokenVec;
  pTokenizer->vTokenise(pTokenVec, m_sRequest);
  pTokenVec->vCutToken(pTokenVec->nTokens());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // populate the job
  
  bool bResult = bPopulateJob(m_pSession, pTokenVec);
  if ( ! bResult ) {
    vAddError(EC_INVALID_COMMAND);
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* query cancel (participant job executes this code)                          */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // get the first op of the first stage
  StartOp * pStartOp = _pStartOp(pStage->pOpVec()->pGet(0));
  
  // send a cancel marker to the stratOp
  pStartOp->vAppendLeft(new CancelMarker);
}

/*----------------------------------------------------------------------------*/
/* query end (participant job executes this code)                             */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // get the first op of the first stage
  StartOp * pStartOp = _pStartOp(pStage->pOpVec()->pGet(0));
  
  // send an end marker to the stratOp
  pStartOp->vAppendLeft(new EndMarker);
}

/*----------------------------------------------------------------------------*/
/* query execute (participant job executes this code)                         */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_DYNAMIC,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  
  i8 iThisNode = AM::pGrid()->iThisNode();
  i8 nOps = m_pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  
  AM::vSetAcksRmg(pTask(),nOps);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn operaotors for this node
  
  m_pPipeline->vSpawnOperators(iThisNode);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // get the first op of the first stage
  StartOp * pStartOp = _pStartOp(pStage->pOpVec()->pGet(0));
  
  // send an end marker to the stratOp
  pStartOp->vAppendLeft(new StartMarker);
  
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
Job* Jobs::pJob_Dynamic_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession)
{
  Job_Dynamic* pJob_Dynamic = new Job_Dynamic(aJobClass,aReqTuple,iForId,pSession);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  // convert tokenvec back to the request string
  pTokenVec->vToString(pJob_Dynamic->m_sRequest);
  
  return pJob_Dynamic;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Dynamic_post(Job* pJob)
{
  Job_Dynamic* pJob_Dynamic = _pJob_Dynamic(pJob);

  if ( pJob_Dynamic->m_bHasError ) {

    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
           aEC[pJob_Dynamic->m_xResponse.m_eErrorCode]);

    return;
  }

  for (;;) {
    
    Operand* pOperand = _pOperand(pJob_Dynamic->m_pBufferQueue->pPop());
    
    if (! pOperand) {
      AM::vNapUsec(250,CN_JOB_DYNAMIC);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
       
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        pCrwBuffer->vPrintRows(pJob_Dynamic->m_pStreamDef);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        pJob_Dynamic->m_xResponse.m_ixRowsProcessed += pCsvBuffer->nRecords();
        pCsvBuffer->vPrintRows();
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        // print the responses received
        AM::pThisTLSPthread()->vSleepUsec(1000000,CN_JOB_DYNAMIC);
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
        pIrwBuffer->vPrintRows(pJob_Dynamic->m_pStreamDef);
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
        if ( ! pJob_Dynamic->m_pStreamDef ) {
          pJob_Dynamic->m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
          
          // print the metadata
          if (AM::bMetadata()) {
            pJob_Dynamic->m_pStreamDef->vToMetadata(pJob_Dynamic->m_sMetadata.szStz());
            vTrace("%s\n"_Stz, pJob_Dynamic->m_sMetadata.szStz());
          }
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
void Jobs::vJob_Dynamic_delete(Job* pJob)
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
