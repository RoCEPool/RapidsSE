/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "ClassData.h"

namespace ClassData
{

/*----------------------------------------------------------------------------*/
/* class numbers                                                              */
/*----------------------------------------------------------------------------*/
enum eCN : u2 // Class Number
{
  /* */ CN_NONE,
  
  /* */ CN__A__,
  /* */ CN_ACCESSPATH,
  /* */ CN_AGGTIMER,
  /* */ CN_ARG,
  /* */ CN_ARGUMENTS,
  /* */ CN_ARRANGE,
  /* */ CN_ARTICLE,
  /* */ CN_ATOMIC,
  /* */ CN_ATTACHMAP,
  
  /* */ CN__B__,
  /* */ CN_BCDVAL,
  /* */ CN_BIRTH,
  /* */ CN_BOLVAL,
  /* */ CN_BPA,
  /* */ CN_BUCKETMAP,
  /* */ CN_BUFFERCURSOR,
  /* */ CN_BUFFERPOOL,
  /* */ CN_BYTERATE,
  /* */ CN_BYTES_T,
  
  /* */ CN__C__,
  /* */ CN_CARRYOVER,
  /* */ CN_CATALOG,
  /* */ CN_CHARSER,
  /* */ CN_CLAUSE_FROM,
  /* */ CN_CLAUSE_GROUPBY,
  /* */ CN_CLAUSE_JOIN,
  /* */ CN_CLAUSE_LIMIT,
  /* */ CN_CLAUSE_ORDERBY,
  /* */ CN_CLAUSE_PROJECT,
  /* */ CN_CLAUSE_WHERE,
  /* */ CN_COLUMN,
  /* */ CN_COMPILER,
  /* */ CN_CONSTDEF,
  /* */ CN_CONSTSET,
  /* */ CN_CRVBUFFER,
  /* */ CN_CRWBUFFER,
  /* */ CN_CRWHASH,
  /* */ CN_CSVBUFFER,
  
  /* */ CN__D__,
  /* */ CN_DATUM,
  /* */ CN_DECACNT,
  /* */ CN_DECIDER,
  /* */ CN_DESCEND,
  /* */ CN_DEVIXT,
  /* */ CN_DEVRSB,
  /* */ CN_DIC,
  /* */ CN_DICSTACK,
  /* */ CN_DIR,
  /* */ CN_DTEVAL,
  /* */ CN_DYNAMO,

  /* */ CN__E__,
  /* */ CN_EDTVAL,
  /* */ CN_ENCODING,
  /* */ CN_ENT,
  /* */ CN_ENTBUFFER,
  /* */ CN_ENTBUFSORTER,
  /* */ CN_ENTWRK,
  /* */ CN_ENUMFLAGS,
  /* */ CN_EXEMPLAR,

  /* */ CN__F__,
  /* */ CN_FETCHBUFFER,
  /* */ CN_FIFO,
  /* */ CN_FILE,
  /* */ CN_FILTER_FETCH,
  /* */ CN_FILTER_FROM,
  /* */ CN_FILTER_GROUPBY,
  /* */ CN_FILTER_INFLATE,
  /* */ CN_FILTER_JOIN,
  /* */ CN_FILTER_LIMIT,
  /* */ CN_FILTER_ORDERBY,
  /* */ CN_FILTER_OUTPUT,
  /* */ CN_FILTER_PROJECT,
  /* */ CN_FILTER_SCAN,
  /* */ CN_FILTER_SELECT,
  /* */ CN_FILTER_SUBQRY,
  /* */ CN_FILTER_WHERE,
  /* */ CN_FILTER_ZED,
  /* */ CN_FIPVAL,
  /* */ CN_FLTVAL,
  /* */ CN_FOCALOBJECTS,
  /* */ CN_FORTH,
  /* */ CN_FORMAT,
  /* */ CN_FREEBUFFER,
  /* */ CN_FREECHUNK,
  /* */ CN_FREEDNOT,
  /* */ CN_FREEITEM,
  /* */ CN_FREQDIST,
  
  /* */ CN__G__,
  /* */ CN_GGADAPTOR,
  /* */ CN_GRID,
  
