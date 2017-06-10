/*============================================================================*/
/* Api.h - RapidsSE Api                                                       */
/*============================================================================*/
#pragma once

// API trace levels
#define TL_ENTRY  1
#define TL_ERROR  2
#define TL_EXIT   4
#define TL_POOL   8

// external SE API calls
extern "C" eEC RapidsSE_AddIndexColumn(SEitf* pSEitf, Str* pTableName, Str* pIndexName, Str* pColumnName, i8 iTraceLevel);
extern "C" eEC RapidsSE_AddTableColumn(SEitf* pSEitf, Str* pTableName, Str* pColumnName, Str* pColumnType,
                                       i8 iTraceLevel);
extern "C" eEC RapidsSE_BeginInsert(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel);
extern "C" eEC RapidsSE_CheckLibraryVersion(Str* pVersion, i8 iTraceLevel);
extern "C" eEC RapidsSE_CreateIndex(SEitf* seitf, Str* catalog, Str* pSchema, Str* table, Str* pIndex, bool bIsUnique, i8 iTraceLevel);
extern "C" eEC RapidsSE_CreateTable(SEitf* seitf, Str* catalog, Str* pSchema, Str* table, i8 iTraceLevel);
extern "C" eEC RapidsSE_DropTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, i8 iTraceLevel);
extern "C" eEC RapidsSE_EndCreateIndex(SEitf* pSEitf, Str* pTable, Str* pIndexi8, i8 iTraceLevel);
extern "C" eEC RapidsSE_EndCreateTable(SEitf* pSEitf, Str* pTable, i8 iTraceLevel);
extern "C" eEC RapidsSE_EndInsert(SEitf* pSEitf, i8 iTraceLevel);
extern "C" eEC RapidsSE_EndScan(SEitf* pSEitf, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalog, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetColumns(SEitf* pSEitf, Str* pCatalog,  Str* pSchema,
                                   Str* pTable, void** ppColumns, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetIndexes(SEitf* pSEitf, Str* pCatalog,  Str* pSchema,
                                   Str* pTable, void** ppIndexes, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetIndexColumns(SEitf* pSEitf, Str* pCatalog,  Str* pSchema,
                                        Str* pTable, Str* pIndex, void** ppColumns, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppParitions, void** ppCount, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetSchemas(SEitf* pSEitf, Str* pCatalog, void** ppSchema, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion, i8 iTraceLevel);
extern "C" eEC RapidsSE_GetTables(SEitf* pSEitf, Str* pCatalog, Str* pSchema, void** ppTables, i8 iTraceLevel);
extern "C" eEC RapidsSE_HashRowPartition(SEitf* pSEitf, Str* pRow, void** ppPartition, i8 iTraceLevel);
extern "C" eEC RapidsSE_InsertRow(SEitf* pSEitf, Str* pRow, i8 iTraceLevel);
extern "C" eEC RapidsSE_PoolOpen(Str* pInstanceName, void** ppInstance, i8 iTraceLevel);
extern "C" eEC RapidsSE_ScanTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, Str* pRange, i8 iTraceLevel);
extern "C" eEC RapidsSE_SEitfDelete(SEitf* pSEitf, i8 iTraceLevel);
extern "C" eEC RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition, i8 iTraceLevel);
extern "C" bool RapidsSE_SEitfInvalid(int eError, i8 iTraceLevel);

bool RapidsSE_ExecuteRequest(SEitf* pSEitf,TokenSet& xTokenSet, Str** ppResponse);

// private calls
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand);
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter);
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter1,
                    Str* aParameter2);
eEC eExecuteRequest(SEitf* pSEitf, Str* aCommand, Str* aParameter1,
                    Str* aParameter2, Str* aParameter3);

bool bExecuteRequest(SEitf* pSEitf, TokenSet& xTokenSet);
eEC eGetResponse(SEitf* pSEitf);

/*----------------------------------------------------------------------------*/
/* get table                                                                  */
/*----------------------------------------------------------------------------*/
#define GET_TABLE(aMarker) \
{  \
  eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable); \
}

/*----------------------------------------------------------------------------*/
/* ret table                                                                  */
/*----------------------------------------------------------------------------*/
#define RETURN_TABLE(aMarker) \
{  \
  eResult = eExecuteRequest(pSEitf, "rettbl"_Str, (Str*)pTable); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P0() \
  {  \
    if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
      vTrace("<%s: %p(%lld)\n"_Str,  \
        aApiCall, pSEitf, pSEitf ? pSEitf->iPartition() : -1); \
  }

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P1(p1) \
  {  \
    if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
      vTrace("<%s: %p(%lld) %s\n"_Str,  \
        aApiCall, pSEitf, pSEitf ? pSEitf->iPartition() : -1, p1); \
  }

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P2(p1, p2) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s\n"_Str,  \
      aApiCall, pSEitf, pSEitf ? pSEitf->iPartition() : -1, p1, p2); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P3(p1, p2, p3) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s\n"_Str,  \
      aApiCall, pSEitf, pSEitf ? pSEitf->iPartition() : -1, p1, p2, p3); \
}

