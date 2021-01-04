/*============================================================================*/
/* Steps_Project.cpp -- Project step group                                    */
/*============================================================================*/

#include "BASE.h"
#include "STORE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "StepTiming.hpp"

#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Project(void)
{
  vAddStep("project[]"_Stz,                 &Step::vProjectGet);
  vAddStep("project[].new"_Stz,             &Step::vProjectNew);
  vAddStep("project[].projectColumns"_Stz,  &Step::vProjectColumns);
  vAddStep("project[].set"_Stz,             &Step::vProjectSet);
}

/*----------------------------------------------------------------------------*/
/* project[]] -- return project[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vProjectGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*( m_pJob->pLogicUnit_job() )->m_xpProject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* project::new -- new a project                                              */
/*----------------------------------------------------------------------------*/
void Step::vProjectNew(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpProject = new Project;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* project[].projectColumns -- project columns                                */
/*----------------------------------------------------------------------------*/
void Step::vProjectColumns(void)
{
  ENTRY();

  Project* pProject = *( m_pJob->pLogicUnit_job() )->m_xpProject;

  StreamDef* pStreamDefOut = _pStreamDef(m_pDataStack->pPopPtr(__func__));
  StreamDef* pStreamDefIn = _pStreamDef(m_pDataStack->pPopPtr(__func__));

  s4K sProjectColumns;
  m_pDataStack->vPopStr(sProjectColumns, __func__);

  eEC eError = pProject->eProjectColumns(pStreamDefIn, pStreamDefOut, sProjectColumns);
  m_pDataStack->vPushInt(eError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* project[].set -- set project[]                                             */
/*----------------------------------------------------------------------------*/
void Step::vProjectSet(void)
{
  ENTRY();

  ( m_pJob->pLogicUnit_job() )->m_xpProject = _pProject(m_pDataStack->pPopPtr(__func__));

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
