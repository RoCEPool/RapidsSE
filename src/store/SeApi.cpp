/*============================================================================*/
/* SeApi.cpp -- Se Reentrant API                                              */
/*============================================================================*/

#include "BASE.h"

#include "RowSingle.hpp"

#include "EntWrk.hpp"

#include "SeApi.hpp"
#include "CrvBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Metadata.hpp"
#include "InfoSet.hpp"

#include "AM.hpp"

#include "Job.hpp"

#include "Job.hpp"
#include "Session.hpp"
#include "Sessions.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,SeApi,CN_SEAPI)
__aNewFunction(SeApi,CN_SEAPI)
__vDeleteFunction(SeApi,CN_SEAPI)
__aFooterFunction(SeApi)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeApi::SeApi(void)
: Object(CN_SEAPI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence = ""_Stz;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeApi::~SeApi(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current folder as string                                               */
/*----------------------------------------------------------------------------*/

 Stz* SeApi::szCurrentFolderNm(String* psFolderNm)
{
  _static_

  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();
  
  // express that as a folder name string
  psFolderNm->vCopy(pDic->szDicNm());
    
  return psFolderNm->szStz();
}

/*----------------------------------------------------------------------------*/
/* get current folder path as string                                          */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szCurrentFolderPath(String* psFolderPath)
{
  _static_

  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // express that as a path-folder string
  pDicStack->vToPathStr(psFolderPath->szStz());
  
  return psFolderPath->szStz();
}

/*----------------------------------------------------------------------------*/
/* test if given table name is known                                          */
/*----------------------------------------------------------------------------*/
bool SeApi::bIsKnownTableNm(Stz* szTableNm)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  s32 sWordNm;
  sWordNm = szTableNm;

  Word* pWord = pDicStack->pFindWord(sWordNm);
 
  if (! pWord)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test if given column name is known within supplied table                   */
/*----------------------------------------------------------------------------*/
bool SeApi::bIsKnownTableColumnNm(Stz* szTableNm, Stz* szColumnNm)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();

  s32 sWordNm;
  sWordNm = szTableNm;

  Word* pPackageWord = pDic->pFindWord(sWordNm);
  if (! pPackageWord)
    return false;

  Dic* pTableDic = _pDic(pPackageWord->pObject());
  if (! pTableDic)
    return false;

  Word* pColumnsWord = pTableDic->pFindWord(sWordNm);
  if (! pColumnsWord)
    return false;

  Table* pTable = _pTable(pColumnsWord->pObject());
  if (! pTable)
    return false;

//  Word* pColumnWord = nullptr;
//  while ( (pColumnWord = pColumnsDic->pEachWord(pColumnWord)) ) {
//    vTrace("  - %s\n"_Stz,pColumnWord->szWordNm());
//   }

  StreamDef* pStreamDef = pTable->pStreamDef();
  if (! pStreamDef)
    return false;
  
  Column* pColumn = pStreamDef->pFindColumn(szColumnNm);
  if (! pColumn)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get original command text                                                  */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szOriginalCommand(void)
{
  Job* pJob = AM::pTaskJob();
  if (! pJob)
    return nullptr;

  i8 iSessionId = pJob->i_Session();
  if (iSessionId == C_UNKNOWN)
    return nullptr;
  
  Session* pSession = AM::pSessions()->pGetSession(iSessionId);
  if (! pSession)
    return nullptr;
  
  Input* pInput = pSession->m_pInput;
  if (! pInput)
    return nullptr;

  Stz* szCommand = pInput->szRawInput();
  
  return szCommand;
}

/*----------------------------------------------------------------------------*/
/* print all columns of given table                                           */
/*----------------------------------------------------------------------------*/
void SeApi::vPrintAllColumnsOfTable(Stz* szTableNm)
{
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();

  s32 sWordNm;
  sWordNm = szTableNm;

  Word* pPackageWord = pDic->pFindWord(sWordNm);
  if (! pPackageWord)
    return ;

  Dic* pTableDic = _pDic(pPackageWord->pObject());
  if (! pTableDic)
    return ;

  Word* pColumnsWord = pTableDic->pFindWord(sWordNm);
  if (! pColumnsWord)
    return ;

  Table* pTable = _pTable(pColumnsWord->pObject());
  if (! pTable)
    return ;

//  Word* pColumnWord = nullptr;
//  while ( (pColumnWord = pColumnsDic->pEachWord(pColumnWord)) ) {
//    vTrace("  - %s\n"_Stz,pColumnWord->szWordNm());
//   }

  StreamDef* pStreamDef = pTable->pStreamDef();
  if (! pStreamDef)
    return ;
  
  pStreamDef->vPrint();
}

/*----------------------------------------------------------------------------*/
/* print all table names in current folder (debug help)                       */
/*----------------------------------------------------------------------------*/
void SeApi::vPrintAllTablesInCurrentFolder(void)
{
  _static_
  
  // get the current task
  Task* pTask = AM::pThisTLSTask();
  
  // get dictionary stack
  DicStack* pDicStack = pTask->pDicStack();
  
  // get dictionary
  Dic* pDic = pDicStack->pTop();

  Word* pWord = nullptr;
  while ( (pWord = pDic->pEachWord(pWord)) ) {
    vTrace("  %s\n"_Stz,pWord->szWordNm());
   }
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pipe | operator                                                            */
/*----------------------------------------------------------------------------*/
SeApi& SeApi::operator |(SeApi* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  UNUSED(pArg);
  
  return *(SeApi*)this;
}

/*----------------------------------------------------------------------------*/
/* add operator argument                                                      */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pAdd(Stz* szOpNm, Stz* szOpArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence.vCatSprint(" %s %s"_Stz,szOpArg,szOpNm);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  pEncode(szAction,""_Stz);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction, Stz* szArgument)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  if (szAction->bSame("scan"_Stz) && szArgument->bSame("tables"_Stz)) {
    pAdd("metaScan_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("into"_Stz)) {
    pAdd("rowToList_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("tbd..."_Stz)) {
    vTrace("pEncode tbd : %s\n"_Stz,szArgument); \
  }
  else {
    NEVER_REACH;
  }
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* get sequence                                                               */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  return m_sSequence;
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current catalog                                                        */
/*----------------------------------------------------------------------------*/
Dic* SeApi::pCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Dic* pDic = pMetadata->pCurrCatalog();

  delete pMetadata;

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* get name of current catalog                                                */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Stz* szCatalogNm = pMetadata->szCurrCatalog();

  delete pMetadata;

  return szCatalogNm;
}

/*----------------------------------------------------------------------------*/
/* get current schema                                                         */
/*----------------------------------------------------------------------------*/
Dic* SeApi::pCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Dic* pDic = pMetadata->pCurrSchema();

  delete pMetadata;

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* get name of current schema                                                 */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Stz* szSchemaNm = pMetadata->szCurrSchema();

  delete pMetadata;

  return szSchemaNm;
}

/*----------------------------------------------------------------------------*/
/* get current tenant                                                         */
/*----------------------------------------------------------------------------*/
Dic* SeApi::pCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Dic* pDic = pMetadata->pCurrTable();

  delete pMetadata;

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Stz* szTableNm = pMetadata->szCurrTable();
  
  delete pMetadata;

  return szTableNm;
}

/*----------------------------------------------------------------------------*/
/* get current tenant                                                         */
/*----------------------------------------------------------------------------*/
Dic* SeApi::pCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Dic* pDic = pMetadata->pCurrTenant();

  delete pMetadata;

  return pDic;
}

/*----------------------------------------------------------------------------*/
/* get name of current tenant                                                 */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Stz* szTenantNm = pMetadata->szCurrTenant();
  
  delete pMetadata;

  return szTenantNm;
}

