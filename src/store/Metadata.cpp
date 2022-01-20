/*============================================================================*/
/* Metadata.cpp -- Metadata                                                   */
/*============================================================================*/

#include "BASE.h"

#include "Metadata.hpp"
#include "Catalog.hpp"
#include "Schema.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"


#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "InfoSet.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "ShardDef.hpp"

#include "Grid.hpp"

#include "SP_Dic.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Metadata,CN_METADATA)
__aNewFunction(Metadata,CN_METADATA)
__vDeleteFunction(Metadata,CN_METADATA)
__aFooterFunction(Metadata)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Metadata::Metadata(void)
: Object(CN_METADATA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Metadata::~Metadata(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get current catalog                                                        */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::TASK::pDicStack();

  for (i8 i_Dic = pDicStack->nItems() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsCatalog())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current catalog                                                */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrCatalog();

  if (pDic)
    return pDic->szDicNm();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current schema                                                         */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::TASK::pDicStack();

  for (i8 i_Dic = pDicStack->nItems() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsSchema())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current schema                                                 */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrSchema();

  if (pDic)
    return pDic->szDicNm();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current table                                                          */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::TASK::pDicStack();

  for (i8 i_Dic = pDicStack->nItems() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsTable())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrTable();

  if (pDic)
    return pDic->szDicNm();
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get current tenant                                                         */
/*----------------------------------------------------------------------------*/
Dic* Metadata::pCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = AM::TASK::pDicStack();

  for (i8 i_Dic = pDicStack->nItems() - 1; i_Dic >= 0; i_Dic--) {
    
    Dic* pDic = pDicStack->pPick(i_Dic);
    
    if (pDic->bIsTenant())
      return pDic;

  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get name of current table                                                  */
/*----------------------------------------------------------------------------*/
Stz* Metadata::szCurrTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Dic* pDic = pCurrTenant();

  if (pDic)
    return pDic->szDicNm();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* list all metadata                                                          */
/*----------------------------------------------------------------------------*/
void Metadata::vListContents(i8 i_Depth, Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  if (! pDic) {
    Dic* pDic = pCurrTenant();
    vTrace("\nTENANT %s \\folder\n"_Stz,pDic->szDicNm());
    vListContents(0,pDic);
    return;
  }
  
  pDic->vGrabRead();
  
  for (Word* pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    
    Word* pCategory = pWord->pCategory();

    vTrace("%s%s %s\n"_Stz,
           Stz::szTab(i_Depth),
           pWord->szWordNm(),
           pCategory ? pCategory->szWordNm() : "n/a"_Stz);
    
    Object* pObject = pWord->pObject();
    if (pObject && CN_DIC == pObject->eGetClassNo())
      vListContents(i_Depth+1,(Dic*)pObject);
    else
    if (pObject && CN_TABLE == pObject->eGetClassNo())
      ((Table*)pObject)->vListContents(i_Depth+1);
    else
    if (pObject && CN_INDEX == pObject->eGetClassNo())
      ((Index*)pObject)->vListContents(i_Depth+1);
  }
  
  pDic->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* search all metadata                                                        */
/*----------------------------------------------------------------------------*/
void Metadata::vSearch(InfoSet* pInfoSet,
                       i8 iScope,
                       Stz* szCatalogName,
                       Stz* szSchemaName,
                       Stz* szTableName,
                       Stz* szColumnName,
                       Stz* szIndexName,
                       Stz* szIndexColumnName,
                       Dic* pDic,
                       Dic* pCatalog,
                       Dic* pSchema)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  if (! pDic) {
    Dic* pDic = pCurrTenant();
    //vTrace("\nTENANT %s \\folder\n"_Stz,pDic->szDicNm());
    vSearch(pInfoSet,iScope,
            szCatalogName,szSchemaName,szTableName,szColumnName,szIndexName,szIndexColumnName,
            pDic,nullptr,nullptr);
    return;
  }
  
  pDic->vGrabRead();
  
  for (Word* pWord = pDic->pFirstWord(); pWord; pWord = pWord->pNextWord()) {
    
    Object* pObject = pWord->pObject();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // catalog or schema
    
    if (pObject && CN_DIC == pObject->eGetClassNo()) {
      Dic* pDic = _pDic(pObject);
      if (pDic->bIsCatalog()) {
        if (szCatalogName && ! pDic->szDicNm()->bSame(szCatalogName))
          continue;
        pCatalog = pDic;
        if (1 == iScope) {
          vSearchCatalogs(pInfoSet,pCatalog);
          continue;
        }
      }
      else
      if (pDic->bIsSchema()) {
        if (szSchemaName && ! pDic->szDicNm()->bSame(szSchemaName))
          continue;
        pSchema = pDic;
        if (2 == iScope) {
          vSearchSchemas(pInfoSet,pCatalog,pSchema);
          continue;
        }
      }
      vSearch(pInfoSet,iScope,
              szCatalogName,szSchemaName,szTableName,szColumnName,szIndexName,szIndexColumnName,
              pDic,pCatalog,pSchema);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // table

    if (pObject && CN_TABLE == pObject->eGetClassNo()) {
      Table* pTable = _pTable(pObject);
      if (szTableName && ! pTable->szTableNm()->bSame(szTableName))
        continue;
      if (3 == iScope)
        vSearchTables(pInfoSet,pCatalog,pSchema,pTable);
      else
      if (4 == iScope)
        vSearchColumns(pInfoSet,pCatalog,pSchema,pTable,szColumnName);
      continue;
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // index

    if (pObject && CN_INDEX == pObject->eGetClassNo()) {
      Index* pIndex = _pIndex(pObject);
      Table* pTable = pIndex->pTable();
      if (szIndexName && ! pIndex->szIndexNm()->bSame(szIndexName))
        continue;
      if (5 == iScope) {
        vSearchIndexes(pInfoSet, pCatalog, pSchema, pTable, szIndexName);
        break;
      }
      else
      if (6 == iScope)
        vSearchIndexColumns(pInfoSet,pCatalog,pSchema,pTable,pIndex,szColumnName);
      continue;
    }

  }
  
  pDic->vDropRead();
}

/*----------------------------------------------------------------------------*/
/* search catalogs                                                            */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchCatalogs(InfoSet* pInfoSet,
                               Dic* pCatalog)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                  pCatalog->szDicNm(),
                                  C_VARGS_END);
  UNUSED(pRow);
}

/*----------------------------------------------------------------------------*/
/* search columns                                                             */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchColumns(InfoSet* pInfoSet,
                              Dic* pCatalog,
                              Dic* pSchema,
                              Table* pTable,
                              Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  Row* pRow;

  StreamDef* pStreamDef = pTable->pStreamDef();
  
  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pColumn = pStreamDef->pGetColumn(i_Col);
    
    if (szColumnName && ! pColumn->szColNm()->bSame(szColumnName))
      continue;
    
    pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                               pCatalog->szDicNm(),
                               pSchema->szDicNm(),
                               pTable->szTableNm(),

                               pColumn->iColumnNo(),
                               pColumn->szColNm(),
                               (i8)pColumn->eColumnType(),
                               aCT[pColumn->eColumnType()],
                               pColumn->iColumnLength(),
                               pColumn->iColumnScale(),
                               (i8)pColumn->bIsNullable(),
                               (i8)pColumn->bIsPtnKey(),

                               C_VARGS_END);
  }
}

/*----------------------------------------------------------------------------*/
/* search indexes                                                             */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchIndexes(InfoSet* pInfoSet,
                              Dic* pCatalog,
                              Dic* pSchema,
                              Table* pTable,
                              Stz* szIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Row* pRow;

  IdxVec* pIdxVec = pTable->pIdxVec();

  i8 nIndexes = pIdxVec->nItems();
  for (i8 i_Idx = 0; i_Idx < nIndexes; i_Idx++) {
    Index* pIndex = pIdxVec->pGet(i_Idx);

    if (szIndexName && ! pIndex->szIndexNm()->bSame(szIndexName))
      continue;

    // Temporary, until we support non-unique indexes
    b1 bIsUnique = false;

    pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,

                               pCatalog->szDicNm(),
                               pSchema->szDicNm(),
                               pTable->szTableNm(),
                               pIndex->szIndexNm(),
                               (i8) bIsUnique,

                               C_VARGS_END);
  }
}