/*----------------------------------------------------------------------------*/
/* trace API entry                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ENTRY_P4(p1, p2, p3, p4) \
{  \
  if ( (iTraceLevel & TL_ENTRY) == TL_ENTRY) \
    vTrace("<%s: %p(%lld) %s %s %s %s\n"_Str,  \
      aApiCall, pSEitf, pSEitf ? pSEitf->iPartition() : -1, p1, p2, p3, p4); \
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
#define TRACE_ERROR(aMarker, eErrorCode) \
  {  \
    if ( (iTraceLevel & TL_ERROR) == TL_ERROR) \
      vTrace(">%s(%s): %p(%lld) %s(%lld)\n"_Str,  \
        aApiCall, aMarker, pSEitf, pSEitf ? pSEitf->iPartition() : -1, aEC[eErrorCode], (i8)eErrorCode); \
  }

/*----------------------------------------------------------------------------*/
/* trace API exit                                                             */
/*----------------------------------------------------------------------------*/
#define TRACE_EXIT(aMarker) \
  {  \
    if ( (iTraceLevel & TL_EXIT) == TL_EXIT) \
      vTrace(">%s(%s): %p(%lld)\n"_Str,  \
        aApiCall, aMarker, pSEitf, pSEitf ? pSEitf->iPartition() : -1); \
  }

/*----------------------------------------------------------------------------*/
/* validate a parameter                                                       */
/*----------------------------------------------------------------------------*/
#define VALIDATE_PARAM(aMarker, pParameter) \
  {  \
    if ( ! pParameter ) { \
      TRACE_ERROR(aMarker, EC_INVALID_PARAMETER); \
      pSEitf->vSetLastError(EC_INVALID_PARAMETER); \
      return EC_INVALID_PARAMETER; \
     } \
  }

