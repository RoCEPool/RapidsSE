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
  pStageSpec->vToPipeStr(sPipe.aStz());
  vTrace("Restated stage 0: ]]%s[[\n"_Stz,sPipe.aStz());
  vTrace("- - - - - - - - - - - - - - - - - -\n\n"_Stz);
  
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
/* query execute (participant job executes this code)                         */
/*----------------------------------------------------------------------------*/
void Job_Dynamic::vExecute(void)
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get no of operators to be executed on this node
  
  i8 iThisNode = AM::pGrid()->iThisNode();
  i8 nOps = m_pPipeline->nOpsOnNode(iThisNode);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // set acks remaining equal to no of operators to be executed on this node
  AM::vSetAcksRmg(pTask(),nOps);

  for (i8 i_Stage = 0; i_Stage < m_pPipeline->pStageVec()->nItems(); i_Stage++) {
    Stage* pStage = m_pPipeline->pStageVec()->pGet(i_Stage);

    for (i8 i_Nde = 0; i_Nde < pStage->pStageNdeVec()->nItems(); i_Nde++) {
      StageNde* pStageNde = pStage->pStageNdeVec()->pGet(i_Nde);
      
      // skip if Op not for this node
      if (pStageNde->iNode() == iThisNode) {
        // for each operator in the stage nde
        for (i8 i_Op = pStageNde->pOpVec()->nItems() - 1; i_Op >= 0; i_Op--) {
          Op* pOp = pStageNde->pOpVec()->pGet(i_Op);
          
          bool bSpawned = AM::bSpawnOp(m_pSession,pOp);
          ASSERT(bSpawned);
        }
      }
    }
    
    for (i8 i_Ptn = 0; i_Ptn < pStage->pStagePtnVec()->nItems(); i_Ptn++) {
      StagePtn* pStagePtn = pStage->pStagePtnVec()->pGet(i_Ptn);
      
      i8 iNode = TablePtn::iPartitionToNode(pStagePtn->m_i_Ptn);
      if (iNode == iThisNode) {
        // for each operator in the stage ptn
        for (i8 i_Op = pStagePtn->m_oOpVec.nItems() - 1; i_Op >= 0; i_Op--) {
          
          Op* pOp = pStagePtn->m_oOpVec.pGet(i_Op);
          
          bool bSpawned = AM::bSpawnOp(m_pSession,pOp);
          ASSERT(bSpawned);
        }
      }
    }
  }
  
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
            pJob_Dynamic->m_pStreamDef->vToMetadata(pJob_Dynamic->m_sMetadata.aStz());
            vTrace("%s\n"_Stz, pJob_Dynamic->m_sMetadata.aStz());
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
