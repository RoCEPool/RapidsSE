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
  /*                           */ CN_ACTIVITYMAP,
  /*  13 0x0D      200B Heap:I */ CN_ACTIVITYMARK,
  /*  14 0x0E   123040B Heap:L */ CN_APISTATE,
  /*  15 0x0F      296B Heap:S */ CN_ARG,
  /*  16 0x10     3408B Heap:I */ CN_ARGUMENTS,
  /*  17 0x11       24B Heap:I */ CN_ATOMIC,
  /*  18 0x12    13872B Heap:I */ CN_ATTACHMAP,
  
  /*  19 0x13       16B Heap:I */ CN_BPA,
  /*  20 0x14      264B Heap:I */ CN_BUFFERPOOL,
  /*  21 0x15      120B Heap:S */ CN_BYTERATE,
  
  /*  22 0x16       40B Heap:I */ CN_CARRYOVER,
  /*  23 0x17      120B Heap:S */ CN_COLUMN,
  /*  24 0x18     8216B Heap:M */ CN_COLUMNVEC,
  /*  26 0x1A       40B Heap:M */ CN_COMMAND,
  /*  25 0x19       64B Heap:M */ CN_COMPILER,
  
  /*  27 0x1B       24B Heap:S */ CN_DECIDER,
  /*  28 0x1C      248B Heap:M */ CN_DIC,
  /*  29 0x1D      792B Heap:S */ CN_DICPATH,
  /*  30 0x1E      152B Heap:S */ CN_DICSTACK,
  /*  31 0x1F      608B Heap:S */ CN_DIR,
  
  /*  32 0x20       20B Heap:I */ CN_ENTRY,
  /*  33 0x21  1043808B Heap:L */ CN_ENTRYBUFFER,
  
  /*  34 0x22  1040440B Heap:L */ CN_FETCHBUFFER,
  /*  35 0x23      376B Heap:S */ CN_FILE,
  /*  36 0x24       48B Heap:S */ CN_FORMATTERS,
  /*  37 0x25       32B Heap:I */ CN_FREEBUFFER,
  /*  38 0x26       24B Heap:I */ CN_FREECHUNK,
  /*  39 0x27        0B Heap:I */ CN_FREEDNOT,
  /*  40 0x28        0B Heap:I */ CN_FREEENTRY,
  
  /*  41 0x29    64024B Heap:I */ CN_GENVAR,
  /*  42 0x2A     1224B Heap:L */ CN_GRID,
  
  /*  43 0x2B       48B Heap:I */ CN_HEAPBUFFER,
  
  /*  44 0x2C      368B Heap:I */ CN_INDEX,
  /*  45 0x2D      184B Heap:S */ CN_INDEXTREE,
  /*  46 0x2E     8224B Heap:M */ CN_INDEXTREEVEC,
  /*  47 0x2F     8216B Heap:M */ CN_IDXCOLVEC,
  /*  48 0x30     8224B Heap:M */ CN_IDXVEC,
  /*  49 0x31  1000096B Heap:L */ CN_INGESTCHUNK,
  /*  50 0x32  1000440B Heap:L */ CN_IOCHUNK,
  /*  51 0x33       32B Heap:S */ CN_IOCOPY,
  /*  52 0x34       12B Heap:I */ CN_ITEM,
  /*  53 0x35  1043480B Heap:I */ CN_ITEMBUFFER,
  
  /*  54 0x36    70736B Heap:L */ CN_JOB__BEGUN,
  /*  55 0x37    70728B Heap:L */ CN_JOB__CANCEL,
  /*  56 0x38    70736B Heap:L */ CN_JOB__CANCELLED,
  /*  57 0x39    70728B Heap:L */ CN_JOB__END,
  /*  58 0x3A    70728B Heap:L */ CN_JOB__ENDED,
  /*  59 0x3B    70728B Heap:L */ CN_JOB__EXECUTE,
  /*  60 0x3C    71032B Heap:L */ CN_JOB__EXECUTED,
  /*  61 0x3D    70728B Heap:L */ CN_JOB__FIND,
  /*  62 0x3E    70728B Heap:L */ CN_JOB_ALARM,
  /*  63 0x3F    70736B Heap:L */ CN_JOB_ANALYZEINDEX,
  /*  64 0x40    70736B Heap:L */ CN_JOB_ANALYZETABLE,
  /*  65 0x41    70728B Heap:L */ CN_JOB_ANSWER,
  /*  66 0x42    70728B Heap:L */ CN_JOB_BOOT,
  /*  67 0x43    71296B Heap:L */ CN_JOB_CAT,
  /*  68 0x44    70728B Heap:L */ CN_JOB_CF,
  /*  69 0x45    70728B Heap:L */ CN_JOB_COMPACTINDEX,
  /*  70 0x46    70728B Heap:L */ CN_JOB_CONNECTCLUSTER,
  /*  71 0x47    70728B Heap:L */ CN_JOB_CONNECTOR,
  /*  72 0x48    70744B Heap:L */ CN_JOB_COPYBUFFER,
  /*  73 0x49    70728B Heap:L */ CN_JOB_CREATECATALOG,
  /*  74 0x4A    77944B Heap:L */ CN_JOB_CREATEINDEX,
  /*  75 0x4B    70728B Heap:L */ CN_JOB_CREATESCHEMA,
  /*  76 0x4C    83944B Heap:L */ CN_JOB_CREATETABLE,
  /*  77 0x4D    70728B Heap:L */ CN_JOB_CREATETENANT,
  /*  78 0x4E    70728B Heap:L */ CN_JOB_DOCOL,
  /*  79 0x4F    70728B Heap:L */ CN_JOB_DROPCATALOG,
  /*  80 0x50    70728B Heap:L */ CN_JOB_DROPINDEX,
  /*  81 0x51    70728B Heap:L */ CN_JOB_DROPSCHEMA,
  /*  82 0x52    70728B Heap:L */ CN_JOB_DROPTABLE,
  /*  83 0x53    70728B Heap:L */ CN_JOB_DROPTENANT,
  /*  84 0x54    70728B Heap:L */ CN_JOB_DUMPINDEX,
  /*  85 0x55    70728B Heap:L */ CN_JOB_EXPORT,
  /*  86 0x56    71072B Heap:L */ CN_JOB_FETCHROWS,
  /*  87 0x57   123048B Heap:L */ CN_JOB_GENERATE,
  /*  88 0x58    71592B Heap:L */ CN_JOB_INGEST,
  /*  89 0x59   118768B Heap:L */ CN_JOB_INSERTROW,
  /*  90 0x5A    70736B Heap:L */ CN_JOB_INSERTTABLE,
  /*  91 0x5B    70728B Heap:L */ CN_JOB_LF,
  /*  92 0x5C    70728B Heap:L */ CN_JOB_LISTCATALOGS,
  /*  93 0x5D    70728B Heap:L */ CN_JOB_LISTCOLUMNS,
  /*  94 0x5E    70728B Heap:L */ CN_JOB_LISTIDXCOLUMNS,
  /*  95 0x5F    70728B Heap:L */ CN_JOB_LISTINDEXES,
  /*  96 0x60    70728B Heap:L */ CN_JOB_LISTPARTCOLUMNS,
  /*  97 0x61    70728B Heap:L */ CN_JOB_LISTPARTITIONS,
  /*  98 0x62    70728B Heap:L */ CN_JOB_LISTSCHEMAS,
  /*  99 0x63    70728B Heap:L */ CN_JOB_LISTTABLES,
  /* 100 0x64    70728B Heap:L */ CN_JOB_LISTTENANTS,
  /* 101 0x65    71064B Heap:L */ CN_JOB_LOADTABLE,
  /* 102 0x66    71032B Heap:L */ CN_JOB_MONITOR,
  /* 103 0x67    70736B Heap:L */ CN_JOB_NETHOG,
  /* 104 0x68    70776B Heap:L */ CN_JOB_NODEINFO,
  /* 105 0x69    70728B Heap:L */ CN_JOB_PERIODIC,
  /* 106 0x6A    70728B Heap:L */ CN_JOB_PING,
  /* 107 0x6B    71136B Heap:L */ CN_JOB_PUBLISH,
  /* 108 0x6C    71016B Heap:L */ CN_JOB_PURGE,
  /* 109 0x6D    70728B Heap:L */ CN_JOB_PWF,
  /* 110 0x6E    70744B Heap:L */ CN_JOB_RECEIVER,
  /* 111 0x6F    71304B Heap:L */ CN_JOB_REPEATER,
  /* 112 0x70    71304B Heap:L */ CN_JOB_REPORT,
  /* 113 0x71    70744B Heap:L */ CN_JOB_SELECTTABLE,
  /* 114 0x72    70744B Heap:L */ CN_JOB_SANITY,
  /* 115 0x73    70728B Heap:L */ CN_JOB_SHOWCLUSTER,
  /* 116 0x74    71016B Heap:L */ CN_JOB_SINK,
  /* 117 0x75    70728B Heap:L */ CN_JOB_SIZEINDEX,
  /* 118 0x76    70728B Heap:L */ CN_JOB_SIZETABLE,
  /* 119 0x77    70728B Heap:L */ CN_JOB_SPACE,
  /* 120 0x78    70728B Heap:L */ CN_JOB_START,
  /* 121 0x79    70728B Heap:L */ CN_JOB_STARTCLUSTER,
  /* 122 0x7A    70728B Heap:L */ CN_JOB_STATUS,
  /* 123 0x7B    70768B Heap:L */ CN_JOB_SUBFETCH,
  /* 124 0x7C   151600B Heap:L */ CN_JOB_SUBINGESTER,
  /* 125 0x7D   151784B Heap:L */ CN_JOB_SUBINSERTER,
  /* 126 0x7E   151608B Heap:L */ CN_JOB_SUBLOADER,
  /* 127 0x7F    71152B Heap:L */ CN_JOB_SUBPUBLISH,
  /* 128 0x80    70768B Heap:L */ CN_JOB_SUBPURGER,
  /* 129 0x81    70744B Heap:L */ CN_JOB_SUBTRUNCATE,
  /* 130 0x82    70728B Heap:L */ CN_JOB_TABLESIZES,
  /* 131 0x83    70728B Heap:L */ CN_JOB_TRUNCATETABLE,
  /* 132 0x84    70728B Heap:L */ CN_JOB_USESCHEMA,
  /* 133 0x85    70728B Heap:L */ CN_JOB_VERIFYINDEX,
  /* 134 0x86    70728B Heap:L */ CN_JOB_VISUALIZEINDEX,
  /* 135 0x87    70728B Heap:L */ CN_JOB_WAITCLUSTER,
  /* 136 0x88    70784B Heap:L */ CN_JOB_WRITEBUFFER,
  /* 137 0x89   118824B Heap:L */ CN_JOB_WRITEROW,
  /* 138 0x8A      168B Heap:S */ CN_JOBLIST,
  /* 139 0x8B      232B Heap:I */ CN_JUMBOPERSISTENT,
  /* 140 0x8C      232B Heap:I */ CN_JUMBOTRANSIENT,
  
  /* 141 0x8D        1B Heap:I */ CN_KEY,
  /* 142 0x8E    17472B Heap:I */ CN_KEYWRK,
  
  /* 143 0x8F      232B Heap:I */ CN_LARGEPERSISTENT,
  /* 144 0x90      232B Heap:I */ CN_LARGETRANSIENT,
  /* 145 0x91  1000440B Heap:L */ CN_LOADCHUNK,
  /* 146 0x92  9389216B Heap:J */ CN_LOADMAP,
  /* 147 0x93     8216B Heap:I */ CN_LOCAL,
  /* 148 0x94   262944B Heap:L */ CN_LOCALSET,
  /* 149 0x95      128B Heap:I */ CN_LOCK,
  
  /* 150 0x96      232B Heap:I */ CN_MEDIUMPERSISTENT,
  /* 151 0x97      232B Heap:I */ CN_MEDIUMTRANSIENT,
  /* 180 0xB4      160B Heap:S */ CN_MUTEX,

  /* 152 0x98      168B Heap:S */ CN_NODE,
  /* 153 0x99       88B Heap:S */ CN_NOW,
  
  /* 154 0x9A       24B Heap:S */ CN_OP,
  
  /* 155 0x9B    48752B Heap:M */ CN_PARSER,
  /* 156 0x9C     1048B Heap:M */ CN_PATHVEC,
  /* 160 0xA0      320B Heap:S */ CN_PIPE,
  /* 161 0xA1    48376B Heap:M */ CN_PLAN,
  /* 162 0xA2     3112B Heap:M */ CN_PLANNER,
  /* 164 0xA4       48B Heap:M */ CN_PREDICATE,
  /* 165 0xA5      184B Heap:S */ CN_PREDICATEVEC,
  /* 163 0xA3       32B Heap:S */ CN_PTHREAD,
  /* 166 0xA6     8216B Heap:M */ CN_PTNCOLVEC,
  
  /* 167 0xA7      168B Heap:S */ CN_QUEUE,
  /* 168 0xA8       64B Heap:I */ CN_QUEUEBUFFER,
  /* 169 0xA9     1232B Heap:S */ CN_QUEUES,
  
  /* 171 0xAB  1043496B Heap:L */ CN_RECORDBUFFER,
  /* 172 0xAC      160B Heap:S */ CN_RESOURCETRACKER,
  /* 174 0xAE      896B Heap:M */ CN_ROWAGG,
  /* 175 0xAF       56B Heap:S */ CN_ROWCURSOR,
  /* 176 0xB0      152B Heap:S */ CN_ROWONCE,
  /* 177 0xB1    17472B Heap:I */ CN_ROWWRK,
  
  /* 178 0xB2    50208B Heap:L */ CN_SCHEMATEST,
  /* 179 0xB3       56B Heap:S */ CN_SELCURSOR,
  /* 181 0xB5       72B Heap:L */ CN_SEPROMPT,
  /* 182 0xB6    48288B Heap:M */ CN_SESSION,
  /* 182 0xB6    48288B Heap:M */ CN_SESSIONSTATE,
  /* 183 0xB7       80B Heap:I */ CN_SINCE,
  /* 184 0xB8       64B Heap:S */ CN_SKIPLIST,
  /* 185 0xB9       32B Heap:S */ CN_SKIPNODE,
  /* 186 0xBA  1040416B Heap:L */ CN_SLAB,
  /* 187 0xBB       24B Heap:S */ CN_SLABALLOCATOR,
  /* 188 0xBC      288B Heap:I */ CN_SLOTS,
  /* 189 0xBD      232B Heap:I */ CN_SMALLPERSISTENT,
  /* 190 0xBE      232B Heap:I */ CN_SMALLTRANSIENT,
  /* 191 0xBF      176B Heap:S */ CN_SOCKET,
  /* 192 0xC0  1043520B Heap:L */ CN_STACK,
  /*                           */ CN_STATISTICS,
  /* 193 0xC1       64B Heap:I */ CN_STEPACCUM,
  /* 194 0xC2   131224B Heap:L */ CN_STEPTIMING,
  /* 195 0xC3       48B Heap:I */ CN_STRING_T,
  /* 196 0xC4       32B Heap:I */ CN_SYSCALL,
  
  /* 197 0xC5     3736B Heap:I */ CN_TABLE,
  /* 198 0xC6      200B Heap:S */ CN_TABLEPTN,
  /* 199 0xC7     1048B Heap:M */ CN_TABLEPTNVEC,
  /* 200 0xC8     1360B Heap:M */ CN_TASK,
  /* 201 0xC9      184B Heap:S */ CN_TASKLIST,
  /* 202 0xCA      160B Heap:S */ CN_TASKQUEUE,
  /* 203 0xCB      224B Heap:S */ CN_THREAD,
  /* 204 0xCC     2208B Heap:M */ CN_THREADVEC,
  /* 205 0xCD       24B Heap:I */ CN_TIME,
  /* 206 0xCE       32B Heap:S */ CN_TIMER,
  /* 207 0xCF      296B Heap:S */ CN_TOKEN,
  /* 208 0xD0    56832B Heap:M */ CN_TOKENISER,
  /* 209 0xD1     8216B Heap:I */ CN_TOKENARY,
  /* 210 0xD2     8232B Heap:M */ CN_TOKENVEC,
  /* 211 0xD3      208B Heap:S */ CN_TX,
  /* 212 0xD4      176B Heap:S */ CN_TXSET,
  
  /* 213 0xD5       12B Heap:I */ CN_WILDERNESS,
  /* 214 0xD6    64672B Heap:M */ CN_WORD,
  
  /* 215 0xD7     3840B Heap:I */ CN_ZSMALL_,
  /* 216 0xD8    65280B Heap:I */ CN_ZMEDIUM_,
  /* 217 0xD9  1048320B Heap:I */ CN_ZLARGE_,
  /* 218 0xDA 16776960B Heap:I */ CN_ZJUMBO_,
  
  /* 219 0xDB       -1B Heap:I */ CN_ENUMS,
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
  /*          */ "ACTIVITYMAP"_Stz,         // CN_ACTIVITYMAP
  /*  13 0x0D */ "ACTIVITYMARK"_Stz,        // CN_ACTIVITYMARK
  /*  14 0x0E */ "APISTATE"_Stz,            // CN_APISTATE
  /*  15 0x0F */ "ARG"_Stz,                 // CN_ARG
  /*  16 0x10 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /*  17 0x11 */ "ATOMIC"_Stz,              // CN_ATOMIC
  /*  18 0x12 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
  
  /*  19 0x13 */ "BPA"_Stz,                 // CN_BPA
  /*  20 0x14 */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /*  21 0x15 */ "BYTERATE"_Stz,            // CN_BYTERATE
  
  /*  22 0x16 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /*  23 0x17 */ "COLUMN"_Stz,              // CN_COLUMN
  /*  24 0x18 */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
  /*  26 0x1A */ "COMMAND"_Stz,             // CN_COMMAND
  /*  25 0x19 */ "COMPILER"_Stz,            // CN_COMPILER
  
  /*  27 0x1B */ "DECIDER"_Stz,             // CN_DECIDER
  /*  28 0x1C */ "DIC"_Stz,                 // CN_DIC
  /*  29 0x1D */ "DICPATH"_Stz,             // CN_DICPATH
  /*  30 0x1E */ "DICSTACK"_Stz,            // CN_DICSTACK
  /*  31 0x1F */ "DIR"_Stz,                 // CN_DIR
  
  /*  32 0x20 */ "ENTRY"_Stz,               // CN_ENTRY
  /*  33 0x21 */ "ENTRYBUFFER"_Stz,         // CN_ENTRYBUFFER
  
  /*  34 0x22 */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /*  35 0x23 */ "FILE"_Stz,                // CN_FILE
  /*  36 0x24 */ "FORMATTERS"_Stz,          // CN_FORMATTERS
  /*  37 0x25 */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /*  38 0x26 */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /*  39 0x27 */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /*  40 0x28 */ "FREEENTRY"_Stz,           // CN_FREEENTRY
  
  /*  41 0x29 */ "GENVAR"_Stz,              // CN_GENVAR
  /*  42 0x2A */ "GRID"_Stz,                // CN_GRID
  
  /*  43 0x2B */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  
  /*  44 0x2C */ "INDEX"_Stz,               // CN_INDEX
  /*  45 0x2D */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /*  46 0x2E */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /*  47 0x2F */ "IDXCOLVEC"_Stz,           // CN_IDXCOLVEC
  /*  48 0x30 */ "IDXVEC"_Stz,              // CN_IDXVEC
  /*  49 0x31 */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /*  50 0x32 */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /*  51 0x33 */ "IOCOPY"_Stz,              // CN_IOCOPY
  /*  52 0x34 */ "ITEM"_Stz,                // CN_ITEM
  /*  53 0x35 */ "ITEMBUFFER"_Stz,          // CN_ITEMBUFFER
  
  /*  54 0x36 */ "JOB__BEGUN"_Stz,          // CN_JOB__BEGUN
  /*  55 0x37 */ "JOB__CANCEL"_Stz,         // CN_JOB__CANCEL
  /*  56 0x38 */ "JOB__CANCELLED"_Stz,      // CN_JOB__CANCELLED
  /*  57 0x39 */ "JOB__END"_Stz,            // CN_JOB__END
  /*  58 0x3A */ "JOB__ENDED"_Stz,          // CN_JOB__ENDED
  /*  59 0x3B */ "JOB__EXECUTE"_Stz,        // CN_JOB__EXECUTE
  /*  60 0x3C */ "JOB__EXECUTED"_Stz,       // CN_JOB__EXECUTED
  /*  61 0x3D */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /*  62 0x3E */ "JOB_ALARM"_Stz,           // CN_JOB_ALARM
  /*  63 0x3F */ "JOB_ANALYZEINDEX"_Stz,    // CN_JOB_ANALYZEINDEX
  /*  64 0x40 */ "JOB_ANALYZETABLE"_Stz,    // CN_JOB_ANALYZETABLE
  /*  65 0x41 */ "JOB_ANSWER"_Stz,          // CN_JOB_ANSWER
  /*  66 0x42 */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
  /*  67 0x43 */ "JOB_CAT"_Stz,             // CN_JOB_CAT
  /*  68 0x44 */ "JOB_CF"_Stz,              // CN_JOB_CF
  /*  69 0x45 */ "JOB_COMPACTINDEX"_Stz,    // CN_JOB_COMPACTINDEX
  /*  70 0x46 */ "JOB_CONNECTCLUSTER"_Stz,  // CN_JOB_CONNECTCLUSTER
  /*  71 0x47 */ "JOB_CONNECTOR"_Stz,       // CN_JOB_CONNECTOR
  /*  72 0x48 */ "JOB_COPYBUFFER"_Stz,      // CN_JOB_COPYBUFFER
  /*  73 0x49 */ "JOB_CREATECATALOG"_Stz,   // CN_JOB_CREATECATALOG
  /*  74 0x4A */ "JOB_CREATEINDEX"_Stz,     // CN_JOB_CREATEINDEX
  /*  75 0x4B */ "JOB_CREATESCHEMA"_Stz,    // CN_JOB_CREATESCHEMA
  /*  76 0x4C */ "JOB_CREATETABLE"_Stz,     // CN_JOB_CREATETABLE
  /*  77 0x4D */ "JOB_CREATETENANT"_Stz,    // CN_JOB_CREATETENANT
  /*  78 0x4E */ "JOB_DOCOL"_Stz,           // CN_JOB_DOCOL
  /*  79 0x4F */ "JOB_DROPCATALOG"_Stz,     // CN_JOB_DROPCATALOG
  /*  80 0x50 */ "JOB_DROPINDEX"_Stz,       // CN_JOB_DROPINDEX
  /*  81 0x51 */ "JOB_DROPSCHEMA"_Stz,      // CN_JOB_DROPSCHEMA
  /*  82 0x52 */ "JOB_DROPTABLE"_Stz,       // CN_JOB_DROPTABLE
  /*  83 0x53 */ "JOB_DROPTENANT"_Stz,      // CN_JOB_DROPTENANT
  /*  84 0x54 */ "JOB_DUMPINDEX"_Stz,       // CN_JOB_DUMPINDEX
  /*  85 0x55 */ "JOB_EXPORT"_Stz,          // CN_JOB_EXPORT
  /*  86 0x56 */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
  /*  87 0x57 */ "JOB_GENERATE"_Stz,         // CN_JOB_GENERATE
  /*  88 0x58 */ "JOB_INGEST"_Stz,          // CN_JOB_INGEST
  /*  89 0x59 */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
  /*  90 0x5A */ "JOB_INSERTTABLE"_Stz,     // CN_Job_INSERTTABLE
  /*  91 0x5B */ "JOB_LF"_Stz,              // CN_JOB_LF
  /*  92 0x5C */ "JOB_LISTCATALOGS"_Stz,    // CN_JOB_LISTCATALOGS
  /*  93 0x5D */ "JOB_LISTCOLUMNS"_Stz,     // CN_JOB_LISTCOLUMNS
  /*  94 0x5E */ "JOB_LISTIDXCOLUMNS"_Stz,  // CN_JOB_LISTIDXCOLUMNS
  /*  95 0x5F */ "JOB_LISTINDEXES"_Stz,     // CN_JOB_LISTINDEXES
  /*  96 0x60 */ "JOB_LISTPARTCOLUMNS"_Stz, // CN_JOB_LISTPARTCOLUMNS
  /*  97 0x61 */ "JOB_LISTPARTITIONS"_Stz,  // CN_JOB_LISTPARTITIONS
  /*  98 0x62 */ "JOB_LISTSCHEMAS"_Stz,     // CN_JOB_LISTSCHEMAS
  /*  99 0x63 */ "JOB_LISTTABLES"_Stz,      // CN_JOB_LISTTABLES
  /* 100 0x64 */ "JOB_LISTTENANTS"_Stz,     // CN_JOB_LISTTENANTS
  /* 101 0x65 */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
  /* 102 0x66 */ "JOB_MONITOR"_Stz,         // CN_JOB_MONITOR
  /* 103 0x67 */ "JOB_NETHOG"_Stz,          // CN_JOB_NETHOG
  /* 104 0x68 */ "JOB_NODEINFO"_Stz,        // CN_JOB_NODEINFO
  /* 105 0x69 */ "JOB_PERIODIC"_Stz,        // CN_JOB_PERIODIC
  /* 106 0x6A */ "JOB_PING"_Stz,            // CN_JOB_PING
  /* 107 0x6B */ "JOB_PUBLISH"_Stz,         // CN_JOB_PUBLISH
  /* 108 0x6C */ "JOB_PURGE"_Stz,           // CN_JOB_PURGE
  /* 109 0x6D */ "JOB_PWF"_Stz,             // CN_JOB_PWF
  /* 110 0x6E */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
  /* 111 0x6F */ "JOB_REPEATER"_Stz,        // CN_JOB_REPEATER
  /* 112 0x70 */ "JOB_REPORT"_Stz,          // CN_JOB_REPORT
  /* 113 0x71 */ "JOB_SELECTTABLE"_Stz,     // CN_JOB_SELECTTABLE
  /* 114 0x72 */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
  /* 115 0x73 */ "JOB_SHOWCLUSTER"_Stz,     // CN_JOB_SHOWCLUSTER
  /* 116 0x74 */ "JOB_SINK"_Stz,            // CN_JOB_SINK
  /* 117 0x75 */ "JOB_SIZEINDEX"_Stz,       // CN_JOB_SIZEINDEX
  /* 118 0x76 */ "JOB_SIZETABLE"_Stz,       // CN_JOB_SIZETABLE
  /* 119 0x77 */ "JOB_SPACE"_Stz,           // CN_JOB_SPACE
  /* 120 0x78 */ "JOB_START"_Stz,           // CN_JOB_START
  /* 121 0x79 */ "JOB_STARTCLUSTER"_Stz,    // CN_JOB_STARTCLUSTER
  /* 122 0x7A */ "JOB_STATUS"_Stz,          // CN_JOB_STATUS
  /* 123 0x7B */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
  /* 124 0x7C */ "JOB_SUBINGESTER"_Stz,     // CN_JOB_SUBINGESTER
  /* 125 0x7D */ "JOB_SUBINSERTER"_Stz,     // CN_JOB_SUBINSERTER
  /* 126 0x7E */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
  /* 127 0x7F */ "JOB_SUBPUBLISH"_Stz,      // CN_JOB_SUBPUBLISH
  /* 128 0x80 */ "JOB_SUBPURGER"_Stz,       // CN_JOB_SUBPURGER
  /* 129 0x81 */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
  /* 130 0x82 */ "JOB_TABLESIZES"_Stz,      // CN_JOB_TABLESIZES
  /* 131 0x83 */ "JOB_TRUNCATETABLE"_Stz,   // CN_JOB_TRUNCATETABLE
  /* 132 0x84 */ "JOB_USESCHEMA"_Stz,       // CN_JOB_USESCHEMA
  /* 133 0x85 */ "JOB_VERIFYINDEX"_Stz,     // CN_JOB_VERIFYINDEX
  /* 134 0x86 */ "JOB_VISUALIZEINDEX"_Stz,  // CN_JOB_VISUALIZEINDEX
  /* 135 0x87 */ "JOB_WAITCLUSTER"_Stz,     // CN_JOB_WAITCLUSTER
  /* 136 0x88 */ "JOB_WRITEBUFFER"_Stz,     // CN_JOB_WRITEBUFFER
  /* 137 0x89 */ "JOB_WRITEROW"_Stz,        // CN_JOB_WRITEROW
  /* 138 0x8A */ "JOBS"_Stz,                // CN_JOBLIST
  /* 139 0x8B */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
  /* 140 0x8C */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
  
  /* 141 0x8D */ "KEY"_Stz,                 // CN_KEY
  /* 142 0x8E */ "KEYWRK"_Stz,              // CN_KEYWRK
  
  /* 143 0x8F */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* 144 0x90 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* 145 0x91 */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* 146 0x92 */ "LOADMAP"_Stz,             // CN_LOADMAP
  /* 147 0x93 */ "LOCAL"_Stz,               // CN_LOCAL
  /* 148 0x94 */ "LOCALSET"_Stz,            // CN_LOCALSET
  /* 149 0x95 */ "LOCK"_Stz,                // CN_LOCK
  
  /* 150 0x96 */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* 151 0x97 */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  /* 180 0xB4 */ "MUTEX"_Stz,               // CN_MUTEX

  /* 152 0x98 */ "NODE"_Stz,                // CN_NODE
  /* 153 0x99 */ "NOW"_Stz,                 // CN_NOW
  
  /* 154 0x9A */ "OP"_Stz,                  // CN_OP
  
  /* 155 0x9B */ "PARSER"_Stz,              // CN_PARSER
  /* 156 0x9C */ "PATHVEC"_Stz,             // CN_PATHVEC
  /* 160 0xA0 */ "PIPE"_Stz,                // CN_PIPE
  /* 161 0xA1 */ "PLAN"_Stz,                // CN_PLAN
  /* 162 0xA2 */ "PLANNER"_Stz,             // CN_PLANNER
  /* 164 0xA4 */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* 165 0xA5 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* 163 0xA3 */ "POSIXTHREAD"_Stz,         // CN_PTHREAD
  /* 166 0xA6 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  
  /* 167 0xA7 */ "QUEUE"_Stz,               // CN_QUEUE
  /* 168 0xA8 */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* 169 0xA9 */ "QUEUES"_Stz,              // CN_QUEUES
  
  /* 171 0xAB */ "RECORDBUFFER"_Stz,        // CN_RECORDBUFFER
  /* 172 0xAC */ "RESOURCETRACKER"_Stz,     // CN_RESOURCETRACKER
  /* 174 0xAE */ "ROWAGG"_Stz,              // CN_ROWAGG
  /* 175 0xAF */ "ROWCURSOR"_Stz,           // CN_ROWCURSOR
  /* 176 0xB0 */ "ROWONCE"_Stz,             // CN_ROWONCE
  /* 177 0xB1 */ "ROWWRK"_Stz,              // CN_ROWWRK
  
  /* 178 0xB2 */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /* 179 0xB3 */ "SELCURSOR"_Stz,           // CN_SELCURSOR
  /* 181 0xB5 */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* 182 0xB6 */ "SESSION"_Stz,             // CN_SESSION
  /* 182 0xB6 */ "SESSION"_Stz,             // CN_SESSIONSTATE
  /* 183 0xB7 */ "SINCE"_Stz,               // CN_SINCE
  /* 184 0xB8 */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* 185 0xB9 */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* 186 0xBA */ "SLAB"_Stz,                // CN_SLAB
  /* 187 0xBB */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* 188 0xBC */ "SLOTS"_Stz,               // CN_SLOTS
  /* 189 0xBD */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* 190 0xBE */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* 191 0xBF */ "SOCKET"_Stz,              // CN_SOCKET
  /* 192 0xC0 */ "STACK"_Stz,               // CN_STACK
  /* 193 0xC1 */ "STATISTICS"_Stz,          // CN_STATISTICS
  /* 193 0xC1 */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* 194 0xC2 */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /* 195 0xC3 */ "STRING_T"_Stz,            // CN_STRING_T
  /* 196 0xC4 */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* 197 0xC5 */ "TABLE"_Stz,               // CN_TABLE
  /* 198 0xC6 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* 199 0xC7 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* 200 0xC8 */ "TASK"_Stz,                // CN_TASK
  /* 201 0xC9 */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* 202 0xCA */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /* 203 0xCB */ "THREAD"_Stz,              // CN_THREAD
  /* 204 0xCC */ "THREADVECTOR"_Stz,        // CN_THREADVEC
  /* 205 0xCD */ "TIME"_Stz,                // CN_TIME
  /* 206 0xCE */ "TIMER"_Stz,               // CN_TIMER
  /* 207 0xCF */ "TOKEN"_Stz,               // CN_TOKEN
  /* 208 0xD0 */ "TOKENISER"_Stz,           // CN_TOKENISER
  /* 209 0xD1 */ "TOKENARY"_Stz,            // CN_TOKENARY
  /* 210 0xD2 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* 211 0xD3 */ "TX"_Stz,                  // CN_TX
  /* 212 0xD4 */ "TXSET"_Stz,               // CN_TXSET
  
  /* 213 0xD5 */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* 214 0xD6 */ "WORD"_Stz,                // CN_WORD
  
  /* 215 0xD7 */ "ZSMALL_"_Stz,             // CN_ZSMALL_
  /* 216 0xD8 */ "ZMEDIUM_"_Stz,            // CN_ZMEDIUM_
  /* 217 0xD9 */ "ZLARGE_"_Stz,             // CN_ZLARGE_
  /* 218 0xDA */ "ZJUMBO_"_Stz,             // CN_ZJUMBO_
  
  /* 219 0xDB */ "ENUMS"_Stz,               // CN_ENUMS
};

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
