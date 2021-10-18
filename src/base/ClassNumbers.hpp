/*============================================================================*/
/* ClassNumbers.hpp - class numbers                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace BASE
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
  /* */ CN_AM,
  /* */ CN_API,
  /* */ CN_ARG,
  /* */ CN_ARRANGE,
  /* */ CN_ATOMIC,
  /* */ CN_ATTACHMAP,
  
  /* */ CN__B__,
  /* */ CN_BACKEND,
  /* */ CN_BCDVAL,
  /* */ CN_BIRTH,
  /* */ CN_BOLVAL,
  /* */ CN_BUCKET,
  /* */ CN_BUCKETGET,
  /* */ CN_BUCKETMAP,
  /* */ CN_BUCKETMAP_dyn,
  /* */ CN_BUCKETVEC,
  /* */ CN_BUFFERMAP,
  /* */ CN_BUFFERPOOL,
  /* */ CN_BYTERATE,
  /* */ CN_BYTES_T,
  
  /* */ CN__C__,
  /* */ CN_CATALOG,
  /* */ CN_CELLSINGLE,
  /* */ CN_CHARSER,
  /* */ CN_CLASSPLACEHOLDER,
  /* */ CN_CMDLINEARGS,
  /* */ CN_CODEBLOCK,
  /* */ CN_CODESYNTH,
  /* */ CN_COLUMN,
  /* */ CN_COMBINED,
  /* */ CN_CRVBUFFER,
  /* */ CN_CRWBUFFER,
  /* */ CN_CRWTABLEPTN,
  /* */ CN_CRWTOROW,
  /* */ CN_CSVBUFFER,
  /* */ CN_CSVMETA,

  /* */ CN__D__,
  /* */ CN_DDLLOCK,
  /* */ CN_DECACNT,
  /* */ CN_DECIDER,
  /* */ CN_DEFINITIONNODE,
  /* */ CN_DEVIXT,
  /* */ CN_DEVROW,
  /* */ CN_DIC,
  /* */ CN_DICLOCK,
  /* */ CN_DICSTACK,
  /* */ CN_DIR,
  /* */ CN_DTEVAL,
  /* */ CN_DYNAMO,

  /* */ CN__E__,
  /* */ CN_EACHROWBUFFER,
  /* */ CN_EDTVAL,
  /* */ CN_ENCODING,
  /* */ CN_ENT,
  /* */ CN_ENTBUFFER,
  /* */ CN_ENTBUFSORTER,
  /* */ CN_ENTWRK,
  /* */ CN_ENUMFLAGS,
  /* */ CN_EXEMPLAR,
  /* */ CN_EXPREVAL,

  /* */ CN__F__,
  /* */ CN_FIFO,
  /* */ CN_FILE,
  /* */ CN_FIPVAL,
  /* */ CN_FLATTEN,
  /* */ CN_FLTVAL,
  /* */ CN_FORMAT,
  /* */ CN_FREEBUFFER,
  /* */ CN_FREECHUNK,
  /* */ CN_FREEDNOT,
  /* */ CN_FREEITEM,
  /* */ CN_FREQDIST,
  
  /* */ CN__G__,
  /* */ CN_GRID,
  
  /* */ CN__H__,
  /* */ CN_HASHBUFFER,
  /* */ CN_HASHMAP,
  /* */ CN_HASHSET,
  /* */ CN_HEAPBUFFER,
  /* */ CN_HEAPMEM,

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
  /* */ CN_INTERNALTABLES,
  /* */ CN_INTSET,
  /* */ CN_INTSTACK,
  /* */ CN_INTVAL,
  /* */ CN_IOCHUNK,
  /* */ CN_IOCOPY,
  /* */ CN_IVLVAL,

  /* */ CN__J__,
  /* */ CN_JOB,
  /* */ CN_JOBLIST,
  /* */ CN_JOBREPORT,
  /* */ CN_JOBS,

  /* */ CN__K__,
  /* */ CN_KEY,

  /* */ CN__L__,
  /* */ CN_LARGEPERSISTENT,
  /* */ CN_LARGETRANSIENT,
  /* */ CN_LARGEVIRTUAL,
  /* */ CN_LICENCE,
  /* */ CN_LIMITS,
  /* */ CN_LISTEXONODE_t,
  /* */ CN_LISTEXOITER_t,
  /* */ CN_LOADCHUNK,
  /* */ CN_LOCAL,
  /* */ CN_LOCALCHAIN,
  /* */ CN_LOCALDIR,
  /* */ CN_LOCALFILE,
  /* */ CN_LOCATION,
  /* */ CN_LOCATOR,
  /* */ CN_LOCK,
  /* */ CN_LOCKSET,
  /* */ CN_LOGICCOMPILER,
  /* */ CN_LOGICTIMER,
  /* */ CN_LOGICUNIT,

  /* */ CN__M__,
  /* */ CN_MARKER,
  /* */ CN_MEDIUMPERSISTENT,
  /* */ CN_MEDIUMTRANSIENT,
  /* */ CN_MEDIUMVIRTUAL,
  /* */ CN_MEMTEST,
  /* */ CN_METADATA,
  /* */ CN_METAGET,
  /* */ CN_MUTEX,

  /* */ CN__N__,
  /* */ CN_NAMEDBUFFER,
  /* */ CN_NESTEDPIPELINESPEC,
  /* */ CN_NEXTGID,
  /* */ CN_NMLSTR,
  /* */ CN_NODE,
  /* */ CN_NODE_I,
  /* */ CN_NOW,
  /* */ CN_NUMBAT,
  /* */ CN_NUMBERING,

  /* */ CN__O__,
  /* */ CN_OBJBUFFER,
  /* */ CN_OBJECTSNAPSHOT,
  /* */ CN_OUTPUT,
  /* */ CN_OP,
  /* */ CN_OPENFILES,
  /* */ CN_OPERANDQUEUE,
  /* */ CN_OPSPEC,
  /* */ CN_OPTIMUS,
  /* */ CN_OTHEROPERAND,

  /* */ CN__P__,
  /* */ CN_PARCEL,
  /* */ CN_PATHIDENTIFIER,
  /* */ CN_PATHVEC,
  /* */ CN_PERUSE,
  /* */ CN_PIPELINE,
  /* */ CN_PIPELINESPEC,
  /* */ CN_PLANDEFINITIONPARSER,
  /* */ CN_PLANNERLOCK,
  /* */ CN_PLANNODE,
  /* */ CN_PLANNODEBUILDER,
  /* */ CN_PLANNODEBUILDERLIST,
  /* */ CN_PLANNODEBUILDERSTACK,
  /* */ CN_PLANNODELIST,
  /* */ CN_PLANOPERAND,
  /* */ CN_POT,
  /* */ CN_PREDICATE,
  /* */ CN_PREDICATEVEC,
  /* */ CN_PRIMITIVENODE,
  /* */ CN_PROCEDURE,
  /* */ CN_PROFILE,
  /* */ CN_PSXTHD,
  /* */ CN_PTNMAP,
  /* */ CN_PTNSTREAM,
  /* */ CN_PTRVAL,
  
  /* */ CN__Q__,
  /* */ CN_QUERYNODE,
  /* */ CN_QUERYNODELIST,
  /* */ CN_QUERYNODESTACK,
  /* */ CN_QUEUE,
  /* */ CN_QUEUEBUFFER,

  /* */ CN__R__,
  /* */ CN_RANGE,
  /* */ CN_READER,
  /* */ CN_REAPLOCK,
  /* */ CN_RECORD,
  /* */ CN_RELOP,
  /* */ CN_RESOURCES,
  /* */ CN_RLUDIARY,
  /* */ CN_RN_RAND,
  /* */ CN_ROOTNODE,
  /* */ CN_ROW,
  /* */ CN_ROWAGG,
  /* */ CN_ROWBOX,
  /* */ CN_ROWBUFFER,
  /* */ CN_ROWCOMPACTOR,
  /* */ CN_ROWONCE,
  /* */ CN_ROWPIX,
  /* */ CN_ROWSINGLE,
  /* */ CN_ROWSINGLELIST,
  /* */ CN_ROWTABLEPTN,
  /* */ CN_ROWTRAY,
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
  /* */ CN_SCANNER,
  /* */ CN_SCHEDULER,
  /* */ CN_SCHEMA,
  /* */ CN_SEAPI,
  /* */ CN_SEDEPUTY,
  /* */ CN_SEEME,
  /* */ CN_SENCOMPILER,
  /* */ CN_SENDS,
  /* */ CN_SEPROMPT,
  /* */ CN_SESSION,
  /* */ CN_SESSIONS,
  /* */ CN_SHAPE,
  /* */ CN_SHARDDEF,
  /* */ CN_SHORTCUT,
  /* */ CN_SHORTCUTVEC,
  /* */ CN_SINCE,
  /* */ CN_SKIPLIST,
  /* */ CN_SKIPNODE,
  /* */ CN_SLAB,
  /* */ CN_SLABALLOCATOR,
  /* */ CN_SMALLPERSISTENT,
  /* */ CN_SMALLTRANSIENT,
  /* */ CN_SMALLVIRTUAL,
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
  /* */ CN_SQLPLANNER,
  /* */ CN_STACK,
  /* */ CN_STAGE,
  /* */ CN_STAGESPEC,
  /* */ CN_STATISTICS,
  /* */ CN_STATS,
  /* */ CN_STENCIL,
  /* */ CN_STEP,
  /* */ CN_STEPACCUM,
  /* */ CN_STEPS,
  /* */ CN_STREAM,
  /* */ CN_STREAMDEF,
  /* */ CN_STREAMDEFRHT,
  /* */ CN_STREAMSET,
  /* */ CN_STRING_T,
  /* */ CN_STZLIST,
  /* */ CN_STZVAL,
  /* */ CN_SYSCALL,
  /* */ CN_SCHEDULE,

  /* */ CN__T__,
  /* */ CN_TABLE,
  /* */ CN_TABLEDEF,
  /* */ CN_TABLEPTN,
  /* */ CN_TABLEPTNVEC,
  /* */ CN_TAGSET,
  /* */ CN_TASK,
  /* */ CN_TASKLIST,
  /* */ CN_TASKQUEUE,
  /* */ CN_TCCLOCK,
  /* */ CN_TEMPNAME,
  /* */ CN_TENANT,
  /* */ CN_TENANTS,
  /* */ CN_TEXT,
  /* */ CN_TEXTLARGE,
  /* */ CN_TEXTMEDIUM,
  /* */ CN_TEXTSMALL,
  /* */ CN_TEXTSTACK,
  /* */ CN_TEXTTINY,
  /* */ CN_THING,
  /* */ CN_THREAD,
  /* */ CN_THREADQUEUE,
  /* */ CN_THREADVEC,
  /* */ CN_TIMER,
  /* */ CN_TINYC,
  /* */ CN_TINYPERSISTENT,
  /* */ CN_TINYTRANSIENT,
  /* */ CN_TINYVIRTUAL,
  /* */ CN_TMEVAL,
  /* */ CN_TOBKTCRW,
  /* */ CN_TOBKTROW,
  /* */ CN_TOKEN,
  /* */ CN_TOKENVEC,
  /* */ CN_TONSTROW,
  /* */ CN_TOPTNCRW,
  /* */ CN_TOPTNROW,
  /* */ CN_TOROW,
  /* */ CN_TRACELOCK,
  /* */ CN_TRACEMULTILOCK,
  /* */ CN_TSPVAL,
  /* */ CN_TUB,
  /* */ CN_TX,
  /* */ CN_TXSET,
  
  /* */ CN__U__,
  /* */ CN_UNITINT,
  /* */ CN_UT_SKIPLIST,

  /* */ CN__V__,
  /* */ CN_VERBOSE,

  /* */ CN__W__,
  /* */ CN_WAKEUP,
  /* */ CN_WILDERNESS,
  /* */ CN_WORD,
  /* */ CN_WSPCONVERGENT_t,

  /* */ CN__X__,
  
  /* */ CN__Y__,
  
  /* */ CN__Z__,
  
  /* */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /* */ "NONE"_Stz,                 // CN_NONE
   
  /* */ "_A__"_Stz,                 // CN__A__
  /* */ "ACCESSPATH"_Stz,           // CN_ACCESSPATH
  /* */ "AGGTIMER"_Stz,             // CN_AGGTIMER
  /* */ "AM"_Stz,                   // CN_AM
  /* */ "API"_Stz,                  // CN_API
  /* */ "ARG"_Stz,                  // CN_ARG
  /* */ "ARRANGE"_Stz,              // CN_ARRANGE
  /* */ "ATOMIC"_Stz,               // CN_ATOMIC
  /* */ "ATTACHMAP"_Stz,            // CN_ATTACHMAP
   
  /* */ "_B__"_Stz,                 // CN__B__
  /* */ "BACKEND"_Stz,              // CN_BACKEND
  /* */ "BCDVAL"_Stz,               // CN_BCDVAL
  /* */ "BIRTH"_Stz,                // CN_BIRTH
  /* */ "BOLVAL"_Stz,               // CN_BOLVAL
  /* */ "BUCKET"_Stz,               // CN_BUCKET
  /* */ "BUCKETGET"_Stz,            // CN_BUCKETGET
  /* */ "BUCKETMAP"_Stz,            // CN_BUCKETMAP
  /* */ "BUCKETMAP_dyn"_Stz,        // CN_BUCKETMAP_dyn
  /* */ "BUCKETVEC"_Stz,            // CN_BUCKETVEC
  /* */ "BUFFERMAP"_Stz,            // CN_BUFFERMAP
  /* */ "BUFFERPOOL"_Stz,           // CN_BUFFERPOOL
  /* */ "BYTERATE"_Stz,             // CN_BYTERATE
  /* */ "BYTES_T"_Stz,              // CN_BYTES_T
   
  /* */ "_C__"_Stz,                 // CN__C__
  /* */ "CATALOG"_Stz,              // CN_CATALOG
  /* */ "CELLSINGLE"_Stz,           // CN_CELLSINGLE
  /* */ "CHARSER"_Stz,              // CN_CHARSER
  /* */ "CLASSPLACEHOLDER"_Stz,     // CN_CLASSPLACEHOLDER
  /* */ "CMDLINEARGS"_Stz,          // CN_CMDLINEARGS
  /* */ "CODEBLOCK"_Stz,            // CN_CODEBLOCK
  /* */ "CODESYNTH"_Stz,            // CN_CODESYNTH
  /* */ "COLUMN"_Stz,               // CN_COLUMN
  /* */ "COMBINED"_Stz,             // CN_COMBINED
  /* */ "CRVBUFFER"_Stz,            // CN_CRVBUFFER
  /* */ "CRWBUFFER"_Stz,            // CN_CRWBUFFER
  /* */ "CRWTABLEPTN"_Stz,          // CN_CRWTABLEPTN
  /* */ "CRWTOROW"_Stz,             // CN_CRWTOROW
  /* */ "CSVBUFFER"_Stz,            // CN_CSVBUFFER
  /* */ "CSVMETA"_Stz,              // CN_CSVMETA
 
  /* */ "_D__"_Stz,                 // CN__D__
  /* */ "DDLLOCK"_Stz,              // CN_DDLLOCK
  /* */ "DECACNT"_Stz,              // CN_DECACNT
  /* */ "DECIDER"_Stz,              // CN_DECIDER
  /* */ "DEFINITIONNODE"_Stz,       // CN_DEFINITIONNODE
  /* */ "DEVIXT"_Stz,               // CN_DEVIXT
  /* */ "DEVROW"_Stz,               // CN_DEVROW
  /* */ "DIC"_Stz,                  // CN_DIC
  /* */ "DICLOCK"_Stz,              // CN_DICLOCK
  /* */ "DICSTACK"_Stz,             // CN_DICSTACK
  /* */ "DIR"_Stz,                  // CN_DIR
  /* */ "DTEVAL"_Stz,               // CN_DTEVAL
  /* */ "DYNAMO"_Stz,               // CN_DYNAMO
 
  /* */ "_E__"_Stz,                 // CN__E__
  /* */ "EACHROWBUFFER"_Stz,         // CN_EACHROWBUFFER
  /* */ "EDTVAL"_Stz,               // CN_EDTVAL
  /* */ "ENCODING"_Stz,             // CN_ENCODING
  /* */ "ENT"_Stz,                  // CN_ENT
  /* */ "ENTBUFFER"_Stz,            // CN_ENTBUFFER
  /* */ "ENTBUFSORTER"_Stz,         // CN_ENTBUFSORTER
  /* */ "ENTWRK"_Stz,               // CN_ENTWRK
  /* */ "ENUMFLAGS"_Stz,            // CN_ENUMFLAGS
  /* */ "EXEMPLAR"_Stz,             // CN_EXEMPLAR
  /* */ "EXPREVAL"_Stz,             // CN_EXPREVAL
 
  /* */ "_F__"_Stz,                 // CN__F__
  /* */ "FIFO"_Stz,                 // CN_FIFO
  /* */ "FILE"_Stz,                 // CN_FILE
  /* */ "FIPVAL"_Stz,               // CN_FIPVAL
  /* */ "FLATTEN"_Stz,              // CN_FLATTEN
  /* */ "FLTVAL"_Stz,               // CN_FLTVAL
  /* */ "FORMAT"_Stz,               // CN_FORMAT
  /* */ "FREEBUFFER"_Stz,           // CN_FREEBUFFER
  /* */ "FREECHUNK"_Stz,            // CN_FREECHUNK
  /* */ "FREEDNOT"_Stz,             // CN_FREEDNOT
  /* */ "FREEITEM"_Stz,             // CN_FREEITEM
  /* */ "FREQDIST"_Stz,             // CN_FREQDIST
   
  /* */ "_G__"_Stz,                 // CN__G__
  /* */ "GRID"_Stz,                 // CN_GRID
 
  /* */ "_H__"_Stz,                 // CN__H__
  /* */ "HASHBUFFER"_Stz,           // CN_HASHBUFFER
  /* */ "HASHSET"_Stz,              // CN_HASHSET
  /* */ "HASHMAP"_Stz,              // CN_HASHMAP
  /* */ "HEAPBUFFER"_Stz,           // CN_HEAPBUFFER
  /* */ "HEAPMEM"_Stz,              // CN_HEAPMEM

  /* */ "_I__"_Stz,                 // CN__I__
  /* */ "INDEX"_Stz,                // CN_INDEX
  /* */ "INDEXDEF"_Stz,             // CN_INDEXDEF
  /* */ "INDEXSTREAM"_Stz,          // CN_INDEXSTREAM
  /* */ "INDEXTREE"_Stz,            // CN_INDEXTREE
  /* */ "INDEXTREEVEC"_Stz,         // CN_INDEXTREEVEC
  /* */ "IDXVEC"_Stz,               // CN_IDXVEC
  /* */ "INFOSET"_Stz,              // CN_INFOSET
  /* */ "INGESTCHUNK"_Stz,          // CN_INGESTCHUNK
  /* */ "INPUT"_Stz,                // CN_INPUT
  /* */ "INTERNALTABLES"_Stz,       // CN_INTERNALTABLES
  /* */ "INTSET"_Stz,               // CN_INTSET
  /* */ "INTSTACK"_Stz,             // CN_INTSTACK
  /* */ "INTVAL"_Stz,               // CN_INTVAL
  /* */ "IOCHUNK"_Stz,              // CN_IOCHUNK
  /* */ "IOCOPY"_Stz,               // CN_IOCOPY
  /* */ "IVLVAL"_Stz,               // CN_IVLVAL
 
  /* */ "_J__"_Stz,                 // CN__J__
  /* */ "JOB"_Stz,                  // CN_JOB
  /* */ "JOBLIST"_Stz,              // CN_JOBLIST
  /* */ "JOBREPORT"_Stz,            // CN_JOBREPORT
  /* */ "JOBS"_Stz,                 // CN_JOBS

  /* */ "_K__"_Stz,                 // CN__K__
  /* */ "KEY"_Stz,                  // CN_KEY
   
  /* */ "_L__"_Stz,                 // CN__L__
  /* */ "LARGEPERSISTENT"_Stz,      // CN_LARGEPERSISTENT
  /* */ "LARGETRANSIENT"_Stz,       // CN_LARGETRANSIENT
  /* */ "LARGEVIRTUAL"_Stz,         // CN_LARGEVIRTUAL
  /* */ "LICENCE"_Stz,              // CN_LICENCE
  /* */ "LIMITS"_Stz,               // CN_LIMITS
  /* */ "LISTEXONODE_t"_Stz,        // CN_LISTEXONODE_t
  /* */ "LISTEXOITER_t"_Stz,        // CN_LISTEXOITER_t
  /* */ "LOADCHUNK"_Stz,            // CN_LOADCHUNK
  /* */ "LOCAL"_Stz,                // CN_LOCAL
  /* */ "LOCALCHAIN"_Stz,           // CN_LOCALCHAIN
  /* */ "LOCALDIR"_Stz,             // CN_LOCALDIR
  /* */ "LOCALFILE"_Stz,            // CN_LOCALFILE
  /* */ "LOCATION"_Stz,             // CN_LOCATION
  /* */ "LOCATOR"_Stz,              // CN_LOCATOR
  /* */ "LOCK"_Stz,                 // CN_LOCK
  /* */ "LOCKSET"_Stz,              // CN_LOCKSET
  /* */ "LOGICCOMPILER"_Stz,        // CN_LOGICCOMPILER
  /* */ "LOGICTIMER"_Stz,           // CN_LOGICTIMER
  /* */ "LOGICUNIT"_Stz,            // CN_LOGICUNIT
 
  /* */ "_M__"_Stz,                 // CN__M__
  /* */ "MARKER"_Stz,               // CN_MARKER
  /* */ "MEDIUMPERSISTENT"_Stz,     // CN_MEDIUMPERSISTENT
  /* */ "MEDIUMTRANSIENT"_Stz,      // CN_MEDIUMTRANSIENT
  /* */ "MEDIUMVIRTUAL"_Stz,        // CN_MEDIUMVIRTUAL
  /* */ "MEMTEST"_Stz,              // CN_MEMTEST
  /* */ "METADATA"_Stz,             // CN_METADATA
  /* */ "METAGET"_Stz,             // CN_METAGET
  /* */ "MUTEX"_Stz,                // CN_MUTEX
 
  /* */ "_N__"_Stz,                 // CN__N__
  /* */ "NAMEDBUFFER"_Stz,          // CN_NAMEDBUFFER
  /* */ "NESTEDPIPELINESPEC"_Stz,   // CN_NESTEDPIPELINESPEC
  /* */ "NEXTGID"_Stz,              // CN_NEXTGID
  /* */ "NMLSTR"_Stz,               // CN_NMLSTR
  /* */ "NODE"_Stz,                 // CN_NODE
  /* */ "NODE_I"_Stz,               // CN_NODE_I
  /* */ "NOW"_Stz,                  // CN_NOW
  /* */ "NUMBAT"_Stz,               // CN_NUMBAT
  /* */ "NUMBERING"_Stz,            // CN_NUMBERING

  /* */ "_O__"_Stz,                 // CN__O__
  /* */ "OBJBUFFER"_Stz,            // CN_OBJBUFFER
  /* */ "OBJECTSNAPSHOT"_Stz,       // CN_OBJECTSNAPSHOT
  /* */ "OUTPUT"_Stz,               // CN_OUTPUT
  /* */ "OP"_Stz,                   // CN_OP
  /* */ "OPENFILES"_Stz,            // CN_OPENFILES
  /* */ "OPERANDQUEUE"_Stz,         // CN_OPERANDQUEUE
  /* */ "OPSPEC"_Stz,               // CN_OPSPEC
  /* */ "OPTIMUS"_Stz,              // CN_OPTIMUS
  /* */ "OTHEROPERAND"_Stz,         // CN_OTHEROPERAND

  /* */ "_P__"_Stz,                 // CN__P__
  /* */ "PARCEL"_Stz,               // CN_PARCEL
  /* */ "PATHIDENTIFIER"_Stz,       // CN_PATHIDENTIFIER
  /* */ "PATHVEC"_Stz,              // CN_PATHVEC
  /* */ "PERUSE"_Stz,               // CN_PERUSE
  /* */ "PIPELINE"_Stz,             // CN_PIPELINE
  /* */ "PIPELINESPEC"_Stz,         // CN_PIPELINESPEC
  /* */ "PLANDEFINITIONPARSER"_Stz, // CN_PLANDEFINITIONPARSER
  /* */ "PLANNERLOCK"_Stz,          // CN_PLANNERLOCK
  /* */ "PLANNODE"_Stz,             // CN_PLANNODE
  /* */ "PLANNODEBUILDER"_Stz,      // CN_PLANNODEBUILDER
  /* */ "PLANNODEBUILDERLIST"_Stz,  // CN_PLANNODEBUILDERLIST
  /* */ "PLANNODEBUILDERSTAACK"_Stz,// CN_PLANNODEBUILDERSTACK
  /* */ "PLANNODELIST"_Stz,         // CN_PLANNODELIST
  /* */ "PLANOPERAND"_Stz,          // CN_PLANOPERAND
  /* */ "POT"_Stz,                  // CN_POT
  /* */ "PREDICATE"_Stz,            // CN_PREDICATE
  /* */ "PREDICATEVEC"_Stz,         // CN_PREDICATEVEC
  /* */ "PRIMITIVENODE"_Stz,        // CN_PRIMITIVENODE
  /* */ "PROCEDURE"_Stz,            // CN_PROCEDURE
  /* */ "PROFILE"_Stz,              // CN_PROFILE
  /* */ "PSXTHD"_Stz,               // CN_PSXTHD
  /* */ "PTNMAP"_Stz,               // CN_PTNMAP
  /* */ "PTNSTREAM"_Stz,            // CN_PTNSTREAM
  /* */ "PTRVAL"_Stz,               // CN_PTRVAL
   
  /* */ "_Q__"_Stz,                 // CN__Q__
  /* */ "QUERYNODE"_Stz,            // CN_QUERYNODE
  /* */ "QUERYNODELIST"_Stz,        // CN_QUERYNODELIST
  /* */ "QUERYNODESTACK"_Stz,       // CN_QUERYNODESTACK
  /* */ "QUEUE"_Stz,                // CN_QUEUE
  /* */ "QUEUEBUFFER"_Stz,          // CN_QUEUEBUFFER
   
  /* */ "_R__"_Stz,                 // CN__R__
  /* */ "RANGE"_Stz,                // CN_RANGE
  /* */ "READER"_Stz,               // CN_READER
  /* */ "REAPLOCK"_Stz,             // CN_REAPLOCK
  /* */ "RECORD"_Stz,               // CN_RECORD
  /* */ "RELOP"_Stz,                // CN_RELOP
  /* */ "RESOURCETRACKER"_Stz,      // CN_RESOURCES
  /* */ "RLUDIARY"_Stz,             // CN_RLUDIARY
  /* */ "RN_RAND"_Stz,              // CN_RN_RAND
  /* */ "ROOTNODE"_Stz,             // CN_ROOTNODE
  /* */ "ROW"_Stz,                  // CN_ROW
  /* */ "ROWAGG"_Stz,               // CN_ROWAGG
  /* */ "ROWBOX"_Stz,               // CN_ROWBOX
  /* */ "ROWBUFFER"_Stz,            // CN_ROWBUFFER
  /* */ "ROWCOMPACTOR"_Stz,         // CN_ROWCOMPACTOR
  /* */ "ROWONCE"_Stz,              // CN_ROWONCE
  /* */ "ROWPIX"_Stz,               // CN_ROWPIX
  /* */ "ROWSINGLE"_Stz,            // CN_ROWSINGLE
  /* */ "ROWSINGLELIST"_Stz,        // CN_ROWSINGLELIST
  /* */ "ROWTABLEPTN"_Stz,          // CN_ROWTABLEPTN
  /* */ "ROWTRAY"_Stz,              // CN_ROWTRAY
  /* */ "RUCPU"_Stz,                // CN_RUCPU
  /* */ "RUDIR"_Stz,                // CN_RUDIR
  /* */ "RUFILE"_Stz,               // CN_RUFILE
  /* */ "RUMEMORY"_Stz,             // CN_RUMEMORY
  /* */ "RUMUTEX"_Stz,              // CN_RUMUTEX
  /* */ "RUPASS"_Stz,               // CN_RUPASS
  /* */ "RUSOCKET"_Stz,             // CN_RUSOCKET
  /* */ "RUTIME"_Stz,               // CN_RUTIME
  /* */ "RUWORK"_Stz,               // CN_RUWORK
 
  /* */ "_S__"_Stz,                 // CN__S__
  /* */ "SCANNER"_Stz,              // CN_SCANNER
  /* */ "SCHEDULER"_Stz,            // CN_SCHEDULER
  /* */ "SCHEMA"_Stz,               // CN_SCHEMA
  /* */ "SEAPI"_Stz,                // CN_SEAPI
  /* */ "SEDEPUTY"_Stz,             // CN_SEDEPUTY
  /* */ "SEEME"_Stz,                // CN_SEEME
  /* */ "SENCOMPILER"_Stz,          // CN_SENCOMPILER
  /* */ "SENDS"_Stz,                // CN_SENDS
  /* */ "SEPROMPT"_Stz,             // CN_SEPROMPT
  /* */ "SESSION"_Stz,              // CN_SESSION
  /* */ "SESSIONARRAY"_Stz,         // CN_SESSIONS
  /* */ "SHAPE"_Stz,                // CN_SHAPE
  /* */ "SHARDDEF"_Stz,             // CN_SHARDDEF
  /* */ "SHORTCUT"_Stz,             // CN_SHORTCUT
  /* */ "SHORTCUTVEC"_Stz,          // CN_SHORTCUTVEC
  /* */ "SINCE"_Stz,                // CN_SINCE
  /* */ "SKIPLIST"_Stz,             // CN_SKIPLIST
  /* */ "SKIPNODE"_Stz,             // CN_SKIPNODE
  /* */ "SLAB"_Stz,                 // CN_SLAB
  /* */ "SLABALLOCATOR"_Stz,        // CN_SLABALLOCATOR
  /* */ "SMALLPERSISTENT"_Stz,      // CN_SMALLPERSISTENT
  /* */ "SMALLTRANSIENT"_Stz,       // CN_SMALLTRANSIENT
  /* */ "SMALLVIRTUAL"_Stz,         // CN_SMALLVIRTUAL
  /* */ "SNBCD"_Stz,                // CN_SNBCD
  /* */ "SNBOL"_Stz,                // CN_SNBOL
  /* */ "SNDTE"_Stz,                // CN_SNDTE
  /* */ "SNFLT"_Stz,                // CN_SNFLT
  /* */ "SNINT"_Stz,                // CN_SNINT
  /* */ "SNIVL"_Stz,                // CN_SNIVL
  /* */ "SNTME"_Stz,                // CN_SNTME
  /* */ "SNTSP"_Stz,                // CN_SNTSP
  /* */ "SNVCH"_Stz,                // CN_SNVCH
  /* */ "SOCKET"_Stz,               // CN_SOCKET
  /* */ "SPELAPSED"_Stz,            // CN_SPELAPSED
  /* */ "SPID"_Stz,                 // CN_SPID
  /* */ "SPSTRING"_Stz,             // CN_SPSTRING
  /* */ "SPMEDIUM"_Stz,             // CN_SPMEDIUM
  /* */ "SPSMALL"_Stz,              // CN_SPSMALL
  /* */ "SQLPLANNER"_Stz,           // CN_SQLPLANNER
  /* */ "STACK"_Stz,                // CN_STACK
  /* */ "STAGE"_Stz,                // CN_STAGE
  /* */ "STAGESPEC"_Stz,            // CN_STAGESPEC
  /* */ "STATISTICS"_Stz,           // CN_STATISTICS
  /* */ "STATS"_Stz,                // CN_STATS
  /* */ "STENCIL"_Stz,              // CN_STENCIL
  /* */ "STEP"_Stz,                 // CN_STEP
  /* */ "STEPACCUM"_Stz,            // CN_STEPACCUM
  /* */ "STEPS"_Stz,                // CN_STEPS
  /* */ "STREAM"_Stz,               // CN_STREAM
  /* */ "STREAMDEF"_Stz,            // CN_STREAMDEF
  /* */ "STREAMDEFRHT"_Stz,         // CN_STREAMDEFRHT
  /* */ "STREAMSET"_Stz,            // CN_STREAMSET
  /* */ "STRING_T"_Stz,             // CN_STRING_T
  /* */ "STZLIST"_Stz,              // CN_STZLIST
  /* */ "STZVAL"_Stz,               // CN_STZVAL
  /* */ "SYSCALL"_Stz,              // CN_SYSCALL
  /* */ "SYSTEMSCHEDULE"_Stz,       // CN_SCHEDULE

  /* */ "_T__"_Stz,                 // CN__T__
  /* */ "TABLE"_Stz,                // CN_TABLE
  /* */ "TABLEDEF"_Stz,             // CN_TABLEDEF
  /* */ "TABLEPTN"_Stz,             // CN_TABLEPTN
  /* */ "TABLEPTNVEC"_Stz,          // CN_TABLEPTNVEC
  /* */ "TAGSET"_Stz,               // CN_TAGSET
  /* */ "TASK"_Stz,                 // CN_TASK
  /* */ "TASKLIST"_Stz,             // CN_TASKLIST
  /* */ "TASKQUEUE"_Stz,            // CN_TASKQUEUE
  /* */ "TCCLOCK"_Stz,              // CN_TCCLOCK
  /* */ "TEMPNAME"_Stz,             // CN_TEMPNAME
  /* */ "TENANT"_Stz,               // CN_TENANT
  /* */ "TENANTS"_Stz,              // CN_TENANTS
  /* */ "TEXT"_Stz,                 // CN_TEXT,
  /* */ "TEXTLARGE"_Stz,            // CN_TEXTLARGE,
  /* */ "TEXTMEDIUM"_Stz,           // CN_TEXTMEDIUM,
  /* */ "TEXTSMALL"_Stz,            // CN_TEXTSMALL,
  /* */ "TEXTSTACK"_Stz,            // CN_TEXTSTACK,
  /* */ "TEXTTINY"_Stz,             // CN_TEXTTINY,
  /* */ "THING"_Stz,                // CN_THING
  /* */ "THREAD"_Stz,               // CN_THREAD
  /* */ "THREADQUEUE"_Stz,          // CN_THREADQUEUE
  /* */ "THREADVEC"_Stz,            // CN_THREADVEC
  /* */ "TIMER"_Stz,                // CN_TIMER
  /* */ "TINYC"_Stz,                // CN_TINYC
  /* */ "TINYPERSISTENT"_Stz,       // CN_TINYPERSISTENT
  /* */ "TINYTRANSIENT"_Stz,        // CN_TINYTRANSIENT
  /* */ "TINYVIRTUAL"_Stz,          // CN_TINYVIRTUAL
  /* */ "TMEVAL"_Stz,               // CN_TMEVAL
  /* */ "TOKEN"_Stz,                // CN_TOKEN
  /* */ "TOBKTCRW"_Stz,             // CN_TOBKTCRW
  /* */ "TOBKTROW"_Stz,             // CN_TOBKTROW
  /* */ "TOKENVEC"_Stz,             // CN_TOKENVEC
  /* */ "TONSTROW"_Stz,             // CN_TONSTROW
  /* */ "TOPTNCRW"_Stz,             // CN_TOPTNCRW
  /* */ "TOPTNROW"_Stz,             // CN_TOPTNROW
  /* */ "TOROW"_Stz,                // CN_TOROW
  /* */ "TRACELOCK"_Stz,            // CN_TRACELOCK
  /* */ "TRACEMULTILOCK"_Stz,       // CN_TRACEMULTILOCK
  /* */ "TSPVAL"_Stz,               // CN_TSPVAL
  /* */ "TUB"_Stz,                  // CN_TUB
  /* */ "TX"_Stz,                   // CN_TX
  /* */ "TXSET"_Stz,                // CN_TXSET
   
  /* */ "_U__"_Stz,                 // CN__U__
  /* */ "UNITINT"_Stz,              // CN_UNITINT
  /* */ "UT_SKIPLIST"_Stz,          // CN_UT_SKIPLIST

  /* */ "_V__"_Stz,                 // CN__V__
  /* */ "VERBOSE"_Stz,              // CN_VERBOSE

  /* */ "_W__"_Stz,                 // CN__W__
  /* */ "WAKEUP"_Stz,               // CN_WAKEUP
  /* */ "WILDERNESS"_Stz,           // CN_WILDERNESS
  /* */ "WORD"_Stz,                 // CN_WORD
  /* */ "WSPCONVERGENT_t"_Stz,      // CN_WSPCONVERGENT_t
 
  /* */ "_X__"_Stz,                 // CN__X__
   
  /* */ "_Y__"_Stz,                 // CN__Y__
   
  /* */ "_Z__"_Stz,                 // CN__Z__
   
  /* */ "ENUMS"_Stz,                // CN_ENUMS
};
  

  class ClassNumbers
  {
  public:
    static eCN eFind(Stz* szszClassNm);

  };

  eCN _eClassNo(i8 iClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
