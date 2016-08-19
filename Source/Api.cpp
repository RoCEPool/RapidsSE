/*============================================================================*/
/* Api.cpp - RapidsSE Api funcstions                                          */
/*============================================================================*/

#include "Api.hpp"

/*----------------------------------------------------------------------------*/
/* get list of catalogs                                                       */
/*----------------------------------------------------------------------------*/
char* pRapidsSE_GetCatalogs(SEitf* pSEitf)
{
  vTrace("pRapidsSE_GetCatalogs\n");
  
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };
  
  // change directory to tenants
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tenants");
  pSEitf->bDecode(pWords);
  vTrace("cd tenants\n");
  
  // get response
  char* pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd tenants): %s\n", pResponse);
  
  // change directory to ten
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "boray");
  pSEitf->bDecode(pWords);
  vTrace("cd ten\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd ten): %s\n", pResponse);
  
  // change directory to catalogs
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "catalogs");
  pSEitf->bDecode(pWords);
  vTrace("cd catalogs\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd catalogs): %s\n", pResponse);
  
  // list all catalogs
  String::vCopy(pWords[0], "lscat");
  pSEitf->bDecode(pWords);
  vTrace("lscat\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (lscat): %s\n", pSEitf->m_sResponse.a());
  
  // return list of catalogs
  return pResponse;
}

/*----------------------------------------------------------------------------*/
/* get list of schemas for given catalog                                      */
/*----------------------------------------------------------------------------*/
char* pRapidsSE_GetSchemas(SEitf* pSEitf, char* pCatalog)
{
  vTrace("pRapidsSE_GetSchemas: %s\n", pCatalog);
  
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };
  
  if (!pCatalog)
    return nullptr;
  
  // change directory to tenants
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tenants");
  pSEitf->bDecode(pWords);
  vTrace("cd tenants\n");
  
  // get response
  char* pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd tenants): %s\n", pResponse);
  
  // change directory to tenant ten
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "boray");
  pSEitf->bDecode(pWords);
  vTrace("cd ten\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd ten): %s\n", pResponse);
  
  // change directory to catalogs
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "catalogs");
  pSEitf->bDecode(pWords);
  vTrace("cd catalogs\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd catalogs): %s\n", pResponse);
  
  // change directory to pCatalog
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], pCatalog);
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pCatalog);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to schemas
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "schemas");
  pSEitf->bDecode(pWords);
  vTrace("cd schemas\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // list all schemas in pCatalog
  String::vCopy(pWords[0], "lssch");
  pSEitf->bDecode(pWords);
  vTrace("lssch\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // return list of schemas in pCatalog
  return pResponse;
}

/*----------------------------------------------------------------------------*/
/* get list of tables for given schema of given catalog                       */
/*----------------------------------------------------------------------------*/
char* pRapidsSE_GetTables(SEitf* pSEitf, char* pCatalog, char* pSchema)
{
  vTrace("pRapidsSE_GetTables: %s %s\n", pCatalog, pSchema);
  
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };
  
  if (!pCatalog)
    return nullptr;
  
  if (!pSchema)
    return nullptr;
  
  // change directory to tenants
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tenants");
  pSEitf->bDecode(pWords);
  vTrace("cd tenants\n");
  
  // get response
  char* pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd tenants): %s\n", pResponse);
  
  // change directory to tenant ten
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "boray");
  pSEitf->bDecode(pWords);
  vTrace("cd ten\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd ten): %s\n", pResponse);
  
  // change directory to catalogs
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "catalogs");
  pSEitf->bDecode(pWords);
  vTrace("cd catalogs\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd catalogs): %s\n", pResponse);
  
  // change directory to pCatalog
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], pCatalog);
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pCatalog);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to schemas
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "schemas");
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pCatalog);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to pSchema
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], pSchema);
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pSchema);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to tables
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tables");
  pSEitf->bDecode(pWords);
  vTrace("cd tables\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // list all the tables in pSchema of pCatalog
  String::vCopy(pWords[0], "lstbl");
  pSEitf->bDecode(pWords);
  vTrace("lstbl\n");
  
  // get the response
  pResponse = pSEitf->pGetResponse();
  
  vTrace("rsp: %s\n", pResponse);
  
  // return the list of tables in pSchema of pCatalog
  return pResponse;
}

