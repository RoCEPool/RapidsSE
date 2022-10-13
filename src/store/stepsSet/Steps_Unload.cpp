/*============================================================================*/
/* Steps_Unload.cpp - unload step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "CmdLineArgs.hpp"

extern CmdLineArgs gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Unload(void)
{
  vAddStep("unload.toFile"_Stz,      &Step::vUnloadToFile);
}

/*----------------------------------------------------------------------------*/
/* unload.toFile -- unload persistent heaps to file                           */
/*----------------------------------------------------------------------------*/
void Step::vUnloadToFile(void)
{
  ENTRY();

  s256 sFileNm;
  m_pDataStack->vPopStr(sFileNm, __func__);

  vTrace("Got filename %s\n"_Stz, sFileNm.szStz());

  sFileNm.vConcat("_"_Stz);

  sFileNm.vConcat(gxCmdLineArgs.m_xSysArgs.m_sNodeName);

  s256 sFilePath = "../tmp/"_Stz;

  sFilePath.vConcat(sFileNm);

  vTrace("Unloading to %s\n"_Stz, sFilePath.szStz());

  PersistenceHelper::vWritePersistentHeaps(sFilePath.szStz());

  vTrace("Successfully unloaded to %s\n"_Stz, sFilePath.szStz());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
