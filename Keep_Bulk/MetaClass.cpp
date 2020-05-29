/*============================================================================*/
/* MetaClass.cpp - MetaClass                                                  */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaClass,CN_METACLASS)
__aNewFunction(MetaClass,CN_METACLASS)
__vDeleteFunction(MetaClass,CN_METACLASS)
__aFooterFunction(MetaClass)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaClass::MetaClass(MetaGraph* p_MetaGraph)
: Object(CN_METACLASS)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
  
  m_p_MetaGraph = p_MetaGraph;
  
  m_sClassNm = C_NO_NAME;
  m_nSize    = 0;
  m_eClass   = CC_CLASS;
  
  _checkInitialised_(CN_METACLASS);
  
  _postcondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaClass::~MetaClass(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new member set with given name                                       */
/*----------------------------------------------------------------------------*/
MetaBlock* MetaClass::pAddBlock(Stz* szBlockNm, MetaBlock* p_ParentBlock)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
  
  MetaBlock* pBlock = new MetaBlock(m_p_MetaGraph,p_ParentBlock);
  
  pBlock->vBlockNm(szBlockNm);
  
  vAppend(pBlock);
  
  return pBlock;
}

/*----------------------------------------------------------------------------*/
/* get class name                                                             */
/*----------------------------------------------------------------------------*/
Stz* MetaClass::szClassNm(void)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
    
  return m_sClassNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set class name                                                             */
/*----------------------------------------------------------------------------*/
void MetaClass::vClassNm(Stz* szClassNm)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
    
  m_sClassNm = szClassNm;
}

/*----------------------------------------------------------------------------*/
/* get meta class kind                                                        */
/*----------------------------------------------------------------------------*/
eCC MetaClass::eMetaClass(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_eClass;
}

/*----------------------------------------------------------------------------*/
/* set meta class kind                                                        */
/*----------------------------------------------------------------------------*/
void MetaClass::vMetaClass(eCC eMetaClass)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  m_eClass = eMetaClass;
}

/*----------------------------------------------------------------------------*/
/* get which meta graph this meta class is within                             */
/*----------------------------------------------------------------------------*/
MetaGraph* MetaClass::pMetaGraph(void)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
    
  return m_p_MetaGraph;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaClass::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
    
  vTrace("%sMetaClass %s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sClassNm.azChar(),
         aCC[m_eClass]->azChar(),
         m_nSize
         );
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaClass::vRecPrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MetaBlock* pMbrSet = pHead(); pMbrSet; pMbrSet = pMbrSet->pNextItem())
    pMbrSet->vRecPrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get rolled-up size of this class                                           */
/*----------------------------------------------------------------------------*/
i8 MetaClass::nSize(void)
{
  _precondition_(_MAGIC_,CN_METAMEMBER,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
