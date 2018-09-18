/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_BeginInsert(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_BeginInsert"_Stz;
  
  try {
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
  
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // set the enclosined_by to be a '
    pSEitf->m_xFormatters.m_cEnclosedBy_Leading = '\'';
    pSEitf->m_xFormatters.m_cEnclosedBy_Trailing = '\'';
    
    // change directory to tables
    CF_TABLES("e"_Stz);
    
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // validate we have a table
    VALIDATE_TABLE("h"_Stz);
    
    // begin a tnx
    eResult = eExecuteRequest(pSEitf, "txBeg"_Stz);
    CHECK_RESULT("i"_Stz);
    CHECK_RESPONSE("j"_Stz);
    
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
*/
/*----------------------------------------------------------------------------*/
/* begin insert on SEitf partiton                                             */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_BeginInsertPartition(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_BeginInsertPartition"_Stz;
  
  try {
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change directory to tables
    CF_TABLES("e"_Stz);
    
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // validate we have a table
    VALIDATE_TABLE("h"_Stz);
    
    // begin a tnx
    bool bResult = pSEitf->bBeginTx();
    
    if (!bResult) {
      pSEitf->vSetLastError(EC_TX_BEGIN_FAILED);
      TRACE_ERROR("i",EC_TX_BEGIN_FAILED);
      return EC_TX_BEGIN_FAILED;
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
*/
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
/*
eEC RapidsSE_EndInsert(SEitf* pSEitf, void* pJob, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_EndInsert"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
        
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
    
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
*/
/*----------------------------------------------------------------------------*/
/* end insert on SEitf partition                                              */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_EndInsertPartition(SEitf* pSEitf, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_EndInsertPartition"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate we have a table
    VALIDATE_TABLE("b"_Stz);
    
    bool bResult = pSEitf->bInsertBuffer();
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      CHECK_RESPONSE_RETURNTABLE("c"_Stz);
    }
    
    eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
    CHECK_RESULT("d"_Stz);
    CHECK_RESPONSE("e"_Stz);
    
    // validate if no table
    VALIDATE_NOTABLE("f"_Stz);
    
    pSEitf->vCommitTx();
    pSEitf->vEndTx();
    
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
*/
/*----------------------------------------------------------------------------*/
/* export data from table into the file                                       */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_ExportData(SEitf* pSEitf, Stz* pTable, Stz* pFilePath, i8 iTraceLevel)
{
  
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_ExportData"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P2(pTable, pFilePath);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pTable);
    VALIDATE_PARAM("c"_Stz, pFilePath);
    
    // add a column
    eResult = eExecuteRequest(pSEitf, "dex"_Stz,
                              pTable,
                              pFilePath);
    
    CHECK_RESULT("d"_Stz);
    CHECK_RESPONSE("e"_Stz);
    
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
*/
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
    
    // clear the last error
    CLEAR_LASTERROR();
    
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
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_LoadResult(SEitf* pSEitf, void* pJob, void** ppValidRows, void** ppInvalidRows, i8 iTraceLevel)
{
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_LoadResult"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, ppValidRows);
    VALIDATE_REFPOINTER("b"_Stz, ppInvalidRows);
    
    Job_LoadTable* pJob_LoadTable = _pJob_LoadTable(pJob);
    *ppValidRows = &pJob_LoadTable->m_iValidRows;
    *ppInvalidRows = &pJob_LoadTable->m_iInvalidRows;
    
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
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorMessage(SEitf* pSEitf, void** ppError, i8 iTraceLevel)
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
    *ppError = pSEitf->m_sErrorMsg.aChar();
    
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
/* return rapidsSE engine's version                                           */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetEngineVersion"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppVersion);
    
    // get the SE version
    eResult = eExecuteRequest(pSEitf, "version"_Stz);
    CHECK_RESULT("c"_Stz);
    CHECK_RESPONSE("d"_Stz);
    
    // return SE version
    *ppVersion = pSEitf->m_sResponse.aChar();
    
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
*/
/*----------------------------------------------------------------------------*/
/* hash the row to a partition                                                */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_HashRowPartition(SEitf* pSEitf, Stz* pRow, void** ppPartition, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_HashRowPartition"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();

    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppPartition);

    // validate parameters
    VALIDATE_PARAM("c"_Stz, pRow);
    
    // validate table
    VALIDATE_TABLE("d"_Stz);
    pSEitf->m_iRowPartition = pSEitf->pCurrentTable()->iRowPartition(pRow);

    *ppPartition = &pSEitf->m_iRowPartition;
    
    //TRACE_RESULT(*ppPartition);
    
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
*/
/*----------------------------------------------------------------------------*/
/* hash the integer value to a node                                           */
/*----------------------------------------------------------------------------*/
/*
i8 RapidsSE_HashIntegerNode(i8 iInt)
{
  // get no of nodes in the grid
  i8 iNodes = gpAttachMap->m_pGrid->nNodes();
  
  // get the hash of the integer
  i8 iHash = XXH32(&iInt,sizeof(i8),0);
  
  // calculate the partition this integer hashes to
  i8 iPartition = iHash % (gpAttachMap->m_pGrid->m_nSlots * iNodes);

  // now figure out the node this partition belongs to
  i8 iNode = iPartition % iNodes;
  
  return iNode;
}
*/
/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, void* pJob, Stz* sRow, i8 iTraceLevel)
{

  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_InsertRow"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);

    // clear the last error
    CLEAR_LASTERROR();

    // validate parameter
    VALIDATE_PARAM("b"_Stz, sRow);
    
    // validate table
    //VALIDATE_TABLE("c"_Stz);
    
    Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(pJob);
    
    i8 iRawDataLength = StrLib::iSize(sRow->aChar());
    
    if (iRawDataLength > C_16KB) {
      eEC eErrorCode = EC_MAX_ROW_LENGTH;
      TRACE_ERROR("d"_Stz, eErrorCode);
      pSEitf->m_sErrorMsg =  aEC[eErrorCode];
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
      pSEitf->m_sErrorMsg =  aEC[eErrorCode];
      return eErrorCode;
    }

    // get the partition this row hashes to
    i8 iRowPartition = pJob_InsertRow->m_pTable->iRowPartition(s);
    
    // insert the row in this partition's buffer
    eEC eErrorCode;
    
    while (true) {
      
      // insert row into a working buffer in job InsertRow
      bool bResult = pJob_InsertRow->bInsertRow(s, iRowPartition, iRowLength, eErrorCode);

      // if inserted then done
      if (bResult)
        break;
      
      // was there an error ?
      if ( eErrorCode != EC_NONE ) {
        pSEitf->m_sErrorMsg =  aEC[eErrorCode];
        TRACE_ERROR("f"_Stz, eErrorCode);
        return eErrorCode;
      }
      
      // buffer was full
      // create InsertBuffer job
      JobClass* pJobClass = JobClass::pFind(CN_JOB_INSERTBUFFER);
      ASSERT(pJobClass);
      Job* pJobInsertBuffer = pJobClass->pPreJob(pSEitf);
      Job_InsertBuffer* pJob_InsertBuffer = _pJob_InsertBuffer(pJobInsertBuffer);
      
      // set table name in InsertBuffer
      pJob_InsertBuffer->m_sObjectName = pJob_InsertRow->m_sObjectName;
      
      // move the working buffer from insertRow to insertBuffer
      pJob_InsertBuffer->m_pWorkingBuffer = pJob_InsertRow->m_pWorkingBuffer[iRowPartition];
      pJob_InsertRow->m_pWorkingBuffer[iRowPartition] = nullptr;
      
      //vLockMutex();
      bResult = RapidsSE_bSpawnJob(pJob_InsertBuffer,SS_SEED_REQ,pSEitf);
      ASSERT(bResult);
    }

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
/* insert the given row into a table in SEitf partition                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRowPartition(SEitf* pSEitf, Stz* pRow, i8 iTraceLevel)
{
  
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_InsertRowPartition"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameter
    VALIDATE_PARAM("b"_Stz, pRow);
    
    // validate table
    VALIDATE_TABLE("c"_Stz);
    
    s17KB sRecord;
    i8 iRowLength;
    bool bBlankLine;
    
    i8 iRawDataLength = StrLib::iSize(pRow->aChar());
    
    if (iRawDataLength > C_16KB) {
      pSEitf->vSetLastError(EC_MAX_ROW_LENGTH);
      TRACE_ERROR("d"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }

    // format the record into row format
    bool bValid = Row::bFormatRow(pSEitf->m_pCurrentTable->pColumnVec(),
                                  &pSEitf->m_xFormatters,
                                  pRow, iRawDataLength,
                                  sRecord, iRowLength, bBlankLine,
                                  true /* allowNonPrintable */);
    if (! bValid) {
      pSEitf->vSetLastError(EC_INVALID_VALUE);
      TRACE_ERROR("d"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
    
    // get the partition this row hashes to
    i8 iRowPartition = pSEitf->pCurrentTable()->iRowPartition(sRecord);
    
    //vTrace("%p(%lld) got row %s\n"_Stz,  
    //       pSEitf, pSEitf ? pSEitf->iPartition() : -1, pRow);
    
    // is this row for this seitf partition ?
    if (pSEitf->m_iPartition == iRowPartition) {
      
      //RowData::vShow(s, pSEitf->m_pCurrentTable->pColumnVec());
      TRACE_INSERT("e"_Stz, pRow);
      
      // insert the row in this partition
      bool bResult = pSEitf->bInsertRow(sRecord, iRowLength);
      
      if (!bResult) {
        pSEitf->vSetLastError(pSEitf->m_eErrorCode);
        TRACE_ERROR("f"_Stz, pSEitf->m_eErrorCode);
        return pSEitf->m_eErrorCode;
      }
      
    }
    
    // validate table
    VALIDATE_TABLE("f"_Stz);
    
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
/* load data from file into the table                                         */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_LoadData(SEitf* pSEitf, Stz* pTable, Stz* pFilePath,
                      Stz* pRecordTerminator, Stz* pRecordEnds, Stz* pFieldDelimiter,
                      Stz* pEnclosedByLeading, Stz* pEnclosedByTrailing,
                      Stz* pEscapeChar, Stz* pDateFormat,
                      void** ppValidRows, void** ppInvalidRows, i8 iTraceLevel)
{
  
  eEC eResult;
  
  Stz* aApiCall = "RapidsSE_LoadData"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P2(pTable, pFilePath);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pTable);
    VALIDATE_PARAM("c"_Stz, pFilePath);
    
//    vTrace("rt = %s\n"_Stz, pRecordTerminator->aChar());
//    vTrace("re = %s\n"_Stz, pRecordEnds);
//    vTrace("fs = %s\n"_Stz, pFieldSeparator->aChar());
//    vTrace("ebl = %s\n"_Stz, pEnclosedByLeading->aChar());
//    vTrace("ebt = %s\n"_Stz, pEnclosedByLeading->aChar());
//    vTrace("ec = %s\n"_Stz, pEscapeChar->aChar());
    
    // set the formatters
    pSEitf->m_xFormatters.m_cRecordTerminator = *(pRecordTerminator->aChar());
    
    // extract the record ends
    if (pRecordEnds->iLength() > 0) {
      if (pRecordEnds->bSame("\\n"_Stz)) {
        pSEitf->m_xFormatters.m_eRecordEnds = RE_NL;
      }
      else if (pRecordEnds->bSame("\\r"_Stz)) {
        pSEitf->m_xFormatters.m_eRecordEnds = RE_CR;
      }
      else if (pRecordEnds->bSame("\\r\\n"_Stz)) {
        pSEitf->m_xFormatters.m_eRecordEnds = RE_CRNL;
      }
      else {
        pSEitf->vSetLastError(EC_INVALID_COMMAND);
        TRACE_ERROR("d",EC_INVALID_COMMAND);
        return EC_INVALID_COMMAND;
      }
    }
    
    // field delimiter
    pSEitf->m_xFormatters.m_cFieldDelimiter   = *(pFieldDelimiter->aChar());
    
    if (pFieldDelimiter->bEqual("\\t"_Stz))
      pSEitf->m_xFormatters.m_cFieldDelimiter   = '\t';
    
    pSEitf->m_xFormatters.m_cEnclosedBy_Leading  = *(pEnclosedByLeading->aChar());
    pSEitf->m_xFormatters.m_cEnclosedBy_Trailing = *(pEnclosedByTrailing->aChar());
    
    pSEitf->m_xFormatters.m_cEscapeChar          = *(pEscapeChar->aChar());
    
    // validate and extract the date format componenets
    if ( pDateFormat->iLength() > 0) {

      if ( ! pSEitf->bExtractDateFormat(pDateFormat) ) {
        pSEitf->vSetLastError(EC_INVALID_COMMAND);
        TRACE_ERROR("e",EC_INVALID_COMMAND);
        return EC_INVALID_COMMAND;
      }
    }
    
    // add a column
    eResult = eExecuteRequest(pSEitf, "ld"_Stz,
                              pTable,
                              pFilePath);
    
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return the vlaid/invalid rows
    *ppInvalidRows = &pSEitf->m_iRowsInvalid;
    *ppValidRows = &pSEitf->m_iRowsValid;
    
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
*/
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
/* launch a prompt job                                                        */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SpawnPrompt(i8 iTraceLevel)
{
  Stz*   aApiCall = "RapidsSE_SpawnPrompt"_Stz;
  SEitf* pSEitf = nullptr;
  s256   sRequest;

  try {
    
    eEC eError = RapidsSE_SEitfNew((void**)&pSEitf, "rapidsse"_Stz, "public"_Stz, 0, 0);
    ASSERT(EC_NONE == eError);
    
    JobClass* pJobClass_Prompt = JobClass::pFind(CN_JOB_PROMPT);
    
    Job_Prompt* pJob_Prompt = static_cast<Job_Prompt*>(pJobClass_Prompt->pNewJob(nullptr));
    
    pJob_Prompt->m_pSEitf = pSEitf;
    
    bool b = RapidsSE_bSpawnJob(pJob_Prompt,SS_SEED_RUN,pSEitf);
    
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
    
    // clear the last error
    CLEAR_LASTERROR();
    
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
/* get response                                                               */
/*----------------------------------------------------------------------------*/
eEC eGetResponse(SEitf* pSEitf)
{
  // get response
  eEC eError = pSEitf->eGetResponse();
  //vTrace("rsp : %s %lld\n"_Stz, pSEitf->m_sResponse.aChar(), eError);
  
  return eError;
}

/*----------------------------------------------------------------------------*/
/* spawn job                                                                  */
/*----------------------------------------------------------------------------*/
bool RapidsSE_bSpawnJob(Job* pJob, eSS eSeed, SEitf* pSEitf)
{
  
  // clear the last error
  if (pSEitf)
    pSEitf->vClearLastError();

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
  TRACE_ENTRY_P0();
  
  //vTrace("Executing command: %s\n"_Stz, aCommand);
  
  // set the catalog and schema in the SEitf
  pSEitf->vInit(sCatalogName, sSchemaName);
  
  Job* pJob = RapidsSE_pExecuteCommand(aCommand, pSEitf, bPostJob);
  //vTrace("pJob = %p\n"_Stz, pJob);
  *ppJob = pJob;
  pSEitf->m_pJob = pJob;
  
  if (pJob->m_bHasError) {
    for (i8 i_Node = 0; i_Node < gpAttachMap->m_pGrid->m_nNodes; i_Node++) {
      if (pJob->m_xNode[i_Node].m_eErrorCode != EC_NONE) {
        vTrace("ERROR*** (NODE%lld): %s\n"_Stz, i_Node, aEC[pJob->m_xNode[i_Node].m_eErrorCode]);
        pSEitf->m_sErrorMsg = aEC[pJob->m_xNode[i_Node].m_eErrorCode];
        return pJob->m_xNode[i_Node].m_eErrorCode;
      }
    }
    ASSERT(0);
  }
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
Job* RapidsSE_pExecuteCommand(Stz* aCommand, SEitf* pSEitf, bool bPostJob)
{
  CommandProcessor* pCommandProcessor = pSEitf->m_pCommandProcessor;
  
  pCommandProcessor->vTokenise(aCommand);
  
  bool bFound = pCommandProcessor->bFindJobClass();
  if (! bFound)
    return nullptr;
  
  pCommandProcessor->vPreJob(pSEitf);
  
  Job* pJob = pCommandProcessor->pJob();
  if (!pJob)
    return nullptr;
  
  // lock this job's mutex
  i8 rCode = pthread_mutex_lock(&pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);

  // spawn the job
  RapidsSE_bSpawnJob(pJob, SS_SEED_REQ, pSEitf);
  
  // wait for condition
  rCode = pthread_cond_wait(&pJob->m_xThreadCond,
                            &pJob->m_xThreadMutex);
  if (0 != rCode)
    vWarn("pthread_cond_wait() error; rCode=%lld\n"_Stz,rCode);
  
  if (bPostJob)
    pCommandProcessor->vPostJob(pSEitf);
  
  return pJob;
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
