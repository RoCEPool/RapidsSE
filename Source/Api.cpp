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
    VALIDATE_SEITF();
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // validate parameters
    VALIDATE_PARAM("a"_Str, pColumnName);
    VALIDATE_PARAM("b"_Str, pColumnType);
    
    Table* pTable = pSEitf->pCurrentTable();
    
    VALIDATE_TABLE("c"_Str, pTable);

    // add a column
    eResult = eExecuteRequest(pSEitf, "add"_Str,
                              (Str*)pTable->aTableName(),
                              (Str*)pColumnName,
                              (Str*)pColumnType);
    
    CHECK_RESULT("d"_Str);
    CHECK_RESPONSE("e"_Str);
    
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
    VALIDATE_SEITF();
    
    // clear the last error
    pSEitf->vClearLastError();
  
    // validate parameters
    VALIDATE_PARAM("a"_Str, pCatalog);
    VALIDATE_PARAM("b"_Str, pSchema);
    VALIDATE_PARAM("c"_Str, pTable);
    
    // change folder to table
    CF_TABLE("d"_Str);
    
    // get a reference to the table
    eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
    CHECK_RESULT("d"_Str);
    CHECK_RESPONSE("e"_Str);
    
    // begin a tnx
    bool bResult = pSEitf->bBeginTx();
    
    if (!bResult) {
      pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
      TRACE_ERROR("f",EC_BEGIN_TX_FAILED);
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
      
      vTrace(">RapidsSE_CheckLibraryVersion: RapidsDB at version %s does not match RapidsSE library version %s"_Str,
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
    VALIDATE_SEITF();
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // validate parameters
    VALIDATE_PARAM("a"_Str, pCatalog);
    VALIDATE_PARAM("b"_Str, pSchema);
    VALIDATE_PARAM("c"_Str, pTable);
    
    // change folder to tables
    CF_TABLE("d"_Str);

    // create a table
    eResult = eExecuteRequest(pSEitf, "mktbl"_Str, (Str*)pTable);
    CHECK_RESULT("e"_Str);
    CHECK_RESPONSE("f"_Str);
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
    CHECK_RESULT("g"_Str);
    CHECK_RESPONSE("h"_Str);
    
    // get pTable
    eEC eGetResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
    if (EC_NONE != eGetResult) {
      
      // remove the table
      eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
      CHECK_RESULT("i"_Str);
      CHECK_RESPONSE("j"_Str);
      
      TRACE_ERROR("k"_Str, eGetResult);
      return eGetResult;
    }
    
    // check response
    CHECK_RESPONSE("l"_Str);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_DropTable: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
    
    eEC eResult;
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_DropTable"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pCatalog) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel, ">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
        aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pSchema) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel, ">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
        aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pTable) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel, ">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
        aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    
    eResult = eChangeFolderTables(pSEitf, pCatalog, pSchema, ">RapidsSE_DropTable"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // drop table
    eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
        aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
        aEC[eResult], eResult);
      return eResult;
    }
    
    vTraceExit(iTraceLevel, ">RapidsSE_DropTable: %p\n"_Str, pSEitf);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;

  try {
    
    vTraceEntry(iTraceLevel, "<RapidsSE_EndCreate: %p\n"_Str, pSEitf);
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_EndCreate"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_EndCreate(a): %p %s(%lld)\n"_Str, pSEitf,
        aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_EndCreate(b): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);
    
    vTraceExit(iTraceLevel, ">RapidsSE_EndCreate: %p\n"_Str, pSEitf);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    
    vTraceEntry(iTraceLevel, "<RapidsSE_EndInsert: %p\n"_Str, pSEitf);
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_EndInsert"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    bool bResult = pSEitf->bInsertBuffer();
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      // get response from se
      eEC eResult = eGetResponse(pSEitf);
      
      if (eResult != EC_NONE) {
        pSEitf->vSetLastError(eResult);
        vTraceError(iTraceLevel, ">RapidsSE_EndInsert(a): %p %s(%lld)\n"_Str, pSEitf,
          aEC[eResult], eResult);
        return eResult;
      }
    }
    
    eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_EndInsert(b): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_EndInsert(c): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);
    
    pSEitf->vCommitTx();
    pSEitf->vEndTx();
    
    vTraceExit(iTraceLevel, ">RapidsSE_EndInsert: %p\n"_Str, pSEitf);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    
    vTraceEntry(iTraceLevel, "<RapidsSE_EndScan: %p\n"_Str, pSEitf);
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_EndScan"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (pSEitf->pCurrentTable() == nullptr) {
      vTraceError(iTraceLevel, ">RapidsSE_EndScan(a): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      return EC_NONE;
    }
    
    i8 iAddress = StrLib::iToI8( pSEitf->m_sResponse.s());
    
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    delete pSelCursor;
    
    // return the table
    eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    if (EC_NONE != eResult) {
      
      vTraceError(iTraceLevel, ">RapidsSE_EndScan(b): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_EndScan(c): %p %s(%lld)\n"_Str, pSEitf,
                  aEC[eResult], eResult);
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);
    
    vTraceExit(iTraceLevel, ">RapidsSE_EndScan: %p\n"_Str, pSEitf);
    
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
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalogs, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    
    vTraceEntry(iTraceLevel, "<RapidsSE_GetCatalogs: %p\n"_Str, pSEitf);
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetCatalogs"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (ppCatalogs == nullptr) {
      pSEitf->vSetLastError(EC_NULLPTR);
      vTraceError(iTraceLevel,
        ">RapidsSE_GetCatalogs(a): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NULLPTR], EC_NULLPTR);
    }

    eResult = eChangeFolderCatalogs(pSEitf, ">RapidsSE_GetCatalogs"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // list all catalogs
    eResult = eExecuteRequest(pSEitf, "lscat"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetCatalogs(b): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetCatalogs(c): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // return list of catalogs
    *ppCatalogs = pSEitf->m_sResponse.s();
    
    vTraceExit(iTraceLevel,
      ">RapidsSE_GetCatalogs(a): %p %s(%lld)\n"_Str, pSEitf);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
  if (gpGrid->m_bTraceapi)
    vTraceEntry(iTraceLevel, "<RapidsSE_GetColumns: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
  
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetColumns"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
  
    // clear the last error
    pSEitf->vClearLastError();
    
    // ppColumns pointer should not be null
    if (ppColumns == nullptr) {
      pSEitf->vSetLastError(EC_NULLPTR);
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(s): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NULLPTR], EC_NULLPTR);
      
      return EC_NULLPTR;
    }

    // do we have a catalog ?
    if (!pCatalog) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(s): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      
      return EC_INVALID_PARAMETER;
    }
    
    // do we have a schema ?
    if (!pSchema) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(s): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      
      return EC_INVALID_PARAMETER;
    }
    
    // do we have a table ?
    if (!pTable) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(s): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      
      return EC_INVALID_PARAMETER;
    }

    eResult = eChangeFolderTable(pSEitf, pCatalog, pSchema, pTable, ">RapidsSE_GetColumns"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
  
    // list all the columns of pTable
    eResult = eExecuteRequest(pSEitf, "lscol"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(d): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetColumns(d): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }

    // return the list of columns of pTable
    *ppColumns = pSEitf->m_sResponse.s();
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetColumns: %p %s\n"_Str, pSEitf);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_GetLastError: %p\n"_Str, pSEitf);
    
    *ppError = pSEitf->m_sErrorMsg.s();
    
    vTraceExit(iTraceLevel, "<RapidsSE_GetLastError: %p %s\n"_Str, pSEitf, *ppError);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel,"<RapidsSE_GetNextRow(%lld): %p\n"_Str, pSEitf->iPartition(), pSEitf);
  
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetNextRow"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
  
    // clear the last error
    pSEitf->vClearLastError();
    
    // ppRow pointer should not be null
    if (ppRow == nullptr) {
      pSEitf->vSetLastError(EC_NULLPTR);
      vTraceError(iTraceLevel,
        ">RapidsSE_GetNextRow(a): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NULLPTR], EC_NULLPTR);
      return EC_NULLPTR;
    }

    i8 iAddress = pSEitf->m_sResponse.iToI8();
    
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    
    *ppRow = pSelCursor->aRowAsStr();
    if (!*ppRow) {
      delete pSelCursor;
      
      // return the table
      eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
      if (EC_NONE != eResult) {
        vTraceError(iTraceLevel,
            ">RapidsSE_GetNextRow(b): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
        return eResult;
      }
      
      // get response
      eResult = eGetResponse(pSEitf);
      if (EC_NONE != eResult) {
        vTraceError(iTraceLevel,
          ">RapidsSE_GetNextRow(c): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
        return eResult;
      }
      
      ASSERT(pSEitf->pCurrentTable() == nullptr);
      
      //ASSERT(0);
      
      vTraceExit(iTraceLevel, ">RapidsSE_GetNextRow(aa): %p\n"_Str, pSEitf);
      return EC_NONE;
    }
    
    //vTraceRows(iTraceLevel, "pRapidsSE_GetNextRow(%lld:%p) %s\n"_Str, pSEitf->iPartition(), pSEitf, *ppRow);
    
    // move cursor to next row
    pSelCursor->vNext();
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetNextRow(bb): %p\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel,
      "<RapidsSE_GetPartitions(%lld): %p\n"_Str, pSEitf->iPartition(), pSEitf);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetPartitions"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // ppPartitions pointer should not be null
    if (ppPartitions == nullptr) {
      pSEitf->vSetLastError(EC_NULLPTR);
      vTraceError(iTraceLevel,
        ">RapidsSE_GetPartitions(a): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NULLPTR], EC_NULLPTR);
      return EC_NULLPTR;
    }

    *ppCount = &gpGrid->m_nSlots;
    *ppPartitions = &gpGrid->m_iPartition;
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetPartitions: %p %s\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel,
      "<RapidsSE_GetSchemas(%lld): %p %s\n"_Str, pSEitf->iPartition(), pSEitf, pCatalog);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetSchemas"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // ppSchemas pointer should not be null
    if (ppSchemas == nullptr) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetSchemas(a): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NULLPTR], EC_NULLPTR);
      pSEitf->vSetLastError(EC_NULLPTR);
      return EC_NULLPTR;
    }
    
    if (!pCatalog) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetSchemas(b): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    // change folder to schemas
    eResult = eChangeFolderSchemas(pSEitf, pCatalog, ">RapidsSE_GetSchemas"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // list all schemas in pCatalog
    eResult = eExecuteRequest(pSEitf, "lssch"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetSchemas(c): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetSchemas(d): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // return list of schemas in ppSchemas
    *ppSchemas = pSEitf->m_sResponse.s();
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetSchemas: %p %s\n"_Str, pSEitf);
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
/* return rapidsSE engine's version                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion, i8 iTraceLevel)
{
  eEC eResult;
  eEC eResponse;
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_GetEngineVersion(%lld): %p\n"_Str, pSEitf->iPartition(), pSEitf);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetEngineVersion"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // get the SE version
    eResult = eExecuteRequest(pSEitf, "version"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetEngineVersion(a): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_GetEngineVersion(b): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // return SE version
    *ppVersion = pSEitf->m_sResponse.s();
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetEngineVersion: %p %s\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel,
                "<RapidsSE_GetTables(%lld): %p %s %s\n"_Str, pSEitf->iPartition(), pSEitf, pCatalog, pSchema);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_GetTables"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    // ppTables pointer should not be null
    if (ppTables == nullptr) {
      pSEitf->vSetLastError(EC_NULLPTR);
      return EC_NULLPTR;
    }
    
    if (!pCatalog)
      return EC_INVALID_PARAMETER;
    
    if (!pSchema)
      return EC_INVALID_PARAMETER;
    
    // change folder to tables
    eResult = eChangeFolderTables(pSEitf, pCatalog, pSchema, ">RapidsSE_GetTables"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }
    
    // list all the tables in pSchema of pCatalog
    eResult = eExecuteRequest(pSEitf, "lstbl"_Str);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetTables(f): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_GetTables(f): %p %s(%lld)\n"_Str, pSEitf, aEC[eResult], eResult);
      return eResult;
    }
    
    // return list of tables in ppSchemas
    *ppTables = pSEitf->m_sResponse.s();
    
    vTraceExit(iTraceLevel, ">RapidsSE_GetTables: %p %s\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_HashRowPartition(%lld): %p \n"_Str, pSEitf->iPartition(), pSEitf);
    
    //vTraceRow(iTraceLevel, ">RapidsSE_HashRowPartition(%lld): %p %s\n", _Str, pSEitf->iPartition(), pSEitf, pRow);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_HashRowPartition"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
      
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pRow) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_HashRowPartition(c): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pSEitf->pCurrentTable()) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_HashRowPartition(c): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NO_SUCH_TABLE], EC_NO_SUCH_TABLE);
      pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
      return EC_NO_SUCH_TABLE;
    }
    
    pSEitf->m_iRowPartition = pSEitf->pCurrentTable()->iRowPartition(pRow);
    *ppPartition = &pSEitf->m_iRowPartition;
    
    //vTraceResult(iTraceLevel, ">RapidsSE_HashRowPartition(%lld): %p %lld\n", _Str, pSEitf->iPartition(), pSEitf, *ppPartition);
    
    vTraceExit(iTraceLevel, ">RapidsSE_HashRowPartition: %p %s\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    
    vTraceEntry(iTraceLevel, "<RapidsSE_InsertRow(%lld): %p %s %s\n"_Str, pSEitf->iPartition());
    //vTraceRow(iTraceLevel, ">RapidsSE_InsertRow(%lld): %p %s\n", _Str, pSEitf->iPartition(), pSEitf, pRow);
    
    // validate the SEitf
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_InsertRow"_Str, iTraceLevel);
    if (eResult != EC_NONE) {
      return eResult;
    }

    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pRow) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_InsertRow(a): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }

    if (!pSEitf->pCurrentTable()) {
      vTraceError(iTraceLevel,
                  ">RapidsSE_InsertRow(b): %p %s(%lld)\n"_Str, pSEitf, aEC[EC_NO_SUCH_TABLE], EC_NO_SUCH_TABLE);
      pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
      return EC_NO_SUCH_TABLE;
    }
    
    bool bResult = pSEitf->bInsertRow(pRow);
    
    if (!bResult) {
      pSEitf->vSetLastError(pSEitf->m_eErrorCode);
      vTraceError(iTraceLevel,
        ">RapidsSE_InsertRow(c): %p %s(%lld)\n"_Str, pSEitf, aEC[pSEitf->m_eErrorCode], pSEitf->m_eErrorCode);
      return pSEitf->m_eErrorCode;
    }
    
    vTraceExit(iTraceLevel, ">RapidsSE_InsertRow: %p %s\n"_Str, pSEitf);
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    vTraceEntry(iTraceLevel,
      "<RapidsSE_PoolOpen %s %x %x\n"_Str, pInstanceName, ppInstance, *ppInstance);
    
    // open the pool
    s256 sPoolName;
    
    sPoolName = (Str*)pInstanceName;
    bool bResult = RapidsSE::bPoolOpen(sPoolName);
    
    if (!bResult) {
      vTraceError(iTraceLevel,
        ">RapidsSE_PoolOpen %x %x\n"_Str, *ppInstance, aEC[EC_INVALID_INSTANCE]);
      return EC_INVALID_INSTANCE;
    }
    
    *ppInstance = gpAttachMap;
    
    vTraceExit(iTraceLevel,
      ">RapidsSE_PoolOpen %x %x\n"_Str, *ppInstance, gpAttachMap);
    
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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p %s %s %s %lld\n"_Str,
           pSEitf, pCatalog, pSchema, pTable, pSEitf->iPartition());
    
    // pSEitf cannot be null
    if (pSEitf == nullptr) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(a): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_INTERFACE]);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be a pool address
    if (!gpBufferPool->bValidAddress(pSEitf)) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(b): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_INTERFACE]);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be an SEitf class
    if (pSEitf->eClass() != CN_SEITF) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(c): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_INTERFACE]);
      return EC_INVALID_INTERFACE;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pCatalog) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(d): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_PARAMETER]);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pSchema) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(e): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_PARAMETER]);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pTable) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(f): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[EC_INVALID_PARAMETER]);
      return EC_INVALID_PARAMETER;
    }
    
    // change directory to ~
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(g): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }

    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(h): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }

    // change directory to boray
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(i): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(j): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // change directory to catalogs
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(k): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(l): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // change directory to pCatalog
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(m): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(n): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // change directory to schemas
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(o): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(p): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // change directory to pSchema
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(q): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(r): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(s): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(t): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }

    // change directory to the table
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(u): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(v): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    
    
    
    // select * from pTable
    eResult = eExecuteRequest(pSEitf, "pwf"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(--): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(++): %p %lld %s(%lld)\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult], eResult);
      return eResult;
    }

    
    vTrace("pwf: %s\n"_Str, pSEitf->m_sResponse.aStr());
    
    
    // select * from pTable
    eResult = eExecuteRequest(pSEitf, "sel"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(w): %p %lld %s\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_ScanTable(x): %p %lld %s(%lld)\n"_Str, pSEitf,
               pSEitf->iPartition(), aEC[eResult], eResult);
      return eResult;
    }

    ASSERT(pSEitf->pCurrentTable() != nullptr);
    
    i8 iAddress = pSEitf->m_sResponse.iToI8();
    
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());

    //vTrace("pSEitf: %p pSelCursor: %p Slot: %lld\n"_Str, pSEitf, pSelCursor,
    //       Partition::iPartitionToSlot(pSEitf->iPartition()));
    
    pSelCursor->vBegin(Partition::iPartitionToSlot(pSEitf->iPartition()));
    
    pSelCursor->vFirst();

    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_ScanTable(y): %p %lld %s\n"_Str, pSEitf,
             pSEitf->iPartition(), aEC[eResult]);

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
eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf, i8 iTraceLevel)
{
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_SEitfDelete: %p\n"_Str, pSEitf);
    
    // clear the last error
    pSEitf->vClearLastError();
    
    RapidsSE::vSEitfDelete(pSEitf);
    
    vTraceExit(iTraceLevel, ">RapidsSE_SEitfDelete(a): %p\n"_Str, pSEitf);

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
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_SEitfInvalid: iError %lld\n"_Str, iError);
    
    if (iError == EC_INVALID_INTERFACE) {
      vTraceExit(iTraceLevel, ">RapidsSE_SEitfInvalid(a): true\n"_Str);
      return true;
    }
    else {
      vTraceExit(iTraceLevel, ">RapidsSE_SEitfInvalid(b): false\n"_Str);
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
eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition, i8 iTraceLevel)
{
  
  Str* aApiCall = "RapidsSE_AddColumn"_Str;
  SEitf* pSEitf = nullptr;
  
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_SEitfNew: Partition %lld\n"_Str, iPartition);
    
    SEitf* pSEitf = new SEitf(iPartition);
    pSEitf->vInit();
    
    // return the newly created SEitf
    *ppSEitf = pSEitf;
    
    vTraceExit(iTraceLevel, ">RapidsSE_SEitfNew(a): %p\n"_Str, pSEitf);
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
