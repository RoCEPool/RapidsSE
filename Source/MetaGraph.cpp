/*============================================================================*/
/* MetaGraph.cpp - MetaGraph                                                  */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaGraph,CN_METAGRAPH)
__aNewFunction(MetaGraph,CN_METAGRAPH)
__vDeleteFunction(MetaGraph,CN_METAGRAPH)
__aFooterFunction(MetaGraph)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaGraph::MetaGraph(void)
: Object(CN_METAGRAPH)
{
  _precondition_(_MAGIC_,CN_METAGRAPH,_FOOTER_);
  
  m_pConstSet = new ConstSet(this);
  
  _checkInitialised_(CN_METAGRAPH);
  
  _postcondition_(_MAGIC_,CN_METAGRAPH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaGraph::~MetaGraph(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METAGRAPH,_FOOTER_);
  
  delete m_pConstSet;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get const set                                                              */
/*----------------------------------------------------------------------------*/
ConstSet* MetaGraph::pConstSet(void)
{
  return m_pConstSet;
}

/*----------------------------------------------------------------------------*/
/* add a new metaclass with given name                                        */
/*----------------------------------------------------------------------------*/
MetaClass* MetaGraph::pAddMetaClass(Stz* szClassNm)
{
  _precondition_(_MAGIC_,CN_METAGRAPH,_FOOTER_);
  
  MetaClass* pMetaClass = new MetaClass(this);
  
  pMetaClass->vClassNm(szClassNm);
  
  vAppend(pMetaClass);
  
  return pMetaClass;
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaGraph::vRecPrint(void)
{
  _precondition_(_MAGIC_,CN_METAGRAPH,_FOOTER_);
    
  m_pConstSet->vPrint(0);  
  
  for (MetaClass* pMetaClass = pHead();
       pMetaClass;
       pMetaClass = pMetaClass->pNextItem())
    pMetaClass->vRecPrint(0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
