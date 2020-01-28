/*============================================================================*/
/* MemberSet.cpp - MemberSet                                                  */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MemberSet,CN_MEMBERSET)
__aNewFunction(MemberSet,CN_MEMBERSET)
__vDeleteFunction(MemberSet,CN_MEMBERSET)
__aFooterFunction(MemberSet)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MemberSet::MemberSet(MetaClass* p_MetaClass, MemberSet* p_ParentMbrSet)
: Object(CN_MEMBERSET)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
  
  m_p_MetaClass = p_MetaClass;
  m_p_ParentMbrSet = p_ParentMbrSet;

  m_sSetNm = "anonymous"_Stz;
  m_nSize  = 0;
  m_eSet   = MS_LINEAR;

  _checkInitialised_(CN_MEMBERSET);
  
  _postcondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MemberSet::~MemberSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
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
MemberVar* MemberSet::pAddMemberVar(Stz* szVarNm)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
  
  MemberVar* pMemberVar = new MemberVar(this);
  
  pMemberVar->vVarNm(szVarNm);
  
  vAppend(pMemberVar);
  
  return pMemberVar;
}

/*----------------------------------------------------------------------------*/
/* get member set kind                                                        */
/*----------------------------------------------------------------------------*/
eMS MemberSet::eMemberSet(void)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
  return m_eSet;
}

/*----------------------------------------------------------------------------*/
/* get parent member set                                                      */
/*----------------------------------------------------------------------------*/
MemberSet* MemberSet::pParentMbrSet(void)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
  
  return m_p_ParentMbrSet;
}

/*----------------------------------------------------------------------------*/
/* set member set kind                                                        */
/*----------------------------------------------------------------------------*/
void MemberSet::vMemberSet(eMS eMemberSet)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
  m_eSet = eMemberSet;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MemberSet::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
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
void MemberSet::vRecPrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MemberVar* pMbrVar = pHead(); pMbrVar; pMbrVar = pMbrVar->pNextItem())
    pMbrVar->vRecPrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get member set name                                                        */
/*----------------------------------------------------------------------------*/
Stz* MemberSet::szSetNm(void)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
  return m_sSetNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set member set name                                                        */
/*----------------------------------------------------------------------------*/
void MemberSet::vSetNm(Stz* szSetNm)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
  m_sSetNm = szSetNm;
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this member set                                      */
/*----------------------------------------------------------------------------*/
i8 MemberSet::nSize(void)
{
  _precondition_(_MAGIC_,CN_MEMBERSET,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
