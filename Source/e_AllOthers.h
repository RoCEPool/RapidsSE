/*============================================================================*/
/* e_AllOthers.h -- collection of all the other enums                         */
/*============================================================================*/
#pragma once

enum eIS // input source
{
  IS_NONE=0,
  IS_FILE,
  IS_STDIN,
  IS_STRING,
  IS_ENUMS
};

static Stz* aIS[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "FILE"_Stz,
  /* 2 */ "STDIN"_Stz,
  /* 3 */ "STRING"_Stz,
  /* 4 */ "ENUMS"_Stz
};

//------------------------------------------------------------------------------

enum eCS // comment style
{
  CS_NONE=0,
  CS_BLOCK,
  CS_LINE
};


//------------------------------------------------------------------------------

enum eJL // jobs list
{
  /* 0 */ JL_NONE=0,
  /* 1 */ JL_ORIGINATING,
  /* 2 */ JL_PARTICIPATING
};

static Stz* aJL[]
{
  /* 0 */ "None"_Stz,
  /* 1 */ "Originating"_Stz,
  /* 2 */ "Participating"_Stz
};

//------------------------------------------------------------------------------

enum ePF // prompt function
{
  /*  0 */ PF_NONE=0,
  
  /*    */ PF_APISTATE,
  /*    */ PF_ARGS,
  /*    */ PF_ASCII,
  /*    */ PF_ATTACHMAP,
  /*    */ PF_BP,
  /*    */ PF_BYE,
  /*    */ PF_CACHE,
  /*    */ PF_CD,
  /*    */ PF_CLASSES,
  /*    */ PF_CMDS,
  /*    */ PF_CONFIG,
  /*    */ PF_CONSTANTS,
  /*    */ PF_ENUMS,
  /*    */ PF_ERRNO,
  /*    */ PF_EXECUTING,
  /*    */ PF_FILECAT,
  /*    */ PF_FILEPROC,
  /*    */ PF_FILESTATS,
  /*    */ PF_FLOATS,
  /*    */ PF_FUNS,
  /*    */ PF_GIVEMEM,
  /*    */ PF_HASHDEFS,
  /*    */ PF_HEAPMEM,
  /*    */ PF_HELP,
  /*    */ PF_HEXDUMP,
  /*    */ PF_HW,
  /*    */ PF_INDEXENTRIES,
  /*    */ PF_JOB,
  /*    */ PF_JOBCAT,
  /*    */ PF_JOBCLASS,
  /*    */ PF_LIMITS,
  /*    */ PF_LOCKS,
  /*    */ PF_LOGNOTE,
  /*    */ PF_LOGSECTION,
  /*    */ PF_LOGWRITE,
  /*    */ PF_LS,
  /*    */ PF_LVL,
  /*    */ PF_MARKACT,
  /*    */ PF_MARKAPI,
  /*    */ PF_MARKJOB,
  /*    */ PF_MARKRES,
  /*    */ PF_MARKTRC,
  /*    */ PF_MASKS,
  /*    */ PF_MEMORY,
  /*    */ PF_MEMTEST,
  /*    */ PF_MKPIPE,
  /*    */ PF_MON,
  /*    */ PF_NOW,
  /*    */ PF_NUMBERS,
  /*    */ PF_OLIST,
  /*    */ PF_OPSYS,
  /*    */ PF_PLIST,
  /*    */ PF_PMC,
  /*    */ PF_POW2,
  /*    */ PF_PREFIXES,
  /*    */ PF_PRINT,
  /*    */ PF_PWD,
  /*    */ PF_QUESTION,
  /*    */ PF_RASSERT,
  /*    */ PF_READY,
  /*    */ PF_RESOURCES,
  /*    */ PF_RMPIPE,
  /*    */ PF_RND,
  /*    */ PF_SE,
  /*    */ PF_SETDBG,
  /*    */ PF_SHELL,
  /*    */ PF_SINCE,
  /*    */ PF_SIZES,
  /*    */ PF_SLEEP,
  /*    */ PF_SLEEPERS,
  /*    */ PF_SLEEPING,
  /*    */ PF_SOCKET,
  /*    */ PF_SSN,
  /*    */ PF_ST,
  /*    */ PF_TAKEMEM,
  /*    */ PF_TASKQUEUES,
  /*    */ PF_TEMPNAME,
  /*    */ PF_THREADS,
  /*    */ PF_TIMEREND,
  /*    */ PF_TIMERSTART,
  /*    */ PF_TPCH,
  /*    */ PF_UPTIME,
  /*    */ PF_VERSION,
  /*    */ PF_WAITING,
  
