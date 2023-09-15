/*============================================================================*/
/* Steps_Stencil.cpp -- stencil step group                                 */
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
void Step::vAddSteps_Stencil(void)
{
  vAddStep("Stencil[]"_Stz,           &Step::vStencilGet);
  vAddStep("Stencil[].delete"_Stz,    &Step::vStencilDelete);
  vAddStep("Stencil[].new"_Stz,       &Step::vStencilNew);
  vAddStep("Stencil[].setup"_Stz,     &Step::vStencilSetup);
  vAddStep("Stencil[].sketch"_Stz,    &Step::vStencilSketch);
  vAddStep("Stencil[].source@"_Stz,   &Step::vStencilSourceFetch);
}

/*----------------------------------------------------------------------------*/
/* Stencil[].delete -- delete stencil                                         */
/*----------------------------------------------------------------------------*/
void Step::vStencilDelete(void)
{
  ENTRY();

  //vTrace("vStencilDelete\n"_Stz);

//  delete * m_pLogicUnit->m_xpStencil;
//
//  m_pLogicUnit->m_xpStencil = nullptr;

  m_pLogicUnit->m_xpStencil.vDeleteComponent();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].get -- get stencil                                               */
/*----------------------------------------------------------------------------*/
void Step::vStencilGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(* m_pLogicUnit->m_xpStencil);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].new -- new stencil                                               */
/*----------------------------------------------------------------------------*/
void Step::vStencilNew(void)
{
  ENTRY();

  //vTrace("vStencilNew\n"_Stz);

  m_pLogicUnit->m_xpStencil = new Stencil;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].setup -- use stencil to setup                                    */
/*----------------------------------------------------------------------------*/
void Step::vStencilSetup(void)
{
  ENTRY();

  Dynamic* pDynamic = _pDynamic(m_pDataStack->pPopPtr(__func__));

  // get the current stream

  pDynamic->aDynamicT()->m_pTub        = *m_pLogicUnit->m_xpTub;
  pDynamic->aDynamicT()->m_pPot        = *m_pLogicUnit->m_xpPot;

  //pStencil->vSetup(pDynamic, pStream);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Stencil[].sketch -- use stencil to sketch                                  */
/*----------------------------------------------------------------------------*/
void Step::vStencilSketch(void)
{
  ENTRY();
/*
  Stencil* pStencil   = * m_pLogicUnit->m_xpStencil;

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
/* Stencil[].source@ -- get sketched source                                   */
/*----------------------------------------------------------------------------*/
void Step::vStencilSourceFetch(void)
{
  ENTRY();

  Stencil* pStencil = * m_pLogicUnit->m_xpStencil;
  
  Stz* szSource = pStencil->szOpSource();

  m_pDataStack->vPushStz(szSource);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
