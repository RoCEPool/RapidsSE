/*============================================================================*/
/* PtnMap.cpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/

#include "BASE.h"

//#include "Gid_t.hpp"
//#include "PtnId.hpp"
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

  vInitNodeBktArray();
  
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
PtnId PtnMap::tHashToDtdPtnId(i8 iHash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  PtnId tPtnId;
  i8 tDtdPtnNo = iHash % m_nPtnsInGrid;
  tPtnId = tDtdPtnNo;
  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/* convert PtnId to Bkt No                                                      */
/*----------------------------------------------------------------------------*/
i8 PtnMap::iToBkt(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  return tPtnId.iAsI8() < C_MAX_PTNSINGRID ? m_xaBkt[(i8) tPtnId.iAsI8()].m_i_Bkt : 0;
}

/*----------------------------------------------------------------------------*/
/* convert iBkt to PtnId                                                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tToPtn(i8 iBktNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  PtnId tPtnId;

  tPtnId = iBktNo < C_MAX_PTNSINGRID ? m_xaBkt[(i8) iBktNo].m_i_Ptn : 0;
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

  i8 iNodeNo = m_xaNodePtn[tPtnId.iAsI8()].m_iNodeNo;

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
/* convert given round robin to distributed partition number                  */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tRobinToDtdPtnNo(i8 iRobin)
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

  i8 i_Ptn = iRobin % m_nPtnsPerNode;

  PtnId tPtnId;
  tPtnId = tToPtnNoOfThisNode(i_Ptn);

  return tPtnId;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to local partition number                        */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tRobinToNdePtnNo(i8 iRobin)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_Ptn = iRobin % m_nPtnsPerNode;

  PtnId tPtnId = tToPtnNoOfThisNode(i_Ptn);

  return tPtnId;
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

    vTrace("  m_xaNodePtn[tPtnId %lld] { m_iNodeNo=%lld %s, m_i_Ptn=%lld }\n"_Stz

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
i8 PtnMap::iWhichNodeNo(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iNodeNo = m_xaNodePtn[tPtnId.iAsI8()].m_iNodeNo;

  return iNodeNo;
}

/*----------------------------------------------------------------------------*/
/* get which node partition a logical partition is mapped to                  */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichPtn(PtnId tPtnId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_Ptn = m_xaNodePtn[tPtnId.iAsI8()].m_i_Ptn;

  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* convert given i_ptn to partition number of given node                      */
/*----------------------------------------------------------------------------*/
PtnId PtnMap::tToPtnNoOfNode(i8 i_Ptn, i8 iNodeNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  PtnId tPtnId;

  tPtnId = C_UNKNOWN;
  for (i8 i_NodePtn = 0; i_NodePtn < m_nPtnsInGrid; i_NodePtn++) {
    if (m_xaNodePtn[i_NodePtn].m_iNodeNo != iNodeNo)
      continue;
    if (m_xaNodePtn[i_NodePtn].m_i_Ptn != i_Ptn)
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
PtnId PtnMap::tToPtnNoOfThisNode(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 iThisNodeNo = AM::GRID::iThisNodeNo();

  PtnId tPtnId = tToPtnNoOfNode(i_Ptn,iThisNodeNo);

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

  for (i8 tPtnNo = 0; tPtnNo < m_nPtnsInGrid; tPtnNo++) {
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

/*----------------------------------------------------------------------------*/
/* initialize the Bkt array                                                   */
/*----------------------------------------------------------------------------*/
void PtnMap::vInitNodeBktArray(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_PTNMAP, _FOOTER_);

#ifdef WITH_BKTMAP_2N4P
  //AM::PTNMAP::nPtnsInGrid()

  i8 i_Ptn = 0;
  i8 i_Bkt = 0;

  while (i_Bkt < C_MAX_BUCKETS)
  {
    m_xaBkt[i_Bkt].m_i_Ptn = i_Ptn;
    m_xaBkt[++i_Bkt].m_i_Bkt = i_Ptn;

    i_Bkt++;
    i_Ptn++;
  }
#else

  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PTNSINGRID; i_Ptn++) {
    m_xaBkt[i_Ptn].m_i_Ptn = i_Ptn;
    m_xaBkt[i_Ptn].m_i_Bkt = i_Ptn;
  }
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
