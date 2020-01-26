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
/* print term sequence                                                        */
/*----------------------------------------------------------------------------*/
void MetaGraph::vPrintTermSeq(EnameSet& roTerms)
{
  i8 nTerms = roTerms.nEnames();
  
  for (i8 i_Term = 0; i_Term < nTerms; i_Term++) {
    if (i_Term)
      vTrace(" "_Stz);
    vTrace("%s"_Stz,roTerms.szName(i_Term));
  }
}

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