/*----------------------------------------------------------------------------*/
/* print full list of metadata                                                */
/*----------------------------------------------------------------------------*/
void SeApi::vPrintMetadata(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  pMetadata->vListContents(0,nullptr);

  delete pMetadata;
}

/*----------------------------------------------------------------------------*/
/* search catalogs for matching name (or nullptr for all)                     */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchCatalogs(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPCATALOG);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,1,szCatalogName,nullptr,nullptr,nullptr,nullptr,nullptr);

  delete pMetadata;
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get column set matching given arguments                                    */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchColumns(Stz* szCatalogName,
                               Stz* szSchemaName,
                               Stz* szTableName,
                               Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Table* pLookupTable = AM::pTableFind(C_TDEF_COLUMNDEFS);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,4,szCatalogName,szSchemaName,szTableName,szColumnName,nullptr,nullptr);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* search indexes for matching name (or nullptr for all)                      */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchIndexes(Stz* szCatalogName,
                               Stz* szSchemaName,
                               Stz* szTableName,
                               Stz* szIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);

  Metadata* pMetadata = new Metadata;

  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPINDEX);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,5,szCatalogName,szSchemaName,szTableName,nullptr,szIndexName,nullptr);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get column set matching given arguments                                    */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchIndexColumns(Stz* szCatalogName,
                               Stz* szSchemaName,
                               Stz* szTableName,
                               Stz* szIndexName,
                               Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);

  Metadata* pMetadata = new Metadata;

  Table* pLookupTable = AM::pTableFind(C_TDEF_INDEXCOLUMNDEFS);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,6,szCatalogName,szSchemaName,szTableName,nullptr,szIndexName,szColumnName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* search schemas for matching name (or nullptr for all)                      */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchSchemas(Stz* szCatalogName,
                               Stz* szSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPSCHEMA);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,2,szCatalogName,szSchemaName,nullptr,nullptr,nullptr,nullptr);

  delete pMetadata;
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* search tables for matching name (or nullptr for all)                       */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pSearchTables(Stz* szCatalogName,
                              Stz* szSchemaName,
                              Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPTABLE);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  pMetadata->vSearch(pInfoSet,3,szCatalogName,szSchemaName,szTableName,nullptr,nullptr,nullptr);

  delete pMetadata;
  
  return pInfoSet;
}


