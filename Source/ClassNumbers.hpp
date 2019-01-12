/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "Base.h"

#include "Stz.hpp"

namespace Base
{

/*----------------------------------------------------------------------------*/
/* class numbers                                                              */
/*----------------------------------------------------------------------------*/
enum eCN : u2 // Class Number
{
  /*   0 */ CN_NONE,
  /*   1 */ CN_BCDVAL,
  /*   3 */ CN_BOLVAL,
  /*   4 */ CN_DTEVAL,
  /*     */ CN_FIPVAL,
  /*   6 */ CN_FLTVAL,
  /*   7 */ CN_INTVAL,
  /*   9 */ CN_IVLVAL,
  /*  11 */ CN_PTRVAL,
  /*  12 */ CN_STRVAL,
  /*  14 */ CN_TMEVAL,
  /*  15 */ CN_TSPVAL,
  
  /*  19 */ CN_ACCESSPATH,
  /*  20 */ CN_ACCUM,
  /*  21 */ CN_ACCUMVEC,
  /*  21 */ CN_ACTIVITYMARK,
  /*     */ CN_APISTATE,
  /*  22 */ CN_ARG,
  /*  23 */ CN_ARGUMENTS,
  /*  24 */ CN_ATOMIC,
  /*  25 */ CN_ATTACHMAP,
  
  /*  26 */ CN_BITARRAY,
  /*  27 */ CN_BITMAP,
  /*  29 */ CN_BPA,
  /*  28 */ CN_BUFFERPOOL,
  /*     */ CN_BYTERATE,

  /*  30 */ CN_CARRYOVER,
  /*  31 */ CN_COLUMN,
  /*  32 */ CN_COLUMNVEC,
  /*  33 */ CN_COMPILER,
  /*  34 */ CN_COMMANDPROCESSOR,
  
  /*     */ CN_DECIDER,
  /*  35 */ CN_DIC,
  /*  36 */ CN_DICPATH,
  /*  37 */ CN_DICSTACK,
  /*  38 */ CN_DIR,
  
  /*  39 */ CN_ENTRY,
  /*  40 */ CN_ENTRYBUFFER,
  /*  41 */ CN_ENTRYVEC,
  
  /*  42 */ CN_FETCHBUFFER,
  /*  43 */ CN_FILE,
  /*  44 */ CN_FORMATTERS,
  /*  45 */ CN_FREEBUFFER,
  /*  46 */ CN_FREECHUNK,
  /*  47 */ CN_FREEDNOT,
  /*  48 */ CN_FREEENTRY,
  
  /*     */ CN_GENVAR,
  /*  49 */ CN_GRAPH,
  /*  50 */ CN_GRAPHCURSOR,
  /*  51 */ CN_GRAPHNODE,
  /*  52 */ CN_GRID,
  /*  53 */ CN_GROUPBY,
  
  /*  54 */ CN_HEAPBUFFER,
  
  /*  55 */ CN_INDEX,
  /*  56 */ CN_INDEXTREE,
  /*  57 */ CN_INDEXTREEVEC,
  /*  58 */ CN_IDXCOLVEC,
  /*  59 */ CN_IDXVEC,
  /*     */ CN_INGESTCHUNK,
  /*     */ CN_IOCHUNK,
  /*     */ CN_IOCOPY,
  /*  61 */ CN_ITEM,
  /*  62 */ CN_ITEMBUFFER,
  
