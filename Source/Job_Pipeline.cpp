/*============================================================================*/
/* Job_Pipeline.cpp -- Pipeline Job                                           */
/*============================================================================*/

#include "Jobs.h"

JobClass xJobClass_Pipeline =
{
  CN_JOB_PIPELINE,        // class number
  JK_DIST2PC,            // job kind
  RS_DATA_ROWS,          // response kind
  "pipeline"_Stz,        // comment about the job
  "pipeline"_Stz,        // the prompt command
  nullptr,               // the second word
  nullptr,               // the _run task seed
  "pipeline_req"_Stz,    // the _req task seed
  "pipeline_beg"_Stz,    // the _rcv task seed
  "pipeline_exe"_Stz,    // the _exe task seed
  "pipeline_end"_Stz,    // the _end task seed
  "pipeline_can"_Stz,    // the _can task seed
  false,                 // check resources?
  false,                 // detached job?
  false,                 // buffer attached?
  DL_GRAB_READ,          // ddl lock model
  PY_NORMAL,             // priority
  JF_NONE,               // flags

  eJob_checkCluster,     // prompt check function
  pJob_Pipeline_pre,      // prompt pre-process function
  vJob_Pipeline_post,     // prompt post-process function
  nullptr,               // job new function
  vJob_Pipeline_delete,   // job delete function
  vJob_send,             // job pre-send function
  vJob_recv,             // job post-receive function
  vJob_Pipeline_show,     // job show function
  
  // help and example
  true,
  "pipeline"_Stz,
  "pipeline;"_Stz,

  { 0, 0, 0, 0 ,0, 0 } // seeds spawned
};

