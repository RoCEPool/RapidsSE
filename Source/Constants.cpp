/*============================================================================*/
/* Constants.cpp -- add constants into main dictionary at startup             */
/*============================================================================*/

#include "StorageEngine.h"

extern Args  gxArgs;

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
  Word* pNewWord = new(AM::pRootDic()) Word(szName);
  
  pNewWord->vCategory(AM::pRootConstant());
  
  Datum xVar;
  xVar = iVal;

  pNewWord->vEncodePush(xVar);
  pNewWord->vEncodeCall("exit_rt"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstant(Stz* szName, Stz* szVal)
{
  Word* pNewWord = new(AM::pRootDic()) Word(szName);
  
  Datum xVar;
  xVar = szVal;
  
  pNewWord->vEncodePush(xVar);
  pNewWord->vEncodeCall("exit_rt"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add arguments as forth constants                                           */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstantsArgs(void)
{
  // Args
  Constants::vAddConstant("args.addNode"_Stz,           gxArgs.m_bAddNode);
  Constants::vAddConstant("args.config"_Stz,            gxArgs.m_bConfig);
  Constants::vAddConstant("args.developer"_Stz,         gxArgs.m_bDeveloper);
  Constants::vAddConstant("args.dontFree"_Stz,          gxArgs.m_bDontFree);
  Constants::vAddConstant("args.freeCB"_Stz,            gxArgs.m_bFreeCB);
  Constants::vAddConstant("args.help"_Stz,              gxArgs.m_bHelp);
  Constants::vAddConstant("args.hugePages"_Stz,         gxArgs.m_bHugePages);
  Constants::vAddConstant("args.killPrev"_Stz,          gxArgs.m_bKillPrev);
  Constants::vAddConstant("args.lock"_Stz,              gxArgs.m_bLock);
  Constants::vAddConstant("args.log"_Stz,               gxArgs.m_bLog);
  Constants::vAddConstant("args.maxLoadErrors"_Stz,     gxArgs.m_nMaxLoadErrors);
  Constants::vAddConstant("args.maxReadyLP"_Stz,        gxArgs.m_nMaxReadyLP);
  Constants::vAddConstant("args.metadata"_Stz,          gxArgs.m_bMetadata);
  Constants::vAddConstant("args.munged"_Stz,            gxArgs.m_bMunged);
  Constants::vAddConstant("args.nodeName"_Stz,          gxArgs.m_sNodeName);
  Constants::vAddConstant("args.nodes"_Stz,             gxArgs.m_nNodes);
  Constants::vAddConstant("args.partitionsPerNode"_Stz, gxArgs.m_nSlotsPerNode);
  Constants::vAddConstant("args.periodicSleep"_Stz,     gxArgs.m_nPeriodicSleep);
  Constants::vAddConstant("args.progress"_Stz,          gxArgs.m_bProgress);
  Constants::vAddConstant("args.prompt"_Stz,            gxArgs.m_bPrompt);
  Constants::vAddConstant("args.regionPrefix"_Stz,      gxArgs.m_sRegionPrefix);
  Constants::vAddConstant("args.regionName"_Stz,        gxArgs.m_sRegionName);
  Constants::vAddConstant("args.regionSize"_Stz,        gxArgs.m_iRegionSize);
  Constants::vAddConstant("args.reload"_Stz,            gxArgs.m_bReload);
  Constants::vAddConstant("args.restart"_Stz,           gxArgs.m_bRestart);
  Constants::vAddConstant("args.senDir"_Stz,            gxArgs.m_sSenDir);
  Constants::vAddConstant("args.slots"_Stz,             gxArgs.m_nSlotsPerNode);
  Constants::vAddConstant("args.start"_Stz,             gxArgs.m_bStart);
  Constants::vAddConstant("args.status"_Stz,            gxArgs.m_bStatus);
  Constants::vAddConstant("args.stop"_Stz,              gxArgs.m_bStop);
  Constants::vAddConstant("args.subIngesters"_Stz,      gxArgs.m_nSubIngesters);
  Constants::vAddConstant("args.subLoaders"_Stz,        gxArgs.m_nSubLoaders);
  Constants::vAddConstant("args.supervisorSleep"_Stz,   gxArgs.m_nSupervisorSleep);
  Constants::vAddConstant("args.threads"_Stz,           gxArgs.m_nThreads);
  Constants::vAddConstant("args.touch"_Stz,             gxArgs.m_bTouch);
  Constants::vAddConstant("args.tracepoint"_Stz,        gxArgs.m_bTracepoint);
  Constants::vAddConstant("args.tracing"_Stz,           gxArgs.m_bTracing);
  Constants::vAddConstant("args.unlink"_Stz,            gxArgs.m_bUnlink);
  Constants::vAddConstant("args.verbose"_Stz,           gxArgs.m_bVerbose);
  Constants::vAddConstant("args.welcome"_Stz,           gxArgs.m_bWelcome);
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Constants::vAddConstantsCpp(void)
{
  if (AM::bVerboseProgress())
    vTrace("[[mainThread]] Adding constants...\n"_Stz);
  
  // true / flase
  vAddConstant("true"_Stz,    (i8)1);
  vAddConstant("false"_Stz,   (i8)0);
  
  vAddConstant("RAPIDSSE_VERSION_STRING"_Stz, (Stz*)RAPIDSSE_VERSION_STRING);
  
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

  vAddConstant("C_MAX_INDEXES"_Stz,       C_MAX_INDEXES);
  vAddConstant("C_MAX_SLOTS"_Stz,         C_MAX_SLOTS);
  vAddConstant("C_MAX_NODES"_Stz,         C_MAX_NODES);
  vAddConstant("C_MAX_PARTITIONS"_Stz,    C_MAX_PARTITIONS);

  vAddConstant("C_LOW_BUFFERS"_Stz,       C_LOW_BUFFERS);
  vAddConstant("C_MAX_BCD_PRECISION"_Stz, C_MAX_BCD_PRECISION);
  vAddConstant("C_MAX_COLUMNS"_Stz,       C_MAX_COLUMNS);
  vAddConstant("C_MAX_RECORD"_Stz,        C_MAX_RECORD);
  vAddConstant("C_MAX_PATH"_Stz,          C_MAX_PATH);
  vAddConstant("C_SIZE_POOL_BUFFER"_Stz,  C_SIZE_POOL_BUFFER);
  
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
  vAddConstant("C_TDEF_ANALYZEINDEX"_Stz, C_TDEF_ANALYZEINDEX);
  vAddConstant("C_TDEF_ANALYZETABLE"_Stz, C_TDEF_ANALYZETABLE);
  vAddConstant("C_TDEF_GRIDSANITY"_Stz,   C_TDEF_GRIDSANITY);
  vAddConstant("C_TDEF_LISTCOLUMNS"_Stz,  C_TDEF_LISTCOLUMNS);
  vAddConstant("C_TDEF_LISTCONTENTS"_Stz, C_TDEF_LISTCONTENTS);
  vAddConstant("C_TDEF_LISTFOLDER"_Stz,   C_TDEF_LISTFOLDER);
  vAddConstant("C_TDEF_LISTINDEX"_Stz,    C_TDEF_LISTINDEX);
  vAddConstant("C_TDEF_LISTPARTITIONS"_Stz,C_TDEF_LISTPARTITIONS);
  vAddConstant("C_TDEF_LOCKSTATS"_Stz,    C_TDEF_LOCKSTATS);
  vAddConstant("C_TDEF_NODEINFO"_Stz,     C_TDEF_NODEINFO);
  vAddConstant("C_TDEF_PINGINFO"_Stz,     C_TDEF_PINGINFO);
  vAddConstant("C_TDEF_PROGRESSUNITS"_Stz,C_TDEF_PROGRESSUNITS);
  vAddConstant("C_TDEF_PWF"_Stz,          C_TDEF_PWF);
  vAddConstant("C_TDEF_SEEKOFFSETS"_Stz,  C_TDEF_SEEKOFFSETS);
  vAddConstant("C_TDEF_SHOWCLUSTER"_Stz,  C_TDEF_SHOWCLUSTER);
  vAddConstant("C_TDEF_SIZEINDEX"_Stz,    C_TDEF_SIZEINDEX);
  vAddConstant("C_TDEF_SIZETABLE"_Stz,    C_TDEF_SIZETABLE);
  vAddConstant("C_TDEF_SPACE"_Stz,        C_TDEF_SPACE);
  vAddConstant("C_TDEF_STATUS"_Stz,       C_TDEF_STATUS);
  vAddConstant("C_TDEF_TABLESIZES"_Stz,   C_TDEF_TABLESIZES);
  vAddConstant("C_TDEF_TOTALUNITS"_Stz,   C_TDEF_TOTALUNITS);
  vAddConstant("C_TDEF_VERIFYINDEX"_Stz,  C_TDEF_VERIFYINDEX);
  vAddConstant("C_TDEF_VERIFYTABLE"_Stz,  C_TDEF_VERIFYTABLE);

  // hardware
  vAddConstant("HW_CLOCK_FREQUENCY"_Stz, TickingDevice::iTicksPerSec());
  vAddConstant("HW_CORE_COUNT"_Stz,      Hardware::iCoreCount());
  vAddConstant("HW_CPU_COUNT"_Stz,       Hardware::iCpuCount());
  vAddConstant("HW_PHYSICAL_MEMORY"_Stz, Hardware::iPhysicalMemory());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // AN_ : action numbers
  /*  0 */ vAddConstant("AN_NONE"_Stz,               AN_NONE);
  /*  0 */ vAddConstant("AN_CONSTRUCT"_Stz,          AN_CONSTRUCT);
  /*  0 */ vAddConstant("AN_DESTRUCT"_Stz,           AN_DESTRUCT);
  /*  0 */ vAddConstant("AN_ERROR_CODE"_Stz,         AN_ERROR_CODE);
  /*  0 */ vAddConstant("AN_JOB_BEGIN"_Stz,          AN_JOB_BEGIN);
  /*  0 */ vAddConstant("AN_JOB_CHUNK"_Stz,          AN_JOB_CHUNK);
  /*  0 */ vAddConstant("AN_JOB_END"_Stz,            AN_JOB_END);
  /*  0 */ vAddConstant("AN_JOB_PHASE"_Stz,          AN_JOB_PHASE);
  /*  0 */ vAddConstant("AN_NEW_JOB"_Stz,            AN_NEW_JOB);
  /*  0 */ vAddConstant("AN_POST_PROMPT"_Stz,        AN_POST_PROMPT);
  /*  0 */ vAddConstant("AN_POST_RECV"_Stz,          AN_POST_RECV);
  /*  0 */ vAddConstant("AN_PRE_PROMPT"_Stz,         AN_PRE_PROMPT);
  /*  0 */ vAddConstant("AN_PRE_SEND"_Stz,           AN_PRE_SEND);
  /*  0 */ vAddConstant("AN_RECV_JOB"_Stz,           AN_RECV_JOB);
  /*  0 */ vAddConstant("AN_RECV_JOB_LOW_MEMORY"_Stz,AN_RECV_JOB_LOW_MEMORY);
  /*  0 */ vAddConstant("AN_RESOURCES_CHECKED"_Stz,  AN_RESOURCES_CHECKED);
  /*  0 */ vAddConstant("AN_RESOURCES_RELEASED"_Stz, AN_RESOURCES_RELEASED);
  /*  0 */ vAddConstant("AN_SEED_CAN"_Stz,           AN_SEED_CAN);
  /*  0 */ vAddConstant("AN_SEED_END"_Stz,           AN_SEED_END);
  /*  0 */ vAddConstant("AN_SEED_EXE"_Stz,           AN_SEED_EXE);
  /*  0 */ vAddConstant("AN_SEED_RCV"_Stz,           AN_SEED_RCV);
  /*  0 */ vAddConstant("AN_SEED_REQ"_Stz,           AN_SEED_REQ);
  /*  0 */ vAddConstant("AN_SEED_RUN"_Stz,           AN_SEED_RUN);
  /*  0 */ vAddConstant("AN_SEND_JOB"_Stz,           AN_SEND_JOB);
  /*  0 */ vAddConstant("AN_SPAWN_JOB"_Stz,          AN_SPAWN_JOB);
  /*  0 */ vAddConstant("AN_ENUMS"_Stz,              AN_ENUMS);
  
  // AT_ : fetch techniques
  /*  0 */ vAddConstant("AT_NONE"_Stz,     AT_NONE);
  /*  1 */ vAddConstant("AT_EXACT"_Stz,    AT_EXACT);
  /*  2 */ vAddConstant("AT_LOWER"_Stz,    AT_LOWER);
  /*  3 */ vAddConstant("AT_UPPER"_Stz,    AT_UPPER);
  /*  4 */ vAddConstant("AT_RANGE"_Stz,    AT_RANGE);
  /*  5 */ vAddConstant("AT_SCAN"_Stz,     AT_SCAN);
  /*  6 */ vAddConstant("AT_ENUMS"_Stz,    AT_ENUMS);
  
  // CK_ : command kind
  /*  0 */ vAddConstant("CK_NONE"_Stz,     CK_NONE);
  /*  0 */ vAddConstant("CK_COMMAND"_Stz,  CK_COMMAND);
  /*  0 */ vAddConstant("CK_CHILD"_Stz,    CK_CHILD);
  /*  0 */ vAddConstant("CK_SERVICE"_Stz,  CK_SERVICE);

  // CN_ : class numbers
  /*  0 */ vAddConstant("CN_NONE"_Stz,         CN_NONE);

  /*  8 */ vAddConstant("CN_STZVAL"_Stz,       CN_STZVAL);
  
  /* 12 */ vAddConstant("CN_ARGUMENTS"_Stz,    CN_ARGUMENTS);
  /* 13 */ vAddConstant("CN_ATOMIC"_Stz,       CN_ATOMIC);
  /* 14 */ vAddConstant("CN_ATTACHMAP"_Stz,    CN_ATTACHMAP);
  
  /*    */ vAddConstant("CN_BIRTH"_Stz,        CN_BIRTH);
  /* 17 */ vAddConstant("CN_BPA"_Stz,          CN_BPA);
  /*    */ vAddConstant("CN_BUFFERCURSOR"_Stz, CN_BUFFERCURSOR);
  /* 16 */ vAddConstant("CN_BUFFERPOOL"_Stz,   CN_BUFFERPOOL);
  /* 16 */ vAddConstant("CN_BYTERATE"_Stz,     CN_BYTERATE);

  /*    */ vAddConstant("CN_CRVBUFFER"_Stz,    CN_CRVBUFFER);
  /*    */ vAddConstant("CN_CRWBUFFER"_Stz,    CN_CRWBUFFER);
  /*    */ vAddConstant("CN_CSVBUFFER"_Stz,    CN_CSVBUFFER);
  
  /* 18 */ vAddConstant("CN_DIC"_Stz,          CN_DIC);
  /* 19 */ vAddConstant("CN_DICSTACK"_Stz,     CN_DICSTACK);
  /* 19 */ vAddConstant("CN_PIPELINE"_Stz,     CN_PIPELINE);
  /* 20 */ vAddConstant("CN_DIR"_Stz,          CN_DIR);
  
  /*    */ vAddConstant("CN_FETCHBUFFER"_Stz,  CN_FETCHBUFFER);
  /* 21 */ vAddConstant("CN_FIFO"_Stz,         CN_FIFO);
  /* 21 */ vAddConstant("CN_FILE"_Stz,         CN_FILE);
  /*  5 */ vAddConstant("CN_FIPVAL"_Stz,       CN_FIPVAL);
  /* 15 */ vAddConstant("CN_FREEBUFFER"_Stz,   CN_FREEBUFFER);
  /* 22 */ vAddConstant("CN_FREECHUNK"_Stz,    CN_FREECHUNK);
  /*    */ vAddConstant("CN_FREQDIST"_Stz,     CN_FREQDIST);

  /* 26 */ vAddConstant("CN_GRID"_Stz,         CN_GRID);
  
  /* 27 */ vAddConstant("CN_HASHBUFFER"_Stz,   CN_HASHBUFFER);
  /* 27 */ vAddConstant("CN_HASHMAP"_Stz,      CN_HASHMAP);
  /* 27 */ vAddConstant("CN_HEAPBUFFER"_Stz,   CN_HEAPBUFFER);
  
  /* 28 */ vAddConstant("CN_INDEX"_Stz,        CN_INDEX);
  /*    */ //vAddConstant("CN_INDEXDEFN"_Stz,    CN_INDEXDEFN);
  /*  4 */ vAddConstant("CN_INTVAL"_Stz,       CN_INTVAL);

  /*    */ vAddConstant("CN_JOB_BOOT"_Stz,           CN_JOB_BOOT);
  /*    */ vAddConstant("CN_JOB_DISTRIBUTED"_Stz,    CN_JOB_DISTRIBUTED);
  /*    */ vAddConstant("CN_JOB_LOCAL"_Stz,          CN_JOB_LOCAL);
  /*    */ vAddConstant("CN_JOB_OPERAND"_Stz,        CN_JOB_OPERAND);
  /*    */ vAddConstant("CN_JOB_START"_Stz,          CN_JOB_START);
  
  /* 31 */ vAddConstant("CN_ENTWRK"_Stz,       CN_ENTWRK);
  
  /*    */ vAddConstant("CN_LEXICON"_Stz,      CN_LEXICON);
  /*    */ vAddConstant("CN_LOADCHUNK"_Stz,    CN_LOADCHUNK);
  /*    */ vAddConstant("CN_LOADER"_Stz,       CN_LOADER);
  /*    */ vAddConstant("CN_LOCAL"_Stz,        CN_LOCAL);
  /* 47 */ vAddConstant("CN_LOCALSCHAIN"_Stz,     CN_LOCALSCHAIN);
  /* 36 */ vAddConstant("CN_LOCK"_Stz,         CN_LOCK);
  
  /*    */ vAddConstant("CN_MARKER"_Stz,           CN_MARKER);
  /*    */ vAddConstant("CN_MEDIUMPERSISTENT"_Stz, CN_MEDIUMPERSISTENT);
  /*    */ vAddConstant("CN_MEDIUMTRANSIENT"_Stz,  CN_MEDIUMTRANSIENT);
  /*    */ vAddConstant("CN_MEMTEST"_Stz,      CN_MEMTEST);
  /* 60 */ vAddConstant("CN_MUTEX"_Stz,        CN_MUTEX);

  /* 38 */ vAddConstant("CN_NODE"_Stz,         CN_NODE);
  /*    */ vAddConstant("CN_NOW"_Stz,          CN_NOW);

  /* 40 */ vAddConstant("CN_CHARSER"_Stz,      CN_CHARSER);
  /*    */ vAddConstant("CN_PLAN"_Stz,         CN_PLAN);
  /*  7 */ vAddConstant("CN_PTRVAL"_Stz,       CN_PTRVAL);

  /* 45 */ vAddConstant("CN_QUEUE"_Stz,        CN_QUEUE);
  /* 46 */ vAddConstant("CN_QUEUEBUFFER"_Stz,  CN_QUEUEBUFFER);
  /*    */ vAddConstant("CN_QUEUES"_Stz,       CN_QUEUES);
  
  /*    */ vAddConstant("CN_READAPI"_Stz,      CN_READAPI);
  /*    */ vAddConstant("CN_RECORD"_Stz,       CN_RECORD);
  /*    */ vAddConstant("CN_RECORDBUFFER"_Stz, CN_RECORDBUFFER);
  /*    */ vAddConstant("CN_LOADCURSOR"_Stz,   CN_LOADCURSOR);
  /*    */ vAddConstant("CN_RESPONSE"_Stz,     CN_RESPONSE);
  /*    */ vAddConstant("CN_RESPONSEVEC"_Stz,  CN_RESPONSEVEC);
  /*    */ vAddConstant("CN_RESULT"_Stz,       CN_RESULT);

  /* 57 */ vAddConstant("CN_SESSION"_Stz,      CN_SESSION);
  /*    */ vAddConstant("CN_SESSIONARRAY"_Stz, CN_SESSIONARRAY);
  /* 60 */ vAddConstant("CN_SEPROMPT"_Stz,     CN_SEPROMPT);
  /* 61 */ vAddConstant("CN_SHORTCUT"_Stz,     CN_SHORTCUT);
  /* 61 */ vAddConstant("CN_SHORTCUTVEC"_Stz,  CN_SHORTCUTVEC);
  /* 61 */ vAddConstant("CN_SINCE"_Stz,        CN_SINCE);
  /* 63 */ vAddConstant("CN_SKIPLIST"_Stz,     CN_SKIPLIST);
  /* 64 */ vAddConstant("CN_SKIPNODE"_Stz,     CN_SKIPNODE);
  /*    */ vAddConstant("CN_SMALLPERSISTENT"_Stz, CN_SMALLPERSISTENT);
  /* 65 */ vAddConstant("CN_SMALLTRANSIENT"_Stz, CN_SMALLTRANSIENT);
  /* 66 */ vAddConstant("CN_SOCKET"_Stz,       CN_SOCKET);
  /* 67 */ vAddConstant("CN_STACK"_Stz,        CN_STACK);
  /* 68 */ vAddConstant("CN_STATISTICS"_Stz,   CN_STATISTICS);
  /* 68 */ vAddConstant("CN_STEPACCUM"_Stz,    CN_STEPACCUM);
  /* 69 */ vAddConstant("CN_STEPTIMING"_Stz,   CN_STEPTIMING);
  /* 69 */ vAddConstant("CN_STMT_SELECT"_Stz,  CN_STMT_SELECT);
  /*  1 */ vAddConstant("CN_STREAMDEF"_Stz,    CN_STREAMDEF);
  /*  1 */ vAddConstant("CN_STREAMSET"_Stz,    CN_STREAMSET);
  /*  1 */ vAddConstant("CN_STRING_T"_Stz,     CN_STRING_T);
  /* 70 */ vAddConstant("CN_SYSCALL"_Stz,      CN_SYSCALL);

  /* 71 */ vAddConstant("CN_TABLE"_Stz,        CN_TABLE);
  /* 41 */ vAddConstant("CN_TABLEPTN"_Stz,     CN_TABLEPTN);
  /* 72 */ vAddConstant("CN_TASK"_Stz,         CN_TASK);
  /*    */ vAddConstant("CN_TASKLIST"_Stz,     CN_TASKLIST);
  /*    */ vAddConstant("CN_TASKQUEUE"_Stz,    CN_TASKQUEUE);
  /*    */ vAddConstant("CN_TEMPNAME"_Stz,     CN_TEMPNAME);
  /* 76 */ vAddConstant("CN_THREAD"_Stz,       CN_THREAD);
  /* 80 */ vAddConstant("CN_TIMER"_Stz,        CN_TIMER);
  /* 81 */ vAddConstant("CN_INPUT"_Stz,    CN_INPUT);
  /* 82 */ vAddConstant("CN_TX"_Stz,           CN_TX);
  /* 87 */ vAddConstant("CN_TXSET"_Stz,        CN_TXSET);
  
  /* 88 */ vAddConstant("CN_WORD"_Stz,         CN_WORD);
   
  /* 93 */ vAddConstant("CN_ENUMS"_Stz,        CN_ENUMS);
 
  // CT_ : columne type
  /*  0 */ vAddConstant("CT_NONE"_Stz,      CT_NONE);
  /*  1 */ vAddConstant("CT_BOOLEAN"_Stz,   CT_BOOLEAN);
  /*  2 */ vAddConstant("CT_CHAR"_Stz,      CT_CHAR);
  /*  3 */ vAddConstant("CT_DATE"_Stz,      CT_DATE);
  /*  4 */ vAddConstant("CT_DECIMAL"_Stz,   CT_DECIMAL);
  /*  5 */ vAddConstant("CT_FLOAT"_Stz,     CT_FLOAT);
  /*  6 */ vAddConstant("CT_INTEGER"_Stz,   CT_INTEGER);
  /*  7 */ vAddConstant("CT_INTERVAL"_Stz,  CT_INTERVAL);
  /*  8 */ vAddConstant("CT_TIME"_Stz,      CT_TIME);
  /*  9 */ vAddConstant("CT_TIMESTAMP"_Stz, CT_TIMESTAMP);
  /* 10 */ vAddConstant("CT_VARCHAR"_Stz,   CT_VARCHAR);
  /* 11 */ vAddConstant("CT_ENUMS"_Stz,     CT_ENUMS);

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
  /*  7 */ vAddConstant("DM_TME"_Stz, DM_TME);
  /*  8 */ vAddConstant("DM_TSP"_Stz, DM_TSP);
  
  // EC_ : error codes
  /*  0 */ vAddConstant("EC_NONE"_Stz,               EC_NONE);
  /*  1 */ vAddConstant("EC_ASSERT"_Stz,             EC_ASSERT);
  
  /*    */ vAddConstant("EC_CATALOG_EXISTS"_Stz,     EC_CATALOG_EXISTS);
  /*  4 */ vAddConstant("EC_CLUSTER_NOT_CONNECTED"_Stz,EC_CLUSTER_NOT_CONNECTED);
  /*  5 */ vAddConstant("EC_CLUSTER_PARTITIONS_MISMATCH"_Stz,EC_CLUSTER_PARTITIONS_MISMATCH);
  /*  6 */ vAddConstant("EC_CLUSTER_SIZE_MISMATCH"_Stz,      EC_CLUSTER_SIZE_MISMATCH);
  /*  7 */ vAddConstant("EC_DUPLICATE_COLUMN"_Stz,   EC_DUPLICATE_COLUMN);
  /*  8 */ vAddConstant("EC_DUPLICATE_KEY"_Stz,      EC_DUPLICATE_KEY);
  /*  9 */ vAddConstant("EC_FOLDER_IN_USE"_Stz,      EC_FOLDER_IN_USE);
  /* 10 */ vAddConstant("EC_FOLDER_NOT_EMPTY"_Stz,   EC_FOLDER_NOT_EMPTY);
  /* 11 */ vAddConstant("EC_INDEX_EXISTS"_Stz,       EC_INDEX_EXISTS);
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
  
  /* 50 */ vAddConstant("EC_RESPONSE_PENDING"_Stz,   EC_RESPONSE_PENDING);
  
  /* 51 */ vAddConstant("EC_SCHEMA_EXISTS"_Stz,      EC_SCHEMA_EXISTS);
  /* 52 */ vAddConstant("EC_SE_NOT_RUNNING"_Stz,     EC_SE_NOT_RUNNING);
  /* 53 */ vAddConstant("EC_SE_UNKNOWN_ERROR"_Stz,   EC_SE_UNKNOWN_ERROR);
  /* 54 */ vAddConstant("EC_SEND_FAILURE"_Stz,       EC_SEND_FAILURE);
  
  /*    */ vAddConstant("EC_TABLE_ALREADY_LOADED"_Stz, EC_TABLE_ALREADY_LOADED);
  /* 55 */ vAddConstant("EC_TABLE_EXISTS"_Stz,       EC_TABLE_EXISTS);
  /* 56 */ vAddConstant("EC_TABLE_IN_USE"_Stz,       EC_TABLE_IN_USE);
  /* 57 */ vAddConstant("EC_TABLE_NOT_EMPTY"_Stz,    EC_TABLE_NOT_EMPTY);
  /* 58 */ vAddConstant("EC_TENANT_EXISTS"_Stz,      EC_TENANT_EXISTS);
  
  /*    */ vAddConstant("EC_TX_BEGIN_FAILED"_Stz,    EC_TX_BEGIN_FAILED);
  /*    */ vAddConstant("EC_TX_COMMIT_FAILED"_Stz,   EC_TX_COMMIT_FAILED);
  /*    */ vAddConstant("EC_TX_END_FAILED"_Stz,      EC_TX_END_FAILED);
  /*    */ vAddConstant("EC_TX_ROLLBACK_FAILED"_Stz, EC_TX_ROLLBACK_FAILED);
  
  /* 59 */ vAddConstant("EC_VALIDATION_FAILURE"_Stz, EC_VALIDATION_FAILURE);
  /* 60 */ vAddConstant("EC_VERSION_MISMATCH"_Stz,   EC_VERSION_MISMATCH);
  /* 61 */ vAddConstant("EC_ENUMS"_Stz,              EC_ENUMS);

  
  // JK_ : job kind
  /*  0 */ vAddConstant("JK_NONE"_Stz,      JK_NONE);
  /*  1 */ vAddConstant("JK_BOOT"_Stz,      JK_BOOT);
  /*  2 */ vAddConstant("JK_CHILD"_Stz,     JK_CHILD);
  /*  3 */ vAddConstant("JK_DIST2PC"_Stz,   JK_DIST2PC);
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
  
  // NS_ : node state
  /*  0 */ vAddConstant("NS_NONE"_Stz,      NS_NONE);
  /*  0 */ vAddConstant("NS_ACTIVE"_Stz,    NS_ACTIVE);
  /*  0 */ vAddConstant("NS_INACTIVE"_Stz,  NS_INACTIVE);

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
  /*  0 */ vAddConstant("RS_OLD_JOB_RESPONSE"_Stz,     RS_OLD_JOB_RESPONSE);

  // SS_ : seed strings
  /*  0 */ vAddConstant("SS_SEED_RUN"_Stz,    SS_SEED_RUN);
  /*  1 */ vAddConstant("SS_SEED_REQ"_Stz,    SS_SEED_REQ);
  /*  2 */ vAddConstant("SS_SEED_RCV"_Stz,    SS_SEED_RCV);
  /*  3 */ vAddConstant("SS_SEED_EXE"_Stz,    SS_SEED_EXE);
  /*  4 */ vAddConstant("SS_SEED_END"_Stz,    SS_SEED_END);
  /*  5 */ vAddConstant("SS_SEED_CAN"_Stz,    SS_SEED_CAN);
  /*  6 */ vAddConstant("SS_SEED_BEG"_Stz,    SS_SEED_BEG);
  /*  7 */ vAddConstant("SS_SEED_KID"_Stz,    SS_SEED_KID);
  
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
  
  // WK_ : word type
  /*  0 */ vAddConstant("WK_NONE"_Stz,       WK_NONE);
  /*  0 */ vAddConstant("WK_CPPOBJECT"_Stz,  WK_CPPOBJECT);
  /*  0 */ vAddConstant("WK_INTRINSIC"_Stz,  WK_INTRINSIC);
  /*  0 */ vAddConstant("WK_LOCAL"_Stz,      WK_LOCAL);
  /*  0 */ vAddConstant("WK_ROUTINE"_Stz,    WK_ROUTINE);
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
  /*  0 */ vAddConstant("ZK_CANCELMARKER"_Stz,    ZK_CANCELMARKER);
  /*  0 */ vAddConstant("ZK_CANCELLEDMARKER"_Stz, ZK_CANCELLEDMARKER);
  /*  0 */ vAddConstant("ZK_CRWBUFFER"_Stz,       ZK_CRWBUFFER);
  /*  0 */ vAddConstant("ZK_CSVBUFFER"_Stz,       ZK_CSVBUFFER);
  /*  0 */ vAddConstant("ZK_EOSMARKER"_Stz,       ZK_EOSMARKER);
  /*  0 */ vAddConstant("ZK_ENTBUFFER"_Stz,       ZK_ENTBUFFER);
  /*  0 */ vAddConstant("ZK_ERRORMARKER"_Stz,     ZK_ERRORMARKER);
  /*  0 */ vAddConstant("ZK_ERROREDMARKER"_Stz,   ZK_ERROREDMARKER);
  /*  0 */ vAddConstant("ZK_EXITMARKER"_Stz,      ZK_EXITMARKER);
  /*  0 */ vAddConstant("ZK_EXITEDMARKER"_Stz,    ZK_EXITEDMARKER);
  /*  0 */ vAddConstant("ZK_IRWBUFFER"_Stz,       ZK_IRWBUFFER);
  /*  0 */ vAddConstant("ZK_LOCKSTATS"_Stz,       ZK_LOCKSTATS);
  /*  0 */ vAddConstant("ZK_NODEINFO"_Stz,        ZK_NODEINFO);
  /*  0 */ vAddConstant("ZK_PROGRESSUNITS"_Stz,   ZK_PROGRESSUNITS);
  /*  0 */ vAddConstant("ZK_PULSEMARKER"_Stz,     ZK_PULSEMARKER);
  /*  0 */ vAddConstant("ZK_PULSEDMARKER"_Stz,    ZK_PULSEDMARKER);
  /*  0 */ vAddConstant("ZK_RSBBUFFER"_Stz,       ZK_RSBBUFFER);
  /*  0 */ vAddConstant("ZK_SEEKOFFSETS"_Stz,     ZK_SEEKOFFSETS);
  /*  0 */ vAddConstant("ZK_SRTBUFFER"_Stz,       ZK_SRTBUFFER);
  /*  0 */ vAddConstant("ZK_STARTMARKER"_Stz,     ZK_STARTMARKER);
  /*  0 */ vAddConstant("ZK_STARTEDMARKER"_Stz,   ZK_STARTEDMARKER);
  /*  0 */ vAddConstant("ZK_STREAMDEF"_Stz,       ZK_STREAMDEF);
  /*  0 */ vAddConstant("ZK_STREAMSET"_Stz,       ZK_STREAMSET);
  /*  0 */ vAddConstant("ZK_TOTALUNITS"_Stz,      ZK_TOTALUNITS);
  /*  0 */ vAddConstant("ZK_ENUMS"_Stz,           ZK_ENUMS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // SIZEOF_ : C++ class sizes
  /*  1 */ vAddConstant("SIZEOF_INTVAL"_Stz, sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_IPVAL"_Stz,  sizeof(FipVal));
  /*  4 */ vAddConstant("SIZEOF_PTRVAL"_Stz, sizeof(PtrVal));
  /*  5 */ vAddConstant("SIZEOF_STZVAL"_Stz, sizeof(StzVal));
  /*  6 */ vAddConstant("SIZEOF_VALSET"_Stz, sizeof(Datum)); // correct
  /*  7 */ vAddConstant("SIZEOF_ValWrk"_Stz, sizeof(Datum)); // correct
  
  /*  9 */ vAddConstant("SIZEOF_ATOMIC"_Stz,       sizeof(Atomic));
  /* 11 */ vAddConstant("SIZEOF_POOLBUFFER"_Stz,   sizeof(PoolBuffer));
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOL"_Stz,   sizeof(BufferPool));
  /* 13 */ vAddConstant("SIZEOF_BPA"_Stz,          sizeof(BPA));
  /* 14 */ vAddConstant("SIZEOF_DIC"_Stz,          sizeof(Dic));
  /* 15 */ vAddConstant("SIZEOF_DICSTACK"_Stz,     sizeof(DicStack));
  /* 16 */ vAddConstant("SIZEOF_DIR"_Stz,          sizeof(Dir));
  /*    */ vAddConstant("SIZEOF_EntBUFFER"_Stz,    sizeof(EntBuffer));
  /* 17 */ vAddConstant("SIZEOF_FILE"_Stz,         sizeof(File));
  /* 18 */ vAddConstant("SIZEOF_FREECHUNK"_Stz,    sizeof(FreeChunk));
  /* 19 */ vAddConstant("SIZEOF_GRID"_Stz,         sizeof(Grid));
  /* 20 */ vAddConstant("SIZEOF_HEAPBUFFER"_Stz,   sizeof(HeapBuffer));
  /*    */ vAddConstant("SIZEOF_INDEX"_Stz,        sizeof(Index));
  
  /* 24 */ vAddConstant("SIZEOF_LOCK"_Stz,         sizeof(Lock));
  /* 25 */ vAddConstant("SIZEOF_MEDIUMHEAP"_Stz,   sizeof(Heap));
  /*    */ vAddConstant("SIZEOF_MUTEX"_Stz,        sizeof(Mutex));
  /* 26 */ vAddConstant("SIZEOF_NODE"_Stz,         sizeof(Node));
  
  /* 28 */ vAddConstant("SIZEOF_PARSER"_Stz,       sizeof(Charser));
  /* 29 */ vAddConstant("SIZEOF_PARTITION"_Stz,    sizeof(TablePtn));
  /* 33 */ vAddConstant("SIZEOF_QUEUE"_Stz,        sizeof(Queue));
  /* 34 */ vAddConstant("SIZEOF_QUEUEBUFFER"_Stz,  sizeof(QueueBuffer));
  /* 36 */ vAddConstant("SIZEOF_ROW"_Stz,          sizeof(Ent));
  /* 37 */ vAddConstant("SIZEOF_CRWBUFFER"_Stz,    sizeof(CrwBuffer));
  /* 38 */ vAddConstant("SIZEOF_ROWCURSOR"_Stz,    (i8)0);
  
  /* 43 */ vAddConstant("SIZEOF_SESSION"_Stz,      sizeof(Session));
  
  /* 46 */ //vAddConstant("SIZEOF_SEPROMPT"_Stz,     sizeof(SePrompt));
  /* 49 */ vAddConstant("SIZEOF_SKIPLIST"_Stz,     sizeof(SkipListNL));
  /* 50 */ vAddConstant("SIZEOF_SKIPNODE"_Stz,     sizeof(SkipNode));
  /* 51 */ vAddConstant("SIZEOF_SMALLHEAP"_Stz,    sizeof(Heap));
  /* 52 */ vAddConstant("SIZEOF_SOCKET"_Stz,       sizeof(Socket));
  /* 53 */ vAddConstant("SIZEOF_STACK"_Stz,        sizeof(Stack));
  /* 54 */ vAddConstant("SIZEOF_STEPACCUM"_Stz,    sizeof(StepAccum));
  /* 55 */ vAddConstant("SIZEOF_STEPTIMING"_Stz,   sizeof(StepTiming));
  /* 56 */ vAddConstant("SIZEOF_SYSCALL"_Stz,      sizeof(SysCall));
  /* 57 */ vAddConstant("SIZEOF_TABLE"_Stz,        sizeof(Table));
  /* 58 */ vAddConstant("SIZEOF_TASK"_Stz,         sizeof(Task));
  
  /*    */ vAddConstant("SIZEOF_TASKQUEUE"_Stz,    sizeof(TaskQueue));
  /* 62 */ vAddConstant("SIZEOF_THREAD"_Stz,       sizeof(Thread));
  
  /* 66 */ vAddConstant("SIZEOF_TIMER"_Stz,        sizeof(Timer));
  /* 67 */ vAddConstant("SIZEOF_TOKENISER"_Stz,    sizeof(Input));
  /* 68 */ vAddConstant("SIZEOF_TX"_Stz,           sizeof(Tx));
  /* 74 */ vAddConstant("SIZEOF_WORD"_Stz,         sizeof(Word));
  
  /*  0 */ vAddConstant("SIZEOF_S32"_Stz,          sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256"_Stz,         sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S48K"_Stz,         sizeof(s48K));
  

  #ifdef WITH_ACTION_SEQUENCE
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_ADDRESS_CHECKING
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_APPLE
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ACTION_SEQUENCE"_Stz,(i8)false);
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
  vAddConstant("WITH_BUCKET_HASHING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_BUCKET_HASHING"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_CORECTNESS_ALGEBRA
  vAddConstant("WITH_CORECTNESS_ALGEBRA"_Stz,(i8)true);
  #else
  vAddConstant("WITH_CORECTNESS_ALGEBRA"_Stz,(i8)false);
  #endif
          
  #ifdef WITH_DEBUG_VERSION
  vAddConstant("WITH_DEBUG_VERSION"_Stz,(i8)true);
  #else
  vAddConstant("WITH_DEBUG_VERSION"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_EB_LAZY_CHAIN
  vAddConstant("WITH_EB_LAZY_CHAIN"_Stz,(i8)true);
  #else
  vAddConstant("WITH_EB_LAZY_CHAIN"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_ENCODING_TRACE
  vAddConstant("WITH_ENCODING_TRACE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ENCODING_TRACE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_ENTRY_EXIT
  vAddConstant("WITH_ENTRY_EXIT"_Stz,(i8)true);
  #else
  vAddConstant("WITH_ENTRY_EXIT"_Stz,(i8)false);
  #endif
      
  #ifdef WITH_INFLATE_TRACE
  vAddConstant("WITH_INFLATE_TRACE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_INFLATE_TRACE"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_IT_COMPACT_TRACE
  vAddConstant("WITH_IT_COMPACT_TRACE"_Stz,(i8)true);
  #else
  vAddConstant("WITH_IT_COMPACT_TRACE"_Stz,(i8)false);
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
    
  #ifdef WITH_PREPOST_CHECKING
  vAddConstant("WITH_PREPOST_CHECKING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_PREPOST_CHECKING"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_PULSING
  vAddConstant("WITH_PULSING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_PULSING"_Stz,(i8)false);
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
        
  #ifdef WITH_SETH_SUPPORT
  vAddConstant("WITH_SETH_SUPPORT"_Stz,(i8)true);
  #else
  vAddConstant("WITH_SETH_SUPPORT"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_SHARED_LOG
  vAddConstant("WITH_SHARED_LOG"_Stz,(i8)true);
  #else
  vAddConstant("WITH_SHARED_LOG"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_SHOCO_DUPLICATES
  vAddConstant("WITH_SHOCO_DUPLICATES"_Stz,(i8)true);
  #else
  vAddConstant("WITH_SHOCO_DUPLICATES"_Stz,(i8)false);
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
    
  #ifdef WITH_TRACE_DYNAMO
  vAddConstant("WITH_TRACE_DYNAMO"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_DYNAMO"_Stz,(i8)false);
  #endif
    
  #ifdef WITH_TRACE_PURGING
  vAddConstant("WITH_TRACE_PURGING"_Stz,(i8)true);
  #else
  vAddConstant("WITH_TRACE_PURGING"_Stz,(i8)false);
  #endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
