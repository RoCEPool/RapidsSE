/*============================================================================*/
/* PtnMap.cpp -- Partition (to node) Map                                      */
/*============================================================================*/

#include "Partitioning.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Partitioning,PtnMap,CN_PTNMAP)
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
  
  for (i8 i_Ptn = 0; i_Ptn < C_MAX_PTNS; i_Ptn++) {
    m_iaInNode[i_Ptn] = C_UNKNOWN;
  }

  _checkInitialised_(CN_PTNMAP);
  
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
/* convert grid-wide partition number to which node its on                    */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_PtnNoToNode(tPN tPtnNo)
{
  _static_
  
  i8 i_Node = tPtnNo % AM::nDefinedNodes();
  
  return i_Node;
}

/*----------------------------------------------------------------------------*/
/* convert grid-wide partition number to strand on its node                   */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_PtnNoToStrand(tPN tPtnNo)
{
  _static_
  
  i8 i_Strand = tPtnNo / AM::nDefinedNodes();
  
  return i_Strand;
}

/*----------------------------------------------------------------------------*/
/* convert i_Strand to partition number (in grid)                             */
/*----------------------------------------------------------------------------*/
tPN PtnMap::tStrandToPtnNo(i8 i_Strand)
{
  _static_
  
  tPN tPtnNo = (i_Strand * AM::nDefinedNodes()) + AM::iThisNodeNo();
  
  return tPtnNo;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get which partition a bucket is currently mapped to                        */
/*----------------------------------------------------------------------------*/
i8 PtnMap::i_WhichNode(i8 i_Ptn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PTNMAP,_FOOTER_);
  
  i8 i_Node = m_iaInNode[i_Ptn];
  
  return i_Node;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
