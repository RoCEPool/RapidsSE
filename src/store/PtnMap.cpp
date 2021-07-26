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
  
  m_nKnownNodes  = nKnownNodes;
  m_nPtnsPerNode = nPtnsPerNode;
  m_nPtnsInGrid  = 0;

  vResetNodePtnArray();
  
  _checkInitialised_(CN_PTNMAP);
  
  vRepartition(nKnownNodes);
  
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
/* convert hash to logical partition number                                   */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tHashToPtnNo(i8 iHash, i8 nPtnsInGrid)
{
  _static_
  
  i8 tPtnNo = iHash % nPtnsInGrid;

  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* update running hash                                                        */
/*----------------------------------------------------------------------------*/
void PtnMap::vRunningHash(i8* aiHash, i8 iMixIn)
{
  _static_
  
  * aiHash += HshLib::iHash((byte*)&iMixIn,sizeof(w8));
}

/*----------------------------------------------------------------------------*/
/* convert i_Stream to logical partition number (in grid)                     */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tStreamToLglPtnNo(i8 i_Stream)
{
  _static_

  //if (AM::PTNMAP::nPtnsInGrid()) {
  //  for (tPN tPtnNo = 0; tPtnNo < AM::PTNMAP::nPtnsInGrid(); tPtnNo++)
  //    vTrace("tStreamToLglPtnNo  Partition: %lld; Node: %lld\n"_Stz,tPtnNo,AM::pPtnMap()->i_WhichNode(tPtnNo));
  //  vTrace("\n"_Stz);
  //}

  //vTrace("PtnMap::tStreamToLglPtnNo %lld %lld %lld\n"_Stz, i_Stream ,AM::nConnectedNodes(),AM::iThisNodeNo());

  // THIS IS WHERE THE CRASH HAPPENED
  // Because if the nodeNo is e.g. 120, then the partitionNo becomes large, will fail
  tPN tPtnNo = (i_Stream * AM::GRID::nKnownNodes()) + 0;//AM::iThisNodeNo();



  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust partition map for given number of nodes and partitions per node     */
/*----------------------------------------------------------------------------*/
void PtnMap::vAdjustMap(i8 nKnownNodes, i8 nPtnsPerNode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  vRepartition(nKnownNodes);
}

/*----------------------------------------------------------------------------*/
/* dump grid                                                                  */
/*----------------------------------------------------------------------------*/
void PtnMap::vDumpDataBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  if (AM::PTNMAP::nPtnsInGrid()) {
    for (tPN tPtnNo = 0; tPtnNo < AM::PTNMAP::nPtnsInGrid(); tPtnNo++)
      vTrace("Partition: %lld; Node: %lld\n"_Stz,tPtnNo,i_WhichNode(tPtnNo));
    vTrace("\n"_Stz);
  }
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
  
}

/*----------------------------------------------------------------------------*/
/* get which node a logical partition is mapped to                            */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichNode(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_Node = m_xaNodePtn[tPtnNo].m_i_Node;
    
  //RASSERT(i_Node == i_LglPtnNoToInitialNode(tPtnNo));
  
  return i_Node;
}

/*----------------------------------------------------------------------------*/
/* get which stream a logical partition is mapped to                          */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichStream(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_Stream = m_xaNodePtn[tPtnNo].m_i_NodePtn;
   
  //RASSERT(i_Stream == i_LglPtnNoToInitialStream(tPtnNo));

  return i_Stream;
}

/*----------------------------------------------------------------------------*/
/* get which table partition a logical partition is mapped to                 */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichTablePtn(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_TablePtn = m_xaNodePtn[tPtnNo].m_i_NodePtn;
   
  RASSERT(i_TablePtn == i_LglPtnNoToInitialStream(tPtnNo));

  return i_TablePtn;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert grid partition number to initial node it is to be mapped to        */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_LglPtnNoToInitialNode(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_Node = tPtnNo % AM::GRID::nKnownNodes();
  vTrace("i_LglPtnNoToInitialNode    BootNodes:%lld i_Node:%lld\n"_Stz,AM::GRID::nKnownNodes(), i_Node);
  return i_Node;
}

/*----------------------------------------------------------------------------*/
/* convert grid partition number to initial node it is to be mapped to        */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_LglPtnNoToInitialStream(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);

  i8 i_Stream = tPtnNo / AM::GRID::nKnownNodes();
  vTrace("i_LglPtnNoToInitialStream    BootNodes:%lld i_Stream:%lld\n"_Stz,AM::GRID::nKnownNodes(), i_Stream);
  return i_Stream;
}

/*----------------------------------------------------------------------------*/
/* repartition the map based on new known nodes                               */
/*----------------------------------------------------------------------------*/
void PtnMap::vRepartition(i8 nKnownNodes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  m_nKnownNodes = nKnownNodes;
  m_nPtnsInGrid = nKnownNodes * m_nPtnsPerNode;
  RASSERT(m_nPtnsInGrid);

  vResetNodePtnArray();
    
  for (tPN tPtnNo = 0; tPtnNo < m_nPtnsInGrid; tPtnNo++) {
    
    // newcode1 we have to replace the node index here with node ID
    i4 i_Node = (i4)tPtnNo % nKnownNodes;
    
    m_xaNodePtn[tPtnNo].m_i_Node =
      //(i4)i_LglPtnNoToInitialNode(tPtnNo);
      //(i4)tPtnNo % nNodes; // oldcode1
      AM::GRID::iGetNthNodeNo(i_Node);  // newcode1    <<<<<<<<<<<<<<<<<
    
    m_xaNodePtn[tPtnNo].m_i_NodePtn =
      //(i4)i_LglPtnNoToInitialStream(tPtnNo);
      (i4)tPtnNo / nKnownNodes;
  }
}

/*----------------------------------------------------------------------------*/
/* reset the {node,nodePtn} array                                             */
/*----------------------------------------------------------------------------*/
void PtnMap::vResetNodePtnArray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PTNSINGRID; i_Ptn++) {
    m_xaNodePtn[i_Ptn].m_i_Node    = C_UNKNOWN;
    m_xaNodePtn[i_Ptn].m_i_NodePtn = C_UNKNOWN;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
