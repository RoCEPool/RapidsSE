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
    /*   2 */ CN_BINVAL,
    /*   3 */ CN_BOLVAL,
    /*   4 */ CN_DTEVAL,
    /*   5 */ CN_ENTWRK,
    /*   6 */ CN_FLTVAL,
    /*   7 */ CN_INTVAL,
    /*   8 */ CN_IPVAL,
    /*   9 */ CN_IVLVAL,
    /*  10 */ CN_OBJVAL,
    /*  11 */ CN_PTRVAL,
    /*  12 */ CN_STRVAL,
    /*  13 */ CN_STRVAR,
    /*  14 */ CN_TMEVAL,
    /*  15 */ CN_TSPVAL,
    /*  16 */ CN_VALREG,
    /*  17 */ CN_VALSET,
    /*  18 */ CN_VALWRK,
    
    /*  19 */ CN_ACCESSPATH,
    /*  20 */ CN_ACCUM,
    /*  21 */ CN_ACCUMVEC,
    /*  22 */ CN_ARG,
    /*  23 */ CN_ARGUMENTS,
    /*  24 */ CN_ATOMIC,
    /*  25 */ CN_ATTACHMAP,
    
    /*  26 */ CN_BITARRAY,
    /*  27 */ CN_BITMAP,
    /*  28 */ CN_BUFFERPOOL,
    /*  29 */ CN_BPA,
    
    /*  30 */ CN_CARRYOVER,
    /*  31 */ CN_COLUMN,
    /*  32 */ CN_COLUMNVEC,
    /*  33 */ CN_COMPILER,
    /*  34 */ CN_COMMANDPROCESSOR,
    
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
    /*  60 */ CN_INDEXLEVELS,
    /*  61 */ CN_ITEM,
    /*  62 */ CN_ITEMBUFFER,
    
    /*  63 */ CN_JOB__BEGUN,
    /*  64 */ CN_JOB__CANCEL,
    /*  65 */ CN_JOB__CANCELLED,
    /*  66 */ CN_JOB__END,
    /*  67 */ CN_JOB__ENDED,
    /*  68 */ CN_JOB__EXECUTE,
    /*  69 */ CN_JOB__EXECUTED,
    /*  70 */ CN_JOB_ANALYZEINDEX,
    /*  71 */ CN_JOB_ANALYZETABLE,
    /*  72 */ CN_JOB_ANSWER,
    /*  73 */ CN_JOB_BOOT,
    /*  74 */ CN_JOB_CF,
    /*  75 */ CN_JOB_COMPACTINDEX,
    /*     */ CN_JOB_CONNECTCLUSTER,
    /*  76 */ CN_JOB_CONNECTOR,
    /*  77 */ CN_JOB_CREATECATALOG,
    /*  78 */ CN_JOB_CREATEINDEX,
    /*  79 */ CN_JOB_CREATESCHEMA,
    /*  80 */ CN_JOB_CREATETABLE,
    /*  81 */ CN_JOB_CREATETENANT,
    /*  82 */ CN_JOB_DOCOL,
    /*  83 */ CN_JOB_DROPCATALOG,
    /*  84 */ CN_JOB_DROPINDEX,
    /*  85 */ CN_JOB_DROPSCHEMA,
    /*  86 */ CN_JOB_DROPTABLE,
    /*  87 */ CN_JOB_DROPTENANT,
    /*  88 */ CN_JOB_DUMPINDEX,
    /*  89 */ CN_JOB_ENDINSERT,
    /*  90 */ CN_JOB_FETCHROWS,
    /*  91 */ CN_JOB_INSERTBUFFER,
    /*  92 */ CN_JOB_INSERTROW,
    /*  93 */ CN_JOB_LF,
    /*  94 */ CN_JOB_LISTCATALOGS,
    /*  95 */ CN_JOB_LISTCOLUMNS,
    /*  96 */ CN_JOB_LISTIDXCOLUMNS,
    /*  97 */ CN_JOB_LISTINDEXES,
    /*  98 */ CN_JOB_LISTPARTCOLUMNS,
    /*  99 */ CN_JOB_LISTPARTITIONS,
    /* 100 */ CN_JOB_LISTSCHEMAS,
    /* 101 */ CN_JOB_LISTTABLES,
    /* 102 */ CN_JOB_LISTTENANTS,
    /* 103 */ CN_JOB_LOADBUFFER,
    /* 104 */ CN_JOB_LOADTABLE,
    /* 105 */ CN_JOB_PING,
    /* 106 */ CN_JOB_PROMPT,
    /* 107 */ CN_JOB_PWF,
    /* 108 */ CN_JOB_RECEIVER,
    /* 109 */ CN_JOB_SANITY,
    /* 110 */ CN_JOB_SHOWCLUSTER,
    /* 111 */ CN_JOB_SIZEINDEX,
    /* 112 */ CN_JOB_SIZETABLE,
    /* 113 */ CN_JOB_SPACE,
    /* 114 */ CN_JOB_START,
    /* 115 */ CN_JOB_STARTCLUSTER,
    /* 116 */ CN_JOB_STATUS,
    /* 117 */ CN_JOB_SUBFETCH,
    /* 118 */ CN_JOB_SUBLOADER,
    /* 119 */ CN_JOB_SUBTRUNCATE,
    /* 120 */ CN_JOB_TABLESIZES,
    /* 121 */ CN_JOB_TRUNCATETABLE,
    /* 122 */ CN_JOB_USESCHEMA,
    /* 123 */ CN_JOB_VERIFYINDEX,
    /* 124 */ CN_JOB_VISUALIZEINDEX,
    /* 125 */ CN_JOB_WAITCLUSTER,
    /* 126 */ CN_JOBS,
    /* 127 */ CN_JUMBOPERSISTENT,
    /* 128 */ CN_JUMBOTRANSIENT,
    
    /* 129 */ CN_KEY,
    /* 130 */ CN_KEYVAL,
    /* 131 */ CN_KEYVEC,
    /* 132 */ CN_KEYWRK,
    
    /* 133 */ CN_LARGEPERSISTENT,
    /* 134 */ CN_LARGETRANSIENT,
    /* 135 */ CN_LINEITEM,
    /* 136 */ CN_LOADCHUNK,
    /* 137 */ CN_LOADTABLE,
    /* 138 */ CN_SUBLOADER,
    /* 139 */ CN_LOADMAP,
    /* 140 */ CN_LOCK,
    
    /* 141 */ CN_MEDIUMPERSISTENT,
    /* 142 */ CN_MEDIUMTRANSIENT,
    
    /* 143 */ CN_NODE,
    
    /* 144 */ CN_OP,
    
    /* 145 */ CN_PARSER,
    /* 146 */ CN_PATHVEC,
    /* 147 */ CN_PERFORMANCEBUFFER,
    /* 148 */ CN_PERFORMANCEENTRY,
    /* 149 */ CN_PERFORMANCETRACE,
    /* 150 */ CN_PLAN,
    /* 151 */ CN_PLANNER,
    /* 152 */ CN_PLANLIST,
    /* 153 */ CN_POSIXTHREAD,
    /* 154 */ CN_PREDICATE,
    /* 155 */ CN_PREDICATEVEC,
    /* 156 */ CN_PTNCOLVEC,
    
    /* 157 */ CN_QUEUE,
    /* 158 */ CN_QUEUEBUFFER,
    /* 159 */ CN_QUEUES,
    
    /* 160 */ CN_RBCURSOR,
    /* 161 */ CN_REGFRAME,
    /* 162 */ CN_RING,
    /* 163 */ CN_ROARRAY,
    /* 164 */ CN_ROWBUFFER,
    /* 165 */ CN_ROWCURSOR,
    /* 166 */ CN_ROWONCE,
    /* 167 */ CN_ROWWRK,
    
    /* 168 */ CN_SCANBUFFER,
    /* 169 */ CN_SCRATCH,
    /* 170 */ CN_SEITF,
    /* 171 */ CN_SELCURSOR,
    /* 172 */ CN_SEMAPHORE,
    /* 173 */ CN_SEPROMPT,
    /* 174 */ CN_SINCE,
    /* 175 */ CN_SKIPCURSOR,
    /* 176 */ CN_SKIPLIST,
    /* 177 */ CN_SKIPNODE,
    /* 178 */ CN_SLAB,
    /* 179 */ CN_SLABALLOCATOR,
    /* 180 */ CN_SLOTS,
    /* 181 */ CN_SMALLPERSISTENT,
    /* 182 */ CN_SMALLTRANSIENT,
    /* 183 */ CN_SOCKET,
    /* 184 */ CN_STACK,
    /* 185 */ CN_STEPACCUM,
    /* 186 */ CN_STEPARRAY,
    /* 187 */ CN_SYSCALL,
    
    /* 188 */ CN_TABLE,
    /* 189 */ CN_TABLEPTN,
    /* 190 */ CN_TABLEPTNVEC,
    /* 191 */ CN_TASK,
    /* 192 */ CN_TASKQUEUE,
    /* 193 */ CN_THREAD,
    /* 194 */ CN_THREADVECTOR,
    /* 195 */ CN_TIME,
    /* 196 */ CN_TIMER,
    /* 197 */ CN_TOKEN,
    /* 198 */ CN_TOKENISER,
    /* 199 */ CN_TOKENARY,
    /* 200 */ CN_TOKENVEC,
    /* 201 */ CN_TX,
    /* 202 */ CN_TXSET,
    
    /* 203 */ CN_WILDERNESS,
    /* 204 */ CN_WORD,
    /* 205 */ CN_WORKINGBUFFER,
    /* 206 */ CN_WORKINGROW,
    
    /* 207 */ CN_ZSMALL_,
    /* 208 */ CN_ZMEDIUM_,
    /* 209 */ CN_ZLARGE_,
    /* 210 */ CN_ZJUMBO_,
    
    /* 211 */ CN_ENUMS,
  };
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
    /*   0 */ "NONE"_Stz,                // CN_NONE
    /*   1 */ "BCDVAL"_Stz,              // CN_BCDVAL
    /*   2 */ "BINVAL"_Stz,              // CN_BINVAL
    /*   3 */ "BOLVAL"_Stz,              // CN_BOLVAL
    /*   4 */ "DTEVAL"_Stz,              // CN_DTEVAL
    /*   5 */ "ENTWRK"_Stz,              // CN_ENTWRK
    /*   6 */ "FLTVAL"_Stz,              // CN_FLTVAL
    /*   7 */ "INTVAL"_Stz,              // CN_INTVAL
    /*   8 */ "IPVAL"_Stz,               // CN_IPVAL
    /*   9 */ "IVLVAL"_Stz,              // CN_IVLVAL
    /*  10 */ "OBJVAL"_Stz,              // CN_OBJVAL
    /*  11 */ "PTRVAL"_Stz,              // CN_PTRVAL
    /*  12 */ "STRVAL"_Stz,              // CN_STRVAL
    /*  13 */ "STRVAR"_Stz,              // CN_STRVAR
    /*  14 */ "TMEVAL"_Stz,              // CN_TMEVAL
    /*  15 */ "TSPVAL"_Stz,              // CN_TSPVAL
    /*  16 */ "VALREG"_Stz,              // CN_VALREG
    /*  17 */ "VALSET"_Stz,              // CN_VALSET
    /*  18 */ "VALWRK"_Stz,              // CN_VALWRK
    
    /*  19 */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
    /*  20 */ "ACCUM"_Stz,               // CN_ACCUM
    /*  21 */ "ACCUMVEC"_Stz,            // CN_ACCUMVEC
    /*  22 */ "ARG"_Stz,                 // CN_ARG
    /*  23 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
    /*  24 */ "ATOMIC"_Stz,              // CN_ATOMIC
    /*  25 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
    
    /*  26 */ "BITARRAY"_Stz,            // CN_BITARRAY
    /*  27 */ "BITMAP"_Stz,              // CN_BITMAP
    /*  28 */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
    /*  29 */ "BPA"_Stz,                 // CN_BPA
    
    /*  30 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
    /*  31 */ "COLUMN"_Stz,              // CN_COLUMN
    /*  32 */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
    /*  33 */ "COMPILER"_Stz,            // CN_COMPILER
    /*  34 */ "COMMANDPROCESSOR"_Stz,    // CN_COMMANDPROCESSOR
    
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
    /*  60 */ "INDEXLEVELS"_Stz,         // CN_INDEXLEVELS
    /*  61 */ "ITEM"_Stz,                // CN_ITEM
    /*  62 */ "ITEMBUFFER"_Stz,          // CN_ITEMBUFFER
    
    /*  63 */ "JOB__BEGUN"_Stz,          // CN_JOB__BEGUN
    /*  64 */ "JOB__CANCEL"_Stz,         // CN_JOB__CANCEL
    /*  65 */ "JOB__CANCELLED"_Stz,      // CN_JOB__CANCELLED
    /*  66 */ "JOB__END"_Stz,            // CN_JOB__END
    /*  67 */ "JOB__ENDED"_Stz,          // CN_JOB__ENDED
    /*  68 */ "JOB__EXECUTE"_Stz,        // CN_JOB__EXECUTE
    /*  69 */ "JOB__EXECUTED"_Stz,       // CN_JOB__EXECUTED
    /*  70 */ "JOB_ANALYZEINDEX"_Stz,    // CN_JOB_ANALYZEINDEX
    /*  71 */ "JOB_ANALYZETABLE"_Stz,    // CN_JOB_ANALYZETABLE
    /*  72 */ "JOB_ANSWER"_Stz,          // CN_JOB_ANSWER
    /*  73 */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
    /*  74 */ "JOB_CF"_Stz,              // CN_JOB_CF
    /*  75 */ "JOB_COMPACTINDEX"_Stz,    // CN_JOB_COMPACTINDEX
    /*     */ "JOB_CONNECTCLUSTER"_Stz,  // CN_JOB_CONNECTCLUSTER
    /*  76 */ "JOB_CONNECTOR"_Stz,       // CN_JOB_CONNECTOR
    /*  77 */ "JOB_CREATECATALOG"_Stz,   // CN_JOB_CREATECATALOG
    /*  78 */ "JOB_CREATEINDEX"_Stz,     // CN_JOB_CREATEINDEX
    /*  79 */ "JOB_CREATESCHEMA"_Stz,    // CN_JOB_CREATESCHEMA
    /*  80 */ "JOB_CREATETABLE"_Stz,     // CN_JOB_CREATETABLE
    /*  81 */ "JOB_CREATETENANT"_Stz,    // CN_JOB_CREATETENANT
    /*  82 */ "JOB_DOCOL"_Stz,           // CN_JOB_DOCOL
    /*  83 */ "JOB_DROPCATALOG"_Stz,     // CN_JOB_DROPCATALOG
    /*  84 */ "JOB_DROPINDEX"_Stz,       // CN_JOB_DROPINDEX
    /*  85 */ "JOB_DROPSCHEMA"_Stz,      // CN_JOB_DROPSCHEMA
    /*  86 */ "JOB_DROPTABLE"_Stz,       // CN_JOB_DROPTABLE
    /*  87 */ "JOB_DROPTENANT"_Stz,      // CN_JOB_DROPTENANT
    /*  88 */ "JOB_DUMPINDEX"_Stz,       // CN_JOB_DUMPINDEX
    /*  89 */ "JOB_ENDINSERT"_Stz,       // CN_JOB_ENDINSERT
    /*  90 */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
    /*  91 */ "JOB_INSERTBUFFER"_Stz,    // CN_JOB_INSERTBUFFER
    /*  92 */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
    /*  93 */ "JOB_LF"_Stz,              // CN_JOB_LF
    /*  94 */ "JOB_LISTCATALOGS"_Stz,    // CN_JOB_LISTCATALOGS
    /*  95 */ "JOB_LISTCOLUMNS"_Stz,     // CN_JOB_LISTCOLUMNS
    /*  96 */ "JOB_LISTIDXCOLUMNS"_Stz,  // CN_JOB_LISTIDXCOLUMNS
    /*  97 */ "JOB_LISTINDEXES"_Stz,     // CN_JOB_LISTINDEXES
    /*  98 */ "JOB_LISTPARTCOLUMNS"_Stz, // CN_JOB_LISTPARTCOLUMNS
    /*  99 */ "JOB_LISTPARTITIONS"_Stz,  // CN_JOB_LISTPARTITIONS
    /* 100 */ "JOB_LISTSCHEMAS"_Stz,     // CN_JOB_LISTSCHEMAS
    /* 101 */ "JOB_LISTTABLES"_Stz,      // CN_JOB_LISTTABLES
    /* 102 */ "JOB_LISTTENANTS"_Stz,     // CN_JOB_LISTTENANTS
    /* 103 */ "JOB_LOADBUFFER"_Stz,      // CN_JOB_LOADBUFFER
    /* 104 */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
    /* 105 */ "JOB_PING"_Stz,            // CN_JOB_PING
    /* 106 */ "JOB_PROMPT"_Stz,          // CN_JOB_PROMPT
    /* 107 */ "JOB_PWF"_Stz,             // CN_JOB_PWF
    /* 108 */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
    /* 109 */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
    /* 110 */ "JOB_SHOWCLUSTER"_Stz,     // CN_JOB_SHOWCLUSTER
    /* 111 */ "JOB_SIZEINDEX"_Stz,       // CN_JOB_SIZEINDEX
    /* 112 */ "JOB_SIZETABLE"_Stz,       // CN_JOB_SIZETABLE
    /* 113 */ "JOB_SPACE"_Stz,           // CN_JOB_SPACE
    /* 114 */ "JOB_START"_Stz,           // CN_JOB_START
    /* 115 */ "JOB_STARTCLUSTER"_Stz,    // CN_JOB_STARTCLUSTER
    /* 116 */ "JOB_STATUS"_Stz,          // CN_JOB_STATUS
    /* 117 */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
    /* 118 */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
    /* 119 */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
    /* 120 */ "JOB_TABLESIZES"_Stz,      // CN_JOB_TABLESIZES
    /* 121 */ "JOB_TRUNCATETABLE"_Stz,   // CN_JOB_TRUNCATETABLE
    /* 122 */ "JOB_USESCHEMA"_Stz,       // CN_JOB_USESCHEMA
    /* 123 */ "JOB_VERIFYINDEX"_Stz,     // CN_JOB_VERIFYINDEX
    /* 124 */ "JOB_VISUALIZEINDEX"_Stz,  // CN_JOB_VISUALIZEINDEX
    /* 125 */ "JOB_WAITCLUSTER"_Stz,     // CN_JOB_WAITCLUSTER
    /* 126 */ "JOBS"_Stz,                // CN_JOBS
    /* 127 */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
    /* 128 */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
    
    /* 129 */ "KEY"_Stz,                 // CN_KEY
    /* 130 */ "KEYVAL"_Stz,              // CN_KEYVAL
    /* 131 */ "KEYVEC"_Stz,              // CN_KEYVEC
    /* 132 */ "KEYWRK"_Stz,              // CN_KEYWRK
    
    /* 133 */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
    /* 134 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
    /* 135 */ "LINEITEM"_Stz,            // CN_LINEITEM
    /* 136 */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
    /* 137 */ "LOADER"_Stz,              // CN_LOADTABLE
    /* 138 */ "LOADERSUB"_Stz,           // CN_SUBLOADER
    /* 139 */ "LOADMAP"_Stz,             // CN_LOADMAP
    /* 140 */ "LOCK"_Stz,                // CN_LOCK
    
    /* 141 */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
    /* 142 */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
    
    /* 143 */ "NODE"_Stz,                // CN_NODE
    
    /* 144 */ "OP"_Stz,                  // CN_OP
    
    /* 145 */ "PARSER"_Stz,              // CN_PARSER
    /* 146 */ "PATHVEC"_Stz,             // CN_PATHVEC
    /* 147 */ "PERFORMANCEBUFFER"_Stz,   // CN_PERFORMANCEBUFFER
    /* 148 */ "PERFORMANCEENTRY"_Stz,    // CN_PERFORMANCEENTRY
    /* 149 */ "PERFORMANCETRACE"_Stz,    // CN_PERFORMANCETRACE
    /* 150 */ "PLAN"_Stz,                // CN_PLAN
    /* 151 */ "PLANNER"_Stz,             // CN_PLANNER
    /* 152 */ "PLANLIST"_Stz,            // CN_PLANLIST
    /* 153 */ "POSIXTHREAD"_Stz,         // CN_POSIXTHREAD
    /* 154 */ "PREDICATE"_Stz,           // CN_PREDICATE
    /* 155 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
    /* 156 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
    
    /* 157 */ "QUEUE"_Stz,               // CN_QUEUE
    /* 158 */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
    /* 159 */ "QUEUES"_Stz,              // CN_QUEUES
    
    /* 160 */ "RBCURSOR"_Stz,            // CN_RBCURSOR
    /* 161 */ "REGFRAME"_Stz,            // CN_REGFRAME
    /* 162 */ "RING"_Stz,                // CN_RING
    /* 163 */ "ROARRAY"_Stz,             // CN_ROARRAY
    /* 164 */ "ROWBUFFER"_Stz,           // CN_ROWBUFFER
    /* 165 */ "ROWCURSOR"_Stz,           // CN_ROWCURSOR
    /* 166 */ "ROWONCE"_Stz,             // CN_ROWONCE
    /* 167 */ "ROWWRK"_Stz,              // CN_ROWWRK
    
    /* 168 */ "SCANBUFFER"_Stz,          // CN_SCANBUFFER
    /* 169 */ "SCRATCH"_Stz,             // CN_SCRATCH
    /* 170 */ "SEITF"_Stz,               // CN_SEITF
    /* 171 */ "SELCURSOR"_Stz,           // CN_SELCURSOR
    /* 172 */ "SEMAPHORE"_Stz,           // CN_SEMAPHORE
    /* 173 */ "SEPROMPT"_Stz,            // CN_SEPROMPT
    /* 174 */ "SINCE"_Stz,               // CN_SINCE
    /* 175 */ "SKIPCURSOR"_Stz,          // CN_SKIPCURSOR
    /* 176 */ "SKIPLIST"_Stz,            // CN_SKIPLIST
    /* 177 */ "SKIPNODE"_Stz,            // CN_SKIPNODE
    /* 178 */ "SLAB"_Stz,                // CN_SLAB
    /* 179 */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
    /* 180 */ "SLOTS"_Stz,               // CN_SLOTS
    /* 181 */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
    /* 182 */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
    /* 183 */ "SOCKET"_Stz,              // CN_SOCKET
    /* 184 */ "STACK"_Stz,               // CN_STACK
    /* 185 */ "STEPACCUM"_Stz,           // CN_STEPACCUM
    /* 186 */ "STEPARRAY"_Stz,           // CN_STEPARRAY
    /* 187 */ "SYSCALL"_Stz,             // CN_SYSCALL
    
    /* 188 */ "TABLE"_Stz,               // CN_TABLE
    /* 189 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
    /* 190 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
    /* 191 */ "TASK"_Stz,                // CN_TASK
    /* 192 */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
    /* 193 */ "THREAD"_Stz,              // CN_THREAD
    /* 194 */ "THREADVECTOR"_Stz,        // CN_THREADVECTOR
    /* 195 */ "TIME"_Stz,                // CN_TIME
    /* 196 */ "TIMER"_Stz,               // CN_TIMER
    /* 197 */ "TOKEN"_Stz,               // CN_TOKEN
    /* 198 */ "TOKENISER"_Stz,           // CN_TOKENISER
    /* 199 */ "TOKENARY"_Stz,            // CN_TOKENARY
    /* 200 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
    /* 201 */ "TX"_Stz,                  // CN_TX
    /* 202 */ "TXSET"_Stz,               // CN_TXSET
    
    /* 203 */ "WILDERNESS"_Stz,          // CN_WILDERNESS
    /* 204 */ "WORD"_Stz,                // CN_WORD
    /* 205 */ "WORKINGBUFFER"_Stz,       // CN_WORKINGBUFFER
    /* 206 */ "WORKINGROW"_Stz,          // CN_WORKINGROW
    
    /* 207 */ "ZSMALL_"_Stz,             // CN_ZSMALL_
    /* 208 */ "ZMEDIUM_"_Stz,            // CN_ZMEDIUM_
    /* 209 */ "ZLARGE_"_Stz,             // CN_ZLARGE_
    /* 210 */ "ZJUMBO_"_Stz,             // CN_ZJUMBO_
    
    /* 211 */ "ENUMS"_Stz,               // CN_ENUMS
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