/*----------------------------------------------------------------------------*/
/* search index columns                                                       */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchIndexColumns(InfoSet* pInfoSet,
                              Dic* pCatalog,
                              Dic* pSchema,
                              Table* pTable,
                              Index* pIndex,
                              Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Row* pRow;

  IndexDef* pIndexDef = pIndex->pIndexDef();

  i8 nCols = pIndexDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pColumn = pIndexDef->pGetColumn(i_Col);

    if (szColumnName && ! pColumn->szColNm()->bSame(szColumnName))
      continue;

    pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                               pCatalog->szDicNm(),
                               pSchema->szDicNm(),
                               pTable->szTableNm(),
                               pIndex->szIndexNm(),

                               pColumn->iColumnNo(),
                               pColumn->szColNm(),
                               (i8)pColumn->eColumnType(),
                               aCT[pColumn->eColumnType()],
                               pColumn->iColumnLength(),
                               pColumn->iColumnScale(),
                               (i8)pColumn->bIsNullable(),
                               (i8)pColumn->bIsPtnKey(),

                               C_VARGS_END);
  }
}

/*----------------------------------------------------------------------------*/
/* search schemas                                                             */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchSchemas(InfoSet* pInfoSet,
                              Dic* pCatalog,
                              Dic* pSchema)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                  pCatalog->szDicNm(),
                                  pSchema->szDicNm(),
                                  C_VARGS_END);
  UNUSED(pRow);
}

