/*============================================================================*/
/* ShmUtility.cpp - share memory utility                                      */
/*============================================================================*/

#include "StorageEngine.h"
#include "ShmUtility.hpp"

bool  mbCreate = false;
bool  mbAttach = false;
bool  mbDelete = false;

char* maRegionAddress = nullptr;
i8    miRegionSize = C_1GB;
s32   msRegionName;

extern void* maRegion;

/*----------------------------------------------------------------------------*/
/* run shm utility                                                            */
/*----------------------------------------------------------------------------*/
void ShmUtility::vRun(int argc, Stz* argv[])
{
  vInfo("SHM...\n"_Stz);
  
  msRegionName = ""_Stz;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // parse arguments
  
  for (i8 i_Arg = 1; i_Arg < argc; i_Arg++) {
    
    if (argv[i_Arg]->bSame("--Address"_Stz)) {
      maRegionAddress = (char*)argv[++i_Arg]->iToI8();
      continue;
    }
    
    if (argv[i_Arg]->bSame("--Attach"_Stz)) {
      mbAttach = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--Create"_Stz)) {
      mbCreate = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--Delete"_Stz)) {
      mbDelete = true;
      continue;
    }
    
    if (argv[i_Arg]->bSame("--Name"_Stz)) {
      msRegionName = (Stz*)argv[++i_Arg];
      continue;
    }
    
    if (argv[i_Arg]->bSame("--Size"_Stz)) {
      miRegionSize = argv[++i_Arg]->iToI8();
      continue;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // determine what to do
  
  if (mbAttach) {
    vAttach();
  }
  else if (mbCreate) {
    vCreate();
  }
  else if (mbDelete) {
    vDelete();
  }
  else
    vFatal("Unrecognised SHM command\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region                                              */
/*----------------------------------------------------------------------------*/
void ShmUtility::vAttach(void)
{
  int iFD;
  SysCall oSysCall;
 
  // open existing shm region
  bool bResult = Shm::bAttach(msRegionName,
                              iFD,
                              oSysCall);

  if ( ! bResult ) {
    vFatal("ShmUtility::vAttach: failed to open region: %s\n"_Stz,
           msRegionName.aChar());
    NEVER_REACH;
  }
  
  // allocate an extent of memory
  maRegion = Shm::aMap(iFD,
                       maRegionAddress,
                       miRegionSize,
                       C_SHARED_REGION,
                       false,
                       oSysCall);

  if ( ! maRegion )  {
    Shm::bClose(iFD,oSysCall);
    vFatal("ShmUtility::vAttach failed to map\n"_Stz);
  }

  memcpy(maRegion,"ABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 27);
  
  vTrace("%p %s\n"_Stz, maRegion, maRegion);
  
  vTrace("Region %s Attached\n"_Stz,msRegionName.aChar());
}

/*----------------------------------------------------------------------------*/
/* create a new shm region                                                    */
/*----------------------------------------------------------------------------*/
void ShmUtility::vCreate(void)
{
  int iFD;
  SysCall oSysCall;
  bool bRestart = false;
  
  // open a new shm region
  bool bResult = Shm::bCreate(msRegionName,
                              miRegionSize,
                              iFD,
                              bRestart,
                              oSysCall);
  
  if ( ! bResult ) {
    vFatal("ShmUtility::vCreate: failed to create region %s\n"_Stz,
           msRegionName.aChar());
    NEVER_REACH;
  }
  
  // allocate memory for the shm region
  maRegion = Shm::aMap(iFD,
                       maRegionAddress,
                       miRegionSize,
                       true,
                       false,
                       oSysCall);

  vTrace("Region %s Created\n"_Stz,msRegionName.aChar());
}

/*----------------------------------------------------------------------------*/
/* delete an existing shm region                                              */
/*----------------------------------------------------------------------------*/
void ShmUtility::vDelete(void)
{
  SysCall oSysCall;
  
  if (! Shm::bUnlink(msRegionName,oSysCall)) {
    vFatal("Region %s not unlinked\n"_Stz,msRegionName.aChar());
  }
  
  vTrace("Region %s unlinked\n"_Stz,msRegionName.aChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