  /* */ CN__H__,
  /* */ CN_HASHBUFFER,
  /* */ CN_HASHMAP,
  /* */ CN_HASHSET,
  /* */ CN_HEAPBUFFER,
  /* */ CN_HPROW,
  
  /* */ CN__I__,
  /* */ CN_INDEX,
  /* */ CN_INDEXDEF,
  /* */ CN_INDEXSTREAM,
  /* */ CN_INDEXTREE,
  /* */ CN_INDEXTREEVEC,
  /* */ CN_IDXVEC,
  /* */ CN_INFOSET,
  /* */ CN_INGESTCHUNK,
  /* */ CN_INPUT,
  /* */ CN_INTBUFFER,
  /* */ CN_INTSET,
  /* */ CN_INTVAL,
  /* */ CN_IOCHUNK,
  /* */ CN_IOCOPY,
  /* */ CN_IRWBUFFER,
  /* */ CN_IVLVAL,

  /* */ CN__J__,
  /* */ CN_JOB,
  /* */ CN_JOB__FIND,
  /* */ CN_JOBLIST,

  /* */ CN__K__,
  /* */ CN_KEY,
  
  /* */ CN__L__,
  /* */ CN_LARGEPERSISTENT,
  /* */ CN_LARGETRANSIENT,
  /* */ CN_LEXICON,
  /* */ CN_LICENCE,
  /* */ CN_LIMITS,
  /* */ CN_LOADCHUNK,
  /* */ CN_LOADCURSOR,
  /* */ CN_LOADER,
  /* */ CN_LOCAL,
  /* */ CN_LOCALSCHAIN,
  /* */ CN_LOCK,
  /* */ CN_LOG,
  /* */ CN_LOGENTRY,
  /* */ CN_LOGICCOMPILER,
  /* */ CN_LOGICTIMER,
  /* */ CN_LOGICUNIT,
  /* */ CN_LRW,
  /* */ CN_LRWBUFFER,

  /* */ CN__M__,
  /* */ CN_MARKER,
  /* */ CN_MEDIUMPERSISTENT,
  /* */ CN_MEDIUMTRANSIENT,
  /* */ CN_MEMTEST,
  /* */ CN_METABLOCK,
  /* */ CN_METACELL,
  /* */ CN_METAGRAPH,
  /* */ CN_METAINHERIT,
  /* */ CN_METAINHERITS,
  /* */ CN_METAMEMBER,
  /* */ CN_METATYPE,
  /* */ CN_MIDEND_FROMVEC,
  /* */ CN_MUTEX,
  
  /* */ CN__N__,
  /* */ CN_NEXTGID,
  /* */ CN_NMLSTR,
  /* */ CN_NODE,
  /* */ CN_NOW,
  /* */ CN_NUMBAT,
  
  /* */ CN__O__,
  /* */ CN_OBJECTSINPOOL,
  /* */ CN_OP,
  /* */ CN_OPERANDQUEUE,
  /* */ CN_OPSPEC,
  
  /* */ CN__P__,
  /* */ CN_PATHVEC,
  /* */ CN_PERUSE,
  /* */ CN_PIPELINE,
  /* */ CN_PIPELINESPEC,
  /* */ CN_PLAN,
  /* */ CN_PLANNER,
  /* */ CN_POT,
  /* */ CN_PREDICATE,
  /* */ CN_PREDICATEVEC,
  /* */ CN_PROFILE,
  /* */ CN_PSXTHD,
  /* */ CN_PTNCOLVEC,
  /* */ CN_PTNMAP,
  /* */ CN_PTNSTREAM,
  /* */ CN_PTRVAL,
  
  /* */ CN__Q__,
  /* */ CN_QUEUE,
  /* */ CN_QUEUEBUFFER,
  /* */ CN_QUEUES,
  