  /*  63 */ CN_JOB__BEGUN,
  /*  64 */ CN_JOB__CANCEL,
  /*  65 */ CN_JOB__CANCELLED,
  /*  66 */ CN_JOB__END,
  /*  67 */ CN_JOB__ENDED,
  /*  68 */ CN_JOB__EXECUTE,
  /*  69 */ CN_JOB__EXECUTED,
  /*     */ CN_JOB__FIND,
  /*     */ CN_JOB_ALARM,
  /*  70 */ CN_JOB_ANALYZEINDEX,
  /*  71 */ CN_JOB_ANALYZETABLE,
  /*  72 */ CN_JOB_ANSWER,
  /*  73 */ CN_JOB_BOOT,
  /*     */ CN_JOB_CAT,
  /*  74 */ CN_JOB_CF,
  /*  75 */ CN_JOB_COMPACTINDEX,
  /*  76 */ CN_JOB_CONNECTCLUSTER,
  /*  77 */ CN_JOB_CONNECTOR,
  /*  78 */ CN_JOB_CREATECATALOG,
  /*  79 */ CN_JOB_CREATEINDEX,
  /*  80 */ CN_JOB_CREATESCHEMA,
  /*  81 */ CN_JOB_CREATETABLE,
  /*  82 */ CN_JOB_CREATETENANT,
  /*  83 */ CN_JOB_DOCOL,
  /*  84 */ CN_JOB_DROPCATALOG,
  /*  85 */ CN_JOB_DROPINDEX,
  /*  86 */ CN_JOB_DROPSCHEMA,
  /*  87 */ CN_JOB_DROPTABLE,
  /*  88 */ CN_JOB_DROPTENANT,
  /*  89 */ CN_JOB_DUMPINDEX,
  /*     */ CN_JOB_EXPORT,
  /*  91 */ CN_JOB_FETCHROWS,
  /*     */ CN_JOB_GENERATE,
  /*     */ CN_JOB_INGEST,
  /*  93 */ CN_JOB_INSERTROW,
  /*     */ CN_JOB_INSERTTABLE,
  /*  94 */ CN_JOB_LF,
  /*  95 */ CN_JOB_LISTCATALOGS,
  /*  96 */ CN_JOB_LISTCOLUMNS,
  /*  97 */ CN_JOB_LISTIDXCOLUMNS,
  /*  98 */ CN_JOB_LISTINDEXES,
  /*  99 */ CN_JOB_LISTPARTCOLUMNS,
  /* 100 */ CN_JOB_LISTPARTITIONS,
  /* 101 */ CN_JOB_LISTSCHEMAS,
  /* 102 */ CN_JOB_LISTTABLES,
  /* 103 */ CN_JOB_LISTTENANTS,
  /* 105 */ CN_JOB_LOADTABLE,
  /*     */ CN_JOB_NETHOG,
  /*     */ CN_JOB_NODEINFO,
  /*     */ CN_JOB_PERIODIC,
  /* 106 */ CN_JOB_PING,
  /*     */ CN_JOB_PUBLISH,
  /*     */ CN_JOB_PURGE,
  /* 108 */ CN_JOB_PWF,
  /* 109 */ CN_JOB_RECEIVER,
  /*     */ CN_JOB_REPEATER,
  /* 110 */ CN_JOB_SANITY,
  /*     */ CN_JOB_SELECTTABLE,
  /* 111 */ CN_JOB_SHOWCLUSTER,
  /*     */ CN_JOB_SINK,
  /* 112 */ CN_JOB_SIZEINDEX,
  /* 113 */ CN_JOB_SIZETABLE,
  /* 114 */ CN_JOB_SPACE,
  /* 115 */ CN_JOB_START,
  /* 116 */ CN_JOB_STARTCLUSTER,
  /* 117 */ CN_JOB_STATUS,
  /* 118 */ CN_JOB_SUBFETCH,
  /*     */ CN_JOB_SUBINGESTER,
  /*     */ CN_JOB_SUBINSERTER,
  /* 119 */ CN_JOB_SUBLOADER,
  /*     */ CN_JOB_SUBPUBLISH,
  /* 120 */ CN_JOB_SUBPURGER,
  /* 120 */ CN_JOB_SUBTRUNCATE,
  /* 121 */ CN_JOB_TABLESIZES,
  /* 122 */ CN_JOB_TRUNCATETABLE,
  /* 123 */ CN_JOB_USESCHEMA,
  /* 124 */ CN_JOB_VERIFYINDEX,
  /* 125 */ CN_JOB_VISUALIZEINDEX,
  /* 126 */ CN_JOB_WAITCLUSTER,
  /*     */ CN_JOB_WRITEBUFFER,
  /*     */ CN_JOB_WRITEROW,
  /* 127 */ CN_JOBS,
  /* 128 */ CN_JUMBOPERSISTENT,
  /* 129 */ CN_JUMBOTRANSIENT,
  
  /* 130 */ CN_KEY,
  /* 131 */ CN_KEYVAL,
  /* 132 */ CN_KEYVEC,
  /* 133 */ CN_KEYWRK,
  