/*----------------------------------------------------------------------------*/
/* search tables                                                              */
/*----------------------------------------------------------------------------*/
void Metadata::vSearchTables(InfoSet* pInfoSet,
                             Dic* pCatalog,
                             Dic* pSchema,
                             Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
  
  Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                  pCatalog->szDicNm(),
                                  pSchema->szDicNm(),
                                  pTable->szTableNm(),
                                  C_VARGS_END);
  UNUSED(pRow);
}

/*----------------------------------------------------------------------------*/
/* get column set matching given arguments                                    */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pColumnDefs(Stz* szCatalogName,
                               Stz* szSchemaName,
                               Stz* szTableName,
                               Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Table* pLookupTable = AM::pTableFind(C_TDEF_COLUMNDEFS);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);
  
  // v1: assume szCatalogName, szSchemaName, szTableName are set
  //            szColumnName is not set
  
  RASSERT(szCatalogName);
  RASSERT(szSchemaName);
  RASSERT(szTableName);
  RASSERT(! szColumnName);

  DicStack* pDicStack = new DicStack;
  
  Row* pRow;
  
  if (pDicStack->bTest(szCatalogName,szSchemaName,szTableName)) {
        
    Table* pTable = pDicStack->pTable();
    
    //vTrace("TABLE %s\n"_Stz,pTable->szTableNm());
    
    StreamDef* pStreamDef = pTable->pStreamDef();
    
    i8 nCols = pStreamDef->nColumns();
    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
      Column* pColumn = pStreamDef->pGetColumn(i_Col);
      
      pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                 szCatalogName,
                                 szSchemaName,
                                 szTableName,

                                 pColumn->iColumnNo(),
                                 pColumn->szColNm(),
                                 (i8)pColumn->eColumnType(),
                                 aCT[pColumn->eColumnType()],
                                 pColumn->iColumnLength(),
                                 pColumn->iColumnScale(),
                                 (i8)pColumn->bIsNullable(),
                                 (i8)pColumn->bIsPtnKey(),

                                 C_VARGS_END);
    }
  }
  

  delete pDicStack;
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get index def columns set matching given arguments                         */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pIndexColumnDefs(Stz* szCatalogName,
                                   Stz* szSchemaName,
                                   Stz* szTableName,
                                   Stz* szIndexName,
                                   Stz* szColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Table* pLookupTable = AM::pTableFind(C_TDEF_INDEXCOLUMNDEFS);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  // v1: assume szCatalogName, szSchemaName, szTableName, szIndexName are set
  //            szColumnName is not set

  RASSERT(szCatalogName);
  RASSERT(szSchemaName);
  RASSERT(szTableName);
  RASSERT(szIndexName);
  RASSERT( ! szColumnName );

  DicStack* pDicStack = new DicStack;

  Row* pRow;

  if (pDicStack->bTest(szCatalogName,szSchemaName,szTableName, szIndexName)) {

    Word* pWord = pDicStack->pFindWord(szIndexName);

    Index* pIndex = _pIndex(pWord->pObject());

    //vTrace("INDEX %s\n"_Stz,pIndex->szIndexNm());

    IndexDef* pIndexDef = pIndex->pIndexDef();

    i8 nColumns = pIndexDef->nColumns();
    for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
      Column* pColumn = pIndexDef->pGetColumn(i_Col);

      pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                 szCatalogName,
                                 szSchemaName,
                                 szTableName,
                                 szIndexName,

                                 pColumn->iColumnNo(),
                                 pColumn->szColNm(),
                                 (i8)pColumn->eColumnType(),
                                 aCT[pColumn->eColumnType()],
                                 pColumn->iColumnLength(),
                                 pColumn->iColumnScale(),
                                 (i8)pColumn->bIsNullable(),
                                 (i8)pColumn->bIsPtnKey(),

                                 C_VARGS_END);
    }

  }


  delete pDicStack;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* get the colummns of partition key for table                                */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pPartitionKey(Stz* szCatalogName,
                                 Stz* szSchemaName,
                                 Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Table* pLookupTable = AM::pTableFind(C_TDEF_PARTITIONKEY);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);
  
  // v1: assume szCatalogName, szSchemaName, szTableName are set
  //            szColumnName is not set
  
  RASSERT(szCatalogName);
  RASSERT(szSchemaName);
  RASSERT(szTableName);

  DicStack* pDicStack = new DicStack;
  
  Row* pRow;
  
  if (pDicStack->bTest(szCatalogName,szSchemaName,szTableName)) {
        
    Table* pTable = pDicStack->pTable();
    
    //vTrace("TABLE %s\n"_Stz,pTable->szTableNm());
        
    ShardDef*  pShardDef= pTable->pShardDef();

    i8 nCols = pShardDef->nColumns();
    for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
      Column* pColumn = pShardDef->pGetColumn(i_Col);
      
      pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                 szCatalogName,
                                 szSchemaName,
                                 szTableName,

                                 i_Col,
                                 pColumn->szColNm(),

                                 C_VARGS_END);
    }
  }
  

  delete pDicStack;
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* lookup a catalog and return an infoset about it                            */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pLookupCatalog(Stz* szCatalogName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPCATALOG);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);
  
  DicStack* pDicStack = new DicStack;

  if (pDicStack->bTest(szCatalogName)) {
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    szCatalogName,
                                    C_VARGS_END);
    UNUSED(pRow);
  }
  
  delete pDicStack;
  
  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* lookup a index and return an infoset about it                              */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pLookupIndex(Stz* szCatalogName,
                                Stz* szSchemaName,
                                Stz* szTableName,
                                Stz* szIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);

  Table* pLookupIndex = AM::pTableFind(C_TDEF_LOOKUPINDEX);

  StreamDef* pLookupStreamDef = pLookupIndex->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  DicStack* pDicStack = new DicStack;

  if (pDicStack->bTest(szCatalogName,szSchemaName,szTableName,szIndexName)) {
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    szCatalogName,
                                    szSchemaName,
                                    szTableName,
                                    szIndexName,
                                    C_VARGS_END);
    UNUSED(pRow);
  }

  delete pDicStack;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* lookup a schema and return an infoset about it                             */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pLookupSchema(Stz* szCatalogName,
                                 Stz* szSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPSCHEMA);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);
  
  DicStack* pDicStack = new DicStack;
  
  if (pDicStack->bTest(szCatalogName,szSchemaName)) {
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    szCatalogName,
                                    szSchemaName,
                                    C_VARGS_END);
    UNUSED(pRow);
  }
  
  delete pDicStack;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* lookup a table and return an infoset about it                              */
