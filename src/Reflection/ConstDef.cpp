/*============================================================================*/
/* ConstDef.cpp - Member Variable                                            */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,ConstDef,CN_CONSTDEF)
__aNewFunction(ConstDef,CN_CONSTDEF)
__vDeleteFunction(ConstDef,CN_CONSTDEF)
__aFooterFunction(ConstDef)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ConstDef::ConstDef(ConstSet* p_ConstSet)
: Object(CN_CONSTDEF)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
  
  m_p_ConstSet = p_ConstSet;
  
  m_sConstNm = C_NO_NAME;
  m_iConst   = C_UNKNOWN;

  //m_oTerms
  
  _checkInitialised_(CN_CONSTDEF);
  
  _postcondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ConstDef::~ConstDef(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if term set can be recogised as a member variable                     */
/*----------------------------------------------------------------------------*/
bool ConstDef::bIsConstDef(Stz* szConstNm, TagSet* pTerms)
{
  _static_
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get constant name from given terms                                         */
/*----------------------------------------------------------------------------*/
Stz* ConstDef::szConstNm(TagSet* pTerms)
{
  _static_
  
  Stz* szName = "*UNKNOWN*"_Stz;

  return szName;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get const name                                                             */
/*----------------------------------------------------------------------------*/
Stz* ConstDef::szConstNm(void)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
    
  return m_sConstNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set const name                                                             */
/*----------------------------------------------------------------------------*/
void ConstDef::vConstNm(Stz* szConstNm)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
    
  m_sConstNm = szConstNm;
}

/*----------------------------------------------------------------------------*/
/* get const value                                                            */
/*----------------------------------------------------------------------------*/
i8 ConstDef::iConst(void)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
    
  return m_iConst;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void ConstDef::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
     
  vTrace("%sConst %-16s %6lld :- "_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sConstNm.azChar(),
         m_iConst,
         
         C_VARGS_END);
  
  MetaGraph::vPrintTermSeq(m_oTerms);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get const terms                                                            */
/*----------------------------------------------------------------------------*/
void ConstDef::vTerms(TagSet& roTerms)
{
  _precondition_(_MAGIC_,CN_CONSTDEF,_FOOTER_);
    
  m_oTerms.vCopy(&roTerms);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
