/*============================================================================*/
/* PtnMap.cpp -- Partition (to node) Map                                      */
/*============================================================================*/

#include "Partitioning.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(MetaData,PtnMap,CN_PTNMAP)
__aNewFunction(PtnMap,CN_PTNMAP)
__vDeleteFunction(PtnMap,CN_PTNMAP)
__aFooterFunction(PtnMap)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PtnMap::PtnMap(i8 nPartitions)
: Object(CN_PTNMAP)
, m_okLock(LK_PARTITION,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  for (i8 i = 0; i < C_MAX_PTNS; i++) {
    m_xaInNodeTablePtn[i].m_i_Node = C_UNKNOWN;
    m_xaInNodeTablePtn[i].m_i_TablePtn = C_UNKNOWN;
  }
  
  _checkInitialised_(CN_PTNMAP);
  
  RASSERT(nPartitions);
  for (tPN tPtnNo = 0; tPtnNo < nPartitions; tPtnNo++) {
    m_xaInNodeTablePtn[tPtnNo].m_i_Node = (i4)i_LglPtnNoToInitialNode(tPtnNo);
    m_xaInNodeTablePtn[tPtnNo].m_i_TablePtn = (i4)i_LglPtnNoToStrand(tPtnNo);
  }
  
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
/* convert hash to logical partition number                                   */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tHashToPtnNo(i8 iHash, i8 nPartitionsInGrid)
{
  _static_
  
  i8 tPtnNo = iHash % nPartitionsInGrid;

  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/* convert logical partition number (in grid) to strand on its node           */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_LglPtnNoToStrand(tPN tPtnNo)
{
  _static_
  
  i8 i_Strand = tPtnNo / AM::nDefinedNodes();
  
  return i_Strand;
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
/* convert i_Strand to logical partition number (in grid)                     */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tStrandToLglPtnNo(i8 i_Strand)
{
  _static_
  
  tPN tPtnNo = (i_Strand * AM::nDefinedNodes()) + AM::iThisNodeNo();
  
  return tPtnNo;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get which node a logical partition is mapped to                            */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichNode(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_Node = m_xaInNodeTablePtn[tPtnNo].m_i_Node;
    
  RASSERT(i_Node == i_LglPtnNoToInitialNode(tPtnNo));
  
  return i_Node;
}

/*----------------------------------------------------------------------------*/
/* get which table partition a logical partition is mapped to                 */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichTablePtn(tPN tPtnNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_TablePtn = m_xaInNodeTablePtn[tPtnNo].m_i_TablePtn;
    
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

  i8 i_Node = tPtnNo % AM::nDefinedNodes();
  
  return i_Node;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