  /* */ CN__R__,
  /* */ CN_READAPI,
  /* */ CN_RECORD,
  /* */ CN_RECORDBUFFER,
  /* */ CN_RELOP,
  /* */ CN_RESOURCETRACKER,
  /* */ CN_RESPONSE,
  /* */ CN_RESPONSEVEC,
  /* */ CN_RESULT,
  /* */ CN_RLUDIARY,
  /* */ CN_RN_RAND,
  /* */ CN_ROUTINE,
  /* */ CN_ROW,
  /* */ CN_ROWAGG,
  /* */ CN_ROWCOMPACTOR,
  /* */ CN_ROWONCE,
  /* */ CN_ROWSINGLE,
  /* */ CN_ROWSLAB,
  /* */ CN_ROWTRAY,
  /* */ CN_RSBBUFFER,
  /* */ CN_RUCPU,
  /* */ CN_RUDIR,
  /* */ CN_RUFILE,
  /* */ CN_RUMEMORY,
  /* */ CN_RUMUTEX,
  /* */ CN_RUPASS,
  /* */ CN_RUSOCKET,
  /* */ CN_RUTIME,
  /* */ CN_RUWORK,

  /* */ CN__S__,
  /* */ CN_SCHEMA,
  /* */ CN_SCHEMATEST,
  /* */ CN_SEEME,
  /* */ CN_SEPROMPT,
  /* */ CN_SESSION,
  /* */ CN_SESSIONARRAY,
  /* */ CN_SHORTCUT,
  /* */ CN_SHORTCUTVEC,
  /* */ CN_SINCE,
  /* */ CN_SKIPLIST,
  /* */ CN_SKIPNODE,
  /* */ CN_SLAB,
  /* */ CN_SLABALLOCATOR,
  /* */ CN_SLEEP,
  /* */ CN_SMALLPERSISTENT,
  /* */ CN_SMALLTRANSIENT,
  /* */ CN_SNBCD,
  /* */ CN_SNBOL,
  /* */ CN_SNDTE,
  /* */ CN_SNFLT,
  /* */ CN_SNINT,
  /* */ CN_SNIVL,
  /* */ CN_SNTME,
  /* */ CN_SNTSP,
  /* */ CN_SNVCH,
  /* */ CN_SOCKET,
  /* */ CN_SPELAPSED,
  /* */ CN_SPID,
  /* */ CN_SPSTRING,
  
  /* */ CN_SPMEDIUM,
  /* */ CN_SPSMALL,

  /* */ CN_SRTBUFFER,
  /* */ CN_SRTSET,
  /* */ CN_STACK,
  /* */ CN_STAGE,
  /* */ CN_STAGESPEC,
  /* */ CN_STATISTICS,
  /* */ CN_STEPACCUM,
  /* */ CN_STEPTIMING,
  /* */ CN_STMT_SELECT,
  /* */ CN_STMT_SUBQRY,
  /* */ CN_STRAND,
  /* */ CN_STREAMDEF,
  /* */ CN_STREAMSET,
  /* */ CN_STRING_T,
  /* */ CN_STZVAL,
  /* */ CN_SYSCALL,
  
  /* */ CN__T__,
  /* */ CN_TABLE,
  /* */ CN_TABLEDEF,
  /* */ CN_TABLEPTN,
  /* */ CN_TABLEPTNVEC,
  /* */ CN_TAGSET,
  /* */ CN_TASK,
  /* */ CN_TASKLIST,
  /* */ CN_TASKQUEUE,
  /* */ CN_TEMPNAME,
  /* */ CN_TENANT,
  /* */ CN_TENANTS,
  /* */ CN_THING,
  /* */ CN_THREAD,
  /* */ CN_THREADQUEUE,
  /* */ CN_THREADVEC,
  /* */ CN_TIMER,
  /* */ CN_TINYPERSISTENT,
  /* */ CN_TINYTRANSIENT,
  /* */ CN_TMEVAL,
  /* */ CN_TOHSHCRW,
  /* */ CN_TOKEN,
  /* */ CN_TOKENVEC,
  /* */ CN_TONSTRSB,
  /* */ CN_TOPTNRSB,
  /* */ CN_TORSB,
  /* */ CN_TSPVAL,
  /* */ CN_TX,
  /* */ CN_TXSET,
  
  /* */ CN__U__,
  
  /* */ CN__V__,
  /* */ CN_VORTEX,

