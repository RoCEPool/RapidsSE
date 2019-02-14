/*============================================================================*/
/* AddConstants.cpp - add constants into main dictionary at startup           */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Axioms::vAddConstant(Stz* aName, i8 iVal)
{
  Word* pNewWord = new(&gpAttachMap->m_oRootDic) Word(aName);
  
  IntVal xIntVal;
  xIntVal = iVal;
  
  pNewWord->vEncode("pushValue"_Stz,&xIntVal);
  pNewWord->vEncode("exit"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add constant to main dictionary                                            */
/*----------------------------------------------------------------------------*/
void Axioms::vAddConstant(Stz* aName, Stz* aValue)
{
  Word* pNewWord = new(&gpAttachMap->m_oRootDic) Word(aName);
  
  GenVar xWspStr;
  xWspStr = aValue;
  
  pNewWord->vEncode("pushValue"_Stz,&xWspStr);
  pNewWord->vEncode("exit"_Stz);
}

/*----------------------------------------------------------------------------*/
/* add constants                                                              */
/*----------------------------------------------------------------------------*/
void Axioms::vAddConstants(void)
{
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Adding constants...\n"_Stz);
  
  // true / flase
  vAddConstant("true"_Stz,    (i8)1);
  vAddConstant("false"_Stz,   (i8)0);
  
  vAddConstant("RAPIDSSE_VERSION_STRING"_Stz,  (Stz*)RAPIDSSE_VERSION_STRING);
  
#ifdef WITH_DEBUG_VERSION
  vAddConstant("WITH_DEBUG_VERSION"_Stz,1);
  vAddConstant("WITH_RELEASE_VERSION"_Stz,(i8)0);
#else
  vAddConstant("WITH_DEBUG_VERSION"_Stz,(i8)0);
  vAddConstant("WITH_RELEASE_VERSION"_Stz,1);
#endif
  
  vAddConstant("C_SIZE_JUMBO_OBJECT"_Stz,  C_SIZE_JUMBO_OBJECT);
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
  
  vAddConstant("C_4KB"_Stz,   C_4KB);
  vAddConstant("C_16KB"_Stz,  C_16KB);
  vAddConstant("C_64KB"_Stz,  C_64KB);
  vAddConstant("C_256KB"_Stz, C_256KB);
  
  vAddConstant("C_INDEXES_PER_TABLE"_Stz, C_INDEXES_PER_TABLE);
  vAddConstant("C_SLOTS_PER_NODE"_Stz,    C_SLOTS_PER_NODE);
  vAddConstant("C_MAX_NODES"_Stz,         C_MAX_NODES);
  vAddConstant("C_MAX_PARTITIONS"_Stz,    C_MAX_PARTITIONS);

  vAddConstant("C_LOW_BUFFERS"_Stz,       C_LOW_BUFFERS);
  vAddConstant("C_MAX_PRECISION_BCD"_Stz, C_MAX_PRECISION_BCD);
  vAddConstant("C_MAX_COLUMNS"_Stz,       C_MAX_COLUMNS);
  vAddConstant("C_MAX_RECORD"_Stz,        C_MAX_RECORD);
  vAddConstant("C_PATH_MAX"_Stz,          C_PATH_MAX);
  vAddConstant("C_SIZE_POOL_BUFFER"_Stz,  C_SIZE_POOL_BUFFER);
  
  // OPEN_ : file open argument
  vAddConstant("OPEN_READ"_Stz,              C_OPEN_READ);
  vAddConstant("OPEN_WRITE"_Stz,             C_OPEN_WRITE);
  vAddConstant("OPEN_APPEND"_Stz,            C_OPEN_APPEND);
  vAddConstant("OPEN_READ_WRITE_EXIST"_Stz,  C_OPEN_READ_WRITE_EXIST);
  vAddConstant("OPEN_READ_WRITE_CREATE"_Stz, C_OPEN_READ_WRITE_CREATE);
  vAddConstant("OPEN_READ_APPEND"_Stz,       C_OPEN_READ_APPEND);
  
  // hardware
  vAddConstant("HW_CLOCK_FREQUENCY"_Stz, Tsc::iTickFrequency());
  vAddConstant("HW_CORE_COUNT"_Stz,      Hardware::iCoreCount());
  vAddConstant("HW_CPU_COUNT"_Stz,       Hardware::iCpuCount());
  vAddConstant("HW_PHYSICAL_MEMORY"_Stz, Hardware::iPhysicalMemory());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // AN_ : action numbers
  /*  0 */ vAddConstant("AN_NONE"_Stz,               AN_NONE);
  /*  0 */ vAddConstant("AN_ACK_TASK"_Stz,           AN_ACK_TASK);
  /*  0 */ vAddConstant("AN_ACKS_REQUIRED"_Stz,      AN_ACKS_REQUIRED);
  /*  0 */ vAddConstant("AN_ADDED_ORIGINATOR"_Stz,   AN_ADDED_ORIGINATOR);
  /*  0 */ vAddConstant("AN_ADDED_PARTICIPANT"_Stz,  AN_ADDED_PARTICIPANT);
  /*  0 */ vAddConstant("AN_AM_DECIDER"_Stz,         AN_AM_DECIDER);
  /*  0 */ vAddConstant("AN_CLOSE_FILE"_Stz,         AN_CLOSE_FILE);
  /*  0 */ vAddConstant("AN_CONSTRUCTOR"_Stz,        AN_CONSTRUCTOR);
  /*  0 */ vAddConstant("AN_DDL_DROPPED_READ"_Stz,   AN_DDL_DROPPED_READ);
  /*  0 */ vAddConstant("AN_DDL_DROPPED_WRITE"_Stz,  AN_DDL_DROPPED_WRITE);
  /*  0 */ vAddConstant("AN_DDL_GRAB_READ"_Stz,      AN_DDL_GRAB_READ);
  /*  0 */ vAddConstant("AN_DDL_GRAB_WRITE"_Stz,     AN_DDL_GRAB_WRITE);
  /*  0 */ vAddConstant("AN_DDL_GRABBED_READ"_Stz,   AN_DDL_GRABBED_READ);
  /*  0 */ vAddConstant("AN_DDL_GRABBED_WRITE"_Stz,  AN_DDL_GRABBED_WRITE);
  /*  0 */ vAddConstant("AN_DESTRUCTOR"_Stz,         AN_DESTRUCTOR);
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
  /*  0 */ vAddConstant("AN_REMOVED_ORIGINATOR"_Stz, AN_REMOVED_ORIGINATOR);
  /*  0 */ vAddConstant("AN_REMOVED_PARTICIPANT"_Stz,AN_REMOVED_PARTICIPANT);
  /*  0 */ vAddConstant("AN_RESOURCES_CHECKED"_Stz,  AN_RESOURCES_CHECKED);
  /*  0 */ vAddConstant("AN_RESOURCES_RELEASED"_Stz, AN_RESOURCES_RELEASED);
  /*  0 */ vAddConstant("AN_ROOT"_Stz,               AN_ROOT);
  /*  0 */ vAddConstant("AN_SEED_CAN"_Stz,           AN_SEED_CAN);
  /*  0 */ vAddConstant("AN_SEED_END"_Stz,           AN_SEED_END);
  /*  0 */ vAddConstant("AN_SEED_EXE"_Stz,           AN_SEED_EXE);
  /*  0 */ vAddConstant("AN_SEED_RCV"_Stz,           AN_SEED_RCV);
  /*  0 */ vAddConstant("AN_SEED_REQ"_Stz,           AN_SEED_REQ);
  /*  0 */ vAddConstant("AN_SEED_RUN"_Stz,           AN_SEED_RUN);
  /*  0 */ vAddConstant("AN_SEND_JOB"_Stz,           AN_SEND_JOB);
  /*  0 */ vAddConstant("AN_SPAWN_JOB"_Stz,          AN_SPAWN_JOB);
  /*  0 */ vAddConstant("AN_TABLE_USE"_Stz,          AN_TABLE_USE);
  /*  0 */ vAddConstant("AN_TASK_ACKED"_Stz,         AN_TASK_ACKED);
  /*  0 */ vAddConstant("AN_TASK_LOCKED"_Stz,        AN_TASK_LOCKED);
  /*  0 */ vAddConstant("AN_TASK_SLEEP"_Stz,         AN_TASK_SLEEP);
  /*  0 */ vAddConstant("AN_TASK_STATE"_Stz,         AN_TASK_STATE);
  /*  0 */ vAddConstant("AN_TASK_STATE_SET"_Stz,     AN_TASK_STATE_SET);
  /*  0 */ vAddConstant("AN_TASK_WAIT"_Stz,          AN_TASK_WAIT);
  /*  0 */ vAddConstant("AN_TASK_YIELD"_Stz,         AN_TASK_YIELD);
  /*  0 */ vAddConstant("AN_THREAD_EXECUTE"_Stz,     AN_THREAD_EXECUTE);
  /*  0 */ vAddConstant("AN_ENUMS"_Stz,              AN_ENUMS);
  
  // AT_ : fetch techniques
  /*  0 */ vAddConstant("AT_NONE"_Stz,  AT_NONE);
  /*  1 */ vAddConstant("AT_EXACT"_Stz, AT_EXACT);
  /*  2 */ vAddConstant("AT_LOWER"_Stz, AT_LOWER);
  /*  3 */ vAddConstant("AT_UPPER"_Stz, AT_UPPER);
  /*  4 */ vAddConstant("AT_RANGE"_Stz, AT_RANGE);
  /*  5 */ vAddConstant("AT_SCAN"_Stz,  AT_SCAN);
  /*  6 */ vAddConstant("AT_ENUMS"_Stz, AT_ENUMS);
  
  // CN_ : class numbers
  /*  0 */ vAddConstant("CN_NONE"_Stz,         CN_NONE);

  /*  5 */ vAddConstant("CN_FIPVAL"_Stz,       CN_FIPVAL);
  /*  4 */ vAddConstant("CN_FLTVAL"_Stz,       CN_FLTVAL);
  /*  4 */ vAddConstant("CN_INTVAL"_Stz,       CN_INTVAL);
  /*  7 */ vAddConstant("CN_PTRVAL"_Stz,       CN_PTRVAL);
  /*  8 */ vAddConstant("CN_STRVAL"_Stz,       CN_STRVAL);
  
  /* 12 */ vAddConstant("CN_ARGUMENTS"_Stz,    CN_ARGUMENTS);
  /* 13 */ vAddConstant("CN_ATOMIC"_Stz,       CN_ATOMIC);
  /* 14 */ vAddConstant("CN_ATTACHMAP"_Stz,    CN_ATTACHMAP);
  
  /* 17 */ vAddConstant("CN_BPA"_Stz,          CN_BPA);
  /* 16 */ vAddConstant("CN_BUFFERPOOL"_Stz,   CN_BUFFERPOOL);
  /* 16 */ vAddConstant("CN_BYTERATE"_Stz,     CN_BYTERATE);

  /* 18 */ vAddConstant("CN_DIC"_Stz,          CN_DIC);
  /*    */ vAddConstant("CN_DICPATH"_Stz,      CN_DICPATH);
  /* 19 */ vAddConstant("CN_DICSTACK"_Stz,     CN_DICSTACK);
  /* 20 */ vAddConstant("CN_DIR"_Stz,          CN_DIR);
  
  /*    */ vAddConstant("CN_FETCHBUFFER"_Stz,  CN_FETCHBUFFER);
  /* 21 */ vAddConstant("CN_FILE"_Stz,         CN_FILE);
  /* 15 */ vAddConstant("CN_FREEBUFFER"_Stz,   CN_FREEBUFFER);
  /* 22 */ vAddConstant("CN_FREECHUNK"_Stz,    CN_FREECHUNK);
  
  /*    */ vAddConstant("CN_GENVAR"_Stz,       CN_GENVAR);
  /* 26 */ vAddConstant("CN_GRID"_Stz,         CN_GRID);
  
  /* 27 */ vAddConstant("CN_HEAPBUFFER"_Stz,   CN_HEAPBUFFER);
  
  /* 28 */ vAddConstant("CN_INDEX"_Stz,        CN_INDEX);
  /*    */ //vAddConstant("CN_INDEXDEFN"_Stz,    CN_INDEXDEFN);

  /*    */ vAddConstant("CN_JOB__BEGUN"_Stz,         CN_JOB__BEGUN);
  /*    */ vAddConstant("CN_JOB__CANCEL"_Stz,        CN_JOB__CANCEL);
  /*    */ vAddConstant("CN_JOB__CANCELLED"_Stz,     CN_JOB__CANCELLED);
  /*    */ vAddConstant("CN_JOB__END"_Stz,           CN_JOB__END);
  /*    */ vAddConstant("CN_JOB__ENDED"_Stz,         CN_JOB__ENDED);
  /*    */ vAddConstant("CN_JOB__EXECUTE"_Stz,       CN_JOB__EXECUTE);
  /*    */ vAddConstant("CN_JOB__EXECUTED"_Stz,      CN_JOB__EXECUTED);
  
  /*    */ vAddConstant("CN_JOB_ALARM"_Stz,          CN_JOB_ALARM);
  /*    */ vAddConstant("CN_JOB_ANALYZEINDEX"_Stz,   CN_JOB_ANALYZEINDEX);
  /*    */ vAddConstant("CN_JOB_ANALYZETABLE"_Stz,   CN_JOB_ANALYZETABLE);
  /*    */ vAddConstant("CN_JOB_ANSWER"_Stz,         CN_JOB_ANSWER);
  /*    */ vAddConstant("CN_JOB_BOOT"_Stz,           CN_JOB_BOOT);
  /*    */ vAddConstant("CN_JOB_CAT"_Stz,            CN_JOB_CAT);
  /*    */ vAddConstant("CN_JOB_CF"_Stz,             CN_JOB_CF);
  /*    */ vAddConstant("CN_JOB_COMPACTINDEX"_Stz,   CN_JOB_COMPACTINDEX);
  /*    */ vAddConstant("CN_JOB_CONNECTCLUSTER"_Stz, CN_JOB_CONNECTCLUSTER);
  /*    */ vAddConstant("CN_JOB_CONNECTOR"_Stz,      CN_JOB_CONNECTOR);
  /*    */ vAddConstant("CN_JOB_COPYBUFFER"_Stz,     CN_JOB_COPYBUFFER);
  /*    */ vAddConstant("CN_JOB_CREATECATALOG"_Stz,  CN_JOB_CREATECATALOG);
  /*    */ vAddConstant("CN_JOB_CREATEINDEX"_Stz,    CN_JOB_CREATEINDEX);
  /*    */ vAddConstant("CN_JOB_CREATESCHEMA"_Stz,   CN_JOB_CREATESCHEMA);
  /*    */ vAddConstant("CN_JOB_CREATETABLE"_Stz,    CN_JOB_CREATETABLE);
  /*    */ vAddConstant("CN_JOB_CREATETENANT"_Stz,   CN_JOB_CREATETENANT);
  /*    */ vAddConstant("CN_JOB_DOCOL"_Stz,          CN_JOB_DOCOL);
  /*    */ vAddConstant("CN_JOB_DROPCATALOG"_Stz,    CN_JOB_DROPCATALOG);
  /*    */ vAddConstant("CN_JOB_DROPINDEX"_Stz,      CN_JOB_DROPINDEX);
  /*    */ vAddConstant("CN_JOB_DROPSCHEMA"_Stz,     CN_JOB_DROPSCHEMA);
  /*    */ vAddConstant("CN_JOB_DROPTABLE"_Stz,      CN_JOB_DROPTABLE);
  /*    */ vAddConstant("CN_JOB_DROPTENANT"_Stz,     CN_JOB_DROPTENANT);
  /*    */ vAddConstant("CN_JOB_DUMPINDEX"_Stz,      CN_JOB_DUMPINDEX);
  /*    */ vAddConstant("CN_JOB_EXPORT"_Stz,         CN_JOB_EXPORT);
  /*    */ vAddConstant("CN_JOB_FETCHROWS"_Stz,      CN_JOB_FETCHROWS);
  /*    */ vAddConstant("CN_JOB_INSERTROW"_Stz,      CN_JOB_INSERTROW);
  /*    */ vAddConstant("CN_JOB_INSERTTABLE"_Stz,    CN_JOB_INSERTTABLE);
  /*    */ vAddConstant("CN_JOB_LF"_Stz,             CN_JOB_LF);
  /*    */ vAddConstant("CN_JOB_LISTCATALOGS"_Stz,   CN_JOB_LISTCATALOGS);
  /*    */ vAddConstant("CN_JOB_LISTCOLUMNS"_Stz,    CN_JOB_LISTCOLUMNS);
  /*    */ vAddConstant("CN_JOB_LISTIDXCOLUMNS"_Stz, CN_JOB_LISTIDXCOLUMNS);
  /*    */ vAddConstant("CN_JOB_LISTINDEXES"_Stz,    CN_JOB_LISTINDEXES);
  /*    */ vAddConstant("CN_JOB_LISTPARTCOLUMNS"_Stz,CN_JOB_LISTPARTCOLUMNS);
  /*    */ vAddConstant("CN_JOB_LISTPARTITIONS"_Stz, CN_JOB_LISTPARTITIONS);
  /*    */ vAddConstant("CN_JOB_LISTSCHEMAS"_Stz,    CN_JOB_LISTSCHEMAS);
  /*    */ vAddConstant("CN_JOB_LISTTABLES"_Stz,     CN_JOB_LISTTABLES);
  /*    */ vAddConstant("CN_JOB_LISTTENANTS"_Stz,    CN_JOB_LISTTENANTS);
  /*    */ vAddConstant("CN_JOB_LOADTABLE"_Stz,      CN_JOB_LOADTABLE);
  /*    */ vAddConstant("CN_JOB_MONITOR"_Stz,        CN_JOB_MONITOR);
  /*    */ vAddConstant("CN_JOB_NETHOG"_Stz,         CN_JOB_NETHOG);
  /*    */ vAddConstant("CN_JOB_NODEINFO"_Stz,       CN_JOB_NODEINFO);
  /*    */ vAddConstant("CN_JOB_PERIODIC"_Stz,       CN_JOB_PERIODIC);
  /*    */ vAddConstant("CN_JOB_PING"_Stz,           CN_JOB_PING);
  /*    */ vAddConstant("CN_JOB_PUBLISH"_Stz,        CN_JOB_PUBLISH);
  /*    */ vAddConstant("CN_JOB_PURGE"_Stz,          CN_JOB_PURGE);
  /*    */ vAddConstant("CN_JOB_PWF"_Stz,            CN_JOB_PWF);
  /*    */ vAddConstant("CN_JOB_RECEIVER"_Stz,       CN_JOB_RECEIVER);
  /*    */ vAddConstant("CN_JOB_REPEATER"_Stz,       CN_JOB_REPEATER);
  /*    */ vAddConstant("CN_JOB_REPORT"_Stz,         CN_JOB_REPORT);
  /*    */ vAddConstant("CN_JOB_SANITY"_Stz,         CN_JOB_SANITY);
  /*    */ vAddConstant("CN_JOB_SELECTTABLE"_Stz,    CN_JOB_SELECTTABLE);
  /*    */ vAddConstant("CN_JOB_SHOWCLUSTER"_Stz,    CN_JOB_SHOWCLUSTER);
  /*    */ vAddConstant("CN_JOB_SINK"_Stz,           CN_JOB_SINK);
  /*    */ vAddConstant("CN_JOB_SIZEINDEX"_Stz,      CN_JOB_SIZEINDEX);
  /*    */ vAddConstant("CN_JOB_SIZETABLE"_Stz,      CN_JOB_SIZETABLE);
  /*    */ vAddConstant("CN_JOB_SPACE"_Stz,          CN_JOB_SPACE);
  /*    */ vAddConstant("CN_JOB_START"_Stz,          CN_JOB_START);
  /*    */ vAddConstant("CN_JOB_STARTCLUSTER"_Stz,   CN_JOB_STARTCLUSTER);
  /*    */ vAddConstant("CN_JOB_STATUS"_Stz,         CN_JOB_STATUS);
  /*    */ vAddConstant("CN_JOB_SUBEXPORTER"_Stz,      CN_JOB_SUBEXPORTER);
  /*    */ vAddConstant("CN_JOB_SUBFETCH"_Stz,       CN_JOB_SUBFETCH);
  /*    */ vAddConstant("CN_JOB_SUBINGESTER"_Stz,    CN_JOB_SUBINGESTER);
  /*    */ vAddConstant("CN_JOB_SUBINSERTER"_Stz,    CN_JOB_SUBINSERTER);
  /*    */ vAddConstant("CN_JOB_SUBLOADER"_Stz,      CN_JOB_SUBLOADER);
  /*    */ vAddConstant("CN_JOB_SUBPUBLISH"_Stz,     CN_JOB_SUBPUBLISH);
  /*    */ vAddConstant("CN_JOB_SUBPURGER"_Stz,      CN_JOB_SUBPURGER);
  /*    */ vAddConstant("CN_JOB_SUBTRUNCATE"_Stz,    CN_JOB_SUBTRUNCATE);
  /*    */ vAddConstant("CN_JOB_TABLESIZES"_Stz,     CN_JOB_TABLESIZES);
  /*    */ vAddConstant("CN_JOB_TRUNCATETABLE"_Stz,  CN_JOB_TRUNCATETABLE);
  /*    */ vAddConstant("CN_JOB_USESCHEMA"_Stz,      CN_JOB_USESCHEMA);
  /*    */ vAddConstant("CN_JOB_VERIFYINDEX"_Stz,    CN_JOB_VERIFYINDEX);
  /*    */ vAddConstant("CN_JOB_VISUALIZEINDEX"_Stz, CN_JOB_VISUALIZEINDEX);
  /*    */ vAddConstant("CN_JOB_WAITCLUSTER"_Stz,    CN_JOB_WAITCLUSTER);
  
  /*    */ vAddConstant("CN_JUMBOPERSISTENT"_Stz, CN_JUMBOPERSISTENT);
  /*    */ vAddConstant("CN_JUMBOTRANSIENT"_Stz,  CN_JUMBOTRANSIENT);
  
  /* 31 */ vAddConstant("CN_KEYWRK"_Stz,       CN_KEYWRK);
  
  /*    */ vAddConstant("CN_LOADCHUNK"_Stz,    CN_LOADCHUNK);
  /* 35 */ vAddConstant("CN_LOADMAP"_Stz,      CN_LOADMAP);
  /*    */ vAddConstant("CN_LOCAL"_Stz,        CN_LOCAL);
  /* 47 */ vAddConstant("CN_LOCALSET"_Stz,     CN_LOCALSET);
  /* 36 */ vAddConstant("CN_LOCK"_Stz,         CN_LOCK);
  
  /*    */ vAddConstant("CN_MEDIUMPERSISTENT"_Stz, CN_MEDIUMPERSISTENT);
  /*    */ vAddConstant("CN_MEDIUMTRANSIENT"_Stz,  CN_MEDIUMTRANSIENT);
  /* 60 */ vAddConstant("CN_MUTEX"_Stz,    CN_MUTEX);

  /* 38 */ vAddConstant("CN_NODE"_Stz,         CN_NODE);
  /*    */ vAddConstant("CN_NOW"_Stz,          CN_NOW);

  /* 40 */ vAddConstant("CN_PARSER"_Stz,       CN_PARSER);
  /*    */ vAddConstant("CN_PLAN"_Stz,         CN_PLAN);
  
  /* 45 */ vAddConstant("CN_QUEUE"_Stz,        CN_QUEUE);
  /* 46 */ vAddConstant("CN_QUEUEBUFFER"_Stz,  CN_QUEUEBUFFER);
  /*    */ vAddConstant("CN_QUEUES"_Stz,       CN_QUEUES);
  
  /*    */ vAddConstant("CN_RECORDBUFFER"_Stz, CN_RECORDBUFFER);
  
  /* 57 */ vAddConstant("CN_SESSION"_Stz,      CN_SESSION);
  /* 57 */ vAddConstant("CN_SESSIONSTATE"_Stz, CN_SESSIONSTATE);
  /* 60 */ vAddConstant("CN_SEPROMPT"_Stz,     CN_SEPROMPT);
  /* 61 */ vAddConstant("CN_SINCE"_Stz,        CN_SINCE);
  /* 63 */ vAddConstant("CN_SKIPLIST"_Stz,     CN_SKIPLIST);
  /* 64 */ vAddConstant("CN_SKIPNODE"_Stz,     CN_SKIPNODE);
  /*    */ vAddConstant("CN_SLOTS"_Stz,        CN_SLOTS);
  /*    */ vAddConstant("CN_SMALLPERSISTENT"_Stz, CN_SMALLPERSISTENT);
  /* 65 */ vAddConstant("CN_SMALLTRANSIENT"_Stz, CN_SMALLTRANSIENT);
  /* 66 */ vAddConstant("CN_SOCKET"_Stz,       CN_SOCKET);
  /* 67 */ vAddConstant("CN_STACK"_Stz,        CN_STACK);
  /* 68 */ vAddConstant("CN_STATISTICS"_Stz,   CN_STATISTICS);
  /* 68 */ vAddConstant("CN_STEPACCUM"_Stz,    CN_STEPACCUM);
  /* 69 */ vAddConstant("CN_STEPTIMING"_Stz,   CN_STEPTIMING);
  /*  1 */ vAddConstant("CN_STRING_T"_Stz,       CN_STRING_T);
  /* 70 */ vAddConstant("CN_SYSCALL"_Stz,      CN_SYSCALL);

  /* 71 */ vAddConstant("CN_TABLE"_Stz,        CN_TABLE);
  /* 41 */ vAddConstant("CN_TABLEPTN"_Stz,     CN_TABLEPTN);
  /* 72 */ vAddConstant("CN_TASK"_Stz,         CN_TASK);
  /*    */ vAddConstant("CN_TASKLIST"_Stz,     CN_TASKLIST);
  /*    */ vAddConstant("CN_TASKQUEUE"_Stz,    CN_TASKQUEUE);
  /* 76 */ vAddConstant("CN_THREAD"_Stz,       CN_THREAD);
  /* 79 */ vAddConstant("CN_TIME"_Stz,         CN_TIME);
  /* 80 */ vAddConstant("CN_TIMER"_Stz,        CN_TIMER);
  /* 81 */ vAddConstant("CN_TOKENISER"_Stz,    CN_TOKENISER);
  /* 82 */ vAddConstant("CN_TX"_Stz,           CN_TX);
  /* 87 */ vAddConstant("CN_TXSET"_Stz,        CN_TXSET);
  
  /* 88 */ vAddConstant("CN_WORD"_Stz,         CN_WORD);

  /* 89 */ vAddConstant("CN_ZSMALL_"_Stz,      CN_ZSMALL_);
  /* 90 */ vAddConstant("CN_ZMEDIUM_"_Stz,     CN_ZMEDIUM_);
  /* 91 */ vAddConstant("CN_ZLARGE_"_Stz,      CN_ZLARGE_);
  /* 92 */ vAddConstant("CN_ZJUMBO_"_Stz,      CN_ZJUMBO_);
  
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
  /*    */ vAddConstant("EC_INVALID_INDEX_COLUMN"_Stz, EC_INVALID_INDEX_COLUMN);
  /* 20 */ vAddConstant("EC_INVALID_INSTANCE"_Stz,   EC_INVALID_INSTANCE);
  /*    */ vAddConstant("EC_INVALID_SIZE"_Stz,       EC_INVALID_SIZE);
  /* 21 */ vAddConstant("EC_INVALID_SYNTAX"_Stz,     EC_INVALID_SYNTAX);
  /* 22 */ vAddConstant("EC_INVALID_VALUE"_Stz,      EC_INVALID_VALUE);
                        
  /* 23 */ vAddConstant("EC_LOW_BUFFERS"_Stz,        EC_LOW_BUFFERS);
  /* 24 */ vAddConstant("EC_MAX_INDEXES"_Stz,        EC_MAX_INDEXES);
  /*    */ vAddConstant("EC_MAX_COLUMNS"_Stz,        EC_MAX_COLUMNS);
  /*    */ vAddConstant("EC_MAX_ROW_LENGTH"_Stz,     EC_MAX_ROW_LENGTH);
  /* 25 */ vAddConstant("EC_NO_COLUMNS_DIC"_Stz,     EC_NO_COLUMNS_DIC);
  /* 26 */ vAddConstant("EC_NO_SUCH_CATALOG"_Stz,    EC_NO_SUCH_CATALOG);
  /* 27 */ vAddConstant("EC_NO_SUCH_DIRECTORY"_Stz,  EC_NO_SUCH_DIRECTORY);
  /* 28 */ vAddConstant("EC_NO_SUCH_FOLDER"_Stz,     EC_NO_SUCH_FOLDER);
  /* 29 */ vAddConstant("EC_NO_SUCH_INDEX"_Stz,      EC_NO_SUCH_INDEX);
  /* 30 */ vAddConstant("EC_NO_SUCH_SCHEMA"_Stz,     EC_NO_SUCH_SCHEMA);
  /* 31 */ vAddConstant("EC_NO_SUCH_TABLE"_Stz,      EC_NO_SUCH_TABLE);
  /* 32 */ vAddConstant("EC_NO_SUCH_TENANT"_Stz,     EC_NO_SUCH_TENANT);
  /* 33 */ vAddConstant("EC_NO_SUCH_WORD"_Stz,       EC_NO_SUCH_WORD);
  
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
  /*  0 */ vAddConstant("JK_NONE"_Stz,     JK_NONE);
  /*  1 */ vAddConstant("JK_CHILD"_Stz,    JK_CHILD);
  /*  2 */ vAddConstant("JK_DIST2PC"_Stz,  JK_DIST2PC);
  /*  3 */ vAddConstant("JK_GENERIC"_Stz,  JK_GENERIC);
  /*  4 */ vAddConstant("JK_GRANDKID"_Stz, JK_GRANDKID);
  /*  5 */ vAddConstant("JK_LOCAL"_Stz,    JK_LOCAL);
  /*  6 */ vAddConstant("JK_SYSTEM"_Stz,   JK_SYSTEM);

  // JP_ : job phase
  /*  0 */ vAddConstant("JP_NONE"_Stz,      JP_NONE);
  /*  0 */ vAddConstant("JP_BEGINING"_Stz,  JP_BEGINING);
  /*  0 */ vAddConstant("JP_BEGUN"_Stz,     JP_BEGUN);
  /*  0 */ vAddConstant("JP_EXECUTING"_Stz, JP_EXECUTING);
  /*  0 */ vAddConstant("JP_EXECUTED"_Stz,  JP_EXECUTED);
  /*  0 */ vAddConstant("JP_ENDING"_Stz,    JP_ENDING);
  /*  0 */ vAddConstant("JP_ENDED"_Stz,     JP_ENDED);
  /*  0 */ vAddConstant("JP_CANCELING"_Stz, JP_CANCELING);
  /*  0 */ vAddConstant("JP_CANCELED"_Stz,  JP_CANCELED);
  /*  0 */ vAddConstant("JP_ENUMS"_Stz,     JP_ENUMS);

  // JT_ : job trace
  /*  0 */ vAddConstant("JT_NONE"_Stz,    JT_NONE);
  /*  1 */ vAddConstant("JT_ALL"_Stz,     JT_ALL);
  /*  2 */ vAddConstant("JT_JOB"_Stz,     JT_JOB);
  /*  3 */ vAddConstant("JT_TASK"_Stz,    JT_TASK);

  // TN_ : token types
  /*  0 */ vAddConstant("TN_NONE"_Stz,    TN_NONE);
  /*  1 */ vAddConstant("TN_NAME"_Stz,    TN_NAME);
  /*  2 */ vAddConstant("TN_INTEGER"_Stz, TN_INTEGER);
  /*  3 */ vAddConstant("TN_STRING"_Stz,  TN_STRING);
  /*  4 */ vAddConstant("TN_ENUMS"_Stz,   TN_ENUMS);
    
  // TK_ : task state
  /*  0 */ vAddConstant("TK_NONE"_Stz,       TK_NONE);
  /*  1 */ vAddConstant("TK_EXECUTING"_Stz,  TK_EXECUTING);
  /*  2 */ vAddConstant("TK_EXITED"_Stz,     TK_EXITED);
  /*  3 */ vAddConstant("TK_READY"_Stz,      TK_READY);
  /*  4 */ vAddConstant("TK_SEND"_Stz,       TK_SEND);
  /*  5 */ vAddConstant("TK_SLEEPING"_Stz,   TK_SLEEPING);
  /*  6 */ vAddConstant("TK_WAITING"_Stz,    TK_WAITING);
  /*  7 */ vAddConstant("TK_YIELDED"_Stz,    TK_YIELDED);
  /*  8 */ vAddConstant("TK_ENUMS"_Stz,      TK_ENUMS);

  // TP_ : transaction phase
  /*  0 */ vAddConstant("TP_NONE"_Stz,        TP_NONE);
  /*  1 */ vAddConstant("TP_EXECUTING"_Stz,   TP_EXECUTING);
  /*  2 */ vAddConstant("TP_COMMITTING"_Stz,  TP_COMMITTING);
  /*  3 */ vAddConstant("TP_COMMITTED"_Stz,   TP_COMMITTED);
  /*  4 */ vAddConstant("TP_PREPARING"_Stz,   TP_PREPARING);
  /*  5 */ vAddConstant("TP_PREPARED"_Stz,    TP_PREPARED);
  /*  6 */ vAddConstant("TP_ROLLINGBACK"_Stz, TP_ROLLINGBACK);
  /*  7 */ vAddConstant("TP_ROLLEDBACK"_Stz,  TP_ROLLEDBACK);
  /*  8 */ vAddConstant("TP_ENUMS"_Stz,       TP_ENUMS);

  // WT_ : word type
  /*  0 */ vAddConstant("WT_NONE"_Stz,      WT_NONE);
  /*  0 */ vAddConstant("WT_LOCAL"_Stz,     WT_LOCAL);
  /*  0 */ vAddConstant("WT_OBJECT"_Stz,    WT_OBJECT);
  /*  0 */ vAddConstant("WT_PRIMITIVE"_Stz, WT_PRIMITIVE);
  /*  0 */ vAddConstant("WT_ROUTINE"_Stz,   WT_ROUTINE);
  /*  0 */ vAddConstant("WT_ENUMS"_Stz,     WT_ENUMS);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // SIZEOF_ : C++ class sizes
  /*  1 */ vAddConstant("SIZEOF_INTVAL"_Stz, sizeof(IntVal));
  /*  2 */ vAddConstant("SIZEOF_IPVAL"_Stz,  sizeof(FipVal));
  /*  4 */ vAddConstant("SIZEOF_PTRVAL"_Stz, sizeof(PtrVal));
  /*  5 */ vAddConstant("SIZEOF_STRVAL"_Stz, sizeof(StrVal));
  /*  6 */ vAddConstant("SIZEOF_VALSET"_Stz, sizeof(GenVar)); // correct
  /*  7 */ vAddConstant("SIZEOF_ValWrk"_Stz, sizeof(GenVar)); // correct
  
  /*  8 */ vAddConstant("SIZEOF_ARGUMENTS"_Stz,    sizeof(Arguments));
  /*  9 */ vAddConstant("SIZEOF_ATOMIC"_Stz,       sizeof(Atomic));
  /* 10 */ vAddConstant("SIZEOF_ATTACHMAP"_Stz,    sizeof(AttachMap));
  /* 11 */ vAddConstant("SIZEOF_POOLBUFFER"_Stz,   sizeof(PoolBuffer));
  /* 12 */ vAddConstant("SIZEOF_BUFFERPOOL"_Stz,   sizeof(BufferPool));
  /* 13 */ vAddConstant("SIZEOF_BPA"_Stz,          sizeof(BPA));
  /* 14 */ vAddConstant("SIZEOF_DIC"_Stz,          sizeof(Dic));
  /* 15 */ vAddConstant("SIZEOF_DICSTACK"_Stz,     sizeof(DicStack));
  /* 16 */ vAddConstant("SIZEOF_DIR"_Stz,          sizeof(Dir));
  /* 17 */ vAddConstant("SIZEOF_FILE"_Stz,         sizeof(File));
  /* 18 */ vAddConstant("SIZEOF_FREECHUNK"_Stz,    sizeof(FreeChunk));
  /* 19 */ vAddConstant("SIZEOF_GRID"_Stz,         sizeof(Grid));
  /* 20 */ vAddConstant("SIZEOF_HEAPBUFFER"_Stz,   sizeof(HeapBuffer));
  /*    */ vAddConstant("SIZEOF_INDEX"_Stz,        sizeof(Index));
  
  /* 24 */ vAddConstant("SIZEOF_LOCK"_Stz,         sizeof(Lock));
  /* 25 */ vAddConstant("SIZEOF_MEDIUMHEAP"_Stz,   sizeof(Heap));
  /*    */ vAddConstant("SIZEOF_MUTEX"_Stz,        sizeof(Mutex));
  /* 26 */ vAddConstant("SIZEOF_NODE"_Stz,         sizeof(Node));
  
  /* 28 */ vAddConstant("SIZEOF_PARSER"_Stz,       sizeof(Parser));
  /* 29 */ vAddConstant("SIZEOF_PARTITION"_Stz,    sizeof(TablePtn));
  /* 33 */ vAddConstant("SIZEOF_QUEUE"_Stz,        sizeof(Queue));
  /* 34 */ vAddConstant("SIZEOF_QUEUEBUFFER"_Stz,  sizeof(QueueBuffer));
  /* 36 */ vAddConstant("SIZEOF_ROW"_Stz,          sizeof(Item));
  /* 37 */ vAddConstant("SIZEOF_ROWBUFFER"_Stz,    sizeof(EntryBuffer));
  /* 38 */ vAddConstant("SIZEOF_ROWCURSOR"_Stz,    (i8)0);
  
  /* 43 */ vAddConstant("SIZEOF_SESSION"_Stz,      sizeof(Session));
  
  /* 46 */ vAddConstant("SIZEOF_SEPROMPT"_Stz,     sizeof(SePrompt));
  /* 47 */ vAddConstant("SIZEOF_SINCE"_Stz,        sizeof(Since));
  /* 49 */ vAddConstant("SIZEOF_SKIPLIST"_Stz,     sizeof(SkipListNL));
  /* 50 */ vAddConstant("SIZEOF_SKIPNODE"_Stz,     sizeof(SkipNode));
  /*    */ vAddConstant("SIZEOF_SLOTS"_Stz,        sizeof(Slots));
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
  
  /* 65 */ vAddConstant("SIZEOF_TIME"_Stz,         sizeof(Time));
  /* 66 */ vAddConstant("SIZEOF_TIMER"_Stz,        sizeof(Timer));
  /* 67 */ vAddConstant("SIZEOF_TOKENISER"_Stz,    sizeof(Tokeniser));
  /* 68 */ vAddConstant("SIZEOF_TX"_Stz,           sizeof(Tx));
  /* 74 */ vAddConstant("SIZEOF_WORD"_Stz,         sizeof(Word));
  
  /*  0 */ vAddConstant("SIZEOF_S32"_Stz,          sizeof(s32));
  /*  1 */ vAddConstant("SIZEOF_S256"_Stz,         sizeof(s256));
  /*  2 */ vAddConstant("SIZEOF_S48K"_Stz,         sizeof(s48K));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