  /* 134 */ CN_LARGEPERSISTENT,
  /* 135 */ CN_LARGETRANSIENT,
  /* 136 */ CN_LINEITEM,
  /*     */ CN_LIST,
  /*     */ CN_LOADCHUNK,
  /* 140 */ CN_LOADMAP,
  /* 141 */ CN_LOCK,
  
  /* 142 */ CN_MEDIUMPERSISTENT,
  /* 143 */ CN_MEDIUMTRANSIENT,
  
  /* 144 */ CN_NODE,
  /*     */ CN_NOW,

  /* 145 */ CN_OP,
  
  /* 146 */ CN_PARSER,
  /* 147 */ CN_PATHVEC,
  /* 148 */ CN_PERFORMANCEBUFFER,
  /* 149 */ CN_PERFORMANCEENTRY,
  /* 150 */ CN_PERFORMANCETRACE,
  /*     */ CN_PIPE,
  /* 151 */ CN_PLAN,
  /* 152 */ CN_PLANNER,
  /* 154 */ CN_POSIXTHREAD,
  /* 155 */ CN_PREDICATE,
  /* 156 */ CN_PREDICATEVEC,
  /* 157 */ CN_PTNCOLVEC,
  
  /* 158 */ CN_QUEUE,
  /* 159 */ CN_QUEUEBUFFER,
  /* 160 */ CN_QUEUES,
  
  /* 161 */ CN_RBCURSOR,
  /* 162 */ CN_REGFRAME,
  /*     */ CN_REGISTER,
  /*     */ CN_RESOURCETRACKER,
  /* 163 */ CN_RING,
  /* 164 */ CN_ROWAGG,
  /* 166 */ CN_ROWCURSOR,
  /* 167 */ CN_ROWONCE,
  /* 168 */ CN_ROWWRK,
  
  /* 169 */ CN_SCANBUFFER,
  /*     */ CN_SCHEMATEST,
  /* 170 */ CN_SCRATCH,
  /* 171 */ CN_SELCURSOR,
  /* 172 */ CN_SEMAPHORE,
  /* 173 */ CN_SEPROMPT,
  /* 174 */ CN_SESSION,
  /* 175 */ CN_SINCE,
  /* 176 */ CN_SKIPCURSOR,
  /* 177 */ CN_SKIPLIST,
  /* 178 */ CN_SKIPNODE,
  /* 179 */ CN_SLAB,
  /* 180 */ CN_SLABALLOCATOR,
  /* 181 */ CN_SLOTS,
  /* 182 */ CN_SMALLPERSISTENT,
  /* 183 */ CN_SMALLTRANSIENT,
  /* 184 */ CN_SOCKET,
  /* 185 */ CN_STACK,
  /* 186 */ CN_STEPACCUM,
  /* 187 */ CN_STEPTIMING,
  /*  13 */ CN_STRING_T,
  /* 188 */ CN_SYSCALL,
  
  /* 189 */ CN_TABLE,
  /* 190 */ CN_TABLEPTN,
  /* 191 */ CN_TABLEPTNVEC,
  /* 192 */ CN_TASK,
  /*     */ CN_TASKLIST,
  /* 193 */ CN_TASKQUEUE,
  /* 194 */ CN_THREAD,
  /* 195 */ CN_THREADVECTOR,
  /* 196 */ CN_TIME,
  /* 197 */ CN_TIMER,
  /* 198 */ CN_TOKEN,
  /* 199 */ CN_TOKENISER,
  /* 200 */ CN_TOKENARY,
  /* 201 */ CN_TOKENVEC,
  /* 202 */ CN_TX,
  /* 203 */ CN_TXSET,
  
  /* 204 */ CN_WILDERNESS,
  /* 205 */ CN_WORD,
  
  /* 208 */ CN_ZSMALL_,
  /* 209 */ CN_ZMEDIUM_,
  /* 210 */ CN_ZLARGE_,
  /* 211 */ CN_ZJUMBO_,
  
