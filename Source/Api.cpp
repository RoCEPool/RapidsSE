/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Api.hpp"

extern BufferPool* gpBufferPool;
extern AttachMap*  gpAttachMap;
extern Grid*       gpGrid;

/*----------------------------------------------------------------------------*/
/* add a column to a table                                                    */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_AddColumn(SEitf* pSEitf, char* pColumnName, char* pColumnType)
{
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_AddColumn: %p %s %s\n", pSEitf, pColumnName, pColumnType);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pColumnName) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pColumnType) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }

  Table* pTable = pSEitf->pCurrentTable();
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return EC_NO_SUCH_TABLE;
  }
  
  //if (gpGrid->m_bTraceapi)
  //  vTrace("Table: %s\n", pTable->pTableName());

  // add a column
  eResult = eExecuteRequest(pSEitf, "add", pTable->pTableName(), pColumnName, pColumnType);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n %s", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
    return eResult;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_AddColumn: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_BeginInsert(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  eEC eResult;
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_BeginInsert: %p\n", pSEitf);

  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl", pTable);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return eResult;
  }
  
  bool bResult = pSEitf->bBeginTx();
  
  if (!bResult) {
    pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
    return EC_BEGIN_TX_FAILED;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_BeginInsert: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* check the library version against version passed                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CheckLibraryVersion(char* pVersion)
{
  vTrace("<RapidsSE_CheckLibraryVersion Library version %s; versionString %s\n", RAPIDSSE_VERSION_STRING, pVersion);
  
  if (!String::bEqualTo(pVersion, RAPIDSSE_VERSION_STRING)) {
    
    vTrace("RapidsDB at version %s does not match RapidsSE library version %s",
           pVersion, RAPIDSSE_VERSION_STRING);
    
    vTrace(">RapidsSE_CheckLibraryVersion\n");
    
    return EC_VERSION_MISMATCH;
  }
  
  vTrace(">RapidsSE_CheckLibraryVersion\n");
           
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_CreateTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_CreateTable: %p %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
         
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }

  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }

  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // create a table
  eResult = eExecuteRequest(pSEitf, "mktbl", pTable);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get pTable
  eResult = eExecuteRequest(pSEitf, "gettbl", pTable);
  if (EC_NONE != eResult) {
    vTrace("RapidsSE_CreateTable: gettbl %s FAILED!!!\n", pTable);
    
    // remove the table
    eEC eRemoveResult = eExecuteRequest(pSEitf, "rmtbl", pTable);
    if (EC_NONE != eRemoveResult) {
      if (gpGrid->m_bTraceapi)
        vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
      return eRemoveResult;
    }
    
    // get response
    eRemoveResult = eGetResponse(pSEitf);
    if (EC_NONE != eRemoveResult) {
      if (gpGrid->m_bTraceapi)
        vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
      return eRemoveResult;
    }
    
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
    return eResult;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_CreateTable: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DropTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_DropTable: %p %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // drop table
  eResult = eExecuteRequest(pSEitf, "rmtbl", pTable);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
    return eResult;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_DropTable: %p \n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* end create                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndCreate(SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_EndCreate: %p\n", pSEitf);
  
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndCreate: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndCreate: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl");
  if (EC_NONE != eResult) {
    
    vTrace("RapidsSE_EndCreate: rettbl FAILED!!!\n");
    
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndCreate: %p\n", pSEitf);

    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndCreate: %p\n", pSEitf);
    return eResult;
  }
  
  ASSERT(pSEitf->pCurrentTable() == nullptr);
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_EndCreate: %p\n", pSEitf);
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndInsert(SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_EndInsert: %p\n", pSEitf);
  
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
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
      if (gpGrid->m_bTraceapi)
        vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
      pSEitf->vSetLastError(eResult);
      return eResult;
    }
  }
  
  eEC eResult = eExecuteRequest(pSEitf, "rettbl");
  if (EC_NONE != eResult) {
    
    vTrace("RapidsSE_EndInsert : rettbl FAILED!!!\n");

    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
        return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
    return eResult;
  }
  
  ASSERT(pSEitf->pCurrentTable() == nullptr);
  
  pSEitf->vCommitTx();
  pSEitf->vEndTx();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_EndInsert: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* end scan                                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndScan(SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_EndScan: %p\n", pSEitf);
  
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndScan: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndScan: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  i8 iAddress = String::iToI8( pSEitf->m_sResponse.a());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  delete pSelCursor;
  
  // return the table
  eEC eResult = eExecuteRequest(pSEitf, "rettbl");
  if (EC_NONE != eResult) {
    
    vTrace("RapidsSE_EndScan: rettbl FAILED!!!\n");
    
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndScan: %p\n", pSEitf);
    
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndScan: %p\n", pSEitf);
    return eResult;
  }
  
  ASSERT(pSEitf->pCurrentTable() == nullptr);
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_EndScan: %p\n", pSEitf);
  
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
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetCatalogs: %p\n", pSEitf);
  
  eEC eResult;
  
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (ppCatalogs == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    pSEitf->vSetLastError(EC_NULLPTR);
    return EC_NULLPTR;
  }

  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // list all catalogs
  eResult = eExecuteRequest(pSEitf, "lscat");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
    return eResult;
  }
  
  // return list of catalogs
  *ppCatalogs = pSEitf->m_sResponse.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetCatalogs: %p\n", pSEitf);
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog, char* pSchema,
                        char* pTable, void** ppColumns)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetColumns: %p %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppColumns pointer should not be null
  if (ppColumns == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_NULLPTR;
  }

  // do we have a catalog ?
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // do we have a schema ?
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // do we have a table ?
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }

  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // list all the columns of pTable
  eResult = eExecuteRequest(pSEitf, "lscol", pTable);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
    return eResult;
  }

  // return the list of columns of pTable
  *ppColumns = pSEitf->m_sResponse.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetColumns: %p\n", pSEitf);
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetLastError: %p\n", pSEitf);
  
  *ppError = pSEitf->m_sErrorMsg.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("Error : %s\n", *ppError);
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetLastError: %p\n", pSEitf);

  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetNextRow: %p (%lld)\n", pSEitf, pSEitf->iPartition());
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppRow pointer should not be null
  if (ppRow == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
    return EC_NULLPTR;
  }

  i8 iAddress = String::iToI8( pSEitf->m_sResponse.a());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  *ppRow = pSelCursor->pRowAsStr();
  if (!*ppRow) {
    /*
    delete pSelCursor;
    
    // return the table
    eEC eResult = eExecuteRequest(pSEitf, "rettbl");
    if (EC_NONE != eResult) {
      
      vTrace("RapidsSE_GetNextRow : rettbl FAILED!!!\n");
      
      if (gpGrid->m_bTraceapi)
        vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);
    */
    
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
    
    return EC_NONE;
  }
  
  // move cursor to next row
  pSelCursor->vNext();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetNextRow: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions, void** ppCount)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetParitions: %p\n", pSEitf);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetParitions: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetParitions: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetParitions: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppPartitions pointer should not be null
  if (ppPartitions == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetParitions: %p\n", pSEitf);
    return EC_NULLPTR;
  }

  *ppCount = &gpGrid->m_nSlots;
  *ppPartitions = &gpGrid->m_iPartition;
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetParitions: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchemas)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppSchemas pointer should not be null
  if (ppSchemas == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return EC_NULLPTR;
  }
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // list all schemas in pCatalog
  eResult = eExecuteRequest(pSEitf, "lssch");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
    return eResult;
  }
  
  // return list of schemas in ppSchemas
  *ppSchemas = pSEitf->m_sResponse.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetSchemas: %p %s\n", pSEitf, pCatalog);
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* return rapidsSE engine's version                                           */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetEngineVersion(SEitf* pSEitf, void** ppVersion)
{
  eEC eResult;
  
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetEngineVersion: %p %p\n", pSEitf, ppVersion);
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "version");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
    return eResult;
  }
  
  // return SE version
  *ppVersion = pSEitf->m_sResponse.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetEngineVersion: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_GetTables: %p %s %s\n", pSEitf, pCatalog, pSchema);
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // ppTables pointer should not be null
  if (ppTables == nullptr) {
    pSEitf->vSetLastError(EC_NULLPTR);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_NULLPTR;
  }
  
  if (!pCatalog) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
 
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }

  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }

  // list all the tables in pSchema of pCatalog
  eResult = eExecuteRequest(pSEitf, "lstbl");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
    return eResult;
  }
  
  // return list of tables in ppSchemas
  *ppTables = pSEitf->m_sResponse.a();
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_GetTables: %p\n", pSEitf);
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_HashRowPartition(SEitf* pSEitf, char* pRow, void** ppPartition)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_HashRowPartition: %p (%lld)\n", pSEitf, pSEitf->iPartition());
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pRow) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSEitf->pCurrentTable()) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
    return EC_NO_SUCH_TABLE;
  }
  
  pSEitf->m_iRowPartition = pSEitf->pCurrentTable()->iRowPartition(pRow);
  *ppPartition = &pSEitf->m_iRowPartition;
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_HashRowPartition: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* insert the given row into a table                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_InsertRow(SEitf* pSEitf, char* pRow)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_InsertRow: %p (%lld)\n", pSEitf, pSEitf->iPartition());
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }

  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pRow) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }

  if (!pSEitf->pCurrentTable()) {
    pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return EC_NO_SUCH_TABLE;
  }
  
  bool bResult = pSEitf->bInsertRow(pRow);
  
  if (!bResult) {
    pSEitf->vSetLastError(pSEitf->m_eErrorCode);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
    return pSEitf->m_eErrorCode;
  }
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_InsertRow: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(char* pInstanceName, void** ppInstance)
{
  vTrace(">RapidsSE_PoolOpen %s %x %x\n", pInstanceName, ppInstance, *ppInstance);
  
  // open the pool
  s256 sPoolName;
  
  sPoolName = pInstanceName;
  bool bResult = RapidsSE::bPoolOpen(sPoolName);
  
  if (!bResult) {
    vTrace("<RapidsSE_PoolOpen\n");
    return EC_INVALID_INSTANCE;
  }
  
  vTrace("<RapidsSE_PoolOpen\n");
  *ppInstance = gpAttachMap;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* scan table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_ScanTable: %p %s %s %s (%lld)\n",
         pSEitf, pCatalog, pSchema, pTable, pSEitf->iPartition());
  
  eEC eResult;
  
  // pSEitf cannot be null
  if (pSEitf == nullptr) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be a pool address
  if (!gpBufferPool->bValidAddress(pSEitf)) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // pSEitf should be an SEitf class
  if (pSEitf->eClass() != CN_SEITF) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_INTERFACE;
  }
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pSchema) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  if (!pTable) {
    pSEitf->vSetLastError(EC_INVALID_PARAMETER);
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return EC_INVALID_PARAMETER;
  }
  
  // change directory to ~
  eResult = eExecuteRequest(pSEitf, "cf", "~");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }

  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }

  // change directory to boray
  eResult = eExecuteRequest(pSEitf, "cf", "boray");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteRequest(pSEitf, "cf", "catalogs");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteRequest(pSEitf, "cf", pCatalog);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteRequest(pSEitf, "cf", "schemas");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteRequest(pSEitf, "cf", pSchema);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf", "tables");
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }

  // select * from pTable
  eResult = eExecuteRequest(pSEitf, "sel", pTable);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (EC_NONE != eResult) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
    return eResult;
  }

  ASSERT(pSEitf->pCurrentTable() != nullptr);
  
  i8 iAddress = String::iToI8(pSEitf->m_sResponse.a());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());

  //vTrace("pSEitf: %p pSelCursor: %p Slot: %lld\n", pSEitf, pSelCursor,
  //       Partition::iPartitionToSlot(pSEitf->iPartition()));
  
  pSelCursor->vBegin(Partition::iPartitionToSlot(pSEitf->iPartition()));
  
  pSelCursor->vFirst();

  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_ScanTable: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_SEitfDelete: %p\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  RapidsSE::vSEitfDelete(pSEitf);
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_SEitfDelete: %p\n", pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SEitfInvalid(int iError)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_SEitfInvalid\n");
  
  if (iError == EC_INVALID_INTERFACE) {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_SEitfInvalid\n");
    return true;
  }
  else {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_SEitfInvalid\n");
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition)
{
  if (gpGrid->m_bTraceapi)
    vTrace(">RapidsSE_SEitfNew: (%lld)\n", iPartition);
  
  SEitf* pSEitf = new SEitf(iPartition);
  pSEitf->vInit();
  
  // return the newly created SEitf
  *ppSEitf = pSEitf;
  
  if (gpGrid->m_bTraceapi)
    vTrace("<RapidsSE_SEitfNew: %p (%lld)\n", pSEitf);
  
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
