/*============================================================================*/
/* Steps_Table.cpp - table step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "LglPtnMap.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "OpSpec.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include <new>

#include "Stream.hpp"
#include "Grid.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_MetaDataTable(void)
{
  vAddStep("MetaDataTable[].catalog"_Stz,       &Step::vMetaDataTableCatalog);
  vAddStep("MetaDataTable[].cluster"_Stz,       &Step::vMetaDataTableCluster);
  vAddStep("MetaDataTable[].columns"_Stz,       &Step::vMetaDataTableColumns);
  vAddStep("MetaDataTable[].indexColumns"_Stz,  &Step::vMetaDataTableIndexColumns);
  vAddStep("MetaDataTable[].indexes"_Stz,       &Step::vMetaDataTableIndexes);
  vAddStep("MetaDataTable[].nodes"_Stz,         &Step::vMetaDataTableNodes);
  vAddStep("MetaDataTable[].queries"_Stz,       &Step::vMetaDataTableQueries);
  vAddStep("MetaDataTable[].schema"_Stz,        &Step::vMetaDataTableSchema);
  vAddStep("MetaDataTable[].sessions"_Stz,      &Step::vMetaDataTableSessions);
  vAddStep("MetaDataTable[].tables"_Stz,         &Step::vMetaDataTableTables);
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].catalog --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalog(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  Word* pCatalog = m_oDicStack.pFindWord("catalogs"_Stz);
  Dic* pCatalogDic = _pDic(pCatalog->pObject());

  // Get last word in dic catalog
  Word* pCatalogFolderWord = pCatalogDic->pLastWord();

  // iterate dic catalog
  while(pCatalogFolderWord) {

    // get catalog name
    s256 sCatalogNm(pCatalogFolderWord->szWordNm());

    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         sCatalogNm.szStz(),
                                         C_VARGS_END);

    if(! pRow) {
      ASSERT(0);
      break;
    }

//    vTrace("%s\n"_Stz, sCatalogNm.szStz());

    pCatalogFolderWord = pCatalogFolderWord->pPrevWord();
  }

  m_pDataStack->vPushPtr((Object*)pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].cluster --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCluster(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].columns --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableColumns(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].indexColumns --                                            */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableIndexColumns(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].indexes --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableIndexes(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  // get current folder
  i8 iDepth =  m_oDicStack.nItems();
  Object* pPtrDic =  m_oDicStack.pPick(iDepth - 1);

  // Get table dict
  Dic* pDic = _pDic(pPtrDic);
  Word* pTableFolderWord = pDic->pLastWord();

  // get the schema name
  i8 i = 2;
  while( pDic->eDicKind() != DK_SCHEMA) {
    pPtrDic = m_oDicStack.pPick(iDepth - i);
    pDic = _pDic(pPtrDic);
    i++;
  }

  s256 sSchemaNm(pDic->szDicNm());

  // get the catalog name
  while( pDic->eDicKind() != DK_CATALOG) {
    pPtrDic = m_oDicStack.pPick(iDepth - i);
    pDic = _pDic(pPtrDic);
    i++;
  }

  s256 sCatalogNm(pDic->szDicNm());

  // iterate dic table

  while(pTableFolderWord) {
    // get table name
    s256 sTableNm(pTableFolderWord->szWordNm());

    // find tableNm folder in current dic
    Word *pWordTable = m_oDicStack.pFindWord(sTableNm);
    Dic *pDicTable = _pDic(pWordTable->pObject());

    // find actual table in this table dic
    Word *pWord = pDicTable->pFindWord(sTableNm);
    Table *pTableCur = _pTable(pWord->pObject());

    if( ! pTableCur->bIsUserCreated()) {
      pTableFolderWord = pTableFolderWord->pPrevWord();
      continue;
    }

    // find indexes folder
    Word* pIndexesFolder = pDicTable->pFindWord("indexes"_Stz);

    // Iterate the indexes folder if it exists
    if(pIndexesFolder) {
      Dic* pIndexesDic = _pDic(pIndexesFolder->pObject());
      Word* pIndexWord = pIndexesDic->pLastWord();

      while(pIndexWord) {
        Index* pIndex = _pIndex(pIndexWord->pObject());

        // Add index info to pds
        Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    sCatalogNm.szStz(),
                                    sSchemaNm.szStz(),
                                    pTableCur->szTableNm(),
                                    pIndex->szIndexNm(),
                                    C_VARGS_END);

        if(!pRow) {
          ASSERT(0);
          break;
        }
        pIndexWord = pIndexWord->pPrevWord();
      }
    }

//    vTrace("%s\n"_Stz, pTableCur->szTableNm());

    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  m_pDataStack->vPushPtr((Object*)pPdsRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].nodes --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableNodes(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].queries --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableQueries(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].schema --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSchema(void)
{
  ENTRY();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].sessions --                                                */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableSessions(void)
{
  ENTRY();

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* MetaDataTable[].tables --                                                  */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableTables(void)
{
  ENTRY();

  Table* pTable = _pTable(*m_pLogicUnit->m_xpTable);
  StreamDef* pStreamDef = pTable->pStreamDef();

  Pds* pPdsRowBuffer = new Pds(pStreamDef, false, BT_DATA);

  // get current folder
  i8 iDepth =  m_oDicStack.nItems();
  Object* pPtrDic =  m_oDicStack.pPick(iDepth - 1);

  // Get table dict
  Dic* pDic = _pDic(pPtrDic);
  Word* pTableFolderWord = pDic->pLastWord();

  // iterate dic table

  while(pTableFolderWord) {
    // get table name
    s256 sTableNm(pTableFolderWord->szWordNm());

    // find tableNm folder in current dic
    Word *pWordTable = m_oDicStack.pFindWord(sTableNm);
    Dic *pDicTable = _pDic(pWordTable->pObject());

    // find actual table in this table dic
    Word *pWord = pDicTable->pFindWord(sTableNm);
    Table *pTableCur = _pTable(pWord->pObject());

    if( ! pTableCur->bIsUserCreated()) {
      pTableFolderWord = pTableFolderWord->pPrevWord();
      continue;
    }

    Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                         pTableCur->szTableNm(),
                                         C_VARGS_END);
    if(! pRow) {
      ASSERT(0);
      break;
    }

//    vTrace("%s\n"_Stz, pTableCur->szTableNm());

    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  m_pDataStack->vPushPtr((Object*)pPdsRowBuffer);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
