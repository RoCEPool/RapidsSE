/*============================================================================*/
/* Steps_Grid.cpp - grid step group                                           */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Grid(void)
{
  vAddStep("grid::connectedNodes"_Stz,    &Step::vGridConnectedNodes);
  vAddStep("grid::definedNodes"_Stz,      &Step::vGridDefinedNodes);
  vAddStep("grid::findNode"_Stz,          &Step::vGridFindNode);
  vAddStep("grid::incConnectedNodes"_Stz, &Step::vGridIncConnectedNodes);
  vAddStep("grid::listNodes"_Stz,         &Step::vGridListNodes);
  vAddStep("grid::node"_Stz,              &Step::vGridNode);
  vAddStep("grid::partitions"_Stz,        &Step::vGridPartitions);

  vAddStep("grid::show"_Stz,              &Step::vGridShow);
  vAddStep("grid::started!"_Stz,          &Step::vGridStartedStore);
  vAddStep("grid::started@"_Stz,          &Step::vGridStartedFetch);
  vAddStep("grid::StartNode!"_Stz,        &Step::vGridStartNodeStore);
  vAddStep("grid::StartNode@"_Stz,        &Step::vGridStartNodeFetch);
  vAddStep("grid::tablePtns"_Stz,         &Step::vGridTablePtns);
  vAddStep("grid::tablePtnToPtnNo"_Stz,   &Step::vGridTablePtnToPtnNo);
  vAddStep("grid::thisNode@"_Stz,         &Step::vGridThisNodeFetch);
  vAddStep("grid::thisNodeName@"_Stz,     &Step::vGridThisNodeNameFetch);
  vAddStep("grid::toNode"_Stz,            &Step::vGridToNode);
}

