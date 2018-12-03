/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern AttachMap*   gpAttachMap;
extern ActivityMap* gpActivityMap;

/*----------------------------------------------------------------------------*/
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(Stz* pVersion, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_CheckLibraryVersion"_Stz;
  eAC  eApiCall = AC_CheckLibraryVersion;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    TRACE_ENTRY_P1(pVersion);
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, pVersion);
    
    if ( ! pVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Stz,
          pVersion, RAPIDSSE_VERSION_STRING);
      
      TRACE_ERROR("b"_Stz, EC_VERSION_MISMATCH);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return EC_VERSION_MISMATCH;
    }
    
    TRACE_EXIT("ea"_Stz);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert and all session jobs                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsertJobs(Session* pSession, void* pJobPtr, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_EndInsertJobs"_Stz;
  eAC  eApiCall = AC_EndInsertJobs;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_EndInsertJobs);                               // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
    
    //pJob_InsertRow->m_pWorkingBuffer[0]->vShow(true);
    
    // create Job_InsertTable
//    JobClass* pJobClass_InsertTable = JobClass::pFind(CN_JOB_INSERTTABLE);
//    ASSERT(pJobClass_InsertTable)
//    Job_InsertTable* pJob_InsertTable =
//      static_cast<Job_InsertTable*>(pJobClass_InsertTable->pPreJob(pSession));
    
    Job_InsertTable* pJob_InsertTable = pJob_InsertRow->m_pJob_InsertTable;
    pSession->m_pJobStack->vPushObject(pJob_InsertTable);
    
    vApiJob(pSession,pJob_InsertTable);
    vApiLocation(pSession,105);
    
    i8 iJobSeq = pJob_InsertTable->m_iJobSeq;
    
    // set insertRowJobId in the endInsert job
    pJob_InsertTable->m_pJobInsertRow = pJob_InsertRow;
    
    // lock this job's mutex
    i8 rCode = pthread_mutex_lock(&pJob_InsertTable->m_xThreadMutex);
    if (0 != rCode)
      vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
    
    vApiLocation(pSession,106);
    
    // set the job state to be locked
    pJob_InsertTable->m_eState = JS_LOCKED;
    
    // spawn the insertTable job
    RapidsSE_bSpawnJob(pJob_InsertTable, SS_SEED_REQ, pSession);
    vApiLocation(pSession,107);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // wait for condition & state
    
    // set the job state to be waiting
    pJob_InsertTable->m_eState = JS_WAITING;
    
    // wait till signalled
    do {
      
      RASSERT( _bIsValidJob(pJob_InsertTable->eClass()) );
      RASSERT( iJobSeq == pJob_InsertTable->m_iJobSeq );
      
      //    vTrace("ThreadCond:\n"_Stz);
      //    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
      //
      //    vTrace("ThreadMutex:\n"_Stz);
      //    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));
      
      rCode = pthread_cond_wait(&pJob_InsertTable->m_xThreadCond,
                                &pJob_InsertTable->m_xThreadMutex);
      
      RASSERT( _bIsValidJob(pJob_InsertTable->eClass()) );
      RASSERT( iJobSeq == pJob_InsertTable->m_iJobSeq );
      
      if (0 != rCode)
        vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
      
    } while (pJob_InsertTable->m_eState != JS_SIGNALLED);

    RASSERT( _bIsValidJob(pJob_InsertTable->eClass()) );
    RASSERT( iJobSeq == pJob_InsertTable->m_iJobSeq );
    
    vApiLocation(pSession,108);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    // set the job state back to running
    pJob_InsertTable->m_eState = JS_RUNNING;
    
    // delete all jobs in the ssession
    vDeleteJobs(pSession);
    
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
   return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end all jobs of this session                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndJobs(Session* pSession, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_EndJobs"_Stz;
  eAC  eApiCall = AC_EndJobs;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
  
  vApiCall(pSession,AC_EndJobs);                                     // API CALL
  vApiLocation(pSession,100);
  
  eEC eResult;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate the session
  VALIDATE_SESSION("a"_Stz);
  
  // delete the jobs from the sessions jobStack
  vDeleteJobs(pSession);
  
  // mark session as exiting (any fetches still running will stop)
  pSession->m_bSessionExiting = true;
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* RapidsSE_pExecCmdStr(Stz* aCommand, Session* pSession, bool bPostJob, bool bFromPrompt)
{
  Stz* aApiCall = "RapidsSE_pExecCmdStr"_Stz;
  eAC  eApiCall = AC_ExecCmdStr;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_ExecCmdStr);                                  // API CALL
  vApiLocation(pSession,100);

  CommandProcessor* pCommandProcessor = pSession->m_pCommandProcessor;
  
  vApiLocation(pSession,101);
  
  pCommandProcessor->vTokenise(aCommand);
  
  vApiLocation(pSession,102);
  
  bool bFound = pCommandProcessor->bFindJobClass();
  if (! bFound) {
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,910);
    
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return nullptr;
  }
  
  vApiLocation(pSession,103);
  
  pCommandProcessor->vPreJob(pSession);
  
  vApiLocation(pSession,104);
  
  Job* pJob = pCommandProcessor->pJob();
  if (!pJob) {
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,920);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return nullptr;
  }
  vApiJob(pSession,pJob);                                             // API JOB
  vApiLocation(pSession,105);
  
  i8 iJobSeq = pJob->m_iJobSeq;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // lock this job's mutex
  
  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_lock() error; rCode=%lld\n"_Stz,rCode);
  
  vApiLocation(pSession,106);
  
  // set the job state to be locked
  pJob->m_eState = JS_LOCKED;
  
  //vApiLocation(pSession,107);
  
  // spawn the job
  RapidsSE_bSpawnJob(pJob, SS_SEED_REQ, pSession);
  
  vApiLocation(pSession,108);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // wait for condition & state
  
  // set the job state to be waiting
  pJob->m_eState = JS_WAITING;
  
  // wait till signalled
  i8 iLocation = 10000;
  do {
    
    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );
    
