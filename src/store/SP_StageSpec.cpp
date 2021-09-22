/*============================================================================*/
/* SP_StageSpec.cpp - StageSpec sprint helper                                 */
/*============================================================================*/

#include "BASE.h"

#include "StageSpec.hpp"
#include "SP_StageSpec.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_StageSpec::SP_StageSpec(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_StageSpec::SP_StageSpec(StageSpec* pStageSpec)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
    
  vStageSpec(pStageSpec);

  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_StageSpec::~SP_StageSpec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_StageSpec::vStageSpec(StageSpec* pStageSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vInit();
  
  vCatf("[%lld] StageSpec %s : %s N%lld P%lld\n"_Stz
        ,pStageSpec->i_StageSpec()
        ,pStageSpec->szStageNm()
        ,aPO[pStageSpec->eTopology()]
        ,pStageSpec->iSpecificNode()
#ifdef WITH_PTNID
        ,(i8)pStageSpec->tSpecificPtnNo().iAsI8()
#else
        ,(i8)pStageSpec->tSpecificPtnNo()
#endif
        );
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_StageSpec::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_StageSpec::sz(StageSpec* pStageSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vStageSpec(pStageSpec);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_StageSpec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
