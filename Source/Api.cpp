/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Api.hpp"

extern Arguments gxArguments;
extern BufferPool* gpBufferPool;
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add a column to a table                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddColumn(SEitf* pSEitf, char* pColumnName, char* pColumnType)
{
  //vTrace("RapidsSE_AddColumn: 0x%x %s %s\n", pSEitf, pColumnName, pColumnType);
  
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
  
  //vTrace("Table: %s\n", pTable->pTableName());

  // add a column
  eResult = eExecuteRequest(pSEitf, "add", pTable->pTableName(), pColumnName, pColumnType);
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
eEC RapidsSE_BeginInsert(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  eEC eResult;
  
  //vTrace("RapidsSE_BeginInsert\n");

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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl", pTable);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_BeginInsert : gettbl FAILED!!!\n");
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
eEC RapidsSE_CheckLibraryVersion(char* pVersion)
{
  vTrace("Library version %s; versionString %s\n", RAPIDSSE_VERSION_STRING, pVersion);
  
  if (!String::bEqualTo(pVersion, RAPIDSSE_VERSION_STRING)) {
    vTrace("RapidsDB at version %s does not match RapidsSE library version %s",
           pVersion, RAPIDSSE_VERSION_STRING);
    return EC_VERSION_MISMATCH;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  //vTrace("RapidsSE_CreateTable: 0x%x %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
         
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // create a table
  eResult = eExecuteRequest(pSEitf, "mktbl", pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl", pTable);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_CreateTable: gettbl FAILED!!!\n");
    
    // remove the table
    eResult = eExecuteRequest(pSEitf, "rmtbl", pTable);
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
eEC RapidsSE_DropTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  //vTrace("RapidsSE_DropTable: 0x%x %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // drop table
  eResult = eExecuteRequest(pSEitf, "rmtbl", pTable);
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
  //vTrace("RapidsSE_EndCreate\n");
  
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl");
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_EndCreate: rettbl FAILED!!!\n");
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
  //vTrace("RapidsSE_EndInsert\n");
  
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
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl");
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_EndInsert : rettbl FAILED!!!\n");
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
bool RapidsSE_ExecuteRequest(SEitf* pSEitf, char* pWords[], i8 iWords, char** ppResponse)
{
  bool bResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // send request to se
  bResult = bExecuteRequest(pSEitf, pWords, iWords);
  if (!bResult) {
    *ppResponse = nullptr;
    return bResult;
  }
  
  // get response from se
  eEC eResult = eGetResponse(pSEitf);
  
  // put response into caller
  *ppResponse = pSEitf->m_sResponse.a();
  
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
  //vTrace("RapidsSE_GetCatalogs\n");
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all catalogs
  eResult = eExecuteRequest(pSEitf, "lscat");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of catalogs
  *ppCatalogs = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog, char* pSchema,
                        char* pTable, void** ppColumns)
{
  //vTrace("RapidsSE_GetColumns: %s %s %s\n", pCatalog, pSchema, pTable);
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all the columns of pTable
  eResult = eExecuteRequest(pSEitf, "lscol", pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // return the list of columns of pTable
  *ppColumns = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError)
{
  vTrace("RapidsSE_GetLastError\n");
  
  *ppError = pSEitf->m_sErrorMsg.a();
  
  //vTrace("Error : %s\n", *ppError);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow)
{
  //vTrace("pRapidsSE_GetNextRow(%lld:0x%x)\n", pSEitf->iPartition(), pSEitf);
  
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

  i8 iAddress = String::iToI8( pSEitf->m_sResponse.a());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  *ppRow = pSelCursor->pRowAsStr();
  if (!*ppRow) {
    delete pSelCursor;
    
    // return the table
    eEC eResult = eExecuteRequest(pSEitf, "rettbl");
    if (EC_NONE != eResult) {
      vTrace("RapidsSE_GetNextRow : rettbl FAILED!!!\n");
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
  //vTrace("pRapidsSE_GetNextRow(%lld:0x%x) %s\n", pSEitf->iPartition(), pSEitf, *ppRow);
  // move cursor to next row
  pSelCursor->vNext();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions, void** ppCount)
{
  //vTrace("RapidsSE_GetParitions: 0x%x\n", pSEitf);
  
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

  *ppCount = &gpAttachMap->m_iPartitions;
  *ppPartitions = &gpAttachMap->m_iPartition;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchemas)
{
  //vTrace("RapidsSE_GetSchemas: %s\n", pCatalog);
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // list all schemas in pCatalog
  eResult = eExecuteRequest(pSEitf, "lssch");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of schemas in ppSchemas
  *ppSchemas = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* return rapidsSE engine's version                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion)
{
  eEC eResult;
  
  //vTrace("RapidsSE_GetEngineVersion: %x %x\n", pSEitf, ppVersion);
  
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
  eResult = eExecuteRequest(pSEitf, "version");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return SE version
  *ppVersion = pSEitf->m_sResponse.a();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables)
{
  //vTrace("RapidsSE_GetTables: %s %s\n", pCatalog, pSchema);
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
 
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // list all the tables in pSchema of pCatalog
  eResult = eExecuteRequest(pSEitf, "lstbl");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return list of tables in ppSchemas
  *ppTables = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, char* pRow)
{
  //vTrace("RapidsSE_InsertRow\n");
  
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
eEC RapidsSE_PoolOpen(char* pInstanceName, void** ppInstance)
{
  vTrace("vRapidsSE_PoolOpen %s %x %x\n", pInstanceName, ppInstance, *ppInstance);
  
  // open the pool
  s256 sPoolName;
  
  sPoolName = pInstanceName;
  bool bResult = RapidsSE::bPoolOpen(sPoolName);
  
  if (!bResult) {
    return EC_INVALID_INSTANCE;
  }
  
  vTrace("vRapidsSE_PoolOpen %x %x\n", *ppInstance, gpAttachMap);
  *ppInstance = gpAttachMap;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* scan table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  //vTrace("pRapidsSE_ScanTable: 0x%x %s %s %s %lld\n",
  //       pSEitf, pCatalog, pSchema, pTable, pSEitf->iPartition());
  
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
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  // select * from pTable
  eResult = eExecuteRequest(pSEitf, "sel", pTable);
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    return eResult;
  }

  ASSERT(pSEitf->pCurrentTable() != nullptr);
  
  i8 iAddress = String::iToI8(pSEitf->m_sResponse.a());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());

  //vTrace("pSEitf: 0x%x pSelCursor: 0x%x Slot: %lld\n", pSEitf, pSelCursor,
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
  //vTrace("RapidsSE_SEitfDelete: 0x%x\n", pSEitf);
  
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
  //vTrace("RapidsSE_SEitfInvalid: 0x%x\n", pSEitf);
  
  if (iError == EC_INVALID_INTERFACE)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition)
{
  //if (gxArguments.m_bVerbose)
    vTrace("RapidsSE_SEitfNew: Partition %lld\n", iPartition);
  
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
/* execute command                                                          */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand, const char* pParameter1,
                    const char* pParameter2, const char* pParameter3)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  char xArray[C_1KB * 4];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1],
    &xArray[C_1KB * 2],
    &xArray[C_1KB * 3]
  };
  
  // execute command
  String::vCopy(pWords[0], pCommand);
  String::vCopy(pWords[1], pParameter1);
  String::vCopy(pWords[2], pParameter2);
  String::vCopy(pWords[3], pParameter3);
  
  bResult = pSEitf->bDecode(pWords, 4);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                          */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand, const char* pParameter)
{
  // clear the last error
  pSEitf->vClearLastError();
  
  bool bResult;
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };

  // execute command
  String::vCopy(pWords[0], pCommand);
  String::vCopy(pWords[1], pParameter);
  
  bResult = pSEitf->bDecode(pWords, 2);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, const char* pCommand)
{
  bool bResult;
  char xArray[C_1KB * 1];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
  };
  
  // execute command pCommand
  String::vCopy(pWords[0], pCommand);
  
  bResult = pSEitf->bDecode(pWords, 1);
  
  return pSEitf->eGetLastError();
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
bool bExecuteRequest(SEitf* pSEitf, char* pWords[], i8 iWords)
{
  bool bResult;
  
  // execute command
  bResult = pSEitf->bDecode(pWords, iWords);
  
  return bResult;
}


/*----------------------------------------------------------------------------*/
/* get response                                                               */
/*----------------------------------------------------------------------------*/
eEC eGetResponse(SEitf* pSEitf)
{
  // get response
  eEC eError = pSEitf->eGetResponse();
  //vTrace("rsp : %s %lld\n", pSEitf->m_sResponse.a(), eError);
  
  return eError;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
