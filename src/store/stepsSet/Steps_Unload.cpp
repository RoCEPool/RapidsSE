/*============================================================================*/
/* Steps_Unload.cpp - unload step group                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "Op.hpp"
#include "Stream.hpp"
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

  s256 sFilePath = ""_Stz;

  if(MY::pOp()) {
    sFilePath.vCatSprint("../tmp/%s_%s_%lld"_Stz,
                         sFileNm.szStz(),
                         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                         MY::pOp()->pStream()->i_PtnOfNode());
  } else {
    sFilePath.vCatSprint("../tmp/%s_%s"_Stz,
                         sFileNm.szStz(),
                         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz());
  }

  vTrace("Unloading to %s\n"_Stz, sFilePath.szStz());

  PersistenceHelper::vWritePersistentHeaps(sFilePath.szStz());

  vTrace("Successfully unloaded to %s\n"_Stz, sFilePath.szStz());

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
