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
  vAddStep("MetaDataTable[].print"_Stz,         &Step::vMetaDataTablePrint);
  vAddStep("MetaDataTable[].catalog"_Stz,       &Step::vMetaDataTableCatalog);
  vAddStep("MetaDataTable[].cluster"_Stz,       &Step::vMetaDataTableCluster);
  vAddStep("MetaDataTable[].columns"_Stz,       &Step::vMetaDataTableColumns);
  vAddStep("MetaDataTable[].indexColumns"_Stz,  &Step::vMetaDataTableIndexColumns);
  vAddStep("MetaDataTable[].indexes"_Stz,       &Step::vMetaDataTableIndexes);
  vAddStep("MetaDataTable[].nodes"_Stz,         &Step::vMetaDataTableNodes);
  vAddStep("MetaDataTable[].queries"_Stz,       &Step::vMetaDataTableQueries);
  vAddStep("MetaDataTable[].schema"_Stz,        &Step::vMetaDataTableSchema);
  vAddStep("MetaDataTable[].sessions"_Stz,      &Step::vMetaDataTableSessions);
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].print --                                                   */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTablePrint(void)
{
  ENTRY();

  vTrace("GOT METADATASTEP\n"_Stz);

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

    // find table in current dic
    Word *pWordTable = m_oDicStack.pFindWord(sTableNm);

    // find actual table in this table dic
    Dic *pDicTable = _pDic(pWordTable->pObject());
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
      vTrace("Pds Full\n"_Stz);
    }

//    vTrace("%s\n"_Stz, pTableCur->szTableNm());

    pTableFolderWord = pTableFolderWord->pPrevWord();
  }

  Operand* pOperand = _pOperand(pPdsRowBuffer);

  if(! BufferId::bIsBufferId(pOperand)) {
    if(DataBuffer::bIsDataBuffer(pOperand->eGetClassNo())) {
      DataBuffer* pDataBuffer = _pDataBuffer(pOperand);
      AM::pHashMap()->vDropKeyLock(pDataBuffer->oGetBufferId().m_pvAsAddress);
      pOperand = _pOperand((Object*)pDataBuffer->oGetBufferId().m_pvAsAddress);
    }
  }

  AM::OP::vPipeToNextOp(m_pOp,pOperand,this);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* MetaDataTable[].catalog --                                                 */
/*----------------------------------------------------------------------------*/
void Step::vMetaDataTableCatalog(void)
{
  ENTRY();

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
