/*============================================================================*/
/* ClassNumbers.cpp -- Functions for Class Numbers                            */
/*============================================================================*/

#include "BASE.h"

/*----------------------------------------------------------------------------*/
/* find eCN for given class name                                              */
/*----------------------------------------------------------------------------*/
eCN ClassNumbers::eFind(Stz* szClassNm)
{
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
    if (szClassNm->bSame(aCN[i_Class]))
      return (eCN)i_Class;
  
  return CN_NONE;
}

/*----------------------------------------------------------------------------*/
/* standard safe cast for eCN                                                 */
/*----------------------------------------------------------------------------*/
eCN BASE::_eClassNo(i8 iClassNo)
{
  eCN eClassNo = (eCN)iClassNo;
  
  ASSERT(eClassNo >= CN_NONE && eClassNo <= CN_ENUMS);
  
  return eClassNo;
}

/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
Stz* BASE::aCN[CN_ENUMS]
{
    /* */ "NONE"_Stz,                 // CN_NONE

    /* */ "_A__"_Stz,                 // CN__A__
    /* */ "ACCESSPATH"_Stz,           // CN_ACCESSPATH
    /* */ "ACTION"_Stz,               // CN_ACTION
    /* */ "ADAPTORWL"_Stz,            // CN_ADAPTORWL
    /* */ "AGGTIMER"_Stz,             // CN_AGGTIMER
    /* */ "AM"_Stz,                   // CN_AM
    /* */ "ANSWERWL"_Stz,             // CN_ANSWERWL
    /* */ "API"_Stz,                  // CN_API
    /* */ "ARG"_Stz,                  // CN_ARG
    /* */ "ARGDEF"_Stz,               // CN_ARGDEF
    /* */ "ARRANGE"_Stz,              // CN_ARRANGE
    /* */ "ATOMIC"_Stz,               // CN_ATOMIC
    /* */ "ATTACHMAP"_Stz,            // CN_ATTACHMAP

    /* */ "_B__"_Stz,                 // CN__B__
    /* */ "BACKEND"_Stz,              // CN_BACKEND
    /* */ "BALLOON"_Stz,              // CN_BALLOON
    /* */ "BCDVAL"_Stz,               // CN_BCDVAL
    /* */ "BENCH"_Stz,                // CN_BENCH
    /* */ "BIRTH"_Stz,                // CN_BIRTH
    /* */ "BOARD"_Stz,                // CN_BOARD
    /* */ "BOLVAL"_Stz,               // CN_BOLVAL
    /* */ "BUFFERMAP"_Stz,            // CN_BUFFERMAP
    /* */ "BUFFERPOOL"_Stz,           // CN_BUFFERPOOL
    /* */ "BYTERATE"_Stz,             // CN_BYTERATE
    /* */ "BYTES_T"_Stz,              // CN_BYTES_T

    /* */ "_C__"_Stz,                 // CN__C__
    /* */ "CARD"_Stz,                 // CN_CARD
    /* */ "CARDCTL"_Stz,              // CN_CARDCTL
    /* */ "CARDHISTORY"_Stz,          // CN_CARDHISTORY
    /* */ "CARDIMAGE"_Stz,            // CN_CARDIMAGE
    /* */ "CARDITER"_Stz,             // CN_CARDITER
    /* */ "CARDITERMEMBERS"_Stz,      // CN_CARDITERMEMBERS
    /* */ "CARDUSAGE"_Stz,            // CN_CARDUSAGE
    /* */ "CARRYFORWARD"_Stz,         // CN_CARRYFORWARD
    /* */ "CATALOG"_Stz,              // CN_CATALOG
    /* */ "CHARSER"_Stz,              // CN_CHARSER
    /* */ "CHILDEXIT"_Stz,            // CN_CHILDEXIT
    /* */ "CLASSPLACEHOLDER"_Stz,     // CN_CLASSPLACEHOLDER
    /* */ "CMDLINEARGS"_Stz,          // CN_CMDLINEARGS
    /* */ "CODEBLOCK"_Stz,            // CN_CODEBLOCK
    /* */ "COLHSH"_Stz,               // CN_COLHSH
    /* */ "COLMAP"_Stz,               // CN_COLMAP
    /* */ "COLSREQ"_Stz,              // CN_COLSREQ
    /* */ "COLUMN"_Stz,               // CN_COLUMN
    /* */ "COMBINEDSRC"_Stz,          // CN_COMBINEDSRC
    /* */ "CONDITIONALQUERYNODEITER"_Stz, // CN_CONDITIONALQUERYNODEITER
    /* */ "CORRELATEDJOIN"_Stz,       // CN_CORRELATEDJOIN
    /* */ "CRVBUFFER"_Stz,            // CN_CRVBUFFER
    /* */ "CRWBUFFER"_Stz,            // CN_CRWBUFFER
    /* */ "CRWTABLEPTN"_Stz,          // CN_CRWTABLEPTN
    /* */ "CRWTOROW"_Stz,             // CN_CRWTOROW
    /* */ "CSVBUFFER"_Stz,            // CN_CSVBUFFER
    /* */ "CSVMETA"_Stz,              // CN_CSVMETA

    /* */ "_D__"_Stz,                 // CN__D__
    /* */ "DATATRANSIENT"_Stz,        // CN_DATATRANSIENT
    /* */ "DDLLOCK"_Stz,              // CN_DDLLOCK
    /* */ "DECACNT"_Stz,              // CN_DECACNT
    /* */ "DECIDER"_Stz,              // CN_DECIDER
    /* */ "DECK"_Stz,                 // CN_DECK
    /* */ "DECKSPEC"_Stz,             // CN_DECKSPEC
    /* */ "DEFINITIONNODE"_Stz,       // CN_DEFINITIONNODE
    /* */ "DEREFMETRICS"_Stz,         // CN_DEREFMETRICS
    /* */ "DIC"_Stz,                  // CN_DIC
    /* */ "DICLOCK"_Stz,              // CN_DICLOCK
    /* */ "DICSTACK"_Stz,             // CN_DICSTACK
    /* */ "DIR"_Stz,                  // CN_DIR
    /* */ "DISSECTER"_Stz,            // CN_DISSECTOR
    /* */ "DTEVAL"_Stz,               // CN_DTEVAL
    /* */ "DYNAMIC"_Stz,              // CN_DYNAMIC
    /* */ "DYNAMO"_Stz,               // CN_DYNAMO

    /* */ "_E__"_Stz,                 // CN__E__
    /* */ "EDTVAL"_Stz,               // CN_EDTVAL
    /* */ "ENCODING"_Stz,             // CN_ENCODING
    /* */ "ENT"_Stz,                  // CN_ENT
    /* */ "ENTBUFFER"_Stz,            // CN_ENTBUFFER
    /* */ "ENTBUFSORTER"_Stz,         // CN_ENTBUFSORTER
    /* */ "ENTWRK"_Stz,               // CN_ENTWRK
    /* */ "ENUMFLAGS"_Stz,            // CN_ENUMFLAGS
    /* */ "EQUIVALENCE"_Stz,          // CN_EQUIVALENCE
    /* */ "EVENTHANDLERS"_Stz,        // CN_EVENTHANDLERS
    /* */ "EVMGR"_Stz,                // CN_EVMGR
    /* */ "EXEMPLAR"_Stz,             // CN_EXEMPLAR
    /* */ "EXITOPERAND"_Stz,          // CN_EXITOPERAND
    /* */ "EXTENDEDTEXT"_Stz,         // CN_EXTENDEDTEXT
    /* */ "EXTENDEDTEXTBUFFER"_Stz,   // CN_EXTENDEDTEXTBUFFER
    /* */ "EXTRACTMAP"_Stz,           // CN_EXTRACTMAP

    /* */ "_F__"_Stz,                 // CN__F__
    /* */ "FIFO"_Stz,                 // CN_FIFO
    /* */ "FILE"_Stz,                 // CN_FILE
    /* */ "FILEMAP"_Stz,              // CN_FILEMAP
    /* */ "FIPVAL"_Stz,               // CN_FIPVAL
    /* */ "FLTVAL"_Stz,               // CN_FLTVAL
    /* */ "FOLD"_Stz,                 // CN_FOLD
    /* */ "FORMAT"_Stz,               // CN_FORMAT
    /* */ "FREEPOOLBUFFER"_Stz,       // CN_FREEPOOLBUFFER
    /* */ "FREECHUNK"_Stz,            // CN_FREECHUNK
    /* */ "FREEDNOT"_Stz,             // CN_FREEDNOT
    /* */ "FREEITEM"_Stz,             // CN_FREEITEM
    /* */ "FREQDIST"_Stz,             // CN_FREQDIST

    /* */ "_G__"_Stz,                 // CN__G__
    /* */ "GRID"_Stz,                 // CN_GRID

    /* */ "_H__"_Stz,                 // CN__H__
    /* */ "HASHMAP"_Stz,              // CN_HASHMAP
    /* */ "HASHMAPENTRY"_Stz,         // CN_HASHMAPENTRY
    /* */ "HASHMAPSLOT"_Stz,          // CN_HASHMAPSLOT
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
    /* */ "JOBMETRICS"_Stz,           // CN_JOBMETRICS
    /* */ "JOBREPORT"_Stz,            // CN_JOBREPORT
    /* */ "JOBS"_Stz,                 // CN_JOBS

    /* */ "_K__"_Stz,                 // CN__K__
    /* */ "KEY"_Stz,                  // CN_KEY
    /* */ "KID"_Stz,                  // CN_KID

    /* */ "_L__"_Stz,                 // CN__L__
    /* */ "LARGEPERSISTENT"_Stz,      // CN_LARGEPERSISTENT
    /* */ "LARGETRANSIENT"_Stz,       // CN_LARGETRANSIENT
    /* */ "LARGEVIRTUAL"_Stz,         // CN_LARGEVIRTUAL
    /* */ "LGLBUFSET"_Stz,            // CN_LGLBUFSET
    /* */ "LGLPTNMAP"_Stz,            // CN_LGLPTNMAP
    /* */ "LIBINFO"_Stz,              // CN_LIBINFO
    /* */ "LICENCE"_Stz,              // CN_LICENCE
    /* */ "LIMITS"_Stz,               // CN_LIMITS
    /* */ "LISTEXONODE_t"_Stz,        // CN_LISTEXONODE_t
    /* */ "LISTEXOITER_t"_Stz,        // CN_LISTEXOITER_t
    /* */ "LIT"_Stz,                  // CN_LIT
    /* */ "LITRECOGNISER"_Stz,        // CN_LITRECOGNISER
    /* */ "LOADCHUNK"_Stz,            // CN_LOADCHUNK
    /* */ "LOCALCHAIN"_Stz,           // CN_LOCALCHAIN
    /* */ "LOCALDIR"_Stz,             // CN_LOCALDIR
    /* */ "LOCALFILE"_Stz,            // CN_LOCALFILE
    /* */ "LOCALSTRS"_Stz,            // CN_LOCALSTRS
    /* */ "LOCALVARS"_Stz,            // CN_LOCALVARS
    /* */ "LOCATION"_Stz,             // CN_LOCATION
    /* */ "LOCATOR"_Stz,              // CN_LOCATOR
    /* */ "LOCK"_Stz,                 // CN_LOCK
    /* */ "LOCKS"_Stz,                // CN_LOCKS
    /* */ "LOCKSET"_Stz,              // CN_LOCKSET
    /* */ "LOGICUNIT"_Stz,            // CN_LOGICUNIT

    /* */ "_M__"_Stz,                 // CN__M__
    /* */ "MARK"_Stz,                 // CN_MARK
    /* */ "MARKER"_Stz,               // CN_MARKER
    /* */ "MARKERCHAIN"_Stz,          // CN_MARKERCHAIN
    /* */ "MARKERMGR"_Stz,            // CN_MARKERMGR
    /* */ "MEDIUMPERSISTENT"_Stz,     // CN_MEDIUMPERSISTENT
    /* */ "MEDIUMTRANSIENT"_Stz,      // CN_MEDIUMTRANSIENT
    /* */ "MEDIUMVIRTUAL"_Stz,        // CN_MEDIUMVIRTUAL
    /* */ "MEMTEST"_Stz,              // CN_MEMTEST
    /* */ "METADATA"_Stz,             // CN_METADATA
    /* */ "METAGET"_Stz,              // CN_METAGET
    /* */ "METRICSWSP"_Stz,           // CN_METRICSWSP
    /* */ "MUTEX"_Stz,                // CN_MUTEX

    /* */ "_N__"_Stz,                 // CN__N__
    /* */ "NAMEDBUFFER"_Stz,          // CN_NAMEDBUFFER
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
    /* */ "OBJTRACKER"_Stz,           // CN_OBJTRACKER
    /* */ "OUTPUT"_Stz,               // CN_OUTPUT
    /* */ "OP"_Stz,                   // CN_OP
    /* */ "OPDEF"_Stz,                // CN_OPDEF
    /* */ "OPENFILES"_Stz,            // CN_OPENFILES
    /* */ "OPERANDQUEUE"_Stz,         // CN_OPERANDQUEUE
    /* */ "OPERANDWATCH"_Stz,         // CN_OPERANDWATCH
    /* */ "OPSKETCH"_Stz,             // CN_OPSKETCH
    /* */ "OPMETRICS"_Stz,            // CN_OPMETRICS
    /* */ "OPSPEC"_Stz,               // CN_OPSPEC
    /* */ "OPTIMISER"_Stz,            // CN_OPTIMISER
    /* */ "OTHEROPERAND"_Stz,         // CN_OTHEROPERAND

    /* */ "_P__"_Stz,                 // CN__P__
    /* */ "PAGEFILE"_Stz,             // CN_PAGEFILE
    /* */ "PAGEINBUFFER"_Stz,         // CN_PAGEINBUFFER
    /* */ "PAGINGMETRICS"_Stz,        // CN_PAGINGMETRICS
    /* */ "PARAMSPEC"_Stz,            // CN_PARAMSPEC
    /* */ "PARAMSSPEC"_Stz,           // CN_PARAMSSPEC
    /* */ "PARCEL"_Stz,               // CN_PARCEL
    /* */ "PATHIDENTIFIER"_Stz,       // CN_PATHIDENTIFIER
    /* */ "PATHVEC"_Stz,              // CN_PATHVEC
    /* */ "PAYSTR"_Stz,               // CN_PATHSTR
    /* */ "PAYSTRBUFFER"_Stz,         // CN_PATHSTRBUFFER
    /* */ "PDS"_Stz,                  // CN_PDS
          "PDSBUFFERIDQUEUE"_Stz,     // CN_PDSBUFFERIDQUEUE
    /* */ "PDSCURSOR"_Stz,            // CN_PDSCURSOR
    /* */ "PERMGEN"_Stz,              // CN_PERMGEN
    /* */ "PERMIT"_Stz,               // CN_PERMIT
    /* */ "PERUSE"_Stz,               // CN_PERUSE
    /* */ "PIPEIN"_Stz,               // CN_PIPEIN
    /* */ "PIPELINE"_Stz,             // CN_PIPELINE
    /* */ "PIPELINEDEF"_Stz,          // CN_PIPELINEDEF
    /* */ "PIPELINEMETRICS"_Stz,      // CN_PIPELINEMETRICS
    /* */ "PLANDEFINITIONPARSER"_Stz, // CN_PLANDEFINITIONPARSER
    /* */ "PLANNODE"_Stz,             // CN_PLANNODE
    /* */ "PLANNODEBUILDER"_Stz,      // CN_PLANNODEBUILDER
    /* */ "PLANNODEBUILDERLIST"_Stz,  // CN_PLANNODEBUILDERLIST
    /* */ "PLANNODEBUILDERSTAACK"_Stz,// CN_PLANNODEBUILDERSTACK
    /* */ "PLANNODELIST"_Stz,         // CN_PLANNODELIST
    /* */ "PLANOPERAND"_Stz,          // CN_PLANOPERAND
    /* */ "POT"_Stz,                  // CN_POT
    /* */ "PORT"_Stz,                 // CN_PORT
    /* */ "PPY"_Stz,                  // CN_PPY
    /* */ "PREDICATE"_Stz,            // CN_PREDICATE
    /* */ "PREDICATEVEC"_Stz,         // CN_PREDICATEVEC
    /* */ "PRIMITIVENODE"_Stz,        // CN_PRIMITIVENODE
    /* */ "PROCEDURE"_Stz,            // CN_PROCEDURE
    /* */ "PROFILE"_Stz,              // CN_PROFILE
    /* */ "PSXTHD"_Stz,               // CN_PSXTHD
    /* */ "PTNDEF"_Stz,               // CN_PTNDEF
    /* */ "PTNID"_Stz,                // CN_PTNID
    /* */ "PTRVAL"_Stz,               // CN_PTRVAL

    /* */ "_Q__"_Stz,                 // CN__Q__
    /* */ "QUERYNODE"_Stz,            // CN_QUERYNODE
    /* */ "QUERYNODELIST"_Stz,        // CN_QUERYNODELIST
    /* */ "QUERYNODESTACK"_Stz,       // CN_QUERYNODESTACK
    /* */ "QUEUE"_Stz,                // CN_QUEUE
    /* */ "QUEUEBUFFER"_Stz,          // CN_QUEUEBUFFER

    /* */ "_R__"_Stz,                 // CN__R__
    /* */ "READER"_Stz,               // CN_READER
    /* */ "REAPLOCK"_Stz,             // CN_REAPLOCK
    /* */ "RECORD"_Stz,               // CN_RECORD
    /* */ "RECVWL"_Stz,               // CN_RECVWL
    /* */ "REJECTJOB"_Stz,            // CN_REJECTJOB
    /* */ "RELOP"_Stz,                // CN_RELOP
    /* */ "RESOURCES"_Stz,            // CN_RESOURCES
    /* */ "RIBBON"_Stz,               // CN_RIBBON
    /* */ "RIBBONLINE"_Stz,           // CN_RIBBONLINE
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
    /* */ "SAWOPCARDS"_Stz,           // CN_SAWOPCARDS
    /* */ "SCANNER"_Stz,              // CN_SCANNER
    /* */ "SCHEDULE"_Stz,             // CN_SCHEDULE
    /* */ "SCHEDULER"_Stz,            // CN_SCHEDULER
    /* */ "SCHEMA"_Stz,               // CN_SCHEMA
    /* */ "SCOPE"_Stz,                // CN_SCOPE
    /* */ "SEAPI"_Stz,                // CN_SEAPI
    /* */ "SEDEPUTY"_Stz,             // CN_SEDEPUTY
    /* */ "SEEME"_Stz,                // CN_SEEME
    /* */ "SENCELL"_Stz,              // CN_SENCELL
    /* */ "SENCOMPILER"_Stz,          // CN_SENCOMPILER
    /* */ "SENDS"_Stz,                // CN_SENDS
    /* */ "SENDWL"_Stz,               // CN_SENDWL
    /* */ "SEPROMPT"_Stz,             // CN_SEPROMPT
    /* */ "SHORTCUT"_Stz,             // CN_SHORTCUT
    /* */ "SHORTCUTVEC"_Stz,          // CN_SHORTCUTVEC
    /* */ "SINCE"_Stz,                // CN_SINCE
    /* */ "SKETCHTKN"_Stz,            // CN_SKETCHTKN
    /* */ "SKIPLIST"_Stz,             // CN_SKIPLIST
    /* */ "SKIPLISTNODE"_Stz,         // CN_SKIPLISTNODE
    /* */ "SKIPLISTBLOCK"_Stz,        // CN_SKIPLISTBLOCK
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
    /* */ "STAGEDEF"_Stz,             // CN_STAGEDEF
    /* */ "STAGEMETRICS"_Stz,         // CN_STAGEMETRICS
    /* */ "STAGESPEC"_Stz,            // CN_STAGESPEC
    /* */ "STARTER"_Stz,              // CN_STARTER
    /* */ "STATEMENT"_Stz,            // CN_STATEMENT
    /* */ "STATS"_Stz,                // CN_STATS
    /* */ "STENCIL_V2"_Stz,           // CN_STENCIL_V2
    /* */ "STEP"_Stz,                 // CN_STEP
    /* */ "STEPACCUM"_Stz,            // CN_STEPACCUM
    /* */ "STEPS"_Stz,                // CN_STEPS
    /* */ "STMTLIST"_Stz,             // CN_STMTLIST
    /* */ "STREAM"_Stz,               // CN_STREAM
    /* */ "STREAMDEF"_Stz,            // CN_STREAMDEF
    /* */ "STREAMDEFRHT"_Stz,         // CN_STREAMDEFRHT
    /* */ "STREAMMETRICS"_Stz,        // CN_STREAMMETRICS
    /* */ "STREAMSET"_Stz,            // CN_STREAMSET
    /* */ "STRING_T"_Stz,             // CN_STRING_T
    /* */ "STRLIB"_Stz,               // CN_STRLIB
    /* */ "STZLIST"_Stz,              // CN_STZLIST
    /* */ "STZVAL"_Stz,               // CN_STZVAL
    /* */ "SUBCALL"_Stz,              // CN_SUBCALL
    /* */ "SUBQUERY"_Stz,             // CN_SUBQUERY
    /* */ "SUBRETURN"_Stz,            // CN_SUBRETURN
    /* */ "SYMBOL"_Stz,               // CN_SYMBOL
    /* */ "SYSCALL"_Stz,              // CN_SYSCALL
    /* */ "SYSMETRICS"_Stz,           // CN_SYSMETRICS
    /* */ "SYSREPORT"_Stz,            // CN_SYSREPORT

    /* */ "_T__"_Stz,                 // CN__T__
    /* */ "TABLE"_Stz,                // CN_TABLE
    /* */ "TABLEDEF"_Stz,             // CN_TABLEDEF
    /* */ "TABLEPTN"_Stz,             // CN_TABLEPTN
    /* */ "TABLEPTNVEC"_Stz,          // CN_TABLEPTNVEC
    /* */ "TAGSET"_Stz,               // CN_TAGSET
    /* */ "TASK"_Stz,                 // CN_TASK
    /* */ "TASKLIST"_Stz,             // CN_TASKLIST
    /* */ "TASKQUEUE"_Stz,            // CN_TASKQUEUE
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
    /* */ "THREADSCRATCH"_Stz,        // CN_THREADSCRATCH
    /* */ "THREADVEC"_Stz,            // CN_THREADVEC
    /* */ "TIMER"_Stz,                // CN_TIMER
    /* */ "TINYPERSISTENT"_Stz,       // CN_TINYPERSISTENT
    /* */ "TINYTRANSIENT"_Stz,        // CN_TINYTRANSIENT
    /* */ "TINYVIRTUAL"_Stz,          // CN_TINYVIRTUAL
    /* */ "TMEVAL"_Stz,               // CN_TMEVAL
    /* */ "TOKEN"_Stz,                // CN_TOKEN
    /* */ "TOKENVEC"_Stz,             // CN_TOKENVEC
    /* */ "TOKEYHSHROW"_Stz,          // CN_TOKEYHSHROW
    /* */ "TOLGLPTNCRW"_Stz,          // CN_TOLGLPTNCRW
    /* */ "TOLGLPTNNST"_Stz,          // CN_TOLGLPTNNST
    /* */ "TOLGLPTNROW"_Stz,          // CN_TOLGLPTNROW
    /* */ "TOTUP"_Stz,                // CN_TOTUP
    /* */ "TRACELOCK"_Stz,            // CN_TRACELOCK
    /* */ "TRACEMULTILOCK"_Stz,       // CN_TRACEMULTILOCK
    /* */ "TSPVAL"_Stz,               // CN_TSPVAL
    /* */ "TUB"_Stz,                  // CN_TUB
    /* */ "TX"_Stz,                   // CN_TX
    /* */ "TXSET"_Stz,                // CN_TXSET
    /* */ "CN_TYPEMAP1D"_Stz,         // CN_TYPEMAP1D
    /* */ "CN_TYPEMAP2D"_Stz,         // CN_TYPEMAP2D

    /* */ "_U__"_Stz,                 // CN__U__
    /* */ "UNITINT"_Stz,              // CN_UNITINT
    /* */ "UT_SKIPLIST"_Stz,          // CN_UT_SKIPLIST

    /* */ "_V__"_Stz,                 // CN__V__
    /* */ "VERBOSE"_Stz,              // CN_VERBOSE
    /* */ "VERTEX"_Stz,               // CN_VERTEX
    /* */ "VERTEXSPEC"_Stz,           // CN_VERTEXSPEC

    /* */ "_W__"_Stz,                 // CN__W__
    /* */ "WAKEUP"_Stz,               // CN_WAKEUP
    /* */ "WEAVER"_Stz,               // CN_WEAVER
    /* */ "WILDERNESS"_Stz,           // CN_WILDERNESS
    /* */ "WORD"_Stz,                 // CN_WORD
    /* */ "WSPCONVERGENT_t"_Stz,      // CN_WSPCONVERGENT_t

    /* */ "_X__"_Stz,                 // CN__X__
    /* */ "XC"_Stz,                           // CN_XC
    /* */ "XF"_Stz,                           // CN_XF
    /* */ "XF_LEFT_SUBQUERY"_Stz,             // CN_XF_LEFT_SUBQUERY
    /* */ "XF_EPILOG_ITEMISE_COLNM"_Stz,                 // CN_XF_EPILOG_ITEMISE_COLNM
    /* */ "XF_EPILOG_ITEMSTRIP_COLNM"_Stz,               // CN_XF_EPILOG_ITEMSTRIP_COLNM
    /* */ "XF_PROLOG_ADD_LEVEL"_Stz,                 // CN_XF_ADD_LEVEL
    /* */ "XF_PROLOG_AS_REVIEW"_Stz,                 // CN_XF_AS_REVIEW
    /* */ "XF_PROLOG_COLNM_TO_STMCOL"_Stz,           // CN_XF_COLNM_TO_STMCOL
    /* */ "XF_EPILOG_COLUMN_HASHING"_Stz,            // CN_XF_COLUMN_HASHING
    /* */ "XF_EPILOG_COLUMN_MAPPING"_Stz,            // CN_XF_COLUMN_MAPPING
    /* */ "XF_OPTIMISE_CULL_NONUSED_BOUND"_Stz,        // CN_XF_CULL_NONUSED_BOUND
    /* */ "XF_FLATTEN_DELETE_EMPTY_WHERE"_Stz,        // CN_XF_DELETE_EMPTY_WHERE
    /* */ "XF_FLATTEN_DELETE_FLAT"_Stz,               // CN_XF_DELETE_FLAT
    /* */ "XF_PROLOG_ERASE_CONSTRUCTION"_Stz,           // CN_XF_ERASE_CONSTRUCTION
    /* */ "XF_FLATTEN_DERIVE_BOUNDS"_Stz,             // CN_XF_DERIVE_BOUNDS
    /* */ "XF_OPTIMISE_DERIVE_STREAMKEY"_Stz,         // CN_XF_DERIVE_STREAM_KEY
    /* */ "XF_FLATTEN_FIX_TABLE_POS"_Stz,             // CN_XF_FIX_TABLE_POS
    /* */ "XF_FLATTEN"_Stz,                   // CN_XF_FLATTEN
    /* */ "XF_FLATTEN_10_F"_Stz,              // CN_XF_FLATTEN_10_F,
    /* */ "XF_FLATTEN_24_W_A_C_RO"_Stz,       // CN_XF_FLATTEN_24_W_A_C_RO,
    /* */ "XF_FLATTEN_32_W_A_NC_I"_Stz,       // CN_XF_FLATTEN_32_W_A_NC_I,
    /* */ "XF_FLATTEN_34_W_A_NC_RO"_Stz,      // CN_XF_FLATTEN_34_W_A_NC_RO,
    /* */ "XF_FLATTEN_40_W_L_C_E"_Stz,        // CN_XF_FLATTEN_40_W_L_C_E,
    /* */ "XF_FLATTEN_52_W_L_NC_I_EXISTS"_Stz,// CN_XF_FLATTEN_52_W_L_NC_I_EXISTS,
    /* */ "XF_FLATTEN_52_W_L_NC_I_JOIN"_Stz,  // CN_XF_FLATTEN_52_W_L_NC_I_JOIN,
    /* */ "XF_FLATTEN_53_W_L_NC_NI_EXISTS"_Stz,  // CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS,
    /* */ "XF_FLATTEN_53_W_L_NC_NI_JOIN"_Stz, // CN_XF_FLATTEN_53_W_L_NC_NI_JOIN,
    /* */ "XF_FLATTEN_74_H_A_NC_RO"_Stz,      // CN_XF_FLATTEN_74_H_A_NC_RO,
    /* */ "XF_OPTIMISE_FROM_TO_OP"_Stz,                // CN_XF_FROM_TO_OP
    /* */ "XF_PROLOG_INDEX_ANALYSIS"_Stz,            // CN_XF_INDEX_ANALYSIS
    /* */ "XF_EPILOG_PROMOTE_FILTER"_Stz,        // CN_XF_INDIVIDUATE_FILTER
    /* */ "XF_PROLOG_MOVE_ON"_Stz,                   // CN_XF_MOVE_ON
    /* */ "XF_PROLOG_NOT_OPS"_Stz,                   // CN_XF_NOT_OPS
    /* */ "XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG"_Stz, // CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG
    /* */ "XF_OPTIMISE_EARLY_FILTER"_Stz,     // CN_XF_OPTIMISE_EARLY_FILTER
    /* */ "XF_OPTIMISE_LATE_PROJECTION"_Stz,  // CN_XF_OPTIMISE_LATE_PROJECTION
    /* */ "XF_OPTIMISE_MAX_TO_TOP"_Stz,       // CN_XF_OPTIMISE_MAX_TO_TOP
    /* */ "XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS"_Stz, // CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS
    /* */ "XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP"_Stz, // CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP
    /* */ "XF_OPTIMISE_STATIC_CALC"_Stz,      // CN_XF_OPTIMISE_STATIC_CALC
    /* */ "XF_OT_CUT_SORT"_Stz,                  // CN_XF_OT_CUT_SORT
    /* */ "XF_OT_DUPLICATE_FILTER"_Stz,          // CN_XF_OT_DUPLICATE_FILTER
    /* */ "XF_OT_EARLY_PROJECTION"_Stz,          // CN_XF_OT_EARLY_PROJECTION
    /* */ "XF_OT_JOIN_ORDERING"_Stz,             // CN_XF_OT_JOIN_ORDERING
    /* */ "XF_OT_REDUCE_GBYAGG_KEY"_Stz,         // CN_XF_OT_REDUCE_GBYAGG_KEY
    /* */ "XF_OT_PREPLACE_GBYAGG_BY_ORDAGG"_Stz, // CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG
    /* */ "XF_OT_REPLACE_SCAN_BY_INDEXSCAN"_Stz, // CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN
    /* */ "XF_FLATTEN_PREDICATE_PULL_UP"_Stz,         // CN_XF_PREDICATE_PULL_UP
    /* */ "XF_FLATTEN_PREDICATE_REPOSITION"_Stz,      // CN_XF_PREDICATE_REPOSITION
    /* */ "XF_PROLOG_PROJECTION_ANALYSIS"_Stz,       // CN_XF_PROJECTION_ANALYSIS
    /* */ "XF_EPILOG_REINSTATE_STAGE"_Stz,           // CN_XF_REINSTATE_STAGE
    /* */ "XF_PROLOG_RPNCALC_TO_BOOLCALC"_Stz,       // CN_XF_RPNCALC_TO_BOOLCALC
    /* */ "XF_SCAN_TO_FROM"_Stz,              // CN_XF_SCAN_TO_FROM
    /* */ "XF_SHORTEN_QUERYNNM"_Stz,          // CN_XF_PROLOG_BOARDNM_TO_SQYNM
    /* */ "XF_FLATTEN_SMOOSH"_Stz,                    // CN_XF_SMOOSH
    /* */ "XF_FLATTEN_SMOOTH"_Stz,                    // CN_XF_SMOOTH
    /* */ "XF_EPILOG_STAGE_CAST"_Stz,                // CN_XF_STAGE_CAST
    /* */ "XF_EPILOG_STMCOL_TO_COLNM"_Stz,           // CN_XF_STMCOL_TO_COLNM
    /* */ "XF_EPILOG_STMCOL_TO_TBLCOL"_Stz,          // CN_XF_STMCOL_TO_TBLCOL
    /* */ "XF_PROLOG_SUBQUERY_ANALYSIS"_Stz,         // CN_XF_SUBQUERY_ANALYSIS
    /* */ "XF_SYNTHESISE_CODE"_Stz,           // CN_XF_SYNTHESISE_CODE
    /* */ "XF_SYNTHESISE_PIPELINEDEF"_Stz,    // CN_XF_SYNTHESISE_PIPELINEDEF
    /* */ "XF_PROLOG_TABLE_ANALYSIS"_Stz,            // CN_XF_TABLE_ANALYSIS
    /* */ "XF_EPILOG_TBLCOL_TO_STMCOL"_Stz,          // CN_XF_TBLCOL_TO_STMCOL
    /* */ "XF_FLATTEN_UPDATE_PROJECTION"_Stz,         // CN_XF_UPDATE_PROJECTION

    /* */ "_Y__"_Stz,                 // CN__Y__

    /* */ "_Z__"_Stz,                 // CN__Z__

//    /* */ "ENUMS"_Stz,                // CN_ENUMS
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