/*----------------------------------------------------------------------------*/
/* validate reference pointer                                                 */
/*----------------------------------------------------------------------------*/
#define VALIDATE_REFPOINTER(aMarker, ppRefPointer) \
  { \
    if ( ! ppRefPointer) { \
      TRACE_ERROR(aMarker, EC_INVALID_PARAMETER); \
      pSEitf->vSetLastError(EC_INVALID_PARAMETER); \
      return EC_INVALID_PARAMETER; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* validate table                                                             */
/*----------------------------------------------------------------------------*/
#define VALIDATE_TABLE(aMarker) \
  {  \
    if ( ! pSEitf->pCurrentTable() ) { \
      TRACE_ERROR(aMarker, EC_NO_SUCH_TABLE); \
      pSEitf->vSetLastError(EC_NO_SUCH_TABLE); \
      return EC_NO_SUCH_TABLE; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* validate no table                                                          */
/*----------------------------------------------------------------------------*/
#define VALIDATE_NOTABLE(aMarker) \
  {  \
    if ( pSEitf->pCurrentTable() ) { \
      TRACE_ERROR(aMarker, EC_VALIDATION_FAILURE); \
      pSEitf->vSetLastError(EC_VALIDATION_FAILURE); \
      return EC_NO_SUCH_TABLE; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* validate SEitf                                                             */
/*----------------------------------------------------------------------------*/
#define VALIDATE_SEITF(aMarker) \
  { \
    eResult = eValidateSEitf(pSEitf, aApiCall, iTraceLevel); \
    if (eResult != EC_NONE) { \
        TRACE_ERROR(aMarker, EC_INVALID_INTERFACE); \
        return eResult; \
     } \
  }

/*----------------------------------------------------------------------------*/
/* check result of a request that was sent                                    */
/*----------------------------------------------------------------------------*/
#define CHECK_RESULT(aMarker) \
  { \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* check response of a request that was executed                              */
/*----------------------------------------------------------------------------*/
#define CHECK_RESPONSE(aMarker) \
 { \
   eResult = eGetResponse(pSEitf); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
 }

/*----------------------------------------------------------------------------*/
/* change folder to schemas                                                   */
/*----------------------------------------------------------------------------*/
#define CF_SCHEMAS(aMarker) \
  { \
    eResult = eChangeFolderSchemas(pSEitf, pCatalog, aApiCall, iTraceLevel); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* change folder to catalogs                                                  */
/*----------------------------------------------------------------------------*/
#define CF_CATALOGS(aMarker) \
  { \
    eResult = eChangeFolderCatalogs(pSEitf, aApiCall, iTraceLevel); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* change folder to indexes                                                   */
/*----------------------------------------------------------------------------*/
#define CF_INDEXES(aMarker) \
  { \
    eResult = eChangeFolderIndexes(pSEitf, pCatalog, pSchema, pTable, aApiCall, iTraceLevel); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* change folder to tables                                                    */
/*----------------------------------------------------------------------------*/
#define CF_TABLES(aMarker) \
  { \
    eResult = eChangeFolderTables(pSEitf, pCatalog, pSchema, aApiCall, iTraceLevel); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* change folder to a table                                                   */
/*----------------------------------------------------------------------------*/
#define CF_TABLE(aMarker) \
  { \
    eResult = eChangeFolderTable(pSEitf, pCatalog, pSchema, pTable, aApiCall, iTraceLevel); \
    if (EC_NONE != eResult) { \
      TRACE_ERROR(aMarker,eResult); \
      return eResult; \
    } \
  }

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
#define CLEAR_LASTERROR()  pSEitf->vClearLastError();

/*----------------------------------------------------------------------------*/
/* verify buffer pool boundaries                                              */
/*----------------------------------------------------------------------------*/
#define VALIDATE_POOL() \
  {  \
    bool bPoolValid; \
    if ( (iTraceLevel & TL_POOL) == TL_POOL) { \
      bPoolValid = gpBufferPool->bVerifyBoundaries(true); \
    } \
    else { \
      bPoolValid = gpBufferPool->bVerifyBoundaries(false); \
    } \
    if ( ! bPoolValid )  { \
      vTrace("Pool base and limit are not valid!!!"    \
             "JAVA MAY HAVE RE_MAPPED MEMORY SEGMENTS!!!\n"_Str);  \
      return EC_VALIDATION_FAILURE; \
    } \
  }


/*----------------------------------------------------------------------------*/
/* trace API call entry                                                       */
/*----------------------------------------------------------------------------*/
inline void vTraceEntry(i8 iTraceLevel, Str* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_ENTRY) != TL_ENTRY)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API call exit                                                        */
/*----------------------------------------------------------------------------*/
inline void vTraceExit(i8 iTraceLevel, Str* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_EXIT) != TL_EXIT)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* trace API error                                                            */
/*----------------------------------------------------------------------------*/
inline void vTraceError(i8 iTraceLevel, Str* aFormat, ...)
{
  // trace entry ?
  if ( (iTraceLevel & TL_ERROR) != TL_ERROR)
    return;
  
  va_list xArgs;
  
  va_start(xArgs,aFormat);
  
  vReport(C_REPORT_INFO,aFormat,xArgs);
  
  va_end(xArgs);
}

/*----------------------------------------------------------------------------*/
/* validate the SEitf                                                         */
/*----------------------------------------------------------------------------*/
inline eEC eValidateSEitf(SEitf* pSEitf, Str* aApiCall, i8 iTraceLevel) {
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    TRACE_ERROR("va", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    TRACE_ERROR("vb", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    TRACE_ERROR("vc", EC_INVALID_INTERFACE);
    return EC_INVALID_INTERFACE;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* change folder to catalogs                                                   */
/*----------------------------------------------------------------------------*/
inline eEC eChangeFolderCatalogs(SEitf* pSEitf, Str* aApiCall, i8 iTraceLevel)
{
  eEC eResult;
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fa): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fb): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fc): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fd): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fe): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(ff): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* change folder to schemas                                                   */
/*----------------------------------------------------------------------------*/
inline eEC eChangeFolderSchemas(SEitf* pSEitf, Str* pCatalog, Str* aApiCall, i8 iTraceLevel)
{
  eEC eResult;
  
  eResult = eChangeFolderCatalogs(pSEitf, aApiCall, iTraceLevel);
  if (eResult != EC_NONE)
    return eResult;
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fg): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fh): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fi): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fj): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* change folder to a table                                                   */
/*----------------------------------------------------------------------------*/
inline eEC eChangeFolderTables(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* aApiCall, i8 iTraceLevel)
{
  eEC eResult;
  
  eResult = eChangeFolderSchemas(pSEitf, pCatalog, aApiCall, iTraceLevel);
  if (eResult != EC_NONE)
    return eResult;
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fk): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fl): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fm): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fn): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* change folder to tables                                                   */
/*----------------------------------------------------------------------------*/
inline eEC eChangeFolderTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, Str* aApiCall, i8 iTraceLevel)
{
  eEC eResult;
  
  eResult = eChangeFolderTables(pSEitf, pCatalog, pSchema, aApiCall, iTraceLevel);
  if (eResult != EC_NONE) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fo): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  return EC_NONE;
  
}

/*----------------------------------------------------------------------------*/
/* change folder to indexes                                                   */
/*----------------------------------------------------------------------------*/
inline eEC eChangeFolderIndexes(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable, Str* aApiCall, i8 iTraceLevel)
{
  eEC eResult;
  
  eResult = eChangeFolderTable(pSEitf, pCatalog, pSchema, pTable, aApiCall, iTraceLevel);
  if (eResult != EC_NONE)
  return eResult;
  
  // change directory to indexes
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "indexes"_Str);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fm): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    vTraceError(iTraceLevel, "%s(fn): %p %s(%lld)\n"_Str, pSEitf, aApiCall, aEC[eResult], eResult);
    return eResult;
  }
  
  return EC_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
