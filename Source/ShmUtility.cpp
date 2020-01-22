/*============================================================================*/
/* ShmUtility.cpp - share memory utility                                      */
/*============================================================================*/

#include "Utilities.h"

bool  mbCreate = false;
bool  mbAttach = false;
bool  mbDelete = false;

char* gaSeShmRegionAddress = nullptr;
i8    miRegionSize = C_1GB;
s32   msRegionName;

extern Header* gaSeShmRegion;

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
      gaSeShmRegionAddress = (char*)argv[++i_Arg]->iToI8();
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
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Unrecognised SHM command\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* attach to existing shm region                                              */
/*----------------------------------------------------------------------------*/
void ShmUtility::vAttach(void)
{
  int iFD;
  SysCall oSysCall;
 
  // open existing shm region
  bool bResult = ShmLib::bOpenExisting(iFD,msRegionName,oSysCall);

  if ( ! bResult ) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmUtility::vAttach: failed to open region: %s\n"_Stz,
           msRegionName.azChar());
    NEVER_REACH;
  }
  
  // allocate an extent of memory
  gaSeShmRegion = (Header*)ShmLib::aMap(iFD,
                                     gaSeShmRegionAddress,
                                     miRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);

  if (! gaSeShmRegion)  {
    ShmLib::bClose(iFD,oSysCall);
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmUtility::vAttach failed to map\n"_Stz);
  }

  //memcpy(gaSeShmRegion,"ABCDEFGHIJKLMNOPQRSTUVWXYZ\0", 27);
  
  //vTrace("%p %s\n"_Stz, gaSeShmRegion, gaSeShmRegion);
  
  vTrace("Region %s Attached\n"_Stz,msRegionName.azChar());
}

/*----------------------------------------------------------------------------*/
/* create a new shm region                                                    */
/*----------------------------------------------------------------------------*/
void ShmUtility::vCreate(void)
{
  int iFD;
  SysCall oSysCall;
  bool bRestart = false;
  bool bResult;
  
  // open a new shm region which will contain the activity map
  bResult = ShmLib::bOpenCreate(iFD,msRegionName,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmUtility::vCreate: failed to create region %s\n"_Stz,
           msRegionName.azChar());
  
  // extend the shm region
  bResult = ShmLib::bExtend(iFD,miRegionSize,oSysCall);
  if (! bResult)
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "ShmUtility::vCreate: failed to extend region %s\n"_Stz,
           msRegionName.azChar());

  
  // allocate memory for the shm region
  gaSeShmRegion = (Header*)ShmLib::aMap(iFD,
                                     gaSeShmRegionAddress,
                                     miRegionSize,
                                     C_NO_HUGE_PAGES,
                                     oSysCall);
  gaSeShmRegion->vMagic();
  gaSeShmRegion->vClass(CN_NONE);

  vTrace("Region %s Created\n"_Stz,msRegionName.azChar());
}

/*----------------------------------------------------------------------------*/
/* delete an existing shm region                                              */
/*----------------------------------------------------------------------------*/
void ShmUtility::vDelete(void)
{
  SysCall oSysCall;
  
  if (! ShmLib::bUnlink(msRegionName,oSysCall)) {
    vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
           "Region %s not unlinked\n"_Stz,msRegionName.azChar());
  }
  
  vTrace("Region %s unlinked\n"_Stz,msRegionName.azChar());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
