/*============================================================================*/
/* test.cpp - general testing                                                 */
/*============================================================================*/
  
#include "BASE.h"

#include "Test.hpp"

#include "IndexDef.hpp"
#include "SkipRoots.hpp"

#include "EntBuffer.hpp"
#include "EntBufSorter.hpp"

#include "AM.hpp"
#include "Job.hpp"
#include "MY.hpp"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

#include "TagConstants.h"
#include "Engine.hpp"
#include "SeApi.hpp"
#include "Api.hpp"
#include "CatalogMetadata.hpp"
#include "ColumnMetadata.hpp"
#include "IndexMetadata.hpp"
#include "IndexColumnMetadata.hpp"
#include "PartitionKeyMetadata.hpp"
#include "SchemaMetadata.hpp"
#include "TableMetadata.hpp"
#include "SP_CatalogMetadata.hpp"
#include "SP_ColumnMetadata.hpp"
#include "SP_IndexMetadata.hpp"
#include "SP_IndexColumnMetadata.hpp"
#include "SP_SchemaMetadata.hpp"
#include "SP_TableMetadata.hpp"
#include "SP_PartitionKeyMetadata.hpp"

#include "TpchRatios.h"
#include "Locator.hpp"

#include "SP_Bits.hpp"
#include "SP_Bytes.hpp"
#include "SP_Timer.hpp"

#include "Flatten.hpp"

/*----------------------------------------------------------------------------*/
/* test1                                                                      */
/*----------------------------------------------------------------------------*/
void vTest1(void)
{
  vTrace("Stack Test\n"_Stz);
  
  TextStack* pTextStack = new TextStack;
  
  Text* pText0 = new Text;
  Text* pText1 = new Text;
  Text* pText2 = new Text;

  ASSERT(0 == pTextStack->nItems());
  
  pTextStack->vPush(pText0);
  ASSERT(1 == pTextStack->nItems());
  ASSERT(pText0 == pTextStack->pTop());
  ASSERT(pText0 == pTextStack->pPeek(0));

  pTextStack->vPush(pText1);
  ASSERT(2 == pTextStack->nItems());
  ASSERT(pText1 == pTextStack->pTop());
  ASSERT(pText1 == pTextStack->pPeek(1));
  
  pTextStack->vPush(pText2);
  ASSERT(3 == pTextStack->nItems());
  ASSERT(pText2 == pTextStack->pTop());
  ASSERT(pText2 == pTextStack->pPeek(2));

  ASSERT(pText0 == pTextStack->pPeek(0));
  ASSERT(pText1 == pTextStack->pPeek(1));
  ASSERT(pText2 == pTextStack->pPeek(2));

  ASSERT(pText2 == pTextStack->pPop());
  ASSERT(2 == pTextStack->nItems());

  ASSERT(pText1 == pTextStack->pPop());
  ASSERT(1 == pTextStack->nItems());

  ASSERT(pText0 == pTextStack->pPop());
  ASSERT(0 == pTextStack->nItems());

  delete pText2;
  delete pText1;
  delete pText0;

  delete pTextStack;
}

/*----------------------------------------------------------------------------*/
/* test2                                                                      */
/*----------------------------------------------------------------------------*/
void vTest2(void)
{
  HashMap* pTestMap = new HashMap();

  pTestMap->vInsertAtFront(10,20);
  pTestMap->vInsertAtFront(11,30);
  pTestMap->vInsertAtFront(5,10);

  ASSERT(1 == pTestMap->bExists(10));
  ASSERT(1 == pTestMap->bExists(11));
  ASSERT(1 == pTestMap->bExists(5));
  ASSERT(0 == pTestMap->bExists(9));

  delete pTestMap;
}