  /* */ CN__W__,
  /* */ CN_WILDERNESS,
  /* */ CN_WORD,

  /* */ CN__X__,
  /* */
  /* */ CN_XMITOPERAND,
  /* */ CN_XMITS,
  
  /* */ CN__Y__,
  
  /* */ CN__Z__,
  
  /* */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /* */ "NONE"_Stz,                // CN_NONE
  
  /* */ "_A__"_Stz,                // CN__A__
  /* */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
  /* */ "AGGTIMER"_Stz,            // CN_AGGTIMER
  /* */ "ARG"_Stz,                 // CN_ARG
  /* */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /* */ "ARRANGE"_Stz,             // CN_ARRANGE
  /* */ "ARTICLE"_Stz,             // CN_ARTICLE
  /* */ "ATOMIC"_Stz,              // CN_ATOMIC
  /* */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
  
  /* */ "_B__"_Stz,                // CN__B__
  /* */ "BCDVAL"_Stz,              // CN_BCDVAL
  /* */ "BIRTH"_Stz,               // CN_BIRTH
  /* */ "BOLVAL"_Stz,              // CN_BOLVAL
  /* */ "BPA"_Stz,                 // CN_BPA
  /* */ "BUCKETMAP"_Stz,           // CN_BUCKETMAP
  /* */ "BUFFERCURSOR"_Stz,        // CN_BUFFERCURSOR
  /* */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /* */ "BYTERATE"_Stz,            // CN_BYTERATE
  /* */ "BYTES_T"_Stz,             // CN_BYTES_T
  
  /* */ "_C__"_Stz,                // CN__C__
  /* */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /* */ "CATALOG"_Stz,             // CN_CATALOG
  /* */ "CHARSER"_Stz,             // CN_CHARSER
  /* */ "CLAUSE_FROM"_Stz,         // CN_CLAUSE_FROM
  /* */ "CLAUSE_GROUPBY"_Stz,      // CN_CLAUSE_GROUPBY
  /* */ "CLAUSE_JOIN"_Stz,         // CN_CLAUSE_JOIN
  /* */ "CLAUSE_LIMIT"_Stz,        // CN_CLAUSE_LIMIT
  /* */ "CLAUSE_ORDERBY"_Stz,      // CN_CLAUSE_ORDERBY
  /* */ "CLAUSE_PROJECT"_Stz,      // CN_CLAUSE_PROJECT
  /* */ "CLAUSE_WHERE"_Stz,        // CN_CLAUSE_WHERE
  /* */ "COLUMN"_Stz,              // CN_COLUMN
  /* */ "COMPILER"_Stz,            // CN_COMPILER
  /* */ "CONSTDEF"_Stz,            // CN_CONSTDEF
  /* */ "CONSTSET"_Stz,            // CN_CONSTSET
  /* */ "CRVBUFFER"_Stz,           // CN_CRVBUFFER
  /* */ "CRWBUFFER"_Stz,           // CN_CRWBUFFER
  /* */ "CRWHASH"_Stz,             // CN_CRWHASH
  /* */ "CSVBUFFER"_Stz,           // CN_CSVBUFFER
  
  /* */ "_D__"_Stz,                // CN__D__
  /* */ "DATUM"_Stz,               // CN_DATUM
  /* */ "DECACNT"_Stz,             // CN_DECACNT
  /* */ "DECIDER"_Stz,             // CN_DECIDER
  /* */ "DESCEND"_Stz,             // CN_DESCEND
  /* */ "DEVIXT"_Stz,              // CN_DEVIXT
  /* */ "DEVRSB"_Stz,              // CN_DEVRSB
  /* */ "DIC"_Stz,                 // CN_DIC
  /* */ "DICSTACK"_Stz,            // CN_DICSTACK
  /* */ "DIR"_Stz,                 // CN_DIR
  /* */ "DTEVAL"_Stz,              // CN_DTEVAL
  /* */ "DYNAMO"_Stz,              // CN_DYNAMO

