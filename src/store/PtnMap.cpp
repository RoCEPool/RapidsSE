/*============================================================================*/
/* PtnMap.cpp -- Logical Partition Node Partition Map                         */
/*============================================================================*/

#include "BASE.h"


#include "PtnMap.hpp"
#include "Node.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,PtnMap,CN_PTNMAP)
__aNewFunction(PtnMap,CN_PTNMAP)
__vDeleteFunction(PtnMap,CN_PTNMAP)
__aFooterFunction(PtnMap)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnMap::PtnMap(i8 nKnownNodes, i8 nPtnsPerNode)
: Object(CN_PTNMAP)
, m_okLock(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  m_iMapChange   = C_UNKNOWN;
  m_nPtnsPerNode = nPtnsPerNode;
  
  m_nKnownNodes  = C_UNKNOWN;
  m_nPtnsInGrid  = C_UNKNOWN;

  vResetNodePtnArray();
  
  _checkInitialised_(CN_PTNMAP);
  
  vAdjustMap(nKnownNodes);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PtnMap::~PtnMap(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust partition map for given number of nodes and partitions per node     */
/*----------------------------------------------------------------------------*/
void PtnMap::vAdjustMap(i8 nKnownNodes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  m_nKnownNodes = nKnownNodes;
  m_nPtnsInGrid = m_nKnownNodes * m_nPtnsPerNode;
  RASSERT(m_nPtnsInGrid > 0 && m_nPtnsInGrid < C_MAX_LGLPTNS);

  vRepartition();
}

/*----------------------------------------------------------------------------*/
/* convert hash to logical partition number                                   */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tHashToLglPtnId(i8 iHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  LglPtn tLglPtnId;
  i8 iLglPtnNo = iHash % m_nPtnsInGrid;
  tLglPtnId = iLglPtnNo;
  
  return tLglPtnId;
}

/*----------------------------------------------------------------------------*/
/* get number of nodes known by partition map                                 */
/*----------------------------------------------------------------------------*/
i8 PtnMap::nKnownNodes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  return m_nKnownNodes;
}

/*----------------------------------------------------------------------------*/
/* get which node id (of grid) a logical partition is mapped to               */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iMappedNodeId(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iNodeId = m_xaPtnId_to_NodeIdPtnNo[tLglPtn.iAsI8()].m_iNodeId;

  return iNodeId;
}

/*----------------------------------------------------------------------------*/
/* get which partition no (within node) a logical partition is mapped to      */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_MappedPtnNo(LglPtn tLglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_PtnNo = m_xaPtnId_to_NodeIdPtnNo[tLglPtn.iAsI8()].m_iPtnNo;

  return i_PtnNo;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions in grid                                           */
/*----------------------------------------------------------------------------*/
i8 PtnMap::nPtnsInGrid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  return m_nPtnsInGrid;
}

/*----------------------------------------------------------------------------*/
/* get number of partitions per node                                          */
/*----------------------------------------------------------------------------*/
i8 PtnMap::nPtnsPerNode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  return m_nPtnsPerNode;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to distributed partition stream                  */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tRobinToDtdPtnStream(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  LglPtn tLglPtn;
  i8 iPtnNo = iRobin % m_nPtnsInGrid;
  tLglPtn = iPtnNo;

  return tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tRobinToLclPtnStream(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeId = AM::GRID::iThisNodeId();

  i8 i_LglPtn = iRobin % m_nPtnsPerNode;

  LglPtn tLglPtn;
  tLglPtn = tToPtnNoOfThisNode(i_LglPtn);

  return tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tRobinToNdePtnNo(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_LglPtn = iRobin % m_nPtnsPerNode;

  LglPtn tLglPtn = tToPtnNoOfThisNode(i_LglPtn);

  return tLglPtn;
}

/*----------------------------------------------------------------------------*/
/* see ptnmap                                                                 */
/*----------------------------------------------------------------------------*/
void PtnMap::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  vTrace("PtnMap::\n"_Stz);

  vTrace("  map change  %2lld\n"_Stz,m_iMapChange);
  vTrace("  known nodes %2lld\n"_Stz,m_nKnownNodes);
  vTrace("  node ptns per node %2lld\n"_Stz,m_nPtnsPerNode);
  vTrace("  lgl ptns in grid   %2lld\n"_Stz,m_nPtnsInGrid);

  for (i8 i_LglPtn = 0; i_LglPtn < m_nPtnsInGrid; i_LglPtn++) {
    i8 iNodeId = m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iNodeId;

    vTrace("  m_xaLglToNodePtn[i_LglPtn %lld] { m_iNodeId=%lld %s, m_i_NodePtn=%lld }\n"_Stz

           ,i_LglPtn
           ,iNodeId
           ,AM::GRID::pGetNode(iNodeId)->szNodeNm()
           ,m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iPtnNo
           );
  }

  vTrace(";;\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* convert given i_ptn to partition number of given node                      */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tToPtnNoOfNode(i8 i_LglPtn, i8 iNodeId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  LglPtn tLglPtn;
  tLglPtn = C_UNKNOWN;

  for (i8 i_NodePtn = 0; i_NodePtn < m_nPtnsInGrid; i_NodePtn++) {
    if (m_xaPtnId_to_NodeIdPtnNo[i_NodePtn].m_iNodeId != iNodeId)
      continue;
    if (m_xaPtnId_to_NodeIdPtnNo[i_NodePtn].m_iPtnNo != i_LglPtn)
      continue;
    tLglPtn = i_NodePtn;
    break;
  }

  RASSERT(C_UNKNOWN != tLglPtn.iAsI8());

  return tLglPtn;
}



/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
LglPtn PtnMap::tToPtnNoOfThisNode(i8 i_LglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeId = AM::GRID::iThisNodeId();

  LglPtn tLglPtn = tToPtnNoOfNode(i_LglPtn,iThisNodeId);

  return tLglPtn;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repartition the map based on new known nodes                               */
/*----------------------------------------------------------------------------*/
void PtnMap::vRepartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  vResetNodePtnArray();

  for (i8 i_LglPtn = 0; i_LglPtn < m_nPtnsInGrid; i_LglPtn++) {
    i4 i_Node = (i4)i_LglPtn % m_nKnownNodes;

    m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iNodeId   = AM::GRID::iGetNthNodeId(i_Node);
    m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iPtnNo = (i4)i_LglPtn / m_nKnownNodes;
  }

  m_iMapChange++;
}

/*----------------------------------------------------------------------------*/
/* reset the {node,nodePtn} array                                             */
/*----------------------------------------------------------------------------*/
void PtnMap::vResetNodePtnArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  for (i8 i_LglPtn = 0; i_LglPtn < C_MAX_LGLPTNS; i_LglPtn++) {
    m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iNodeId   = C_UNKNOWN;
    m_xaPtnId_to_NodeIdPtnNo[i_LglPtn].m_iPtnNo = C_UNKNOWN;
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