/*----------------------------------------------------------------------------*/
/* grid::connectedNodes -- get number of connected nodes in the grid          */
/*----------------------------------------------------------------------------*/
void Step::vGridConnectedNodes(void)
{
  ENTRY();
  
  i8 nConnectedNodes = AM::pGrid()->nConnectedNodes();

  m_pDataStack->vPushInt(nConnectedNodes);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::definedNodes -- get number of nodes in the grid                      */
/*----------------------------------------------------------------------------*/
void Step::vGridDefinedNodes(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nDefinedNodes());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::findNode -- find node based on name                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridFindNode(void)
{
  ENTRY();
  
  s256 sNodeNm;
  m_pDataStack->vPopStr(sNodeNm,__func__);
  
  Node* pNode = AM::pGrid()->pFindNode(sNodeNm);
  
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::incConnectedNodes -- inc connected nodes                             */
/*----------------------------------------------------------------------------*/
void Step::vGridIncConnectedNodes(void)
{
  ENTRY();
  
  AM::pGrid()->vIncConnectedNodes();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::listNodes -- list nodes in this grid                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridListNodes(void)
{
  ENTRY();
  
  Table* pStreamSetTable = _pTable(*m_pLogicUnit->m_xpStreamSetTable);
  StreamDef* pStreamDef  = pStreamSetTable->pStreamDef();
  StreamSet* pStreamSet  = _pStreamSet(*m_pLogicUnit->m_xpStreamSet);

  i8 i_Node = m_pDataStack->iPopAsInt(__func__);
  for (Node_I oNode_I(i_Node); oNode_I.bMore(); oNode_I.vNext()) {

    Row* pRow = pStreamSet->aAllotRow(pStreamDef,
                                      C_VARGS_BEGIN,
                                      (*oNode_I)->iNodeNo(),
                                      (*oNode_I)->szNodeNm(),
                                      (*oNode_I)->szIPAddr(),
                                      (*oNode_I)->iPortNo(),
                                      aNS[(*oNode_I)->eState()],
                                      C_VARGS_END);
  
    if (! pRow) {
      // call again
      m_pDataStack->vPushInt(oNode_I.i_Node());
      break;
    }
    
  }
  
  // done (dont call again)
  m_pDataStack->vPushInt(0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::node -- get node from grid                                           */
/*----------------------------------------------------------------------------*/
void Step::vGridNode(void)
{
  ENTRY();
  
  i8 iNode = m_pDataStack->iPopInt(__func__);
  
  Node* pNode = AM::pGrid()->pGetNode(iNode);
  
  m_pDataStack->vPushPtr(pNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::partitions -- get number of partitions in the grid                   */
/*----------------------------------------------------------------------------*/
void Step::vGridPartitions(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nPartitionsInGrid());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::show -- show the status of the grid                                  */
/*----------------------------------------------------------------------------*/
void Step::vGridShow(void)
{
  ENTRY();
  
  vTrace("Grid::\n"_Stz);
  vTrace("  %lld defined nodes\n"_Stz,AM::pGrid()->nDefinedNodes());
  vTrace("  %lld partitions per table\n"_Stz,AM::pGrid()->nTablePtns());
  vTrace("  %lld total partitions\n"_Stz,AM::pGrid()->nPartitionsInGrid());
  
    for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext())
      vTrace("  Node %lld : %s IP %s port %lld rcvSkt %p sndSkt %p [%s]\n"_Stz,
             (*oNode_I)->iNodeNo(),
             (*oNode_I)->szNodeNm(),
             (*oNode_I)->szIPAddr(),
             (*oNode_I)->iPortNo(),
             (*oNode_I)->pRecvSocket(),
             (*oNode_I)->pSendSocket(),

             (*oNode_I)->pSendSocket() ? "connected" : "not connected");
  
  
  vTrace("\n"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::tablePtns -- get number of partitions per table in the grid          */
/*----------------------------------------------------------------------------*/
void Step::vGridTablePtns(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->nTablePtns());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::TablePtnToPtnNo -- tablePtn to ptnNo                                 */
/*----------------------------------------------------------------------------*/
void Step::vGridTablePtnToPtnNo(void)
{
  ENTRY();
  
  i8 i_TablePtn = m_pDataStack->iPopInt(__func__);
  
  tPN tPtnNo = PtnMap::tStrandToLglPtnNo(i_TablePtn);
  
  m_pDataStack->vPushInt(tPtnNo);              //! DISCUSS WITH ALKA
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNode@ -- get this node from the grid                              */
/*----------------------------------------------------------------------------*/
void Step::vGridThisNodeFetch(void)
{
  ENTRY();
  
  Object* pPtrGrid;
  
  i8 iNodeNo = AM::pGrid()->iThisNode();
  
  m_pDataStack->vPushInt(iNodeNo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::thisNodeName@ -- get this node name from the grid                     */
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
/* grid::started! -- set grid started                                          */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedStore(void)
{
  ENTRY();
  
  i8 iStarted = m_pDataStack->iPopInt(__func__);
  
  AM::pGrid()->vHasStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode@ -- get grid start node                                    */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->iStartNode());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::StartNode! -- set grid start node                                    */
/*----------------------------------------------------------------------------*/
void Step::vGridStartNodeStore(void)
{
  ENTRY();
  
  i8 iStartNode = m_pDataStack->iPopInt(__func__);
  
  AM::pGrid()->vStartNode(iStartNode);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::started@ -- get grid started                                         */
/*----------------------------------------------------------------------------*/
void Step::vGridStartedFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::pGrid()->bHasStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* grid::toNode -- convert partition id to node no                            */
/*----------------------------------------------------------------------------*/
void Step::vGridToNode(void)
{
  ENTRY();
  
  tPN tPtnNo = m_pDataStack->iPopInt(__func__);
  
  // for the moment its always node 0                      //! DISCUSS WITH ALKA
  //m_pDataStack->vPush((i8)0);
  
  i8 i_Node = AM::pPtnMap()->i_WhichNode(tPtnNo);
  
  m_pDataStack->vPushInt(i_Node);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
