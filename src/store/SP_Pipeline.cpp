/*============================================================================*/
/* SP_Pipeline.cpp - Pipeline sprint helper                                   */
/*============================================================================*/

#include "BASE.h"

#include "Pipeline.hpp"
#include "Vertex.hpp"
#include "Grid.hpp"

#include "SP_Pipeline.hpp"
#include "SP_Stage.hpp"
#include "SP_PipeWork.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Pipeline::SP_Pipeline(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Pipeline::SP_Pipeline(Pipeline* pPipeline, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vPipeline(pPipeline,bDescend);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Pipeline::~SP_Pipeline(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_Pipeline::vPipeline(Pipeline* pPipeline, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Grid* pGrid = AM::pGrid();
    
  vCatf("Pipeline %s %p "
        "on Grid %s N%lldP%lld "
        "using PipeSpec %p"_Stz
        
        ,pPipeline->szPipelineNm()
        ,pPipeline
        
        ,pGrid->szGridNm()
        ,pGrid->nKnownNodes()
        ,AM::PTNMAP::nPtnsPerNode()
        
        ,pPipeline->pPipeSpec()
        );
  
  if (bDescend) {
    
    SP_PipeWork spPipeWork;

    Vertex* pPipeWork = pPipeline->pVertexRoot();
    
    vCatf("%s"_Stz,spPipeWork.sz(pPipeWork));

//    for (i8 i_Stage = 0; i_Stage < pPipeWork->nItems(); i_Stage++ ) {
//      Stage* pStage = pPipeWork->pGet(i_Stage);
//      SP_Stage spStage(pStage,true);
//      vCatf("%s"_Stz,spStage.sz());
//    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_Pipeline::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Pipeline::sz(Pipeline* pPipeline, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vPipeline(pPipeline,bDescend);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_Pipeline::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
