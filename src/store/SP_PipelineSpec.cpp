/*============================================================================*/
/* SP_PipelineSpec.cpp - PipelineSpec sprint helper                           */
/*============================================================================*/

#include "BASE.h"

#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"

#include "SP_PipelineSpec.hpp"
#include "SP_StageSpec.hpp"
#include "SP_OpSpec.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_PipelineSpec::SP_PipelineSpec(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_PipelineSpec::SP_PipelineSpec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vPipelineSpec(pPipelineSpec);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_PipelineSpec::~SP_PipelineSpec(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_PipelineSpec::vPipelineSpec(PipelineSpec* pPipelineSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  SP_StageSpec spStageSpec;
  SP_OpSpec spOpSpec;

  i8 iDepth = 0;

  vInit();
  
  if (iDepth)
    vCatf(iDepth,
          "PipelineSpec %s %p\n"_Stz
          ,pPipelineSpec->szPipelineNm()
          ,pPipelineSpec
          );
  else
    vCatf("PipelineSpec %s %p\n"_Stz
          ,pPipelineSpec->szPipelineNm()
          ,pPipelineSpec
          );
//  vCats(iDepth,
//        "begin\n"_Stz);
  
  for (i8 i_Stage = 0; i_Stage < pPipelineSpec->nItems(); i_Stage++) {
    StageSpec* pStageSpec = pPipelineSpec->pStageSpec(i_Stage);
    vCatf(iDepth,
          "  %s"_Stz,spStageSpec.sz(pStageSpec));
    
    for (i8 i_Op = 0; i_Op < pStageSpec->nItems(); i_Op++) {
      OpSpec* pOpSpec = pStageSpec->pOpSpec(i_Op);
//      if (! i_Op)
//        vCatf(iDepth,
//              "        %s"_Stz,spOpSpec.sz(pOpSpec));
//      else
        vCatf(iDepth,
              "    %s"_Stz,spOpSpec.sz(pOpSpec));
    }
  }
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_PipelineSpec::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_PipelineSpec::sz(PipelineSpec* pPipelineSpec)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vPipelineSpec(pPipelineSpec);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_PipelineSpec::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
