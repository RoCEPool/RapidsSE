/*============================================================================*/
/* SP_OpSpec.cpp - OpSpec sprint helper                                       */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "SP_OpSpec.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_OpSpec::SP_OpSpec(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_OpSpec::SP_OpSpec(OpSpec* pOpSpec)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vOpSpec(pOpSpec);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_OpSpec::~SP_OpSpec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_OpSpec::vOpSpec(OpSpec* pOpSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  s16KB sArgs;
  s16KB sTopo;

  vInit();

  TagSet* pTopoTags = pOpSpec->pTopoTags();
  pTopoTags->vSprint(sTopo,true,true);
  
  TagSet* pArgTags  = pOpSpec->pArgTags();
  
  if (pArgTags->nItems()) {
    pArgTags->vSprint(sArgs);
    vCatf("[%02lld] %s args=%stopo=%s"_Stz,
          pOpSpec->i_OpSpec(),
          pOpSpec->szOpNm(),
          sArgs.azChar(),
          sTopo.azChar());
  }
  else {
    vCatf("[%02lld] %s topo=%s"_Stz,
          pOpSpec->i_OpSpec(),
          pOpSpec->szOpNm(),
          sTopo.azChar());
  }
  
  if (pOpSpec->bAppendRight())
    vCats(" right"_Stz);
  
  if (XK_NONE != pOpSpec->eSendKind()) {
    vCatf(" %s"_Stz,aXK[pOpSpec->eSendKind()]);
  }

  vCats("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_OpSpec::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_OpSpec::sz(OpSpec* pOpSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vOpSpec(pOpSpec);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_OpSpec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
