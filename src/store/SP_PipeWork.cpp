/*============================================================================*/
/* SP_PipeWork.cpp - PipeWork sprint helper                                   */
/*============================================================================*/

#include "BASE.h"

#include "Pipeline.hpp"
#include "PipeWork.hpp"
#include "Grid.hpp"

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
SP_PipeWork::SP_PipeWork(PipeWork* pPipeWork, bool bDescend)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  vPipeWork(pPipeWork,bDescend);
  
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
void SP_PipeWork::vPipeWork(PipeWork* pPipeWork, bool bDescend)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  if (bDescend) {
    
  }
  
  for (i8 i_Stage = 0; i_Stage < pPipeWork->nItems(); i_Stage++ ) {
    Stage* pStage = pPipeWork->pGet(i_Stage);
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
Stz* SP_PipeWork::sz(PipeWork* pPipeWork)
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