//    vTrace("ThreadCond:\n"_Stz);
//    MemLib::vHexDump(&pJob->m_xThreadCond,  sizeof(pthread_cond_t));
//
//    vTrace("ThreadMutex:\n"_Stz);
//    MemLib::vHexDump(&pJob->m_xThreadMutex, sizeof(pthread_mutex_t));

    rCode = pthread_cond_wait(&pJob->m_xThreadCond,
                              &pJob->m_xThreadMutex);

    RASSERT( _bIsValidJob(pJob->eClass()) );
    RASSERT( iJobSeq == pJob->m_iJobSeq );

    vApiLocation(pSession,iLocation++);
    if (0 != rCode)
      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);

  } while (pJob->m_eState != JS_SIGNALLED);

  RASSERT( _bIsValidJob(pJob->eClass()) );
  RASSERT( iJobSeq == pJob->m_iJobSeq );

  vApiLocation(pSession,109);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // set the job state back to running
  pJob->m_eState = JS_RUNNING;

  // unlock this job's mutex
  rCode = pthread_mutex_unlock(&pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_mutex_unlock() error; rCode=%lld\n"_Stz,rCode);

  vApiLocation(pSession,110);

  RASSERT( _bIsValidJob(pJob->eClass()) );
  RASSERT( iJobSeq == pJob->m_iJobSeq );

  //vApiLocation(pSession,111);
  
  if (bPostJob)
    pCommandProcessor->vPostJob(pSession);
  
  //vApiLocation(pSession,112);
  
  vApiReturn(pSession);
  // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteCommand(Session* pSession, Stz* aCommand,
                            Stz* sCatalogName, Stz* sSchemaName,
                            void** ppJob, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_ExecuteCommand"_Stz;
  eAC  eApiCall = AC_ExecuteCommand;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_ExecuteCommand);                              // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(aCommand);

    vApiLocation(pSession,200);

    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // set the catalog and schema in the session
    eEC eError = pSession->eInit(sCatalogName, sSchemaName);

    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      vApiError(pSession,eError);                                     // API ERROR
      vApiLocation(pSession,811);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
    
    // execute the command
    vApiLocation(pSession,210);
    Job* pJob = RapidsSE_pExecCmdStr(aCommand, pSession, false, false);
    vApiLocation(pSession,220);

    // check if job had an error
    if (pJob->m_bHasError) {
      for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
        if (pJob->m_xNode[i_Node].m_eErrorCode != EC_NONE) {
          //vTrace("ERROR*** (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob->m_xNode[i_Node].m_eErrorCode]);
          TRACE_EXIT("eb"_Stz);
          vApiError(pSession,eError);                                 // API ERROR
          vApiLocation(pSession,812);
          pSession->m_pJobStack->vPushObject(pJob);
          ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
          return pJob->m_xNode[i_Node].m_eErrorCode;
        }
      }
      ASSERT(0);
    }
    
    vApiLocation(pSession,230);

    // return job in ppJob
    *ppJob = pJob;
    
    vApiLocation(pSession,240);

    // push the job on the sessions job stack
    pSession->m_pJobStack->vPushObject(pJob);
    
    TRACE_EXIT("ec"_Stz);

    vApiLocation(pSession,250);

    vApiReturn(pSession);                                            // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextBuffer(Session* pSession, void* pJobPtr, void** ppFetchBuffer, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_FetchNextBuffer"_Stz;
  eAC  eApiCall = AC_FetchNextBuffer;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_FetchNextBuffer);                             // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
           
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, ppFetchBuffer);
    
    Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
    
    // if we had an entry buffer delete it
    pJob_FetchRows->vDeleteBuffer();
    
    // are we done ?
    if (pJob_FetchRows->m_nLastBuffersScanned == pJob_FetchRows->m_nLastBuffers) {
      *ppFetchBuffer = nullptr;
  
      // trace exit
      TRACE_EXIT("ea"_Stz);
      vApiReturn(pSession);                                        // API RETURN
      vApiLocation(pSession,910);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return EC_NONE;
    }
    
    // pop an entryBuffer off the queue
    ValWrk xValWrk;
    while (!pJob_FetchRows->m_pBufferQueue->bPop(&xValWrk)) {
      PosixThread::vSleepMilli(1);
    }
    
    //vTrace("Got buffer %lld\n"_Stz, pJob_FetchRows->m_iPartition);
    
    //ASSERT(pSession->m_pJob == pJob_FetchRows);
    
    // get entry buffer
    PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
    pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(pPtrVal->pPtr());
    //vTrace("Got buffer %p %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer, pJob_FetchRows->m_iPartition);
    
    // is it a last buffer ?
    if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
      pJob_FetchRows->m_nLastBuffersScanned++;
