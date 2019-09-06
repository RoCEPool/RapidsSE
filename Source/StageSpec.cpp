/*============================================================================*/
/* StageSpec.cpp -- Stage Specification                                       */
/*============================================================================*/

#include "Dynamics.h"


/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* StageSpec::operator new(size_t)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(HeapMem::pAllocate(CN_STAGESPEC));
  
  return pStageSpec;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void StageSpec::operator delete(void* aAddr)
{
  StageSpec* pStageSpec = reinterpret_cast<StageSpec*>(aAddr);
  
  HeapMem::vFree(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StageSpec::StageSpec(Stz* szName)
: Object(CN_STAGESPEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sName = szName;
  
   m_eTopology = PO_NONE;
  
   m_pOpSpecVec = new OpSpecVec;
  
   _checkInitialised_(CN_STAGESPEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StageSpec::~StageSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  delete m_pOpSpecVec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add op spec                                                                */
/*----------------------------------------------------------------------------*/
void StageSpec::vAddOpSpec(OpSpec* pOpSpec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_pOpSpecVec->vAdd(pOpSpec);
}

/*----------------------------------------------------------------------------*/
/* set locus                                                                  */
/*----------------------------------------------------------------------------*/
void StageSpec::vLocus(Stz* szLocus)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_sLocus = szLocus;
}

/*----------------------------------------------------------------------------*/
/* set topology                                                               */
/*----------------------------------------------------------------------------*/
void StageSpec::vTopology(ePO eTopology)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  m_eTopology = eTopology;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* print stage spec                                                           */
/*----------------------------------------------------------------------------*/
void StageSpec::vPrint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STAGESPEC,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("stage %s "_Stz,m_sName.aStz());
  
  switch (m_eTopology) {
    case PO_ALL_NODES:
      vTrace("on all nodes"_Stz);
      break;
    case PO_ALL_PARTITIONS:
      vTrace("on all partitions"_Stz);
      break;
    case PO_ORIGIN:
      vTrace("on origin"_Stz);
      break;
    case PO_SPECIFIC_NODE:
      vTrace("on node %s"_Stz,m_sLocus.aStz());
      break;
    case PO_SPECIFIC_PARTITION:
      vTrace("on partition %s"_Stz,m_sLocus.aStz());
      break;
    default:
      ;
  }
  vTrace(":\n"_Stz);


  vTrace("  "_Stz);
  for (i8 i_Op = 0; i_Op < m_pOpSpecVec->nItems(); i_Op++) {
    if (i_Op)
      vTrace(" | "_Stz);
    m_pOpSpecVec->pGet(i_Op)->vPrint();
  }
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(StageSpec)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
