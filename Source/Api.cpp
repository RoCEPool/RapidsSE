/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Api.hpp"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* Execute a request                                                          */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ExecuteRequest(SEitf* pSEitf, char* pWords[], char** ppResponse)
{
//  vTrace("RapidsSE_ExecuteRequest: ");
//  for (i8 i = 0; i < 8 && *pWords[i]; i++)
//    vTrace("%s ",pWords[i]);
//  vTrace("\n");
  
  eEC eResult = EC_NONE;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // send request to se
  eResult = eExecuteRequest(pSEitf, pWords);
  if (EC_NONE != eResult) {
    *ppResponse = nullptr;
    return eResult;
  }
  
  // get response from se
  eResult = eGetResponse(pSEitf);
  
  // put response into caller
  *ppResponse = pSEitf->m_sResponse.a();
  
  // test for error
  if (EC_NONE != eResult) {
    return eResult;
  }
  
  // return result code
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalogs)
{
  vTrace("RapidsSE_GetCatalogs\n");
  
  eEC eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
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
  
  *ppCatalogs = pSEitf->m_sResponse.a();
  
  // return list of catalogs
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog, char* pSchema,
                        char* pTable, void** ppColumns)
{
  vTrace("RapidsSE_GetColumns: %s %s %s\n", pCatalog, pSchema, pTable);
  
  eEC eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return EC_INVALID_PARAMETER;
  
  if (!pSchema)
    return EC_INVALID_PARAMETER;
  
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
/* get error code                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetErrorCode(SEitf* pSEitf, i8* pError)
{
  //vTrace("RapidsSE_GetErrorCode\n");
  
  *pError = (i8)pSEitf->m_eErrorCode;
  
  //vTrace("Error : %lld\n", *pError);
  return EC_NONE;
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
  //vTrace("pRapidsSE_GetNextRow: 0x%x\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  i8 iAddress = String::iToI8( pSEitf->m_sResponse.a());
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  *ppRow = pRowCursor->pRowAsStr();
  if (!*ppRow) {
    delete pRowCursor;
    return EC_NONE;
  }
  
  // move cursor to next row
  pRowCursor->vNext();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions)
{
  vTrace("RapidsSE_GetParitions: 0x%x\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  *(i8**)(ppPartitions) = &pSEitf->m_iPartitions;
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchemas)
{
  vTrace("RapidsSE_GetSchemas: %s\n", pCatalog);
  
  eEC eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return EC_INVALID_PARAMETER;
  
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
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables)
{
  vTrace("RapidsSE_GetTables: %s %s\n", pCatalog, pSchema);
  
  eEC eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return EC_INVALID_PARAMETER;
  
  if (!pSchema)
    return EC_INVALID_PARAMETER;
  
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
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_PoolOpen(void)
{
  vTrace("vRapidsSE_PoolOpen\n");
  
  // open the pool
  RapidsSE::vPoolOpen();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  vTrace("pRapidsSE_ScanTable: 0x%x %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
  eEC eResult;

  // clear the last error
  pSEitf->vClearLastError();
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return EC_INVALID_PARAMETER;
  
  if (!pSchema)
    return EC_INVALID_PARAMETER;
  
  if (!pTable)
    return EC_INVALID_PARAMETER;
  
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

  i8 iAddress = String::iToI8(pSEitf->m_sResponse.a());
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());

  vTrace("pRowCursor: 0x%x\n", pRowCursor);
  pRowCursor->vBegin(pSEitf->iPartition());
  pRowCursor->vFirst();
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfDelete(SEitf* pSEitf)
{
  vTrace("RapidsSE_SEitfDelete: 0x%x\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  RapidsSE::vSEitfDelete(pSEitf);
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition)
{
  if (gxArguments.m_bVerbose)
    vTrace("RapidsSE_SEitfNew: Partition %lld\n", iPartition);
  
  //SEitf* pSEitf = pSEitf = RapidsSE::pSEitfNew();
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
  
  vTrace("%s %s\n", pCommand, pParameter);
  bResult = pSEitf->bDecode(pWords);
  
  if (!bResult) {
    // set last error
    pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
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
  
  vTrace("cf %s\n", pCommand);
  bResult = pSEitf->bDecode(pWords);
  
  if (!bResult) {
    // set last error
    pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eEC eExecuteRequest(SEitf* pSEitf, char* pWords[])
{
  bool bResult;
  
  // execute command
  bResult = pSEitf->bDecode(pWords);
  
  if (!bResult) {
    // set last error
    pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return EC_INVALID_COMMAND;
  }
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* get response                                                               */
/*----------------------------------------------------------------------------*/
eEC eGetResponse(SEitf* pSEitf)
{
  // get response
  eEC eError = pSEitf->eGetResponse();
//  vTrace("rsp : %lld\n", iResponse);
  
  //if (eError != EC_NONE) {
  //  pSEitf->vSetLastError(EC_COMMAND_FAILED);
  //  return EC_COMMAND_FAILED;
  //}
  
  pSEitf->vSetLastError(eError);
  return eError;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/