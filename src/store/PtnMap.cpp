/*============================================================================*/
/* PtnMap.cpp -- Partition (to node,tablePtn) Map                             */
/*============================================================================*/

#include "BASE.h"

#include "PtnMap.hpp"

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
/* convert running partition-key hash to logical partition number             */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tHashToPtnNo(i8 iPtnKeyHash)
{
  _static_
  
  i8 tPtnNo = iPtnKeyHash % AM::PTNMAP::nPtnsInGrid();

  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* update running partition-key hash with given mix in value                  */
/*----------------------------------------------------------------------------*/
void PtnMap::vRunningHash(i8* aiPtnKeyHash, i8 iMixIn)
{
  _static_
  
  * aiPtnKeyHash += HshLib::iHash((byte*)&iMixIn,sizeof(iMixIn));
}

/*----------------------------------------------------------------------------*/
/* convert i_Stream to logical partition number (in grid)                     */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tStreamToLglPtnNo(i8 i_Stream)
{
  _static_

  //if (AM::PTNMAP::nPtnsInGrid()) {
  //  for (tPN tPtnNo = 0; tPtnNo < AM::PTNMAP::nPtnsInGrid(); tPtnNo++)
  //    vTrace("tStreamToLglPtnNo  Partition: %lld; Node: %lld\n"_Stz,tPtnNo,AM::pPtnMap()->iWhichNodeNo(tPtnNo));
  //  vTrace("\n"_Stz);
  //}

  //vTrace("PtnMap::tStreamToLglPtnNo %lld %lld %lld\n"_Stz, i_Stream ,AM::nConnectedNodes(),AM::iThisNodeNo());

  // THIS IS WHERE THE CRASH HAPPENED
  // Because if the nodeNo is e.g. 120, then the partitionNo becomes large, will fail
  tPN tPtnNo = (i_Stream * AM::GRID::nKnownNodes()) + 0;//AM::iThisNodeNo();



  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to logical partition number                      */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tRobinToPtnNo(i8 iRobin)
{
  _static_
  
  tPN tPtnNo = iRobin;
  
  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* convert given round robin to node partition (aka partition of node)        */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_RobinToPtn(i8 iRobin)
{
  _static_
  
  i8 i_Ptn = iRobin;
  
  return i_Ptn;
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
/* see ptnmap                                                                 */
/*----------------------------------------------------------------------------*/
void PtnMap::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  vTrace("PtnMap:\n"_Stz);
  
  for (i8 i_Ptn = 0; i_Ptn < m_nPtnsInGrid; i_Ptn++) {
    
    vTrace("[%lld] (NodeNo %lld, i_Ptn %lld)\n"_Stz
           
           ,i_Ptn
           ,m_xaNodePtn[i_Ptn].m_iNodeNo
           ,m_xaNodePtn[i_Ptn].m_i_Ptn
           );
  }

  vTrace("\n"_Stz);
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
