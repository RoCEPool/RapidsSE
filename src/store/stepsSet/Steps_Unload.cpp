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
  vAddStep("unload.toDisk"_Stz,      &Step::vUnloadToDisk);
  vAddStep("unload.toFile"_Stz,      &Step::vUnloadToFile);
}

/*----------------------------------------------------------------------------*/
/* unload.toDisk -- unload persistent heaps to disk                           */
/*----------------------------------------------------------------------------*/
void Step::vUnloadToDisk(void)
{
  ENTRY();

  s256 sDiskPath;
  i8 iPathOf, iPathNo;

  m_pDataStack->vPopStr(sDiskPath, __func__);
  iPathNo = m_pDataStack->iPopInt(__func__ );
  iPathOf = m_pDataStack->iPopInt(__func__ );

  i8 iPtnOfNode = -1;
  if(MY::pOp()) {
    iPtnOfNode = MY::pOp()->pStream()->i_PtnOfNode();
  }

  if(iPtnOfNode == iPathNo) {
    if(iPathOf != gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode) {
      vTrace("Please give %lld directory paths to unload to, %lld given\n"_Stz,
             gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode,
             iPathOf);
      EXIT();
      return;
    }

    if( ! Dir::bExists(sDiskPath.szStz()))
      Dir::bMake(sDiskPath.szStz());

    s256 sFilePath = ""_Stz;

    sFilePath.vCatSprint("%ssaveFile_%s_%lld"_Stz,
                         sDiskPath.szStz(),
                         gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                         iPtnOfNode);

    PersistenceHelper::vWritePersistentHeaps(sFilePath.szStz());
//    PersistenceHelper::vWriteDiskTest(sFilePath.szStz(), 16);

    vTrace("Successfully unloaded to %s\n"_Stz, sFilePath.szStz());
  }

  EXIT();
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
