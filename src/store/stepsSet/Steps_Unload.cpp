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

  s256 sDirNm;
  m_pDataStack->vPopStr(sDirNm, __func__);

  s256 sDirPath = "../tmp/";

  sDirPath.vConcat(sDirNm);

  if( ! Dir::bExists(sDirPath.szStz()))
    Dir::bMake(sDirPath.szStz());

  s256 sFilePath = ""_Stz;

  if(MY::pOp()) {
    sFilePath.vCatSprint("%s/saveFile_%s_%lld"_Stz,
                         sDirPath.szStz(),
                         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                         MY::pOp()->pStream()->i_PtnOfNode());
  } else {
    sFilePath.vCatSprint("%s/saveFile_%s"_Stz,
                         sDirPath.szStz(),
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
