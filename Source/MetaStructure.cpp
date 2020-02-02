/*============================================================================*/
/* MetaStructure.cpp - Meta Structure (definition of class, struct, union)    */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaStructure,CN_METASTRUCTURE)
__aNewFunction(MetaStructure,CN_METASTRUCTURE)
__vDeleteFunction(MetaStructure,CN_METASTRUCTURE)
__aFooterFunction(MetaStructure)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaStructure::MetaStructure(Stz* szStructureNm, eMS eStructure,
                             MetaGraph* p_MetaGraph,
                             MetaStructure* p_ParentStructure)
: Object(CN_METASTRUCTURE)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  m_p_MetaGraph       = p_MetaGraph;
  m_p_ParentStructure = p_ParentStructure;

  m_sStructureNm = szStructureNm;
  m_eStructure   = eStructure;
  
  //m_oTplArgs
  
  m_nSize        = 0;

  _checkInitialised_(CN_METASTRUCTURE);
  
  _postcondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaStructure::~MetaStructure(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new member with given name                                           */
/*----------------------------------------------------------------------------*/
MetaMember* MetaStructure::pAddMember(Stz* szMemberNm)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  MetaMember* pMember = nullptr; //new MetaMember(szMemberNm,this);
    
  vAppend(pMember);
  
  return pMember;
}

/*----------------------------------------------------------------------------*/
/* get parent structure                                                       */
/*----------------------------------------------------------------------------*/
MetaStructure* MetaStructure::p_ParentStructure(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  return m_p_ParentStructure;
}

/*----------------------------------------------------------------------------*/
/* get structure kind                                                         */
/*----------------------------------------------------------------------------*/
eMS MetaStructure::eStructure(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_eStructure;
}

/*----------------------------------------------------------------------------*/
/* set structure kind                                                         */
/*----------------------------------------------------------------------------*/
void MetaStructure::vStructure(eMS eStructure)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  m_eStructure = eStructure;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaStructure::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  vTrace("%sMetaStructure %16s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sStructureNm.azChar(),
         aMS[m_eStructure]->azChar(),
         m_nSize
         );
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaStructure::vRecursivePrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MetaMember* pMember = pHead(); pMember; pMember = pMember->pNextItem())
    pMember->vRecursivePrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this structure                                       */
/*----------------------------------------------------------------------------*/
i8 MetaStructure::nSize(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_nSize;
}

/*----------------------------------------------------------------------------*/
/* get structure name                                                         */
/*----------------------------------------------------------------------------*/
Stz* MetaStructure::szStructureNm(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_sStructureNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set structure name                                                         */
/*----------------------------------------------------------------------------*/
void MetaStructure::vStructureNm(Stz* szStructureNm)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  m_sStructureNm = szStructureNm;
}

/*----------------------------------------------------------------------------*/
/* get template arguments                                                     */
/*----------------------------------------------------------------------------*/
EnameSet* MetaStructure::pTplArgs(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return & m_oTplArgs;
}

/*----------------------------------------------------------------------------*/
/* set template arguments                                                     */
/*----------------------------------------------------------------------------*/
void MetaStructure::vTplArgs(EnameSet* pTplArgs)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  m_oTplArgs.vCopy(pTplArgs);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
