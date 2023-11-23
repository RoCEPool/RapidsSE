/*============================================================================*/
/* Steps_StreamSet.cpp -- Register StreamSet step group                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "RowBuffer.hpp"

#include "StreamSet.hpp"


#include "LglPtnMap.hpp"


#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_StreamSet(void)
{
  vAddStep("StreamSet[]"_Stz,                &Step::vStreamSetGet);
	vAddStep("StreamSet[]"_Stz,                &Step::vStreamSetGet);

	//vAddStep("StreamSet[].addAnalyzeTable"_Stz,&Step::vStreamSetAddAnalyzeTable);
  //vAddStep("StreamSet[].addBpaObjects"_Stz,  &Step::vStreamSetAddBpaObjects);
  //vAddStep("StreamSet[].addBpaRegions"_Stz,  &Step::vStreamSetAddBpaRegions);
  //vAddStep("StreamSet[].addColumnDef"_Stz,   &Step::vStreamSetAddColumnDef);
  //vAddStep("StreamSet[].addGridSanity"_Stz,  &Step::vStreamSetAddGridSanity);
  vAddStep("StreamSet[].addListContents"_Stz,&Step::vStreamSetAddListContents);
  //vAddStep("StreamSet[].addListFolder"_Stz,  &Step::vStreamSetAddListFolder)
	//vAddStep("StreamSet[].addListIndex"_Stz,   &Step::vStreamSetAddListIndex);
  //vAddStep("StreamSet[].addListPartitions"_Stz,&Step::vStreamSetAddListPartitions);
  vAddStep("StreamSet[].addLogMsg"_Stz,      &Step::vStreamSetAddLogMsg);
  vAddStep("StreamSet[].addMemoryLeaks"_Stz, &Step::vStreamSetAddMemoryLeaks);
  vAddStep("StreamSet[].addNodeHeartbeat"_Stz, &Step::vStreamSetAddNodeHeartbeat);
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
  vAddStep("StreamSet[].isNull"_Stz,         &Step::vStreamSetIsNull);
  vAddStep("StreamSet[].name"_Stz,           &Step::vStreamSetName);
  vAddStep("StreamSet[].new"_Stz,            &Step::vStreamSetNew);
  vAddStep("StreamSet[].nextRow"_Stz,        &Step::vStreamSetNextRow);
  vAddStep("StreamSet[].set"_Stz,            &Step::vStreamSetSet);
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                  C_VARGS_BEGIN,
                                  sContents.azChar(),
                                  C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addLogMsg -- add rows to streamSet with log message info       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddLogMsg(void)
{
  ENTRY();

  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the timestamp to use
  i8 iTsp = m_pDataStack->iPopInt(__func__);

  // get the log message to use
  s2K sMsg;
  m_pDataStack->vPopStr(sMsg, __func__ );


  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();

  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  ASSERT(pStreamSet);

  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    iTsp,
                                    sMsg.szStz(),
                                    C_VARGS_END);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addMemoryLeaks -- add rows to streamSet with memory leak info  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddMemoryLeaks(void)
{
  ENTRY();

  // get the table
  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the streamDef from the table
  StreamDef* pStreamDef = pTable->pStreamDef();

  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  i8 nLeaks = 0;

  ObjectSnapshot* pSnapshotThen = AM::MEMORY::pSnapshotAtTrackingMemory();
  ObjectSnapshot* pSnapshotNow  = AM::MEMORY::pSnapshotAtMemoryLeaks();

  for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {

    eCN eClass = (eCN)i_Class;

    if (eClass == CN_FREEPOOLBUFFER || eClass == CN_HEAPBUFFER)
      continue;

    // check if this class to be ignored for memory leak detection
    if (gxaSizeHeap[eClass].m_bIgnoreMemoryLeak)
      continue;

    i8 nDelta = pSnapshotNow->m_nObjects[eClass] -
                pSnapshotThen->m_nObjects[eClass];

    if (nDelta <= 0)
      continue;

    ++nLeaks;
    /*Row* pRow =*/ pStreamSet->pAllotRow(pStreamDef,
                                          C_VARGS_BEGIN,
                                          aCN[eClass],
                                          nDelta,
                                          C_VARGS_END);
  }

  m_pDataStack->vPushBol(nLeaks > 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].addNodeInfo -- add a row to the streamSet with this node info  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetAddNodeHeartbeat(void)
{
  ENTRY();

  // get the table
  Table *pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the streamDef from the table
  StreamDef *pStreamDef = pTable->pStreamDef();

  StreamSet *pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Node* pThisNode = MY::pNode();
  
  Row *pRow = nullptr;
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

    if (pThisNode != *oNode_I)
      continue;
    
    pRow = pStreamSet->pAllotRow(pStreamDef,
                                 C_VARGS_BEGIN,
                                 (*oNode_I)->szNodeNm(),
                                 (*oNode_I)->xGetHeartbeat().m_acRegionName,
                                 (*oNode_I)->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz,
                                 (*oNode_I)->pSendSocket() ? true : false,
                                 (*oNode_I)->pRecvSocket() ? true : false,
                                 C_VARGS_END);

    break;
  }

  // done
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
  Table *pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  // get the streamDef from the table
  StreamDef *pStreamDef = pTable->pStreamDef();

  StreamSet *pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  /* commented out, the old status command which is required to go to all nodes
   * for each node, it gets back a row and its status.
   * With the new model, we need a grid of the requested node,
   * each individual node in the grid will have its own status.
   */
  /*
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                        C_VARGS_BEGIN,
                        pGrid->pThisNode()->szNodeNm(),
                        sRegionNm.azChar(),
                        Region::bTest(sRegionNm) ? "running"_Stz : "not running"_Stz,
                        C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);
  
  m_pDataStack->vPushInt(pRow ? true : false);
  */
  Row *pRow = nullptr;
  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

    pRow = pStreamSet->pAllotRow(pStreamDef,
                                 C_VARGS_BEGIN,
                                 (*oNode_I)->szNodeNm(),
                                 (*oNode_I)->xGetHeartbeat().m_acRegionName,
                                 (*oNode_I)->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz,
                                 (*oNode_I)->pSendSocket() ? true : false,
                                 (*oNode_I)->pRecvSocket() ? true : false,
                                 C_VARGS_END);

    if (!pRow) {
      // call again
      m_pDataStack->vPushInt(oNode_I.i_Node());
      break;
    }
  }

  // done
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Node* pNode = AM::pGrid()->pFindNodeById(i_Node);
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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

  i8 iSize = m_pDataStack->iPopInt(__func__);

  s256 sSize;
  m_pDataStack->vPopStr(sSize, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    iSize,
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

  i8 iSize = m_pDataStack->iPopInt(__func__);

  s256 sSize;
  m_pDataStack->vPopStr(sSize, __func__);
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    AM::pGrid()->pThisNode()->szNodeNm(),
                                    iSize,
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
  
  i8 iTotalSpace = AM::POOL::iTotalSpace();
  i8 iInUseSpace = AM::POOL::iUsedSpace();
  i8 iFreeSpace  = AM::POOL::iFreeSpace();
  
  i8 iFreeSpaceInPool = PoolHelper::iPoolFree();
  i8 iTotalFreeSpace  = iFreeSpace + iFreeSpaceInPool;
  
  s256 sTotalSpace;
  MagLib::vByteMag(iTotalSpace, 0, sTotalSpace);
  
  s256 sInUseSpace;
  MagLib::vByteMag(iInUseSpace, 0, sInUseSpace);
  
  s256 sTotalFreeSpace;
  MagLib::vByteMag(iTotalFreeSpace, 0, sTotalFreeSpace);
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
                                    C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    iTotalSpace,
                                    sTotalSpace.azChar(),
                                    iInUseSpace,
                                    sInUseSpace.azChar(),
                                    iTotalFreeSpace,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  Row* pRow = pStreamSet->pAllotRow(pStreamDef,
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
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpStreamSet,nullptr);
  
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
  
  m_pDataStack->vPushPtr(pStreamSet->pFirstRow());
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
  
  m_pDataStack->vPushInt(pStreamSet->nRows());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].isNull -- is streamSet null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpStreamSet.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].name -- get infoset name                                       */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetName(void)
{
  ENTRY();
  
  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  Stz* szBufferNm = pStreamSet->szBufferNm();
  
  m_pDataStack->vPushStz(szBufferNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* StreamSet[].new -- new an streamset operand                               */
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
/* StreamSet[].nextRow -- get next row from the streamset                     */
/*----------------------------------------------------------------------------*/
void Step::vStreamSetNextRow(void)
{
  ENTRY();

  StreamSet* pStreamSet = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);
  Row* pRow = _pRow(*m_pLogicUnit->m_xpInfoSetRow);

  ASSERT(pRow);

  m_pDataStack->vPushPtr(pStreamSet->pNextRow(pRow));
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
  UNUSED(nBytesRead);

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
