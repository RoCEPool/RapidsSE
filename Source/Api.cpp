/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;
extern Grid*       gpGrid;

/*----------------------------------------------------------------------------*/
/* add a column to an index                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddIndexColumn(SEitf* pSEitf, Stz* pTableName, Stz* pIndexName, Stz* pColumnName, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_AddIndexColumn"_Stz;
  
  try {
    TRACE_ENTRY_P3(pTableName, pIndexName, pColumnName);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pColumnName);
    
    // validate that we have a current table
    VALIDATE_TABLE("d"_Stz);
    
    // get the current table
    Table* pTable = pSEitf->pCurrentTable();
    ASSERT(pTable);
    
    // add a column
    eResult = eExecuteRequest(pSEitf, "addidxcol"_Stz,
                              pTableName,
                              pIndexName,
                              pColumnName);
    
    CHECK_RESULT("e"_Stz);
    CHECK_RESPONSE("f"_Stz);
    
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
/* add a column to a table                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddTableColumn(SEitf* pSEitf, Stz* pTableName, Stz* pColumnName, Stz* pColumnType, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_AddTableColumn"_Stz;
  
  try {
    TRACE_ENTRY_P2(pColumnName, pColumnType);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pColumnName);
    VALIDATE_PARAM("c"_Stz, pColumnType);
    
    // validate that we have a current table
    VALIDATE_TABLE("d"_Stz);
    
    // get the current table
    Table* pTable = pSEitf->pCurrentTable();
    
    // add a column
    eResult = eExecuteRequest(pSEitf, "addcol"_Stz,
                              pTableName,
                              pColumnName,
                              pColumnType);
    
    CHECK_RESULT("e"_Stz);
    CHECK_RESPONSE("f"_Stz);
    
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
/* begin fetch table                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginFetch(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable,
                        Stz* pRange, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_BeginFetch"_Stz;
  
  try {
    
    vTrace("Table: %s Range: %s\n"_Stz, pTable, pRange);
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change directory to table
    CF_TABLES("e"_Stz);
    
    // select * from pTable
    eResult = eExecuteRequest(pSEitf, "spnFetch"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // validate table
    VALIDATE_TABLE("h"_Stz);
    
    // validate index
    VALIDATE_INDEX("i"_Stz);
    
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
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
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
    
    // change folder to table
    CF_TABLE("e"_Stz);
    
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // validate we have a table
    VALIDATE_TABLE("h"_Stz);
    
    // begin a tnx
    bool bResult = pSEitf->bBeginTx();
    
    if (!bResult) {
      pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
      TRACE_ERROR("i",EC_BEGIN_TX_FAILED);
      return EC_BEGIN_TX_FAILED;
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
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(Stz* pVersion, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
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
/* create index                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateIndex(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable,
                         Stz* pIndex, bool bIsUnique, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_CreateIndex"_Stz;
  
  try {
    TRACE_ENTRY_P4(pCatalog, pSchema, pTable, pIndex);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    VALIDATE_PARAM("e"_Stz, pTable);
    VALIDATE_PARAM("f"_Stz, pIndex);
    
    // change folder to table
    CF_TABLE("g"_Stz);
    
    // get pTable
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, pTable);
    CHECK_RESPONSE("h"_Stz);
    VALIDATE_TABLE("i"_Stz);
    
    // change folder to tables
    CF_TABLES("j"_Stz);
    
    // create an index
    eResult = eExecuteRequest(pSEitf, "mkidx"_Stz, pTable, pIndex);
    CHECK_RESULT("k"_Stz);
    CHECK_RESPONSE("l"_Stz);
    
    // change folder to indexes
    CF_INDEXES("m"_Stz);
    
    eResult = eExecuteRequest(pSEitf, "getidx"_Stz, pIndex);
    CHECK_RESULT("n"_Stz);
    CHECK_RESPONSE("o"_Stz);

    if (bIsUnique) {
      // set unique index
      eResult = eExecuteRequest(pSEitf, "setidxunique"_Stz, pTable, pIndex, "true"_Stz);
      CHECK_RESULT("q"_Stz);
      CHECK_RESPONSE("r"_Stz);
    }
    
    VALIDATE_TABLE("ff"_Stz);
    
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
/* create table                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateTable(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_CreateTable"_Stz;
  
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
    
    // change folder to tables
    CF_TABLES("e"_Stz);

    // create a table
    eResult = eExecuteRequest(pSEitf, "mktbl"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // change directory to table
    eEC eResult = eExecuteRequest(pSEitf, "cf"_Stz, (Stz*)pTable);
    CHECK_RESULT("h"_Stz);
    CHECK_RESPONSE("i"_Stz);
    
    eEC eGetResult = eExecuteRequest(pSEitf, "gettbl"_Stz, (Stz*)pTable);

    if (EC_NONE != eGetResult) {
      
      // remove the table
      eResult = eExecuteRequest(pSEitf, "rmtbl"_Stz, (Stz*)pTable);
      CHECK_RESULT("j"_Stz);
      CHECK_RESPONSE("k"_Stz);
      
      TRACE_ERROR("l"_Stz, eGetResult);
      return eGetResult;
    }
    
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eGetResult) {
      TRACE_ERROR("m"_Stz,eGetResult);
      return eGetResult;
    }
    
    // validate current table
    VALIDATE_TABLE("n");
    
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
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DropTable(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_DropTable"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change folder to table
    CF_TABLES("e"_Stz);
    
    // drop table
    eResult = eExecuteRequest(pSEitf, "rmtbl"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
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
/* end create index                                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCreateIndex(SEitf* pSEitf, Stz* pTable, Stz* pIndex, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_EndCreateIndex"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    VALIDATE_TABLE("b"_Stz);
    
    // endmkidx only if we managed to getidx
    if (pSEitf->pCurrentIndex()) {
      eEC eResult = eExecuteRequest(pSEitf, "endmkidx"_Stz, pTable, pIndex);
      CHECK_RESULT("c"_Stz);
      CHECK_RESPONSE("d"_Stz);
      
      VALIDATE_NOTABLE("e"_Stz);
      
      // return the index
      eResult = eExecuteRequest(pSEitf, "retidx"_Stz);
      CHECK_RESULT("f"_Stz);
      CHECK_RESPONSE("g"_Stz);
      
    } else {
      // rettbl
      eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
      CHECK_RESULT("d"_Stz);
      CHECK_RESPONSE("e"_Stz);
    }
    
    // should not have a table
    VALIDATE_NOTABLE("h"_Stz);
    
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
/* end create table                                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCreateTable(SEitf* pSEitf, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_EndCreateTable"_Stz;

  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // endmktbl only if we managed to create the table
    if (pSEitf->pCurrentTable()) {
      eEC eResult = eExecuteRequest(pSEitf, "endmktbl"_Stz, pTable);
      CHECK_RESULT("b"_Stz);
      CHECK_RESPONSE("c"_Stz);
    }

    // rettbl only if we managed to create the table
    if (pSEitf->pCurrentTable()) {
      eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
      CHECK_RESULT("d"_Stz);
      CHECK_RESPONSE("e"_Stz);
    }
    
    // should not have a table
    VALIDATE_NOTABLE("f"_Stz);
    
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
/* end fetch                                                                  */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndFetch(SEitf* pSEitf, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_EndFetch"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate table
    VALIDATE_TABLE("b"_Stz);
    
    // validate index
    VALIDATE_INDEX("c"_Stz);
    
    // reset the index
    pSEitf->vCurrentIndex(nullptr);
    
    // validate no index
    VALIDATE_NOINDEX("d"_Stz);
    
    // return the table
    Table* pTable = pSEitf->pCurrentTable();
    eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
    CHECK_RESULT("e"_Stz);
    CHECK_RESPONSE("f"_Stz);
    
    // validate no table
    VALIDATE_NOTABLE("g"_Stz);
    
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
eEC RapidsSE_EndInsert(SEitf* pSEitf, i8 iTraceLevel)
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
    
    // validate we have a table
    VALIDATE_TABLE("b"_Stz);
    
    bool bResult = pSEitf->bInsertBuffer();
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      CHECK_RESPONSE("c"_Stz);
    }
    
    Table* pTable = pSEitf->pCurrentTable();
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