/*----------------------------------------------------------------------------*/
/* open the shared memory buffer pool                                         */
/*----------------------------------------------------------------------------*/
void vRapidsSE_PoolOpen(void)
{
  vTrace("vRapidsSE_PoolOpen\n");
  
  RapidsSE::vPoolOpen();
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void vRapidsSE_ScanTable(SEitf* pSEitf, char* pCatalog, char* pSchema, char* pTable)
{
  vTrace("pRapidsSE_ScanTable: 0x%x %s %s %s\n", pSEitf, pCatalog, pSchema,pTable);
  
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };
  
  if (!pCatalog)
    return;
  
  if (!pSchema)
    return;
  
  if (!pTable)
    return;

  // change directory to tenants
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tenants");
  pSEitf->bDecode(pWords);
  vTrace("cd tenants\n");
  
  // get response
  char* pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd tenants): %s\n", pResponse);
  
  // change directory to tenant ten
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "boray");
  pSEitf->bDecode(pWords);
  vTrace("cd boray\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd boray): %s\n", pResponse);
  
  // change directory to catalogs
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "catalogs");
  pSEitf->bDecode(pWords);
  vTrace("cd catalogs\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (cd catalogs): %s\n", pResponse);
  
  // change directory to pCatalog
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], pCatalog);
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pCatalog);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to schemas
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "schemas");
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pCatalog);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to pSchema
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], pSchema);
  pSEitf->bDecode(pWords);
  vTrace("cd %s\n", pSchema);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);
  
  // change directory to tables
  String::vCopy(pWords[0], "cd");
  String::vCopy(pWords[1], "tables");
  pSEitf->bDecode(pWords);
  vTrace("cd tables\n");
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp: %s\n", pResponse);

  // select * from pTable
  String::vCopy(pWords[0], "sel");
  String::vCopy(pWords[1], pTable);
  pSEitf->bDecode(pWords);
  vTrace("sel %s\n", pTable);
  
  // get response
  pResponse = pSEitf->pGetResponse();
  vTrace("rsp (sel %s): 0x%x\n", pTable, pResponse);
  
  i8 iAddress = String::iToI8(pResponse);
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());

  vTrace("pRowCursor: 0x%x\n", pRowCursor);
  pRowCursor->vBegin();
  
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
char* pRapidsSE_GetNextRow(SEitf* pSEitf)
{
  //vTrace("pRapidsSE_GetNextRow: 0x%x\n", pSEitf);
  //vTrace("pRapidsSE_GetNextRow: 0x%x\n", pSEitf->m_sResponse.a());
  
  i8 iAddress = String::iToI8( pSEitf->m_sResponse.a());
  
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  //vTrace("pRowCursor: 0x%x\n", pRowCursor);
  
  char* pStr = pRowCursor->pRowAsStr();
  if (!pStr) {
    delete pRowCursor;
    return nullptr;
  }
  
  //vTrace("Row: %s\n",pStr);
  pRowCursor->vNext();
  
  return pStr;
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
void vRapidsSE_SEitfDelete(SEitf* pSEitf)
{
  vTrace("vRapidsSE_SEitfDelete: 0x%x\n", pSEitf);
  
  RapidsSE::vSEitfDelete(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
SEitf* pRapidsSE_SEitfNew(void)
{
  vTrace("pRapidsSE_SEitfNew\n");
  
  SEitf* pSEitf = RapidsSE::pSEitfNew();
  pSEitf->vInit();
  
  return pSEitf;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