  /* 212 */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /*   0 */ "NONE"_Stz,                // CN_NONE
  /*   1 */ "BCDVAL"_Stz,              // CN_BCDVAL
  /*   3 */ "BOLVAL"_Stz,              // CN_BOLVAL
  /*   4 */ "DTEVAL"_Stz,              // CN_DTEVAL
  /*   8 */ "FIPVAL"_Stz,              // CN_FIPVAL
  /*   6 */ "FLTVAL"_Stz,              // CN_FLTVAL
  /*   7 */ "INTVAL"_Stz,              // CN_INTVAL
  /*   9 */ "IVLVAL"_Stz,              // CN_IVLVAL
  /*  11 */ "PTRVAL"_Stz,              // CN_PTRVAL
  /*  12 */ "STRVAL"_Stz,              // CN_STRVAL
  /*  14 */ "TMEVAL"_Stz,              // CN_TMEVAL
  /*  15 */ "TSPVAL"_Stz,              // CN_TSPVAL
  
  /*  19 */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
  /*  20 */ "ACCUM"_Stz,               // CN_ACCUM
  /*  21 */ "ACCUMVEC"_Stz,            // CN_ACCUMVEC
  /*  21 */ "ACTIVITYMARK"_Stz,        // CN_ACTIVITYMARK
  /*     */ "APISTATE"_Stz,            // CN_APISTATE
  /*  22 */ "ARG"_Stz,                 // CN_ARG
  /*  23 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /*  24 */ "ATOMIC"_Stz,              // CN_ATOMIC
  /*  25 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
  
  /*  26 */ "BITARRAY"_Stz,            // CN_BITARRAY
  /*  27 */ "BITMAP"_Stz,              // CN_BITMAP
  /*  29 */ "BPA"_Stz,                 // CN_BPA
  /*  28 */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /*  28 */ "BYTERATE"_Stz,            // CN_BYTERATE

  /*  30 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /*  31 */ "COLUMN"_Stz,              // CN_COLUMN
  /*  32 */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
  /*  33 */ "COMPILER"_Stz,            // CN_COMPILER
  /*  34 */ "COMMANDPROCESSOR"_Stz,    // CN_COMMANDPROCESSOR
  
  /*     */ "DECIDER"_Stz,             // CN_DECIDER
  /*  35 */ "DIC"_Stz,                 // CN_DIC
  /*  36 */ "DICPATH"_Stz,             // CN_DICPATH
  /*  37 */ "DICSTACK"_Stz,            // CN_DICSTACK
  /*  38 */ "DIR"_Stz,                 // CN_DIR
  
  /*  39 */ "ENTRY"_Stz,               // CN_ENTRY
  /*  40 */ "ENTRYBUFFER"_Stz,         // CN_ENTRYBUFFER
  /*  41 */ "ENTRYVEC"_Stz,            // CN_ENTRYVEC
  
  /*  42 */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /*  43 */ "FILE"_Stz,                // CN_FILE
  /*  44 */ "FORMATTERS"_Stz,          // CN_FORMATTERS
  /*  45 */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /*  46 */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /*  47 */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /*  48 */ "FREEENTRY"_Stz,           // CN_FREEENTRY
  
  /*     */ "GENVAR"_Stz,              // CN_GENVAR
  /*  49 */ "GRAPH"_Stz,               // CN_GRAPH
  /*  50 */ "GRAPHCURSOR"_Stz,         // CN_GRAPHCURSOR
  /*  51 */ "GRAPHNODE"_Stz,           // CN_GRAPHNODE
  /*  52 */ "GRID"_Stz,                // CN_GRID
  /*  53 */ "GROUPBY"_Stz,             // CN_GROUPBY
  
  /*  54 */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  
  /*  55 */ "INDEX"_Stz,               // CN_INDEX
  /*  56 */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /*  57 */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /*  58 */ "IDXCOLVEC"_Stz,           // CN_IDXCOLVEC
  /*  59 */ "IDXVEC"_Stz,              // CN_IDXVEC
  /*     */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /*     */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /*     */ "IOCOPY"_Stz,              // CN_IOCOPY
  /*  61 */ "ITEM"_Stz,                // CN_ITEM
  /*  62 */ "ITEMBUFFER"_Stz,          // CN_ITEMBUFFER
  
