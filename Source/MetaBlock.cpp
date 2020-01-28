/*============================================================================*/
/* MetaBlock.cpp - MetaBlock                                                  */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaBlock,CN_METABLOCK)
__aNewFunction(MetaBlock,CN_METABLOCK)
__vDeleteFunction(MetaBlock,CN_METABLOCK)
__aFooterFunction(MetaBlock)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaBlock::MetaBlock(MetaGraph* p_MetaGraph, MetaBlock* p_ParentBlock)
: Object(CN_METABLOCK)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  m_p_MetaGraph   = p_MetaGraph;
  m_p_ParentBlock = p_ParentBlock;

  m_sBlockNm = "anonymous"_Stz;
  m_nSize    = 0;
  m_eBlock   = MB_NONE;

  _checkInitialised_(CN_METABLOCK);
  
  _postcondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaBlock::~MetaBlock(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
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
MetaMember* MetaBlock::pAddMember(Stz* szVarNm)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  MetaMember* pMember = new MetaMember(this);
  
  pMember->vVarNm(szVarNm);
  
  vAppend(pMember);
  
  return pMember;
}

/*----------------------------------------------------------------------------*/
/* get member set kind                                                        */
/*----------------------------------------------------------------------------*/
eMB MetaBlock::eBlock(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_eBlock;
}

/*----------------------------------------------------------------------------*/
/* get parent member set                                                      */
/*----------------------------------------------------------------------------*/
MetaBlock* MetaBlock::pParentBlock(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  return m_p_ParentBlock;
}

/*----------------------------------------------------------------------------*/
/* set member set kind                                                        */
/*----------------------------------------------------------------------------*/
void MetaBlock::vBlock(eMB eBlock)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_eBlock = eBlock;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaBlock::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  vTrace("%sMemberSet %s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sBlockNm.azChar(),
         aMB[m_eBlock]->azChar(),
         m_nSize
         );
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaBlock::vRecPrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MetaMember* pMember = pHead(); pMember; pMember = pMember->pNextItem())
    pMember->vRecPrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get member set name                                                        */
/*----------------------------------------------------------------------------*/
Stz* MetaBlock::szBlockNm(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_sBlockNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set member set name                                                        */
/*----------------------------------------------------------------------------*/
void MetaBlock::vBlockNm(Stz* szBlockNm)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_sBlockNm = szBlockNm;
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this member set                                      */
/*----------------------------------------------------------------------------*/
i8 MetaBlock::nSize(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
