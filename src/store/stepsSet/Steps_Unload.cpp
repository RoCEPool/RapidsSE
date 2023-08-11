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

  i8 nDiskpaths = gxCmdLineArgs.m_xSysArgs.m_nDiskPaths;
  i8 iShmSize = gxCmdLineArgs.m_xSysArgs.m_iRegionSize;

  if(nDiskpaths <= 0)
  {
    s256 eErrMsg( (Stz*) "SE was not started with the '--disk <path-to-disk-root>' option. Cannot unload.\n" );
    vTrace("%s"_Stz, eErrMsg.szStz());

    s256 sDir;
    m_pDataStack->vPopStr(sDir, __func__);

    m_pDataStack->vPushStr(eErrMsg);
    m_pDataStack->vPushInt(EC_INVALID_COMMAND);
    m_pDataStack->vPushBol(false);
    return;
  }

  i8 iPtnOfNode = -1;
  if(MY::pOp()) {
    iPtnOfNode = MY::pOp()->pStream()->i_PtnOfNode();
  }

  s256 sDirPath = ""_Stz;
  Stz* sRootDir = gxCmdLineArgs.m_xSysArgs.m_sDiskRootDir[iPtnOfNode % nDiskpaths];
  ASSERT( ! sRootDir->bIsEmpty());

  s256 sDir;
  m_pDataStack->vPopStr(sDir, __func__);

  sDirPath.vCatSprint("%s%s/"_Stz,
                      sRootDir,
                      sDir.szStz());

  if( ! Dir::bExists(sDirPath.szStz()))
    Dir::bMake(sDirPath.szStz());

  // Check disk space before unload

  i8 iFreeSpace = Dir::iFreeSpace(sDirPath.szStz());

  if(iFreeSpace < 0) {
    s256 eErrMsg = ""_Stz;

    eErrMsg.vCatSprint("Error getting free space of dir %s. Cannot unload.\n"_Stz,
                       sDirPath.szStz());
    vTrace("%s"_Stz, eErrMsg.szStz());

    m_pDataStack->vPushStr(eErrMsg);
    m_pDataStack->vPushInt(EC_DISK_SPACE_CHECK_FAILED);
    m_pDataStack->vPushBol(false);
    return;
  }

  i8 iMinSpace = iShmSize / nDiskpaths;
  if(iFreeSpace < iMinSpace)
  {
    s256 eErrMsg = ""_Stz;

    eErrMsg.vCatSprint("Dir %s does not contain enough space for unload.\n"
                       "Dir has %.2f GB\n"
                       "Minimum space required %.2f GB\n"
                       "Cannot unload\n"_Stz,
                       sDirPath.szStz(),
                       (d8)iFreeSpace/(C_1GB),
                       (d8)iMinSpace/(C_1GB));
    vTrace("%s"_Stz, eErrMsg.szStz());

    m_pDataStack->vPushStr(eErrMsg);
    m_pDataStack->vPushInt(EC_LOW_DISK_SPACE);
    m_pDataStack->vPushBol(false);
    return;
  }

  s256 sFilePath = ""_Stz;

  sFilePath.vCatSprint("%ssaveFile_%s_%lld"_Stz,
                       sDirPath.szStz(),
                       gxCmdLineArgs.m_xSysArgs.m_sNodeName.szStz(),
                       iPtnOfNode);

  PersistenceHelper::vWritePersistentHeaps(sFilePath.szStz());
//    PersistenceHelper::vWriteDiskTest(sFilePath.szStz(), 4);

  vTrace("Successfully unloaded to %s\n"_Stz, sFilePath.szStz());

  m_pDataStack->vPushBol(true);

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

  s256 sDirPath = "../tmp/"_Stz;

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