/*----------------------------------------------------------------------------*/
InfoSet* Metadata::pLookupTable(Stz* szCatalogName,
                                Stz* szSchemaName,
                                Stz* szTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  Table* pLookupTable = AM::pTableFind(C_TDEF_LOOKUPTABLE);
  StreamDef* pLookupStreamDef = pLookupTable->pStreamDef();

  InfoSet* pInfoSet = new InfoSet(pLookupStreamDef);

  DicStack* pDicStack = new DicStack;
  
  if (pDicStack->bTest(szCatalogName,szSchemaName,szTableName)) {
    Row* pRow = pInfoSet->pAllotRow(C_VARGS_BEGIN,
                                    szCatalogName,
                                    szSchemaName,
                                    szTableName,
                                    C_VARGS_END);
    UNUSED(pRow);
  }
  
  delete pDicStack;

  return pInfoSet;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog                                                     */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm);
  
  delete pDicStack;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog, schema                                             */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm, Stz* szSchemaNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm,szSchemaNm);
  
  delete pDicStack;
  
  return bFound;
}

/*----------------------------------------------------------------------------*/
/* test for given catalog, schema, table                                      */
/*----------------------------------------------------------------------------*/
bool Metadata::bTest(Stz* szCatalogNm, Stz* szSchemaNm, Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_METADATA,_FOOTER_);
    
  DicStack* pDicStack = new DicStack;

  bool bFound = pDicStack->bTest(szCatalogNm,szSchemaNm,szTableNm);
  
  delete pDicStack;
  
  return bFound;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*
 
