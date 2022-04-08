/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "RowBuffer.hpp"

#include "StreamSet.hpp"

#include "PtnMap.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Node_I.hpp"
#include "Grid.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "SP_Node.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Grid(void)
{
  vAddStep("[grid].dumpInfo"_Stz,          &Step::vGridDumpInfo);
  vAddStep("[grid].findNodeByName"_Stz,    &Step::vGridFindNodeByName);
  vAddStep("[grid].findNodeByNo"_Stz,      &Step::vGridFindNodeByNo);
  vAddStep("[grid].getGrid"_Stz,           &Step::vGridGetGrid);
  vAddStep("[grid].getParts"_Stz,          &Step::vGridGetGridParts);
  vAddStep("[grid].knownNodes"_Stz,        &Step::vGridKnownNodes);
  vAddStep("[grid].ptnsPerNode"_Stz,       &Step::vGridPtnsPerNode);
  vAddStep("[grid].putParts"_Stz,          &Step::vGridPutGridParts);
  vAddStep("[grid].seeNodes"_Stz,          &Step::vGridSeeNodes);
  vAddStep("[grid].show"_Stz,              &Step::vGridShow);
  vAddStep("[grid].tablePtnToPtnNo"_Stz,   &Step::vGridTablePtnToPtnNo);
  vAddStep("[grid].thisNodeId@"_Stz,       &Step::vGridThisNodeIdFetch);
  vAddStep("[grid].thisNodeName@"_Stz,     &Step::vGridThisNodeNameFetch);
  vAddStep("[grid].toNode"_Stz,            &Step::vGridToNode);

  vAddStep("Grid[]"_Stz,                &Step::vGridGet);
  vAddStep("Grid[]\\clear"_Stz,         &Step::vGridGetAndClear);
  vAddStep("Grid[].delete"_Stz,         &Step::vGridDelete);
  vAddStep("Grid[].isNotNull"_Stz,      &Step::vGridIsNotNull);
  vAddStep("Grid[].isNull"_Stz,         &Step::vGridIsNull);
  vAddStep("Grid[].set"_Stz,            &Step::vGridSet);
}

