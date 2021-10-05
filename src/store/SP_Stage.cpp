/*============================================================================*/
/* SP_Stage.cpp - Stage sprint helper                                         */
/*============================================================================*/

#include "BASE.h"

#include "Stage.hpp"
#include "StageSpec.hpp"
#include "SP_Stage.hpp"
#include "SP_Stream.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Stage::SP_Stage(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Stage::SP_Stage(Stage* pStage, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
    
  vStage(pStage,bDescend);

  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Stage::~SP_Stage(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_Stage::vStage(Stage* pStage, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StageSpec* pStageSpec = pStage->pStageSpec();
  
  i8 i_Stage = pStageSpec->i_StageSpec();
  
  if (bDescend)
    vCatf("\n  "_Stz);
  
  vCatf("[%lld] Stage %s "
        "%s"_Stz
        
        ,i_Stage
        ,pStage->szStageNm()
        
        ,aPO[pStageSpec->eStageTopology()]
        );

  if (bDescend) {
    
    for (i8 i_Stream = 0; i_Stream < pStage->nItems(); i_Stream++) {
      Stream* pStream = pStage->pGet(i_Stream);
      SP_Stream spStream(pStream,true);
      vCatf("%s"_Stz,spStream.sz());
    }
    
  }
  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Stage::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Stage::sz(Stage* pStage)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vStage(pStage);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Stage::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