  /*    */ PF_ENUMS_
};

static Stz* aPF[]
{
  /*  0 */ "NONE"_Stz,
  /*    */ "APISTATE"_Stz,
  /*    */ "ARGS"_Stz,
  /*    */ "ASCII"_Stz,
  /*    */ "ATTACHMAP"_Stz,
  /*    */ "BP"_Stz,
  /*    */ "BYE"_Stz,
  /*    */ "CACHE"_Stz,
  /*    */ "CD"_Stz,
  /*    */ "CLASSES"_Stz,
  /*    */ "CMDS"_Stz,
  /*    */ "CONFIG"_Stz,
  /*    */ "CONSTANTS"_Stz,
  /*    */ "ENUMS"_Stz,
  /*    */ "ERRNO"_Stz,
  /*    */ "EXECUTING"_Stz,
  /*    */ "FILECAT"_Stz,
  /*    */ "FILEPROC"_Stz,
  /*    */ "FILESTATS"_Stz,
  /*    */ "FLOATS"_Stz,
  /*    */ "FUNS"_Stz,
  /*    */ "GIVEMEM"_Stz,
  /*    */ "HASHDEFS"_Stz,
  /*    */ "HEAPMEM"_Stz,
  /*    */ "HELP"_Stz,
  /*    */ "HEXDUMP"_Stz,
  /*    */ "HW"_Stz,
  /*    */ "INDEXENTRIES"_Stz,
  /*    */ "JOB"_Stz,
  /*    */ "JOBCAT"_Stz,
  /*    */ "JOBCLASS"_Stz,
  /*    */ "LIMITS"_Stz,
  /*    */ "LOCKS"_Stz,
  /*    */ "LOGNOTE"_Stz,
  /*    */ "LOGSECTION"_Stz,
  /*    */ "LOGWRITE"_Stz,
  /*    */ "LS"_Stz,
  /*    */ "LVL"_Stz,
  /*    */ "MARKACT"_Stz,
  /*    */ "MARKAPI"_Stz,
  /*    */ "MARKJOB"_Stz,
  /*    */ "MARKRES"_Stz,
  /*    */ "MARKTRC"_Stz,
  /*    */ "MASKS"_Stz,
  /*    */ "MEMORY"_Stz,
  /*    */ "MEMTEST"_Stz,
  /*    */ "MKPIPE"_Stz,
  /*    */ "MON"_Stz,
  /*    */ "OW"_Stz,
  /*    */ "NUMBERS"_Stz,
  /*    */ "OLIST"_Stz,
  /*    */ "OPSYS"_Stz,
  /*    */ "PLIST"_Stz,
  /*    */ "PMC"_Stz,
  /*    */ "POW2"_Stz,
  /*    */ "PREFIXES"_Stz,
  /*    */ "PRINT"_Stz,
  /*    */ "PWD"_Stz,
  /*    */ "QUESTION"_Stz,
  /*    */ "RASSERT"_Stz,
  /*    */ "READY"_Stz,
  /*    */ "RESOURCES"_Stz,
  /*    */ "RMPIPE"_Stz,
  /*    */ "RND"_Stz,
  /*    */ "SE"_Stz,
  /*    */ "SETDBG"_Stz,
  /*    */ "SHELL"_Stz,
  /*    */ "SINCE"_Stz,
  /*    */ "SIZES"_Stz,
  /*    */ "SLEEP"_Stz,
  /*    */ "SLEEPERS"_Stz,
  /*    */ "SLEEPING"_Stz,
  /*    */ "SOCKET"_Stz,
  /*    */ "SSN"_Stz,
  /*    */ "ST"_Stz,
  /*    */ "TAKEMEM"_Stz,
  /*    */ "TASKQUEUES"_Stz,
  /*    */ "TEMPNAME"_Stz,
  /*    */ "THREADS"_Stz,
  /*    */ "TIMEREND"_Stz,
  /*    */ "TIMERSTART"_Stz,
  /*    */ "TPCH"_Stz,
  /*    */ "UPTIME"_Stz,
  /*    */ "VERSION"_Stz,
  /*    */ "WAITING"_Stz,
  
  /*    */ "ENUMS_"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eTD // thread state
{
  /*  0 */ TD_NONE=0,
  /*  1 */ TD_ASSIGNED,
  /*  2 */ TD_IDLE,
  /*  3 */ TD_UNASSIGNED,
  /*  4 */ TD_ENUMS,
};
  
static Stz* aTD[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ASSIGNED"_Stz,
  /*  2 */ "IDLE"_Stz,
  /*  3 */ "UNASSIGNED"_Stz,
  /*  4 */ "ENUMS"_Stz,
};
  
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eTK // Task Kind
{
  /* 0 */ TK_NONE=0,
  /* 1 */ TK_JOB,      // task is a job
  /* 2 */ TK_OPERATOR, // task is an operator
  /* 3 */ TK_ENUMS
};

static Stz* aTK[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "JOB"_Stz,
  /* 2 */ "OPERATOR"_Stz,
  /* 3 */ "ENUMS"_Stz,
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eJF // job flags
{
  /*  0 */ JF_NONE   = 0x0,
  /*  1 */ JF_LOWMEM = 0x1,  // low memory sensistive
};

static Stz* aJF[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "LOWMEM"_Stz,
};

enum ePY // priority
{
  /*  0 */ PY_NONE=0,
  /*  1 */ PY_LOW,
  /*  2 */ PY_NORMAL
};

static Stz* aPY[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "LOW"_Stz,
  /*  2 */ "NORMAL"_Stz
};

enum eJK // job kind
{
  /*  0 */ JK_NONE=0,
  /*  1 */ JK_CHILD,     // subordinate helper run on originator
  /*  2 */ JK_DIST2PC,   // distributed two-phase jobs
  /*  3 */ JK_GENERIC,   // the generic jobs for two-phase messaging
  /*  4 */ JK_GRANDKID,  // subordinate helper run on participant
  /*  5 */ JK_INFO,      // job is for info purposes only
  /*  6 */ JK_LOCAL,     // job is run on the local node
  /*  7 */ JK_SYSTEM,    // system job (such as socket receiver)
  /*  8 */ JK_THREAD,    // job is run by calling thread
  /*  9 */ JK_OTHER,     // any other
};

static Stz* aJK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "CHILD"_Stz,
  /*  2 */ "DIST2PC"_Stz,
  /*  3 */ "GENERIC"_Stz,
  /*  4 */ "GRANDKID"_Stz,
  /*  5 */ "INFO"_Stz,
  /*  6 */ "LOCAL"_Stz,
  /*  7 */ "SYSTEM"_Stz,
  /*  8 */ "THREAD"_Stz,
  /*  9 */ "OTHER"_Stz,
};

enum eJP // job phase
{
  /*  0 */ JP_NONE=0,
  /*  1 */ JP_BEGINING,
  /*  2 */ JP_BEGUN,
  /*  3 */ JP_EXECUTING,
  /*  4 */ JP_EXECUTED,
  /*  5 */ JP_ENDING,
  /*  6 */ JP_ENDED,
  /*  7 */ JP_CANCELING,
  /*  8 */ JP_CANCELED,
  /*  9 */ JP_ENUMS
};

static Stz* aJP[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BEGINING"_Stz,
  /*  2 */ "BEGUN"_Stz,
  /*  3 */ "EXECUTING"_Stz,
  /*  4 */ "EXECUTED"_Stz,
  /*  5 */ "ENDING"_Stz,
  /*  6 */ "ENDED"_Stz,
  /*  7 */ "CANCELING"_Stz,
  /*  8 */ "CANCELED"_Stz,
  /*  9 */ "ENUMS"_Stz,
};

enum eJS // job state
{
  /*  0 */ JS_RUNNING=0,
  /*  1 */ JS_LOCKED,
  /*  2 */ JS_WAITING,
  /*  3 */ JS_SIGNALLED
};

static Stz* aJS[]
{
  /*  0 */ "RUNNING"_Stz,
  /*  1 */ "LOCKED"_Stz,
  /*  2 */ "WAITING"_Stz,
  /*  2 */ "SIGNALLED"_Stz,
};

enum eJT // job trace
{
  /*  0 */ JT_NONE=0,
  /*  1 */ JT_ALL,
  /*  2 */ JT_JOB,
  /*  3 */ JT_TASK
};

static Stz* aJT[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ALL"_Stz,
  /*  2 */ "JOB"_Stz,
  /*  3 */ "TASK"_Stz
};

enum eSS // seed string
{
  /*  0 */ SS_SEED_RUN=0,
  /*  1 */ SS_SEED_REQ,
  /*  2 */ SS_SEED_RCV,
  /*  3 */ SS_SEED_EXE,
  /*  4 */ SS_SEED_END,
  /*  5 */ SS_SEED_CAN,
  /*  6 */ SS_ENUMS
};

static Stz* aSS[]
{
  /*  0 */ "SS_SEED_RUN"_Stz,
  /*  1 */ "SS_SEED_REQ"_Stz,
  /*  2 */ "SS_SEED_RCV"_Stz,
  /*  3 */ "SS_SEED_EXE"_Stz,
  /*  4 */ "SS_SEED_END"_Stz,
  /*  5 */ "SS_SEED_CAN"_Stz,
  /*  6 */ "SS_ENUMS"_Stz,
};

enum eRU // Resource Usage
{
  /*  0 */ RU_NONE=0,
  /*  1 */ RU_SMALL,
  /*  2 */ RU_MEDIUM,
  /*  3 */ RU_LARGE,
  /*  4 */ RU_ENUMS
};

static Stz* aRU[]
{
  /*  0 */ "RU_NONE"_Stz,
  /*  1 */ "RU_SMALL"_Stz,
  /*  2 */ "RU_MEDIUM"_Stz,
  /*  3 */ "RU_LARGE"_Stz,
  /*  4 */ "RU_ENUMS"_Stz,
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eDL // ddl lock
{
  /* 0 */ DL_NONE=0,
  /* 1 */ DL_GRAB_READ,
  /* 2 */ DL_GRAB_WRITE,
  /* 3 */ DL_ENUMS
};

static Stz* aDL[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "grab_read"_Stz,
  /* 2 */ "grab_write"_Stz,
  /* 3 */ "enums"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eRK // response kind
{
  /* 0 */ RK_NONE=0,
  
  /* 1 */ RK_DATA_ROWS,
  /* 2 */ RK_LOAD_ROWS,
  /* 3 */ RK_LOCAL_ERRORS,
  /* 4 */ RK_LOCAL_ROWS,
  /* 5 */ RK_LOCAL_ZERO,
  /* 6 */ RK_NODE_ERRORS,
  /* 7 */ RK_NODE_ROWS,
  /* 8 */ RK_NODE_ZERO,
  
  /* 9 */ RK_ENUMS_
};

static Stz* aRK[]
{
  /* 0 */ "NONE"_Stz,
  
  /* 1 */ "DATA_ROWS"_Stz,
  /* 2 */ "LOAD_ROWS"_Stz,
  /* 3 */ "LOCAL_ERRORS"_Stz,
  /* 4 */ "LOCAL_ROWS"_Stz,
  /* 5 */ "LOCAL_ZERO"_Stz,
  /* 6 */ "NODE_ERRORS"_Stz,
  /* 7 */ "NODE_ROWS"_Stz,
  /* 8 */ "NODE_ZERO"_Stz,
  
  /* 9 */ "ENUMS_"_Stz
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eOR // op return
{
  /*  0 */ OR_NONE = 0,
  /*  1 */ OR_BIDE,  // bid till operand
  /*  2 */ OR_EXIT,  // exit
  /*  3 */ OR_PAUSE, // pause till continued
};

static Stz* aOR[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BIDE"_Stz,
  /*  2 */ "EXIT"_Stz,
  /*  3 */ "PAUSE"_Stz,
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

enum eNE // next entry
{
  /*  0 */ NE_NEXT_LEFT=0,
  /*  1 */ NE_NEXT_RIGHT,
  /*  2 */ NE_NEXT_LEFT_RIGHT,
  /*  3 */ NE_NEXT_NONE,
  /*  4 */ NE_NEXT_EXIT,
  /*  5 */ NE_ENUMS_
};

static Stz* aNE[]
{
  /*  0 */ "NEXT_LEFT"_Stz,
  /*  1 */ "NEXT_RIGHT"_Stz,
  /*  2 */ "NEXT_LEFT_RIGHT"_Stz,
  /*  3 */ "NEXT_NONE"_Stz,
  /*  4 */ "NEXT_EXIT"_Stz,
  /*  5 */ "ENUMS_"_Stz
};

enum eAK // arg kind
{
  /* 0 */ AK_NONE=0,
  /* 1 */ AK_COLUMNNM,
  /* 2 */ AK_LITERAL,
  /* 4 */ AK_ENUMS
};

static Stz* aAK[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "columnnm"_Stz,
  /* 2 */ "literal"_Stz,
};

enum eAT // access technique
{
  /* 0 */ AT_NONE=0,
  /* 1 */ AT_EXACT,
  /* 2 */ AT_LOWER,
  /* 3 */ AT_UPPER,
  /* 4 */ AT_RANGE,
  /* 5 */ AT_SCAN,
  /* 6 */ AT_ENUMS
};

static Stz* aAT[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "exact"_Stz,
  /* 2 */ "lower"_Stz,
  /* 3 */ "upper"_Stz,
  /* 4 */ "range"_Stz,
  /* 5 */ "scan"_Stz,
  /* 6 */ "enums"_Stz
};

enum ePK // predicate kind
{
  /* 0 */ PK_NONE=0,
  /* 1 */ PK_COLUMN_COLUMN_OP,
  /* 2 */ PK_COLUMN_LITERAL_OP,   // this should always be the case
  /* 3 */ PK_LITERAL_LITERAL_OP,
  /* 4 */ PK_LITERAL_COLUMN_OP,
  /* 5 */ PK_ENUMS
};
  
static Stz* aPK[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "column column op"_Stz,
  /* 2 */ "column literal op"_Stz,
  /* 3 */ "literal literal op"_Stz,
  /* 4 */ "literal column op"_Stz,
  /* 5 */ "enums"_Stz,
};
  
enum eTP // Transaction Phase
{
  /* 0 */ TP_NONE=0,
  /* 1 */ TP_EXECUTING,
  /* 2 */ TP_COMMITTING,
  /* 3 */ TP_COMMITTED,
  /* 4 */ TP_PREPARING,
  /* 5 */ TP_PREPARED,
  /* 6 */ TP_ROLLINGBACK,
  /* 7 */ TP_ROLLEDBACK,
  /* 8 */ TP_ENUMS
};

static Stz* aTP[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "EXECUTING"_Stz,
  /* 2 */ "COMMITTING"_Stz,
  /* 3 */ "COMMITTED"_Stz,
  /* 4 */ "PREPARING"_Stz,
  /* 5 */ "PREPARED"_Stz,
  /* 6 */ "ROLLINGBACK"_Stz,
  /* 7 */ "ROLLEDBACK"_Stz,
  /* 8 */ "ENUMS"_Stz
};

enum eKC // KeyCompare
{
  /*  0 */ KC_NONE=0,
  /*  1 */ KC_BEFORE,
  /*  2 */ KC_SAME,
  /*  3 */ KC_AFTER,
  /*  4 */ KC_ENUMS
};

static Stz* aKC[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BEFORE"_Stz,
  /*  2 */ "SAME"_Stz,
  /*  3 */ "AFTER"_Stz,
  /*  4 */ "ENUMS"_Stz,
};

enum eDS // data source
{
  /* 0 */ DS_NONE=0,
  /* 1 */ DS_INGEST,
  /* 2 */ DS_INSERT,
  /* 3 */ DS_LOAD,
  /* 3 */ DS_ENUMS
};

static Stz* aDS[]
{
  /* 0 */ "none"_Stz,
  /* 1 */ "ingest"_Stz,
  /* 2 */ "insert"_Stz,
  /* 3 */ "load"_Stz,
  /* 4 */ "enums"_Stz
};

enum eCT // Column Type
{
  /*  0 */ CT_NONE=0,
  /*  1 */ CT_BOOLEAN,
  /*  2 */ CT_CHAR,
  /*  3 */ CT_DATE,
  /*  4 */ CT_DECIMAL,
  /*  5 */ CT_FLOAT,
  /*  6 */ CT_INTEGER,
  /*  7 */ CT_INTERVAL,
  /*  8 */ CT_TIME,
  /*  9 */ CT_TIMESTAMP,
  /* 10 */ CT_VARCHAR,
  /* 11 */ CT_ENUMS
};

static Stz* aCT[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BOOLEAN"_Stz,
  /*  2 */ "CHAR"_Stz,
  /*  3 */ "DATE"_Stz,
  /*  4 */ "DECIMAL"_Stz,
  /*  5 */ "FLOAT"_Stz,
  /*  6 */ "INTEGER"_Stz,
  /*  7 */ "INTERVAL"_Stz,
  /*  8 */ "TIME"_Stz,
  /*  9 */ "TIMESTAMP"_Stz,
  /* 10 */ "VARCHAR"_Stz,
  /* 11 */ "ENUMS"_Stz
};

static Stz* aCT_short[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "b"_Stz, // boolean
  /*  2 */ "c"_Stz, // char
  /*  3 */ "d"_Stz, // date
  /*  4 */ "m"_Stz, // decimal
  /*  5 */ "f"_Stz, // float
  /*  6 */ "i"_Stz, // integer
  /*  7 */ "l"_Stz, // interval
  /*  8 */ "t"_Stz, // time
  /*  9 */ "p"_Stz, // timestamp
  /* 10 */ "s"_Stz, // varchar
  /* 11 */ "ENUMS"_Stz
};

enum eIT // Index Type
{
  /* 0 */ IT_NONE=0,
  /* 1 */ IT_SKIP_LIST
};

static Stz* aIT[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "SKIP_LIST"_Stz
};

enum eTT // Table Type
{
  /* 0 */ TT_NONE=0,
  /* 1 */ TT_NORMAL
};

static Stz* aTT[]
{
  /* 0 */ "NONE"_Stz,
  /* 1 */ "NORMAL"_Stz
};

enum eWT // word type
{
  /*  0 */ WT_NONE=0,     // none
  /*  1 */ WT_LOCAL,      // word ia a local
  /*  2 */ WT_OBJECT,     // word is an object
  /*  3 */ WT_PRIMITIVE,  // word is a primitive (eg +, return)
  /*  4 */ WT_ROUTINE,    // word is a routine
  /*  5 */ WT_ENUMS
};

static Stz* aWT[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "LOCAL"_Stz,
  /*  2 */ "OBJECT"_Stz,
  /*  3 */ "PRIMITIVE"_Stz,
  /*  4 */ "ROUTINE"_Stz,
  /*  5 */ "ENUMS"_Stz,
};

enum eTN // token type
{
  /*  0 */ TN_NONE=0,  // none
  /*  1 */ TN_INTEGER, // integer literal, e.g. 1, 42
  /*  2 */ TN_NAME,    // name token, e.g. WASHER (or ++ in special mode)
  /*  3 */ TN_OTHER,   // other literal, e.g. {,~
  /*  4 */ TN_STRING,  // string literal, e.g. "the answer"
  /*  5 */ TN_ENUMS
};

static Stz* aTN[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "INTEGER"_Stz,
  /*  2 */ "NAME"_Stz,
  /*  3 */ "OTHER"_Stz,
  /*  4 */ "STRING"_Stz,
  /*  5 */ "ENUMS"_Stz,
};

enum eRE // record ends
{
  /* 0 */ RE_NL,     // record finally ends with \n
  /* 1 */ RE_CR,     // record finally ends with \r
  /* 2 */ RE_CRNL    // record finally ends with \r\n
};

static Stz* aRE[]
{
  /* 0 */ "NL"_Stz,
  /* 1 */ "CR"_Stz,
  /* 2 */ "CRNL"_Stz
};

enum eHT // Heap type
{
  /*  0 */ HT_INVALID=0,
  /*  1 */ HT_PERSISTENT_SMALL,
  /*  2 */ HT_PERSISTENT_MEDIUM,
  /*  3 */ HT_PERSISTENT_LARGE,
  /*  4 */ HT_PERSISTENT_JUMBO,
  /*  5 */ HT_TRANSIENT_SMALL,
  /*  6 */ HT_TRANSIENT_MEDIUM,
  /*  7 */ HT_TRANSIENT_LARGE,
  /*  8 */ HT_TRANSIENT_JUMBO,
  /*  9 */ HT_ENUMS
};

static Stz* aHT[]
{
  /*  0 */ "INVALID"_Stz,
  /*  1 */ "PERSISTENT_SMALL"_Stz,
  /*  2 */ "PERSISTENT_MEDIUM"_Stz,
  /*  3 */ "PERSISTENT_LARGE"_Stz,
  /*  4 */ "PERSISTENT_JUMBO"_Stz,
  /*  5 */ "TRANSIENT_SMALL"_Stz,
  /*  6 */ "TRANSIENT_MEDIUM"_Stz,
  /*  7 */ "TRANSIENT_LARGE"_Stz,
  /*  8 */ "TRANSIENT_JUMBO"_Stz,
  /*  9 */ "ENUMS"_Stz
};


enum eDM : u1 // Dtm Kind
{
  /*  0 */ DM_NONE=0,
  
  /*  1 */ DM_BCD,
  /*  2 */ DM_BOL,
  /*  3 */ DM_CHR,
  /*  4 */ DM_DTE,
  /*  5 */ DM_FLT,
  /*  6 */ DM_INT,
  /*  7 */ DM_IVL,
  /*  8 */ DM_TME,
  /*  9 */ DM_TSP,
  /* 10 */ DM_VBN,
  /* 11 */ DM_VCH,
  
  /* 12 */ DM_ENUMS
};

static Stz* aDM[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "BCD"_Stz,
  /*  2 */ "BOL"_Stz,
  /*  3 */ "CHR"_Stz,
  /*  4 */ "DTE"_Stz,
  /*  5 */ "FLT"_Stz,
  /*  6 */ "INT"_Stz,
  /*  7 */ "IVL"_Stz,
  /*  8 */ "TME"_Stz,
  /*  9 */ "TSP"_Stz,
  /* 10 */ "VBN"_Stz,
  /* 11 */ "VCH"_Stz,
  
  /* 12 */ "ENUMS"_Stz,
};

//----------------------------------------------------------------------------

enum eLK // lock kind
{
  /*  0 */ LK_NONE=0,
  /*  1 */ LK_ACTION_LOG,
  /*  2 */ LK_API_STATE,
  /*  3 */ LK_BIT_ARRAY,
  /*  4 */ LK_BUFFER_POOL,
  /*  5 */ LK_COLLECTION,
  /*  6 */ LK_COMPLETER,
  /*  7 */ LK_CRWBUFFER,
  /*  8 */ LK_DATA,
  /*  9 */ LK_DDL,
  /* 10 */ LK_DIC,
  /* 11 */ LK_ENTRYBUFFER,
  /* 12 */ LK_EXPORT,
  /* 13 */ LK_HEAP,
  /* 14 */ LK_INDEX_TREE,
  /* 15 */ LK_JOBS,
  /* 16 */ LK_LIST,
  /* 17 */ LK_LOAD_MAP,
  /* 18 */ LK_OP,
  /* 19 */ LK_PARTITION,
  /* 20 */ LK_PIPE,
  /* 21 */ LK_QUEUE,
  /* 22 */ LK_SOCKET,
  /* 23 */ LK_SUBINSERTER,
  /* 24 */ LK_STEP_ARRAY,
  /* 25 */ LK_TASK,
  /* 26 */ LK_TASK_QUEUE,
  /* 27 */ LK_TASK_SET,
  /* 28 */ LK_TEMP_NAME,
  /* 29 */ LK_THREAD_QUEUE,
  /* 30 */ LK_THREAD_VECTOR,
  /* 31 */ LK_TRACE,
  /* 32 */ LK_TX_SET,
  /* 33 */ LK_UNKNOWN,
  /* 34 */ LK_ENUMS
};

static Stz* aLK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ACTION_LOG"_Stz,
  /*  2 */ "API_STATE"_Stz,
  /*  3 */ "BIT_ARRAY"_Stz,
  /*  4 */ "BUFFER_POOL"_Stz,
  /*  5 */ "COLLECTION"_Stz,
  /*  6 */ "COMPLETER"_Stz,
  /*  7 */ "CRWBUFFER"_Stz,
  /*  8 */ "DATA"_Stz,
  /*  9 */ "DDL"_Stz,
  /* 10 */ "DIC"_Stz,
  /* 11 */ "ENTRYBUFFER"_Stz,
  /* 12 */ "EXPORT"_Stz,
  /* 13 */ "HEAP"_Stz,
  /* 14 */ "INDEX_TREE"_Stz,
  /* 15 */ "JOBS"_Stz,
  /* 16 */ "LIST"_Stz,
  /* 17 */ "LOAD_MAP"_Stz,
  /* 18 */ "OP"_Stz,
  /* 19 */ "PARTITION"_Stz,
  /* 20 */ "PIPE"_Stz,
  /* 21 */ "QUEUE"_Stz,
  /* 22 */ "SOCKET"_Stz,
  /* 23 */ "SUBINSERTER"_Stz,
  /* 24 */ "STEP_ARRAY"_Stz,
  /* 25 */ "TASK"_Stz,
  /* 26 */ "TASK_QUEUE"_Stz,
  /* 27 */ "TASK_SET"_Stz,
  /* 28 */ "TEMP_NAME"_Stz,
  /* 29 */ "THREAD_QUEUE"_Stz,
  /* 30 */ "THREAD_VECTOR"_Stz,
  /* 31 */ "TRACE"_Stz,
  /* 32 */ "TX_SET"_Stz,
  /* 33 */ "UNKNOWN"_Stz,
  /* 34 */ "ENUMS"_Stz
};

enum eOC // other characters
{
  /*  0 */ OC_NONE=0,
  /*    */ OC_1,
  /*    */ OC_ENUMS,
};

static Stz* aOC[]
{
  /*  0 */ "NONE"_Stz,
  /*    */ "⬛️"_Stz,
  /*    */ "⬜️"_Stz,
  /*    */ "🛑"_Stz,
  /*    */ "🅰️"_Stz,
  /*    */ "🅱️"_Stz,
  /*    */ "🔵"_Stz,
  /*    */ "⚫️"_Stz,
  /*    */ "⚪️"_Stz,
  /*    */ "🔴"_Stz,
  /*    */ "⭕️"_Stz,
  /*    */ "❌"_Stz,
  /*    */ "💢"_Stz,
  /*    */ "↙️"_Stz,
  /*    */ "↘️"_Stz,
  /*    */ "🔷"_Stz,
  /*    */ "🔶"_Stz,
  /*    */ "♠️"_Stz,
  /*    */ "♣️"_Stz,
  /*    */ "♥️"_Stz,
  /*    */ "♦️"_Stz,
  /*    */ "🆗"_Stz,
  /*    */ "0️⃣"_Stz,
  /*    */ "9️⃣"_Stz,
  /*    */ "#️⃣"_Stz,
  /*    */ "*️⃣"_Stz,
  /*    */ "📈"_Stz,
  /*    */ "📉"_Stz,
  /*    */ "✅"_Stz,
  /*    */ "✅"_Stz,
  /*    */ "❎"_Stz,
  /*    */ "✳️"_Stz,
  /*    */ "🚹"_Stz,
  /*    */ "🚺"_Stz,
  /*    */ "⚛️"_Stz,
  /*    */ "❗️"_Stz,
  /*    */ "‼️"_Stz,
  /*    */ "❓"_Stz,
  /*    */ "⁉️"_Stz,
  /*    */ "➕"_Stz,
  /*    */ "➖"_Stz,
  /*    */ "➗"_Stz,
  /*    */ "✖️"_Stz,
  /*    */ "♾"_Stz,
  /*    */ "💲"_Stz,
  /*    */ "ENUMS"_Stz,
};


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