  /* */ "_E__"_Stz,                // CN__E__
  /* */ "EDTVAL"_Stz,              // CN_EDTVAL
  /* */ "ENCODING"_Stz,            // CN_ENCODING
  /* */ "ENT"_Stz,                 // CN_ENT
  /* */ "ENTBUFFER"_Stz,           // CN_ENTBUFFER
  /* */ "ENTBUFSORTER"_Stz,        // CN_ENTBUFSORTER
  /* */ "ENTWRK"_Stz,              // CN_ENTWRK
  /* */ "ENUMFLAGS"_Stz,           // CN_ENUMFLAGS
  /* */ "EXEMPLAR"_Stz,            // CN_EXEMPLAR

  /* */ "_F__"_Stz,                // CN__F__
  /* */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /* */ "FIFO"_Stz,                // CN_FIFO
  /* */ "FILE"_Stz,                // CN_FILE
  /* */ "FILTER_FETCH"_Stz,        // CN_FILTER_FETCH
  /* */ "FILTER_FROM"_Stz,         // CN_FILTER_FROM
  /* */ "FILTER_GROUPBY"_Stz,      // CN_FILTER_GROUPBY
  /* */ "FILTER_INFLATE"_Stz,      // CN_FILTER_INFLATE
  /* */ "FILTER_JOIN"_Stz,         // CN_FILTER_JOIN
  /* */ "FILTER_LIMIT"_Stz,        // CN_FILTER_LIMIT
  /* */ "FILTER_ORDERBY"_Stz,      // CN_FILTER_ORDERBY
  /* */ "FILTER_OUTPUT"_Stz,       // CN_FILTER_OUTPUT
  /* */ "FILTER_PROJECT"_Stz,      // CN_FILTER_PROJECT
  /* */ "FILTER_SCAN"_Stz,         // CN_FILTER_SCAN
  /* */ "FILTER_SELECT"_Stz,       // CN_FILTER_SELECT
  /* */ "FILTER_SUBQRY"_Stz,       // CN_FILTER_SUBQRY
  /* */ "FILTER_WHERE"_Stz,        // CN_FILTER_WHERE
  /* */ "FILTER_ZED"_Stz,          // CN_FILTER_ZED
  /* */ "FIPVAL"_Stz,              // CN_FIPVAL
  /* */ "FLTVAL"_Stz,              // CN_FLTVAL
  /* */ "FOCALOBJECTS"_Stz,        // CN_FOCALOBJECTS
  /* */ "FORTH"_Stz,               // CN_FORTH
  /* */ "FORMAT"_Stz,              // CN_FORMAT
  /* */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /* */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /* */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /* */ "FREEITEM"_Stz,            // CN_FREEITEM
  /* */ "FREQDIST"_Stz,            // CN_FREQDIST
  
  /* */ "_G__"_Stz,                // CN__G__
  /* */ "GGADAPTOR"_Stz,           // CN_GGADAPTOR
  /* */ "GRID"_Stz,                // CN_GRID
  
  /* */ "_H__"_Stz,                // CN__H__
  /* */ "HASHBUFFER"_Stz,          // CN_HASHBUFFER
  /* */ "HASHSET"_Stz,             // CN_HASHSET
  /* */ "HASHMAP"_Stz,             // CN_HASHMAP
  /* */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  /* */ "HPROW"_Stz,               // CN_HPROW
  
  /* */ "_I__"_Stz,                // CN__I__
  /* */ "INDEX"_Stz,               // CN_INDEX
  /* */ "INDEXDEF"_Stz,            // CN_INDEXDEF
  /* */ "INDEXSTREAM"_Stz,         // CN_INDEXSTREAM
  /* */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /* */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /* */ "IDXVEC"_Stz,              // CN_IDXVEC
  /* */ "INFOSET"_Stz,             // CN_INFOSET
  /* */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /* */ "INPUT"_Stz,               // CN_INPUT
  /* */ "INTBUFFER"_Stz,           // CN_INTBUFFER
  /* */ "INTUTILITY"_Stz,          // CN_INTSET
  /* */ "INTVAL"_Stz,              // CN_INTVAL
  /* */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /* */ "IOCOPY"_Stz,              // CN_IOCOPY
  /* */ "IRWBUFFER"_Stz,           // CN_IRWBUFFER
  /* */ "IVLVAL"_Stz,              // CN_IVLVAL

