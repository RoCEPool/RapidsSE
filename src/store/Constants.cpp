/*============================================================================*/
/* Constants.cpp -- add constants into main dictionary at startup             */
/*============================================================================*/

#include "BASE.h"

#include "Constants.hpp"

#include "RowSingle.hpp"
#include "RowBuffer.hpp"

#include "InfoSet.hpp"
#include "RowCoder.hpp"
#include "HeadLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "Crv.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Session.hpp"
#include "Job.hpp"

#include "EnumGroups.h"
#include "eAT.h"
#include "eDL.h"
#include "ePD.h"
#include "ePP.h"
#include "ePY.h"
#include "eRG.h"
#include "eUF.h"
#include "eUK.h"

#include "StepAccum.hpp"
#include "Steps.hpp"

#include "CmdLineArgs.hpp"
#include "TagConstants.h"

extern CmdLineArgs  gxCmdLineArgs;

extern Stz* gsBuildGitHash;
extern Stz* gsBuildDate;
extern Stz* gsBuildHostName;
extern Stz* gsBuildUserName;
extern Stz* gsBuildBranch;

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstant(Stz* szName, i8 iVal)
{
  Word* pNewWord = new(AM::DIC::pRoot()) Word(szName);
  
  pNewWord->vCategory(AM::pConstantRoot());
  
  CellSingle xVar;
  xVar = iVal;

  pNewWord->vEncodePush(xVar);
  pNewWord->vEncodeCall("exit_rt"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstant(Stz* szName, Stz* szVal)
{
  Word* pNewWord = new(AM::DIC::pRoot()) Word(szName);
  
  CellSingle xVar;
  xVar = szVal;
  
  pNewWord->vEncodePush(xVar);
  pNewWord->vEncodeCall("exit_rt"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add arguments as forth constants                                           */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstantsArgs(void)
{
  // CmdLineArgs
  Constants::vAddConstant("args.addNode"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bAddNode);
  Constants::vAddConstant("args.m_nDefinedNodes"_Stz,         gxCmdLineArgs.m_xSysArgs.m_nDefinedNodes);
  Constants::vAddConstant("args.condCode"_Stz,          gxCmdLineArgs.m_xSysArgs.m_iCondCode);
  Constants::vAddConstant("args.config"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bConfig);
  Constants::vAddConstant("args.developer"_Stz,         gxCmdLineArgs.m_xSysArgs.m_bDeveloper);
  Constants::vAddConstant("args.dontFree"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bDontFree);
  Constants::vAddConstant("args.freeCB"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bFreeSpecific);
  Constants::vAddConstant("args.help"_Stz,              gxCmdLineArgs.m_xSysArgs.m_bHelp);
  Constants::vAddConstant("args.hugePages"_Stz,         gxCmdLineArgs.m_xSysArgs.m_bHugePages);
  Constants::vAddConstant("args.heartbeat"_Stz,         gxCmdLineArgs.m_xSysArgs.m_nDefinedHeartbeats);
  Constants::vAddConstant("args.killPrev"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bKillPrev);
  Constants::vAddConstant("args.maxLoadErrors"_Stz,     gxCmdLineArgs.m_xSysArgs.m_nMaxLoadErrors);
  Constants::vAddConstant("args.maxReadyLP"_Stz,        gxCmdLineArgs.m_xSysArgs.m_nMaxReadyLP);
  Constants::vAddConstant("args.metadata"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bMetadata);
  Constants::vAddConstant("args.munged"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bMunged);
  Constants::vAddConstant("args.nodeName"_Stz,          gxCmdLineArgs.m_xSysArgs.m_sNodeName);
  Constants::vAddConstant("args.operands"_Stz,          gxCmdLineArgs.m_xSysArgs.m_nOperands);
  Constants::vAddConstant("args.operandYield"_Stz,          gxCmdLineArgs.m_xSysArgs.m_nOperandYield);
  Constants::vAddConstant("args.partitionsPerNode"_Stz, gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode);
  Constants::vAddConstant("args.periodicSleep"_Stz,     gxCmdLineArgs.m_xSysArgs.m_nPeriodicSleep);
  Constants::vAddConstant("args.progress"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bProgress);
  Constants::vAddConstant("args.prompt"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bPrompt);
  Constants::vAddConstant("args.regionPrefix"_Stz,      gxCmdLineArgs.m_xSysArgs.m_sRegionPrefix);
  Constants::vAddConstant("args.regionName"_Stz,        gxCmdLineArgs.m_xSysArgs.m_sRegionName);
  Constants::vAddConstant("args.regionSize"_Stz,        gxCmdLineArgs.m_xSysArgs.m_iRegionSize);
  Constants::vAddConstant("args.reload"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bReload);
  Constants::vAddConstant("args.restart"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bRestart);
  Constants::vAddConstant("args.RLUDiary"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bRLUDiary);
  Constants::vAddConstant("args.RLUTimer"_Stz,          gxCmdLineArgs.m_xSysArgs.m_bRLUTimer);
  Constants::vAddConstant("args.senDir"_Stz,            gxCmdLineArgs.m_xSysArgs.m_sSenDir);
  Constants::vAddConstant("args.shmlock"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bShmLock);
  Constants::vAddConstant("args.start"_Stz,             gxCmdLineArgs.m_xSysArgs.m_bStart);
  Constants::vAddConstant("args.status"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bStatus);
  Constants::vAddConstant("args.stop"_Stz,              gxCmdLineArgs.m_xSysArgs.m_bStop);
  Constants::vAddConstant("args.demonSleep"_Stz,   gxCmdLineArgs.m_xSysArgs.m_nDemonSleep);
  Constants::vAddConstant("args.tablePtns"_Stz,         gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode);
  Constants::vAddConstant("args.threads"_Stz,           gxCmdLineArgs.m_xSysArgs.m_nThreads);
  Constants::vAddConstant("args.touch"_Stz,             gxCmdLineArgs.m_xSysArgs.m_bTouch);
  Constants::vAddConstant("args.tracepoint"_Stz,        gxCmdLineArgs.m_xSysArgs.m_bTracepoint);
  Constants::vAddConstant("args.tracing"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bTracing);
  Constants::vAddConstant("args.unlink"_Stz,            gxCmdLineArgs.m_xSysArgs.m_bUnlink);
  Constants::vAddConstant("args.verbose"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bVerbose);
  Constants::vAddConstant("args.welcome"_Stz,           gxCmdLineArgs.m_xSysArgs.m_bWelcome);
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstantsCpp(void)
{
  if (AM::AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Adding constants...\n"_Stz);
  
  // true / flase
  vAddConstant("true"_Stz,    (i8)1);
  vAddConstant("false"_Stz,   (i8)0);
  
  vAddConstant("RAPIDSSE_VERSION_STRING"_Stz, AM::szVersionString());
  
  vAddConstant("C_BUILD_GIT_HASH"_Stz, gsBuildGitHash);
  vAddConstant("C_BUILD_DATE"_Stz,     gsBuildDate);
  vAddConstant("C_BUILD_HOSTNAME"_Stz, gsBuildHostName);
  vAddConstant("C_BUILD_USERNAME"_Stz, gsBuildUserName);
  vAddConstant("C_BUILD_BRANCH"_Stz,   gsBuildBranch);


  vAddConstant("C_SIZE_POOL_BUFFER"_Stz,   C_SIZE_POOL_BUFFER);
  vAddConstant("C_SIZE_MEDIUM_OBJECT"_Stz, C_SIZE_MEDIUM_OBJECT);
  vAddConstant("C_SIZE_SMALL_OBJECT"_Stz,  C_SIZE_SMALL_OBJECT);
  vAddConstant("C_MAGIC"_Stz,              C_MAGIC);
  
  // useful numbers
  vAddConstant("C_1KB"_Stz,   C_1KB);
  vAddConstant("C_1MB"_Stz,   C_1MB);
  vAddConstant("C_1GB"_Stz,   C_1GB);
  vAddConstant("C_1TB"_Stz,   C_1TB);
  vAddConstant("C_1PB"_Stz,   C_1PB);
  vAddConstant("C_1EB"_Stz,   C_1EB);
  
  vAddConstant("C_1K"_Stz,    C_1K);
  vAddConstant("C_10K"_Stz,   10 * C_1K);
  vAddConstant("C_100K"_Stz,  100 * C_1K);
  vAddConstant("C_1M"_Stz,    C_1M);
  vAddConstant("C_1G"_Stz,    C_1G);
  
  vAddConstant("C_2KB"_Stz,   C_2KB);
  vAddConstant("C_4KB"_Stz,   C_4KB);
  vAddConstant("C_8KB"_Stz,   C_8KB);
  vAddConstant("C_16KB"_Stz,  C_16KB);
  vAddConstant("C_32KB"_Stz,  C_32KB);
  vAddConstant("C_64KB"_Stz,  C_64KB);
  vAddConstant("C_128KB"_Stz, C_128KB);
  vAddConstant("C_256KB"_Stz, C_256KB);
  vAddConstant("C_512KB"_Stz, C_512KB);
  
  vAddConstant("C_ALL"_Stz, C_ALL);

  vAddConstant("C_MAX_INDEXES"_Stz,       C_MAX_INDEXES);
  vAddConstant("C_MAX_PTNSPERNODE"_Stz,         C_MAX_PTNSPERNODE);
  vAddConstant("C_MAX_NODES"_Stz,         C_MAX_NODES);
  vAddConstant("C_MAX_LGLPTNS"_Stz,    C_MAX_LGLPTNS);

  vAddConstant("C_LOW_BUFFERS"_Stz,       C_LOW_BUFFERS);
  vAddConstant("C_MAX_COLUMNS"_Stz,       C_MAX_COLUMNS);
  vAddConstant("C_MAX_RECORD"_Stz,        C_MAX_RECORD);
  vAddConstant("C_MAX_PATH"_Stz,          C_MAX_PATH);
  vAddConstant("C_SIZE_LARGE_TEXT"_Stz,   C_SIZE_LARGE_TEXT);

  // var args
  vAddConstant("C_VARGS_BEGIN"_Stz,       C_VARGS_BEGIN);
  vAddConstant("C_VARGS_END"_Stz,         C_VARGS_END);
  
  // OPEN_ : file open argument
  vAddConstant("OPEN_READ"_Stz,              C_OPEN_READ);
  vAddConstant("OPEN_WRITE"_Stz,             C_OPEN_WRITE);
  vAddConstant("OPEN_APPEND"_Stz,            C_OPEN_APPEND);
  vAddConstant("OPEN_READ_WRITE_EXIST"_Stz,  C_OPEN_READ_WRITE_EXIST);
  vAddConstant("OPEN_READ_WRITE_CREATE"_Stz, C_OPEN_READ_WRITE_CREATE);
  vAddConstant("OPEN_READ_APPEND"_Stz,       C_OPEN_READ_APPEND);
  
  // table definition
  vAddConstant("C_TDEF_ANALYZEINDEX"_Stz,  C_TDEF_ANALYZEINDEX);
  vAddConstant("C_TDEF_ANALYZETABLE"_Stz,  C_TDEF_ANALYZETABLE);
  vAddConstant("C_TDEF_CALLPIPELINE"_Stz,  C_TDEF_CALLPIPELINE);
  vAddConstant("C_TDEF_COLUMNDEFS"_Stz,    C_TDEF_COLUMNDEFS);
  vAddConstant("C_TDEF_COMMANDERRORS"_Stz, C_TDEF_COMMANDERRORS);
  vAddConstant("C_TDEF_COUNT"_Stz,         C_TDEF_COUNT);
  vAddConstant("C_TDEF_COUNTINFO"_Stz,     C_TDEF_COUNTINFO);
  vAddConstant("C_TDEF_GRIDSANITY"_Stz,    C_TDEF_GRIDSANITY);
  vAddConstant("C_TDEF_EXECERRORS"_Stz,    C_TDEF_EXECERRORS);
  vAddConstant("C_TDEF_EXPLAIN"_Stz,       C_TDEF_EXPLAIN);
  vAddConstant("C_TDEF_INDEXCOLUMNDEFS"_Stz, C_TDEF_INDEXCOLUMNDEFS);
  vAddConstant("C_TDEF_LISTCOLUMNS"_Stz,   C_TDEF_LISTCOLUMNS);
  vAddConstant("C_TDEF_LISTCONTENTS"_Stz,  C_TDEF_LISTCONTENTS);
  vAddConstant("C_TDEF_LISTFOLDER"_Stz,    C_TDEF_LISTFOLDER);
  vAddConstant("C_TDEF_LISTINDEX"_Stz,     C_TDEF_LISTINDEX);
  vAddConstant("C_TDEF_LISTNODES"_Stz,     C_TDEF_LISTNODES);
  vAddConstant("C_TDEF_LISTPARTITIONS"_Stz,C_TDEF_LISTPARTITIONS);
  vAddConstant("C_TDEF_LOADRESULT"_Stz,    C_TDEF_LOADRESULT);
  vAddConstant("C_TDEF_LOCKSTATS"_Stz,     C_TDEF_LOCKSTATS);
  vAddConstant("C_TDEF_LOOKUPCATALOG"_Stz, C_TDEF_LOOKUPCATALOG);
  vAddConstant("C_TDEF_LOOKUPINDEX"_Stz,   C_TDEF_LOOKUPINDEX);
  vAddConstant("C_TDEF_LOOKUPSCHEMA"_Stz,  C_TDEF_LOOKUPSCHEMA);
  vAddConstant("C_TDEF_LOOKUPTABLE"_Stz,   C_TDEF_LOOKUPTABLE);
  vAddConstant("C_TDEF_NODEINFO"_Stz,      C_TDEF_NODEINFO);
  vAddConstant("C_TDEF_PATHINFO"_Stz,      C_TDEF_PATHINFO);
  vAddConstant("C_TDEF_PARTITIONKEY"_Stz,  C_TDEF_PARTITIONKEY);
  vAddConstant("C_TDEF_PINGINFO"_Stz,      C_TDEF_PINGINFO);
  vAddConstant("C_TDEF_PROGRESSUNITS"_Stz, C_TDEF_PROGRESSUNITS);
  vAddConstant("C_TDEF_PROMPTOUT"_Stz,     C_TDEF_PROMPTOUT);
  vAddConstant("C_TDEF_PWF"_Stz,           C_TDEF_PWF);
  vAddConstant("C_TDEF_RETURNPIPELINE"_Stz,C_TDEF_RETURNPIPELINE);
  vAddConstant("C_TDEF_SEEKOFFSETS"_Stz,   C_TDEF_SEEKOFFSETS);
  vAddConstant("C_TDEF_SHOWCLUSTER"_Stz,   C_TDEF_SHOWCLUSTER);
  vAddConstant("C_TDEF_SIZEINDEX"_Stz,     C_TDEF_SIZEINDEX);
  vAddConstant("C_TDEF_SIZETABLE"_Stz,     C_TDEF_SIZETABLE);
  vAddConstant("C_TDEF_SPACE"_Stz,         C_TDEF_SPACE);
  vAddConstant("C_TDEF_STATUS"_Stz,        C_TDEF_STATUS);
  vAddConstant("C_TDEF_TABLESIZES"_Stz,    C_TDEF_TABLESIZES);
  vAddConstant("C_TDEF_TOTALUNITS"_Stz,    C_TDEF_TOTALUNITS);
  vAddConstant("C_TDEF_VERIFYINDEX"_Stz,   C_TDEF_VERIFYINDEX);
  vAddConstant("C_TDEF_VERIFYTABLE"_Stz,   C_TDEF_VERIFYTABLE);

  // hardware
  vAddConstant("HW_CLOCK_FREQUENCY"_Stz, TickingDevice::iTicksPerSec());
  vAddConstant("HW_CORE_COUNT"_Stz,      Hardware::iCoreCount());
  vAddConstant("HW_CPU_COUNT"_Stz,       Hardware::iCpuCount());
  vAddConstant("HW_PHYSICAL_MEMORY"_Stz, Hardware::iPhysicalMemory());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  // AT_ : fetch techniques
  /*  0 */ vAddConstant("AT_NONE"_Stz,     AT_NONE);
  /*  1 */ vAddConstant("AT_EXACT"_Stz,    AT_EXACT);
  /*  2 */ vAddConstant("AT_LOWER"_Stz,    AT_LOWER);
  /*  3 */ vAddConstant("AT_UPPER"_Stz,    AT_UPPER);
  /*  4 */ vAddConstant("AT_RANGE"_Stz,    AT_RANGE);
  /*  5 */ vAddConstant("AT_SCAN"_Stz,     AT_SCAN);
  /*  6 */ vAddConstant("AT_ENUMS"_Stz,    AT_ENUMS);
  
  // CN_ : class number
  /*    */ vAddConstant("CN_EACHROWBUFFER"_Stz,CN_EACHROWBUFFER);
  /*    */ vAddConstant("CN_CRWBUFFER"_Stz,    CN_CRWBUFFER);
  /*    */ vAddConstant("CN_JOB"_Stz,          CN_JOB);
  /*    */ vAddConstant("CN_ROWBUFFER"_Stz,    CN_ROWBUFFER);
  /*    */ vAddConstant("CN_STZVAL"_Stz,       CN_STZVAL);
  /*    */ vAddConstant("CN_PARCEL"_Stz,  CN_PARCEL);

  
  // CT_ : columne type
  /*  0 */ vAddConstant("CT_NONE"_Stz,        CT_NONE);
  /*  1 */ vAddConstant("CT_BOOLEAN"_Stz,     CT_BOOLEAN);
  /*  2 */ vAddConstant("CT_CHAR"_Stz,        CT_CHAR);
  /*  3 */ vAddConstant("CT_DATE"_Stz,        CT_DATE);
  /*  4 */ vAddConstant("CT_DECIMAL"_Stz,     CT_DECIMAL);
  /*  5 */ vAddConstant("CT_FLOAT"_Stz,       CT_FLOAT);
  /*  6 */ vAddConstant("CT_INTEGER"_Stz,     CT_INTEGER);
  /*  7 */ vAddConstant("CT_INTERVAL_DT"_Stz, CT_INTERVAL_DT);
  /*  8 */ vAddConstant("CT_INTERVAL_YM"_Stz, CT_INTERVAL_YM);
  /*  9 */ vAddConstant("CT_TIME"_Stz,        CT_TIME);
  /* 10 */ vAddConstant("CT_TIMESTAMP"_Stz,   CT_TIMESTAMP);
  /* 11 */ vAddConstant("CT_VARCHAR"_Stz,     CT_VARCHAR);
  /* 12 */ vAddConstant("CT_UNKNOWN"_Stz,     CT_UNKNOWN);
  /* 13 */ vAddConstant("CT_ENUMS"_Stz,       CT_ENUMS);

  // DE_ : directory entry type -- n.b. the DT_FIFO etc are from dirent.h
  /*  1 */ vAddConstant("DE_FIFO"_Stz, DT_FIFO);
  /*  2 */ vAddConstant("DE_CHR"_Stz,  DT_CHR);
  /*  3 */ vAddConstant("DE_DIR"_Stz,  DT_DIR);
  /*  6 */ vAddConstant("DE_BLK"_Stz,  DT_BLK);
  /*  8 */ vAddConstant("DE_REG"_Stz,  DT_REG);
  /* 10 */ vAddConstant("DE_LNK"_Stz,  DT_LNK);
  /* 12 */ vAddConstant("DE_SOCK"_Stz, DT_SOCK);

  // DL_ : ddl lock
  /*  0 */ vAddConstant("DL_NONE"_Stz,       DL_NONE);
  /*  1 */ vAddConstant("DL_GRAB_READ"_Stz,  DL_GRAB_READ);
  /*  2 */ vAddConstant("DL_GRAB_WRITE"_Stz, DL_GRAB_WRITE);

  // DM_ : datum type
  /*  1 */ vAddConstant("DM_BCD"_Stz, DM_BCD);
  /*  2 */ vAddConstant("DM_BOL"_Stz, DM_BOL);
  /*  3 */ vAddConstant("DM_DTE"_Stz, DM_DTE);
  /*  4 */ vAddConstant("DM_FLT"_Stz, DM_FLT);
  /*  5 */ vAddConstant("DM_INT"_Stz, DM_INT);
  /*  6 */ vAddConstant("DM_IVL"_Stz, DM_IVL);
  /*  7 */ vAddConstant("DM_IVY"_Stz, DM_IVY);
  /*  8 */ vAddConstant("DM_TME"_Stz, DM_TME);
  /*  9 */ vAddConstant("DM_TSP"_Stz, DM_TSP);
  /* 10 */ vAddConstant("DM_VCH"_Stz, DM_VCH);
  /* 10 */ vAddConstant("DM_UNK"_Stz, DM_UNK);

  // EC_ : error codes
  /*  0 */ vAddConstant("EC_NONE"_Stz,               EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT"_Stz,             EC_ASSERT);
  
  /*    */ vAddConstant("EC_CATALOG_EXISTS"_Stz,     EC_CATALOG_EXISTS);
  /*  4 */ vAddConstant("EC_CLUSTER_NOT_CONNECTED"_Stz,EC_CLUSTER_NOT_CONNECTED);
  /*  5 */ vAddConstant("EC_CLUSTER_PARTITIONS_MISMATCH"_Stz,EC_CLUSTER_PARTITIONS_MISMATCH);
  /*  6 */ vAddConstant("EC_CLUSTER_SIZE_MISMATCH"_Stz,      EC_CLUSTER_SIZE_MISMATCH);

  /*  6 */ vAddConstant("EC_DECIMAL_NOT_SUPPORTED"_Stz,      EC_DECIMAL_NOT_SUPPORTED);
  /*  7 */ vAddConstant("EC_DUPLICATE_COLUMN"_Stz,   EC_DUPLICATE_COLUMN);
  /*  8 */ vAddConstant("EC_DUPLICATE_KEY"_Stz,      EC_DUPLICATE_KEY);
  /*  9 */ vAddConstant("EC_FOLDER_IN_USE"_Stz,      EC_FOLDER_IN_USE);
  /* 10 */ vAddConstant("EC_FOLDER_NOT_EMPTY"_Stz,   EC_FOLDER_NOT_EMPTY);
  /* 11 */ vAddConstant("EC_INDEX_EXISTS"_Stz,       EC_INDEX_EXISTS);
  /* 11 */ vAddConstant("EC_INTERNAL_ERROR"_Stz,             EC_INTERNAL_ERROR);
  /* 11 */ vAddConstant("EC_INTERVAL_NOT_SUPPORTED"_Stz,          EC_INTERVAL_NOT_SUPPORTED);

  /* 12 */ vAddConstant("EC_INVALID_CLASS"_Stz,      EC_INVALID_CLASS);
  /* 13 */ vAddConstant("EC_INVALID_COLUMN"_Stz,     EC_INVALID_COLUMN);
  /* 14 */ vAddConstant("EC_INVALID_COLUMNS_UNIQUE_INDEX"_Stz,EC_INVALID_COLUMNS_UNIQUE_INDEX);
  /* 15 */ vAddConstant("EC_INVALID_COMMAND"_Stz,    EC_INVALID_COMMAND);
  
  /* 16 */ vAddConstant("EC_INVALID_FILE"_Stz,       EC_INVALID_FILE);
  /* 17 */ vAddConstant("EC_INVALID_INTERFACE"_Stz,  EC_INVALID_INTERFACE);
  /* 18 */ vAddConstant("EC_INVALID_PARAMETER"_Stz,  EC_INVALID_PARAMETER);
  /* 19 */ vAddConstant("EC_INVALID_PARAMETERS"_Stz, EC_INVALID_PARAMETERS);
  /* 19 */ vAddConstant("EC_INVALID_PARTITION"_Stz,  EC_INVALID_PARTITION);
  /* 19 */ vAddConstant("EC_INVALID_PARTITION_KEY"_Stz,  EC_INVALID_PARTITION_KEY);
  /*    */ vAddConstant("EC_INVALID_INDEX_COLUMN"_Stz, EC_INVALID_INDEX_COLUMN);
  /* 20 */ vAddConstant("EC_INVALID_INSTANCE"_Stz,   EC_INVALID_INSTANCE);
  /*    */ vAddConstant("EC_INVALID_SIZE"_Stz,       EC_INVALID_SIZE);
  /* 21 */ vAddConstant("EC_INVALID_SYNTAX"_Stz,     EC_INVALID_SYNTAX);
  /* 22 */ vAddConstant("EC_INVALID_VALUE"_Stz,      EC_INVALID_VALUE);
                        
  /* 23 */ vAddConstant("EC_LOW_MEMORY"_Stz,         EC_LOW_MEMORY);
  
  /* 24 */ vAddConstant("EC_MAX_INDEXES"_Stz,        EC_MAX_INDEXES);
  /*    */ vAddConstant("EC_MAX_COLUMNS"_Stz,        EC_MAX_COLUMNS);
  /*    */ vAddConstant("EC_MAX_ROW_LENGTH"_Stz,     EC_MAX_ROW_LENGTH);
  
  /* 25 */ vAddConstant("EC_NO_COLUMNS_DIC"_Stz,     EC_NO_COLUMNS_DIC);
  /* 26 */ vAddConstant("EC_NO_SUCH_CATALOG"_Stz,    EC_NO_SUCH_CATALOG);
  /* 27 */ vAddConstant("EC_NO_SUCH_DIRECTORY"_Stz,  EC_NO_SUCH_DIRECTORY);
  /* 28 */ vAddConstant("EC_NO_SUCH_FOLDER"_Stz,     EC_NO_SUCH_FOLDER);
  /* 28 */ vAddConstant("EC_NO_SUCH_FUNCTION"_Stz,   EC_NO_SUCH_FUNCTION);
  /* 29 */ vAddConstant("EC_NO_SUCH_INDEX"_Stz,      EC_NO_SUCH_INDEX);
  /* 29 */ vAddConstant("EC_NO_SUCH_NODE"_Stz,       EC_NO_SUCH_NODE);
  /* 30 */ vAddConstant("EC_NO_SUCH_SCHEMA"_Stz,     EC_NO_SUCH_SCHEMA);
  /* 31 */ vAddConstant("EC_NO_SUCH_TABLE"_Stz,      EC_NO_SUCH_TABLE);
  /* 32 */ vAddConstant("EC_NO_SUCH_TENANT"_Stz,     EC_NO_SUCH_TENANT);
  /* 33 */ vAddConstant("EC_NO_SUCH_WORD"_Stz,       EC_NO_SUCH_WORD);
  
  /* 34 */ vAddConstant("EC_NODE_EXISTS"_Stz,        EC_NODE_EXISTS);
  
  /* 34 */ vAddConstant("EC_NOT_A_CATALOG"_Stz,      EC_NOT_A_CATALOG);
  /* 35 */ vAddConstant("EC_NOT_A_FOLDER"_Stz,       EC_NOT_A_FOLDER);
  /* 36 */ vAddConstant("EC_NOT_AN_INDEX"_Stz,       EC_NOT_AN_INDEX);
  /* 37 */ vAddConstant("EC_NOT_A_SCHEMA"_Stz,       EC_NOT_A_SCHEMA);
  /* 38 */ vAddConstant("EC_NOT_A_TABLE"_Stz,        EC_NOT_A_TABLE);
  /* 39 */ vAddConstant("EC_NOT_A_TENANT"_Stz,       EC_NOT_A_TENANT);
  
  /* 40 */ vAddConstant("EC_NOT_IN_CATALOGS"_Stz,    EC_NOT_IN_CATALOGS);
  /* 41 */ vAddConstant("EC_NOT_IN_COLUMNS"_Stz,     EC_NOT_IN_COLUMNS);
  /* 42 */ vAddConstant("EC_NOT_IN_INDEXES"_Stz,     EC_NOT_IN_INDEXES);
  /* 43 */ vAddConstant("EC_NOT_IN_PARTITIONS"_Stz,  EC_NOT_IN_PARTITIONS);
  /* 44 */ vAddConstant("EC_NOT_IN_SCHEMAS"_Stz,     EC_NOT_IN_SCHEMAS);
  /* 45 */ vAddConstant("EC_NOT_IN_TABLE"_Stz,       EC_NOT_IN_TABLE);
  /* 46 */ vAddConstant("EC_NOT_IN_TABLES"_Stz,      EC_NOT_IN_TABLES);
  /* 47 */ vAddConstant("EC_NOT_IN_TENANTS"_Stz,     EC_NOT_IN_TENANTS);
  /* 48 */ vAddConstant("EC_NULL_INTERFACE"_Stz,     EC_NULL_INTERFACE);
  /* 49 */ vAddConstant("EC_NULLPTR"_Stz,            EC_NULLPTR);

  /*    */ vAddConstant("EC_ORDERBY_NOT_SUPPORTED"_Stz,EC_ORDERBY_NOT_SUPPORTED);

  /*    */ vAddConstant("EC_PIPELINE_PROTOCOL_ERROR"_Stz,EC_PIPELINE_PROTOCOL_ERROR);
  
  /* 50 */ vAddConstant("EC_RESPONSE_PENDING"_Stz,   EC_RESPONSE_PENDING);
  
  /* 51 */ vAddConstant("EC_SCHEMA_EXISTS"_Stz,      EC_SCHEMA_EXISTS);
  /* 52 */ vAddConstant("EC_SE_NOT_RUNNING"_Stz,     EC_SE_NOT_RUNNING);
  /* 53 */ vAddConstant("EC_SE_UNKNOWN_ERROR"_Stz,   EC_SE_UNKNOWN_ERROR);
  /* 54 */ vAddConstant("EC_SEND_FAILURE"_Stz,       EC_SEND_FAILURE);
  /*    */ vAddConstant("EC_SYSTEM_CLOSED"_Stz,      EC_SYSTEM_CLOSED);

  /*    */ vAddConstant("EC_TABLE_ALREADY_LOADED"_Stz, EC_TABLE_ALREADY_LOADED);
  /* 55 */ vAddConstant("EC_TABLE_EXISTS"_Stz,       EC_TABLE_EXISTS);
  /* 56 */ vAddConstant("EC_TABLE_IN_USE"_Stz,       EC_TABLE_IN_USE);
  /* 57 */ vAddConstant("EC_TABLE_NOT_EMPTY"_Stz,    EC_TABLE_NOT_EMPTY);
  /* 58 */ vAddConstant("EC_TENANT_EXISTS"_Stz,      EC_TENANT_EXISTS);
  /*    */ vAddConstant("EC_TINYC_ERROR"_Stz,        EC_TINYC_ERROR);
  
  /*    */ vAddConstant("EC_TX_BEGIN_FAILED"_Stz,    EC_TX_BEGIN_FAILED);
  /*    */ vAddConstant("EC_TX_COMMIT_FAILED"_Stz,   EC_TX_COMMIT_FAILED);
  /*    */ vAddConstant("EC_TX_END_FAILED"_Stz,      EC_TX_END_FAILED);
  /*    */ vAddConstant("EC_TX_ROLLBACK_FAILED"_Stz, EC_TX_ROLLBACK_FAILED);
  
  /* 59 */ vAddConstant("EC_VALIDATION_FAILURE"_Stz, EC_VALIDATION_FAILURE);
  /* 60 */ vAddConstant("EC_VERSION_MISMATCH"_Stz,   EC_VERSION_MISMATCH);
  /* 61 */ vAddConstant("EC_ENUMS"_Stz,              EC_ENUMS);

  
  
  vAddConstant("EG_NONE"_Stz, EG_NONE);

  vAddConstant("EG_AA"_Stz,   EG_AA);
  vAddConstant("EG_AC"_Stz,   EG_AC);
  vAddConstant("EG_AK"_Stz,   EG_AK);
  vAddConstant("EG_AN"_Stz,   EG_AN);
  vAddConstant("EG_AT"_Stz,   EG_AT);
  vAddConstant("EG_BO"_Stz,   EG_BO);
  vAddConstant("EG_CC"_Stz,   EG_CC);
  vAddConstant("EG_CM"_Stz,   EG_CM);
  vAddConstant("EG_CN"_Stz,   EG_CN);
  vAddConstant("EG_CP"_Stz,   EG_CP);
  vAddConstant("EG_CS"_Stz,   EG_CS);
  vAddConstant("EG_CT"_Stz,   EG_CT);
  vAddConstant("EG_CX"_Stz,   EG_CX);
  vAddConstant("EG_DA"_Stz,   EG_DA);
  vAddConstant("EG_DC"_Stz,   EG_DC);
  vAddConstant("EG_DL"_Stz,   EG_DL);
  vAddConstant("EG_DM"_Stz,   EG_DM);
  vAddConstant("EG_DS"_Stz,   EG_DS);
  vAddConstant("EG_EA"_Stz,   EG_EA);
  vAddConstant("EG_EC"_Stz,   EG_EC);
  vAddConstant("EG_ED"_Stz,   EG_ED);
  vAddConstant("EG_EO"_Stz,   EG_EO);
  vAddConstant("EG_EP"_Stz,   EG_EP);
  vAddConstant("EG_FN"_Stz,   EG_FN);
  vAddConstant("EG_GA"_Stz,   EG_GA);
  vAddConstant("EG_HA"_Stz,   EG_HA);
  vAddConstant("EG_HS"_Stz,   EG_HS);
  vAddConstant("EG_HT"_Stz,   EG_HT);
  vAddConstant("EG_IK"_Stz,   EG_IK);
  vAddConstant("EG_IS"_Stz,   EG_IS);
  vAddConstant("EG_IT"_Stz,   EG_IT);
  vAddConstant("EG_JF"_Stz,   EG_JF);
  vAddConstant("EG_JK"_Stz,   EG_JK);
  vAddConstant("EG_JL"_Stz,   EG_JL);
  vAddConstant("EG_JP"_Stz,   EG_JP);
  vAddConstant("EG_JS"_Stz,   EG_JS);
  vAddConstant("EG_KC"_Stz,   EG_KC);
  vAddConstant("EG_KE"_Stz,   EG_KE);
  vAddConstant("EG_KK"_Stz,   EG_KK);
  vAddConstant("EG_LA"_Stz,   EG_LA);
  vAddConstant("EG_LH"_Stz,   EG_LH);
  vAddConstant("EG_LG"_Stz,   EG_LG);
  vAddConstant("EG_MA"_Stz,   EG_MA);
  vAddConstant("EG_MC"_Stz,   EG_MC);
  vAddConstant("EG_MM"_Stz,   EG_MM);
  vAddConstant("EG_MT"_Stz,   EG_MT);
  vAddConstant("EG_NE"_Stz,   EG_NE);
  vAddConstant("EG_NS"_Stz,   EG_NS);
  vAddConstant("EG_OA"_Stz,   EG_OA);
  vAddConstant("EG_OI"_Stz,   EG_OI);
  vAddConstant("EG_OK"_Stz,   EG_OK);
  vAddConstant("EG_OM"_Stz,   EG_OM);
  vAddConstant("EG_OR"_Stz,   EG_OR);
  vAddConstant("EG_PA"_Stz,   EG_PA);
  vAddConstant("EG_PF"_Stz,   EG_PF);
  vAddConstant("EG_PK"_Stz,   EG_PK);
  vAddConstant("EG_PP"_Stz,   EG_PP);
  vAddConstant("EG_PT"_Stz,   EG_PT);
  vAddConstant("EG_PY"_Stz,   EG_PY);
  vAddConstant("EG_QK"_Stz,   EG_QK);
  vAddConstant("EG_RA"_Stz,   EG_RA);
  vAddConstant("EG_RE"_Stz,   EG_RE);
  vAddConstant("EG_RG"_Stz,   EG_RG);
  vAddConstant("EG_RO"_Stz,   EG_RO);
  vAddConstant("EG_RS"_Stz,   EG_RS);
  vAddConstant("EG_RU"_Stz,   EG_RU);
  vAddConstant("EG_SC"_Stz,   EG_SC);
  vAddConstant("EG_SI"_Stz,   EG_SI);
  vAddConstant("EG_SK"_Stz,   EG_SK);
  vAddConstant("EG_ST"_Stz,   EG_ST);
  vAddConstant("EG_TA"_Stz,   EG_TA);
  vAddConstant("EG_TN"_Stz,   EG_TN);
  vAddConstant("EG_TR"_Stz,   EG_TR);
  vAddConstant("EG_TS"_Stz,   EG_TS);
  vAddConstant("EG_TT"_Stz,   EG_TT);
  vAddConstant("EG_UF"_Stz,   EG_UF);
  vAddConstant("EG_UK"_Stz,   EG_UK);
  vAddConstant("EG_VA"_Stz,   EG_VA);
  vAddConstant("EG_WK"_Stz,   EG_WK);
  vAddConstant("EG_XA"_Stz,   EG_XA);
  vAddConstant("EG_XK"_Stz,   EG_XK);
  vAddConstant("EG_XP"_Stz,   EG_XP);
  vAddConstant("EG_YG"_Stz,   EG_YG);
  vAddConstant("EG_YX"_Stz,   EG_YX);
  vAddConstant("EG_ZK"_Stz,   EG_ZK);

  vAddConstant("EG_ENUMS"_Stz, EC_ENUMS);

  
  
  
  // JK_ : job kind
  /*  0 */ vAddConstant("JK_NONE"_Stz,      JK_NONE);
  /*  1 */ vAddConstant("JK_BOOT"_Stz,      JK_BOOT);
  /*  2 */ vAddConstant("JK_CHILD"_Stz,     JK_CHILD);
  /*  3 */ vAddConstant("JK_PIPELINE"_Stz,   JK_PIPELINE);
  /*  4 */ vAddConstant("JK_GENERIC"_Stz,   JK_GENERIC);
  /*  5 */ vAddConstant("JK_GRANDKID"_Stz,  JK_GRANDKID);
  /*  6 */ vAddConstant("JK_INFO"_Stz,      JK_INFO);
  /*  7 */ vAddConstant("JK_LOCAL"_Stz,     JK_LOCAL);
  /*  8 */ vAddConstant("JK_META"_Stz,      JK_META);
  /*  9 */ vAddConstant("JK_OP"_Stz,        JK_OP);
  /* 10 */ vAddConstant("JK_SERVICE"_Stz,   JK_SERVICE);
  /* 11 */ vAddConstant("JK_OTHER"_Stz,     JK_OTHER);

  /* 0 */ vAddConstant("JL_NONE"_Stz,         JL_NONE);
  /* 1 */ vAddConstant("JL_DEMON"_Stz,        JL_DEMON);
  /* 2 */ vAddConstant("JL_LOCAL"_Stz,        JL_LOCAL);
  /* 3 */ vAddConstant("JL_ORIGINATING"_Stz,  JL_ORIGINATING);
  /* 4 */ vAddConstant("JL_PARTICIPATING"_Stz,JL_PARTICIPATING);
  
  // JP_ : job phase
  /*  0 */ vAddConstant("JP_NONE"_Stz,      JP_NONE);
  /*  0 */ vAddConstant("JP_BEGINNING"_Stz, JP_BEGINNING);
  /*  0 */ vAddConstant("JP_BEGUN"_Stz,     JP_BEGUN);
  /*  0 */ vAddConstant("JP_EXECUTING"_Stz, JP_EXECUTING);
  /*  0 */ vAddConstant("JP_EXECUTED"_Stz,  JP_EXECUTED);
  /*  0 */ vAddConstant("JP_ENDING"_Stz,    JP_ENDING);
  /*  0 */ vAddConstant("JP_ENDED"_Stz,     JP_ENDED);
  /*  0 */ vAddConstant("JP_CANCELING"_Stz, JP_CANCELING);
  /*  0 */ vAddConstant("JP_CANCELED"_Stz,  JP_CANCELED);
  /*  0 */ vAddConstant("JP_ENUMS"_Stz,     JP_ENUMS);

  // JS_ : job state
  /*  0 */ vAddConstant("JS_NONE"_Stz,      JS_NONE);
  /*  0 */ vAddConstant("JS_ACTIVE"_Stz,    JS_ACTIVE);
  /*  0 */ vAddConstant("JS_DELETABLE"_Stz, JS_DELETABLE);
  /*  0 */ vAddConstant("JS_LOCKED"_Stz,    JS_LOCKED);
  /*  0 */ vAddConstant("JS_RETIRED"_Stz,   JS_RETIRED);
  /*  0 */ vAddConstant("JS_SIGNALLED"_Stz, JS_SIGNALLED);

  // C_MAX : MAX
  /*  0 */ vAddConstant("C_MAX_BCD_PRECISION"_Stz, C_MAX_BCD_PRECISION);
  /*  0 */ vAddConstant("C_MAX_FLT_PRECISION"_Stz, C_MAX_FLT_PRECISION);
  /*  0 */ vAddConstant("C_MAX_INT_PRECISION"_Stz, C_MAX_INT_PRECISION);
  /*  0 */ vAddConstant("C_MAX_VCH_PRECISION"_Stz, C_MAX_VCH_PRECISION);
  
  // NS_ : node state
  /*  0 */ vAddConstant("NS_NONE"_Stz,      NS_NONE);
  /*  0 */ vAddConstant("NS_ACTIVE"_Stz,    NS_ACTIVE);
  /*  0 */ vAddConstant("NS_INACTIVE"_Stz,  NS_INACTIVE);

  // OI_ : operator infoset
  /*  0 */ vAddConstant("OI_NONE"_Stz,          OI_NONE);
  /*  0 */ vAddConstant("OI_CALLPIPELINE"_Stz,  OI_CALLPIPELINE);
  /*  0 */ vAddConstant("OI_COMMANDERRORS"_Stz, OI_COMMANDERRORS);
  /*  0 */ vAddConstant("OI_COUNTINFO"_Stz,     OI_COUNTINFO);
  /*  0 */ vAddConstant("OI_EXECERRORS"_Stz,    OI_EXECERRORS);
  /*  0 */ vAddConstant("OI_LOADRESULT"_Stz,    OI_LOADRESULT);
  /*  0 */ vAddConstant("OI_LOCKSTATS"_Stz,     OI_LOCKSTATS);
  /*  0 */ vAddConstant("OI_NODEINFO"_Stz,      OI_NODEINFO);
  /*  0 */ vAddConstant("OI_PATHINFO"_Stz,      OI_PATHINFO);
  /*  0 */ vAddConstant("OI_PROGRESSUNITS"_Stz, OI_PROGRESSUNITS);
  /*  0 */ vAddConstant("OI_RETURNPIPELINE"_Stz,OI_RETURNPIPELINE);
  /*  0 */ vAddConstant("OI_SEEKOFFSETS"_Stz,   OI_SEEKOFFSETS);
  /*  0 */ vAddConstant("OI_TOTALUNITS"_Stz,    OI_TOTALUNITS);

  // PD_ : pipe direction
  /*  0 */ vAddConstant("PD_NONE"_Stz,      PP_NONE);
  /*  0 */ vAddConstant("PD_PIPE_IN"_Stz,   PD_PIPE_IN);
  /*  0 */ vAddConstant("PD_PIPE_OUT"_Stz,  PD_PIPE_OUT);
  
  // PP_ : pipeline protocol
  /*  0 */ vAddConstant("PP_NONE"_Stz,            PP_NONE);
  /*  0 */ vAddConstant("PP_SIGNAL_SESSION"_Stz,  PP_SIGNAL_SESSION);
  /*  0 */ vAddConstant("PP_RETURN_PIPELINE"_Stz, PP_RETURN_PIPELINE);
  
  // PY_ : priority
  /*  0 */ vAddConstant("PY_NONE"_Stz,      PY_NONE);
  /*  0 */ vAddConstant("PY_LOW"_Stz,       PY_LOW);
  /*  0 */ vAddConstant("PY_NORMAL"_Stz,    PY_NORMAL);

  // RG_ : register
  /*  0 */ vAddConstant("dir[0]"_Stz,       RG_DIR0);
  /*  0 */ vAddConstant("dir[1]"_Stz,       RG_DIR1);
  /*  0 */ vAddConstant("dir[2]"_Stz,       RG_DIR2);
  /*  0 */ vAddConstant("file[0]"_Stz,      RG_FILE0);
  /*  0 */ vAddConstant("file[1]"_Stz,      RG_FILE1);
  /*  0 */ vAddConstant("file[2]"_Stz,      RG_FILE2);

  // RS_ : response kind
  /*  0 */ vAddConstant("RS_NONE"_Stz,                 RS_NONE);
  /*  0 */ vAddConstant("RS_LOCAL_RESPONSE"_Stz,       RS_LOCAL_RESPONSE);
  /*  0 */ vAddConstant("RS_DIS2PC_RESPONSE"_Stz,      RS_DIS2PC_RESPONSE);
  
  // TN_ : token types
  /*  0 */ vAddConstant("TN_NONE"_Stz,      TN_NONE);
  /*  1 */ vAddConstant("TN_NULL"_Stz,      TN_NULL);
  /*  2 */ vAddConstant("TN_INTEGER"_Stz,   TN_INTEGER);
  /*  3 */ vAddConstant("TN_NAME"_Stz,      TN_NAME);
  /*  4 */ vAddConstant("TN_OTHER"_Stz,     TN_OTHER);
  /*  5 */ vAddConstant("TN_STRING"_Stz,    TN_STRING);
  /*  6 */ vAddConstant("TN_TAG"_Stz,       TN_TAG);
  /*  7 */ vAddConstant("TN_ENUMS"_Stz,     TN_ENUMS);
    
  // TS_ : task state
  /*  0 */ vAddConstant("TS_NONE"_Stz,       TS_NONE);
  /*  1 */ vAddConstant("TS_BIDING"_Stz,     TS_BIDING);
  /*  2 */ vAddConstant("TS_EXECUTING"_Stz,  TS_EXECUTING);
  /*  3 */ vAddConstant("TS_COMPLETED"_Stz,  TS_COMPLETED);
  /*  4 */ vAddConstant("TS_READY"_Stz,      TS_READY);
  /*  5 */ vAddConstant("TS_SLEEPING"_Stz,   TS_SLEEPING);
  /*  6 */ vAddConstant("TS_WAITING"_Stz,    TS_WAITING);
  /*  7 */ vAddConstant("TS_ENUMS"_Stz,      TS_ENUMS);
 
  // TR_ : task transition
  /*  0 */ vAddConstant("TR_NONE"_Stz,             TR_NONE);
  /*  1 */ vAddConstant("TR_BIDE_FOR_OPERAND"_Stz, TR_BIDE_FOR_OPERAND);
  /*  2 */ vAddConstant("TR_DELETE_THIS_TASK"_Stz, TR_DELETE_THIS_TASK);
  /*  3 */ vAddConstant("TR_RETIRE_THIS_TASK"_Stz, TR_RETIRE_THIS_TASK);
  /*  4 */ vAddConstant("TR_SLEEP_SOME_TIME"_Stz,  TR_SLEEP_SOME_TIME);
  /*  5 */ vAddConstant("TR_WAIT_FOR_RESUME"_Stz,  TR_WAIT_FOR_RESUME);
  /*  6 */ vAddConstant("TR_YIELD_FOR_OTHERS"_Stz, TR_YIELD_FOR_OTHERS);
  /*  7 */ vAddConstant("TR_ENUMS"_Stz,            TR_ENUMS);
  
  // UK_ : function type
  /*  0 */ vAddConstant("UK_NONE"_Stz,       UK_NONE);
  /*  0 */ vAddConstant("UK_AGG"_Stz,        UK_AGG);
  /*  0 */ vAddConstant("UK_FILTER"_Stz,     UK_FILTER);
  /*  0 */ vAddConstant("UK_KEY"_Stz,        UK_KEY);
  /*  0 */ vAddConstant("UK_PROJECT"_Stz,    UK_PROJECT);

  // WK_ : word type
  /*  0 */ vAddConstant("WK_NONE"_Stz,       WK_NONE);
  /*  0 */ vAddConstant("WK_CPPOBJECT"_Stz,  WK_CPPOBJECT);
  /*  0 */ vAddConstant("WK_INTRINSIC"_Stz,  WK_INTRINSIC);
  /*  0 */ vAddConstant("WK_LOCAL"_Stz,      WK_LOCAL);
  /*  0 */ vAddConstant("WK_COLON"_Stz,      WK_COLON);
  /*  0 */ vAddConstant("WK_ENUMS"_Stz,      WK_ENUMS);
  
  // XP_ : transaction phase
  /*  0 */ vAddConstant("XP_NONE"_Stz,        XP_NONE);
  /*  1 */ vAddConstant("XP_EXECUTING"_Stz,   XP_EXECUTING);
  /*  2 */ vAddConstant("XP_COMMITTING"_Stz,  XP_COMMITTING);
  /*  3 */ vAddConstant("XP_COMMITTED"_Stz,   XP_COMMITTED);
  /*  4 */ vAddConstant("XP_PREPARING"_Stz,   XP_PREPARING);
  /*  5 */ vAddConstant("XP_PREPARED"_Stz,    XP_PREPARED);
  /*  6 */ vAddConstant("XP_ROLLINGBACK"_Stz, XP_ROLLINGBACK);
  /*  7 */ vAddConstant("XP_ROLLEDBACK"_Stz,  XP_ROLLEDBACK);
  /*  8 */ vAddConstant("XP_ENUMS"_Stz,       XP_ENUMS);
  
  // ZK_ : operand kind
  /*  0 */ vAddConstant("ZK_NONE"_Stz,            ZK_NONE);
  /*  0 */ vAddConstant("ZK_D_BKTMAPGET"_Stz,       ZK_D_BKTMAPGET);
  /*  0 */ vAddConstant("ZK_D_BUCKETGET"_Stz,       ZK_D_BUCKETGET);
  /*  0 */ vAddConstant("ZK_D_BUCKETMAP_dyn"_Stz,       ZK_D_BUCKETMAP_dyn);
  /*  0 */ vAddConstant("ZK_I_CALLPIPELINE"_Stz,    ZK_I_CALLPIPELINE);
  /*  0 */ vAddConstant("ZK_I_COMMANDERRORS"_Stz,   ZK_I_COMMANDERRORS);
  /*  0 */ vAddConstant("ZK_M_CANCEL"_Stz,      ZK_M_CANCEL);
  /*  0 */ vAddConstant("ZK_M_CANCELLING"_Stz,    ZK_M_CANCELLING);
  /*  0 */ vAddConstant("ZK_M_CANCELLED"_Stz, ZK_M_CANCELLED);
  /*  0 */ vAddConstant("ZK_M_CLEANING"_Stz,   ZK_M_CLEANING);
  /*  0 */ vAddConstant("ZK_M_CLEANED"_Stz, ZK_M_CLEANED);
  /*  0 */ vAddConstant("ZK_I_COUNTINFO"_Stz,       ZK_I_COUNTINFO);
  /*  0 */ vAddConstant("ZK_D_CRWBUFFER"_Stz,       ZK_D_CRWBUFFER);
  /*  0 */ vAddConstant("ZK_D_CSVBUFFER"_Stz,       ZK_D_CSVBUFFER);
  /*  0 */ vAddConstant("ZK_M_DATAPULSE"_Stz,       ZK_M_DATAPULSE);
  /*  0 */ vAddConstant("ZK_M_DESTROY"_Stz,   ZK_M_DESTROY);
  /*  0 */ vAddConstant("ZK_EACHROWMARKER"_Stz,   ZK_D_EACHROWBUFFER);
  /*  0 */ vAddConstant("ZK_M_EACHSCANEOS"_Stz,ZK_M_EACHSCANEOS);
  /*  0 */ vAddConstant("ZK_D_EACHROWBUFFER"_Stz,   ZK_D_EACHROWBUFFER);
  /*  0 */ vAddConstant("ZK_M_EOS"_Stz,       ZK_M_EOS);
  /*  0 */ vAddConstant("ZK_M_END"_Stz,       ZK_M_END);
  /*  0 */ vAddConstant("ZK_M_ENDED"_Stz,     ZK_M_ENDED);
  /*  0 */ vAddConstant("ZK_M_ENDING"_Stz,    ZK_M_ENDING);
  /*  0 */ vAddConstant("ZK_M_ENDCRWRHT"_Stz,ZK_M_ENDCRWRHT);
  /*  0 */ vAddConstant("ZK_M_ENDROWRHT"_Stz,ZK_M_ENDROWRHT);
  /*  0 */ vAddConstant("ZK_D_ENTBUFFER"_Stz,       ZK_D_ENTBUFFER);
  /*  0 */ vAddConstant("ZK_M_ERROR"_Stz,     ZK_M_ERROR);
  /*  0 */ vAddConstant("ZK_M_ERRORED"_Stz,   ZK_M_ERRORED);
  /*  0 */ vAddConstant("ZK_M_ERRORING"_Stz,  ZK_M_ERRORING);
  /*  0 */ vAddConstant("ZK_I_EXECERRORS"_Stz,      ZK_I_EXECERRORS);
  /*  0 */ vAddConstant("ZK_M_EXITING"_Stz,      ZK_M_EXITING);
  /*  0 */ vAddConstant("ZK_M_EXITED"_Stz,    ZK_M_EXITED);
  /*  0 */ vAddConstant("ZK_M_FLUSH"_Stz,    ZK_M_FLUSH);
  /*  0 */ vAddConstant("ZK_D_GRID"_Stz,            ZK_D_GRID);
  /*  0 */ vAddConstant("ZK_D_INDEX"_Stz,           ZK_D_INDEX);
  /*  0 */ vAddConstant("ZK_D_INDEXDEF"_Stz,        ZK_D_INDEXDEF);
  /*  0 */ vAddConstant("ZK_D_JOB"_Stz,       ZK_D_JOB);
  /*  0 */ vAddConstant("ZK_M_LEAVE"_Stz,     ZK_M_LEAVE);
  /*  0 */ vAddConstant("ZK_M_LEAVING"_Stz,  ZK_M_LEAVING);
  /*  0 */ vAddConstant("ZK_M_LEFT"_Stz,     ZK_M_LEFT);

  /*  0 */ vAddConstant("ZK_M_LEFTOUTER_CRWLFT"_Stz, ZK_M_LEFTOUTER_CRWLFT);
  /*  0 */ vAddConstant("ZK_M_LEFTOUTER_ROWLFT"_Stz, ZK_M_LEFTOUTER_ROWLFT);
  /*  0 */ vAddConstant("ZK_I_LOADRESULT"_Stz,      ZK_I_LOADRESULT);
  /*  0 */ vAddConstant("ZK_I_LOCKSTATS"_Stz,       ZK_I_LOCKSTATS);
  /*  0 */ vAddConstant("ZK_D_METAGET"_Stz,         ZK_D_METAGET);
  /*  0 */ vAddConstant("ZK_D_NAMEDBUFFER"_Stz,     ZK_D_NAMEDBUFFER);
  /*  0 */ vAddConstant("ZK_M_NEXTBUFFER"_Stz,      ZK_M_NEXTBUFFER);
  /*  0 */ vAddConstant("ZK_I_NODEINFO"_Stz,        ZK_I_NODEINFO);
  /*  0 */ vAddConstant("ZK_D_OP"_Stz,              ZK_D_OP);
  /*  0 */ vAddConstant("ZK_D_OTHEROPERAND"_Stz,    ZK_D_OTHEROPERAND);
  /*  0 */ vAddConstant("ZK_D_PARCEL"_Stz,          ZK_D_PARCEL);
  /*  0 */ vAddConstant("ZK_I_PATHINFO"_Stz,        ZK_I_PATHINFO);
  /*  0 */ vAddConstant("ZK_D_PLANOPERAND"_Stz,     ZK_D_PLANOPERAND);
  /*  0 */ vAddConstant("ZK_D_PROFILE"_Stz,         ZK_D_PROFILE);
  /*  0 */ vAddConstant("ZK_I_PROGRESSUNITS"_Stz,   ZK_I_PROGRESSUNITS);
  /*  0 */ vAddConstant("ZK_I_PROMPTOUT"_Stz,       ZK_I_PROMPTOUT);
  /*  0 */ vAddConstant("ZK_I_RETURNPIPELINE"_Stz,  ZK_I_RETURNPIPELINE);
  /*  0 */ vAddConstant("ZK_D_ROWBUFFER"_Stz,       ZK_D_ROWBUFFER);
  /*  0 */ vAddConstant("ZK_I_SEEKOFFSETS"_Stz,     ZK_I_SEEKOFFSETS);
  /*  0 */ vAddConstant("ZK_D_SRTBUFFER"_Stz,       ZK_D_SRTBUFFER);
  /*  0 */ vAddConstant("ZK_M_STARTCRWRHT"_Stz,     ZK_M_STARTCRWRHT);
  /*  0 */ vAddConstant("ZK_M_START"_Stz,           ZK_M_START);
  /*  0 */ vAddConstant("ZK_M_STARTROWRHT"_Stz,     ZK_M_STARTROWRHT);
  /*  0 */ vAddConstant("ZK_M_STARTED"_Stz,         ZK_M_STARTED);
  /*  0 */ vAddConstant("ZK_D_STREAMDEF"_Stz,       ZK_D_STREAMDEF);
  /*  0 */ vAddConstant("ZK_D_STREAMDEFRHT"_Stz,    ZK_D_STREAMDEFRHT);
  /*  0 */ vAddConstant("ZK_M_STREAMDEFRHT"_Stz,    ZK_M_STREAMDEFRHT);
  /*  0 */ vAddConstant("ZK_D_STREAMSET"_Stz,       ZK_D_STREAMSET);
  /*  0 */ vAddConstant("ZK_D_TABLE"_Stz,           ZK_D_TABLE);
  /*  0 */ vAddConstant("ZK_D_TABLEPTN"_Stz,        ZK_D_TABLEPTN);
  /*  0 */ vAddConstant("ZK_I_TOTALUNITS"_Stz,      ZK_I_TOTALUNITS);
  /*  0 */ vAddConstant("ZK_M_TRIGGER"_Stz,         ZK_M_TRIGGER);
  /*  0 */ vAddConstant("ZK_D_JOBMETRICS"_Stz,      ZK_D_JOBMETRICS);
  /*  0 */ vAddConstant("ZK_ENUMS"_Stz,             ZK_ENUMS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
  #ifdef WITH_APPLE
  vAddConstant("WITH_APPLE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_APPLE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_ANALYSE_FREE
  vAddConstant("WITH_ANALYSE_FREE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ANALYSE_FREE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_ASSERT_MACRO
  vAddConstant("WITH_ASSERT_MACRO"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ASSERT_MACRO"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_BUCKET_HASHING
  vAddConstant("WITH_BUCKET_HASHING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_BUCKET_HASHING"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_CHECK_INITIALISED
  vAddConstant("WITH_CHECK_INITIALISED"_Stz,(i8)true);
  #else
  vAddConstant("WITH_CHECK_INITIALISED"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_CORRECTNESS_ALGEBRA
  vAddConstant("WITH_CORRECTNESS_ALGEBRA"_Stz,(i8)true);
  #else
  vAddConstant("WITH_CORRECTNESS_ALGEBRA"_Stz,(i8)false);
  #endif
          
  #ifdef WITH_DEBUG_VERSION
  vAddConstant("WITH_DEBUG_VERSION"_Stz,(i8)true);
  #else
  vAddConstant("WITH_DEBUG_VERSION"_Stz,(i8)false);
  #endif
        
  #ifdef WITH_TRACE_ENCODING
  vAddConstant("WITH_TRACE_ENCODING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_ENCODING"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_STEP_TIMING
  vAddConstant("WITH_STEP_TIMING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_STEP_TIMING"_Stz,(i8)false);
  #endif
      
  #ifdef WITH_TRACE_INFLATE
  vAddConstant("WITH_TRACE_INFLATE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_INFLATE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_TRACE_INDEX_COMPACTION
  vAddConstant("WITH_TRACE_INDEX_COMPACTION"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_INDEX_COMPACTION"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_LINUX
  vAddConstant("WITH_LINUX"_Stz,(i8)true);
  #else
  vAddConstant("WITH_LINUX"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_LOCK_TRACKING
  vAddConstant("WITH_LOCK_TRACKING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_LOCK_TRACKING"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_MUNGED_SEN
  vAddConstant("WITH_MUNGED_SEN"_Stz,(i8)true);
  #else
  vAddConstant("WITH_MUNGED_SEN"_Stz,(i8)false);
  #endif
            
  #ifdef WITH_PYTHON_SUPPORT
  vAddConstant("WITH_PYTHON_SUPPORT"_Stz,(i8)true);
  #else
  vAddConstant("WITH_PYTHON_SUPPORT"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_RELEASE_VERSION
  vAddConstant("WITH_RELEASE_VERSION"_Stz,(i8)true);
  #else
  vAddConstant("WITH_RELEASE_VERSION"_Stz,(i8)false);
  #endif
                    
  #ifdef WITH_STATISTICS_COUNTERS
  vAddConstant("WITH_STATISTICS_COUNTERS"_Stz,(i8)true);
  #else
  vAddConstant("WITH_STATISTICS_COUNTERS"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_TRACE_API
  vAddConstant("WITH_TRACE_API"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_API"_Stz,(i8)false);
  #endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