/*----------------------------------------------------------------------------*/
/* fetch next row                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextRow(SEitf* pSEitf, void** ppRow, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_FetchNextRow"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppRow);
    
    // get index buffer from seitf
    EntryBuffer* pEntryBuffer = pSEitf->m_pIndexBuffer;
    Entry* pEntry = nullptr;
    
    while (!pEntry) {
      
      bool bLastBuffer = pEntryBuffer ? pEntryBuffer->bIsLast() : false;
      
      if (!pEntryBuffer || !pEntryBuffer->bIsLast()) {

        // get the buffer queue from the seitf
        Queue* pBufferQueue = pSEitf->pBufferQueue();
      
        // pop buffer off the buffer queue
        ValWrk xValWrk;
        while (!pBufferQueue->bPop(&xValWrk)) {
          PosixThread::vSleepMilli(1);
        }

        // get entry buffer
        PtrVal* pPtrVal = xValWrk.pNestedPtrVal();
        pEntryBuffer = (EntryBuffer*)(pPtrVal->pPtr());

        // save the entry buffer in the SEitf
        pSEitf->m_pIndexBuffer = pEntryBuffer;
        
        // get first entry from buffer
        pEntry = pEntryBuffer->pFirstEntry();
      }
      else {
        
        ASSERT(pSEitf->m_pEntry);
        
        // get next entry from buffer
        pEntry = pEntryBuffer->pNextEntry(pSEitf->m_pEntry);
        
        if (bLastBuffer && !pEntry)
          break;
      }
    }
    
    // save entry in SEitf
    pSEitf->m_pEntry = pEntry;
    
    if (pEntry) {
      
      RowData::vRowAsString(pSEitf->m_sResponse, (Stz*)pEntry->aRow(), pSEitf->m_pCurrentTable->pColumnVec());
      *ppRow = (void*)pSEitf->m_sResponse.aChar();
    }
    else {
      // trace exit
      TRACE_EXIT("ea"_Stz);
      
      return EC_NONE;
    }
    
    //TRACE_ROW();
    
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
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalogs, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetCatalogs"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);

    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppCatalogs);
    
    // clear the last error
    CLEAR_LASTERROR();

    // change folder to catalogs
    CF_CATALOGS("c"_Stz);
    
    // list all catalogs
    eResult = eExecuteRequest(pSEitf, "lscat"_Stz);
    CHECK_RESULT("d"_Stz);
    CHECK_RESPONSE("e"_Stz);
    
    // return list of catalogs
    *ppCatalogs = pSEitf->m_sResponse.aChar();
    
    TRACE_EXIT("ea"_Stz);
    
    return eResult;
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
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumns(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema,
                        Stz* pTable, void** ppColumns, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetColumns"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
  
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change folder to table
    CF_TABLES("e"_Stz);
  
    // list all the columns of pTable
    eResult = eExecuteRequest(pSEitf, "lscol"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return eResult;
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
/* get list of indexes for given table of given catalog, schema and table     */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetIndexes(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema,
                        Stz* pTable, void** ppColumns, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetIndexes"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change folder to table
    CF_TABLES("e"_Stz);
    
    // list all the indexes of pTable
    eResult = eExecuteRequest(pSEitf, "lsidx"_Stz, pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return eResult;
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
/* get list of columns for given table of given catalog, schema, table, index */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetIndexColumns(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema,
                        Stz* pTable, Stz* pIndex, void** ppColumns, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetIndexes"_Stz;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    VALIDATE_PARAM("d"_Stz, pTable);
    
    // change folder to table
    CF_TABLES("e"_Stz);
    
    // list all the indexes of pTable
    eResult = eExecuteRequest(pSEitf, "lsidxcol"_Stz, pTable, pIndex);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);
    
    return eResult;
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
eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetLastError"_Stz;
  
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
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions, void** ppCount, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetPartitions"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, ppPartitions);
    VALIDATE_REFPOINTER("c"_Stz, ppCount);

    *ppCount = &gpGrid->m_nSlots;
    *ppPartitions = &gpGrid->m_iPartition;
    
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
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetSchemas(SEitf* pSEitf, Stz* pCatalog, void** ppSchemas, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetSchemas"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(pCatalog);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Stz, ppSchemas);
    
    // validate parameters
    VALIDATE_PARAM("c"_Stz, pCatalog);
    
    // change folder to schemas
    CF_SCHEMAS("d"_Stz);
    
    // list all schemas in pCatalog
    eResult = eExecuteRequest(pSEitf, "lssch"_Stz);
    CHECK_RESULT("e"_Stz);
    CHECK_RESPONSE("f"_Stz);
    
    // return list of schemas in ppSchemas
    *ppSchemas = pSEitf->m_sResponse.aChar();
    
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

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetTables(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, void** ppTables, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetTables"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P2(pCatalog, pSchema);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();

    // validate parameters
    VALIDATE_PARAM("b"_Stz, pCatalog);
    VALIDATE_PARAM("c"_Stz, pSchema);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("d"_Stz, ppTables);
    
    // change folder to tables
    CF_TABLES("e"_Stz);
    
    // list all the tables in pSchema of pCatalog
    eResult = eExecuteRequest(pSEitf, "lstbl"_Stz);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return list of tables in ppSchemas
    *ppTables = pSEitf->m_sResponse.aChar();
    
    // trace exit
    TRACE_EXIT("ea"_Stz);

    return eResult;
    
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
/* hash the row to a partition                                                */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, Stz* pRow, i8 iTraceLevel)
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
    VALIDATE_PARAM("b"_Stz, pRow);
    
    // validate table
    VALIDATE_TABLE("c"_Stz);
    
    bool bResult = pSEitf->bInsertRow(pRow);
    
    if (!bResult) {
      pSEitf->vSetLastError(pSEitf->m_eErrorCode);
      TRACE_ERROR("d"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
    
    // validate table
    VALIDATE_TABLE("e"_Stz);

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
    bool bResult = RapidsSE::bPoolOpen(sPoolName);
    
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
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // delete the SEitf
    RapidsSE::vSEitfDelete(pSEitf);
    
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
eEC RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition, i8 iTraceLevel)
{
  eEC eResult;
  Stz* aApiCall = "RapidsSE_SEitfNew"_Stz;
  SEitf* pSEitf = nullptr;
  
  try {
    
     // trace entry
    TRACE_ENTRY_P0();
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Stz, ppSEitf);
    
    VALIDATE_POOL();
    
    SEitf* pSEitf = new SEitf(iPartition);
    
    pSEitf->vInit();
    
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
/* Execute a request                                                          */
/*----------------------------------------------------------------------------*/
bool RapidsSE_ExecuteRequest(SEitf* pSEitf, TokenVec& xTokenVec, Stz** ppResponse)
{
  bool bResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // send request to se
  bResult = bExecuteRequest(pSEitf, xTokenVec);
  if (!bResult) {
    *ppResponse = nullptr;
    return bResult;
  }
  
  // get response from se
  eEC eResult = eGetResponse(pSEitf);
  
  // put response into caller
  *ppResponse = pSEitf->m_sResponse;
  
  if (eResult == EC_NONE)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Stz* aCommand)
{
  bool bResult;
  
  TokenVec xTokenVec;
  
  // execute command pCommand
  //xTokenVec[0]->vCopy(aCommand);
  //xTokenVec[1]->vReset();
  
  xTokenVec.vAddToken(aCommand);
  xTokenVec[1]->vReset();
  
  bResult = pSEitf->bDecode(xTokenVec);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Stz* aCommand, Stz* aParameter)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  
  TokenVec xTokenVec;
  
  xTokenVec.vAddToken(aCommand);
  xTokenVec.vAddToken(aParameter);
  xTokenVec[2]->vReset();
  
  bResult = pSEitf->bDecode(xTokenVec);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Stz* aCommand, Stz* aParameter1,
                    Stz* aParameter2)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  
  TokenVec xTokenVec;
  
  xTokenVec.vAddToken(aCommand);
  xTokenVec.vAddToken(aParameter1);
  xTokenVec.vAddToken(aParameter2);
  xTokenVec[3]->vReset();
  
  bResult = pSEitf->bDecode(xTokenVec);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                           */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Stz* aCommand, Stz* aParameter1,
                    Stz* aParameter2, Stz* aParameter3)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  TokenVec xTokenVec;
  
  xTokenVec.vAddToken(aCommand);
  xTokenVec.vAddToken(aParameter1);
  xTokenVec.vAddToken(aParameter2);
  xTokenVec.vAddToken(aParameter3);
  xTokenVec[4]->vReset();

  bResult = pSEitf->bDecode(xTokenVec);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
bool bExecuteRequest(SEitf* pSEitf, TokenVec& xTokenVec)
{
  bool bResult;
  
  // execute command
  bResult = pSEitf->bDecode(xTokenVec);
  
  return bResult;
}


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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
