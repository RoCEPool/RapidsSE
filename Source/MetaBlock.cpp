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
MetaBlock::MetaBlock(MetaClass* p_MetaClass, MetaBlock* p_ParentBlock)
: Object(CN_METABLOCK)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  m_p_MetaClass = p_MetaClass;
  m_p_ParentBlock = p_ParentBlock;

  m_sSetNm = "anonymous"_Stz;
  m_nSize  = 0;
  m_eSet   = MS_LINEAR;

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
eMS MetaBlock::eMemberSet(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_eSet;
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
void MetaBlock::vMemberSet(eMS eMemberSet)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_eSet = eMemberSet;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaBlock::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  vTrace("%sMemberSet %s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sSetNm.azChar(),
         aMS[m_eSet]->azChar(),
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
Stz* MetaBlock::szSetNm(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_sSetNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set member set name                                                        */
/*----------------------------------------------------------------------------*/
void MetaBlock::vSetNm(Stz* szSetNm)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_sSetNm = szSetNm;
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
