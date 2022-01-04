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
PtnId PtnMap::tHashToLglPtnId(i8 iHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  PtnId tLglPtnId;
  i8 iLglPtnNo = iHash % m_nPtnsInGrid;
  tLglPtnId = iLglPtnNo;
  
  return tLglPtnId;
}




/*----------------------------------------------------------------------------*/
/* convert iBkt to PtnId                                                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tToPtn(i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  PtnId tPtnId;

  //tPtnId = iBktNo < C_MAX_LGLPTNS ? m_xaBkt[(i8) iBktNo].m_i_NodePtn : 0;
  return tPtnId;

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
/* get which node a logical partition is mapped to                            */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iMappedNodeNo(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iNodeNo = m_xaLglToNodePtn[tPtnId.iAsI8()].m_iNodeNo;

  return iNodeNo;
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
PtnId PtnMap::tRobinToDtdPtnStream(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  PtnId tPtnId;
  i8 iPtnNo = iRobin % m_nPtnsInGrid;
  tPtnId = iPtnNo;

  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tRobinToLclPtnStream(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  i8 i_LglPtn = iRobin % m_nPtnsPerNode;

  PtnId tPtnId;
  tPtnId = tToPtnNoOfThisNode(i_LglPtn);

  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tRobinToNdePtnNo(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_LglPtn = iRobin % m_nPtnsPerNode;

  PtnId tPtnId = tToPtnNoOfThisNode(i_LglPtn);

  return tPtnId;
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
    i8 iNodeNo = m_xaLglToNodePtn[i_LglPtn].m_iNodeNo;

    vTrace("  m_xaLglToNodePtn[i_LglPtn %lld] { m_iNodeNo=%lld %s, m_i_NodePtn=%lld }\n"_Stz

           ,i_LglPtn
           ,iNodeNo
           ,AM::GRID::pGetNode(iNodeNo)->szNodeNm()
           ,m_xaLglToNodePtn[i_LglPtn].m_i_NodePtn
           );
  }

  vTrace(";;\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get which node a logical partition is mapped to                            */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iWhichNodeNo(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iNodeNo = m_xaLglToNodePtn[tPtnId.iAsI8()].m_iNodeNo;

  return iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get which node partition a logical partition is mapped to                  */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichPtn(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_LglPtn = m_xaLglToNodePtn[tPtnId.iAsI8()].m_i_NodePtn;

  return i_LglPtn;
}

/*----------------------------------------------------------------------------*/
/* convert given i_ptn to partition number of given node                      */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tToPtnNoOfNode(i8 i_LglPtn, i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  PtnId tPtnId;
  tPtnId = C_UNKNOWN;

  for (i8 i_NodePtn = 0; i_NodePtn < m_nPtnsInGrid; i_NodePtn++) {
    if (m_xaLglToNodePtn[i_NodePtn].m_iNodeNo != iNodeNo)
      continue;
    if (m_xaLglToNodePtn[i_NodePtn].m_i_NodePtn != i_LglPtn)
      continue;
    tPtnId = i_NodePtn;
    break;
  }

  RASSERT(C_UNKNOWN != tPtnId.iAsI8());

  return tPtnId;
}



/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tToPtnNoOfThisNode(i8 i_LglPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  PtnId tPtnId = tToPtnNoOfNode(i_LglPtn,iThisNodeNo);

  return tPtnId;
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

    m_xaLglToNodePtn[i_LglPtn].m_iNodeNo   = AM::GRID::iGetNthNodeNo(i_Node);
    m_xaLglToNodePtn[i_LglPtn].m_i_NodePtn = (i4)i_LglPtn / m_nKnownNodes;
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
    m_xaLglToNodePtn[i_LglPtn].m_iNodeNo   = C_UNKNOWN;
    m_xaLglToNodePtn[i_LglPtn].m_i_NodePtn = C_UNKNOWN;
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