  /* */ "_J__"_Stz,                // CN__J__
  /* */ "JOB"_Stz,                 // CN_JOB
  /* */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /* */ "JOBLIST"_Stz,             // CN_JOBLIST

  /* */ "_K__"_Stz,                // CN__K__
  /* */ "KEY"_Stz,                 // CN_KEY
  
  /* */ "_L__"_Stz,                // CN__L__
  /* */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* */ "LEXICON"_Stz,             // CN_LEXICON
  /* */ "LICENCE"_Stz,             // CN_LICENCE
  /* */ "LIMITS"_Stz,              // CN_LIMITS
  /* */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* */ "LOADCURSOR"_Stz,          // CN_LOADCURSOR
  /* */ "LOADER"_Stz,              // CN_LOADER
  /* */ "LOCAL"_Stz,               // CN_LOCAL
  /* */ "LOCALSCHAIN"_Stz,         // CN_LOCALSCHAIN
  /* */ "LOCK"_Stz,                // CN_LOCK
  /* */ "LOG"_Stz,                 // CN_LOG
  /* */ "LOGENTRY"_Stz,            // CN_LOGENTRY
  /* */ "LOGICCOMPILER"_Stz,       // CN_LOGICCOMPILER
  /* */ "LOGICTIMER"_Stz,          // CN_LOGICTIMER
  /* */ "LOGICUNIT"_Stz,           // CN_LOGICUNIT
  /* */ "LRW"_Stz,                 // CN_LRW
  /* */ "LRWBUFFER"_Stz,           // CN_LRWBUFFER

  /* */ "_M__"_Stz,                // CN__M__
  /* */ "MARKER"_Stz,              // CN_MARKER
  /* */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  /* */ "MEMTEST"_Stz,             // CN_MEMTEST
  /* */ "METABLOCK"_Stz,           // CN_METABLOCK
  /* */ "METACELL"_Stz,            // CN_METACELL
  /* */ "METAGRAPH"_Stz,           // CN_METAGRAPH
  /* */ "METAINHERIT"_Stz,         // CN_METAINHERIT
  /* */ "METAINHERITS"_Stz,        // CN_METAINHERITS
  /* */ "METAMEMBER"_Stz,          // CN_METAMEMBER
  /* */ "METATYPE"_Stz,            // CN_METATYPE
  /* */ "MIDEND_FROMVEC"_Stz,      // CN_MIDEND_FROMVEC
  /* */ "MUTEX"_Stz,               // CN_MUTEX
  
  /* */ "_N__"_Stz,                // CN__N__
  /* */ "NEXTGID"_Stz,             // CN_NEXTGID
  /* */ "NMLSTR"_Stz,              // CN_NMLSTR
  /* */ "NODE"_Stz,                // CN_NODE
  /* */ "NOW"_Stz,                 // CN_NOW
  /* */ "NUMBAT"_Stz,              // CN_NUMBAT
  
  /* */ "_O__"_Stz,                // CN__O__
  /* */ "OBJECTSINPOOL"_Stz,       // CN_OBJECTSINPOOL
  /* */ "OP"_Stz,                  // CN_OP
  /* */ "OPERANDQUEUE"_Stz,        // CN_OPERANDQUEUE
  /* */ "OPSPEC"_Stz,              // CN_OPSPEC
  
  /* */ "_P__"_Stz,                // CN__P__
  /* */ "PATHVEC"_Stz,             // CN_PATHVEC
  /* */ "PERUSE"_Stz,              // CN_PERUSE
  /* */ "PIPELINE"_Stz,            // CN_PIPELINE
  /* */ "PIPELINESPEC"_Stz,        // CN_PIPELINESPEC
  /* */ "PLAN"_Stz,                // CN_PLAN
  /* */ "PLANNER"_Stz,             // CN_PLANNER
  /* */ "POT"_Stz,                 // CN_POT
  /* */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* */ "PROFILE"_Stz,             // CN_PROFILE
  /* */ "PSXTHD"_Stz,              // CN_PSXTHD
  /* */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  /* */ "PTNMAP"_Stz,              // CN_PTNMAP
  /* */ "PTNSTREAM"_Stz,           // CN_PTNSTREAM
  /* */ "PTRVAL"_Stz,              // CN_PTRVAL
  
