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
MetaStructure::MetaStructure(Stz* szBlockNm, eMB eBlock,
                             MetaGraph* p_MetaGraph,
                             MetaStructure* p_ParentStructure)
: Object(CN_METASTRUCTURE)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  m_p_MetaGraph       = p_MetaGraph;
  m_p_ParentStructure = p_ParentStructure;

  m_sBlockNm = szBlockNm;
  m_nSize    = 0;
  m_eBlock   = eBlock;

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
/* add a new member set with given name                                       */
/*----------------------------------------------------------------------------*/
MetaMember* MetaStructure::pAddMember(Stz* szMemberNm)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  MetaMember* pMember = nullptr; //new MetaMember(szMemberNm,this);
    
  vAppend(pMember);
  
  return pMember;
}

/*----------------------------------------------------------------------------*/
/* get member set kind                                                        */
/*----------------------------------------------------------------------------*/
eMB MetaStructure::eBlock(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_eBlock;
}

/*----------------------------------------------------------------------------*/
/* get parent member set                                                      */
/*----------------------------------------------------------------------------*/
MetaStructure* MetaStructure::p_ParentStructure(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
  
  return m_p_ParentStructure;
}

/*----------------------------------------------------------------------------*/
/* set member set kind                                                        */
/*----------------------------------------------------------------------------*/
void MetaStructure::vBlock(eMB eBlock)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  m_eBlock = eBlock;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaStructure::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  vTrace("%MetaStructure %s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sBlockNm.azChar(),
         aMB[m_eBlock]->azChar(),
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
/* get member set name                                                        */
/*----------------------------------------------------------------------------*/
Stz* MetaStructure::szBlockNm(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_sBlockNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set member set name                                                        */
/*----------------------------------------------------------------------------*/
void MetaStructure::vBlockNm(Stz* szBlockNm)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  m_sBlockNm = szBlockNm;
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this member set                                      */
/*----------------------------------------------------------------------------*/
i8 MetaStructure::nSize(void)
{
  _precondition_(_MAGIC_,CN_METASTRUCTURE,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
