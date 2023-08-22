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
  /* */ CN_ACTION,
  /* */ CN_ADAPTORWL,
  /* */ CN_AGGTIMER,
  /* */ CN_AM,
  /* */ CN_ANSWERWL,
  /* */ CN_API,
  /* */ CN_ARG,
  /* */ CN_ARGDEF,
  /* */ CN_ARRANGE,
  /* */ CN_ATOMIC,
  /* */ CN_ATTACHMAP,

  /* */ CN__B__,
  /* */ CN_BACKEND,
  /* */ CN_BALLOON,
  /* */ CN_BCDVAL,
  /* */ CN_BENCH,
  /* */ CN_BIRTH,
  /* */ CN_BOARD,
  /* */ CN_BOLVAL,
  /* */ CN_BUFFERMAP,
  /* */ CN_BUFFERPOOL,
  /* */ CN_BYTERATE,
  /* */ CN_BYTES_T,

  /* */ CN__C__,
  /* */ CN_CARD,
  /* */ CN_CARDCTL,
  /* */ CN_CARDHISTORY,
  /* */ CN_CARDIMAGE,
  /* */ CN_CARDITER,
  /* */ CN_CARDITERMEMBERS,
  /* */ CN_CARDUSAGE,
  /* */ CN_CARRYFORWARD,
  /* */ CN_CATALOG,
  /* */ CN_CHARSER,
  /* */ CN_CHILDEXIT,
  /* */ CN_CLASSPLACEHOLDER,
  /* */ CN_CMDLINEARGS,
  /* */ CN_CODEBLOCK,
  /* */ CN_COLMAP,
  /* */ CN_COLSREQ,
  /* */ CN_COLUMN,
  /* */ CN_COMBINED,
  /* */ CN_COMBINEDSRC,
  /* */ CN_CONDITIONALQUERYNODEITER,
  /* */ CN_CORRELATEDJOIN,
  /* */ CN_CRVBUFFER,
  /* */ CN_CRWBUFFER,
  /* */ CN_CRWTABLEPTN,
  /* */ CN_CRWTOROW,
  /* */ CN_CSVBUFFER,
  /* */ CN_CSVMETA,

  /* */ CN__D__,
  /* */ CN_DATATRANSIENT,
  /* */ CN_DDLLOCK,
  /* */ CN_DECACNT,
  /* */ CN_DECIDER,
  /* */ CN_DECK,
  /* */ CN_DECKSPEC,
  /* */ CN_DEFINITIONNODE,
  /* */ CN_DEREFMETRICS,
  /* */ CN_DIC,
  /* */ CN_DICLOCK,
  /* */ CN_DICSTACK,
  /* */ CN_DIR,
  /* */ CN_DISSECTOR,
  /* */ CN_DTEVAL,
  /* */ CN_DYNAMIC,
  /* */ CN_DYNAMO,

  /* */ CN__E__,
  /* */ CN_EDTVAL,
  /* */ CN_ENCODING,
  /* */ CN_ENT,
  /* */ CN_ENTBUFFER,
  /* */ CN_ENTBUFSORTER,
  /* */ CN_ENTWRK,
  /* */ CN_ENUMFLAGS,
  /* */ CN_EQUIVALENCE,
  /* */ CN_EVENTHANDLERS,
  /* */ CN_EVMGR,
  /* */ CN_EXEMPLAR,
  /* */ CN_EXITOPERAND,
  /* */ CN_EXTENDEDTEXT,
  /* */ CN_EXTENDEDTEXTBUFFER,
  /* */ CN_EXTRACTMAP,

  /* */ CN__F__,
  /* */ CN_FIFO,
  /* */ CN_FILE,
  /* */ CN_FILEMAP,
  /* */ CN_FIPVAL,
  /* */ CN_FLTVAL,
  /* */ CN_FOLD,
  /* */ CN_FORMAT,
  /* */ CN_FREEPOOLBUFFER,
  /* */ CN_FREECHUNK,
  /* */ CN_FREEDNOT,
  /* */ CN_FREEITEM,
  /* */ CN_FREQDIST,

  /* */ CN__G__,
  /* */ CN_GRID,

  /* */ CN__H__,
  /* */ CN_HASHMAP,
  /* */ CN_HASHMAPENTRY,
  /* */ CN_HASHMAPSLOT,
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
  /* */ CN_JOBMETRICS,
  /* */ CN_JOBREPORT,
  /* */ CN_JOBS,

  /* */ CN__K__,
  /* */ CN_KEY,
  /* */ CN_KID,

  /* */ CN__L__,
  /* */ CN_LARGEPERSISTENT,
  /* */ CN_LARGETRANSIENT,
  /* */ CN_LARGEVIRTUAL,
  /* */ CN_LGLBUFSET,
  /* */ CN_LGLPTNMAP,
  /* */ CN_LIBINFO,
  /* */ CN_LICENCE,
  /* */ CN_LIMITS,
  /* */ CN_LISTEXONODE_t,
  /* */ CN_LISTEXOITER_t,
  /* */ CN_LIT,
  /* */ CN_LITRECOGNISER,
  /* */ CN_LOADCHUNK,
  /* */ CN_LOCALCHAIN,
  /* */ CN_LOCALDIR,
  /* */ CN_LOCALFILE,
  /* */ CN_LOCALSTRS,
  /* */ CN_LOCALVARS,
  /* */ CN_LOCATION,
  /* */ CN_LOCATOR,
  /* */ CN_LOCK,
  /* */ CN_LOCKS,
  /* */ CN_LOCKSET,
  /* */ CN_LOGICUNIT,

  /* */ CN__M__,
  /* */ CN_MARK,
  /* */ CN_MARKER,
  /* */ CN_MARKERCHAIN,
  /* */ CN_MARKERMGR,
  /* */ CN_MEDIUMPERSISTENT,
  /* */ CN_MEDIUMTRANSIENT,
  /* */ CN_MEDIUMVIRTUAL,
  /* */ CN_MEMTEST,
  /* */ CN_METADATA,
  /* */ CN_METAGET,
  /* */ CN_METRICSWSP,
  /* */ CN_MUTEX,

  /* */ CN__N__,
  /* */ CN_NAMEDBUFFER,
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
  /* */ CN_OBJTRACKER,
  /* */ CN_OUTPUT,
  /* */ CN_OP,
  /* */ CN_OPDEF,
  /* */ CN_OPENFILES,
  /* */ CN_OPERANDQUEUE,
  /* */ CN_OPERANDWATCH,
  /* */ CN_OPSKETCH,
  /* */ CN_OPMETRICS,
  /* */ CN_OPSPEC,
  /* */ CN_OPTIMISER,
  /* */ CN_OTHEROPERAND,

  /* */ CN__P__,
  /* */ CN_PAGEFILE,
  /* */ CN_PAGEINBUFFER,
  /* */ CN_PAGINGMETRICS,
  /* */ CN_PARAMSPEC,
  /* */ CN_PARAMSSPEC,
  /* */ CN_PARCEL,
  /* */ CN_PATHIDENTIFIER,
  /* */ CN_PATHVEC,
  /* */ CN_PAYSTR,
  /* */ CN_PAYSTRBUFFER,
  /* */ CN_PDS,
  /* */ CN_PERMGEN,
  /* */ CN_PERMIT,
  /* */ CN_PERUSE,
  /* */ CN_PIPEIN,
  /* */ CN_PIPELINE,
  /* */ CN_PIPELINEDEF,
  /* */ CN_PIPELINEMETRICS,
  /* */ CN_PLANDEFINITIONPARSER,
  /* */ CN_PLANNODE,
  /* */ CN_PLANNODEBUILDER,
  /* */ CN_PLANNODEBUILDERLIST,
  /* */ CN_PLANNODEBUILDERSTACK,
  /* */ CN_PLANNODELIST,
  /* */ CN_PLANOPERAND,
  /* */ CN_POT,
  /* */ CN_PORT,
  /* */ CN_PPY,
  /* */ CN_PREDICATE,
  /* */ CN_PREDICATEVEC,
  /* */ CN_PRIMITIVENODE,
  /* */ CN_PROCEDURE,
  /* */ CN_PROFILE,
  /* */ CN_PSXTHD,
  /* */ CN_PTNDEF,
  /* */ CN_PTNID,
  /* */ CN_PTNSTMCURSOR,
  /* */ CN_PTNSTREAM,
  /* */ CN_PTRVAL,

  /* */ CN__Q__,
  /* */ CN_QUERYNODE,
  /* */ CN_QUERYNODELIST,
  /* */ CN_QUERYNODESTACK,
  /* */ CN_QUEUE,
  /* */ CN_QUEUEBUFFER,

  /* */ CN__R__,
  /* */ CN_READER,
  /* */ CN_REAPLOCK,
  /* */ CN_RECORD,
  /* */ CN_RECVWL,
  /* */ CN_REJECTJOB,
  /* */ CN_RELOP,
  /* */ CN_RESOURCES,
  /* */ CN_RIBBON,
  /* */ CN_RIBBONLINE,
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
  /* */ CN_SAWOPCARDS,
  /* */ CN_SCANNER,
  /* */ CN_SCHEDULE,
  /* */ CN_SCHEDULER,
  /* */ CN_SCHEMA,
  /* */ CN_SCOPE,
  /* */ CN_SEAPI,
  /* */ CN_SEDEPUTY,
  /* */ CN_SEEME,
  /* */ CN_SENCELL,
  /* */ CN_SENCOMPILER,
  /* */ CN_SENDS,
  /* */ CN_SENDWL,
  /* */ CN_SEPROMPT,
  /* */ CN_SHORTCUT,
  /* */ CN_SHORTCUTVEC,
  /* */ CN_SINCE,
  /* */ CN_SKETCHTKN,
  /* */ CN_SKIPLIST,
  /* */ CN_SKIPLISTNODE,
  /* */ CN_SKIPLISTBLOCK,
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
  /* */ CN_STAGEDEF,
  /* */ CN_STAGEMETRICS,
  /* */ CN_STAGESPEC,
  /* */ CN_STARTER,
  /* */ CN_STATEMENT,
  /* */ CN_STATS,
  /* */ CN_STENCIL_V2,
  /* */ CN_STEP,
  /* */ CN_STEPACCUM,
  /* */ CN_STEPS,
  /* */ CN_STMTLIST,
  /* */ CN_STREAM,
  /* */ CN_STREAMDEF,
  /* */ CN_STREAMDEFRHT,
  /* */ CN_STREAMMETRICS,
  /* */ CN_STREAMSET,
  /* */ CN_STRING_T,
  /* */ CN_STZLIST,
  /* */ CN_STZVAL,
  /* */ CN_SUBCALL,
  /* */ CN_SUBQUERY,
  /* */ CN_SUBRETURN,
  /* */ CN_SYMBOL,
  /* */ CN_SYSCALL,
  /* */ CN_SYSMETRICS,
  /* */ CN_SYSREPORT,

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
  /* */ CN_TEXT,
  /* */ CN_TEXTLARGE,
  /* */ CN_TEXTMEDIUM,
  /* */ CN_TEXTSMALL,
  /* */ CN_TEXTSTACK,
  /* */ CN_TEXTTINY,
  /* */ CN_THING,
  /* */ CN_THREAD,
  /* */ CN_THREADQUEUE,
  /* */ CN_THREADSCRATCH,
  /* */ CN_THREADVEC,
  /* */ CN_TIMER,
  /* */ CN_TINYPERSISTENT,
  /* */ CN_TINYTRANSIENT,
  /* */ CN_TINYVIRTUAL,
  /* */ CN_TMEVAL,
  /* */ CN_TOKEN,
  /* */ CN_TOKENVEC,
  /* */ CN_TOKEYHSHROW,
  /* */ CN_TOLGLPTNCRW,
  /* */ CN_TOLGLPTNNST,
  /* */ CN_TOLGLPTNROW,
  /* */ CN_TOTUP,
  /* */ CN_TRACELOCK,
  /* */ CN_TRACEMULTILOCK,
  /* */ CN_TSPVAL,
  /* */ CN_TUB,
  /* */ CN_TX,
  /* */ CN_TXSET,
  /* */ CN_TYPEMAP1D,
  /* */ CN_TYPEMAP2D,

  /* */ CN__U__,
  /* */ CN_UNITINT,
  /* */ CN_UT_SKIPLIST,

  /* */ CN__V__,
  /* */ CN_VERBOSE,
  /* */ CN_VERTEX,
  /* */ CN_VERTEXSPEC,

  /* */ CN__W__,
  /* */ CN_WAKEUP,
  /* */ CN_WEAVER,
  /* */ CN_WILDERNESS,
  /* */ CN_WORD,
  /* */ CN_WSPCONVERGENT_t,

  /* */ CN__X__,
  /* */ CN_XC,
  /* */ CN_XF,
  /* */ CN_XF_ADD_LEVEL,
  /* */ CN_XF_AS_REVIEW,
  /* */ CN_XF_COLNM_TO_STMCOL,
  /* */ CN_XF_COLUMN_MAPPING,
  /* */ CN_XF_CULL_NONUSED_BOUND,
  /* */ CN_XF_DEDUPE_AS,
  /* */ CN_XF_DELETE_EMPTY_WHERE,
  /* */ CN_XF_DELETE_FLAT,
  /* */ CN_XF_DELETE_UNNEEDED,
  /* */ CN_XF_DERIVE_BOUNDS,
  /* */ CN_XF_DERIVE_STREAM_KEY,
  /* */ CN_XF_FIX_TABLE_POS,
  /* */ CN_XF_FLATTEN,
  /* */ CN_XF_FLATTEN_10_F,
  /* */ CN_XF_FLATTEN_24_W_A_C_RO,
  /* */ CN_XF_FLATTEN_32_W_A_NC_I,
  /* */ CN_XF_FLATTEN_34_W_A_NC_RO,
  /* */ CN_XF_FLATTEN_40_W_L_C_E,
  /* */ CN_XF_FLATTEN_52_W_L_NC_I_EXISTS,
  /* */ CN_XF_FLATTEN_52_W_L_NC_I_JOIN,
  /* */ CN_XF_FLATTEN_53_W_L_NC_NI_EXISTS,
  /* */ CN_XF_FLATTEN_53_W_L_NC_NI_JOIN,
  /* */ CN_XF_FLATTEN_74_H_A_NC_RO,
  /* */ CN_XF_FROM_TO_OP,
  /* */ CN_XF_INDEX_ANALYSIS,
  /* */ CN_XF_INDIVIDUATE_FILTER,
  /* */ CN_XF_MOVE_ON,
  /* */ CN_XF_NOT_OPS,
  /* */ CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG,
  /* */ CN_XF_OPTIMISE_DUPLICATE_FILTER,
  /* */ CN_XF_OPTIMISE_EARLY_FILTER,
  /* */ CN_XF_OPTIMISE_LATE_PROJECTION,
  /* */ CN_XF_OPTIMISE_MAX_TO_TOP,
  /* */ CN_XF_OPTIMISE_ONETIME_CALC,
  /* */ CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS,
  /* */ CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP,
  /* */ CN_XF_OT_CUT_SORT,
  /* */ CN_XF_OT_EARLY_PROJECTION,
  /* */ CN_XF_OT_JOIN_ORDERING,
  /* */ CN_XF_OT_REDUCE_GBYAGG_KEY,
  /* */ CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG,
  /* */ CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN,
  /* */ CN_XF_PREDICATE_PULL_UP,
  /* */ CN_XF_PREDICATE_REPOSITION,
  /* */ CN_XF_PROJECTION_ANALYSIS,
  /* */ CN_XF_REINSTATE_STAGE,
  /* */ CN_XF_RPNCALC_TO_BOOLCALC,
  /* */ CN_XF_SCAN_TO_FROM,
  /* */ CN_XF_SHORTEN_QUERYNM,
  /* */ CN_XF_SMOOSH,
  /* */ CN_XF_SMOOTH,
  /* */ CN_XF_STAGE_CAST,
  /* */ CN_XF_STMCOL_TO_COLNM,
  /* */ CN_XF_STMCOL_TO_TBLCOL,
  /* */ CN_XF_SUBQUERY_ANALYSIS,
  /* */ CN_XF_SYNTHESISE_CODE,
  /* */ CN_XF_SYNTHESISE_PIPELINEDEF,
  /* */ CN_XF_TABLE_ANALYSIS,
  /* */ CN_XF_TBLCOL_TO_STMCOL,
  /* */ CN_XF_UPDATE_PROJECTION,

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
  /* */ "COLMAP"_Stz,               // CN_COLMAP
  /* */ "COLSREQ"_Stz,              // CN_COLSREQ
  /* */ "COLUMN"_Stz,               // CN_COLUMN
  /* */ "COMBINED"_Stz,             // CN_COMBINED
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
  /* */ "PTNSTMCURSOR"_Stz,         // CN_PTNSTMCURSOR
  /* */ "PTNSTREAM"_Stz,            // CN_PTNSTREAM
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
  /* */ "XF_ADD_LEVEL"_Stz,                 // CN_XF_ADD_LEVEL
  /* */ "XF_AS_REVIEW"_Stz,                 // CN_XF_AS_REVIEW
  /* */ "XF_COLNM_TO_STMCOL"_Stz,           // CN_XF_COLNM_TO_STMCOL
  /* */ "XF_COLUMN_MAPPING"_Stz,            // CN_XF_COLUMN_MAPPING
  /* */ "XF_CULL_NONUSED_BOUND"_Stz,        // CN_XF_CULL_NONUSED_BOUND
  /* */ "XF_DEDUPE_AS"_Stz,                 // CN_XF_DEDUPE_AS
  /* */ "XF_DELETE_EMPTY_WHERE"_Stz,        // CN_XF_DELETE_EMPTY_WHERE
  /* */ "XF_DELETE_FLAT"_Stz,               // CN_XF_DELETE_FLAT
  /* */ "XF_DELETE_UNNEEDED"_Stz,           // CN_XF_DELETE_UNNEEDED
  /* */ "XF_DERIVE_BOUNDS"_Stz,             // CN_XF_DERIVE_BOUNDS
  /* */ "XF_DERIVE_STREAM_KEY"_Stz,         // CN_XF_DERIVE_STREAM_KEY
  /* */ "XF_FIX_TABLE_POS"_Stz,             // CN_XF_FIX_TABLE_POS
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
  /* */ "XF_FROM_TO_OP"_Stz,                // CN_XF_FROM_TO_OP
  /* */ "XF_INDEX_ANALYSIS"_Stz,            // CN_XF_INDEX_ANALYSIS
  /* */ "XF_INDIVIDUATE_FILTER"_Stz,        // CN_XF_INDIVIDUATE_FILTER
  /* */ "XF_MOVE_ON"_Stz,                   // CN_XF_MOVE_ON
  /* */ "XF_NOT_OPS"_Stz,                   // CN_XF_NOT_OPS
  /* */ "XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG"_Stz, // CN_XF_OPTIMISE_AGGREG_TO_SORT_ORDAGG
  /* */ "XF_OPTIMISE_EARLY_FILTER"_Stz,     // CN_XF_OPTIMISE_EARLY_FILTER
  /* */ "XF_OPTIMISE_LATE_PROJECTION"_Stz,  // CN_XF_OPTIMISE_LATE_PROJECTION
  /* */ "XF_OPTIMISE_MAX_TO_TOP"_Stz,       // CN_XF_OPTIMISE_MAX_TO_TOP
  /* */ "XF_OPTIMISE_ONETIME_CALC"_Stz,     // CN_XF_OPTIMISE_ONETIME_CALC
  /* */ "XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS"_Stz, // CN_XF_OPTIMISE_SORT_LIMIT_ALL_LEVELS
  /* */ "XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP"_Stz, // CN_XF_OPTIMISE_SORTDESC_LIMIT_TO_TOP
  /* */ "XF_OT_CUT_SORT"_Stz,                  // CN_XF_OT_CUT_SORT
  /* */ "XF_OT_DUPLICATE_FILTER"_Stz,          // CN_XF_OT_DUPLICATE_FILTER
  /* */ "XF_OT_EARLY_PROJECTION"_Stz,          // CN_XF_OT_EARLY_PROJECTION
  /* */ "XF_OT_JOIN_ORDERING"_Stz,             // CN_XF_OT_JOIN_ORDERING
  /* */ "XF_OT_REDUCE_GBYAGG_KEY"_Stz,         // CN_XF_OT_REDUCE_GBYAGG_KEY
  /* */ "XF_OT_PREPLACE_GBYAGG_BY_ORDAGG"_Stz, // CN_XF_OT_REPLACE_GBYAGG_BY_ORDAGG
  /* */ "XF_OT_REPLACE_SCAN_BY_INDEXSCAN"_Stz, // CN_XF_OT_REPLACE_SCAN_BY_INDEXSCAN
  /* */ "XF_PREDICATE_PULL_UP"_Stz,         // CN_XF_PREDICATE_PULL_UP
  /* */ "XF_PREDICATE_REPOSITION"_Stz,      // CN_XF_PREDICATE_REPOSITION
  /* */ "XF_PROJECTION_ANALYSIS"_Stz,       // CN_XF_PROJECTION_ANALYSIS
  /* */ "XF_REINSTATE_STAGE"_Stz,           // CN_XF_REINSTATE_STAGE
  /* */ "XF_RPNCALC_TO_BOOLCALC"_Stz,       // CN_XF_RPNCALC_TO_BOOLCALC
  /* */ "XF_SCAN_TO_FROM"_Stz,              // CN_XF_SCAN_TO_FROM
  /* */ "XF_SHORTEN_QUERYNNM"_Stz,          // CN_XF_SHORTEN_QUERYNM
  /* */ "XF_SMOOSH"_Stz,                    // CN_XF_SMOOSH
  /* */ "XF_SMOOTH"_Stz,                    // CN_XF_SMOOTH
  /* */ "XF_STAGE_CAST"_Stz,                // CN_XF_STAGE_CAST
  /* */ "XF_STMCOL_TO_COLNM"_Stz,           // CN_XF_STMCOL_TO_COLNM
  /* */ "XF_STMCOL_TO_TBLCOL"_Stz,          // CN_XF_STMCOL_TO_TBLCOL
  /* */ "XF_SUBQUERY_ANALYSIS"_Stz,         // CN_XF_SUBQUERY_ANALYSIS
  /* */ "XF_SYNTHESISE_CODE"_Stz,           // CN_XF_SYNTHESISE_CODE
  /* */ "XF_SYNTHESISE_PIPELINEDEF"_Stz,    // CN_XF_SYNTHESISE_PIPELINEDEF
  /* */ "XF_TABLE_ANALYSIS"_Stz,            // CN_XF_TABLE_ANALYSIS
  /* */ "XF_TBLCOL_TO_STMCOL"_Stz,          // CN_XF_TBLCOL_TO_STMCOL
  /* */ "XF_UPDATE_PROJECTION"_Stz,         // CN_XF_UPDATE_PROJECTION

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

} // namespace

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