/*----------------------------------------------------------------------------*/
/* get column set matching given arguments                                    */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pColumnDefs(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName,
                            Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  InfoSet* pInfoSet = pMetadata->pColumnDefs(szCatalogName,
                                             szSchemaName,
                                             szTableName,
                                             szColumnName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get index set matching given arguments                                    */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pIndexColumnDefs(Stz* szCatalogName,
                           Stz* szSchemaName,
                           Stz* szTableName,
                           Stz* szIndexName,
                           Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);

  Metadata* pMetadata = new Metadata;

  InfoSet* pInfoSet = pMetadata->pIndexColumnDefs(szCatalogName,
                                                  szSchemaName,
                                                  szTableName,
                                                  szIndexName,
                                                  szColumnName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* determine whether a catalog exists and return its full name                */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pLookupCatalog(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  InfoSet* pInfoSet = pMetadata->pLookupCatalog(szCatalogName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* determine whether an index exists and return its full name                 */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pLookupIndex(Stz* szCatalogName,
                             Stz* szSchemaName,
                             Stz* szTableName,
                             Stz* szIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);

  Metadata* pMetadata = new Metadata;

  InfoSet* pInfoSet = pMetadata->pLookupIndex(szCatalogName,
                                              szSchemaName,
                                              szTableName,
                                              szIndexName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* determine whether a schema exists and return its full name                 */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pLookupSchema(Stz* szCatalogName,
                                Stz* szSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
 
  InfoSet* pInfoSet = pMetadata->pLookupSchema(szCatalogName,
                                               szSchemaName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* determine whether a table exists and return its full name                  */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pLookupTable(Stz* szCatalogName,
                               Stz* szSchemaName,
                               Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  InfoSet* pInfoSet = pMetadata->pLookupTable(szCatalogName,
                                              szSchemaName,
                                              szTableName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get the colummns of partition key for table                                */
/*----------------------------------------------------------------------------*/
InfoSet* SeApi::pPartitionKey(Stz* szCatalogName,
                            Stz* szSchemaName,
                            Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;
  
  InfoSet* pInfoSet = pMetadata->pPartitionKey(szCatalogName,
                                             szSchemaName,
                                             szTableName);

  delete pMetadata;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog                                                     */
/*----------------------------------------------------------------------------*/
bool SeApi::bTest(Stz* szCatalogNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;

  bool bFound = pMetadata->bTest(szCatalogNm);
  
  delete pMetadata;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog                                                     */
/*----------------------------------------------------------------------------*/
bool SeApi::bTest(Stz* szCatalogNm, Stz* szSchemaNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;

  bool bFound = pMetadata->bTest(szCatalogNm,szSchemaNm);
  
  delete pMetadata;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog                                                     */
/*----------------------------------------------------------------------------*/
bool SeApi::bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  Metadata* pMetadata = new Metadata;

  bool bFound = pMetadata->bTest(szCatalogNm,szSchemaNm,szTableNm);
  
  delete pMetadata;
  
  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
