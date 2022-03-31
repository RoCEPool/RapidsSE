/*============================================================================*/
/* SP_PipeWork.cpp - PipeWork sprint helper                                   */
/*============================================================================*/

#include "BASE.h"

#include "Pipeline.hpp"
#include "VertexSpec.hpp"
#include "Vertex.hpp"
#include "Grid.hpp"
#include "Deck.hpp"

#include "SP_PipeWork.hpp"
#include "SP_Stage.hpp"


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_PipeWork::SP_PipeWork(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_PipeWork::SP_PipeWork(Vertex* pPipeWork)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vPipeWork(pPipeWork);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_PipeWork::~SP_PipeWork(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_PipeWork::vPipeWork(Vertex* pPipeWork)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  vInit();
  
  vPipeWork(pPipeWork,0);
}

/*----------------------------------------------------------------------------*/
/* sprint pipeline                                                            */
/*----------------------------------------------------------------------------*/
void SP_PipeWork::vPipeWork(Vertex* pPipeWork, i8 iDepth)
{
  _precondition_(_MAGIC_,_FOOTER_);
    
  vCatf(iDepth,"PipeWork %s %p\n"_Stz
        ,pPipeWork->pPipeSpec()->szVertexNm()
        ,pPipeWork
        );

  i8 nSubWorks = pPipeWork->nItems();
  for (i8 i_SubWork = 0; i_SubWork < nSubWorks; i_SubWork++) {
    Vertex* pSubPipeWork = pPipeWork->pGet(i_SubWork);
    vPipeWork(pSubPipeWork,iDepth+1);
  }

  for (i8 i_Stage = 0; i_Stage < pPipeWork->nItems(); i_Stage++ ) {
    Stage* pStage = pPipeWork->pStaging()->pGet(i_Stage);
    SP_Stage spStage(pStage,true);
    vCatf("%s"_Stz,spStage.sz());
  }
}

/*----------------------------------------------------------------------------*/
/* return Stz* of result                                                      */
/*----------------------------------------------------------------------------*/
Stz* SP_PipeWork::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Vch and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_PipeWork::sz(Vertex* pPipeWork)
{
  _precondition_(_MAGIC_,_FOOTER_);

  vPipeWork(pPipeWork);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SP_PipeWork::aFooter(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