/*----------------------------------------------------------------------------*/
/* next function                                                              */
/*----------------------------------------------------------------------------*/
JobClass* Jobs::pJob_Pipeline_class(pFUN** pNextFun)
{
  *pNextFun = (pFUN*)pJob_Publish_class;

  return &xJobClass_Pipeline;
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Job_Pipeline* Jobs::_pJob_Pipeline(void* aAddress)
{
  Job_Pipeline* pJob_Pipeline = reinterpret_cast<Job_Pipeline*>(aAddress);
  
  RASSERT_TYPE(pJob_Pipeline,CN_JOB_PIPELINE);
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Pipeline::operator new(size_t)
{
  Job_Pipeline* pJob_Pipeline =
    static_cast<Job_Pipeline*>(HeapMem::pAllocate(CN_JOB_PIPELINE));
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Job_Pipeline::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::operator delete(void* aAddress)
{
  Job_Pipeline* pJob_Pipeline = static_cast<Job_Pipeline*>(aAddress);
  
  HeapMem::vFree(pJob_Pipeline);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Job_Pipeline::Job_Pipeline(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId)
: JobMark2(aJobClass, aReqTuple)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  //m_oOpVec
  
  m_pPipeline     = nullptr;
  m_pPipelineSpec = nullptr;
  m_pStreamDef    = nullptr;
  m_pBufferQueue  = new Queue;
  m_sRequest      = ""_Stz;
  
  _checkInitialised_(CN_JOB_PIPELINE);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Job_Pipeline::~Job_Pipeline(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
 
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
void Job_Pipeline::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pObject);

  pJob_Pipeline->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* populate the job                                                           */
/*----------------------------------------------------------------------------*/
bool Job_Pipeline::bPopulateJob(Session* pSession, TokenVec* pTknVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  // first token should be "pipeline"
  if ( ! pTknVec->pGet(0)->bSame("pipeline"_Stz) )
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
  
  s4K sPipe("pipeline "_Stz);
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
void Job_Pipeline::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* query begin (participant job executes this code)                           */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::vBegin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
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
  
  bool bResult = bPopulateJob(pSession(), pTokenVec);
  if ( ! bResult ) {
    vAddError(EC_INVALID_COMMAND);
    return;
  }
}

/*----------------------------------------------------------------------------*/
/* query cancel (participant job executes this code)                          */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::vCancel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // send cancelMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vOperandToStartOps(new CancelMarker, iThisNode);
}

/*----------------------------------------------------------------------------*/
/* query end (participant job executes this code)                             */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::vEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // send endMarker to the startOps of the first stage of the pipeline
  i8 iThisNode = AM::pGrid()->iThisNode();
  pStage->vOperandToStartOps(new EndMarker, iThisNode);
}

/*----------------------------------------------------------------------------*/
/* query execute (participant job executes this code)                         */
/*----------------------------------------------------------------------------*/
void Job_Pipeline::vExecute(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  
  i8 iThisNode = AM::pGrid()->iThisNode();
  i8 nOps = m_pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set acks remaining equal to no of operators to be executed on this node
  
  AM::vSetAcksRmg(pTask(),nOps);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // spawn operaotors for this node
  
  m_pPipeline->vSpawnOperatorJobs(iThisNode);
  
  // get the first stage of the pipeline
  Stage* pStage = m_pPipeline->pStageVec()->pGet(0);
  
  // send startMarker to the startOps of the first stage of the pipeline
  pStage->vOperandToStartOps(new StartMarker, iThisNode);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Job_Pipeline::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_JOB_PIPELINE);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Catalog Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt pre execute function                                                */
/*----------------------------------------------------------------------------*/
Job* Jobs::pJob_Pipeline_pre(Session* pSession, JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId)
{
  Job_Pipeline* pJob_Pipeline = new Job_Pipeline(aJobClass,aReqTuple,iForId);

  TokenVec* pTokenVec = pSession->m_pInput->pTknVec();
  
  // convert tokenvec back to the request string
  pTokenVec->vToString(pJob_Pipeline->m_sRequest);
  
  return pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* prompt post execute function                                               */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pipeline_post(Job* pJob)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJob);

  if ( pJob_Pipeline->m_bHasError ) {

    vTrace("error: (node%lld) %s\n"_Stz, AM::pGrid()->iThisNode(),
           aEC[pJob_Pipeline->m_xResponse.m_eErrorCode]);

    return;
  }

  for (;;) {
    
    Operand* pOperand = _pOperand(pJob_Pipeline->m_pBufferQueue->pPop());
    
    if (! pOperand) {
      AM::vNapUsec(250,CN_JOB_PIPELINE);
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
      
     // -- -- -- -- -- --
        
      case CN_CANCELMARKER: {
        vTrace("Query %lld cancelled!!! (rows fetched so far %lld)\n"_Stz, pJob_Pipeline->iJobId(),
               *pJob_Pipeline->m_xResponse.m_ixRowsProcessed);
        delete (_pCancelMarker(pOperand));
        goto L_EXIT;
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        pJob_Pipeline->m_xResponse.m_ixRowsProcessed += pCrwBuffer->nRowCount();
        pCrwBuffer->vPrintRows(pJob_Pipeline->m_pStreamDef);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        pJob_Pipeline->m_xResponse.m_ixRowsProcessed += pCsvBuffer->nRecords();
        pCsvBuffer->vPrintRows();
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        // print the responses received
        vTrace("success: %lld rows fetched\n"_Stz, *pJob_Pipeline->m_xResponse.m_ixRowsProcessed);
        delete (EndMarker*)pOperand;
        goto L_EXIT;
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        pJob_Pipeline->m_xResponse.m_ixRowsProcessed += pEntBuffer->nItems();
        pEntBuffer->vPrintRows();
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        pJob_Pipeline->m_xResponse.m_ixRowsProcessed += pIrwBuffer->nItems();
        pIrwBuffer->vPrintRows(pJob_Pipeline->m_pStreamDef);
        delete pIrwBuffer;
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        pJob_Pipeline->m_xResponse.m_ixRowsProcessed += pSrtBuffer->nItems();
        pSrtBuffer->vPrintRows(pJob_Pipeline->m_pStreamDef);
        delete pSrtBuffer;
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        if ( ! pJob_Pipeline->m_pStreamDef ) {
          pJob_Pipeline->m_pStreamDef = new StreamDef(_pStreamDef(pOperand));
          
          // print the metadata
          if (AM::bMetadata()) {
            pJob_Pipeline->m_pStreamDef->vToMetadata(pJob_Pipeline->m_sMetadata.szStz());
            vTrace("%s\n"_Stz, pJob_Pipeline->m_sMetadata.szStz());
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
void Jobs::vJob_Pipeline_delete(Job* pJob)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJob);
  
  delete pJob_Pipeline;
}

/*----------------------------------------------------------------------------*/
/* show job function                                                          */
/*----------------------------------------------------------------------------*/
void Jobs::vJob_Pipeline_show(Job* pJob)
{
  Job_Pipeline* pJob_Pipeline = _pJob_Pipeline(pJob);

  pJob_Pipeline->vShow();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
