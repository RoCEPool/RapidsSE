/*============================================================================*/
/* Steps_System.cpp - system step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_System(void)
{
  vAddStep("[system].activeJobs"_Stz, &Step::vSystemActiveJobs);
  vAddStep("[system].bpa"_Stz,        &Step::vSystemBpa);
  vAddStep("[system].open@"_Stz,      &Step::vSystemOpenFetch);
  vAddStep("[system].open!"_Stz,      &Step::vSystemOpenStore);
  vAddStep("[system].see"_Stz,        &Step::vSystemSee);
  vAddStep("[system].started@"_Stz,   &Step::vSystemStartedFetch);
  vAddStep("[system].started!"_Stz,   &Step::vSystemStartedStore);
}

/*----------------------------------------------------------------------------*/
/* [system].activeJobs -- get number of active jobs                           */
/*----------------------------------------------------------------------------*/
void Step::vSystemActiveJobs(void)
{
  ENTRY();
    
  i8 nActiveJobs = AM::SYSTEM::nActiveJobs();
  
  m_pDataStack->vPushInt(nActiveJobs);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].bpa -- buffer pool analysis                                       */
/*----------------------------------------------------------------------------*/
void Step::vSystemBpa(void)
{
  ENTRY();
  
  PoolHelper::vPoolAnalyze();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].open@ -- fetch system open indicator                              */
/*----------------------------------------------------------------------------*/
void Step::vSystemOpenFetch(void)
{
  ENTRY();
  
  b8 bSystemOpen = AM::SYSTEM::bSystemOpen();
  
  m_pDataStack->vPushInt(bSystemOpen);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].open! -- store system open indicator                              */
/*----------------------------------------------------------------------------*/
void Step::vSystemOpenStore(void)
{
  ENTRY();
  
  b8 bSystemOpen = m_pDataStack->iPopInt(__func__);
  
  AM::SYSTEM::vSystemOpen(bSystemOpen);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].see -- see system                                                 */
/*----------------------------------------------------------------------------*/
void Step::vSystemSee(void)
{
  ENTRY();
  
  vTrace("See System::\n"_Stz);

  vTrace("  name %s\n"_Stz
        ,AM::NODE::szNodeNm()
         );

  vTrace("  pid %lld (%s)\n"_Stz
         ,AM::PROCESS::iSePid()
         ,Process::bTestPid(AM::PROCESS::iSePid()) ? "exists"_Stz : "non-existant"_Stz
         );

  vTrace("  system %s\n"_Stz
         ,AM::SYSTEM::bSystemOpen() ? "open" : "closed"
         );

  vTrace(";;\n\n"_Stz);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].started@ -- get grid started                                      */
/*----------------------------------------------------------------------------*/
void Step::vSystemStartedFetch(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(AM::SYSTEM::bStarted());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [system].started! -- set grid started                                      */
/*----------------------------------------------------------------------------*/
void Step::vSystemStartedStore(void)
{
  ENTRY();
  
  i8 iStarted = m_pDataStack->iPopBol(__func__);
  
  AM::SYSTEM::vStarted(iStarted == 1 ? true : false);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
