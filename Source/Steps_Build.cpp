/*============================================================================*/
/* Steps_Build.cpp - build step group                                         */
/*============================================================================*/

#include "Steps.h"

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Build(void)
{
  vAddStep("build::branch"_Stz,     &Step::vBuildBranch);
  vAddStep("build::date"_Stz,       &Step::vBuildDate);
  vAddStep("build::gitHash"_Stz,    &Step::vBuildGitHash);
  vAddStep("build::hostName"_Stz,   &Step::vBuildHostName);
  vAddStep("build::userName"_Stz,   &Step::vBuildUserName);
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vBuildBranch(void)
{
  ENTRY();
  
  s32 sBuildItem(gsBuildBranch);
  
  m_pDataStack->vPushStr(sBuildItem);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vBuildDate(void)
{
  ENTRY();
  
  s32 sBuildItem(gsBuildDate);
  
  m_pDataStack->vPushStr(sBuildItem);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vBuildGitHash(void)
{
  ENTRY();
  
  s32 sBuildItem(gsBuildGitHash);
  
  m_pDataStack->vPushStr(sBuildItem);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vBuildHostName(void)
{
  ENTRY();
  
  s32 sBuildItem(gsBuildHostName);
  
  m_pDataStack->vPushStr(sBuildItem);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* user::name -- get user name                                                */
/*----------------------------------------------------------------------------*/
void Step::vBuildUserName(void)
{
  ENTRY();
  
  s32 sBuildItem(gsBuildUserName);
  
  m_pDataStack->vPushStr(sBuildItem);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