/*----------------------------------------------------------------------------*/
/* [grid].vGridDumpInfo -- display nodes in the grid                          */
/*----------------------------------------------------------------------------*/
void Step::vGridDumpInfo(void)
{
  ENTRY();

  //vTrace(">>>>>>>> [grid].dumpInfo"_Stz);

  s16KB pDumpBuffer = AM::pGrid()->szDumpInfo();

  m_pDataStack->vPushStr(pDumpBuffer);

  //m_pDataStack->vPushStz(pDumpBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].findNodeByName -- find node based on name                           */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNodeByName(void)
{
  ENTRY();
  
  s256 sNodeNm;
  m_pDataStack->vPopStr(sNodeNm,__func__);
  
  Node* pNode = AM::pGrid()->pFindNodeNm(sNodeNm);
  
  m_pDataStack->vPushPtr(pNode);

  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].findNodeByNo -- find node based on no                               */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNodeByNo(void)
{
  ENTRY();
  
  i8 iNode = m_pDataStack->iPopInt(__func__);
  
  Node* pNode = AM::pGrid()->pFindNodeById(iNode);
  
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].getGrid -- get grid object                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridGetGrid(void)
{
  ENTRY();

  // ... code which initiates the gridGet transfer ...
  vTrace("C++ gets called: vGridGetGrid\n"_Stz);

  Grid* pGrid = AM::pGrid();

  m_pDataStack->vPushPtr(pGrid);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].getParts -- get grid parts                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridGetGridParts(void)
{
  ENTRY();

  Grid* pGrid = _pGrid(m_pDataStack->pPopPtr(__func__));

  vTrace("C++ gets called: vGridGetGridParts for pGrid:%s %lld\n"_Stz,
         pGrid->szGridNm()->azChar(),
         pGrid->iThisNodeId());

  pGrid->iPrepareParcel();
  Parcel* pParcel = pGrid->pParcel();

  if (pParcel && pParcel->bOriginalParcel()) {
    vTrace("C++ prepared pParcel:%p for sending, it is originally created by Grid\n"_Stz, pGrid);
  }

  m_pDataStack->vPushPtr(pParcel);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].knownNodes -- get number of connected nodes in the grid             */
/*----------------------------------------------------------------------------*/
void Step::vGridKnownNodes(void)
{
  ENTRY();
  
  i8 nNodes = AM::pGrid()->nKnownNodes();

  m_pDataStack->vPushInt(nNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].ptnsPerNode -- get number of partitions per node                    */
/*----------------------------------------------------------------------------*/
void Step::vGridPtnsPerNode(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::PTNMAP::nPtnsPerNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].putParts -- put grid parts                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridPutGridParts(void)
{
  ENTRY();

  //Grid* pGrid = _pGrid(m_pDataStack->pPopPtr(__func__));

  Grid* pGridRemote  = nullptr;
  Parcel* pParcel = (Parcel*)m_pDataStack->pPopPtr(__func__);

  i8 nItems = pParcel->nItems();

  vTrace("C++ gets called: vGridPutGridParts. Received pParcel:%p with %lld parts\n"_Stz, pParcel, nItems);

  for (i8 i_Item = 0; i_Item < nItems; i_Item++) {
    Object *pObject = pParcel->pGetItem(i_Item);
    if (pObject && (pObject->eGetClassNo() == CN_GRID)) {
      pGridRemote = (Grid *) pObject;
      vTrace("Grid:%p reassemble from parcel, grid:%s\n"_Stz,pGridRemote, pGridRemote->szClassNm()->azChar());
      // call reassemble parts code
      pGridRemote->iReassembleFromParcel(pParcel);
      break;
    }
  }

  bool bMatched = false;

  Grid* pGridLocal = AM::pGrid();

  if (pGridLocal->nKnownNodes() == pGridRemote->nKnownNodes()) {
    for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
      Node* pNode = (*oNode_I);

      if (pGridRemote->pFindNodeById(pNode->iNodeId()) == nullptr) {
        bMatched = false;
        break;
      }
    }

  }

  if (bMatched) vTrace(">>>>>>>>>>>Matched Grid\n"_Stz);
  else vTrace(">>>>>>>>>>>NOT Matched Grid\n"_Stz);

  bMatched ? m_pDataStack->vPushInt(1) : m_pDataStack->vPushInt(0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].seeNodes -- see nodes of the grid                                   */
/*----------------------------------------------------------------------------*/
void Step::vGridSeeNodes(void)
{
  ENTRY();
  
  AM::pGrid()->vSeeNodes();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].show -- show the status of the grid                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridShow(void)
{
  ENTRY();
  
  AM::pGrid()->vShowGrid();
  
//  Node*   pNode;
//  Socket* pSndSkt;
//  Socket* pRcvSkt;
//
//  s32 sBytes;
//  s32 sUnits;
//  s32 sRate;
//
//  vTrace("Grid::\n"_Stz);
//  vTrace("  %lld nodes\n"_Stz,AM::pGrid()->nKnownNodes());
//  vTrace("  %lld ptns per node\n"_Stz,AM::PTNMAP::nPtnsPerNode());
//  vTrace("  %lld ptns in grid\n"_Stz,AM::PTNMAP::nPtnsInGrid());
//
//  for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {
//    pNode = (*oNode_I);
//    pSndSkt =  pNode->pSendSocket();
//    pRcvSkt =  pNode->pRecvSocket();
//
//    vTrace("  Node %lld : %s IP %s port %lld [%s]\n"_Stz
//           ,pNode->iNodeId()
//           ,pNode->szNodeNm()
//           ,pNode->szIPAddr()
//           ,pNode->iPortNo()
//           ,pSndSkt ? "connected" : "not connected"
//           );
//
//    if (pSndSkt) {
//      MagLib::vByteMag(pSndSkt->nBytesSent(),sBytes);
//      MagLib::vUnitMag(pSndSkt->nSends(),sUnits);
//      vTrace("    sndSkt %p %4s sends %s bytes sent (%lldB)\n"_Stz
//             ,pSndSkt
//             ,sUnits.szStz()
//             ,sBytes.szStz()
//             ,pSndSkt->nBytesSent()
//             );
//    }
//
//    if (pRcvSkt) {
//      MagLib::vByteMag(pRcvSkt->nBytesRcvd(),sBytes);
//      MagLib::vUnitMag(pRcvSkt->nRecvs(),sUnits);
//      vTrace("    rcvSkt %p %4s recvs %s bytes rcvd (%lldB)\n"_Stz
//             ,pRcvSkt
//             ,sUnits.szStz()
//             ,sBytes.szStz()
//             ,pRcvSkt->nBytesRcvd()
//             );
//    }
//
//  }
//
//  vTrace(";;\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].TablePtnToPtnNo -- get tLglPtn { m_iNodeId=thisNode, m_i_Ptn=i_Ptn } */
/*----------------------------------------------------------------------------*/
void Step::vGridTablePtnToPtnNo(void)
{
  ENTRY();
  
  i8 i_Ptn = m_pDataStack->iPopInt(__func__);

  LglPtn tLglPtn;
  tLglPtn = AM::pPtnMap()->tToPtnNoOfThisNode(i_Ptn);
  m_pDataStack->vPushInt(tLglPtn.iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].thisNodeId@ -- get this NodeId from the grid                        */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeIdFetch(void)
{
  ENTRY();
    
  i8 iNodeId = AM::pGrid()->iThisNodeId();
  
  m_pDataStack->vPushInt(iNodeId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].thisNodeName@ -- get this node name from the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeNameFetch(void)
{
  ENTRY();
  
  s256 sName;
  sName = AM::pGrid()->pThisNode()->szNodeNm();
  
  m_pDataStack->vPushStr(sName);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [grid].toNode -- convert partition id to node no                           */
/*----------------------------------------------------------------------------*/
void Step::vGridToNode(void)
{
  ENTRY();

  LglPtn tLglPtn;
  tLglPtn = m_pDataStack->iPopInt(__func__);

  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //m_pDataStack->vPush((i8)0);

  i8 iNodeId = AM::pPtnMap()->iNodeId(tLglPtn);
  m_pDataStack->vPushInt(iNodeId);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].delete -- operand delete                                            */
/*----------------------------------------------------------------------------*/
void Step::vGridDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpGrid);

  m_pLogicUnit->m_xpGrid = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].get -- fetch operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vGridGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpGrid);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].getAndClear -- fetch operand and set to nullptr                     */
/*----------------------------------------------------------------------------*/
void Step::vGridGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpGrid);

  m_pLogicUnit->m_xpGrid = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].isNotNull -- is narker not null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vGridIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpGrid.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].isNull -- is operand null ?                                         */
/*----------------------------------------------------------------------------*/
void Step::vGridIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpGrid.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Grid[].set -- set operand                                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridSet(void)
{
  ENTRY();

  Grid* pGrid = _pGrid(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pGrid || m_pLogicUnit->m_xpGrid.bIsNull());

  m_pLogicUnit->m_xpGrid = pGrid;

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
