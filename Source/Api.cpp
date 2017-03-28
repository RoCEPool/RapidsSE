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
  try {
    vTraceEntry(iTraceLevel,
          "<RapidsSE_AddColumn: %p %s %s\n"_Str, pSEitf, pColumnName, pColumnType);
    
    eEC eResult;
    
    eResult = eValidateSEitf(pSEitf, ">RapidsSE_AddColumn"_Str, iTraceLevel);
    if (eResult != EC_NONE)
      return eResult;
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pColumnName) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel, ">RapidsSE_AddColumn(s): %p %s\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER]);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pColumnType) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      vTraceError(iTraceLevel, ">RapidsSE_AddColumn(b): %p %s\n"_Str, pSEitf, aEC[EC_INVALID_PARAMETER]);
      return EC_INVALID_PARAMETER;
    }

    Table* pTable = pSEitf->pCurrentTable();
    
    if (!pTable) {
      pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
      vTraceError(iTraceLevel, ">RapidsSE_AddColumn(c): %p %s\n"_Str, pSEitf, aEC[EC_NO_SUCH_TABLE]);
      return EC_NO_SUCH_TABLE;
    }

    // add a column
    eResult = eExecuteRequest(pSEitf, "add"_Str,
                              (Str*)pTable->aTableName(),
                              (Str*)pColumnName,
                              (Str*)pColumnType);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_AddColumn(d): %p %s\n"_Str, pSEitf, aEC[eResult]);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      vTraceError(iTraceLevel, ">RapidsSE_AddColumn(e): %p %s\n"_Str, pSEitf, aEC[eResult]);
      return eResult;
    }
    
    vTraceExit(iTraceLevel, ">RapidsSE_AddColumn: %p %s\n"_Str, pSEitf);
    return EC_NONE;
  }
  catch (Error* pError) {
    vTrace(">RapidsSE_AddColumn: %p %s\n"_Str, pSEitf, pError->aErrorMsg());
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    vTrace(">RapidsSE_AddColumn: %p\n"_Str, pSEitf);
    return EC_SE_UNKNOWN_ERROR;
  }
  
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
  
  // change directory to tables
  eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
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
  try {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_CreateTable: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
    
    eEC eResult;
    
    // pSEitf cannot be null
    if (pSEitf == nullptr) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(a): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be a pool address
    if (!gpBufferPool->bValidAddress(pSEitf)) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(b): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be an SEitf class
    if (pSEitf->eClass() != CN_SEITF) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(c): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pCatalog) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(d): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pSchema) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(e): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pTable) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(f): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    // change directory to ~
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(g): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
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
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(h): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(i): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }

    // change directory to catalogs
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(j): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(k): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }

    // change directory to pCatalog
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(l): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(m): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to schemas
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(n): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(o): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to pSchema
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(p): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(q): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(r): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(s): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // create a table
    eResult = eExecuteRequest(pSEitf, "mktbl"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(t): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(u): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(r): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(s): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get pTable
    eResult = eExecuteRequest(pSEitf, "gettbl"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      
      vTrace("RapidsSE_CreateTable: gettbl FAILED!!!\n"_Str);
      
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(v): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      
      // remove the table
      eEC eRmTblResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
      if (EC_NONE != eRmTblResult) {
        if (gpGrid->m_bTraceapi)
          vTrace(">RapidsSE_CreateTable(w): %p %s(%lld)\n"_Str, pSEitf,
                 aEC[eRmTblResult], eRmTblResult);
        return eResult;
      }
      
      // get response
      eRmTblResult = eGetResponse(pSEitf);
      if (EC_NONE != eResult) {
        if (gpGrid->m_bTraceapi)
          vTrace(">RapidsSE_CreateTable(x): %p %s(%lld)\n"_Str, pSEitf,
                 aEC[eRmTblResult], eRmTblResult);
        return eRmTblResult;
      }
      
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(y): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_CreateTable(z): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    return EC_NONE;
  }
  catch (Error* pError) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_CreateTable(!): %p %s\n"_Str, pSEitf, pError->aErrorMsg());
      RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_CreateTable(?): %p\n"_Str, pSEitf);
      return EC_SE_UNKNOWN_ERROR;
  }
  
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_DropTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  try {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_DropTable: %p %s %s %s\n"_Str, pSEitf, pCatalog, pSchema, pTable);
    
    eEC eResult;
    
    // pSEitf cannot be null
    if (pSEitf == nullptr) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(a): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be a pool address
    if (!gpBufferPool->bValidAddress(pSEitf)) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(b): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // pSEitf should be an SEitf class
    if (pSEitf->eClass() != CN_SEITF) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(c): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_INTERFACE], EC_INVALID_INTERFACE);
      return EC_INVALID_INTERFACE;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (!pCatalog) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(d): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pSchema) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(e): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    if (!pTable) {
      pSEitf->vSetLastError(EC_INVALID_PARAMETER);
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(f): %p %s(%lld)\n"_Str, pSEitf,
               aEC[EC_INVALID_PARAMETER], EC_INVALID_PARAMETER);
      return EC_INVALID_PARAMETER;
    }
    
    // change directory to ~
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "~"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(g): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(h): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to boray
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "boray"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(i): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(j): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to catalogs
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "catalogs"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(k): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(l): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to pCatalog
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pCatalog);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(m): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(n): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to schemas
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "schemas"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(o): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(p): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to pSchema
    eResult = eExecuteRequest(pSEitf, "cf"_Str, (Str*)pSchema);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(q): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(r): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // change directory to tables
    eResult = eExecuteRequest(pSEitf, "cf"_Str, "tables"_Str);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(s): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(t): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }

    // drop table
    eResult = eExecuteRequest(pSEitf, "rmtbl"_Str, (Str*)pTable);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(w): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_DropTable(x): %p %s(%lld)\n"_Str, pSEitf,
               aEC[eResult], eResult);
      return eResult;
    }
    
    return EC_NONE;
  }
  catch (Error* pError) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_CreateTable(!): %p %s\n"_Str, pSEitf, pError->aErrorMsg());
      RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_CreateTable(?): %p\n"_Str, pSEitf);
      return EC_SE_UNKNOWN_ERROR;
  }
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
/* end scan                                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_EndScan(SEitf* pSEitf)
{
  try {
    
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_EndScan: %p\n"_Str, pSEitf);
    
    if (pSEitf == nullptr) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_EndScan: %p %s\n"_Str, pSEitf, aEC[EC_INVALID_INTERFACE]);
      return EC_INVALID_INTERFACE;
    }
    
    if (!gpBufferPool->bValidAddress(pSEitf)) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_EndScan: %p %s\n"_Str, pSEitf, aEC[EC_INVALID_INTERFACE]);
      return EC_INVALID_INTERFACE;
    }
    
    // clear the last error
    pSEitf->vClearLastError();
    
    if (pSEitf->pCurrentTable() == nullptr) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_EndScan: %p\n"_Str, pSEitf);
      return EC_NONE;
    }
    
    i8 iAddress = StrLib::iToI8( pSEitf->m_sResponse.s());
    
    SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
    ASSERT(CN_SELCURSOR == pSelCursor->eClass());
    delete pSelCursor;
    
    // return the table
    eEC eResult = eExecuteRequest(pSEitf, "rettbl"_Str);
    if (EC_NONE != eResult) {
      
      vTrace(">RapidsSE_EndScan: rettbl FAILED!!!\n"_Str);
      
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_EndScan: %p %s\n"_Str, pSEitf, aEC[eResult]);
      
      return eResult;
    }
    
    // get response
    eResult = eGetResponse(pSEitf);
    if (EC_NONE != eResult) {
      if (gpGrid->m_bTraceapi)
        vTrace(">RapidsSE_EndScan: %p %s\n"_Str, pSEitf, aEC[eResult]);
      return eResult;
    }
    
    ASSERT(pSEitf->pCurrentTable() == nullptr);
    
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_EndScan: %p\n"_Str, pSEitf);
    
    return EC_NONE;
  }
  catch (Error* pError) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_EndScan: %p %s\n"_Str, pSEitf, pError->aErrorMsg());
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    vTrace("hello\n"_Str);
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_EndScan: %p\n"_Str, pSEitf);
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
  *ppCatalogs = pSEitf->m_sResponse.s();
  
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
  *ppColumns = pSEitf->m_sResponse.s();
  
  return eResult;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_GetLastError(SEitf* pSEitf, void** ppError)
{
  if (gpGrid->m_bTraceapi)
    vTrace("RapidsSE_GetLastError\n"_Str);
  
  *ppError = pSEitf->m_sErrorMsg.s();
  
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
  *ppSchemas = pSEitf->m_sResponse.s();
  
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
  *ppVersion = pSEitf->m_sResponse.s();
  
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
  *ppTables = pSEitf->m_sResponse.s();
  
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
eEC RapidsSE_PoolOpen(Str* pInstanceName, void** ppInstance, i8 iTraceLevel)
{
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
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_PoolOpen: %s %s\n"_Str, pInstanceName, pError->aErrorMsg());
      RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_PoolOpen: %s\n"_Str, pInstanceName);
      return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* scan table                                                                 */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_ScanTable(SEitf* pSEitf, Str* pCatalog, Str* pSchema, Str* pTable)
{
  try {
    if (gpGrid->m_bTraceapi)
      vTrace("<RapidsSE_ScanTable: %p %s %s %s %lld\n"_Str,
           pSEitf, pCatalog, pSchema, pTable, pSEitf->iPartition());
    
    eEC eResult;
    
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
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn(!): %p %s\n"_Str, pSEitf, pError->aErrorMsg());
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    if (gpGrid->m_bTraceapi)
      vTrace(">RapidsSE_AddColumn(?): %p %s\n"_Str, pSEitf, aEC[EC_SE_UNKNOWN_ERROR]);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* delete an seitf instance                                                   */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfDelete(void* pAttachMap, SEitf* pSEitf, i8 iTraceLevel)
{
  try {
    vTraceEntry(iTraceLevel, "<RapidsSE_SEitfDelete: %p\n"_Str, pSEitf);
    
    // clear the last error
    pSEitf->vClearLastError();
    
    RapidsSE::vSEitfDelete(pSEitf);
    
    vTraceExit(iTraceLevel, ">RapidsSE_SEitfDelete(a): %p\n"_Str, pSEitf);

    return EC_NONE;
  }
  catch (Error* pError) {
    vTrace(">RapidsSE_SEitfDelete: %p %s\n"_Str, pSEitf, pError->aErrorMsg());
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    vTrace(">RapidsSE_SEitfDelete: %p %s\n"_Str, pSEitf, aEC[EC_SE_UNKNOWN_ERROR]);
    return EC_SE_UNKNOWN_ERROR;
  }

}

/*----------------------------------------------------------------------------*/
/* is the error an EC_INVALID_INTERFACE ?                                     */
/*----------------------------------------------------------------------------*/
bool RapidsSE_SEitfInvalid(int iError, i8 iTraceLevel)
{
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
    vTrace(">RapidsSE_SEitfInvalid\n"_Str);
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    vTrace(">RapidsSE_SEitfInvalid\n"_Str);
    return EC_SE_UNKNOWN_ERROR;
  }
}

/*----------------------------------------------------------------------------*/
/* create a new seitf instance                                                */
/*----------------------------------------------------------------------------*/
eEC RapidsSE_SEitfNew(void* pAttachMap, void** ppSEitf, i8 iPartition, i8 iTraceLevel)
{
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
    vTrace(">RapidsSE_SEitfNew: Partition %lld\n"_Str, iPartition);
    RapidsSE::vHandler(pError);
    return pError->eErrorCode();
  }
  catch (...) {
    vTrace(">RapidsSE_SEitfNew: Partition %lld\n"_Str, iPartition);
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