Metadata Goal
=============
 
  Represent all metadata in system tables (including session information)
  Query the system tables with SQL and/or pipeline statements
  Return responses in row format
 
  Metadata Interim Solution:
    Get metadata by querying the SE API (example APIs provided below)
    Return responses in row format.
 
 
Resolution/Lookup of Tables
---------------------------
 
  Determine whether a table exists and return its full name from (potentially) partial information.
 
  Ideally: “SELECT catalogName, schemaName, tableName FROM rapids.system.tables [ WHERE … ] ;”
 
  InfoSet* pLookupTable (Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName);
 
  Where each row in the InfoSet contains rows with the following columns:
    CatalogName,
    SchemaName
    TableName
 
  Note: Any of the parameter strings passed in may be null, in which case they would not be used to filter/limit the results returned.
    e.g. to get a list of all tables in a catalog, simply supply the catalogName but supply nullptr for szSchemaName and szTableName.
    Or to get a list of all tables with a given name in all catalogs and schemas then supply the tablename but provide nulls for the catalog and schema names.
 
  If a table is not known then zero rows will be returned.
  If a table is ambiguous then multiple rows will be returned.
 

 Resolution/Lookup of Schemas
 ----------------------------
 
  Determine whether a schema exists and return its full name from (potentially) partial information.
 
  Ideally: “SELECT catalogName, schemaName FROM rapids.system.schemas [ WHERE … ] ;”
 
  InfoSet* pLookupSchema(Stz* szCatalogName, Stz* szSchemaName);
 
  Where each row in the InfoSet contains rows with the following columns:
    CatalogName
    SchemaName
 
 
Resolution/Lookup of Catalogs
-----------------------------
 
  Determine whether a catalog exists and return its full name.
 
  Ideally: “SELECT catalogName FROM rapids.system.schemas [ WHERE … ] ;”
 
  RowBuffer* pLookupCatalog(Stz* szCatalogName);
 
  Where each row in the InfoSet contains rows with the following columns:
    CatalogName
 
 
Retrieve Column Definition Metadata
-----------------------------------
 
  Retrieve definition information about columns in a table.
 
  Ideally: “SELECT catalogName, schemaName, tableName, columnName, columnType, notNull FROM rapids.system.columns WHERE …;”
 
  InfoSet* pColumnDefs(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szColumnName);
 
  Where each row in the InfoSet contains:
    catalogName
    schemaName
    tableName
    columnName
    columnType
    notNull
    (other fields to be added later)
 
  Note: Any of the parameter strings passed in may be null, in which case they would not be used to filter/limit the results returned.
 
 
Retrieve Index Definition Metadata
----------------------------------

  Retrieve definition information about indexes in a table. Note that this does not return index columns.
 
  Ideally: “SELECT catalogName, schemaName, tableName, indexName, isUnique, isPrimary, indexType FROM rapids.system.indexes WHERE …;”
 
  InfoSet* pIndexDefs(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szIndexName);
 
  Where each row in the InfoSet contains:
    catalogName
    schemaName
    tableName
    indexName
    isUnique
    isPrimary
    indexType
    (other fields to be added later)
 

Retrieve Index Column Metadata
------------------------------
 
  Retrieve information about the columns used in an index.
 
  Ideally: “SELECT catalogName, schemaName, tableName, indexName, ordinal, columnName FROM rapids.system.indexColumns WHERE …;”
 
  InfoSet* pIndexDefs(Stz* szCatalogName, Stz* szSchemaName, Stz* szTableName, Stz* szIndexName);
 
  Where each row in the InfoSet contains:
    catalogName
    schemaName
    tableName
    indexName
    ordinal
    columnName
    (other fields to be added later)

*/