  /*  63 */ "JOB__BEGUN"_Stz,          // CN_JOB__BEGUN
  /*  64 */ "JOB__CANCEL"_Stz,         // CN_JOB__CANCEL
  /*  65 */ "JOB__CANCELLED"_Stz,      // CN_JOB__CANCELLED
  /*  66 */ "JOB__END"_Stz,            // CN_JOB__END
  /*  67 */ "JOB__ENDED"_Stz,          // CN_JOB__ENDED
  /*  68 */ "JOB__EXECUTE"_Stz,        // CN_JOB__EXECUTE
  /*  69 */ "JOB__EXECUTED"_Stz,       // CN_JOB__EXECUTED
  /*     */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /*     */ "JOB_ALARM"_Stz,           // CN_JOB_ALARM
  /*  70 */ "JOB_ANALYZEINDEX"_Stz,    // CN_JOB_ANALYZEINDEX
  /*  71 */ "JOB_ANALYZETABLE"_Stz,    // CN_JOB_ANALYZETABLE
  /*  72 */ "JOB_ANSWER"_Stz,          // CN_JOB_ANSWER
  /*  73 */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
  /*     */ "JOB_CAT"_Stz,             // CN_JOB_CAT
  /*  74 */ "JOB_CF"_Stz,              // CN_JOB_CF
  /*  75 */ "JOB_COMPACTINDEX"_Stz,    // CN_JOB_COMPACTINDEX
  /*  76 */ "JOB_CONNECTCLUSTER"_Stz,  // CN_JOB_CONNECTCLUSTER
  /*  77 */ "JOB_CONNECTOR"_Stz,       // CN_JOB_CONNECTOR
  /*  78 */ "JOB_CREATECATALOG"_Stz,   // CN_JOB_CREATECATALOG
  /*  79 */ "JOB_CREATEINDEX"_Stz,     // CN_JOB_CREATEINDEX
  /*  80 */ "JOB_CREATESCHEMA"_Stz,    // CN_JOB_CREATESCHEMA
  /*  81 */ "JOB_CREATETABLE"_Stz,     // CN_JOB_CREATETABLE
  /*  82 */ "JOB_CREATETENANT"_Stz,    // CN_JOB_CREATETENANT
  /*  83 */ "JOB_DOCOL"_Stz,           // CN_JOB_DOCOL
  /*  84 */ "JOB_DROPCATALOG"_Stz,     // CN_JOB_DROPCATALOG
  /*  85 */ "JOB_DROPINDEX"_Stz,       // CN_JOB_DROPINDEX
  /*  86 */ "JOB_DROPSCHEMA"_Stz,      // CN_JOB_DROPSCHEMA
  /*  87 */ "JOB_DROPTABLE"_Stz,       // CN_JOB_DROPTABLE
  /*  88 */ "JOB_DROPTENANT"_Stz,      // CN_JOB_DROPTENANT
  /*  89 */ "JOB_DUMPINDEX"_Stz,       // CN_JOB_DUMPINDEX
  /*     */ "JOB_EXPORT"_Stz,          // CN_JOB_EXPORT
  /*  91 */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
  /*     */ "JOB_GENDATA"_Stz,         // CN_JOB_GENERATE
  /*     */ "JOB_INGEST"_Stz,          // CN_JOB_INGEST
  /*  93 */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
  /*     */ "Job_INSERTTABLE"_Stz,     // CN_JOB_INSERTTABLE
  /*  94 */ "JOB_LF"_Stz,              // CN_JOB_LF
  /*  95 */ "JOB_LISTCATALOGS"_Stz,    // CN_JOB_LISTCATALOGS
  /*  96 */ "JOB_LISTCOLUMNS"_Stz,     // CN_JOB_LISTCOLUMNS
  /*  97 */ "JOB_LISTIDXCOLUMNS"_Stz,  // CN_JOB_LISTIDXCOLUMNS
  /*  98 */ "JOB_LISTINDEXES"_Stz,     // CN_JOB_LISTINDEXES
  /*  99 */ "JOB_LISTPARTCOLUMNS"_Stz, // CN_JOB_LISTPARTCOLUMNS
  /* 100 */ "JOB_LISTPARTITIONS"_Stz,  // CN_JOB_LISTPARTITIONS
  /* 101 */ "JOB_LISTSCHEMAS"_Stz,     // CN_JOB_LISTSCHEMAS
  /* 102 */ "JOB_LISTTABLES"_Stz,      // CN_JOB_LISTTABLES
  /* 103 */ "JOB_LISTTENANTS"_Stz,     // CN_JOB_LISTTENANTS
  /* 105 */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
  /*     */ "JOB_NETHOG"_Stz,          // CN_JOB_NETHOG
  /*     */ "JOB_NODEINFO"_Stz,        // CN_JOB_NODEINFO
  /*     */ "JOB_PERIODIC"_Stz,        // CN_JOB_PERIODIC
  /* 106 */ "JOB_PING"_Stz,            // CN_JOB_PING
  /*     */ "JOB_PUBLISH"_Stz,         // CN_JOB_PUBLISH
  /*     */ "JOB_PURGE"_Stz,           // CN_JOB_PURGE
  /* 108 */ "JOB_PWF"_Stz,             // CN_JOB_PWF
  /* 109 */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
  /*     */ "JOB_REPEATER"_Stz,        // CN_JOB_REPEATER
  /* 110 */ "JOB_SELECTTABLE"_Stz,     // CN_JOB_SELECTTABLE
  /*     */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
  /* 111 */ "JOB_SHOWCLUSTER"_Stz,     // CN_JOB_SHOWCLUSTER
  /*     */ "JOB_SINK"_Stz,            // CN_JOB_SINK
  /* 112 */ "JOB_SIZEINDEX"_Stz,       // CN_JOB_SIZEINDEX
  /* 113 */ "JOB_SIZETABLE"_Stz,       // CN_JOB_SIZETABLE
  /* 114 */ "JOB_SPACE"_Stz,           // CN_JOB_SPACE
  /* 115 */ "JOB_START"_Stz,           // CN_JOB_START
  /* 116 */ "JOB_STARTCLUSTER"_Stz,    // CN_JOB_STARTCLUSTER
  /* 117 */ "JOB_STATUS"_Stz,          // CN_JOB_STATUS
  /* 118 */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
  /*     */ "JOB_SUBINGESTER"_Stz,     // CN_JOB_SUBINGESTER
  /*     */ "Job_SUBINSERTER"_Stz,     // CN_JOB_SUBINSERTER
  /* 119 */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
  /*     */ "JOB_SUBPUBLISH"_Stz,      // CN_JOB_SUBPUBLISH
  /* 120 */ "JOB_SUBPURGER"_Stz,       // CN_JOB_SUBPURGER
  /* 120 */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
  /* 121 */ "JOB_TABLESIZES"_Stz,      // CN_JOB_TABLESIZES
  /* 122 */ "JOB_TRUNCATETABLE"_Stz,   // CN_JOB_TRUNCATETABLE
  /* 123 */ "JOB_USESCHEMA"_Stz,       // CN_JOB_USESCHEMA
  /* 124 */ "JOB_VERIFYINDEX"_Stz,     // CN_JOB_VERIFYINDEX
  /* 125 */ "JOB_VISUALIZEINDEX"_Stz,  // CN_JOB_VISUALIZEINDEX
  /* 126 */ "JOB_WAITCLUSTER"_Stz,     // CN_JOB_WAITCLUSTER
  /*     */ "JOB_WRITEBUFFER"_Stz,     // CN_JOB_WRITEBUFFER
  /*     */ "JOB_WRITEROW"_Stz,        // CN_JOB_WRITEROW
  /* 127 */ "JOBS"_Stz,                // CN_JOBS
  /* 128 */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
  /* 129 */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
  