  /* */ "_Q__"_Stz,                // CN__Q__
  /* */ "QUEUE"_Stz,               // CN_QUEUE
  /* */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* */ "QUEUES"_Stz,              // CN_QUEUES
  
  /* */ "_R__"_Stz,                // CN__R__
  /* */ "READAPI"_Stz,             // CN_READAPI
  /* */ "RECORD"_Stz,              // CN_RECORD
  /* */ "RECORDBUFFER"_Stz,        // CN_RECORDBUFFER
  /* */ "RELOP"_Stz,               // CN_RELOP
  /* */ "RESOURCETRACKER"_Stz,     // CN_RESOURCETRACKER
  /* */ "RESPONSE"_Stz,            // CN_RESPONSE
  /* */ "RESPONSEVEC"_Stz,         // CN_RESPONSEVEC
  /* */ "RESULT"_Stz,              // CN_RESULT
  /* */ "RLUDIARY"_Stz,            // CN_RLUDIARY
  /* */ "RN_RAND"_Stz,             // CN_RN_RAND
  /* */ "ROUTINE"_Stz,             // CN_ROUTINE
  /* */ "ROW"_Stz,                 // CN_ROW
  /* */ "ROWAGG"_Stz,              // CN_ROWAGG
  /* */ "ROWCOMPACTOR"_Stz,        // CN_ROWCOMPACTOR
  /* */ "ROWONCE"_Stz,             // CN_ROWONCE
  /* */ "ROWSINGLE"_Stz,           // CN_ROWSINGLE
  /* */ "ROWSLAB"_Stz,             // CN_ROWSLAB
  /* */ "ROWTRAY"_Stz,             // CN_ROWTRAY
  /* */ "RSBBUFFER"_Stz,           // CN_RSBBUFFER
  /* */ "RUCPU"_Stz,               // CN_RUCPU
  /* */ "RUDIR"_Stz,               // CN_RUDIR
  /* */ "RUFILE"_Stz,              // CN_RUFILE
  /* */ "RUMEMORY"_Stz,            // CN_RUMEMORY
  /* */ "RUMUTEX"_Stz,             // CN_RUMUTEX
  /* */ "RUPASS"_Stz,              // CN_RUPASS
  /* */ "RUSOCKET"_Stz,            // CN_RUSOCKET
  /* */ "RUTIME"_Stz,              // CN_RUTIME
  /* */ "RUWORK"_Stz,              // CN_RUWORK

  /* */ "_S__"_Stz,                // CN__S__
  /* */ "SCHEMA"_Stz,              // CN_SCHEMA
  /* */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /* */ "SEEME"_Stz,               // CN_SEEME
  /* */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* */ "SESSION"_Stz,             // CN_SESSION
  /* */ "SESSIONARRAY"_Stz,        // CN_SESSIONARRAY
  /* */ "SHORTCUT"_Stz,            // CN_SHORTCUT
  /* */ "SHORTCUTVEC"_Stz,         // CN_SHORTCUTVEC
  /* */ "SINCE"_Stz,               // CN_SINCE
  /* */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* */ "SLAB"_Stz,                // CN_SLAB
  /* */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* */ "SLEEP"_Stz,               // CN_SLEEP
  /* */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* */ "SNBCD"_Stz,               // CN_SNBCD
  /* */ "SNBOL"_Stz,               // CN_SNBOL
  /* */ "SNDTE"_Stz,               // CN_SNDTE
  /* */ "SNFLT"_Stz,               // CN_SNFLT
  /* */ "SNINT"_Stz,               // CN_SNINT
  /* */ "SNIVL"_Stz,               // CN_SNIVL
  /* */ "SNTME"_Stz,               // CN_SNTME
  /* */ "SNTSP"_Stz,               // CN_SNTSP
  /* */ "SNVCH"_Stz,               // CN_SNVCH
  /* */ "SOCKET"_Stz,              // CN_SOCKET
  /* */ "SPELAPSED"_Stz,           // CN_SPELAPSED
  /* */ "SPID"_Stz,                // CN_SPID
  /* */ "SPSTRING"_Stz,            // CN_SPSTRING
  
