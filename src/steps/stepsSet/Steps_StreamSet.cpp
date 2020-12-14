/*============================================================================*/
/* Steps_StreamSet.cpp -- Register StreamSet step group                       */
/*============================================================================*/

#include "Steps.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamSet(void)
{
  vAddStep("StreamSet[]"_Stz,                &Step::vStreamSetGet);
  vAddStep("StreamSet[].addAnalyzeIndex"_Stz,&Step::vStreamSetAddAnalyzeIndex);
  vAddStep("StreamSet[].addAnalyzeTable"_Stz,&Step::vStreamSetAddAnalyzeTable);
  vAddStep("StreamSet[].addColumnDef"_Stz,   &Step::vStreamSetAddColumnDef);
  vAddStep("StreamSet[].addGridSanity"_Stz,  &Step::vStreamSetAddGridSanity);
  vAddStep("StreamSet[].addListContents"_Stz,&Step::vStreamSetAddListContents);
  vAddStep("StreamSet[].addListFolder"_Stz,  &Step::vStreamSetAddListFolder);
  vAddStep("StreamSet[].addListIndex"_Stz,   &Step::vStreamSetAddListIndex);
  vAddStep("StreamSet[].addListPartitions"_Stz,&Step::vStreamSetAddListPartitions);
  vAddStep("StreamSet[].addNodeStatus"_Stz,  &Step::vStreamSetAddNodeStatus);
  vAddStep("StreamSet[].addPingInfo"_Stz,    &Step::vStreamSetAddPingInfo);
  vAddStep("StreamSet[].addPwfInfo"_Stz,       &Step::vStreamSetAddPwfInfo);
  vAddStep("StreamSet[].addRowCount"_Stz,      &Step::vStreamSetAddRowCount);
  vAddStep("StreamSet[].addShowCluster"_Stz,   &Step::vStreamSetAddShowCluster);
  vAddStep("StreamSet[].addSizeIndex"_Stz,     &Step::vStreamSetAddSizeIndex);
  vAddStep("StreamSet[].addSizeTable"_Stz,     &Step::vStreamSetAddSizeTable);
  vAddStep("StreamSet[].addSpaceInfo"_Stz,     &Step::vStreamSetAddSpaceInfo);
  vAddStep("StreamSet[].addTableSizes"_Stz,    &Step::vStreamSetAddTableSizes);
  vAddStep("StreamSet[].addVerifyIndex"_Stz,   &Step::vStreamSetAddVerifyIndex);
  vAddStep("StreamSet[].addVerifyTable"_Stz,   &Step::vStreamSetAddVerifyTable);
  vAddStep("StreamSet[].delete"_Stz,         &Step::vStreamSetDelete);
  vAddStep("StreamSet[].firstRow"_Stz,       &Step::vStreamSetFirstRow);
  vAddStep("StreamSet[].get"_Stz,            &Step::vStreamSetGet);
  vAddStep("StreamSet[].infoRows"_Stz,       &Step::vStreamSetInfoRows);
  vAddStep("StreamSet[].name"_Stz,           &Step::vStreamSetName);
  vAddStep("StreamSet[].new"_Stz,            &Step::vStreamSetNew);
  vAddStep("StreamSet[].nextRow"_Stz,        &Step::vStreamSetNextRow);
  vAddStep("StreamSet[].pipeOut"_Stz,        &Step::vStreamSetPipeOut);
  vAddStep("StreamSet[].set"_Stz,            &Step::vStreamSetSet);
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addAnalyzeIndex -- add a row to the streamSet index analysis   */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddAnalyzeIndex(void)
{
  ENTRY();
  
  // get the streamSet table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the index to be analyzed
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  // get the tablePtn
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  // get the stream set
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  i8 nBuffers = 0;
  i8 nEntries = 0;
  
  pIndex->vAnalyze(i_TablePtn, nBuffers, nEntries);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    pStreamSetTable->tTablePtnToPtnNo(i_TablePtn),
                                    nBuffers,
                                    nEntries,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addAnalyzeTable -- add a row to the streamSet table analysis   */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddAnalyzeTable(void)
{
  ENTRY();
  
  // get the streamSet table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the index to be analyzed
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the tablePtn
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  // get the stream set
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  i8 nBuffers = 0;
  i8 nRows = 0;
  
  pTable->vAnalyze(i_TablePtn, nBuffers, nRows);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    pStreamSetTable->tTablePtnToPtnNo(i_TablePtn),
                                    nBuffers,
                                    nRows,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addColumnDef -- add a row to the streamSet with the column def */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddColumnDef(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();

  Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pColumn->szColNm(),
                                  aCT[pColumn->eColumnType()],
                                  pColumn->iColumnLength(),
                                  pColumn->iColumnScale(),
                                  (i8)pColumn->bIsNullable(),
                                  (i8)pColumn->bIsPtnKey(),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addGridSanity -- add a row to the infoset with the gridSanity  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddGridSanity(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  Row* pNodeInfo = _pRow(m_pDataStack->pPopPtr(__func__));
  //StreamDef* pStreamDefNI = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  
  //pNodeInfo->vDump(pStreamDefNI);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Grid* pGrid = AM::pGrid();
  
  // get regionSize on this node
  bool bRegionSize = false;
  i8 iRegionSize = AM::iRegionSize();
  if (pNodeInfo->iColVal_int(1) == iRegionSize)
    bRegionSize = true;
  
  // get partitions per table for this node
  bool bPartitions = false;
  i8 nPtnsPerTable = AM::nPtnsPerTable();
  if (pNodeInfo->iColVal_int(2) == nPtnsPerTable)
    bPartitions = true;
  
  // check if the regionSize and partitions match
  bool bHomogenous = false;
  if ( bRegionSize && bPartitions )
    bHomogenous = true;
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pNodeInfo->szColVal_ofs(0),
                                  bHomogenous,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListContents -- add a row to infoset with the list contents */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListContents(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  s256 sContents;
  m_pDataStack->vPopStr(sContents, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sContents.azChar(),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListFolder -- add a row to infoset with the list folder     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListFolder(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  // get the name
  s256 sName;
  m_pDataStack->vPopStr(sName, __func__);
  
  // get the kind
  s256 sKind;
  m_pDataStack->vPopStr(sKind, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sName.azChar(),
                                  sKind.azChar(),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListIndex -- add a row to infoset with the list index       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListIndex(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  // get the index
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                      pIndex->szIndexNm(),
                                      (i8)pIndex->bIsUnique(),
                                      (i8)pIndex->bIsPrimaryKey(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addListPartitions -- add a row to infoset with the partitions  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddListPartitions(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Grid* pGrid = AM::pGrid();
  i8 i_Node = AM::pPtnMap()->i_WhichNode(tPtnNo);
  Node* pNode = pGrid->pGetNode(i_Node);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    tPtnNo,
                                    pNode->szNodeNm(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addNodeStatus -- add a row to the streamSet with node status   */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddNodeStatus(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Grid* pGrid = AM::pGrid();
  
  s32 sRegionNm(AM::szRegionName());
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        pGrid->pThisNode()->szNodeNm(),
                        sRegionNm.azChar(),
                        Region::bTest(sRegionNm) ? "running"_Stz : "not running"_Stz,
                        C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addPingInfo -- add a row to infoset with the ping response     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddPingInfo(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  "pong"_Stz,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addProgressUnits -- add a row to streamSet with progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddProgressUnits(void)
{
  ENTRY();
  
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);
  
  StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  iProgressUnits,
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addStr -- add a row to streamSet with a __pwf response         */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddPwfInfo(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  s256 sString;
  m_pDataStack->vPopStr(sString, __func__);
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    sString.azChar(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addShowCluster -- add a row to streamSet with the cluster info */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddRowCount(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  i8 i_Count = m_pDataStack->iPopInt(__func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    i_Count,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addShowCluster -- add a row to streamSet with the cluster info */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddShowCluster(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  i8 i_Node = m_pDataStack->iPopInt(__func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Node* pNode = AM::pGrid()->pGetNode(i_Node);
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pNode->szNodeNm(),
                                  pNode->szIPAddr(),
                                  pNode->iPortNo(),
                                  (i8)(pNode->pSendSocket() ? true : false),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addSizeIndex -- add a row to streamSet with the index size     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddSizeIndex(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  s256 sSize;
  m_pDataStack->vPopStr(sSize, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    sSize.azChar(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addSizeTable -- add a row to streamSet with the table size     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddSizeTable(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  s256 sSize;
  m_pDataStack->vPopStr(sSize, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    sSize.azChar(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addSpaceInfo -- add a row to the streamSet with the spaceInfo  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddSpaceInfo(void)
{
  ENTRY();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Grid* pGrid = AM::pGrid();
  
  i8 iTotalSpace = AM::iRegionSize();
  
  i8 iInUseSpace = AM::iRegionSize()
    - gpBufferPool->iFreeSpace()
    - PoolHelper::iPoolFree();
  
  i8 iFreeSpace = gpBufferPool->iFreeSpace();
  i8 iFreeSpaceInPool = PoolHelper::iPoolFree();
  i8 iTotalFreeSpace = iFreeSpace + iFreeSpaceInPool;
  
  s256 sTotalSpace;
  MagLib::vByteMag(iTotalSpace, 0, sTotalSpace);
  
  s256 sInUseSpace;
  MagLib::vByteMag(iInUseSpace, 0, sInUseSpace);
  
  s256 sTotalFreeSpace;
  MagLib::vByteMag(iTotalFreeSpace, 0, sTotalFreeSpace);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  pGrid->pThisNode()->szNodeNm(),
                                  sTotalSpace.azChar(),
                                  sInUseSpace.azChar(),
                                  sTotalFreeSpace.azChar(),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addTableSizes -- add a row to streamSet with the table sizes   */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddTableSizes(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  // get the streamDef from the table
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  s256 sTableName;
  m_pDataStack->vPopStr(sTableName, __func__);
  
  s256 sSize;
  m_pDataStack->vPopStr(sSize, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    sTableName.azChar(),
                                    sSize.azChar(),
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addVerifyIndex -- add a row to streamSet with the index verify */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddVerifyIndex(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  // get the index
  Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));
  
  // get the table from the index
  Table* pTable = pIndex->pTable();
  
  i8 iStatus = m_pDataStack->iPopInt(__func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    pTable->szTableNm(),
                                    pIndex->szIndexNm(),
                                    iStatus ? "Valid"_Stz : "Invalid"_Stz,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addVerifyTable -- add a row to streamSet with the table verify */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddVerifyTable(void)
{
  ENTRY();
  
  // get the table
  Table* pStreamSetTable = _pTable(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDef = pStreamSetTable->pStreamDef();
  
  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  i8 iStatus = m_pDataStack->iPopInt(__func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    pTable->szTableNm(),
                                    iStatus ? "valid"_Stz : "invalid"_Stz,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].delete -- operand streamSet                                    */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpStreamSet);
  
  m_pLogicUnit->m_xpStreamSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].firstRow -- get first row from the streamSet                   */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetFirstRow(void)
{
  ENTRY();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  m_pDataStack->vPushPtr(pStreamSet->aFirstRow());
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].get -- get streamSet                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetGet(void)
{
  ENTRY();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  m_pDataStack->vPushPtr(pStreamSet);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].infoRows -- get now of info rows in info set                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetInfoRows(void)
{
  ENTRY();
  
  ASSERT ( ! m_pLogicUnit->m_xpStreamSet.bIsNull() );
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  m_pDataStack->vPushInt(pStreamSet->nInfoRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].name -- get infoset name                                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetName(void)
{
  ENTRY();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Stz* szInfoSetNm = pStreamSet->szInfoSetNm();
  
  m_pDataStack->vPushStz(szInfoSetNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].new -- new an infoset operand                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetNew(void)
{
  ENTRY();
  
  ASSERT(m_pLogicUnit->m_xpStreamSet.bIsNull());
  
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  
  StreamDef* pStreamDef = pTable->pStreamDef();
  
  m_pLogicUnit->m_xpStreamSet = new StreamSet(pStreamDef);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].nextRow -- get next row from the infoset                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetNextRow(void)
{
  ENTRY();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  Row* pRow = _pRow(*m_pLogicUnit->m_xpRow);
  
  ASSERT(pRow);
  
  m_pDataStack->vPushPtr(pStreamSet->aNextRow(pRow));
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].pipeOut -- pipeout infoset                                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpStreamSet);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpStreamSet,m_pTask);
  
  m_pLogicUnit->m_xpStreamSet = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].readChunk -- reach a chunk from file given the streamSet       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetReadChunk(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpStreamSet);

  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);
  
  Row* pRow = _pRow(m_pDataStack->pPopPtr(__func__));
  
  i8 iOffset = pRow->iColVal_int(0);
  i8 nBytes  = pRow->iColVal_int(1);
  
  //vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
  pFile->vSeek(iOffset);
  pCsvBuffer->vTake(nBytes);
  i8 nBytesRead = pFile->nReadBytes(pCsvBuffer->ayDataArea(),nBytes);
  ASSERT(nBytesRead == nBytes);
  
  // return the CsvBuffer
  m_pDataStack->vPushPtr(pCsvBuffer);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].set -- set streamSet                                           */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpStreamSet.bIsNull());
  m_pLogicUnit->m_xpStreamSet = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