  /* 130 */ "KEY"_Stz,                 // CN_KEY
  /* 131 */ "KEYVAL"_Stz,              // CN_KEYVAL
  /* 132 */ "KEYVEC"_Stz,              // CN_KEYVEC
  /* 133 */ "KEYWRK"_Stz,              // CN_KEYWRK
  
  /* 134 */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* 135 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* 136 */ "LINEITEM"_Stz,            // CN_LINEITEM
  /*     */ "LIST"_Stz,                // CN_LIST
  /*     */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* 140 */ "LOADMAP"_Stz,             // CN_LOADMAP
  /* 141 */ "LOCK"_Stz,                // CN_LOCK
  
  /* 142 */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* 143 */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  
  /* 144 */ "NODE"_Stz,                // CN_NODE
  /*     */ "NOW"_Stz,                 // CN_NOW

  /* 145 */ "OP"_Stz,                  // CN_OP
  
  /* 146 */ "PARSER"_Stz,              // CN_PARSER
  /* 147 */ "PATHVEC"_Stz,             // CN_PATHVEC
  /* 148 */ "PERFORMANCEBUFFER"_Stz,   // CN_PERFORMANCEBUFFER
  /* 149 */ "PERFORMANCEENTRY"_Stz,    // CN_PERFORMANCEENTRY
  /* 150 */ "PERFORMANCETRACE"_Stz,    // CN_PERFORMANCETRACE
  /*     */ "PIPE"_Stz,                // CN_PIPE
  /* 151 */ "PLAN"_Stz,                // CN_PLAN
  /* 152 */ "PLANNER"_Stz,             // CN_PLANNER
  /* 154 */ "POSIXTHREAD"_Stz,         // CN_POSIXTHREAD
  /* 155 */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* 156 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* 157 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  
  /* 158 */ "QUEUE"_Stz,               // CN_QUEUE
  /* 159 */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* 160 */ "QUEUES"_Stz,              // CN_QUEUES
  
