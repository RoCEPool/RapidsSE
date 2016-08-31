/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Api.hpp"

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_ExecuteCommand(SEitf* pSEitf, char* pWords[], char** ppResponse)
{
//  vTrace("RapidsSE_ExecuteCommand: ");
//  for (i8 i = 0; i < 8 && *pWords[i]; i++)
//    vTrace("%s ",pWords[i]);
//  vTrace("\n");
  
  eIE eResult = IE_NONE;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // send command to se
  eResult = eExecuteCommand(pSEitf, pWords);
  if (IE_NONE != eResult) {
    *ppResponse = nullptr;
    return eResult;
  }
  
  // get response from se
  eResult = eGetResponse(pSEitf);
  
  // put response into caller
  *ppResponse = pSEitf->m_sResponse.a();
  
  // test for error
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // return result code
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetCatalogs(SEitf* pSEitf, void** ppCatalogs)
{
  vTrace("RapidsSE_GetCatalogs\n");
  
  eIE eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  // change directory to tenants
  eResult = eExecuteCommand(pSEitf, "cd", "tenants");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteCommand(pSEitf, "cd", "boray");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteCommand(pSEitf, "cd", "catalogs");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // list all catalogs
  eResult = eExecuteCommand(pSEitf, "lscat");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  *ppCatalogs = pSEitf->m_sResponse.a();
  
  // return list of catalogs
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of columns for given table of given catalog and schema            */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetColumns(SEitf* pSEitf, char* pCatalog, char* pSchema,
                        char* pTable, void** ppColumns)
{
  vTrace("RapidsSE_GetColumns: %s %s %s\n", pCatalog, pSchema, pTable);
  
  eIE eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return IE_INVALID_PARAMETER;
  
  if (!pSchema)
    return IE_INVALID_PARAMETER;
  
  // change directory to tenants
  eResult = eExecuteCommand(pSEitf, "cd", "tenants");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteCommand(pSEitf, "cd", "boray");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteCommand(pSEitf, "cd", "catalogs");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteCommand(pSEitf, "cd", pCatalog);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteCommand(pSEitf, "cd", "schemas");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteCommand(pSEitf, "cd", pSchema);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteCommand(pSEitf, "cd", "tables");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // list all the columns of pTable
  eResult = eExecuteCommand(pSEitf, "lscol", pTable);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }

  // return the list of columns of pTable
  *ppColumns = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get error code                                                             */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetErrorCode(SEitf* pSEitf, i8* pError)
{
  //vTrace("RapidsSE_GetErrorCode\n");
  
  *pError = pSEitf->m_iErrorCode;
  
  //vTrace("Error : %lld\n", *pError);
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetLastError(SEitf* pSEitf, void** ppError)
{
  vTrace("RapidsSE_GetLastError\n");
  
  *ppError = pSEitf->m_sInterfaceError.a();
  
  //vTrace("Error : %s\n", *ppError);
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetNextRow(SEitf* pSEitf, void** ppRow)
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
    return IE_NONE;
  }
  
  // move cursor to next row
  pRowCursor->vNext();
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions                                                   */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetPartitions(SEitf* pSEitf, void** ppPartitions)
{
  vTrace("RapidsSE_GetParitions: 0x%x\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  *(i8**)(ppPartitions) = &pSEitf->m_iPartitions;
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog, void** ppSchemas)
{
  vTrace("RapidsSE_GetSchemas: %s\n", pCatalog);
  
  eIE eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return IE_INVALID_PARAMETER;
  
  // change directory to tenants
  eResult = eExecuteCommand(pSEitf, "cd", "tenants");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteCommand(pSEitf, "cd", "boray");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteCommand(pSEitf, "cd", "catalogs");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteCommand(pSEitf, "cd", pCatalog);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteCommand(pSEitf, "cd", "schemas");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // list all schemas in pCatalog
  eResult = eExecuteCommand(pSEitf, "lssch");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // return list of schemas in ppSchemas
  *ppSchemas = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema, void** ppTables)
{
  vTrace("RapidsSE_GetTables: %s %s\n", pCatalog, pSchema);
  
  eIE eResult;
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return IE_INVALID_PARAMETER;
  
  if (!pSchema)
    return IE_INVALID_PARAMETER;
  
  // change directory to tenants
  eResult = eExecuteCommand(pSEitf, "cd", "tenants");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteCommand(pSEitf, "cd", "boray");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteCommand(pSEitf, "cd", "catalogs");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
 
  // change directory to pCatalog
  eResult = eExecuteCommand(pSEitf, "cd", pCatalog);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteCommand(pSEitf, "cd", "schemas");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteCommand(pSEitf, "cd", pSchema);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }

  // change directory to tables
  eResult = eExecuteCommand(pSEitf, "cd", "tables");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }

  // list all the tables in pSchema of pCatalog
  eResult = eExecuteCommand(pSEitf, "lstbl");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // return list of tables in ppSchemas
  *ppTables = pSEitf->m_sResponse.a();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_PoolOpen(void)
{
  vTrace("vRapidsSE_PoolOpen\n");
  
  // open the pool
  RapidsSE::vPoolOpen();
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  vTrace("pRapidsSE_ScanTable: 0x%x %s %s %s\n", pSEitf, pCatalog, pSchema, pTable);
  
  eIE eResult;

  // clear the last error
  pSEitf->vClearLastError();
  
  // clear the last error
  pSEitf->vClearLastError();
  
  if (!pCatalog)
    return IE_INVALID_PARAMETER;
  
  if (!pSchema)
    return IE_INVALID_PARAMETER;
  
  if (!pTable)
    return IE_INVALID_PARAMETER;

  // change directory to tenants
  eResult = eExecuteCommand(pSEitf, "cd", "tenants");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to boray
  eResult = eExecuteCommand(pSEitf, "cd", "boray");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to catalogs
  eResult = eExecuteCommand(pSEitf, "cd", "catalogs");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pCatalog
  eResult = eExecuteCommand(pSEitf, "cd", pCatalog);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to schemas
  eResult = eExecuteCommand(pSEitf, "cd", "schemas");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to pSchema
  eResult = eExecuteCommand(pSEitf, "cd", pSchema);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // change directory to tables
  eResult = eExecuteCommand(pSEitf, "cd", "tables");
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }

  // select * from pTable
  eResult = eExecuteCommand(pSEitf, "sel", pTable);
  if (IE_NONE != eResult) {
    return eResult;
  }
  
  // get response
  eResult = eGetResponse(pSEitf);
  if (IE_NONE != eResult) {
    return eResult;
  }

  i8 iAddress = String::iToI8(pSEitf->m_sResponse.a());
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());

  vTrace("pRowCursor: 0x%x\n", pRowCursor);
  pRowCursor->vBegin(pSEitf->iPartition());
  pRowCursor->vFirst();
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_SEitfDelete(SEitf* pSEitf)
{
  vTrace("RapidsSE_SEitfDelete: 0x%x\n", pSEitf);
  
  // clear the last error
  pSEitf->vClearLastError();
  
  RapidsSE::vSEitfDelete(pSEitf);
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eIE RapidsSE_SEitfNew(void** ppSEitf, i8 iPartition)
{
  vTrace("RapidsSE_SEitfNew: Partition %lld\n", iPartition);
  
  //SEitf* pSEitf = pSEitf = RapidsSE::pSEitfNew();
  SEitf* pSEitf = new SEitf(iPartition);
  pSEitf->vInit();
  
  // return the newly created SEitf
  *ppSEitf = pSEitf;
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                          */
/*----------------------------------------------------------------------------*/
eIE eExecuteCommand(SEitf* pSEitf, const char* pCommand, const char* pParameter)
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
    pSEitf->vSetLastError(IE_INVALID_COMMAND);
    return IE_INVALID_COMMAND;
  }
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eIE eExecuteCommand(SEitf* pSEitf, const char* pCommand)
{
  bool bResult;
  char xArray[C_1KB * 1];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
  };
  
  // execute command pCommand
  String::vCopy(pWords[0], pCommand);
  
  vTrace("cd %s\n", pCommand);
  bResult = pSEitf->bDecode(pWords);
  
  if (!bResult) {
    // set last error
    pSEitf->vSetLastError(IE_INVALID_COMMAND);
    return IE_INVALID_COMMAND;
  }
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
eIE eExecuteCommand(SEitf* pSEitf, char* pWords[])
{
  bool bResult;
  
  // execute command
  bResult = pSEitf->bDecode(pWords);
  
  if (!bResult) {
    // set last error
    pSEitf->vSetLastError(IE_INVALID_COMMAND);
    return IE_INVALID_COMMAND;
  }
  
  return IE_NONE;
}

/*----------------------------------------------------------------------------*/
/* get response                                                               */
/*----------------------------------------------------------------------------*/
eIE eGetResponse(SEitf* pSEitf)
{
  // get response
  i8 iError = pSEitf->iGetResponse();
//  vTrace("rsp : %lld\n", iResponse);
  
  if (iError) {
    pSEitf->vSetLastError(IE_COMMAND_FAILED);
    return IE_COMMAND_FAILED;
  }
  
  pSEitf->vSetLastError(IE_NONE);
  return IE_NONE;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/