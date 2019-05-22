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
  /*   0 0x00        0B Heap:I */ CN_NONE,
  /*   1 0x01       16B Heap:I */ CN_BCDVAL,
  /*   2 0x02       16B Heap:I */ CN_BOLVAL,
  /*   3 0x03       16B Heap:I */ CN_DTEVAL,
  /*   4 0x04       16B Heap:I */ CN_FIPVAL,
  /*   5 0x05       16B Heap:I */ CN_FLTVAL,
  /*   6 0x06       16B Heap:I */ CN_INTVAL,
  /*   7 0x07       16B Heap:I */ CN_IVLVAL,
  /*   8 0x08       16B Heap:I */ CN_PTRVAL,
  /*   9 0x09      272B Heap:I */ CN_STRVAL,
  /*  10 0x0A       16B Heap:I */ CN_TMEVAL,
  /*  11 0x0B       16B Heap:I */ CN_TSPVAL,
  
  /*  12 0x0C       80B Heap:M */ CN_ACCESSPATH,
  /*                           */ CN_ACTION,
  /*                           */ CN_ACTIONSEQUENCE,
  /*  13 0x0D      200B Heap:I */ CN_ACTIVITYMAP,
  /*  14 0x0E      200B Heap:I */ CN_ACTIVITYMARK,
  /*  15 0x0F   213152B Heap:L */ CN_APISTATE,
  /*  16 0x10      296B Heap:S */ CN_ARG,
  /*  17 0x11     2808B Heap:I */ CN_ARGUMENTS,
  /*  18 0x12       24B Heap:I */ CN_ATOMIC,
  /*  19 0x13    19672B Heap:I */ CN_ATTACHMAP,

  /*  20 0x14        1B Heap:I */ CN_BPA,
  /*  21 0x15      312B Heap:I */ CN_BUFFERPOOL,
  /*  22 0x16      120B Heap:S */ CN_BYTERATE,
  
  /*  23 0x17       40B Heap:I */ CN_CARRYOVER,
  /*  24 0x18      120B Heap:S */ CN_COLUMN,
  /*  25 0x19     8216B Heap:M */ CN_COLUMNVEC,
  /*  26 0x1A       24B Heap:M */ CN_COMMAND,
  /*  27 0x1B       64B Heap:M */ CN_COMPILER,
  /*                           */ CN_COMPLETER,

  /*                           */ CN_DATUM,
  /*                           */ CN_DECACNT,
  /*  28 0x1C       24B Heap:S */ CN_DECIDER,
  /*  29 0x1D      248B Heap:M */ CN_DIC,
  /*  30 0x1E      792B Heap:S */ CN_DICPATH,
  /*  31 0x1F      152B Heap:S */ CN_DICSTACK,
  /*  32 0x20      608B Heap:S */ CN_DIR,
  
  /*  33 0x21       40B Heap:I */ CN_ENTRY,
  /*  34 0x22  1043848B Heap:L */ CN_ENTRYBUFFER,
  /*                           */ CN_ENUMFLAGS,
  /*                           */ CN_EXPORTBUFFER,

  /*  35 0x23  1040440B Heap:L */ CN_FETCHBUFFER,
  /*  36 0x24      376B Heap:S */ CN_FILE,
  /*  37 0x25       48B Heap:S */ CN_FORMATTERS,
  /*  38 0x26       32B Heap:I */ CN_FREEBUFFER,
  /*  39 0x27       24B Heap:I */ CN_FREECHUNK,
  /*  40 0x28        0B Heap:I */ CN_FREEDNOT,
  /*  41 0x29        0B Heap:I */ CN_FREEENTRY,
  
  /*  42 0x2A    64024B Heap:I */ CN_GENVAR,
  /*  43 0x2B     1224B Heap:L */ CN_GRID,
  
  /*                           */ CN_HASHBUFFER,
  /*                           */ CN_HASHMAP,
  /*  44 0x2C       48B Heap:I */ CN_HEAPBUFFER,
  
  /*  45 0x2D      368B Heap:I */ CN_INDEX,
  /*  46 0x2E      184B Heap:S */ CN_INDEXTREE,
  /*  47 0x2F     8224B Heap:M */ CN_INDEXTREEVEC,
  /*  48 0x30     8216B Heap:M */ CN_IDXCOLVEC,
  /*  49 0x31     8224B Heap:M */ CN_IDXVEC,
  /*  50 0x32  1000096B Heap:L */ CN_INGESTCHUNK,
  /*  51 0x33  1000440B Heap:L */ CN_IOCHUNK,
  /*  52 0x34       32B Heap:S */ CN_IOCOPY,
  /*  53 0x35       40B Heap:I */ CN_ITEM,
  /*  54 0x36  1043632B Heap:I */ CN_ITEMBUFFER,
  
  /*  55 0x37    70600B Heap:L */ CN_JOB__BEGUN,
  /*  56 0x38    70592B Heap:L */ CN_JOB__CANCEL,
  /*  57 0x39    70600B Heap:L */ CN_JOB__CANCELLED,
  /*  58 0x3A    70592B Heap:L */ CN_JOB__END,
  /*  59 0x3B    70592B Heap:L */ CN_JOB__ENDED,
  /*  60 0x3C    70592B Heap:L */ CN_JOB__EXECUTE,
  /*  61 0x3D    70896B Heap:L */ CN_JOB__EXECUTED,
  /*  62 0x3E    70592B Heap:L */ CN_JOB__FIND,
  /*  63 0x3F    70592B Heap:L */ CN_JOB_ALARM,
  /*  64 0x40    70600B Heap:L */ CN_JOB_ANALYZEINDEX,
  /*  65 0x41    70600B Heap:L */ CN_JOB_ANALYZETABLE,
  /*  66 0x42    70592B Heap:L */ CN_JOB_ANSWER,
  /*  67 0x43    70592B Heap:L */ CN_JOB_BOOT,
  /*  68 0x44    71160B Heap:L */ CN_JOB_CAT,
  /*  69 0x45    70592B Heap:L */ CN_JOB_CF,
  /*  70 0x46    70592B Heap:L */ CN_JOB_COMPACTINDEX,
  /*  71 0x47    70592B Heap:L */ CN_JOB_CONNECTCLUSTER,
  /*  72 0x48    70592B Heap:L */ CN_JOB_CONNECTOR,
  /*  73 0x49   118688B Heap:L */ CN_JOB_COPYBUFFER,
  /*  74 0x4A    70592B Heap:L */ CN_JOB_CREATECATALOG,
  /*  75 0x4B    77808B Heap:L */ CN_JOB_CREATEINDEX,
  /*  76 0x4C    70592B Heap:L */ CN_JOB_CREATESCHEMA,
  /*  77 0x4D    83808B Heap:L */ CN_JOB_CREATETABLE,
  /*  78 0x4E    70592B Heap:L */ CN_JOB_CREATETENANT,
  /*  79 0x4F    70592B Heap:L */ CN_JOB_DOCOL,
  /*  80 0x50    70592B Heap:L */ CN_JOB_DROPCATALOG,
  /*  81 0x51    70592B Heap:L */ CN_JOB_DROPINDEX,
  /*  82 0x52    70592B Heap:L */ CN_JOB_DROPSCHEMA,
  /*  83 0x53    70592B Heap:L */ CN_JOB_DROPTABLE,
  /*  84 0x54    70592B Heap:L */ CN_JOB_DROPTENANT,
  /*  85 0x55    70592B Heap:L */ CN_JOB_DUMPINDEX,
  /*                           */ CN_JOB_DUMPTABLE,
  /*  86 0x56    70864B Heap:L */ CN_JOB_EXPORT,
  /*  87 0x57    70936B Heap:L */ CN_JOB_FETCHROWS,
  /*  88 0x58   122912B Heap:L */ CN_JOB_GENERATE,
  /*                           */ CN_JOB_HEALTH,
  /*  89 0x59    71456B Heap:L */ CN_JOB_INGEST,
  /*  90 0x5A   118632B Heap:L */ CN_JOB_INSERTROW,
  /*  91 0x5B    70608B Heap:L */ CN_JOB_INSERTTABLE,
  /* 139 0x8B        0B Heap:L */ CN_JOB_KIDEXPORT,
  /*  92 0x5C    70592B Heap:L */ CN_JOB_LF,
  /*  93 0x5D    70592B Heap:L */ CN_JOB_LISTCATALOGS,
  /*  94 0x5E    70592B Heap:L */ CN_JOB_LISTCOLUMNS,
  /*  95 0x5F    70592B Heap:L */ CN_JOB_LISTIDXCOLUMNS,
  /*  96 0x60    70592B Heap:L */ CN_JOB_LISTINDEXES,
  /*  97 0x61    70592B Heap:L */ CN_JOB_LISTPARTCOLUMNS,
  /*  98 0x62    70592B Heap:L */ CN_JOB_LISTPARTITIONS,
  /*  99 0x63    70592B Heap:L */ CN_JOB_LISTSCHEMAS,
  /* 100 0x64    70592B Heap:L */ CN_JOB_LISTTABLES,
  /* 101 0x65    70592B Heap:L */ CN_JOB_LISTTENANTS,
  /* 102 0x66    70928B Heap:L */ CN_JOB_LOADTABLE,
  /* 103 0x67    70888B Heap:L */ CN_JOB_MONITOR,
  /* 104 0x68    70600B Heap:L */ CN_JOB_NETHOG,
  /* 105 0x69    70640B Heap:L */ CN_JOB_NODEINFO,
  /* 106 0x6A    70592B Heap:L */ CN_JOB_PERIODIC,
  /* 107 0x6B    70592B Heap:L */ CN_JOB_PING,
  /* 108 0x6C    71000B Heap:L */ CN_JOB_PUBLISH,
  /* 109 0x6D    70880B Heap:L */ CN_JOB_PURGE,
  /* 110 0x6E    70592B Heap:L */ CN_JOB_PWF,
  /* 111 0x6F    70608B Heap:L */ CN_JOB_RECEIVER,
  /* 112 0x70    71168B Heap:L */ CN_JOB_REPEATER,
  /* 113 0x71    71168B Heap:L */ CN_JOB_REPORT,
  /* 114 0x72    70608B Heap:L */ CN_JOB_SELECTTABLE,
  /* 115 0x73    70608B Heap:L */ CN_JOB_SANITY,
  /* 116 0x74    70592B Heap:L */ CN_JOB_SHOWCLUSTER,
  /* 117 0x75    70880B Heap:L */ CN_JOB_SINK,
  /* 118 0x76    70592B Heap:L */ CN_JOB_SIZEINDEX,
  /* 119 0x77    70592B Heap:L */ CN_JOB_SIZETABLE,
  /* 120 0x78    70592B Heap:L */ CN_JOB_SPACE,
  /* 121 0x79    70592B Heap:L */ CN_JOB_START,
  /* 122 0x7A    70592B Heap:L */ CN_JOB_STARTCLUSTER,
  /* 123 0x7B    70592B Heap:L */ CN_JOB_STATUS,
  /* 124 0x7C    70632B Heap:L */ CN_JOB_SUBEXPORTER,
  /* 125 0x7D    70632B Heap:L */ CN_JOB_SUBFETCH,
  /* 126 0x7E   151464B Heap:L */ CN_JOB_SUBINGESTER,
  /* 127 0x7F   151648B Heap:L */ CN_JOB_SUBINSERTER,
  /* 128 0x80   151472B Heap:L */ CN_JOB_SUBLOADER,
  /* 129 0x81    71016B Heap:L */ CN_JOB_SUBPUBLISH,
  /* 130 0x82    70632B Heap:L */ CN_JOB_SUBPURGER,
  /* 131 0x83    70608B Heap:L */ CN_JOB_SUBTRUNCATE,
  /*                           */ CN_JOB_SUBWALKER,
  /*                           */ CN_JOB_SUPERVISOR,
  /* 132 0x84    70592B Heap:L */ CN_JOB_TABLESIZES,
  /* 133 0x85    70592B Heap:L */ CN_JOB_TRUNCATETABLE,
  /* 134 0x86    70592B Heap:L */ CN_JOB_USESCHEMA,
  /* 135 0x87    70592B Heap:L */ CN_JOB_VERIFYINDEX,
  /*                           */ CN_JOB_VERIFYTABLE,
  /* 136 0x88    70592B Heap:L */ CN_JOB_VISUALIZEINDEX,
  /*                           */ CN_JOB_VISUALIZETABLE,
  /* 137 0x89    70592B Heap:L */ CN_JOB_WAITCLUSTER,
  /*                           */ CN_JOB_WALK,
  /* 138 0x8A    70648B Heap:L */ CN_JOB_WRITEBUFFER,
  /* 140 0x8C   118688B Heap:L */ CN_JOB_WRITEROW,
  /* 141 0x8D      168B Heap:S */ CN_JOBLIST,
  /* 142 0x8E      232B Heap:I */ CN_JUMBOPERSISTENT,
  /* 143 0x8F      232B Heap:I */ CN_JUMBOTRANSIENT,
  
  /* 144 0x90        1B Heap:I */ CN_KEY,
  /* 145 0x91    17448B Heap:I */ CN_KEYWRK,
  
  /* 146 0x92      232B Heap:I */ CN_LARGEPERSISTENT,
  /* 147 0x93      232B Heap:I */ CN_LARGETRANSIENT,
  /* 148 0x94  1000440B Heap:L */ CN_LOADCHUNK,
  /* 149 0x95  9389216B Heap:J */ CN_LOADMAP,
  /* 150 0x96     8216B Heap:I */ CN_LOCAL,
  /* 151 0x97   262944B Heap:L */ CN_LOCALSET,
  /* 152 0x98      128B Heap:I */ CN_LOCK,
  
  /* 153 0x99      232B Heap:I */ CN_MEDIUMPERSISTENT,
  /* 154 0x9A      232B Heap:I */ CN_MEDIUMTRANSIENT,
  /*                           */ CN_MEMTEST,
  /* 155 0x9B      160B Heap:S */ CN_MUTEX,
  
  /*                           */ CN_NMVAL,
  /* 156 0x9C      168B Heap:S */ CN_NODE,
  /* 157 0x9D       88B Heap:S */ CN_NOW,
  /*                           */ CN_NUMBAT,

  /* 158 0x9E       24B Heap:S */ CN_OP,
  
  /* 159 0x9F    48752B Heap:M */ CN_PARSER,
  /* 160 0xA0     1048B Heap:M */ CN_PATHVEC,
  /*                           */ CN_PERUSE,
  /* 161 0xA1      320B Heap:S */ CN_PIPE,
  /* 162 0xA2    48376B Heap:M */ CN_PLAN,
  /* 163 0xA3     3112B Heap:M */ CN_PLANNER,
  /* 164 0xA4       48B Heap:S */ CN_PREDICATE,
  /* 165 0xA5      184B Heap:M */ CN_PREDICATEVEC,
  /* 166 0xA6       32B Heap:S */ CN_PTHREAD,
  /* 167 0xA7     8216B Heap:M */ CN_PTNCOLVEC,
  
  /* 168 0xA8      168B Heap:S */ CN_QUEUE,
  /* 169 0xA9       64B Heap:I */ CN_QUEUEBUFFER,
  /* 170 0xAA     1232B Heap:S */ CN_QUEUES,
  
  /*                           */ CN_RECORD,
  /* 171 0xAB  1043496B Heap:L */ CN_RECORDBUFFER,
  /* 172 0xAC      184B Heap:S */ CN_RESOURCETRACKER,
  /*                           */ CN_RESPONSE,
  /*                           */ CN_RESPONSEVEC,
  /* 173 0xAD      896B Heap:M */ CN_ROWAGG,
  /*                    Heap:L */ CN_ROWBUFFER,
  /*                           */ CN_ROWCODER,
  /* 174 0xAE      152B Heap:S */ CN_ROWONCE,
  /*                           */ CN_ROWTRAY,
  
  /* 176 0xB0    50208B Heap:L */ CN_SCHEMATEST,
  /*                           */ CN_SEEME,
  /* 177 0xB1       88B Heap:L */ CN_SEPROMPT,
  /* 178 0xB2    48248B Heap:M */ CN_SESSION,
  /* 179 0xB3      208B Heap:S */ CN_SESSIONSTATE,
  /* 180 0xB4       80B Heap:I */ CN_SINCE,
  /* 181 0xB5       64B Heap:S */ CN_SKIPLIST,
  /* 182 0xB6       32B Heap:S */ CN_SKIPNODE,
  /* 183 0xB7  1040416B Heap:L */ CN_SLAB,
  /* 184 0xB8       24B Heap:S */ CN_SLABALLOCATOR,
  /* 185 0xB9      288B Heap:I */ CN_SLOTS,
  /* 186 0xBA      232B Heap:I */ CN_SMALLPERSISTENT,
  /* 187 0xBB      232B Heap:I */ CN_SMALLTRANSIENT,
  /* 188 0xBC      176B Heap:S */ CN_SOCKET,
  /* 189 0xBD  1043520B Heap:L */ CN_STACK,
  /* 190 0xBE    12824B Heap:I */ CN_STATISTICS,
  /* 191 0xBF       64B Heap:S */ CN_STEPACCUM,
  /* 192 0xC0   131224B Heap:L */ CN_STEPTIMING,
  /* 193 0xC1       48B Heap:I */ CN_STRING_T,
  /* 194 0xC2       32B Heap:I */ CN_SYSCALL,
  
  /* 195 0xC3     3736B Heap:I */ CN_TABLE,
  /* 196 0xC4      208B Heap:S */ CN_TABLEPTN,
  /* 197 0xC5     1048B Heap:M */ CN_TABLEPTNVEC,
  /* 198 0xC6     1360B Heap:M */ CN_TASK,
  /* 199 0xC7      184B Heap:S */ CN_TASKLIST,
  /* 200 0xC8      160B Heap:S */ CN_TASKQUEUE,
  /*                           */ CN_TEMPNAME,
  /* 201 0xC9       96B Heap:S */ CN_THREAD,
  /* 202 0xCA     2208B Heap:M */ CN_THREADVEC,
  /* 203 0xCB       24B Heap:I */ CN_TIME,
  /* 204 0xCC       32B Heap:S */ CN_TIMER,
  /* 205 0xCD      296B Heap:S */ CN_TOKEN,
  /* 206 0xCE    56832B Heap:M */ CN_TOKENISER,
  /* 207 0xCF     8216B Heap:I */ CN_TOKENARY,
  /* 208 0xD0     8232B Heap:M */ CN_TOKENVEC,
  /* 209 0xD1      208B Heap:S */ CN_TX,
  /* 210 0xD2      176B Heap:S */ CN_TXSET,
  
  /* 211 0xD3       40B Heap:I */ CN_WILDERNESS,
  /* 212 0xD4    64672B Heap:M */ CN_WORD,
  
  /* 213 0xD5     3840B Heap:I */ CN_ZSMALL_,
  /* 214 0xD6    65280B Heap:I */ CN_ZMEDIUM_,
  /* 215 0xD7  1048320B Heap:I */ CN_ZLARGE_,
  /* 216 0xD8 16776960B Heap:I */ CN_ZJUMBO_,
  
  /* 217 0xD9       -1B Heap:I */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /*   0 0x00 */ "NONE"_Stz,                // CN_NONE
  /*   1 0x01 */ "BCDVAL"_Stz,              // CN_BCDVAL
  /*   2 0x02 */ "BOLVAL"_Stz,              // CN_BOLVAL
  /*   3 0x03 */ "DTEVAL"_Stz,              // CN_DTEVAL
  /*   4 0x04 */ "FIPVAL"_Stz,              // CN_FIPVAL
  /*   5 0x05 */ "FLTVAL"_Stz,              // CN_FLTVAL
  /*   6 0x06 */ "INTVAL"_Stz,              // CN_INTVAL
  /*   7 0x07 */ "IVLVAL"_Stz,              // CN_IVLVAL
  /*   8 0x08 */ "PTRVAL"_Stz,              // CN_PTRVAL
  /*   9 0x09 */ "STRVAL"_Stz,              // CN_STRVAL
  /*  10 0x0A */ "TMEVAL"_Stz,              // CN_TMEVAL
  /*  11 0x0B */ "TSPVAL"_Stz,              // CN_TSPVAL
  
  /*  12 0x0C */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
  /*          */ "ACTION"_Stz,              // CN_ACTION
  /*          */ "ACTIONJSEQUENCE"_Stz,     // CN_ACTIONSEQUENCE
  /*  13 0x0D */ "ACTIVITYMAP"_Stz,         // CN_ACTIVITYMAP
  /*  14 0x0E */ "ACTIVITYMARK"_Stz,        // CN_ACTIVITYMARK
  /*  15 0x0F */ "APISTATE"_Stz,            // CN_APISTATE
  /*  16 0x10 */ "ARG"_Stz,                 // CN_ARG
  /*  17 0x11 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /*  18 0x12 */ "ATOMIC"_Stz,              // CN_ATOMIC
  /*  19 0x13 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP

  /*  20 0x14 */ "BPA"_Stz,                 // CN_BPA
  /*  21 0x15 */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /*  22 0x16 */ "BYTERATE"_Stz,            // CN_BYTERATE
  
  /*  23 0x17 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /*  24 0x18 */ "COLUMN"_Stz,              // CN_COLUMN
  /*  25 0x19 */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
  /*  26 0x1A */ "COMMAND"_Stz,             // CN_COMMAND
  /*  27 0x1B */ "COMPILER"_Stz,            // CN_COMPILER
  /*          */ "COMPLETER"_Stz,           // CN_COMPLETER

  /*          */ "DATUM"_Stz,               // CN_DATUM
  /*          */ "DECACNT"_Stz,             // CN_DECACNT
  /*  28 0x1C */ "DECIDER"_Stz,             // CN_DECIDER
  /*  29 0x1D */ "DIC"_Stz,                 // CN_DIC
  /*  30 0x1E */ "DICPATH"_Stz,             // CN_DICPATH
  /*  31 0x1F */ "DICSTACK"_Stz,            // CN_DICSTACK
  /*  32 0x20 */ "DIR"_Stz,                 // CN_DIR
  
  /*  33 0x21 */ "ENTRY"_Stz,               // CN_ENTRY
  /*  34 0x22 */ "ENTRYBUFFER"_Stz,         // CN_ENTRYBUFFER
  /*          */ "ENUMFLAGS"_Stz,           // CN_ENUMFLAGS
  /*          */ "EXPORTBUFFER"_Stz,        // CN_EXPORTBUFFER

  /*  35 0x23 */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /*  36 0x24 */ "FILE"_Stz,                // CN_FILE
  /*  37 0x25 */ "FORMATTERS"_Stz,          // CN_FORMATTERS
  /*  38 0x26 */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /*  39 0x27 */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /*  40 0x28 */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /*  41 0x29 */ "FREEENTRY"_Stz,           // CN_FREEENTRY
  
  /*  42 0x2A */ "GENVAR"_Stz,              // CN_GENVAR
  /*  43 0x2B */ "GRID"_Stz,                // CN_GRID
  
  /*          */ "HASHBUFFER"_Stz,          // CN_HASHBUFFER
  /*          */ "HASHMAP"_Stz,             // CN_HASHMAP
  /*  44 0x2C */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  
  /*  45 0x2D */ "INDEX"_Stz,               // CN_INDEX
  /*  46 0x2E */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /*  47 0x2F */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /*  48 0x30 */ "IDXCOLVEC"_Stz,           // CN_IDXCOLVEC
  /*  49 0x31 */ "IDXVEC"_Stz,              // CN_IDXVEC
  /*  50 0x32 */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /*  51 0x33 */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /*  52 0x34 */ "IOCOPY"_Stz,              // CN_IOCOPY
  /*  53 0x35 */ "ITEM"_Stz,                // CN_ITEM
  /*  54 0x36 */ "ITEMBUFFER"_Stz,          // CN_ITEMBUFFER
  
  /*  55 0x37 */ "JOB__BEGUN"_Stz,          // CN_JOB__BEGUN
  /*  56 0x38 */ "JOB__CANCEL"_Stz,         // CN_JOB__CANCEL
  /*  57 0x39 */ "JOB__CANCELLED"_Stz,      // CN_JOB__CANCELLED
  /*  58 0x3A */ "JOB__END"_Stz,            // CN_JOB__END
  /*  59 0x3B */ "JOB__ENDED"_Stz,          // CN_JOB__ENDED
  /*  60 0x3C */ "JOB__EXECUTE"_Stz,        // CN_JOB__EXECUTE
  /*  61 0x3D */ "JOB__EXECUTED"_Stz,       // CN_JOB__EXECUTED
  /*  62 0x3E */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /*  63 0x3F */ "JOB_ALARM"_Stz,           // CN_JOB_ALARM
  /*  64 0x40 */ "JOB_ANALYZEINDEX"_Stz,    // CN_JOB_ANALYZEINDEX
  /*  65 0x41 */ "JOB_ANALYZETABLE"_Stz,    // CN_JOB_ANALYZETABLE
  /*  66 0x42 */ "JOB_ANSWER"_Stz,          // CN_JOB_ANSWER
  /*  67 0x43 */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
  /*  68 0x44 */ "JOB_CAT"_Stz,             // CN_JOB_CAT
  /*  69 0x45 */ "JOB_CF"_Stz,              // CN_JOB_CF
  /*  70 0x46 */ "JOB_COMPACTINDEX"_Stz,    // CN_JOB_COMPACTINDEX
  /*  71 0x47 */ "JOB_CONNECTCLUSTER"_Stz,  // CN_JOB_CONNECTCLUSTER
  /*  72 0x48 */ "JOB_CONNECTOR"_Stz,       // CN_JOB_CONNECTOR
  /*  73 0x49 */ "JOB_COPYBUFFER"_Stz,      // CN_JOB_COPYBUFFER
  /*  74 0x4A */ "JOB_CREATECATALOG"_Stz,   // CN_JOB_CREATECATALOG
  /*  75 0x4B */ "JOB_CREATEINDEX"_Stz,     // CN_JOB_CREATEINDEX
  /*  76 0x4C */ "JOB_CREATESCHEMA"_Stz,    // CN_JOB_CREATESCHEMA
  /*  77 0x4D */ "JOB_CREATETABLE"_Stz,     // CN_JOB_CREATETABLE
  /*  78 0x4E */ "JOB_CREATETENANT"_Stz,    // CN_JOB_CREATETENANT
  /*  79 0x4F */ "JOB_DOCOL"_Stz,           // CN_JOB_DOCOL
  /*  80 0x50 */ "JOB_DROPCATALOG"_Stz,     // CN_JOB_DROPCATALOG
  /*  81 0x51 */ "JOB_DROPINDEX"_Stz,       // CN_JOB_DROPINDEX
  /*  82 0x52 */ "JOB_DROPSCHEMA"_Stz,      // CN_JOB_DROPSCHEMA
  /*  83 0x53 */ "JOB_DROPTABLE"_Stz,       // CN_JOB_DROPTABLE
  /*  84 0x54 */ "JOB_DROPTENANT"_Stz,      // CN_JOB_DROPTENANT
  /*  85 0x55 */ "JOB_DUMPINDEX"_Stz,       // CN_JOB_DUMPINDEX
  /*          */ "JOB_DUMPTABLE"_Stz,       // CN_JOB_DUMPTABLE
  /*  86 0x56 */ "JOB_EXPORT"_Stz,          // CN_JOB_EXPORT
  /*  87 0x57 */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
  /*  88 0x58 */ "JOB_GENERATE"_Stz,        // CN_JOB_GENERATE
  /*          */ "JOB_HEALTH"_Stz,          // CN_JOB_HEALTH
  /*  89 0x59 */ "JOB_INGEST"_Stz,          // CN_JOB_INGEST
  /*  90 0x5A */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
  /*  91 0x5B */ "JOB_INSERTTABLE"_Stz,     // CN_JOB_INSERTTABLE
  /* 139 0x8B */ "JOB_KIDEXPORT"_Stz,       // CN_JOB_KIDEXPORT
  /*  92 0x5C */ "JOB_LF"_Stz,              // CN_JOB_LF
  /*  93 0x5D */ "JOB_LISTCATALOGS"_Stz,    // CN_JOB_LISTCATALOGS
  /*  94 0x5E */ "JOB_LISTCOLUMNS"_Stz,     // CN_JOB_LISTCOLUMNS
  /*  95 0x5F */ "JOB_LISTIDXCOLUMNS"_Stz,  // CN_JOB_LISTIDXCOLUMNS
  /*  96 0x60 */ "JOB_LISTINDEXES"_Stz,     // CN_JOB_LISTINDEXES
  /*  97 0x61 */ "JOB_LISTPARTCOLUMNS"_Stz, // CN_JOB_LISTPARTCOLUMNS
  /*  98 0x62 */ "JOB_LISTPARTITIONS"_Stz,  // CN_JOB_LISTPARTITIONS
  /*  99 0x63 */ "JOB_LISTSCHEMAS"_Stz,     // CN_JOB_LISTSCHEMAS
  /* 100 0x64 */ "JOB_LISTTABLES"_Stz,      // CN_JOB_LISTTABLES
  /* 101 0x65 */ "JOB_LISTTENANTS"_Stz,     // CN_JOB_LISTTENANTS
  /* 102 0x66 */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
  /* 103 0x67 */ "JOB_MONITOR"_Stz,         // CN_JOB_MONITOR
  /* 104 0x68 */ "JOB_NETHOG"_Stz,          // CN_JOB_NETHOG
  /* 105 0x69 */ "JOB_NODEINFO"_Stz,        // CN_JOB_NODEINFO
  /* 106 0x6A */ "JOB_PERIODIC"_Stz,        // CN_JOB_PERIODIC
  /* 107 0x6B */ "JOB_PING"_Stz,            // CN_JOB_PING
  /* 108 0x6C */ "JOB_PUBLISH"_Stz,         // CN_JOB_PUBLISH
  /* 109 0x6D */ "JOB_PURGE"_Stz,           // CN_JOB_PURGE
  /* 110 0x6E */ "JOB_PWF"_Stz,             // CN_JOB_PWF
  /* 111 0x6F */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
  /* 112 0x70 */ "JOB_REPEATER"_Stz,        // CN_JOB_REPEATER
  /* 113 0x71 */ "JOB_REPORT"_Stz,          // CN_JOB_REPORT
  /* 114 0x72 */ "JOB_SELECTTABLE"_Stz,     // CN_JOB_SELECTTABLE
  /* 115 0x73 */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
  /* 116 0x74 */ "JOB_SHOWCLUSTER"_Stz,     // CN_JOB_SHOWCLUSTER
  /* 117 0x75 */ "JOB_SINK"_Stz,            // CN_JOB_SINK
  /* 118 0x76 */ "JOB_SIZEINDEX"_Stz,       // CN_JOB_SIZEINDEX
  /* 119 0x77 */ "JOB_SIZETABLE"_Stz,       // CN_JOB_SIZETABLE
  /* 120 0x78 */ "JOB_SPACE"_Stz,           // CN_JOB_SPACE
  /* 121 0x79 */ "JOB_START"_Stz,           // CN_JOB_START
  /* 122 0x7A */ "JOB_STARTCLUSTER"_Stz,    // CN_JOB_STARTCLUSTER
  /* 123 0x7B */ "JOB_STATUS"_Stz,          // CN_JOB_STATUS
  /* 124 0x7C */ "JOB_SUBEXPORTER"_Stz,     // CN_JOB_SUBEXPORTER
  /* 125 0x7D */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
  /* 126 0x7E */ "JOB_SUBINGESTER"_Stz,     // CN_JOB_SUBINGESTER
  /* 127 0x7F */ "JOB_SUBINSERTER"_Stz,     // CN_JOB_SUBINSERTER
  /* 128 0x80 */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
  /* 129 0x81 */ "JOB_SUBPUBLISH"_Stz,      // CN_JOB_SUBPUBLISH
  /* 130 0x82 */ "JOB_SUBPURGER"_Stz,       // CN_JOB_SUBPURGER
  /* 131 0x83 */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
  /*          */ "JOB_SUBWALKER"_Stz,       // CN_JOB_SUBWALKER
  /*          */ "JOB_SUPERVISOR"_Stz,      // CN_JOB_SUPERVISOR
  /* 132 0x84 */ "JOB_TABLESIZES"_Stz,      // CN_JOB_TABLESIZES
  /* 133 0x85 */ "JOB_TRUNCATETABLE"_Stz,   // CN_JOB_TRUNCATETABLE
  /* 134 0x86 */ "JOB_USESCHEMA"_Stz,       // CN_JOB_USESCHEMA
  /* 135 0x87 */ "JOB_VERIFYINDEX"_Stz,     // CN_JOB_VERIFYINDEX
  /*          */ "JOB_VERIFYTABLE"_Stz,     // CN_JOB_VERIFYTABLE
  /* 136 0x88 */ "JOB_VISUALIZEINDEX"_Stz,  // CN_JOB_VISUALIZEINDEX
  /*          */ "JOB_VISUALIZETABLE"_Stz,  // CN_JOB_VISUALIZETABLE
  /* 137 0x89 */ "JOB_WAITCLUSTER"_Stz,     // CN_JOB_WAITCLUSTER
  /* 137 0x89 */ "JOB_WALK"_Stz,            // CN_JOB_WALK
  /* 138 0x8A */ "JOB_WRITEBUFFER"_Stz,     // CN_JOB_WRITEBUFFER
  /* 140 0x8C */ "JOB_WRITEROW"_Stz,        // CN_JOB_WRITEROW
  /* 141 0x8D */ "JOBLIST"_Stz,             // CN_JOBLIST
  /* 142 0x8E */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
  /* 143 0x8F */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
  
  /* 144 0x90 */ "KEY"_Stz,                 // CN_KEY
  /* 145 0x91 */ "KEYWRK"_Stz,              // CN_KEYWRK
  
  /* 146 0x92 */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* 147 0x93 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* 148 0x94 */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* 149 0x95 */ "LOADMAP"_Stz,             // CN_LOADMAP
  /* 150 0x96 */ "LOCAL"_Stz,               // CN_LOCAL
  /* 151 0x97 */ "LOCALSET"_Stz,            // CN_LOCALSET
  /* 152 0x98 */ "LOCK"_Stz,                // CN_LOCK
  
  /* 153 0x99 */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* 154 0x9A */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  /*          */ "MEMTEST"_Stz,             // CN_MEMTEST
  /* 155 0x9B */ "MUTEX"_Stz,               // CN_MUTEX
  
  /*          */ "NMVAL"_Stz,               // CN_NMVAL
  /* 156 0x9C */ "NODE"_Stz,                // CN_NODE
  /* 157 0x9D */ "NOW"_Stz,                 // CN_NOW
  /*          */ "NUMBAT"_Stz,              // CN_NUMBAT

  /* 158 0x9E */ "OP"_Stz,                  // CN_OP
  
  /* 159 0x9F */ "PARSER"_Stz,              // CN_PARSER
  /* 160 0xA0 */ "PATHVEC"_Stz,             // CN_PATHVEC
  /*          */ "PERUSE"_Stz,              // CN_PERUSE
  /* 161 0xA1 */ "PIPE"_Stz,                // CN_PIPE
  /* 162 0xA2 */ "PLAN"_Stz,                // CN_PLAN
  /* 163 0xA3 */ "PLANNER"_Stz,             // CN_PLANNER
  /* 164 0xA4 */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* 165 0xA5 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* 166 0xA6 */ "PTHREAD"_Stz,             // CN_PTHREAD
  /* 167 0xA7 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  
  /* 168 0xA8 */ "QUEUE"_Stz,               // CN_QUEUE
  /* 169 0xA9 */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* 170 0xAA */ "QUEUES"_Stz,              // CN_QUEUES
  
  /*          */ "RECORD"_Stz,              // CN_RECORD
  /* 171 0xAB */ "RECORDBUFFER"_Stz,        // CN_RECORDBUFFER
  /* 172 0xAC */ "RESOURCETRACKER"_Stz,     // CN_RESOURCETRACKER
  /*          */ "RESPONSE"_Stz,            // CN_RESPONSE
  /*          */ "RESPONSEVEC"_Stz,         // CN_RESPONSEVEC
  /* 173 0xAD */ "ROWAGG"_Stz,              // CN_ROWAGG
  /*          */ "ROWBUFFER"_Stz,           // CN_ROWBUFFER
  /*          */ "ROWCODER"_Stz,            // CN_ROWCODER
  /* 174 0xAE */ "ROWONCE"_Stz,             // CN_ROWONCE
  /*          */ "ROWTRAY"_Stz,             // CN_ROWTRAY
  
  /* 176 0xB0 */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /*          */ "SEEME"_Stz,               // CN_SEEME
  /* 177 0xB1 */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* 178 0xB2 */ "SESSION"_Stz,             // CN_SESSION
  /* 179 0xB3 */ "SESSIONSTATE"_Stz,        // CN_SESSIONSTATE
  /* 180 0xB4 */ "SINCE"_Stz,               // CN_SINCE
  /* 181 0xB5 */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* 182 0xB6 */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* 183 0xB7 */ "SLAB"_Stz,                // CN_SLAB
  /* 184 0xB8 */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* 185 0xB9 */ "SLOTS"_Stz,               // CN_SLOTS
  /* 186 0xBA */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* 187 0xBB */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* 188 0xBC */ "SOCKET"_Stz,              // CN_SOCKET
  /* 189 0xBD */ "STACK"_Stz,               // CN_STACK
  /* 190 0xBE */ "STATISTICS"_Stz,          // CN_STATISTICS
  /* 191 0xBF */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* 192 0xC0 */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /* 193 0xC1 */ "STRING_T"_Stz,            // CN_STRING_T
  /* 194 0xC2 */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* 195 0xC3 */ "TABLE"_Stz,               // CN_TABLE
  /* 196 0xC4 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* 197 0xC5 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* 198 0xC6 */ "TASK"_Stz,                // CN_TASK
  /* 199 0xC7 */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* 200 0xC8 */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /*          */ "TEMPNAME"_Stz,            // CN_TEMPNAME
  /* 201 0xC9 */ "THREAD"_Stz,              // CN_THREAD
  /* 202 0xCA */ "THREADVEC"_Stz,           // CN_THREADVEC
  /* 203 0xCB */ "TIME"_Stz,                // CN_TIME
  /* 204 0xCC */ "TIMER"_Stz,               // CN_TIMER
  /* 205 0xCD */ "TOKEN"_Stz,               // CN_TOKEN
  /* 206 0xCE */ "TOKENISER"_Stz,           // CN_TOKENISER
  /* 207 0xCF */ "TOKENARY"_Stz,            // CN_TOKENARY
  /* 208 0xD0 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* 209 0xD1 */ "TX"_Stz,                  // CN_TX
  /* 210 0xD2 */ "TXSET"_Stz,               // CN_TXSET
  
  /* 211 0xD3 */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* 212 0xD4 */ "WORD"_Stz,                // CN_WORD
  
  /* 213 0xD5 */ "ZSMALL_"_Stz,             // CN_ZSMALL_
  /* 214 0xD6 */ "ZMEDIUM_"_Stz,            // CN_ZMEDIUM_
  /* 215 0xD7 */ "ZLARGE_"_Stz,             // CN_ZLARGE_
  /* 216 0xD8 */ "ZJUMBO_"_Stz,             // CN_ZJUMBO_
  
  /* 217 0xD9 */ "ENUMS"_Stz,               // CN_ENUMS
};

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