//      vTrace("FNB: LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pFetchBuffer,
//             pJob_FetchRows->m_pTable->aTableName(),
//             pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    }
    
//    vTrace("FNB: NOT LAST %p %s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pFetchBuffer,
//           pJob_FetchRows->m_pTable->aTableName(),
//           pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    
    // return the fetched buffer
    *ppFetchBuffer = pJob_FetchRows->m_pFetchBuffer;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorMessage(Session* pSession, int iErrorCode, void** ppError, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetErrorMessage"_Stz;
  eAC  eApiCall = AC_GetErrorMessage;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetErrorMessage);                             // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppError);
    
    // get the error message
    pSession->m_sResponse = aEC[(eEC)iErrorCode];
    *ppError = pSession->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);

    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,802);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRow(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRow"_Stz;
  eAC  eApiCall = AC_GetRow;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRow);                                      // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  // trace entry
  TRACE_ENTRY_P0();

  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // get the jobptr
  Job* pJob = (Job*)pJobPtr;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
 
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, ppRow);
  
  // tokenize the response
  TokenVec* pTknVec = Tokeniser::pTokenise(pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_sResponse);
  
  i8 iRows = 0;
  i8 iToken = 0;
  
  while (true) {
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame("row:"_Stz));
    
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame("("_Stz));
    
    if (iRows == iRow)
      break;
    
    while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
     iToken++;
    }
  
    RASSERT(pTknVec->pGet(iToken++)->aText()->bSame(")"_Stz));
    iRows++;
  }
  
  pSession->m_sResponse.vCopy(""_Stz);
  while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
    pSession->m_sResponse.vConcat(pTknVec->pGet(iToken++)->aText());
  }
  
  // return the row
  *ppRow = pSession->m_sResponse.aChar();
  //vTrace("Returning row %s\n"_Stz, pSession->m_sResponse.aChar());
  
  // delete the token vector
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCount(Session* pSession, void* pJobPtr, i8 *pRowCount, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRowCount"_Stz;
  eAC  eApiCall = AC_GetRowCount;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowCount);                                 // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  Job* pJob = (Job*)pJobPtr;
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, pRowCount);
  
  *pRowCount = *(pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed);
  //vTrace("RowCount = %lld\n"_Stz, *pRowCount);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCountTableColumn(Session* pSession, void* pJobPtr, i8 *pRowCount, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRowCountTableColumn"_Stz;
  eAC  eApiCall = AC_GetRowCountTableColumn;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowCountTableColumn);                      // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the pointer
  VALIDATE_PARAM("c"_Stz, pRowCount);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJobPtr);
  Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
  ColumnVec* pColumnVec = pTable->pColumnVec();
  pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed =  pColumnVec->nItems() - 1;
  *pRowCount = *(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_ziRowsProcessed);
  
  //vTrace("RowCount = %lld\n"_Stz, pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowTableColumn(Session* pSession, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRowTableColumn"_Stz;
  eAC  eApiCall = AC_GetRowTableColumn;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_GetRowTableColumn);                           // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate session
  VALIDATE_SESSION("a"_Stz);
  
  // validate the job
  VALIDATE_JOB("b"_Stz);
  
  // validate the reference pointer
  VALIDATE_REFPOINTER("c"_Stz, ppRow);
  
  Job_ListColumns* pJob_ListColumns = _pJob_ListColumns(pJobPtr);
  
  Table* pTable = (Table*)StrLib::iToI8(pJob_ListColumns->m_xNode[gpAttachMap->m_pGrid->m_iThisNode].m_sResponse.aChar());
  ColumnVec* pColumnVec = pTable->pColumnVec();
  
  Column* pColumn = pColumnVec->pGet(iRow);
  
  sprintf(pSession->m_sResponse.aChar(),"%lld,%s,%s,%lld,%lld,%s,%s",
           pColumn->iColumnNo(),
           pColumn->sColumnName().aChar(),
           aCT[pColumn->eColumnType()]->aChar(),
           pColumn->iColumnLength(),
           pColumn->iColumnScale(),
           pColumn->bIsNullable() ? "true" : "false",
           pColumn->bIsPartitionKey() ? "true": "false"
  );
    
  
  *ppRow = pSession->m_sResponse.aChar();
  
  // trace exit
  TRACE_EXIT("ea"_Stz);
  
  vApiReturn(pSession);                                            // API RETURN
  vApiLocation(pSession,900);
  
  ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(Session* pSession, void* pJobPtr, Stz* sRow, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_InsertRow"_Stz;
  eAC  eApiCall = AC_InsertRow;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_InsertRow);                                   // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);

    // validate parameter
    VALIDATE_PARAM("b"_Stz, sRow);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // validate table
    //VALIDATE_TABLE("c"_Stz);
    
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
    
    i8 iRawDataLength = StrLib::iSize(sRow->aChar());
    
    //vTrace("row length %lld\n"_Stz, iRawDataLength);
    
    if (iRawDataLength > C_MAX_RECORD) {
      eEC eErrorCode = EC_MAX_ROW_LENGTH;
      TRACE_ERROR("d"_Stz, eErrorCode);
      vApiReturn(pSession);                                        // API RETURN
      vApiLocation(pSession,910);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eErrorCode;
    }
    
    bool bValid = pJob_InsertRow->bFormatRow(sRow, iRawDataLength);
    
