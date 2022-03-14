/*============================================================================*/
/* SP_Formwork.cpp - Pipeline sprint helper                                   */
/*============================================================================*/

#include "BASE.h"

#include "Pipeline.hpp"
#include "Grid.hpp"

#include "SP_Formwork.hpp"
#include "SP_Stage.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Formwork::SP_Formwork(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Formwork::SP_Formwork(Pipeline* pPipeline, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vPipeline(pPipeline,bDescend);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Formwork::~SP_Formwork(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_Formwork::vPipeline(Pipeline* pPipeline, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Grid* pGrid = AM::pGrid();
    
  vCatf("Pipeline %s %p "
        "on Grid %s N%lldP%lld "
        "using PipelineSpec %p"_Stz
        
        ,pPipeline->szPipelineNm()
        ,pPipeline
        
        ,pGrid->szGridNm()
        ,pGrid->nKnownNodes()
        ,AM::PTNMAP::nPtnsPerNode()
        
        ,pPipeline->pPipelineSpec()
        );
  
  if (bDescend) {
    for (i8 i_Stage = 0; i_Stage < pPipeline->nItems(); i_Stage++ ) {
      Stage* pStage = pPipeline->pGet(i_Stage);
      SP_Stage spStage(pStage,true);
      vCatf("%s"_Stz,spStage.sz());
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Formwork::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Formwork::sz(Pipeline* pPipeline)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vPipeline(pPipeline);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Formwork::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
