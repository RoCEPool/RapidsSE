/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;

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
/* add a partitionBy column to a table                                        */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddPartitionByColumn(SEitf* pSEitf, Stz* pTableName, Stz* pColumnName, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_AddPartitionByColumn"_Stz;
  
  try {
    TRACE_ENTRY_P2(pTableName, pColumnName);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Stz, pTableName);
    VALIDATE_PARAM("b"_Stz, pColumnName);
    
    // validate that we have a current table
    VALIDATE_TABLE("d"_Stz);
    
    // add a column
    eResult = eExecuteRequest(pSEitf, "addpartcol"_Stz,
                              pTableName,
                              pColumnName);
    
    CHECK_RESULT("d"_Stz);
    CHECK_RESPONSE("e"_Stz);
    
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
    VALIDATE_PARAM("b"_Stz, pTableName);
    VALIDATE_PARAM("b"_Stz, pColumnName);
    VALIDATE_PARAM("c"_Stz, pColumnType);
    
    // validate that we have a current table
    VALIDATE_TABLE("d"_Stz);
    
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
    
    //vTrace("Table: %s Range: %s\n"_Stz, pTable, pRange);
    
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
    eResult = eExecuteRequest(pSEitf, "spnFetch"_Stz, pTable, pRange);
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

/*----------------------------------------------------------------------------*/
/* begin insert on SEitf partiton                                             */
/*----------------------------------------------------------------------------*/
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
    VALIDATE_PARAM("e"_Stz, pIndex);
    
    // change directory to table
    CF_TABLES("f"_Stz);
    
    // get pTable
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, pTable);
    CHECK_RESPONSE("g"_Stz);
    VALIDATE_TABLE("h"_Stz);
    
    // create an index
    eResult = eExecuteRequest(pSEitf, "mkidx"_Stz, pTable, pIndex);
    CHECK_RESULT("j"_Stz);
    CHECK_RESPONSE("k"_Stz);
    
    eResult = eExecuteRequest(pSEitf, "getidx"_Stz, pTable, pIndex);
    CHECK_RESULT("m"_Stz);
    CHECK_RESPONSE("n"_Stz);

    if (bIsUnique) {
      // set unique index
      eResult = eExecuteRequest(pSEitf, "setidxunique"_Stz, pTable, pIndex, "true"_Stz);
      CHECK_RESULT("o"_Stz);
      CHECK_RESPONSE("p"_Stz);
    }
    
    VALIDATE_TABLE("q"_Stz);
    
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
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DropIndex(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable,
                       Stz* pIndex, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_DropIndex"_Stz;
  
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
    VALIDATE_PARAM("d"_Stz, pIndex);
    
    // change folder to tables
    CF_TABLES("e"_Stz);
    
    // drop the index
    eResult = eExecuteRequest(pSEitf, "rmidx"_Stz, pTable, (pIndex));
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
    
    // endmkidx only if we managed to getidx
    if (pSEitf->pCurrentIndex()) {
      
      VALIDATE_TABLE("b"_Stz);
      
      eEC eResult = eExecuteRequest(pSEitf, "endmkidx"_Stz, pTable, pIndex);
      CHECK_RESULT_RETURNTABLE("c"_Stz);
      CHECK_RESPONSE_RETURNTABLE("d"_Stz);
      
      // return the index
      eResult = eExecuteRequest(pSEitf, "retidx"_Stz);
      CHECK_RESULT_RETURNTABLE("e"_Stz);
      CHECK_RESPONSE_RETURNTABLE("f"_Stz);
    
    } else {
      // rettbl
      if (pSEitf->pCurrentTable()) {
        eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
        CHECK_RESULT("g"_Stz);
        CHECK_RESPONSE("h"_Stz);
      }
    }
    
    // should not have a table
    VALIDATE_NOTABLE("i"_Stz);
    
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
    
    Table* pTablePtr = pSEitf->pCurrentTable();
    
    // did we manage to create the table
    if (pTablePtr) {
      
      // endmktbl
      eResult = eExecuteRequest(pSEitf, "endmktbl"_Stz, pTable);
      CHECK_RESULT_RETURNTABLE("d"_Stz);
      CHECK_RESPONSE_RETURNTABLE("e"_Stz);
      
      // rettbl
      eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
      CHECK_RESULT("f"_Stz);
      CHECK_RESPONSE("g"_Stz);
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
  //vTrace("EndFetch %s\n"_Stz, pSEitf->pCurrentTable()->aTableName());
  
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
    
    // mark that seitf is exiting
    pSEitf->m_bSeitfExiting = true;
    
    // End the fetch by waiting for all fetch tasks to finish
    // and then clean up the buffer queue etc
    pSEitf->vEndFetch();
    
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
    
    Grid* pGrid = gpAttachMap->m_pGrid;
    
    // before sending buffers set no of remaining acks to no of partitons
    pSEitf->vSetAcksRemaining(pGrid->nSlots() * pGrid->nNodes());
    //vTrace("Seitf:%p Setting acksRemaining : %lld\n"_Stz, pSEitf, *pSEitf->m_ziAcksRemaining);
    
    for (i8 iPartition = 0; iPartition < pGrid->m_nPartitions; iPartition++) {
      bool bResult = pSEitf->bInsertLastBuffer(iPartition);
      
      //vTrace("bInsertLastBuffer returned %lld %lld\n"_Stz, iPartition, bResult);
      
      // if rowBuffer request was appended to requestQ, then wait for resonse
      if (bResult) {
        CHECK_RESPONSE_RETURNTABLE("c"_Stz);
      }
      //vTrace("done for loop\n"_Stz);
    }
    
    // now wait till acks from all partitions have been received
    //vTrace("Seitf:%p waiting for acksRemaing\n"_Stz, pSEitf);
    
    while (*pSEitf->m_ziAcksRemaining != 0) {
      //vTrace("*pSEitf->m_ziAcksRemaining %lld ...\n"_Stz, *pSEitf->m_ziAcksRemaining);
      ;
    }
    
    eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
    CHECK_RESULT("d"_Stz);
    CHECK_RESPONSE("e"_Stz);
    
    // validate if no table
    VALIDATE_NOTABLE("f"_Stz);
    
    eResult = eExecuteRequest(pSEitf, "txCmt"_Stz);
    CHECK_RESULT("g"_Stz);
    CHECK_RESPONSE("h"_Stz);
    
    eResult = eExecuteRequest(pSEitf, "txEnd"_Stz);
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

/*----------------------------------------------------------------------------*/
/* end insert on SEitf partition                                              */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* export data from table into the file                                       */
/*----------------------------------------------------------------------------*/
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

/*----------------------------------------------------------------------------*/
/* fetch next buffer                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextBuffer(SEitf* pSEitf, void** ppWorkingBuffer, i8 iTraceLevel)
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
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppWorkingBuffer);
    
    bool bNextBuffer = pSEitf->bNextBuffer();
    
    if (bNextBuffer) {
      *ppWorkingBuffer = pSEitf->m_pResultBuffer;
      //pSEitf->m_pResultBuffer->vShow();
    }
    else {
      *ppWorkingBuffer = nullptr;
    }
    //vTrace("Buffer %p FirstRow %p Entries %lld\n"_Stz, *ppEntryBuffer, *ppFirstEntry, *ppEntries);
    
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
/* fetch next row                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_FetchNextRow(SEitf* pSEitf, void** ppRow, i8 iTraceLevel)
{
  eEC eResult;
  
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
    
    Entry* pEntry = nullptr;
    
    i8 iPartition;
    bool bNextRow = pSEitf->bNextRow2(iPartition);
    
    if (bNextRow) {
      //Entry* pEntry = pSEitf->m_pEntry;
      
      // return back row
      *ppRow = (void*)(Stz*)pSEitf->m_aRow;
    }
    else {
      *ppRow = nullptr;
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
/* fetch next row                                                             */
/*----------------------------------------------------------------------------*/
/*
eEC RapidsSE_FetchNextRow2(SEitf* pSEitf, void** ppRow, i8 iTraceLevel)
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
    EntryBuffer* pEntryBuffer = pSEitf->m_pEntryBuffer;
    Entry* pEntry = nullptr;
    
    bool bNextRow = pSEitf->bNextRow();
    
    if (bNextRow) {
      Entry* pEntry = pSEitf->m_pEntry;
      *ppRow = (void*)pEntry->aRowData();
    }
    else {
      *ppRow = nullptr;
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
*/

/*----------------------------------------------------------------------------*/
/* fetch next row                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumnFromRow(SEitf* pSEitf, i8 iColumnNo, void** ppRow, void** ppValue, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_GetColumnFromRow"_Stz;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Stz);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Stz, ppRow);
    VALIDATE_REFPOINTER("c"_Stz, ppValue);
    
    char* pRow = (char*)(*ppRow);
    Row* aRow = (Row*)pRow;
    //RowData::vRowAsString(pSEitf->m_sResponse, (Stz*)pRow, pSEitf->m_pCurrentTable->pColumnVec());
    //vTrace("Row: %s\n"_Stz, pSEitf->m_sResponse.aChar());
    
    // get the column from the index column vector
    ColumnVec* pColumnVec = pSEitf->m_pCurrentTable->pColumnVec();
    
    Column* pColumn = pColumnVec->pGet(iColumnNo);
    
    // get the column value from the row for the iColumnNo
    char* pSrc = aRow->aColumnDatum(iColumnNo);
    
    switch(pColumn->eDatumType()) {
        
      case DM_BOL: {
        Bol xBol = *((Bol*)pSrc);
        *((i8*)ppValue) = xBol.iAsI8();
        break;
      }
        
      case DM_BCD: {
        Bcd::vSprint(pSEitf->m_sResponse, (Stz*)pSrc, pColumn);
        *ppValue = pSEitf->m_sResponse.aChar();
        break;
      }
        
      case DM_FLT: {
        Flt xFlt = *((Flt*)pSrc);
        *(ppValue) = pSrc;
        break;
      }
        
      case DM_INT: {
        Int xInt = *((Int*)pSrc);
        *(ppValue) = pSrc;
        break;
      }
        
      case DM_TSP: {
        Tsp::vSprintNonSQL(pSEitf->m_sResponse, (Stz*)pSrc);
        //bool b = Tsp::bExtract(pDst,&xTsp,iLength,pColumn);
        //ASSERT(b);
        *ppValue = pSEitf->m_sResponse.aChar();
        break;
      }
        
      case DM_VCH: {
        *ppValue = pSrc;
        break;
      }
        
      default:
        NEVER_REACH;
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

/*
    // list all the columns of pTable
    eResult = eExecuteRequest(pSEitf, "lscol"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.aChar();
 */
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Stz, (Stz*)pTable);
    CHECK_RESULT("f"_Stz);
    CHECK_RESPONSE("g"_Stz);
    
    // validate we have a table
    VALIDATE_TABLE("h"_Stz);
    
    // iterate through columns of the table and for a string
    ColumnVec* pColumnVec = pSEitf->m_pCurrentTable->pColumnVec();
    i8 iColumns = pColumnVec->nItems();
    
    s48K sColumns;
    
    sColumns.vCopy(""_Stz);
    s32 sValue;
    
    // skip the hidden column
    for (i8 i = 0; i < iColumns - 1; i++) {
      if (i > 0)
        sColumns.vConcat(", "_Stz);
      
      Column* pColumn = pColumnVec->pGet(i);
      //pColumn->vShow();
      
      sColumns.vConcat(pColumn->sColumnName());
      sColumns.vConcat(" "_Stz);
      sColumns.vConcat(aCT[pColumn->eColumnType()]);
      
      // no precision/scale for boolean and timestamps
      if (pColumn->eColumnType() != CT_TIMESTAMP &&
          pColumn->eColumnType() != CT_BOOLEAN) {
        
        sColumns.vConcat("("_Stz);
        i8 iPrecision = pColumn->iColumnLength();
        StrLib::wToString(iPrecision, sValue.aChar());
        sColumns.vConcat(sValue);
        
        if (pColumn->eColumnType() == CT_DECIMAL) {
          sColumns.vConcat(","_Stz);
          StrLib::wToString(pColumn->iColumnScale(), sValue.aChar());
          sColumns.vConcat(sValue);
        }
        sColumns.vConcat(")"_Stz);
      }
      
      // check for NULLABLE
      sColumns.vConcat(pColumn->bIsNullable() ?
                                    " "_Stz : " NOT NULL "_Stz);
      
      // check if partition key
      if (pColumn->bIsPartitionKey())
        sColumns.vConcat(pColumn->bIsPartitionKey() ?
                         " PARTITION_KEY "_Stz : " "_Stz);
    }
    
    //vTrace("%s\n"_Stz, sColumns.aChar());
    
    // return reference to the table
    eResult = eExecuteRequest(pSEitf, "rettbl"_Stz);
    CHECK_RESULT("i"_Stz);
    CHECK_RESPONSE("j"_Stz);
    
    // validate we have a table
    VALIDATE_NOTABLE("k"_Stz);
    
    pSEitf->m_sResponse.vCopy(sColumns);
    
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

    *ppCount = &gpAttachMap->m_pGrid->m_nSlots;
    *ppPartitions = &gpAttachMap->m_pGrid->m_iPartition;
    
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
/* hash the integer value to a node                                           */
/*----------------------------------------------------------------------------*/
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
    
    s48K s;
    i8 iRowLength;
    bool bBlankLine;
    
    // format the record into row format
    bool bValid = Row::bFormatRow(pSEitf->m_pCurrentTable->pColumnVec(),
                                  &pSEitf->m_xFormatters,
                                  pRow, StrLib::iSize(pRow->aChar()),
                                  s, iRowLength, bBlankLine,
                                  true /* allowNonPrintable */);
    if (! bValid) {
      pSEitf->vSetLastError(EC_INVALID_VALUE);
      TRACE_ERROR("d"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
 
    // get the partition this row hashes to
    i8 iRowPartition = pSEitf->pCurrentTable()->iRowPartition(s);
    
    //vTrace("%p(%lld) got row %s\n"_Stz,
    //       pSEitf, pSEitf ? pSEitf->iPartition() : -1, pRow);
    
    
    //RowData::vShow(s, pSEitf->m_pCurrentTable->pColumnVec());
    TRACE_INSERT("e"_Stz, pRow);
    
    // insert the row in this partition
    bool bResult = pSEitf->bInsertRow(s, iRowLength, iRowPartition);
   
    if (!bResult) {
      pSEitf->vSetLastError(pSEitf->m_eErrorCode);
      TRACE_ERROR("f"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
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
    
    s48K s;
    i8 iRowLength;
    bool bBlankLine;
    
    // format the record into row format
    bool bValid = Row::bFormatRow(pSEitf->m_pCurrentTable->pColumnVec(),
                                  &pSEitf->m_xFormatters,
                                  pRow, StrLib::iSize(pRow->aChar()),
                                  s, iRowLength, bBlankLine,
                                  true /* allowNonPrintable */);
    if (! bValid) {
      pSEitf->vSetLastError(EC_INVALID_VALUE);
      TRACE_ERROR("d"_Stz, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
    
    // get the partition this row hashes to
    i8 iRowPartition = pSEitf->pCurrentTable()->iRowPartition(s);
    
    //vTrace("%p(%lld) got row %s\n"_Stz,  
    //       pSEitf, pSEitf ? pSEitf->iPartition() : -1, pRow);
    
    // is this row for this seitf partition ?
    if (pSEitf->m_iPartition == iRowPartition) {
      
      //RowData::vShow(s, pSEitf->m_pCurrentTable->pColumnVec());
      TRACE_INSERT("e"_Stz, pRow);
      
      // insert the row in this partition
      bool bResult = pSEitf->bInsertRow(s, iRowLength);
      
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
/* launch a prompt job                                                        */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_Prompt(i8 iTraceLevel)
{
  Stz*   aApiCall = "RapidsSE_Prompt"_Stz;
  SEitf* pSEitf = nullptr;
  s256   sRequest;

  try {
    
    eEC eError = RapidsSE_SEitfNew((void**)&pSEitf, 0, 0);
    ASSERT(EC_NONE == eError);
    
    Job_Prompt* pJob_Prompt = new Job_Prompt(pSEitf);
    
    pJob_Prompt->m_sSeed = "promptR35_req"_Stz;
    pJob_Prompt->m_bDetached = true;
    
    bool b = pSEitf->bAppendJob((Job**)&pJob_Prompt);
    
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
/* truncate table                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_TruncateTable(SEitf* pSEitf, Stz* pCatalog, Stz* pSchema, Stz* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Stz* aApiCall = "RapidsSE_TruncateTable"_Stz;
  
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
    eResult = eExecuteRequest(pSEitf, "tr"_Stz, (Stz*)pTable);
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
  if (! bResult) {
    *ppResponse = nullptr;
    return bResult;
  }
  
  if (xTokenVec.bDetachedRequest())
    return true;
  
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
/* Execute a request                                                          */
/*----------------------------------------------------------------------------*/
bool RapidsSE_DispatchJob(Job* pJob, SEitf* pSEitf, Stz** ppResponse)
{
  bool bResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // dispatch job to se
  bResult = pSEitf->bAppendJob(&pJob);
  
  if (! bResult) {
    *ppResponse = nullptr;
    return bResult;
  }
  
  if (pJob->m_bDetached)
    return true;
  
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
  
  xTokenVec.vSetAllTokens();
  
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
  
  xTokenVec.vSetAllTokens();
  
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
  
  xTokenVec.vSetAllTokens();
  
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
  
  xTokenVec.vSetAllTokens();
  
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
