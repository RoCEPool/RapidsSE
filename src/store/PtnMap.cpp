/*============================================================================*/
/* PtnMap.cpp -- Partition (to node,tablePtn) Map                             */
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert hash to bucket id                                                  */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iHashToBucketId(i8 iHash)
{
  _static_
  
  i8 iBucket = iHash % C_MAX_BUCKETS;

  return iBucket;
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
  RASSERT(m_nPtnsInGrid > 0 && m_nPtnsInGrid < C_MAX_PTNSINGRID);

  vRepartition();
}

/*----------------------------------------------------------------------------*/
/* convert hash to distributed partition number                               */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tHashToDtdPtnNo(i8 iHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 tDtdPtnNo = iHash % m_nPtnsInGrid;

  return tDtdPtnNo;
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
/* convert given round robin to distributed partition number                  */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tRobinToDtdPtnNo(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  tPN tPtnNo = iRobin % m_nPtnsInGrid;
  
  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tRobinToLclPtnNo(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  i8 i_Ptn = iRobin % m_nPtnsPerNode;

  tPN tPtnNo = tToPtnNoOfThisNode(i_Ptn);

  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tRobinToNdePtnNo(i8 iRobin, i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  i8 i_Ptn = iRobin % m_nPtnsPerNode;

  tPN tPtnNo = tToPtnNoOfThisNode(i_Ptn);

  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* see ptnmap                                                                 */
/*----------------------------------------------------------------------------*/
void PtnMap::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  vTrace("See PtnMap::\n"_Stz);
  
  vTrace("  map change    %2lld\n"_Stz,m_iMapChange);
  vTrace("  ptns per node %2lld\n"_Stz,m_nPtnsPerNode);
  vTrace("  known nodes   %2lld\n"_Stz,m_nPtnsPerNode);
  vTrace("  ptns in grid  %2lld\n"_Stz,m_nPtnsInGrid);

  for (i8 i_Ptn = 0; i_Ptn < m_nPtnsInGrid; i_Ptn++) {
    i8 iNodeNo = m_xaNodePtn[i_Ptn].m_iNodeNo;

    vTrace("  m_xaNodePtn[tPtnNo %lld] { m_iNodeNo=%lld %s, m_i_Ptn=%lld }\n"_Stz
           
           ,i_Ptn
           ,iNodeNo
           ,AM::GRID::pGetNode(iNodeNo)->szNodeNm()
           ,m_xaNodePtn[i_Ptn].m_i_Ptn
           );
  }

  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get which node a logical partition is mapped to                            */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iWhichNodeNo(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 iNodeNo = m_xaNodePtn[tPtnNo].m_iNodeNo;
      
  return iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get which node partition a logical partition is mapped to                  */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichPtn(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_Ptn = m_xaNodePtn[tPtnNo].m_i_Ptn;
   
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* convert given i_ptn to partition number of given node                      */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tToPtnNoOfNode(i8 i_Ptn, i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  tPN tPtnNo = C_UNKNOWN;
  for (i8 i_NodePtn = 0; i_NodePtn < m_nPtnsInGrid; i_NodePtn++) {
    if (m_xaNodePtn[i_NodePtn].m_iNodeNo != iNodeNo)
      continue;
    if (m_xaNodePtn[i_NodePtn].m_i_Ptn != i_Ptn)
      continue;
    tPtnNo = i_NodePtn;
    break;
  }

  RASSERT(C_UNKNOWN != tPtnNo);

  return tPtnNo;
}



/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tToPtnNoOfThisNode(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  tPN tPtnNo = tToPtnNoOfNode(i_Ptn,iThisNodeNo);

  return tPtnNo;
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
    
  for (tPN tPtnNo = 0; tPtnNo < m_nPtnsInGrid; tPtnNo++) {
    i4 i_Node = (i4)tPtnNo % m_nKnownNodes;
    
    m_xaNodePtn[tPtnNo].m_iNodeNo = AM::GRID::iGetNthNodeNo(i_Node);
    m_xaNodePtn[tPtnNo].m_i_Ptn   = (i4)tPtnNo / m_nKnownNodes;
  }
  
  m_iMapChange++;
}

/*----------------------------------------------------------------------------*/
/* reset the {node,nodePtn} array                                             */
/*----------------------------------------------------------------------------*/
void PtnMap::vResetNodePtnArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PTNSINGRID; i_Ptn++) {
    m_xaNodePtn[i_Ptn].m_iNodeNo = C_UNKNOWN;
    m_xaNodePtn[i_Ptn].m_i_Ptn   = C_UNKNOWN;
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