//    s17KB s;
//    i8 iRowLength;
//    bool bBlankLine;
//
//    // format the record into row format
//    bool bValid = Row::bFormatRow(pJob_InsertRow->m_pTable->pColumnVec(),
//                                  &pJob_InsertRow->m_xFormatters,
//                                  sRow, iRawDataLength,
//                                  s, iRowLength, bBlankLine,
//                                  true /* allowNonPrintable */);
    if (! bValid) {
      eEC eErrorCode = EC_INVALID_VALUE;
      TRACE_ERROR("d"_Stz, eErrorCode);
      vApiError(pSession,eErrorCode);                               // API ERROR
      vApiLocation(pSession,811);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eErrorCode;
    }

    //vTrace("row formatted\n"_Stz);
    
    // hash the row to a partition
    pJob_InsertRow->vHashRecord(pJob_InsertRow->m_pTable);
    
    //vTrace("Row partition %lld\n"_Stz, iRowPartition);
    
    // insert the row in this partition's buffer
    eEC eErrorCode = EC_NONE;
    
    while (true) {
      
      // insert row into a working buffer in job InsertRow
      bool bResult = pJob_InsertRow->bInsertRow(pJob_InsertRow->m_pTable, eErrorCode);

      //vTrace("Inserted row into buffer\n"_Stz);
      
      // if inserted then done
      if (bResult)
        break;
      
      // was there an error ?
      if ( eErrorCode != EC_NONE ) {
        vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eErrorCode]);
        TRACE_ERROR("f"_Stz, eErrorCode);
        vApiError(pSession,eErrorCode);                             // API ERROR
        vApiLocation(pSession,812);
        ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
        return eErrorCode;
      }
      
      // buffer was full
      i8 iParentJobId = pJob_InsertRow->m_pJob_InsertTable->m_iJobId;
      RASSERT(C_UNKNOWN != iParentJobId);
      pJob_InsertRow->vWriteBuffer(pSession, pJob_InsertRow->m_sObjectName, DS_INSERT, iParentJobId);
    
      /*
      // create InsertBuffer job
      //vTrace("Buffer full!!! Creating InsertBuffer job\n"_Stz);
      JobClass* pJobClass = JobClass::pFind(CN_JOB_WRITEBUFFER);
      ASSERT(pJobClass);
      Job* pJobInsertBuffer = pJobClass->pPreJob(pSession);
      Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobInsertBuffer);
      
      vApiJob(pSession,pJob_InsertBuffer);
      
      // set table name in InsertBuffer
      //vTrace("Setting tableName %s in InsertBuffer job\n"_Stz, pJob_InsertRow->m_sObjectName.aChar());
      pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
      
      // move the working buffer from insertRow to insertBuffer
      //vTrace("Setting workingBuffer %p in InsertBuffer job\n"_Stz, pJob_InsertRow->m_pWorkingBuffer[iRowPartition]);
      i8 iPartition = pJob_InsertRow->m_iPartition;
      pJob_InsertBuffer->m_iPartitionId = iPartition;
      pJob_InsertBuffer->m_pCurrentBuffer = pJob_InsertRow->m_pWorkingBuffer[iPartition];
      pJob_InsertRow->m_pWorkingBuffer[iPartition] = nullptr;
      
      //vLockMutex();
      //vTrace("Spawning InsertBuffer job\n"_Stz);
      bResult = RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_RUN,pSession);
      ASSERT(bResult);
      */
    }

    // trace exit
    TRACE_EXIT("ea"_Stz);
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_LoadResult(Session* pSession, void* pJobPtr, i8* pValidRows, i8* pInvalidRows, i8* pSkippedRows, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_LoadResult"_Stz;
  eAC  eApiCall = AC_LoadResult;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_LoadResult);                                  // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, pValidRows);
    VALIDATE_REFPOINTER("b"_Stz, pInvalidRows);
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJobPtr);
//    vTrace("L %lld IN %lld\n"_Stz,
//           pJob_LoadTable->m_iValidRows,
//           pJob_LoadTable->m_iInvalidRows);
    
    i8 iRowsProcessed = 0;
    i8 iRowsInvalid   = 0;
    i8 iRowsSkipped   = 0;
    
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      
      //vTrace("%s"_Stz, pJob_LoadTable->m_xNode[i_Node].m_sResponse.aChar());
      iRowsProcessed += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsProcessed;
      iRowsInvalid   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsInvalid;
      iRowsSkipped   += *pJob_LoadTable->m_xNode[i_Node].m_ziRowsSkipped;
    }
    
    *pValidRows   = iRowsProcessed;
    *pInvalidRows = iRowsInvalid;
    *pSkippedRows = iRowsSkipped;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    vApiError(pSession,eError);                                     // API ERROR
    vApiLocation(pSession,800);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    vApiLocation(pSession,801);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(Stz* pInstanceName, void** ppInstance, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_PoolOpen"_Stz;
                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(pInstanceName);
    
    // open the pool
    s256 sPoolName;
    
    sPoolName = (Stz*)pInstanceName;
    bool bResult = RapidsSE::bOpenPool(sPoolName);
    
    if (!bResult) {
      TRACE_ERROR("a"_Stz, EC_INVALID_INSTANCE);
      return EC_INVALID_INSTANCE;
    }
    
    VALIDATE_POOL();
    
    *ppInstance = gpAttachMap;
    
    ActivityMap::vOpenByApi();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    //vApiReturn(pSession);                                          // API RETURN
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    //vApiError(pSession,eError);                                     // API ERROR
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* delete an se session                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SessionDelete(Session* pSession, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_SessionDelete"_Stz;
  eAC  eApiCall = AC_SessionDelete;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);

  vApiCall(pSession,AC_SessionDelete);                               // API CALL
  vApiLocation(pSession,100);

  eEC eResult;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the session
    VALIDATE_SESSION("a"_Stz);
    
    // delete jobs from teh job stack of the session
    vDeleteJobs(pSession);
    
    vApiReturn(pSession);                                          // API RETURN
    vApiLocation(pSession,900);

    // delete the session
    RapidsSE::vDeleteSession(pSession);
    
    // set session to nullptr so trace knows not to look at it
    
    pSession = nullptr;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    eEC eError = pError->eErrorCode();
    //vApiError(pSession,eError);                                     // API ERROR
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return eError;
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    //vApiError(pSession,EC_SE_UNKNOWN_ERROR);                        // API ERROR
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_SessionE ?                                      */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SessionInvalid(int iError, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_SessionInvalid"_Stz;
  eAC  eApiCall = AC_SessionInvalid;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
                                                                      // API ???
  Session* pSession = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    if (iError == EC_INVALID_INTERFACE) {
      // trace exit
      TRACE_EXIT("ea"_Stz);

      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return true;
    }
    else {
      // trace exit
      TRACE_EXIT("eb"_Stz);

      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return false;
    }
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* create a new se session                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SessionNew(void** ppSession, Stz* sCatalogName, Stz* sSchemaName, i8 iPartition, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_SessionNew"_Stz;
  eAC  eApiCall = AC_SessionNew;
  ActivityMap::vMarkApi(eApiCall,0,">Api %s"_Stz,aApiCall);
                                                                      // API ???
  eEC eResult;
  Session* pSession = nullptr;
  
  try {
    
    TRACE_ENTRY_P0();
    
    // trace entry
    TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    //TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppSession);
    
    VALIDATE_POOL();
    
    Session* pSession = new Session(iPartition);
    
    eEC eError = pSession->eInit(sCatalogName, sSchemaName);
    
    if (eError != EC_NONE) {
      TRACE_EXIT("ea"_Stz);
      ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
      return eError;
    }
    
    // validate the session just created
    VALIDATE_SESSION("b"_Stz);
    
    // return the newly created session
    *ppSession = pSession;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);

    ActivityMap::vMarkApi(eApiCall,0,"<Api %s"_Stz,aApiCall);
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    ActivityMap::vMarkApi(eApiCall,0,"!Api %s"_Stz,aApiCall);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, Session* pSession)
{
  //----------------------------------------------------------------------------
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSession)
      vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    return false;
  }
  
  //----------------------------------------------------------------------------
  
  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  Task* pNewTask = new Task(pJob);
  
  pNewTask->vSession(pSession);
  
  Handlers::vSpawnedTaskHandler(pNewTask);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* mark call of an api                                                        */
/*----------------------------------------------------------------------------*/
void vApiCall(Session* pSession, eAC eApiCall)
{
  gpAttachMap->m_pApiState->vApiCall(pSession,eApiCall);
}

/*----------------------------------------------------------------------------*/
/* mark error of an api                                                       */
/*----------------------------------------------------------------------------*/
void vApiError(Session* pSession, eEC eError)
{
  gpAttachMap->m_pApiState->vApiError(pSession,eError);
}

/*----------------------------------------------------------------------------*/
/* mark job of an api                                                         */
/*----------------------------------------------------------------------------*/
void vApiJob(Session* pSession, Job* pJob)
{
  gpAttachMap->m_pApiState->vApiJob(pSession,pJob);
}

/*----------------------------------------------------------------------------*/
/* mark location of an api                                                    */
/*----------------------------------------------------------------------------*/
void vApiLocation(Session* pSession, i8 iLocation)
{
  gpAttachMap->m_pApiState->vSetLocation(pSession,iLocation);
}

/*----------------------------------------------------------------------------*/
/* mark return of an api                                                      */
/*----------------------------------------------------------------------------*/
void vApiReturn(Session* pSession)
{
  gpAttachMap->m_pApiState->vApiReturn(pSession);
}

/*----------------------------------------------------------------------------*/
/* delete jobs                                                                */
/*----------------------------------------------------------------------------*/
void vDeleteJobs(Session* pSession)
{
  Stack* pJobStack = pSession->m_pJobStack;
  Object* pJobObject;

  while (pJobStack->bPop(pJobObject)) {
    
    Job* pJobPtr = (Job*)pJobObject;
    
    // find the job class on this node
    eCN eClass = pJobPtr->eClass();
    JobClass* pJobClass = JobClass::pFind(eClass);
    
    vApiLocation(pSession,200);
    
    //vTrace("API deleting job %p jobId %lld %s\n"_Stz, pJobPtr, pJobPtr->m_iJobId, aCN[eClass]);
    
    // If we come across a JOB_FETCHROWS then we need to empty its queues
    if (eClass == CN_JOB_FETCHROWS) {
      Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
  
      //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz,pJob_FetchRows->m_pTable->aTableName(),
      //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
      
      // if we had an entry buffer delete it
      pJob_FetchRows->vDeleteBuffer();
      
      while (pJob_FetchRows->m_nLastBuffersScanned != pJob_FetchRows->m_nLastBuffers) {
        // pop an entryBuffer off the queue
        vApiLocation(pSession,300);
        ValWrk xValWrk;
        while (!pJob_FetchRows->m_pBufferQueue->bPop(&xValWrk)) {
          PosixThread::vSleepMilli(1);
          vApiLocation(pSession,400);
          continue;
        }
        
        vApiLocation(pSession,500);
        
        PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
        pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(pPtrVal->pPtr());
        
        if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
          pJob_FetchRows->m_nLastBuffersScanned++;
          //vTrace("%s m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_pTable->aTableName(),
          //       pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
        }
        
        // delete the buffer
        pJob_FetchRows->vDeleteBuffer();
      }
    }
    vApiLocation(pSession,600);
    
    // delete the job
    vTrace("Deleting job %p %s\n"_Stz, pJobPtr, aCN[pJobPtr->eClass()]);
    pJobClass->vDeleteJob(pSession,pJobPtr);
    
    pSession->m_nJobDelete++;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
