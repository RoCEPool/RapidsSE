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
MemberSet* MetaClass::pAddMemberSet(Stz* szSetNm)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
  
  MemberSet* pMemberSet = new MemberSet(this);
  
  pMemberSet->vSetNm(szSetNm);
  
  vAppend(pMemberSet);
  
  return pMemberSet;
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
    
  vTrace("%sMetaClass %s\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sClassNm.azChar()
         );
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaClass::vRecPrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METACLASS,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MemberSet* pMbrSet = pHead(); pMbrSet; pMbrSet = pMbrSet->pNextItem())
    pMbrSet->vRecPrint(i_Depth + 1);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
