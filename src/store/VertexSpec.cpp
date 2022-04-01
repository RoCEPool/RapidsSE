/*============================================================================*/
/* PipeSpec.cpp -- Pipe Specification                                         */
/*============================================================================*/

#include "BASE.h"

#include "OpSpec.hpp"
#include "StageSpec.hpp"
#include "VertexSpec.hpp"
#include "DeckSpec.hpp"
#include "Dynamo.hpp"
#include "Procedure.hpp"
#include "Job.hpp"
#include "Grid.hpp"
#include "TagConstants.h"

#include "SP_PipeSpec.hpp"
#include "SP_Token.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, VertexSpec, CN_VERTEXSPEC)
__aNewFunction(VertexSpec, CN_VERTEXSPEC)
__vDeleteFunction(VertexSpec, CN_VERTEXSPEC)
__aFooterFunction(VertexSpec)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
VertexSpec::VertexSpec(VertexSpec* pParentVertexSpec)
: ListNL_t<VertexSpec>(CN_VERTEXSPEC)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  m_pParentVertexSpec = pParentVertexSpec;
  m_sVertexNm = "(unknown)"_Stz;
  
  m_pDeckSpec = new DeckSpec(this);

  _checkInitialised_(CN_VERTEXSPEC);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
VertexSpec::~VertexSpec(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  while (VertexSpec *pChildVertexSpec = pPop())
    delete pChildVertexSpec;
  
  delete m_pDeckSpec;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add stage spec                                                             */
/*----------------------------------------------------------------------------*/
void VertexSpec::vAddStageSpec(StageSpec *pStageSpec)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  m_pDeckSpec->vAppend(pStageSpec);
}

/*----------------------------------------------------------------------------*/
/* get sub-pipelines                                                          */
/*----------------------------------------------------------------------------*/
DeckSpec* VertexSpec::pDeckSpec(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  return m_pDeckSpec;
}

/*----------------------------------------------------------------------------*/
/* find stage matching given name                                             */
/*----------------------------------------------------------------------------*/
StageSpec* VertexSpec::pFindStageSpec(Stz *szFindNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  i8 nStageSpecs = m_pDeckSpec->nItems();
  for (i8 i_Stage = 0; i_Stage < nStageSpecs; i_Stage++) {
    StageSpec *pStageSpec = m_pDeckSpec->pGet(i_Stage);

    if (pStageSpec->szStageNm()->bSame(szFindNm)) {
      RASSERT(pStageSpec->i_StageSpec() == i_Stage);
      return pStageSpec;
    }
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get vertex name                                                            */
/*----------------------------------------------------------------------------*/
Stz* VertexSpec::szVertexNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  return m_sVertexNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* number of stage specs                                                      */
/*----------------------------------------------------------------------------*/
i8 VertexSpec::nStageSpecs(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  i8 nStageSpecs = m_pDeckSpec->nItems();
  
  return nStageSpecs;
}

/*----------------------------------------------------------------------------*/
/* get stage spec                                                             */
/*----------------------------------------------------------------------------*/
StageSpec* VertexSpec::pStageSpec(i8 i_Stage)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_VERTEXSPEC, _FOOTER_);

  StageSpec *pStageSpec = m_pDeckSpec->pGet(i_Stage);

  return pStageSpec;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
