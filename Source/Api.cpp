/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;


/*----------------------------------------------------------------------------*/
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(Stz* pVersion, i8 iTraceLevel)
{
  SEitf* pSEitf = nullptr;
  
  Stz* aApiCall = "RapidsSE_CheckLibraryVersion"_Stz;
  
  try {
    TRACE_ENTRY_P1(pVersion);
    
    // validate the parameter
    VALIDATE_PARAM("a"_Stz, pVersion);
    
    if ( ! pVersion->bEqual((Stz*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Stz,
          pVersion, RAPIDSSE_VERSION_STRING);
      
      TRACE_ERROR("b"_Stz, EC_VERSION_MISMATCH);
      return EC_VERSION_MISMATCH;
    }
    
    TRACE_EXIT("ea"_Stz);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsert(SEitf* pSEitf, void* pJobPtr, i8 iTraceLevel)
{
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_EndInsert"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // validate the job
    VALIDATE_JOB("b"_Stz);
    
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJobPtr);
    
    //pJob_InsertRow->m_pWorkingBuffer[0]->vShow(true);
    
    // create job_EndInsert
    JobClass* pJobClass_EndInsert = JobClass::pFind(CN_JOB_ENDINSERT);
    ASSERT(pJobClass_EndInsert)
    Job_EndInsert* pJob_EndInsert =
      static_cast<Job_EndInsert*>(pJobClass_EndInsert->pPreJob(pSEitf));
    
    // set insertRowJobId in the endInsert job
    pJob_EndInsert->m_iInsertRowJobId = pJob_InsertRow->m_iJobId;
    
    // lock this job's mutex
    i8 rCode = pthread_mutex_lock(&pJob_EndInsert->m_xThreadMutex);
    if (0 != rCode)
      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
    
    // spawn the endInsert job
    RapidsSE_bSpawnJob(pJob_EndInsert, SS_SEED_REQ, pSEitf);
    
    // wait for condition
    rCode = pthread_cond_wait(&pJob_EndInsert->m_xThreadCond,
                              &pJob_EndInsert->m_xThreadMutex);
    if (0 != rCode)
      vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
    
    TRACE_EXIT("ea"_Stz);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCommand(SEitf* pSEitf, void* pJobPtr, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_EndCommand"_Stz;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  // validate the job
  VALIDATE_JOB("a"_Stz);
  
  Job* pJob = (Job*)(pJobPtr);
  
  // find the job class on this node
  eCN eClass = pJob->eClass();
  JobClass* pJobClass = JobClass::pFind(eClass);
  
  // delete the job
  pJobClass->vDeleteJob(pSEitf,pJob);
  
  // null the job in the SEitf
  pSEitf->m_pJob = nullptr;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteCommand(SEitf* pSEitf, Stz* aCommand,
                            Stz* sCatalogName, Stz* sSchemaName,
                            bool bPostJob, void** ppJob, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_ExecuteCommand"_Stz;
  
  // trace entry
  TRACE_ENTRY_P1(aCommand);
  
  if (pSEitf)
    vTrace("SEitf: %p\n"_Stz, pSEitf);
  
  if (sCatalogName)
    vTrace("Catalog: %s\n"_Stz, sCatalogName);
  
  if (sSchemaName)
    vTrace("Schema: %s\n"_Stz, sSchemaName);
  
  vTrace("bPostJob: %lld\n"_Stz, bPostJob);
  vTrace("ppJob: %p\n"_Stz, ppJob);
  
  
  // set the catalog and schema in the SEitf
  vTrace("Intializing SEITF\n"_Stz);
  pSEitf->vInit(sCatalogName, sSchemaName);
  
  vTrace("Invoking RapidsSE_pExecuteCommand\n"_Stz);
  Job* pJob = RapidsSE_pExecuteCommand(aCommand, pSEitf, bPostJob);
  //vTrace("pJob = %p\n"_Stz, pJob);
  *ppJob = pJob;
  pSEitf->m_pJob = pJob;
  
  if (pJob->m_bHasError) {
    vTrace("Got an error\n"_Stz);
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      if (pJob->m_xNode[i_Node].m_eErrorCode != EC_NONE) {
        vTrace("ERROR*** (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob->m_xNode[i_Node].m_eErrorCode]);
        return pJob->m_xNode[i_Node].m_eErrorCode;
      }
    }
    vTrace("Hit ASSERT\n"_Stz);
    ASSERT(0);
  }
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextBuffer(SEitf* pSEitf, void* pJobPtr, void** ppFetchBuffer, i8 iTraceLevel)
{
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_FetchNextBuffer"_Stz;
  
  try {
           
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // Validate job
    VALIDATE_JOB("b"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("c"_Stz, ppFetchBuffer);
    
    Job_FetchRows* pJob_FetchRows = _pJob_FetchRows(pJobPtr);
    
    // if we had an entry buffer delete it
    pJob_FetchRows->vDeleteBuffer();
    
    if (pJob_FetchRows->m_nLastBuffersScanned == pJob_FetchRows->m_nLastBuffers) {
      *ppFetchBuffer = nullptr;
  
      // trace exit
      TRACE_EXIT("ea"_Stz);
      return EC_NONE;
    }
    
    // pop an entryBuffer off the queue
    ValWrk xValWrk;
    while (!pJob_FetchRows->m_pBufferQueue->bPop(&xValWrk)) {
      PosixThread::vSleepMilli(1);
    }
    
    ASSERT(pSEitf->m_pJob == pJob_FetchRows);
    
    // get entry buffer
    PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
    pJob_FetchRows->m_pFetchBuffer = (FetchBuffer*)(pPtrVal->pPtr());
    
    
    if (pJob_FetchRows->m_pFetchBuffer->bIsLast()) {
      pJob_FetchRows->m_nLastBuffersScanned++;
      //vTrace("m_nLastBuffersScanned: %lld %lld\n"_Stz, pJob_FetchRows->m_nLastBuffersScanned, pJob_FetchRows->m_nLastBuffers );
    }
    
    *ppFetchBuffer = pJob_FetchRows->m_pFetchBuffer;
    
    // trace exit
    TRACE_EXIT("eb"_Stz);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorMessage(SEitf* pSEitf, int iErrorCode, void** ppError, i8 iTraceLevel)
{
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_GetErrorMessage"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppError);
    
    // get the error message
    pSEitf->m_sResponse = aEC[(eEC)iErrorCode];
    *ppError = pSEitf->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRow(SEitf* pSEitf, void* pJobPtr, i8 iRow, void** ppRow, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRow"_Stz;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  Job* pJob = (Job*)pJobPtr;
  
  TokenVec* pTknVec = Tokeniser::pTokenise(pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_sResponse);
  
  i8 iRows = -1;
  i8 iToken = 0;
  
  while (iRows != iRow) {
    ASSERT(pTknVec->pGet(iToken++)->aText()->bSame("row:"_Stz));
    
    ASSERT(pTknVec->pGet(iToken++)->aText()->bSame("("_Stz));
    
    pSEitf->m_sResponse.vCopy(""_Stz);
    while ( ! pTknVec->pGet(iToken)->aText()->bSame(")"_Stz)) {
      pSEitf->m_sResponse.vConcat(pTknVec->pGet(iToken++)->aText());
    }
    
    //vTrace("row found %s\n"_Stz, pSEitf->m_sResponse.aChar());
    ASSERT(pTknVec->pGet(iToken++)->aText()->bSame(")"_Stz));
    iRows++;
  }
  *ppRow = pSEitf->m_sResponse.aChar();
  
  pTknVec->vSetAllTokens();
  delete pTknVec;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetRowCount(SEitf* pSEitf, void* pJobPtr, void** ppRowCount, i8 iTraceLevel)
{
  Stz* aApiCall = "RapidsSE_GetRowCount"_Stz;
  
  // trace entry
  TRACE_ENTRY_P0();
  
  Job* pJob = (Job*)pJobPtr;
  
  *ppRowCount = &(pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  //vTrace("RowCount = %lld\n"_Stz, pJob->m_xNode[gpAttachMap->m_pGrid->iThisNode()].m_iRowsProcessed);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, void* pJob, Stz* sRow, i8 iTraceLevel)
{

  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_InsertRow"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);

    // validate parameter
    VALIDATE_PARAM("b"_Stz, sRow);
    
    // validate table
    //VALIDATE_TABLE("c"_Stz);
    
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
 
    i8 iRawDataLength = StrLib::iSize(sRow->aChar());
    
    //vTrace("row length %lld\n"_Stz, iRawDataLength);
    
    if (iRawDataLength > C_16KB) {
      eEC eErrorCode = EC_MAX_ROW_LENGTH;
      TRACE_ERROR("d"_Stz, eErrorCode);
      return eErrorCode;
    }
    
    s17KB s;
    i8 iRowLength;
    bool bBlankLine;
    
    // format the record into row format
    bool bValid = Row::bFormatRow(pJob_InsertRow->m_pTable->pColumnVec(),
                                  &pJob_InsertRow->m_xFormatters,
                                  sRow, iRawDataLength,
                                  s, iRowLength, bBlankLine,
                                  true /* allowNonPrintable */);
    if (! bValid) {
      eEC eErrorCode = EC_INVALID_VALUE;
      TRACE_ERROR("d"_Stz, eErrorCode);
      return eErrorCode;
    }

    //vTrace("row formatted\n"_Stz);
    
    // get the partition this row hashes to
    i8 iRowPartition = pJob_InsertRow->m_pTable->iRowPartition(s);
    
    vTrace("Row partition %lld\n"_Stz, iRowPartition);
    
    // insert the row in this partition's buffer
    eEC eErrorCode = EC_NONE;
    
    while (true) {
      
      // insert row into a working buffer in job InsertRow
      bool bResult = pJob_InsertRow->bInsertRow(s, iRowPartition, iRowLength, eErrorCode);

      //vTrace("Inserted row into buffer\n"_Stz);
      
      // if inserted then done
      if (bResult)
        break;
      
      // was there an error ?
      if ( eErrorCode != EC_NONE ) {
        vTrace("Inserting row into buffer got error %s\n"_Stz, aEC[eErrorCode]);
        TRACE_ERROR("f"_Stz, eErrorCode);
        return eErrorCode;
      }
      
      // buffer was full
      // create InsertBuffer job
      vTrace("Buffer full!!! Creating InsertBuffer job\n"_Stz);
      JobClass* pJobClass = JobClass::pFind(CN_JOB_INSERTBUFFER);
      ASSERT(pJobClass);
      Job* pJobInsertBuffer = pJobClass->pPreJob(pSEitf);
      Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobInsertBuffer);
      
      // set table name in InsertBuffer
      //vTrace("Setting tableName %s in InsertBuffer job\n"_Stz, pJob_InsertRow->m_sObjectName.aChar());
      pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
      
      // move the working buffer from insertRow to insertBuffer
      //vTrace("Setting workingBuffer %p in InsertBuffer job\n"_Stz, pJob_InsertRow->m_pWorkingBuffer[iRowPartition]);
      pJob_InsertBuffer->m_iPartitionId = iRowPartition;
      pJob_InsertBuffer->m_pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iRowPartition];
      pJob_InsertRow->m_pWorkingBuffer[iRowPartition] = nullptr;
      
      //vLockMutex();
      vTrace("Spawning InsertBuffer job\n"_Stz);
      bResult = RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_RUN,pSEitf);
      ASSERT(bResult);
    }

    // trace exit
    //TRACE_EXIT("ea"_Stz);

    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_LoadResult(SEitf* pSEitf, void* pJob, i8* pValidRows, i8* pInvalidRows, i8 iTraceLevel)
{
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_LoadResult"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, pValidRows);
    VALIDATE_REFPOINTER("b"_Stz, pInvalidRows);
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);
    vTrace("L %lld IN %lld\n"_Stz, *pJob_LoadTable->m_ziValidRows, *pJob_LoadTable->m_ziInvalidRows);
    
    *pValidRows = *pJob_LoadTable->m_ziValidRows;
    *pInvalidRows = *pJob_LoadTable->m_ziInvalidRows;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(Stz* pInstanceName, void** ppInstance, i8 iTraceLevel)
{
  
  Stz* aApiCall = "RapidsSE_PoolOpen"_Stz;
  SEitf* pSEitf = nullptr;
  
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
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfDelete(SEitf* pSEitf, i8 iTraceLevel)
{
  eEC eResult;
  Stz* aApiCall = "RapidsSE_SEitfDelete"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    Job* pJob = pSEitf->m_pJob;
    
    if (pJob) {
      // find the job class on this node
      eCN eClass = pJob->eClass();
      JobClass* pJobClass = JobClass::pFind(eClass);
      
      // delete the job
      pJobClass->vDeleteJob(pSEitf,pJob);
      
      // null the job in the SEitf
      pSEitf->m_pJob = nullptr;
    }
    
    // delete the SEitf
    RapidsSE::vDeleteSEitf(pSEitf);
    
    // set SEitf to nullptr so trace knows not to look at it
    pSEitf = nullptr;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SEitfInvalid(int iError, i8 iTraceLevel)
{
  
  Stz* aApiCall = "RapidsSE_SEitfInvalid"_Stz;
  SEitf* pSEitf = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    if (iError == EC_INVALID_INTERFACE) {
      // trace exit
      TRACE_EXIT("ea"_Stz);

      return true;
    }
    else {
      // trace exit
      TRACE_EXIT("eb"_Stz);

      return false;
    }
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void** ppSEitf, Stz* sCatalogName, Stz* sSchemaName, i8 iPartition, i8 iTraceLevel)
{
  eEC eResult;
  Stz* aApiCall = "RapidsSE_SEitfNew"_Stz;
  SEitf* pSEitf = nullptr;
  
  try {
    
    TRACE_ENTRY_P0();
    
    // trace entry
    TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    //TRACE_ENTRY_P2(sCatalogName, sSchemaName);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppSEitf);
    
    VALIDATE_POOL();
    
    SEitf* pSEitf = new SEitf(iPartition);
    
    pSEitf->vInit(sCatalogName, sSchemaName);
    
    // validate the SEitf just created
    VALIDATE_SEITF("b"_Stz);
    
    // return the newly created SEitf
    *ppSEitf = pSEitf;
    
    // trace exit
    TRACE_EXIT("ea"_Stz);

    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vErrorHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* RapidsSE_pExecuteCommand(Stz* aCommand, SEitf* pSEitf, bool bPostJob)
{
  CommandProcessor* pCommandProcessor = pSEitf->m_pCommandProcessor;
  
  vTrace("Tokenizing command\n"_Stz);
  pCommandProcessor->vTokenise(aCommand);
  
  vTrace("Looking for jobClass\n"_Stz);
  bool bFound = pCommandProcessor->bFindJobClass();
  if (! bFound) {
    vTrace("No jobClass\n"_Stz);
    return nullptr;
  }
  
  vTrace("Executing pre\n"_Stz);
  pCommandProcessor->vPreJob(pSEitf);
  
  Job* pJob = pCommandProcessor->pJob();
  if (!pJob) {
    vTrace("No job\n"_Stz);
    return nullptr;
  }
  
  // lock this job's mutex
  vTrace("Getting mutex lock\n"_Stz);
  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);

  // spawn the job
  vTrace("Spawning job\n"_Stz);
  RapidsSE_bSpawnJob(pJob, SS_SEED_REQ, pSEitf);
  
  // wait for condition
  rCode = pthread_cond_wait(&pJob->m_xThreadCond,
                            &pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
  
  vTrace("Returned from job\n"_Stz);
  
  if (bPostJob)
    pCommandProcessor->vPostJob(pSEitf);
  
  return pJob;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, SEitf* pSEitf)
{
  //----------------------------------------------------------------------------
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    if (pSEitf)
      vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    vError("Cannot find pid %lld\n"_Stz, gpAttachMap->m_iPid);
    return false;
  }
  
  //----------------------------------------------------------------------------
  
  // form the word to execute for the request
  pJob->vSeed(eSeed);
  
  Task* pNewTask = new Task(pJob);
  
  pNewTask->vSEitf(pSEitf);
  
  Handlers::vSpawnedTaskHandler(pNewTask);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
