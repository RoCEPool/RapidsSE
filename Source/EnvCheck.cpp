/*============================================================================*/
/* EnvCheck.cpp -- Environment Check                                          */
/*============================================================================*/

#include "StorageEngine.h"

extern Args gxArgs;

extern char giProgressPoint;

/*----------------------------------------------------------------------------*/
/* check mainfest list                                                        */
/*----------------------------------------------------------------------------*/
void EnvCheck::vCheckManifest(void)
{
  if (AM::bVerboseProgress())
    vInfo("\n*\n* %lld) Checking Resource Manifest\n*\n"_Stz,giProgressPoint++);
  
  vCheckDir("."_Stz,            C_EXIST_EXISTS,C_ACCESS_READ);
  vCheckDir("../Commands"_Stz,  C_EXIST_EXISTS,C_ACCESS_READ);
  vCheckDir("../tmp"_Stz,       C_EXIST_EXISTS,C_ACCESS_READWRITE);

  vCheckFile("_bootCluster"_Stz,C_EXIST_EXISTS,C_ACCESS_READ);
}

/*----------------------------------------------------------------------------*/
/* check directory                                                            */
/*----------------------------------------------------------------------------*/
void EnvCheck::vCheckDir(Stz* szDirPath,  i8 iExist, i8 iPermissions)
{
  if (AM::bVerboseProgress())
    vInfo("Checking dir: %s\n"_Stz,szDirPath);
  
  if (iExist == C_EXIST_EXISTS) {
    if (! Dir::bExists(szDirPath))
      Process::vUnrecoverableError("Mandatory dir missing"_Stz, szDirPath);
  }
  
  if (iExist == C_EXIST_NOTEXISTS) {
    if (Dir::bExists(szDirPath))
      Process::vUnrecoverableError("Non-existant dir exists"_Stz, szDirPath);
  }
  
  if (C_ACCESS_NOCHECK != iPermissions) {
    if (! Dir::bAccess(szDirPath,iPermissions))
      Process::vUnrecoverableError("Insufficient dir permissions"_Stz, szDirPath);
  }
}

/*----------------------------------------------------------------------------*/
/* check file                                                                 */
/*----------------------------------------------------------------------------*/
void EnvCheck::vCheckFile(Stz* szFilePath, i8 iExist, i8 iPermissions)
{
  if (AM::bVerboseProgress())
    vInfo("Checking file: %s\n"_Stz,szFilePath);
  
  if (iExist == C_EXIST_EXISTS) {
    if (! File::bExists(szFilePath))
      Process::vUnrecoverableError("Mandatory file missing"_Stz, szFilePath);
  }

  if (iExist == C_EXIST_NOTEXISTS) {
    if (File::bExists(szFilePath))
      Process::vUnrecoverableError("Non-existant file exists"_Stz, szFilePath);
  }

  if (C_ACCESS_NOCHECK != iPermissions) {
    if (! File::bAccess(szFilePath,iPermissions))
      Process::vUnrecoverableError("Insufficient file permissions"_Stz, szFilePath);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
