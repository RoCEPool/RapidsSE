/*============================================================================*/
/* Steps_Stencil_v2.cpp -- stencil step group                                 */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Op.hpp"
#include "Stream.hpp"
#include "Job.hpp"
#include "TagConstants.h"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Stencil_v2(void)
{
  vAddStep("Stencil_v2[]"_Stz,           &Step::vStencilGet_v2);
  vAddStep("Stencil_v2[].delete"_Stz,    &Step::vStencilDelete_v2);
  vAddStep("Stencil_v2[].new"_Stz,       &Step::vStencilNew_v2);
  vAddStep("Stencil_v2[].setup"_Stz,     &Step::vStencilSetup_v2);
  vAddStep("Stencil_v2[].sketch"_Stz,    &Step::vStencilSketch_v2);
  vAddStep("Stencil_v2[].source@"_Stz,   &Step::vStencilSourceFetch_v2);
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].delete -- delete stencil                                      */
/*----------------------------------------------------------------------------*/
void Step::vStencilDelete_v2(void)
{
  ENTRY();

  //vTrace("vStencilDelete\n"_Stz);

//  delete * m_pLogicUnit->m_xpStencil_v2;
//
//  m_pLogicUnit->m_xpStencil_v2 = nullptr;

  m_pLogicUnit->m_xpStencil_v2.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].get -- get stencil                                            */
/*----------------------------------------------------------------------------*/
void Step::vStencilGet_v2(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpStencil_v2);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].new -- new stencil                                            */
/*----------------------------------------------------------------------------*/
void Step::vStencilNew_v2(void)
{
  ENTRY();

  //vTrace("vStencilNew\n"_Stz);

  m_pLogicUnit->m_xpStencil_v2 = new Stencil_v2;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].setup -- use stencil to setup                                 */
/*----------------------------------------------------------------------------*/
void Step::vStencilSetup_v2(void)
{
  ENTRY();

  Stencil_v2* pStencil   = * m_pLogicUnit->m_xpStencil_v2;

  Dynamic* pDynamic = _pDynamic(m_pDataStack->pPopPtr(__func__));

  // get the current stream
  Stream* pStream = _pStream(m_pOp->pStream());

  pDynamic->aDynamicT()->m_pTub        = *m_pLogicUnit->m_xpTub;
  pDynamic->aDynamicT()->m_pPot        = *m_pLogicUnit->m_xpPot;

  pStencil->vSetup(pDynamic, pStream);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].sketch -- use stencil to sketch                               */
/*----------------------------------------------------------------------------*/
void Step::vStencilSketch_v2(void)
{
  ENTRY();
/*
  Stencil_v2* pStencil   = * m_pLogicUnit->m_xpStencil_v2;

  Dynamic* pDynamic = _pDynamic(m_pDataStack->pPopPtr(__func__));

  // get the current stream
  Stream* pStream = _pStream(m_pOp->pStream());

  pDynamic->aDynamicT()->m_pTub        = *m_pLogicUnit->m_xpTub;
  pDynamic->aDynamicT()->m_pPot        = *m_pLogicUnit->m_xpPot;

  eEC eError = pStencil->eSketch(pDynamic, pStream);

  if (eError == EC_NONE) {

    Job* pJob = pOuterJobOfTask();
    TagSet* pTagSet = pJob->pJobTags();

    //pTagSet->vContentDump();

    bool bGenerateCCode = false;

    if (pTagSet->bHasName(C_TAG_CCODE))
      bGenerateCCode = true;

    pStencil->vCombine(true, bGenerateCCode);
    pStencil->vPrint();
  }

  m_pDataStack->vPushInt(eError);
*/
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil_v2[].source@ -- get sketched source                                */
/*----------------------------------------------------------------------------*/
void Step::vStencilSourceFetch_v2(void)
{
  ENTRY();

  Stencil_v2* pStencil = * m_pLogicUnit->m_xpStencil_v2;
  
  Stz* szSource = pStencil->szOpSource();

  m_pDataStack->vPushStz(szSource);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