/*----------------------------------------------------------------------------*/
/* test3                                                                      */
/*----------------------------------------------------------------------------*/
void vTest3(void)
{
  byte xVlq[16];
  
  i8 nBytes = IntLib::iIntToVlq(C_DATUM_NULL,xVlq);
  i8 i = IntLib::iVlqToInt(xVlq);
  ASSERT(C_DATUM_NULL == i);
  
  nBytes = IntLib::iIntToVlq(0,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(0 == i);
  
  nBytes = IntLib::iIntToVlq(30,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(30 == i);
  
  nBytes = IntLib::iIntToVlq(-30,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(-30 == i);
  
  nBytes = IntLib::iIntToVlq(31,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(31 == i);
  
  nBytes = IntLib::iIntToVlq(32,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(32 == i);
  
  nBytes = IntLib::iIntToVlq(127,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(127 == i);
  
  nBytes = IntLib::iIntToVlq(128,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(128 == i);
  
  nBytes = IntLib::iIntToVlq(-130,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(-130 == i);
  
  nBytes = IntLib::iIntToVlq(-130,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(-130 == i);
  
  nBytes = IntLib::iIntToVlq(4096,xVlq);
  i = IntLib::iVlqToInt(xVlq);
  ASSERT(4096 == i);
  
}

/*----------------------------------------------------------------------------*/
/* test4                                                                      */
/*----------------------------------------------------------------------------*/
void vTest4(void)
{
  /*
  vTrace("vTest4\n"_Stz);

  HashMap* pHashMap = new HashMap(false,13999,8);

  //s32 sKey("ALPHA"_Stz);
  s32 sKey;

  i8 iKey = 0;
  for (i8 nRepeats = 0; nRepeats < 10; nRepeats++) {
    for (i8 nTimes = 0; nTimes < 1000000; nTimes++) {
      sKey.vSprint("%02lld"_Stz,iKey);

      pHashMap->vInsert((byte*)sKey.azChar(),sKey.nLength(),iKey,false);
      iKey++;
    }
    pHashMap->vAnalysis(false);
  }
  */

//  HashCursor xCursor(pHashMap);
//
//  while (xCursor.bIterate()) {
//    StrLib::vCopyLen(sKey.azChar(),(char*)xCursor.m_yaKey,xCursor.m_nBytes);
//    vTrace("[%lld] %s/%lld %lld\n"_Stz,
//           xCursor.m_ia_HashEntry[xCursor.m_i_Stack],
//           sKey.azChar(),
//           xCursor.m_nBytes,
//           xCursor.m_iValue);
//  }
  
//  sKey.vSprint("%02lld"_Stz,4206);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),4206,true);
//
//  sKey.vSprint("%02lld"_Stz,4249);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),4249,true);
//
//  sKey.vSprint("%02lld"_Stz,25396);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),25396,true);
//
//  sKey.vSprint("%02lld"_Stz,28848);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),28848,true);
//
//  sKey.vSprint("%02lld"_Stz,40263);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),40263,true);
//
//  sKey.vSprint("%02lld"_Stz,48122);
//  pHashMap->vInsert((byte*)sKey.azChar(),sKey.iLength(),48122,true);
  
  //pHashMap->vAnalysis(false);
  
//  delete pHashMap;
}

/*----------------------------------------------------------------------------*/
/* test5                                                                      */
/*----------------------------------------------------------------------------*/
void vTest5(void)
{
#define nINTS (C_1MB * 8)
  i8* aArray = (i8*)MemLib::aMallocBlock(8 * nINTS);
  
  for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
    aArray[i_Int] = i_Int;
  
  Timer oTimer;
  oTimer.vStart();
  
  i8 iSum = 0;
  for (i8 i_Times = 0; i_Times < 1000; i_Times++)
    for (i8 i_Int = 0; i_Int < nINTS; i_Int++)
      iSum += aArray[i_Int];

  oTimer.vUpdate();
  vTrace("duration %lldms %lld\n"_Stz,oTimer.iMilliseconds(),iSum);
}

/*----------------------------------------------------------------------------*/
/* test6                                                                      */
/*----------------------------------------------------------------------------*/
void vTest6(void)
{
  /*
  DicStack* pDicStack = AM::TASK::pDicStack();
  pDicStack->vPrint();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  SeApi* pSeApi = new SeApi;

  Stz* szCurrTenant = pSeApi->szCurrTenant();
  vTrace("\nTENANT %s\n"_Stz,szCurrTenant);
  Dic* pDic = pSeApi->pCurrTenant();
  //pDic->vListWords(0,true);
  
  Stz* szCurrCatalog = pSeApi->szCurrCatalog();
  vTrace("\nCATALOG %s\n"_Stz,szCurrCatalog);
  pDic = pSeApi->pCurrCatalog();
  //pDic->vListWords(0,true);

  Stz* szCurrSchema  = pSeApi->szCurrSchema();
  vTrace("\nSCHEMA %s\n"_Stz,szCurrSchema);
  pDic = pSeApi->pCurrSchema();
  //pDic->vListWords(0,true);

  Stz* szCurrTable   = pSeApi->szCurrTable();
  vTrace("\nTABLE %s\n"_Stz,szCurrTable);

  RowBuffer* pRowBuffer;
  pRowBuffer = pSeApi->pLookupCatalog(szCurrCatalog);
  delete pRowBuffer;
  pRowBuffer = pSeApi->pLookupSchema(szCurrCatalog,szCurrSchema);
  delete pRowBuffer;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("\n\n"_Stz,szCurrTable);
  pSeApi->vPrintMetadata();
  vTrace("\n\n"_Stz,szCurrTable);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("Test %s : %s \n"_Stz,
         "rapidsse"_Stz,
         pSeApi->bTest("rapidsse"_Stz)
           ? "found"_Stz : "not-found"_Stz);

  vTrace("Test %s.%s : %s \n"_Stz,
         "rapidsse"_Stz,"public"_Stz,
         pSeApi->bTest("rapidsse"_Stz,"public"_Stz)
           ? "found"_Stz : "not-found"_Stz);

  vTrace("Test %s.%s.%s : %s \n"_Stz,
         "rapidsse"_Stz,"public"_Stz,"lineitem"_Stz,
         pSeApi->bTest("rapidsse"_Stz,"public"_Stz,"lineitem"_Stz)
           ? "found"_Stz : "not-found"_Stz);
  
  vTrace("\n\n"_Stz);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  InfoSet* pInfoSet;
  pInfoSet = pSeApi->pLookupCatalog("n/a"_Stz);
  vTrace("LookupCatalog: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;
  pInfoSet = pSeApi->pLookupCatalog("rapidsse"_Stz);
  vTrace("LookupCatalog: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;
  
  pInfoSet = pSeApi->pLookupSchema("n/a"_Stz,
                                   "public"_Stz);
  vTrace("LookupSchema: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;
  pInfoSet = pSeApi->pLookupSchema("rapidsse"_Stz,
                                   "public"_Stz);
  vTrace("LookupSchema: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;
  
  pInfoSet = pSeApi->pLookupTable("n/a"_Stz,
                                  "public"_Stz,
                                  "lineitem"_Stz);
  vTrace("LookupTable: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;
  pInfoSet = pSeApi->pLookupTable("rapidsse"_Stz,
                                  "public"_Stz,
                                  "lineitem"_Stz);
  vTrace("LookupTable: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pInfoSet = pSeApi->pSearchMetadata("rapidsse"_Stz,
                                     nullptr,
                                     "lineitem"_Stz);
  vTrace("SearchMetadata: %lld rows\n"_Stz,pInfoSet->nInfoRows());
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    ColumnMetadata* pColMeta = static_cast<ColumnMetadata*>(pRow);

    SP_ColumnMetadata spColMeta;
    vTrace("META %s\n"_Stz,spColMeta.sz(pColMeta));
  }
  delete pInfoSet;
  
  delete pSeApi;
   
  */

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  InfoSet* pInfoSet;

  SeApi* pSeApi = new SeApi;

  vTrace("\n\n"_Stz);
  pSeApi->vPrintMetadata();
  vTrace("\n\n"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pColumnDefs("rapidsse"_Stz,
                                 "public"_Stz,
                                 "lineitem"_Stz,
                                 nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    ColumnMetadata* pColMeta = static_cast<ColumnMetadata*>(pRow);
    
    SP_ColumnMetadata spColMeta;
    vTrace("COLUMNMETA %s\n"_Stz,spColMeta.sz(pColMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pIndexColumnDefs("rapidsse"_Stz,
                                 "public"_Stz,
                                 "lineitem"_Stz,
                                 "IxL_ORDERKEY"_Stz,
                                 nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    IndexColumnMetadata* pColMeta = static_cast<IndexColumnMetadata*>(pRow);

    SP_IndexColumnMetadata spColMeta;
    vTrace("INDEX COLUMNMETA %s\n"_Stz,spColMeta.sz(pColMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pPartitionKey("rapidsse"_Stz,
                                   "public"_Stz,
                                   "lineitem"_Stz);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    PartitionKeyMetadata* pPtnKeyMeta = static_cast<PartitionKeyMetadata*>(pRow);
    
    SP_PartitionKeyMetadata spPtnKeyMeta;
    vTrace("PARTITIONKEY %s\n"_Stz,spPtnKeyMeta.sz(pPtnKeyMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search catalogs
  
  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchCatalogs(nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    CatalogMetadata* pCatalogMeta = static_cast<CatalogMetadata*>(pRow);

    SP_CatalogMetadata spCatalogMeta;
    vTrace("SEARCHCATALOG %s\n"_Stz,spCatalogMeta.sz(pCatalogMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search schemas
  
  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchSchemas(nullptr,nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    SchemaMetadata* pSchemaMeta = static_cast<SchemaMetadata*>(pRow);

    SP_SchemaMetadata spSchemaMeta;
    vTrace("SEARCHSCHEMA %s\n"_Stz,spSchemaMeta.sz(pSchemaMeta));
  }
  delete pInfoSet;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search tables
  
  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchTables(nullptr,nullptr,nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    TableMetadata* pTableMeta = static_cast<TableMetadata*>(pRow);

    SP_TableMetadata spTableMeta;
    vTrace("SEARCHTABLE %s\n"_Stz,spTableMeta.sz(pTableMeta));
  }
  delete pInfoSet;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search columns
  
  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchColumns(nullptr,nullptr,nullptr,nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    ColumnMetadata* pColumnMeta = static_cast<ColumnMetadata*>(pRow);

    SP_ColumnMetadata spColumnMeta;
    vTrace("SEARCHCOLUMN %s\n"_Stz,spColumnMeta.sz(pColumnMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search for nodeName columns
  
  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchColumns(nullptr,nullptr,nullptr,"nodeName"_Stz);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    ColumnMetadata* pColumnMeta = static_cast<ColumnMetadata*>(pRow);

    SP_ColumnMetadata spColumnMeta;
    vTrace("SEARCHCOLUMN %s\n"_Stz,spColumnMeta.sz(pColumnMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search indexes

  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchIndexes(nullptr,nullptr,nullptr,nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    IndexMetadata* pIndexMeta = static_cast<IndexMetadata*>(pRow);

    SP_IndexMetadata spIndexMeta;
    vTrace("SEARCHINDEXES %s\n"_Stz,spIndexMeta.sz(pIndexMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // search index columns

  vTrace("\n\n"_Stz);
  pInfoSet = pSeApi->pSearchIndexColumns(nullptr,nullptr,nullptr,nullptr,nullptr);
  for (Row* pRow = pInfoSet->pFirstRow();
       pRow;
       pRow = pInfoSet->pNextRow(pRow)) {
    IndexColumnMetadata* pIndexColumnMeta = static_cast<IndexColumnMetadata*>(pRow);

    SP_IndexColumnMetadata spIndexColumnMeta;
    vTrace("SEARCHCOLUMN %s\n"_Stz,spIndexColumnMeta.sz(pIndexColumnMeta));
  }
  delete pInfoSet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  delete pSeApi;

}

/*----------------------------------------------------------------------------*/
/* test7                                                                      */
/*----------------------------------------------------------------------------*/
void vTest7(void)
{
  vTrace("Arrange Test\n"_Stz);
  
  Arrange* pArrange = new Arrange;
  
  Dic* pDicRoot = AM::DIC::pRoot();
  
  Word* pWord = nullptr;
  
  while ( (pWord = pDicRoot->pEachWord(pWord)) )
    pArrange->vInsert(pWord->szWordNm(),pWord);
  
  pArrange->vSequence();
  
  void* vpCursor;

  //pArrange->vBeginIter(&vpCursor);
  
  for (pArrange->bFirst(&vpCursor);
       vpCursor;
       pArrange->bNext(&vpCursor))
    vTrace("  [%03lld] %s\n"_Stz,
           pArrange->iSeqNo(&vpCursor),
           pArrange->szObjNm(&vpCursor));
  
  //pArrange->vEndIter(&vpCursor);

  delete pArrange;
  
  vTrace("End Arrange Test\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* test8                                                                      */
/*----------------------------------------------------------------------------*/
void vTest8(void)
{
  vTrace("Test 8\n"_Stz);
  vTrace("original command: %s\n"_Stz,SeApi::szOriginalCommand());

  Text* pText;
  Text* pText0 = new Text("A"_Stz);
  Text* pText1 = new Text("B"_Stz);
  Text* pText2 = new Text("C"_Stz);
  Text* pText3 = new Text("D"_Stz);
  Text* pText4 = new Text("E"_Stz);
  Text* pText5 = new Text("F"_Stz);

  ListExo_t<CN_CLASSPLACEHOLDER,Text>* pListExo;
  
  pListExo = new ListExo_t<CN_CLASSPLACEHOLDER,Text>;
  
  pListExo->vAppend(pText0);
  pListExo->vAppend(pText1);
  pListExo->vAppend(pText2);
  pListExo->vAppend(pText3);
  pListExo->vAppend(pText4);
  pListExo->vAppend(pText5);

//  vTrace("0: %s\n"_Stz,pListExo->pPeek(0)->szGet());
//  vTrace("1: %s\n"_Stz,pListExo->pPeek(1)->szGet());
//  vTrace("2: %s\n"_Stz,pListExo->pPeek(2)->szGet());

  //ListExoIter_t<CN_CLASSPLACEHOLDER,Text>* pListExoIter = pListExo->pNewIter();
  ListExoIter_t<Text>* pListExoIter = pListExo->pNewIter();

  // - - - -
  
  vTrace("\nSeq (a) : testing 6 items : pNewIter() ; while ( pNext() )\n"_Stz);
  
  while (pListExoIter->bHasNext()) {
    pText = pListExoIter->pNext();
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  // - - - -

  vTrace("\nSeq (b) : testing 6 items : vBeforeFirst(); while ( pNext() )\n"_Stz);
  
  pListExoIter->vBeforeFirst();
  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  // - - - -

  vTrace("\nSeq (c) : testing 6 items : pFirst(), pNext(), pNext()\n"_Stz);

  pText = pListExoIter->pFirst();
  vTrace("pFirst: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  ASSERT(! pText);
  
  // - - - -
  
  vTrace("\nSeq (c) : testing 6 items : pNext(), pNext(), pNext()\n"_Stz);
  pListExoIter->vBeforeFirst();

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  
  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  
  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  vTrace("pNext: %s\n"_Stz,pText->szGet());
  vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");

  pText = pListExoIter->pNext();
  ASSERT(! pText);

  // - - - -

  vTrace("\nSeq (d) : testing 3 items : while ( pNext() )\n"_Stz);
  pListExoIter->vBeforeFirst();

  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }
  
  // - - - -
  
  pListExo->vRemoveAll();

  vTrace("\nSeq (e) : testing empty list : while ( pNext() )\n"_Stz);
  pListExoIter->vBeforeFirst();

  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  pListExo->vAppend(pText0);
  pListExo->vAppend(pText1);
  pListExo->vAppend(pText2);
  pListExo->vAppend(pText3);
  pListExo->vAppend(pText4);
  pListExo->vAppend(pText5);

  // - - - -

  vTrace("\nSeq (f) : testing bIsBeforeFirst(), bIsAfterLast()\n"_Stz);
  pListExoIter->vBeforeFirst();
  vTrace("bIsBeforeFirst = %s; bIsAfterLast = %s\n"_Stz,
         pListExoIter->bIsBeforeFirst() ? "true"_Stz : "false"_Stz,
         pListExoIter->bIsAfterLast() ? "true"_Stz : "false"_Stz);
  pListExoIter->vAfterLast();
  vTrace("bIsBeforeFirst = %s; bIsAfterLast = %s\n"_Stz,
         pListExoIter->bIsBeforeFirst() ? "true"_Stz : "false"_Stz,
         pListExoIter->bIsAfterLast() ? "true"_Stz : "false"_Stz);

  // - - - -

  vTrace("\nSeq (g) : testing reverse iteration : while ( bHasPrev() )\n"_Stz);
  pListExoIter->vAfterLast();
  while (pListExoIter->bHasPrev()) {
    pText = pListExoIter->pPrev();
    vTrace("pPrev: %s\n"_Stz,pText->szGet());
    vTrace("bHasPrev: %s\n"_Stz,pListExoIter->bHasPrev() ? "true" : "false");
  }

  // - - - -

  vTrace("\nSeq (h) : testing removal of head node\n"_Stz);
  pListExoIter->vBeforeFirst();
  pListExoIter->pNext();
  pListExoIter->vRemove();

  pListExoIter->vBeforeFirst();
  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  pListExo->vPush(pText0);  // replace

  // - - - -

  vTrace("\nSeq (i) : testing removal of tail node\n"_Stz);

  pListExoIter->vBeforeFirst();
  while (pListExoIter->bHasNext()) {
    pListExoIter->pNext();
  }
  pListExoIter->vRemove();

  pListExoIter->vBeforeFirst();
  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  pListExo->vAppend(pText5);  // replace

  // - - - -

  vTrace("\nSeq (j) : testing removal of middle node (C) moving forwards\n"_Stz);

  pListExoIter->vBeforeFirst();
  pListExoIter->pNext();
  pListExoIter->pNext();
  pListExoIter->pNext();
  pListExoIter->vRemove();

  pListExoIter->vBeforeFirst();
  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  pListExo->vInsertAfter(pText2, pText1);

  // - - - -

  vTrace("\nSeq (k) : testing removal of middle node (D) moving backwards\n"_Stz);

  pListExoIter->vAfterLast();
  pListExoIter->pPrev();
  pListExoIter->pPrev();
  pListExoIter->pPrev();
  pListExoIter->vRemove();

  pListExoIter->vBeforeFirst();
  while ( (pText = pListExoIter->pNext()) ) {
    vTrace("pNext: %s\n"_Stz,pText->szGet());
    vTrace("bHasNext: %s\n"_Stz,pListExoIter->bHasNext() ? "true" : "false");
  }

  pListExo->vInsertAfter(pText3, pText2);


  delete pListExoIter;
  

  delete pText5;
  delete pText4;
  delete pText3;
  delete pText2;
  delete pText1;
  delete pText0;
  
  delete pListExo;
}

/*----------------------------------------------------------------------------*/
/* test9                                                                      */
/*----------------------------------------------------------------------------*/
//#define nElem (1024*1024*16)
//i8 xI8[nElem];
//
//void vTest9(void)
//{
//  i8 iSum = 0;
//
//  vTrace("iPmc_Info %llx\n"_Stz,iPmc_Info());
//
//  vTrace("Starting   iPmc_GetLlcMisses %lld\n"_Stz,iPmc_GetLlcMisses());
//  for (i8 i = 0; i < nElem; i++)
//    xI8[i] = i;
//  //vTrace("Sequential iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,iPmc_GetLlcMisses(),iSum);
//  for (i8 i = 0; i < nElem; i++) {
//    i8 iElem = Random::iInteger(nElem-1);
//    //vTrace("iElem %lld; iPmc_GetLlcMisses %llx\n"_Stz,iElem,iPmc_GetLlcMisses());
//    iSum += xI8[iElem];
//  }
//  vTrace("Random     iPmc_GetLlcMisses %lld; iSum %lld\n"_Stz,iPmc_GetLlcMisses(),iSum);
//}

/*----------------------------------------------------------------------------*/
/* test10                                                                      */
/*----------------------------------------------------------------------------*/
void vTest10(void)
{
  
  i8 i1 = RndLib::iInteger(-999999,999999);
  i8 i2 = RndLib::iInteger(-999999,999999);
  
  for (i8 i = 0; i < 10000000; i ++) {
    //i8 i1 = Random::iInteger(-999999,999999);
    //i8 i2 = Random::iInteger(-999999,999999);
  
    i8 iCmp = MemLib::iCompareWords(&i1, &i2, 8);
    UNUSED(iCmp);
    //i8 iCmp2 = MemLib::iCompare2(&i1, &i2, 8);
  }
  
  
//    if (iCmp > 0)
//      vTrace("%lld > %lld\n"_Stz, i1, i2);
//    else if (iCmp < 0)
//      vTrace("%lld > %lld\n"_Stz, i2, i1);
//    else
//      vTrace("%lld == %lld\n"_Stz, i1, i2);
  
}

/*----------------------------------------------------------------------------*/
/* test11 --first string contains                                             */
/*----------------------------------------------------------------------------*/
void vTest11(void)
{
  char* azString = (char*)"the quick brown fox jumped over the lazy dog yell";
  char* azSubStr = (char*)"yellow";

  char* azString_cursor_outer = azString;
  char* azString_cursor_inner;

  char* azSubstr_cursor;

  L_NEXT_OUTER:
  while (*azString_cursor_outer) {
    
    azString_cursor_inner = azString_cursor_outer;
    azSubstr_cursor       = azSubStr;
    
    while (*azString_cursor_inner && *azSubstr_cursor)
      if (*azSubstr_cursor++ != *azString_cursor_inner++) {
        azString_cursor_outer++;
        goto L_NEXT_OUTER;
      }
    
    vTrace("Found\n"_Stz);
    return; // true
  }
  
  vTrace("Not Found\n"_Stz);
  return; // false
}

/*----------------------------------------------------------------------------*/
/* test12 -- second string contains                                           */
/*----------------------------------------------------------------------------*/
void vTest12(void)
{
  char* azString = (char*)"the quick brown fox jumped over the lazy dog";
  char* azSubStr = (char*)          "brown";

  
  char* azString_outer = azString;
  char* azString_limit = azString + strlen(azString) - strlen(azSubStr);
    
  char* azString_inner;
  char* azSubstr_cursor;

  char cStringChar;
  char cSubstrChar;

  char cSubStart = *azSubStr;
  while (azString_outer <= azString_limit) {
    
    if (*azString_outer != cSubStart) {
      azString_outer++;
      continue;
    }
    
    azString_inner  = azString_outer;
    azSubstr_cursor = azSubStr;
      
    cSubstrChar = *azSubstr_cursor;
    cStringChar = *azString_inner;
    while (cSubstrChar && cSubstrChar == cStringChar) {
      cSubstrChar = *++azSubstr_cursor;
      cStringChar = *++azString_inner;
    }

    if (! cSubstrChar) {
      vTrace("Found\n"_Stz);
      return; // true
    }
    
    azString_outer++;
  }
  
  vTrace("Not Found\n"_Stz);
  return; // false
}

/*----------------------------------------------------------------------------*/
/* test13                                                                     */
/*----------------------------------------------------------------------------*/
void vTest13(void)
{
  union Dte_T xDte;
  
  xDte.m_uDay       = 25;
  xDte.m_uMonth     = 3;
  xDte.m_uYearDelta = 1961 - 1900;
  
  i8 iYear = xDte.m_uYearDelta + 1900;
  
  vTrace("Year %lld\n"_Stz,iYear);

}

/*----------------------------------------------------------------------------*/
/* test14 -- API for Compiler calls >interpret                                */
/*----------------------------------------------------------------------------*/
void vTest14(void)
{

}

/*----------------------------------------------------------------------------*/
/* test15                                                                     */
/*----------------------------------------------------------------------------*/
void vTest15(void)
{
}

/*----------------------------------------------------------------------------*/
/* test16                                                                     */
/*----------------------------------------------------------------------------*/
void vTest16(void)
{
}

/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* test17                                                                     */
/*----------------------------------------------------------------------------*/
void vTest17(void)
{
}

/*----------------------------------------------------------------------------*/
/* test18                                                                     */
/*----------------------------------------------------------------------------*/
void vTest18(void)
{
  s32 sFolderNm;
  vTrace("\nCurrentFolderNm : %s\n"_Stz,
         SeApi::szCurrentFolderNm(sFolderNm));
  
  s256 sFolderPath;
  vTrace("\nCurrentFolderPath : %s\n"_Stz,
         SeApi::szCurrentFolderPath(sFolderPath));
  
  vTrace("\nCurrent Folder Contents:\n"_Stz);
  SeApi::vPrintAllTablesInCurrentFolder();

  //
  // There is a hidden/metatable called __listColumns.  Lets us that.
  //
  
  if (SeApi::bIsKnownTableNm("__listColumns"_Stz))
    vTrace("\n__listColumns is a known table\n"_Stz);
  else
    vTrace("\n__listColumns is not a known table\n"_Stz);

  vTrace("\nColumns of __listColumns:\n"_Stz);
  SeApi::vPrintAllColumnsOfTable("__listColumns"_Stz);
  
  if (SeApi::bIsKnownTableColumnNm("__listColumns"_Stz,"columnName"_Stz))
    vTrace("\n__listColumns.columnName is a known column\n"_Stz);
  else
    vTrace("\n__listColumns.columnName is not a known column\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* test19 :                                                                   */
/*----------------------------------------------------------------------------*/
void vTest19(void)
{
  Stz* szRegExp;
  Stz* szText = "the quick brown fox jumped over the lazy dog"_Stz;
  
  for (i8 i_Test = 0; /* none */; i_Test++) {
    switch (i_Test) {
      case 0 : szRegExp = "^the"_Stz;      break;
      case 1 : szRegExp = "^quick"_Stz;    break;
      case 2 : szRegExp = "*quick"_Stz;    break;
      case 3 : szRegExp = "he *quick"_Stz; break;
      case 4 : szRegExp = ".*fox.*over.*"_Stz; break;
      default:
        return;
    }
    vTrace("%lld : %s\n"_Stz
           ,(i8)StrLib::bMatch((char*)szText,(char*)szRegExp)
           ,szRegExp);
  }
}

/*----------------------------------------------------------------------------*/
/* test20 : row slab test                                                     */
/*----------------------------------------------------------------------------*/
void vTest20(void)
{
}

/*----------------------------------------------------------------------------*/
/* test21 : determine the number of int entries which fit into an ent buffer  */
/*----------------------------------------------------------------------------*/
i8 nTest21_intEnts(void)
{
  Rrf xRef;
  EntWrk xEntWrk;
  
  i8 nEnts = 0;

  EntBuffer* pEntBufferCalc = nullptr; // new EntBuffer(nullptr);
  
  for (;;) {
    
    i8 nOffsets = AM::nRndSkipLevel();
    
    //vTrace("%lld "_Stz,iInt);
    xEntWrk.vAppendInt(nEnts);
    xEntWrk.vOffsets(nOffsets);
    
    i8 nBytesNeeded = xEntWrk.nTotalSize();
    if (! pEntBufferCalc->bHasSpace(nBytesNeeded))
      break;
    
    //pEntBufferCalc->vShow();
    Ent* aEntToIns = xEntWrk.aEnt();
    Ent* aEnt = pEntBufferCalc->aInsertEntAbs(aEntToIns);
    UNUSED(aEnt);
    nEnts++;
    
    xEntWrk.vReset();

  }
  
  delete pEntBufferCalc;
  
  return nEnts;
}

/*----------------------------------------------------------------------------*/
/* test21 : create an int set for key samples                                 */
/*----------------------------------------------------------------------------*/
IntSet* pTest21_keySamples(i8 nSamples)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // first calculate how many int entries fit into an EntBuffer
  
  i8 nEntsWhichFit = C_UNKNOWN == nSamples ? nTest21_intEnts() : nSamples;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create that many keys

  IntSet* pKeySamples = new IntSet(nEntsWhichFit);
  vTrace("KeySamples of %lld ints\n"_Stz,pKeySamples->nInts());
  
  return pKeySamples;
}

/*----------------------------------------------------------------------------*/
/* test21 : print test results                                                */
/*----------------------------------------------------------------------------*/
void vTest21_printResults(Stz* szTestFn, IntSet* pKeySamples,
                         AggTimer* pAggTimer, i8 nTotalEnts)
{
  SP_Bytes spBytes;
  SP_Timer spTimer1;
  SP_Timer spTimer2;

  i8 nElapsedNsec = pAggTimer->iNanoseconds();
  d8 dRatePerSec  = pAggTimer->dRatePerSec(nTotalEnts);

  i8 nBytesPerDay = (i8)(dRatePerSec * C_SECS_PER_DAY * 128 * 48);
  
  vTrace("\n"_Stz);
  vTrace("%s of %lld %s entries took %s "
         ": %s/ent : %6.2f ent/s "
         ": %4.2fs/1M "
         ": %4.2fs for SF100 LineItem over 48ptn "
         ": %s per day"
         "\n"_Stz,
         
         szTestFn,
         nTotalEnts,
         pKeySamples->szDesc(),
         spTimer1.sz(nElapsedNsec),
         
         spTimer2.sz(nElapsedNsec / nTotalEnts),
         dRatePerSec,
         
         (d8)C_1M / dRatePerSec,
         
         (d8)(C_TPCH_LINEITEM * 100) / dRatePerSec / 48,
         
         spBytes.sz(nBytesPerDay)
         
         );
}

/*----------------------------------------------------------------------------*/
/* test21 : delete entries from given EntBuffer using KeySamples              */
/*----------------------------------------------------------------------------*/
void vTest21_delete(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalDel_)
{
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();
  
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iInt(i_Int);
        
    // key
    xEntWrk.vAppendInt(iInt);
        
    Ent* aPriorEnt = pEntBuffer->aFindEntryBefore(xEntWrk.aKey());
    RASSERT(aPriorEnt);
    
    Ent* aDeleteEnt = pEntBuffer->aNextEntry(aPriorEnt,0);
    
    if (aDeleteEnt) {
      bool b = Key::bEqualTo(aDeleteEnt->aKey(),xEntWrk.aKey());
      
      if (b) {
        pEntBuffer->vDeleteEntry(&aDeleteEnt);
        //if (nTotalDel_ > 16000)
        //  pEntBuffer->vShow();
        nTotalDel_++;
      }
    }

    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 : insert entries into EntBuffer using KeySamples                    */
/*----------------------------------------------------------------------------*/
void vTest21_insert(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalIns_)
{
  Rrf xRef;
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();
  
  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iInt(i_Int);
    
    // ref
    xRef.vBuffer(0);
    xRef.vItem(iInt);
    xRef.vKind(pEntBuffer->eEntKind());
    xEntWrk.vRef(xRef);
    
    // key
    xEntWrk.vAppendInt(iInt);
    
    // neo
    i8 nOffsets = AM::nRndSkipLevel();
    xEntWrk.vOffsets(nOffsets);
    
    i8 nBytesNeeded = xEntWrk.nTotalSize();
    if (! pEntBuffer->bHasSpace(nBytesNeeded))
      break;
    
    Ent* aEnt = pEntBuffer->aInsertEntAbs(xEntWrk.aEnt());
    UNUSED(aEnt);
    
    //pEntBuffer->vShow();
    
    nTotalIns_++;
    
    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 : search entries in EntBuffer using KeySamples                      */
/*----------------------------------------------------------------------------*/
void vTest21_search(EntBuffer* pEntBuffer,IntSet* pKeySamples, i8& nTotalSel_)
{
  EntWrk xEntWrk;

  i8 nInts = pKeySamples->nInts();

  if (KE_INT == pEntBuffer->eEntKind()) {
    for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
      i8 iInt = pKeySamples->iInt(i_Int);
      UNUSED(iInt);
      //Ent* aSearchedEnt = pEntBuffer->aFindEnt_IntOpt(iInt);

      nTotalSel_++;
    }
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  for (i8 i_Int = 0; i_Int < nInts; i_Int++) {
          
    i8 iInt = pKeySamples->iInt(i_Int);
        
    // key
    xEntWrk.vAppendInt(iInt);
        
    Ent* aPriorEnt = pEntBuffer->aFindEntryBefore(xEntWrk.aKey());
    RASSERT(aPriorEnt);
    
    Ent* aSearchedEnt = pEntBuffer->aNextEntry(aPriorEnt,0);
    
    if (aSearchedEnt) {
      bool b = Key::bEqualTo(aSearchedEnt->aKey(),xEntWrk.aKey());
      UNUSED(b);
    }
    //RASSERT(aSearchedEnt);

    nTotalSel_++;
    
    xEntWrk.vReset();
  }

}

/*----------------------------------------------------------------------------*/
/* test21 :sort the ent buffer                                                */
/*----------------------------------------------------------------------------*/
void vTest21_sort(EntBuffer* pEntBuffer)
{
  EntBufSorter* pEntBufSorter = new EntBufSorter;
  
  pEntBufSorter->vSort_int(pEntBuffer);
  
  delete pEntBufSorter;
}

/*----------------------------------------------------------------------------*/
/* test21 : run delete test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testDelete(IntSet* pKeySamples,
                        i8 nBuffers, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalDel = 0;
  
  Timer    xTimer;
  AggTimer xAggTimer;

  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer(nullptr);

    vTest21_insert(pEntBuffer,pKeySamples,nTotalDel);
    
    xTimer.vStart();
    
    vTest21_delete(pEntBuffer,pKeySamples,nTotalDel);
    if (bDump) {
      bDump = false;
      pEntBuffer->vDump();
    }

    xTimer.vUpdate();
    xAggTimer.vAdd(&xTimer);

    delete pEntBuffer;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("vDeleteEntry()"_Stz,pKeySamples,&xAggTimer,nTotalDel);
}

/*----------------------------------------------------------------------------*/
/* test21 : run insert test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testInsert(IntSet* pKeySamples,
                        i8 nBuffers, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalIns = 0;
  
  Timer    xTimer;
  AggTimer xAggTimer;

  xTimer.vStart();
  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer(nullptr);

    vTest21_insert(pEntBuffer,pKeySamples,nTotalIns);
    if (bDump) {
      bDump = false;
      pEntBuffer->vDump();
    }
    delete pEntBuffer;
  }

  xTimer.vUpdate();

  xAggTimer.vAdd(&xTimer);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("aInsertEntAbs()"_Stz,pKeySamples,&xAggTimer,nTotalIns);
}

/*----------------------------------------------------------------------------*/
/* test21 : run search test                                                   */
/*----------------------------------------------------------------------------*/
void vTest21_testSearch(IntSet* pKeySamples,
                        i8 nBuffers, i8 nTimes, bool bDump)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create a number of EntBuffers using the key samples
  
  i8 nTotalIns = 0;
  i8 nTotalSel = 0;

  Timer    xTimer;
  AggTimer xAggTimer;

  
  for (i8 i_Buf = 0; i_Buf < nBuffers; i_Buf++) {
    EntBuffer* pEntBuffer = new EntBuffer(nullptr);

    vTest21_insert(pEntBuffer,pKeySamples,nTotalIns);
    
//    if (bDump) {
//      pEntBuffer->vDump();
//    }

    Timer    xSortTimer;
    xSortTimer.vStart();
    vTest21_sort(pEntBuffer);
    xSortTimer.vUpdate();
    SP_Timer spTimer1;
    //vTrace("Time to sort : %s\n"_Stz,spTimer1.sz(xSortTimer.iNanoseconds()));

    if (bDump) {
      //bDump = false;
      pEntBuffer->vDump(false);
    }

    xTimer.vStart();
    
    for (i8 i_Loop = 0; i_Loop < nTimes; i_Loop++)
      vTest21_search(pEntBuffer,pKeySamples,nTotalSel);

    xTimer.vUpdate();
    xAggTimer.vAdd(&xTimer);

    delete pEntBuffer;
  }

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // print timing results
  
  vTest21_printResults("aPositionPriorEntry()"_Stz,pKeySamples,&xAggTimer,nTotalSel);
}

/*----------------------------------------------------------------------------*/
/* test21 : EntBuffer Test -- tune the insert of ent into skiplist of 1 buffer*/
/*----------------------------------------------------------------------------*/
void vTest21(void)
{
  i8   nSamples = C_UNKNOWN;
  i8   nBuffers = 10;
  i8   nTimes   = 10;
  bool bDump    = true;

  //bool bInsertTest = true;
  bool bSearchTest = true;
  //bool bDeleteTest = true;
  
  vTrace("Test21 -- This is EntBuffer Test MODE: %s %s\n"_Stz,
         AM::szBuild(),
         AM::bDeveloper() ? "DEVELOPER"_Stz : ""_Stz
         );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create key sample int set

  IntSet* pKeySamples = pTest21_keySamples(nSamples);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // run our tests
  
  pKeySamples->vAscending();
//  if (bInsertTest) vTest21_testInsert(eEntKind,pKeySamples,nBuffers,bDump);
//  if (bSearchTest) vTest21_testSearch(eEntKind,pKeySamples,nBuffers,nTimes,bDump);
//  if (bDeleteTest) vTest21_testDelete(eEntKind,pKeySamples,nTimes,bDump);

  pKeySamples->vDescending();
//  if (bInsertTest) vTest21_testInsert(eEntKind,pKeySamples,nBuffers,bDump);
//  if (bSearchTest) vTest21_testSearch(eEntKind,pKeySamples,nBuffers,nTimes,bDump);
//  if (bDeleteTest) vTest21_testDelete(eEntKind,pKeySamples,nTimes,bDump);

  pKeySamples->vRandomise();
//  if (bInsertTest) vTest21_testInsert(eEntKind,pKeySamples,nBuffers,bDump);
  if (bSearchTest) vTest21_testSearch(pKeySamples,nBuffers,nTimes,bDump);
//  if (bDeleteTest) vTest21_testDelete(eEntKind,pKeySamples,nTimes,bDump);
}

/*----------------------------------------------------------------------------*/
/* test22                                                                     */
/*----------------------------------------------------------------------------*/
void vTest22(void)
{
}

/*----------------------------------------------------------------------------*/
/* test23                                                                     */
/*----------------------------------------------------------------------------*/
void vTest23(void)
{
  DicStack* pDicStack = MY::pDicStack();
  s256 sPath;
  
  pDicStack->vToPathStr(sPath);
  
  vTrace("Path: %s\n"_Stz,sPath.szStz());
  
  i8 nDics = pDicStack->nItems();
  
  for (i8 i_Dic = 0; i_Dic < nDics; i_Dic++) {
    Dic* pDic = pDicStack->pPick(i_Dic);
    vTrace("  %s %s\n"_Stz,pDic->szDicNm(),pDic->szDicKind());
  }
  
  vTrace("Path: %s %s %s\n"_Stz,
         MY::szTenantNm(),MY::szCatalogNm(),MY::szSchemaNm());
}

/*----------------------------------------------------------------------------*/
/* test24                                                                     */
/*----------------------------------------------------------------------------*/
void vTest24(void)
{
}

/*----------------------------------------------------------------------------*/
/* test25                                                                     */
/*----------------------------------------------------------------------------*/
void vTest25(void)
{
  IntSet* pIntSet = new IntSet(64);

  pIntSet->vAscending();          pIntSet->vPrint(false);
  pIntSet->vDescending();         pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandoms(0,16);        pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandomsUnique(0,64);  pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandomsUnique(0,128); pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);

  pIntSet->vRandoms(0,128);       pIntSet->vPrint(false);
  pIntSet->vSort();               pIntSet->vPrint(false);
  pIntSet->vSortDescend();        pIntSet->vPrint(false);
}

/*----------------------------------------------------------------------------*/
/* test26                                                                     */
/*----------------------------------------------------------------------------*/
void vTest26(void)
{
  bool bDumpTable = false;
  i8 nSamples = C_32KB;

  i8 i_NextRnd = C_UNKNOWN;
  
  static const i8 C_FRQ = C_MAX_LVL_LEVELS;
  static const i8 C_LVL = C_MAX_LVL_RATIO;
  static const i8 C_CNT = 8;

  i8 iaCounts[C_LVL+1][C_FRQ+1][C_CNT+1];
  
  for (i8 i_Lvl = 0; i_Lvl <= C_LVL; i_Lvl++)
    for (i8 i_Frq = 0; i_Frq <= C_FRQ; i_Frq++)
      for (i8 i_Cnt = 0; i_Cnt <= C_CNT; i_Cnt++)
        iaCounts[i_Lvl][i_Frq][i_Cnt] = 0;


  if (bDumpTable) {
    vTrace("L     3           4           5\n"_Stz);
    vTrace("F 1 2 3 4 5   1 2 3 4 5   1 2 3 4 5\n"_Stz);
    vTrace("  - - - - -   - - - - -   - - - - -\n"_Stz);
  }
  
  for (i8 i_Sample = 0; i_Sample < nSamples; i_Sample++) {
    if (bDumpTable)
      vTrace("  "_Stz);

    for (i8 i_Lvl = 3; i_Lvl <= C_LVL; i_Lvl++) {

      if (bDumpTable)
        if (i_Lvl > 3)
          vTrace(": "_Stz);

      for (i8 i_Frq = 2; i_Frq <= C_FRQ; i_Frq++) {

        //i8 iLvl = RndLib::iLogLevel(nLevels,iFreq,i_NextRnd);
        i8 i_Cnt = RndLib::iSkipLevel(i_Lvl,i_Frq,i_NextRnd) ;
        if (bDumpTable)
          vTrace("%lld "_Stz,i_Cnt);

        iaCounts[i_Lvl][i_Frq][i_Cnt]++;
        
      } // freq
      
    } // level
    if (bDumpTable)
      vTrace("\n"_Stz);
    

  } // sample
  
  i8 iXchk = 0;
  vTrace("\n"_Stz);
  for (i8 i_Lvl = 3; i_Lvl <= C_LVL; i_Lvl++) {
    vTrace("%lld Levels\n"_Stz,i_Lvl);

    for (i8 i_Frq = 2; i_Frq <= C_FRQ; i_Frq++) {
      vTrace("Ratio %lld: "_Stz,i_Frq);

      iXchk = 0;
      for (i8 i_Cnt = 1; i_Cnt <= C_CNT; i_Cnt++) {
        if (i_Cnt == 1)
          vTrace("%lld "_Stz,iaCounts[i_Lvl][i_Frq][i_Cnt]);
        else {
          i8 iRatio = iaCounts[i_Lvl][i_Frq][i_Cnt]
                        ? iaCounts[i_Lvl][i_Frq][i_Cnt - 1] /
                          iaCounts[i_Lvl][i_Frq][i_Cnt]
                        : 0;
                      
          vTrace("/%lld=%lld "_Stz,iRatio,iaCounts[i_Lvl][i_Frq][i_Cnt]);
        }
        iXchk += iaCounts[i_Lvl][i_Frq][i_Cnt];
      }
      vTrace(" (iXchk=%lld)\n"_Stz,iXchk);
    }
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* test27 : SP_bits test (along with using IntLib::iBytePower() striding      */
/*----------------------------------------------------------------------------*/
void vTest27(void)
{
  u8 i;
  i8 iStart;
  i8 iStride;
  
  i8 iLoop;

  i8 iTimes;

  SP_Bits spBits;
  
  iTimes = 16;
    
  for (i8 iPower = 0; iPower < (i8)C_BYTES_PER_WORD; iPower += 1) {
    
    iStart  = IntLib::iBytePower(iPower);
    iStride = iStart ? iStart : 1;
    
    i = iStart;

    for (iLoop = 0; iLoop < iTimes; iLoop++, i += iStride)
      vTrace("[%02lld] %20llu %s %016llx Power=%lld Start=%llx Stride=%llx\n"_Stz,
             iLoop,i,spBits.sz(i),i,iPower,iStart,iStride);
    vTrace("...\n"_Stz);
    
    iStride = iStride * 16;
    i = iStride;
    for (iLoop = 0; iLoop < iTimes - 1; iLoop++, i += iStride)
      vTrace("[%02lld] %20llu %s %016llx Power=%lld Start=%llx Stride=%llx\n"_Stz,
             iLoop,i,spBits.sz(i),i,iPower,iStart,iStride);
    vTrace("\n"_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* test28 -- visualise random levels                                          */
/*----------------------------------------------------------------------------*/
void vTest28(void)
{
  i8 i_NextRnd = C_UNKNOWN;
  
  i8 iTimes = C_2KB;
  i8 iLevel;
  
  i8 nLvlLevels = AM::nLvlLevels();
  i8 iLvlRatio  = AM::iLvlRatio();

  
  for (i8 i = 0; i < iTimes; i++) {
    iLevel = RndLib::iLogLevel(nLvlLevels,iLvlRatio,i_NextRnd);
    //vTrace("[%02lld] %lld "_Stz,i,iLevel);
    if (i && ! (i % 64))
      vTrace("\n"_Stz);
    if (3 == iLevel)
      vTrace("$ "_Stz);
    else
    if (1 == iLevel)
      vTrace("  "_Stz);
    else
      vTrace(". "_Stz);

  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* test29                                                                     */
/*----------------------------------------------------------------------------*/
void vTest29(void)
{
  i8 iTimes = 32;
  
  i8 i_NextRnd = C_UNKNOWN;

  for (i8 i = 0; i < iTimes; i++) {
    i8 iLevel = RndLib::iSkipLevel(3,3,i_NextRnd);
    vTrace("Level = %lld\n"_Stz,iLevel);
  }

}

/*----------------------------------------------------------------------------*/
/* test30 : observe generation of random bit patterns                         */
/*----------------------------------------------------------------------------*/
void vTest30(void)
{
 i8 iTimes = 32;
 u8 uPattern;
 
  SP_Bits spBits;

 for (i8 i = 0; i < iTimes; i++) {
   uPattern = RndLib::uInteger();
   vTrace("[%02lld] %s\n"_Stz,i,spBits.sz(uPattern));
 }
}

/*----------------------------------------------------------------------------*/
/* test31                                                                     */
/*----------------------------------------------------------------------------*/
void vTest31(void)
{
  vTrace("Test31 -- Lite tokenise of file\n"_Stz);
  File* pFile = new File(nullptr);
  
  bool b = pFile->bOpenRead("../src/store/LogicUnit.cpp"_Stz);
  RASSERT(b);
  
  s4K sTok;
  while (pFile->bNextLiteTok(sTok)) {
    
    if (sTok.bSame("EO_ADD_INTINT:"_Stz)) {
      
      do {
        vTrace(">>%s<<\n"_Stz,sTok.szStz());
        if (! (pFile->bNextLiteTok(sTok)))
          goto L_Close;
      } while (! sTok.bSame("break;//+endCase"_Stz));
      vTrace(">>%s<<\n"_Stz,sTok.szStz());
      goto L_Close;
    }
  
  }
  
  L_Close:
  pFile->vClose();
  
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* test32                                                                     */
/*----------------------------------------------------------------------------*/
void vTest32(void)
{
  Location* pLocation = new Location;
  
  pLocation->vSet();
  pLocation->vPrint();
  
  Locator* pLocator = new Locator(pLocation);

  delete pLocator;
  delete pLocation;
}

/*----------------------------------------------------------------------------*/
/* test33                                                                     */
/*----------------------------------------------------------------------------*/

#include "SP_DecVal.hpp"

void vTest33(void)
{
  vTrace("vTest33 -- Decimal Experiments\n"_Stz);
      
  DecVal xArgA;
  DecVal xArgB;
  DecVal xResC;

//  xArgA.vAssign(20000,0);
//  xArgB.vAssign(30000,0);
  xArgA = "20000."_Dec;
  xArgB = "30000."_Dec;

  for (xArgA.m_nPlaces = 0; xArgA.m_nPlaces <= 4; xArgA.m_nPlaces++) {
    for (xArgB.m_nPlaces = 0; xArgB.m_nPlaces <= 4; xArgB.m_nPlaces++) {
      vTrace("%lld x %lld places: "_Stz,xArgA.m_nPlaces,xArgB.m_nPlaces);
      //xResC.vMultiply(xArgA,xArgB);
      xResC = xArgA * xArgB;
      vTrace("\n"_Stz);
    }
  }
  
  vTrace("- - - - - - -\n"_Stz);

  DecVal xuResC;
  DecVal xrResC;
  SP_DecVal spResC;

  xuResC.vAssign(12345678,4);
  vTrace("%s | rounded down to: "_Stz,spResC.sz(xuResC));

  for (i8 nPlaces = 0; nPlaces <= 4; nPlaces++) {
    xrResC = xuResC;
    xrResC.vRoundDown(nPlaces);
    vTrace("%lld places) %s; "_Stz,nPlaces,spResC.sz(xrResC));
  }

  vTrace("\n"_Stz);
  
  xuResC.vAssign(12345678,4);
  vTrace("%s | rounded up to  : "_Stz,spResC.sz(xuResC));

  for (i8 nPlaces = 0; nPlaces <= 4; nPlaces++) {
    xrResC = xuResC;
    xrResC.vRoundUp(nPlaces);
    vTrace("%lld places) %s; "_Stz,nPlaces,spResC.sz(xrResC));
  }

  vTrace("\n"_Stz);

  vTrace("- - - - - - -\n"_Stz);
  
  xArgA = "31415."_Dec; // the first 5 digits of pi
  xArgB = "27182."_Dec; // the first 5 digits of e

  for (xArgA.m_nPlaces = 0; xArgA.m_nPlaces <= 4; xArgA.m_nPlaces++) {
    for (xArgB.m_nPlaces = 0; xArgB.m_nPlaces <= 4; xArgB.m_nPlaces++) {
      vTrace("%lld x %lld places: "_Stz,xArgA.m_nPlaces,xArgB.m_nPlaces);
      
      if (4 == xArgA.m_nPlaces && 4 == xArgB.m_nPlaces)
        vDebugPoint();
      
      xResC = xArgA * xArgB;
      vTrace("\n"_Stz);
    }
  }
  
  vTrace("- - - - - - -\n"_Stz);
  
  xResC = "4.8"_Dec * "2.10"_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  
  vTrace("- - - - - - -\n"_Stz);
  xResC = "34.56"_Dec;
  xResC.vRoundUp(1);
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  xResC = "34.56"_Dec;
  xResC.vRoundUp(3);
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  
  vTrace("- - - - - - -\n"_Stz);

  xResC = "6."_Dec / "2."_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));

  xResC = "10."_Dec / "3."_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  
  xResC = "10.00"_Dec / "3."_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  
  vTrace("- - - - - - -\n"_Stz);

  xResC = "1."_Dec + "2."_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));

  xResC = "12."_Dec + "0.345"_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));

  xResC = "12."_Dec - "0.345"_Dec;
  vTrace("xResC = %s\n"_Stz,spResC.sz(xResC));
  
}

/*----------------------------------------------------------------------------*/
/* test34                                                                     */
/*----------------------------------------------------------------------------*/
void vTest34(void)
{
}

/*----------------------------------------------------------------------------*/
/* test35                                                                     */
/*----------------------------------------------------------------------------*/
void vTest35(void)
{
}

/*----------------------------------------------------------------------------*/
/* test36                                                                     */
/*----------------------------------------------------------------------------*/
void vTest36(void)
{
}

/*----------------------------------------------------------------------------*/
/* test37                                                                     */
/*----------------------------------------------------------------------------*/
void vTest37(void)
{
}

/*----------------------------------------------------------------------------*/
/* test38                                                                     */
/*----------------------------------------------------------------------------*/
void vTest38(void)
{
}

/*----------------------------------------------------------------------------*/
/* test39                                                                     */
/*----------------------------------------------------------------------------*/
void vTest39(void)
{
}

/*----------------------------------------------------------------------------*/
/* test40                                                                     */
/*----------------------------------------------------------------------------*/
void vTest40(void)
{
}

/*----------------------------------------------------------------------------*/
/* test41                                                                     */
/*----------------------------------------------------------------------------*/
void vTest41(void)
{
}

/*----------------------------------------------------------------------------*/
/* test42                                                                     */
/*----------------------------------------------------------------------------*/
void vTest42(void)
{
}

/*----------------------------------------------------------------------------*/
/* test43                                                                     */
/*----------------------------------------------------------------------------*/
void vTest43(void)
{
}

/*----------------------------------------------------------------------------*/
/* test44                                                                     */
/*----------------------------------------------------------------------------*/
void vTest44(void)
{
}

/*----------------------------------------------------------------------------*/
/* test45                                                                     */
/*----------------------------------------------------------------------------*/
void vTest45(void)
{
}

/*----------------------------------------------------------------------------*/
/* test46                                                                     */
/*----------------------------------------------------------------------------*/
void vTest46(void)
{
}

/*----------------------------------------------------------------------------*/
/* test47                                                                     */
/*----------------------------------------------------------------------------*/
void vTest47(void)
{
}

/*----------------------------------------------------------------------------*/
/* test48                                                                     */
/*----------------------------------------------------------------------------*/
void vTest48(void)
{
}

/*----------------------------------------------------------------------------*/
/* test49                                                                     */
/*----------------------------------------------------------------------------*/
void vTest49(void)
{
}

/*----------------------------------------------------------------------------*/
/* test50                                                                     */
/*----------------------------------------------------------------------------*/
void vTest50(void)
{
}

/*----------------------------------------------------------------------------*/
/* test51                                                                     */
/*----------------------------------------------------------------------------*/
void vTest51(void)
{
}

/*----------------------------------------------------------------------------*/
/* test52                                                                     */
/*----------------------------------------------------------------------------*/
void vTest52(void)
{
}

/*----------------------------------------------------------------------------*/
/* test53                                                                     */
/*----------------------------------------------------------------------------*/
void vTest53(void)
{
}

/*----------------------------------------------------------------------------*/
/* test54                                                                     */
/*----------------------------------------------------------------------------*/
void vTest54(void)
{
}

/*----------------------------------------------------------------------------*/
/* test55                                                                     */
/*----------------------------------------------------------------------------*/
void vTest55(void)
{
}

/*----------------------------------------------------------------------------*/
/* test56                                                                     */
/*----------------------------------------------------------------------------*/
void vTest56(void)
{
}

/*----------------------------------------------------------------------------*/
/* test57                                                                     */
/*----------------------------------------------------------------------------*/
void vTest57(void)
{
}

/*----------------------------------------------------------------------------*/
/* test58                                                                     */
/*----------------------------------------------------------------------------*/
void vTest58(void)
{
}

/*----------------------------------------------------------------------------*/
/* test59                                                                     */
/*----------------------------------------------------------------------------*/
void vTest59(void)
{
}

/*----------------------------------------------------------------------------*/
/* test60                                                                     */
/*----------------------------------------------------------------------------*/
void vTest60(void)
{
}

/*----------------------------------------------------------------------------*/
/* test61                                                                     */
/*----------------------------------------------------------------------------*/
void vTest61(void)
{
}

/*----------------------------------------------------------------------------*/
/* test62                                                                     */
/*----------------------------------------------------------------------------*/
void vTest62(void)
{
}

/*----------------------------------------------------------------------------*/
/* test63                                                                     */
/*----------------------------------------------------------------------------*/
void vTest63(void)
{
  char a[C_1K];
  char b[C_1K];
  char c[C_1K];
  Timer oTimer;
  
  for (i8 i_Char = 0; i_Char < C_1K-1; i_Char++)
    a[i_Char] = 'a';
  a[C_1K-1] = '\0';
  
  i8 n = 0;
  
  oTimer.vStart();
  for (i8 i_Char = 0; i_Char < C_1K-1; i_Char++) {
    MemLib::vCopy(&c[0], &a[0], C_1K, CN_NONE);
    n++;
  }
  oTimer.vUpdate();
  vTrace("%s %lld\n"_Stz, c, n);
  vTrace("duration %lldns\n"_Stz,oTimer.iNanoseconds());
  
  n = 0;
  oTimer.vStart();
  for (i8 i_Char = 0; i_Char < C_1K-1; i_Char++) {
    i8 nL = StrLib::nCopy(&b[0],&a[0]);
    UNUSED(nL);
    n++;
    //RASSERT(nL == C_1K-1);
  }
  oTimer.vUpdate();
  vTrace("%s %lld\n"_Stz, b, n);
  vTrace("duration %lldns\n"_Stz,oTimer.iNanoseconds());
}

/*----------------------------------------------------------------------------*/
/* test64                                                                     */
/*----------------------------------------------------------------------------*/
void vTest64(void)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