  /* */ "SPMEDIUM"_Stz,            // CN_SPMEDIUM
  /* */ "SPSMALL"_Stz,             // CN_SPSMALL

  /* */ "SRTBUFFER"_Stz,           // CN_SRTBUFFER
  /* */ "SRTSET"_Stz,              // CN_SRTSET
  /* */ "STACK"_Stz,               // CN_STACK
  /* */ "STAGE"_Stz,               // CN_STAGE
  /* */ "STAGESPEC"_Stz,           // CN_STAGESPEC
  /* */ "STATISTICS"_Stz,          // CN_STATISTICS
  /* */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /* */ "STMT_SELECT"_Stz,         // CN_STMT_SELECT
  /* */ "STMT_SUBQRY"_Stz,         // CN_STMT_SUBQRY
  /* */ "STRAND"_Stz,              // CN_STRAND
  /* */ "STREAMDEF"_Stz,           // CN_STREAMDEF
  /* */ "STREAMSET"_Stz,           // CN_STREAMSET
  /* */ "STRING_T"_Stz,            // CN_STRING_T
  /* */ "STZVAL"_Stz,              // CN_STZVAL
  /* */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* */ "_T__"_Stz,                // CN__T__
  /* */ "TABLE"_Stz,               // CN_TABLE
  /* */ "TABLEDEF"_Stz,            // CN_TABLEDEF
  /* */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* */ "TAGSET"_Stz,              // CN_TAGSET
  /* */ "TASK"_Stz,                // CN_TASK
  /* */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /* */ "TEMPNAME"_Stz,            // CN_TEMPNAME
  /* */ "TENANT"_Stz,              // CN_TENANT
  /* */ "TENANTS"_Stz,             // CN_TENANTS
  /* */ "THING"_Stz,               // CN_THING
  /* */ "THREAD"_Stz,              // CN_THREAD
  /* */ "THREADQUEUE"_Stz,         // CN_THREADQUEUE
  /* */ "THREADVEC"_Stz,           // CN_THREADVEC
  /* */ "TIMER"_Stz,               // CN_TIMER
  /* */ "TINYPERSISTENT"_Stz,      // CN_TINYPERSISTENT
  /* */ "TINYTRANSIENT"_Stz,       // CN_TINYTRANSIENT
  /* */ "TMEVAL"_Stz,              // CN_TMEVAL
  /* */ "TOHSHCRW"_Stz,            // CN_TOHSHCRW
  /* */ "TOKEN"_Stz,               // CN_TOKEN
  /* */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* */ "TONSTRSB"_Stz,            // CN_TONSTRSB
  /* */ "TOPTNRSB"_Stz,            // CN_TOPTNRSB
  /* */ "TORSB"_Stz,               // CN_TORSB
  /* */ "TSPVAL"_Stz,              // CN_TSPVAL
  /* */ "TX"_Stz,                  // CN_TX
  /* */ "TXSET"_Stz,               // CN_TXSET
  
  /* */ "_U__"_Stz,                // CN__U__
  
  /* */ "_V__"_Stz,                // CN__V__
  /* */ "VORTEX"_Stz,              // CN_VORTEX

  /* */ "_W__"_Stz,                // CN__W__
  /* */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* */ "WORD"_Stz,                // CN_WORD

  /* */ "_X__"_Stz,                // CN__X__
  /* */ "XmitOperand"_Stz,         // CN_XMITOPERAND
  /* */ "Xmits"_Stz,               // CN_XMITS
  
  /* */ "_Y__"_Stz,                // CN__Y__
  
  /* */ "_Z__"_Stz,                // CN__Z__
  
  /* */ "ENUMS"_Stz,               // CN_ENUMS
};
  
  class ClassNumbers
  {
  public:
    static eCN eFind(Stz* szszClassNm);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
