/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;
extern Grid*       gpGrid;

/*----------------------------------------------------------------------------*/
/* add a column to a table                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddColumn(SEitf* pSEitf, Str* pColumnName, Str* pColumnType, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;

  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    TRACE_ENTRY_P2(pColumnName, pColumnType);

    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Str, pColumnName);
    VALIDATE_PARAM("c"_Str, pColumnType);
    
    Table* pTable = pSEitf->pCurrentTable();
    
    VALIDATE_TABLE("d"_Str);

    // add a column
    eResult = eExecuteRequest(pSEitf, "add"_Str,
                              (Str*)pTable->aTableName(),
                              (Str*)pColumnName,
                              (Str*)pColumnType);
    
    CHECK_RESULT("e"_Str);
    CHECK_RESPONSE("f"_Str);
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_BeginInsert(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_BeginInsert"_Str;
  
  try {
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
  
    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    VALIDATE_PARAM("d"_Str, pTable);
    
    // change folder to table
    CF_TABLE("e"_Str);
    
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);
    
    // begin a tnx
    bool bResult = pSEitf->bBeginTx();
    
    if (!bResult) {
      pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
      TRACE_ERROR("h",EC_BEGIN_TX_FAILED);
      return EC_BEGIN_TX_FAILED;
    }
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_CheckLibraryVersion(Str* pVersion, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  SEitf* pSEitf = nullptr;
  
  Str* aApiCall = "RapidsSE_CheckLibraryVersion"_Str;
  
  try {
    TRACE_ENTRY_P1(pVersion);
    
    if ( ! pVersion->bEqual((Str*)RAPIDSSE_VERSION_STRING)) {
      
      vTrace(">RapidsSE_CheckLibraryVersion: RDP (%s) does not match RapidsSE (%s)"_Str,
          pVersion, RAPIDSSE_VERSION_STRING);
      
      TRACE_ERROR("a"_Str, EC_VERSION_MISMATCH);
      return EC_VERSION_MISMATCH;
    }
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_CreateTable"_Str;
  
  try {
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    VALIDATE_PARAM("d"_Str, pTable);
    
    // change folder to tables
    CF_TABLE("e"_Str);

    // create a table
    eResult = eExecuteRequest(pSEitf, "mktbl"_Str, (Str*)pTable);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
    CHECK_RESULT("h"_Str);
    CHECK_RESPONSE("i"_Str);
    
    // get pTable
    eEC eGetResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
    if (EC_NONE != eGetResult) {
      
      // remove the table
      eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
      CHECK_RESULT("j"_Str);
      CHECK_RESPONSE("k"_Str);
      
      TRACE_ERROR("l"_Str, eGetResult);
      return eGetResult;
    }
    
    // check response
    CHECK_RESPONSE("m"_Str);
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_DropTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_DropTable"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    VALIDATE_PARAM("d"_Str, pTable);
    
    // change folder to table
    CF_TABLES("e"_Str);
    
    // drop table
    eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* end create                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCreate(SEitf* pSEitf, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_EndCreate"_Str;

  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    CHECK_RESULT("b"_Str);
    CHECK_RESPONSE("c"_Str);
    
    VALIDATE_TABLE("d"_Str);
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_EndInsert"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    bool bResult = pSEitf->bInsertBuffer();
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      CHECK_RESPONSE("b"_Str);
    }
    
    eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    CHECK_RESULT("c"_Str);
    CHECK_RESPONSE("d"_Str);
    
    VALIDATE_TABLE("e"_Str);
    
    pSEitf->vCommitTx();
    pSEitf->vEndTx();
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* end scan                                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndScan(SEitf* pSEitf, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_EndScan"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate table
    VALIDATE_TABLE("b"_Str);
    
    // get address of select cursor
    i8 iAddress = StrLib::iToI8( pSEitf->m_sResponse.s());
    
    // delete the SelCursor
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    delete pSelCursor;
    
    // return the table
    eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    CHECK_RESULT("c"_Str);
    CHECK_RESPONSE("d"_Str);
    
    VALIDATE_TABLE("e"_Str);
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_GetCatalogs"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);

    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Str, ppCatalogs);
    
    // clear the last error
    CLEAR_LASTERROR();

    // change folder to catalogs
    CF_CATALOGS("c"_Str);
    
    // list all catalogs
    eResult = eExecuteRequest(pSEitf, "lscat"_Str);
    CHECK_RESULT("d"_Str);
    CHECK_RESPONSE("e"_Str);
    
    // return list of catalogs
    *ppCatalogs = pSEitf->m_sResponse.s();
    
    TRACE_EXIT("ea"_Str);
    
    return eResult;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_GetColumns(SEitf* pSEitf, Str* pCatalog, Str* pSchema,
                        Str* pTable, void** ppColumns, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_GetColumns"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
  
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    VALIDATE_PARAM("d"_Str, pTable);
    
    // change folder to table
    CF_TABLE("e"_Str);
  
    // list all the columns of pTable
    eResult = eExecuteRequest(pSEitf, "lscol"_Str, (Str*)pTable);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);
    
    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.s();
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return eResult;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_GetLastError"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Str, ppError);
    
    // get the error message
    *ppError = pSEitf->m_sErrorMsg.s();
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_GetNextRow"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
  
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointer
    VALIDATE_REFPOINTER("b"_Str, ppRow);

    i8 iAddress = pSEitf->m_sResponse.iToI8();
    
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    
    *ppRow = pSelCursor->aRowAsStr();
    if (!*ppRow) {
      delete pSelCursor;
      
      // return the table
      eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
      CHECK_RESULT("c"_Str);
      CHECK_RESPONSE("d"_Str);
      
      ASSERT(pSEitf->pCurrentTable() == nullptr);
      
      // trace exit
      TRACE_EXIT("ea"_Str);
      
      return EC_NONE;
    }
    
    //TRACE_ROW();
    
    // move cursor to next row
    pSelCursor->vNext();
    
    // trace exit
    TRACE_EXIT("eb"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_GetPartitions"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Str, ppPartitions);
    VALIDATE_REFPOINTER("c"_Str, ppCount);

    *ppCount = &gpGrid->m_nSlots;
    *ppPartitions = &gpGrid->m_iPartition;
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_GetSchemas(SEitf* pSEitf, Str* pCatalog, void** ppSchemas, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_GetSchemas"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(pCatalog);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // validate the reference pointers
    VALIDATE_REFPOINTER("b"_Str, ppSchemas);
    
    // validate parameters
    VALIDATE_PARAM("c"_Str, pCatalog);
    
    // change folder to schemas
    CF_SCHEMAS("d"_Str);
    
    // list all schemas in pCatalog
    eResult = eExecuteRequest(pSEitf, "lssch"_Str);
    CHECK_RESULT("e"_Str);
    CHECK_RESPONSE("f"_Str);
    
    // return list of schemas in ppSchemas
    *ppSchemas = pSEitf->m_sResponse.s();
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_GetEngineVersion"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Str, ppVersion);
    
    // get the SE version
    eResult = eExecuteRequest(pSEitf, "version"_Str);
    CHECK_RESULT("c"_Str);
    CHECK_RESPONSE("d"_Str);
    
    // return SE version
    *ppVersion = pSEitf->m_sResponse.s();
    
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_GetTables(SEitf* pSEitf, Str* pCatalog, Str* pSchema, void** ppTables, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_GetTables"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();

    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    
    // valide reference pointer
    VALIDATE_REFPOINTER("d"_Str, ppTables);
    
    // change folder to tables
    CF_TABLES("e"_Str);
    
    // list all the tables in pSchema of pCatalog
    eResult = eExecuteRequest(pSEitf, "lstbl"_Str);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);
    
    // return list of tables in ppSchemas
    *ppTables = pSEitf->m_sResponse.s();
    
    // trace exit
    TRACE_EXIT("ea"_Str);

    return eResult;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_HashRowPartition(SEitf* pSEitf, Str* pRow, void** ppPartition, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_HashRowPartition"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();

    // valide reference pointer
    VALIDATE_REFPOINTER("b"_Str, ppPartition);

    // validate parameters
    VALIDATE_PARAM("c"_Str, pRow);
    
    // validate table
    VALIDATE_TABLE("d"_Str);
    
    pSEitf->m_iRowPartition = pSEitf->pCurrentTable()->iRowPartition(pRow);
    *ppPartition = &pSEitf->m_iRowPartition;
    
    //TRACE_RESULT(*ppPartition);
    
    // trace exit
    TRACE_EXIT("ea"_Str);

    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_InsertRow(SEitf* pSEitf, Str* pRow, i8 iTraceLevel)
{

  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_InsertRow"_Str;
  
  try {
    
    // trace entry
    TRACE_ENTRY_P0();
    
    //TRACE_ROW();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);

    // clear the last error
    CLEAR_LASTERROR();

    // validate parameter
    VALIDATE_PARAM("b"_Str, pRow);
    
    // validate table
    VALIDATE_TABLE("c"_Str);
    
    bool bResult = pSEitf->bInsertRow(pRow);
    
    if (!bResult) {
      pSEitf->vSetLastError(pSEitf->m_eErrorCode);
      TRACE_ERROR("d"_Str, pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
    
    // trace exit
    TRACE_EXIT("ea"_Str);

    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
eEC RapidsSE_PoolOpen(Str* pInstanceName, void** ppInstance, i8 iTraceLevel)
{
  
  Str* aApiCall = "RapidsSE_PoolOpen"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P1(pInstanceName);
    
    // open the pool
    s256 sPoolName;
    
    sPoolName = (Str*)pInstanceName;
    bool bResult = RapidsSE::bPoolOpen(sPoolName);
    
    if (!bResult) {
      TRACE_ERROR("a"_Str, EC_INVALID_INSTANCE);
      return EC_INVALID_INSTANCE;
    }
    
    *ppInstance = gpAttachMap;
    
    // trace exit
    TRACE_EXIT("ea"_Str);
    
    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
    TRACE_ERROR("!", pError->eErrorCode());
    return pError->eErrorCode();
  }
  catch (...) {
    TRACE_ERROR("?", EC_SE_UNKNOWN_ERROR);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* scan table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_ScanTable"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P3(pCatalog, pSchema, pTable);
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();

    // validate parameters
    VALIDATE_PARAM("b"_Str, pCatalog);
    VALIDATE_PARAM("c"_Str, pSchema);
    VALIDATE_PARAM("d"_Str, pTable);
    
    // change directory to table
    CF_TABLE("e"_Str);
    
    // select * from pTable
    eResult = eExecuteRequest(pSEitf, "sel"_Str, (Str*)pTable);
    CHECK_RESULT("f"_Str);
    CHECK_RESPONSE("g"_Str);

    // validate table
    VALIDATE_TABLE("h");
    
    // get response (address of selcussor)
    i8 iAddress = pSEitf->m_sResponse.iToI8();
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    
    // get slot for seitf partition on this node
    i8 iSlot = Partition::iPartitionToSlot(pSEitf->iPartition());
    
    // TRACE_SLOT(iSlot);
    
    // begin the select
    pSelCursor->vBegin(iSlot);
    
    // get first row
    pSelCursor->vFirst();

    // trace exit
    TRACE_EXIT("ea"_Str);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  Str* aApiCall = "RapidsSE_SEitfDelete"_Str;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // validate the SEitf
    VALIDATE_SEITF("a"_Str);
    
    // clear the last error
    CLEAR_LASTERROR();
    
    // delete to SEitf
    RapidsSE::vSEitfDelete(pSEitf);
    
    // trace exit
    TRACE_EXIT("ea"_Str);

    return EC_NONE;
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  
  Str* aApiCall = "RapidsSE_SEitfInvalid"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    if (iError == EC_INVALID_INTERFACE) {
      // trace exit
      TRACE_EXIT("ea"_Str);

      return true;
    }
    else {
      // trace exit
      TRACE_EXIT("ea"_Str);

      return false;
    }
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
  Str* aApiCall = "RapidsSE_SEitfNew"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    // trace entry
    TRACE_ENTRY_P0();
    
    // valide reference pointer
    VALIDATE_REFPOINTER("a"_Str, ppSEitf);
    
    SEitf* pSEitf = new SEitf(iPartition);
    pSEitf->vInit();
    
    // validate the SEitf just created
    VALIDATE_SEITF("a"_Str);
    
    // return the newly created SEitf
    *ppSEitf = pSEitf;
    
    // trace exit
    TRACE_EXIT("ea"_Str);

    return EC_NONE;
    
  }
  catch (Error* pError) {
    RapidsSE::vHandler(pError);
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
bool RapidsSE_ExecuteRequest(SEitf* pSEitf, Str* aWords[], i8 iWords, Str** ppResponse)
{
  bool bResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // send request to se
  bResult = bExecuteRequest(pSEitf, aWords, iWords);
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
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand)
{
  bool bResult;
  char xArray[C_1KB * 1];
  Str* aWords[16] = {
    (Str*)&xArray[C_1KB * 0],
  };
  
  // execute command pCommand
  aWords[0]->vCopy(aCommand);
  
  bResult = pSEitf->bDecode((Str**)aWords, 1);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  char xArray[C_1KB * 2];
  Str* aWords[16] = {
    (Str*)&xArray[C_1KB * 0],
    (Str*)&xArray[C_1KB * 1]
  };
  
  // execute command
  aWords[0]->vCopy(aCommand);
  aWords[1]->vCopy(aParameter);
  
  bResult = pSEitf->bDecode((Str**)aWords, 2);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                           */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter1,
                    Str* aParameter2, Str* aParameter3)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  char xArray[C_1KB * 4];
  Str* aWords[16] = {
    (Str*)&xArray[C_1KB * 0],
    (Str*)&xArray[C_1KB * 1],
    (Str*)&xArray[C_1KB * 2],
    (Str*)&xArray[C_1KB * 3]
  };
  
  // execute command
  aWords[0]->vCopy(aCommand);
  aWords[1]->vCopy(aParameter1);
  aWords[2]->vCopy(aParameter2);
  aWords[3]->vCopy(aParameter3);
  
  bResult = pSEitf->bDecode((Str**)aWords, 4);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
bool bExecuteRequest(SEitf* pSEitf, Str* aWords[], i8 iWords)
{
  bool bResult;
  
  // execute command
  bResult = pSEitf->bDecode(aWords, iWords);
  
  return bResult;
}


/*----------------------------------------------------------------------------*/
/* get response                                                               */
/*----------------------------------------------------------------------------*/
eEC eGetResponse(SEitf* pSEitf)
{
  // get response
  eEC eError = pSEitf->eGetResponse();
  //vTrace("rsp : %s %lld\n"_Str, pSEitf->m_sResponse.s(), eError);
  
  return eError;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
