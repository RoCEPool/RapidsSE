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
eEC RapidsSE_AddColumn(SEitf* pSEitf, Str* pColumnName, Str* pColumnType)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_AddColumn: %p %s %s\n"_Str, pSEitf, pColumnName, pColumnType);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pColumnName) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pColumnType) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }

  Table* pTable = pSEitf->pCurrentTable();
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return EC_NO_SUCH_TABLE;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace("Table: %s\n"_Str, pTable->aTableName());

  // add a column
  eResult = eExecuteRequest(pSEitf, "add"_Str,
                            (Str*)pTable->aTableName(),
                            (Str*)pColumnName,
                            (Str*)pColumnType);
  if (EC_NONE != eResult) {
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
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginInsert(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  eEC eResult;
  
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_BeginInsert\n"_Str);

  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_BeginInsert : gettbl FAILED!!!\n"_Str);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  bool bResult = pSEitf->bBeginTx();
  
  if (!bResult) {
    pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
    return EC_BEGIN_TX_FAILED;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(Str* pVersion)
{
  vTrace("Library version %s; versionString %s\n"_Str, RAPIDSSE_VERSION_STRING, pVersion);
  
  if ( ! pVersion->bEqual((Str*)RAPIDSSE_VERSION_STRING)) {
    vTrace("RapidsDB at version %s does not match RapidsSE library version %s"_Str,
           pVersion, RAPIDSSE_VERSION_STRING);
    return EC_VERSION_MISMATCH;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_CreateTable: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
         
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // create a table
  eResult = eExecuteRequest(pSEitf, "mktbl"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_CreateTable: gettbl FAILED!!!\n"_Str);
    
    // remove the table
    eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      return eResult;
    }
    
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
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DropTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_DropTable: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // drop table
  eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
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
/* end create                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCreate(SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_EndCreate\n"_Str);
  
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_EndCreate: rettbl FAILED!!!\n"_Str);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  ASSERT(pSEitf->pCurrentTable() == nullptr);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsert(SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_EndInsert\n"_Str);
  
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult = pSEitf->bInsertBuffer();
  
  // if rowBuffer request was appended to requestQ, then wait for resonse
  if (bResult) {
    // get response from se
    eEC eResult = eGetResponse(pSEitf);
    
    if (eResult != EC_NONE) {
      pSEitf->vSetLastError(eResult);
      return eResult;
    }
  }
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_EndInsert : rettbl FAILED!!!\n"_Str);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  ASSERT(pSEitf->pCurrentTable() == nullptr);
  
  pSEitf->vCommitTx();
  pSEitf->vEndTx();
  
  return EC_NONE;
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
eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalogs)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetCatalogs\n"_Str);
  
  eEC eResult;
  
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (ppCatalogs == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    return EC_NULLPTR;
  }

  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all catalogs
  eResult = eExecuteRequest(pSEitf, "lscat"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of catalogs
  *ppCatalogs = pSEitf->m_sResponse.aStr();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumns(SEitf* pSEitf, Str* pCatalog, Str* pSchema,
                        Str* pTable, void** ppColumns)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetColumns: %s %s %s\n"_Str, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppColumns pointer should not be null
  if (ppColumns == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    return EC_NULLPTR;
  }

  // do we have a catalog ?
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // do we have a schema ?
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // do we have a table ?
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }

  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to the table
  eResult = eExecuteRequest(pSEitf, "cf"_Str, pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all the columns of pTable
  eResult = eExecuteRequest(pSEitf, "lscol"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // return the list of columns of pTable
  *ppColumns = pSEitf->m_sResponse.aStr();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetLastError\n"_Str);
  
  *ppError = pSEitf->m_sErrorMsg.aStr();
  
  if (gpGrid->m_bTraceapi)
    vTrace("Error : %s\n"_Str, *ppError);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow)
{
  if (gpGrid->m_bTraceapi)
    vTrace("pRapidsSE_GetNextRow(%lld:%p)\n"_Str, pSEitf->iPartition(), pSEitf);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppRow pointer should not be null
  if (ppRow == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
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
      vTrace("RapidsSE_GetNextRow : rettbl FAILED!!!\n"_Str);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);

    return EC_NONE;
  }
  if (gpGrid->m_bTraceapi)
    vTrace("pRapidsSE_GetNextRow(%lld:%p) %s\n"_Str, pSEitf->iPartition(), pSEitf, *ppRow);
  
  // move cursor to next row
  pSelCursor->vNext();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions, void** ppCount)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetParitions: %p\n"_Str, pSEitf);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppPartitions pointer should not be null
  if (ppPartitions == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    return EC_NULLPTR;
  }

  *ppCount = &gpGrid->m_nSlots;
  *ppPartitions = &gpGrid->m_iPartition;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetSchemas(SEitf* pSEitf, Str* pCatalog, void** ppSchemas)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetSchemas: %s\n"_Str, pCatalog);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppSchemas pointer should not be null
  if (ppSchemas == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    return EC_NULLPTR;
  }
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all schemas in pCatalog
  eResult = eExecuteRequest(pSEitf, "lssch"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of schemas in ppSchemas
  *ppSchemas = pSEitf->m_sResponse.aStr();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* return rapidsSE engine's version                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion)
{
  eEC eResult;
  
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetEngineVersion: %x %x\n"_Str, pSEitf, ppVersion);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "version"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return SE version
  *ppVersion = pSEitf->m_sResponse.aStr();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetTables(SEitf* pSEitf, Str* pCatalog, Str* pSchema, void** ppTables)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetTables: %s %s\n"_Str, pCatalog, pSchema);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
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
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
 
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // list all the tables in pSchema of pCatalog
  eResult = eExecuteRequest(pSEitf, "lstbl"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of tables in ppSchemas
  *ppTables = pSEitf->m_sResponse.aStr();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_HashRowPartition(SEitf* pSEitf, Str* pRow, void** ppPartition)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_InsertRow(%lld:%p) %s\n"_Str, pSEitf->iPartition(), pSEitf, pRow);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pRow) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSEitf->pCurrentTable()) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return EC_NO_SUCH_TABLE;
  }
  
  pSEitf->m_iRowPartition = pSEitf->pCurrentTable()->iRowPartition(pRow);
  *ppPartition = &pSEitf->m_iRowPartition;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, Str* pRow)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_InsertRow(%lld:%p) %s\n"_Str, pSEitf->iPartition(), pSEitf, pRow);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }

  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pRow) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }

  if (!pSEitf->pCurrentTable()) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return EC_NO_SUCH_TABLE;
  }
  
  bool bResult = pSEitf->bInsertRow(pRow);
  
  if (!bResult) {
    pSEitf->vSetLastError(pSEitf->m_eErrorCode);
    return pSEitf->m_eErrorCode;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(Str* pInstanceName, void** ppInstance)
{
  vTrace("vRapidsSE_PoolOpen %s %x %x\n"_Str, pInstanceName, ppInstance, *ppInstance);
  
  // open the pool
  s256 sPoolName;
  
  sPoolName = (Str*)pInstanceName;
  bool bResult = RapidsSE::bPoolOpen(sPoolName);
  
  if (!bResult) {
    return EC_INVALID_INSTANCE;
  }
  
  vTrace("vRapidsSE_PoolOpen %x %x\n"_Str, *ppInstance, gpAttachMap);
  *ppInstance = gpAttachMap;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* scan table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace("pRapidsSE_ScanTable: %p %s %s %s %lld\n"_Str,
         pSEitf, pCatalog, pSchema, pTable, pSEitf->iPartition());
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // select * from pTable
  eResult = eExecuteRequest(pSEitf, "sel"_Str, (Str*)pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
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

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_SEitfDelete: %p\n"_Str, pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  RapidsSE::vSEitfDelete(pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SEitfInvalid(int iError)
{
  
  if (iError == EC_INVALID_INTERFACE) {
    if (gpGrid->m_bTraceapi)
      vTrace("RapidsSE_SEitfInvalid\n"_Str);
    return true;
  }
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_SEitfNew: Partition %lld\n"_Str, iPartition);
  
  SEitf* pSEitf = new SEitf(iPartition);
  pSEitf->vInit();
  
  // return the newly created SEitf
  *ppSEitf = pSEitf;
  
  return EC_NONE;
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
  //vTrace("rsp : %s %lld\n"_Str, pSEitf->m_sResponse.aStr(), eError);
  
  return eError;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
