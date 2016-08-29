/*============================================================================*/
/* Arguments.cpp - command line arguments                                     */
/*============================================================================*/

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Arguments::Arguments(void)
: InProcess(CN_ARGUMENTS)
{
  m_sRegionName = "/RapidsSE_DefaultRegion";
  m_iRegionSize = 1_GB * 2;
  m_iPartitions = 1;
  
  m_sCommand    = "";
  
  m_bCommand    = false;
  m_bDocol      = false;
  m_bHelp       = false;
  m_bList       = false;
  m_bSeitf      = false;
  m_bSePrompt   = false;
  m_bStart      = false;
  m_bStatus     = false;
  m_bStop       = false;

  m_bBgTasks    = false;
  m_iPartition  = 0;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Arguments::~Arguments(void)
{
}

/*----------------------------------------------------------------------------*/
/* parse command line arguments                                               */
/*----------------------------------------------------------------------------*/
bool Arguments::bParse(int argc, const char* argv[])
{
  for (i8 iArg = 1; iArg < argc; iArg++) {
    
    if (String::bEqualTo("--command",argv[iArg],C_NO_CASE)) {
      m_sCommand = argv[++iArg];
      m_bCommand = true;
      continue;
    }
    
    if (String::bEqualTo("--docol",argv[iArg],C_NO_CASE)) {
      m_bDocol = true;
      continue;
    }
    
    if (
        String::bEqualTo("--help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("-help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("help",argv[iArg],C_NO_CASE)
        || String::bEqualTo("--?",argv[iArg],C_NO_CASE)
        || String::bEqualTo("-?",argv[iArg],C_NO_CASE)
        || String::bEqualTo("?",argv[iArg],C_NO_CASE)
        )  {
      vPrintHelp();
      exit(EXIT_SUCCESS);
    }
    
    if (String::bEqualTo("--list",argv[iArg],C_NO_CASE)) {
      m_bList = false;
      continue;
    }
    
    if (String::bEqualTo("--partitions",argv[iArg],C_NO_CASE)) {
      m_iPartitions = String::iToI8(argv[++iArg]);
      ASSERT(m_iPartitions <= C_TABLE_PARTITIONS);
      continue;
    }
    
    if (String::bEqualTo("--seitf",argv[iArg],C_NO_CASE)) {
      m_bSeitf = true;
      continue;
    }
    
    if (String::bEqualTo("--seprompt",argv[iArg],C_NO_CASE)) {
      m_bSePrompt = true;
      continue;
    }
    
    if (String::bEqualTo("--size",argv[iArg],C_NO_CASE)) {
      m_iRegionSize = String::iToI8(argv[++iArg]);
      continue;
    }
    
    if (String::bEqualTo("--start",argv[iArg],C_NO_CASE)) {
      m_bStart = true;
      continue;
    }

    if (String::bEqualTo("--status",argv[iArg],C_NO_CASE)) {
      m_bStatus = true;
      continue;
    }
    
    if (String::bEqualTo("--stop",argv[iArg],C_NO_CASE)) {
      m_bStop = true;
      continue;
    }

    // options -----------------------------------------------------------------
    
    if (String::bEqualTo("--bgtasks",argv[iArg],C_NO_CASE)) {
      m_bBgTasks = true;
      continue;
    }
    
    // seitf options -----------------------------------------------------------
    if (String::bEqualTo("--partition",argv[iArg],C_NO_CASE)) {
      m_iPartition = String::iToI8(argv[++iArg]);
      continue;
    }
    
    vTrace("Unrecognized command argument: %s\n",argv[iArg]);
    return false;
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* print arguments                                                            */
/*----------------------------------------------------------------------------*/
void Arguments::vPrint(void)
{
  vTrace("Args:\n");
  
  if (m_bCommand) vTrace("  --command\n");
  if (m_bDocol)   vTrace("  --docol\n");
  if (m_bHelp)    vTrace("  --help\n");
  if (m_bList)    vTrace("  --list\n");
  if (m_bSeitf)   vTrace("  --seitf\n");
  if (m_bStart)   vTrace("  --start\n");
  if (m_bStatus)  vTrace("  --status\n");
  if (m_bStop)    vTrace("  --stop\n");

  vTrace("  --regionName %s\n",m_sRegionName.a());
  vTrace("  --regionSize %lld\n",m_iRegionSize);
  vTrace("  --partition %lld\n",m_iPartition);
  vTrace("  --partitions %lld\n",m_iPartitions);
}

/*----------------------------------------------------------------------------*/
/* print help                                                                 */
/*----------------------------------------------------------------------------*/
void Arguments::vPrintHelp(void)
{
  printf("\n");
  
  printf("Help:\n");
  printf("--command <cmd>   (run given command)\n");
  printf("--help            (print this list)\n");
  printf("--list            (print list of available systems)\n");
  printf("--start           (start system)\n");
  printf("--status          (print status of system)\n");
  printf("--stop            (stop system)\n");
  
  printf("\n");
  printf("--partitions <i>  (number of partitions)\n");
  printf("--partition <i>   (which partition)\n");
  printf("--region <name>   (name of shm region)\n");
  printf("--size <i>        (size in bytes of shm region)\n");

  printf("\n");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/