  /* 161 */ "RBCURSOR"_Stz,            // CN_RBCURSOR
  /* 162 */ "REGFRAME"_Stz,            // CN_REGFRAME
  /*     */ "REGISTER"_Stz,            // CN_REGISTER
  /*     */ "RESOURCETRACK"_Stz,       // CN_RESOURCETRACKER
  /* 163 */ "RING"_Stz,                // CN_RING
  /* 164 */ "ROWAGG"_Stz,              // CN_ROWAGG
  /* 166 */ "ROWCURSOR"_Stz,           // CN_ROWCURSOR
  /* 167 */ "ROWONCE"_Stz,             // CN_ROWONCE
  /* 168 */ "ROWWRK"_Stz,              // CN_ROWWRK
  
  /* 169 */ "SCANBUFFER"_Stz,          // CN_SCANBUFFER
  /*     */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /* 170 */ "SCRATCH"_Stz,             // CN_SCRATCH
  /* 171 */ "SELCURSOR"_Stz,           // CN_SELCURSOR
  /* 172 */ "SEMAPHORE"_Stz,           // CN_SEMAPHORE
  /* 173 */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* 174 */ "SESSION"_Stz,             // CN_SESSION
  /* 175 */ "SINCE"_Stz,               // CN_SINCE
  /* 176 */ "SKIPCURSOR"_Stz,          // CN_SKIPCURSOR
  /* 177 */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* 178 */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* 179 */ "SLAB"_Stz,                // CN_SLAB
  /* 180 */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* 181 */ "SLOTS"_Stz,               // CN_SLOTS
  /* 182 */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* 183 */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* 184 */ "SOCKET"_Stz,              // CN_SOCKET
  /* 185 */ "STACK"_Stz,               // CN_STACK
  /* 186 */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* 187 */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /*  13 */ "STRING_T"_Stz,            // CN_STRING_T
  /* 188 */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* 189 */ "TABLE"_Stz,               // CN_TABLE
  /* 190 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* 191 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* 192 */ "TASK"_Stz,                // CN_TASK
  /*     */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* 193 */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /* 194 */ "THREAD"_Stz,              // CN_THREAD
  /* 195 */ "THREADVECTOR"_Stz,        // CN_THREADVECTOR
  /* 196 */ "TIME"_Stz,                // CN_TIME
  /* 197 */ "TIMER"_Stz,               // CN_TIMER
  /* 198 */ "TOKEN"_Stz,               // CN_TOKEN
  /* 199 */ "TOKENISER"_Stz,           // CN_TOKENISER
  /* 200 */ "TOKENARY"_Stz,            // CN_TOKENARY
  /* 201 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* 202 */ "TX"_Stz,                  // CN_TX
  /* 203 */ "TXSET"_Stz,               // CN_TXSET
  
  /* 204 */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* 205 */ "WORD"_Stz,                // CN_WORD
  
  /* 208 */ "ZSMALL_"_Stz,             // CN_ZSMALL_
  /* 209 */ "ZMEDIUM_"_Stz,            // CN_ZMEDIUM_
  /* 210 */ "ZLARGE_"_Stz,             // CN_ZLARGE_
  /* 211 */ "ZJUMBO_"_Stz,             // CN_ZJUMBO_
  
  /* 212 */ "ENUMS"_Stz,               // CN_ENUMS
};

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
