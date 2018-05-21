/*============================================================================*/
/* OpSys.cpp -- operating system utility                                      */
/*============================================================================*/

#include "System.h"

static struct utsname gxUtsName;

/*----------------------------------------------------------------------------*/
/* get information about o/s and machine                                      */
/*----------------------------------------------------------------------------*/
void OpSys::vGetInfo(void)
{
  int iResult = uname(&gxUtsName);
  
  if (0 == iResult)
    return;
 
  vWarn("OpSys::vGetInfo: uname() error; errno=%d"_Stz,errno);  
}

/*----------------------------------------------------------------------------*/
/* trace utsname information                                                  */
/*----------------------------------------------------------------------------*/
void OpSys::vTraceInfo(void)
{
  vTrace("OpSys::vGetInfo:\n"_Stz);
  vTrace("  sysname : %s\n"_Stz,aSysName());
  vTrace("  nodename: %s\n"_Stz,aNodeName());
  vTrace("  release : %s\n"_Stz,aRelease());
  vTrace("  version : %s\n"_Stz,aVersion());
  vTrace("  machine : %s\n"_Stz,aMachine());
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* get utsnamme.machine (Hardware type)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aMachine(void)
{
  return gxUtsName.machine;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.nodename (Name of this network node)                          */
/*----------------------------------------------------------------------------*/
char* OpSys::aNodeName(void)
{
  return gxUtsName.nodename;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.release (Release level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aRelease(void)
{
  return gxUtsName.release;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.sysname (Name of OS)                                          */
/*----------------------------------------------------------------------------*/
char* OpSys::aSysName(void)
{
  return gxUtsName.sysname;
}

/*----------------------------------------------------------------------------*/
/* get utsnamme.version (Version level)                                       */
/*----------------------------------------------------------------------------*/
char* OpSys::aVersion(void)
{
  return gxUtsName.version;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
