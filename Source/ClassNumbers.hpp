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
  /*   0 0x000        0B Heap:I */ CN_NONE,
  
  /*   1 0x001        1B Heap:I */ CN__A__,
  /*   2 0x002       80B Heap:M */ CN_ACCESSPATH,
  /*   3 0x003      304B Heap:S */ CN_ARG,
  /*   4 0x004    16584B Heap:I */ CN_ARGUMENTS,
  /*   5 0x005      384B Heap:S */ CN_ARTICLE,
  /*   6 0x006       24B Heap:I */ CN_ATOMIC,
  /*   7 0x007    26000B Heap:I */ CN_ATTACHMAP,
  
  /*   8 0x008        2B Heap:I */ CN__B__,
  /*   9 0x009       32B Heap:I */ CN_BIRTH,
  /*  10 0x00A        1B Heap:I */ CN_BPA,
  /*  11 0x00B     8400B Heap:L */ CN_BUCKETMAP,
  /*  12 0x00C       32B Heap:L */ CN_BUFFERCURSOR,
  /*  13 0x00D      384B Heap:I */ CN_BUFFERPOOL,
  /*  14 0x00E      128B Heap:S */ CN_BYTERATE,
  /*  15 0x00F       56B Heap:I */ CN_BYTES_T,
  
  /*  16 0x010        3B Heap:I */ CN__C__,
  /*  17 0x011    36880B Heap:M */ CN_CALCULATOR,
  /*  18 0x012       80B Heap:S */ CN_CANCELMARKER,
  /*  19 0x013       40B Heap:I */ CN_CARRYOVER,
  /*  20 0x014       16B Heap:I */ CN_CATALOG,
  /*  21 0x015      136B Heap:M */ CN_CLAUSE_FROM,
  /*  22 0x016      136B Heap:M */ CN_CLAUSE_GROUPBY,
  /*  23 0x017      136B Heap:M */ CN_CLAUSE_JOIN,
  /*  24 0x018      136B Heap:M */ CN_CLAUSE_LIMIT,
  /*  25 0x019      136B Heap:M */ CN_CLAUSE_ORDERBY,
  /*  26 0x01A      136B Heap:M */ CN_CLAUSE_SELECT,
  /*  27 0x01B      136B Heap:M */ CN_CLAUSE_SUBQRY,
  /*  28 0x01C      136B Heap:M */ CN_CLAUSE_WHERE,
  /*  29 0x01D     4096B Heap:M */ CN_CMD_SELECT,
  /*  30 0x01E      184B Heap:S */ CN_COLUMN,
  /*  31 0x01F     8216B Heap:M */ CN_COLUMNVEC,
  /*  32 0x020      176B Heap:M */ CN_COMMAND,
  /*  33 0x021      176B Heap:M */ CN_COMPILER,
  /*  34 0x022  1044008B Heap:L */ CN_CRWBUFFER,
  /*  35 0x023  1040784B Heap:L */ CN_CSVBUFFER,
  
  /*  36 0x024        4B Heap:I */ CN__D__,
  /*  37 0x025       80B Heap:S */ CN_DECACNT,
  /*  38 0x026       32B Heap:S */ CN_DECIDER,
  /*  39 0x027       88B Heap:M */ CN_DIC,
  /*  40 0x028      920B Heap:S */ CN_DICPATH,
  /*  41 0x029      152B Heap:S */ CN_DICSTACK,
  /*  42 0x02A      680B Heap:S */ CN_DIR,
  /*  43 0x02B     4072B Heap:M */ CN_DTMVAL,
  /*  44 0x02C       56B Heap:S */ CN_DYNAMO,
  
  /*  45 0x02D        5B Heap:I */ CN__E__,
  /*  46 0x02E       72B Heap:S */ CN_EARLYMARKER,
  /*  47 0x02F    27264B Heap:L */ CN_ENCODING,
  /*  48 0x030       80B Heap:S */ CN_ENDMARKER,
  /*  49 0x031     5584B Heap:M */ CN_ENMSET,
  /*  50 0x032       32B Heap:I */ CN_ENT,
  /*  51 0x033  1043952B Heap:L */ CN_ENTBUFFER,
  /*  52 0x034      304B Heap:I */ CN_ENTWRK,
  /*  53 0x035      312B Heap:S */ CN_ENUMFLAGS,
  
  /*  54 0x036        6B Heap:I */ CN__F__,
  /*  55 0x037  1040440B Heap:L */ CN_FETCHBUFFER,
  /*  56 0x038      384B Heap:S */ CN_FIFO,
  /*  57 0x039      448B Heap:S */ CN_FILE,
  /*  58 0x03A      136B Heap:M */ CN_FILTER_FETCH,
  /*  59 0x03B      136B Heap:M */ CN_FILTER_FROM,
  /*  60 0x03C      136B Heap:M */ CN_FILTER_GROUPBY,
  /*  61 0x03D      136B Heap:M */ CN_FILTER_INFLATE,
  /*  62 0x03E      136B Heap:M */ CN_FILTER_JOIN,
  /*  63 0x03F      136B Heap:M */ CN_FILTER_LIMIT,
  /*  64 0x040      136B Heap:M */ CN_FILTER_ORDERBY,
  /*  65 0x041      136B Heap:M */ CN_FILTER_OUTPUT,
  /*  66 0x042      136B Heap:M */ CN_FILTER_PROJECT,
  /*  67 0x043      136B Heap:M */ CN_FILTER_SCAN,
  /*  68 0x044      136B Heap:M */ CN_FILTER_SELECT,
  /*  69 0x045      136B Heap:M */ CN_FILTER_SUBQRY,
  /*  70 0x046      136B Heap:M */ CN_FILTER_WHERE,
  /*  71 0x047      136B Heap:M */ CN_FILTER_ZED,
  /*  72 0x048       16B Heap:I */ CN_FIPVAL,
  /*  73 0x049   524520B Heap:L */ CN_FOCALOBJECTS,
  /*  74 0x04A      256B Heap:S */ CN_FORTH,
  /*  75 0x04B       56B Heap:S */ CN_FORMATTERS,
  /*  76 0x04C       32B Heap:I */ CN_FREEBUFFER,
  /*  77 0x04D       24B Heap:I */ CN_FREECHUNK,
  /*  78 0x04E        0B Heap:I */ CN_FREEDNOT,
  /*  79 0x04F        0B Heap:I */ CN_FREEITEM,
  /*  80 0x050     2072B Heap:S */ CN_FREQDIST,
  
  /*  81 0x051        7B Heap:I */ CN__G__,
  /*  82 0x052    49168B Heap:I */ CN_GENVAR,
  /*  83 0x053     2184B Heap:L */ CN_GRID,
  
  /*  84 0x054        8B Heap:I */ CN__H__,
  /*  85 0x055  1044336B Heap:L */ CN_HASHBUFFER,
  /*  86 0x056       96B Heap:S */ CN_HASHMAP,
  /*  87 0x057       64B Heap:I */ CN_HEAPBUFFER,
  /*  88 0x058    20448B Heap:I */ CN_HPROW,
  
  /*  89 0x059        9B Heap:I */ CN__I__,
  /*  90 0x05A      184B Heap:I */ CN_INDEX,
  /*  91 0x05B     4168B Heap:M */ CN_INDEXDEF,
  /*  92 0x05C      232B Heap:S */ CN_INDEXTREE,
  /*  93 0x05D     8224B Heap:M */ CN_INDEXTREEVEC,
  /*  94 0x05E       24B Heap:M */ CN_INPUT,
  /*  95 0x05F     8216B Heap:M */ CN_IDXCOLVEC,
  /*  96 0x060     8224B Heap:M */ CN_IDXVEC,
  /*  97 0x061  1000088B Heap:L */ CN_INGESTCHUNK,
  /*  98 0x062     1192B Heap:L */ CN_INTBUFFER,
  /*  99 0x063       16B Heap:I */ CN_INTVAL,
  /* 100 0x064  1000512B Heap:L */ CN_IOCHUNK,
  /* 101 0x065       32B Heap:S */ CN_IOCOPY,
  /* 102 0x066  1043864B Heap:L */ CN_IRWBUFFER,
  
  /* 103 0x067       10B Heap:I */ CN__J__,
  /* 104 0x068    14224B Heap:L */ CN_JOB__FIND,
  /* 105 0x069    14224B Heap:L */ CN_JOB_BOOT,
  /* 106 0x06A   145384B Heap:L */ CN_JOB_COPYBUFFER,
  /* 107 0x06B    14888B Heap:L */ CN_JOB_FETCHROWS,
  /* 108 0x06C    14248B Heap:L */ CN_JOB_INSERTROW,
  /* 109 0x06D    14240B Heap:L */ CN_JOB_INSERTTABLE,
  /* 110 0x06E    14576B Heap:L */ CN_JOB_LOADTABLE,
  /* 111 0x06F    14280B Heap:L */ CN_JOB_NODEINFO,
  /* 112 0x070    14264B Heap:L */ CN_JOB_OPERAND,
  /* 113 0x071    14240B Heap:L */ CN_JOB_RECEIVER,
  /* 114 0x072    14240B Heap:L */ CN_JOB_SANITY,
  /* 115 0x073    14272B Heap:L */ CN_JOB_SELECT,
  /* 116 0x074    14224B Heap:L */ CN_JOB_START,
  /* 117 0x075    14256B Heap:L */ CN_JOB_SUBFETCH,
  /* 118 0x076    14256B Heap:L */ CN_JOB_SUBINFLATE,
  /* 119 0x077    14256B Heap:L */ CN_JOB_SUBINGESTER,
  /* 120 0x078   407784B Heap:L */ CN_JOB_SUBINSERTER,
  /* 121 0x079   407544B Heap:L */ CN_JOB_SUBLOADER,
  /* 122 0x07A    14232B Heap:L */ CN_JOB_SUBTRUNCATE,
  /* 123 0x07B    14224B Heap:L */ CN_JOB_UNIVERSAL,
  /* 124 0x07C    14280B Heap:L */ CN_JOB_WRITEBUFFER,
  /* 125 0x07D   145392B Heap:L */ CN_JOB_WRITEROW,
  /* 126 0x07E      224B Heap:S */ CN_JOBLIST,
  /* 127 0x07F      296B Heap:I */ CN_JUMBOPERSISTENT,
  /* 128 0x080      296B Heap:I */ CN_JUMBOTRANSIENT,
  
  /* 129 0x081       11B Heap:I */ CN__K__,
  /* 130 0x082        8B Heap:I */ CN_KEY,
  
  /* 131 0x083       12B Heap:I */ CN__L__,
  /* 132 0x084      296B Heap:I */ CN_LARGEPERSISTENT,
  /* 133 0x085      296B Heap:I */ CN_LARGETRANSIENT,
  /* 134 0x086       80B Heap:S */ CN_LEXICON,
  /* 135 0x087  1000512B Heap:L */ CN_LOADCHUNK,
  /* 136 0x088  9389360B Heap:J */ CN_LOADMAP,
  /* 137 0x089     8208B Heap:I */ CN_LOCAL,
  /* 138 0x08A   262688B Heap:L */ CN_LOCALSCHAIN,
  /* 139 0x08B      192B Heap:I */ CN_LOCK,
  /* 140 0x08C 325632272B Heap:I */ CN_LOG,
  /* 141 0x08D     1272B Heap:I */ CN_LOGENTRY,
  
  /* 142 0x08E       13B Heap:I */ CN__M__,
  /* 143 0x08F      296B Heap:I */ CN_MEDIUMPERSISTENT,
  /* 144 0x090      296B Heap:I */ CN_MEDIUMTRANSIENT,
  /* 145 0x091      136B Heap:S */ CN_MEMTEST,
  /* 146 0x092      480B Heap:S */ CN_MUTEX,
  
  /* 147 0x093       14B Heap:I */ CN__N__,
  /* 148 0x094       40B Heap:I */ CN_NEXTID,
  /* 149 0x095     4112B Heap:M */ CN_NMSTR,
  /* 150 0x096      248B Heap:S */ CN_NMSTRLIST,
  /* 151 0x097       80B Heap:S */ CN_NMVAL,
  /* 152 0x098      216B Heap:S */ CN_NODE,
  /* 153 0x099       56B Heap:S */ CN_NOW,
  /* 154 0x09A   118856B Heap:L */ CN_NUMBAT,
  
  /* 155 0x09B       15B Heap:I */ CN__O__,
  /* 156 0x09C      464B Heap:S */ CN_OP,
  /* 157 0x09D      528B Heap:M */ CN_OP_TOCLAUSE,
  /* 158 0x09E     4496B Heap:M */ CN_OP_TOCOMMAND,
  /* 159 0x09F      472B Heap:M */ CN_OP_TODO,
  /* 160 0x0A0      568B Heap:M */ CN_OP_TOFILTER,
  /* 161 0x0A1       56B Heap:S */ CN_OPERANDQUEUE,
  /* 162 0x0A2     5696B Heap:M */ CN_OPSPEC,
  /* 163 0x0A3     2080B Heap:S */ CN_OPSPECVEC,
  /* 164 0x0A4     2080B Heap:S */ CN_OPVEC,
  
  /* 165 0x0A5       16B Heap:I */ CN__P__,
  /* 166 0x0A6    48840B Heap:M */ CN_PARSER,
  /* 167 0x0A7     1048B Heap:M */ CN_PATHVEC,
  /* 168 0x0A8       72B Heap:M */ CN_PAUSEMARKER,
  /* 169 0x0A9      232B Heap:M */ CN_PERUSE,
  /* 170 0x0AA      384B Heap:S */ CN_PIPE,
  /* 171 0x0AB     2136B Heap:M */ CN_PIPELINE,
  /* 172 0x0AC       80B Heap:S */ CN_PIPELINESPEC,
  /* 173 0x0AD    48224B Heap:M */ CN_PLAN,
  /* 174 0x0AE     3112B Heap:M */ CN_PLANNER,
  /* 175 0x0AF       48B Heap:S */ CN_PREDICATE,
  /* 176 0x0B0      184B Heap:M */ CN_PREDICATEVEC,
  /* 177 0x0B1      104B Heap:S */ CN_PSXTHD,
  /* 178 0x0B2     8216B Heap:M */ CN_PTNCOLVEC,
  /* 179 0x0B3       16B Heap:I */ CN_PTRVAL,
  
  /* 180 0x0B4       17B Heap:I */ CN__Q__,
  /* 181 0x0B5      232B Heap:S */ CN_QUEUE,
  /* 182 0x0B6       64B Heap:I */ CN_QUEUEBUFFER,
  /* 183 0x0B7      160B Heap:S */ CN_QUEUES,
  
  /* 184 0x0B8       18B Heap:I */ CN__R__,
  /* 185 0x0B9    16448B Heap:M */ CN_RECORD,
  /* 186 0x0BA  1043496B Heap:L */ CN_RECORDBUFFER,
  /* 187 0x0BB      184B Heap:S */ CN_REGSET,
  /* 188 0x0BC       24B Heap:S */ CN_RELOP,
  /* 189 0x0BD      176B Heap:M */ CN_REQUEST,
  /* 190 0x0BE      184B Heap:S */ CN_RESOURCETRACKER,
  /* 191 0x0BF     4128B Heap:M */ CN_RESPONSE,
  /* 192 0x0C0     1048B Heap:M */ CN_RESPONSEVEC,
  /* 193 0x0C1    17808B Heap:L */ CN_ROW,
  /* 194 0x0C2      848B Heap:M */ CN_ROWAGG,
  /* 195 0x0C3     2184B Heap:L */ CN_ROWCOMPACTOR,
  /* 196 0x0C4      216B Heap:S */ CN_ROWONCE,
  /* 197 0x0C5  1043608B Heap:L */ CN_ROWTRAY,
  
  /* 198 0x0C6       19B Heap:I */ CN__S__,
  /* 199 0x0C7       16B Heap:S */ CN_SCHEMA,
  /* 200 0x0C8    50336B Heap:L */ CN_SCHEMATEST,
  /* 201 0x0C9        8B Heap:I */ CN_SEEME,
  /* 202 0x0CA     5600B Heap:M */ CN_SEMANTIC,
  /* 203 0x0CB     6280B Heap:L */ CN_SEPROMPT,
  /* 204 0x0CC    52520B Heap:M */ CN_SESSION,
  /* 205 0x0CD     1248B Heap:L */ CN_SESSIONARRAY,
  /* 206 0x0CE     4096B Heap:M */ CN_SHORTCUT,
  /* 207 0x0CF      536B Heap:M */ CN_SHORTCUTVEC,
  /* 208 0x0D0       32B Heap:I */ CN_SINCE,
  /* 209 0x0D1       64B Heap:S */ CN_SKIPLIST,
  /* 210 0x0D2       32B Heap:S */ CN_SKIPNODE,
  /* 211 0x0D3  1040416B Heap:L */ CN_SLAB,
  /* 212 0x0D4       24B Heap:S */ CN_SLABALLOCATOR,
  /* 213 0x0D5       48B Heap:I */ CN_SLEEP,
  /* 214 0x0D6      128B Heap:I */ CN_SLOTS,
  /* 215 0x0D7      296B Heap:I */ CN_SMALLPERSISTENT,
  /* 216 0x0D8      296B Heap:I */ CN_SMALLTRANSIENT,
  /* 217 0x0D9       32B Heap:I */ CN_SNBCD,
  /* 218 0x0DA       32B Heap:I */ CN_SNBOL,
  /* 219 0x0DB       32B Heap:I */ CN_SNDTE,
  /* 220 0x0DC       32B Heap:I */ CN_SNFLT,
  /* 221 0x0DD       32B Heap:I */ CN_SNINT,
  /* 222 0x0DE       32B Heap:I */ CN_SNIVL,
  /* 223 0x0DF       32B Heap:I */ CN_SNTME,
  /* 224 0x0E0       32B Heap:I */ CN_SNTSP,
  /* 225 0x0E1     4128B Heap:I */ CN_SNVCH,
  /* 226 0x0E2      304B Heap:S */ CN_SOCKET,
  /* 227 0x0E3     4064B Heap:I */ CN_SPBCD,
  /* 228 0x0E4     4064B Heap:I */ CN_SPBOL,
  /* 229 0x0E5     4064B Heap:I */ CN_SPDTE,
  /* 230 0x0E6     4056B Heap:I */ CN_SPELAPSED,
  /* 231 0x0E7     4064B Heap:I */ CN_SPFLT,
  /* 232 0x0E8     4056B Heap:I */ CN_SPID,
  /* 233 0x0E9     4064B Heap:I */ CN_SPINT,
  /* 234 0x0EA     4064B Heap:I */ CN_SPIVL,
  /* 235 0x0EB     4056B Heap:I */ CN_SPSTRING,
  /* 236 0x0EC     4056B Heap:I */ CN_SPTKNVECSTR,
  /* 237 0x0ED     4064B Heap:I */ CN_SPTME,
  /* 238 0x0EE     4064B Heap:I */ CN_SPTSP,
  /* 239 0x0EF     8160B Heap:I */ CN_SPVCH,
  /* 240 0x0F0  1043888B Heap:L */ CN_SRTBUFFER,
  /* 241 0x0F1       24B Heap:I */ CN_SRTSET,
  /* 242 0x0F2  1043528B Heap:L */ CN_STACK,
  /* 243 0x0F3     6264B Heap:M */ CN_STAGE,
  /* 244 0x0F4     2112B Heap:S */ CN_STAGENDE,
  /* 245 0x0F5     2080B Heap:S */ CN_STAGENDEVEC,
  /* 246 0x0F6     2112B Heap:S */ CN_STAGEPTN,
  /* 247 0x0F7     2080B Heap:S */ CN_STAGEPTNVEC,
  /* 248 0x0F8      144B Heap:S */ CN_STAGESPEC,
  /* 249 0x0F9     2080B Heap:S */ CN_STAGESPECVEC,
  /* 250 0x0FA     2080B Heap:S */ CN_STAGEVEC,
  /* 251 0x0FB       72B Heap:S */ CN_STARTMARKER,
  /* 252 0x0FC    16840B Heap:I */ CN_STATISTICS,
  /* 253 0x0FD       64B Heap:I */ CN_STEPACCUM,
  /* 254 0x0FE   131288B Heap:L */ CN_STEPTIMING,
  /* 255 0x0FF       80B Heap:L */ CN_STMT_SELECT,
  /* 256 0x100    47800B Heap:L */ CN_STREAMDEF,
  /* 257 0x101       56B Heap:I */ CN_STRING_T,
  /* 258 0x102       24B Heap:I */ CN_STZVAL,
  /* 259 0x103       32B Heap:I */ CN_SYSCALL,
  
  /* 260 0x104       20B Heap:I */ CN__T__,
  /* 261 0x105     3416B Heap:I */ CN_TABLE,
  /* 262 0x106     4112B Heap:M */ CN_TABLEDEF,
  /* 263 0x107      296B Heap:S */ CN_TABLEPTN,
  /* 264 0x108     1048B Heap:M */ CN_TABLEPTNVEC,
  /* 265 0x109      432B Heap:M */ CN_TASK,
  /* 266 0x10A      256B Heap:S */ CN_TASKLIST,
  /* 267 0x10B      224B Heap:S */ CN_TASKQUEUE,
  /* 268 0x10C      296B Heap:S */ CN_TEMPNAME,
  /* 269 0x10D       16B Heap:S */ CN_TENANT,
  /* 270 0x10E       16B Heap:S */ CN_TENANTS,
  /* 271 0x10F      120B Heap:S */ CN_THREAD,
  /* 272 0x110      248B Heap:S */ CN_THREADQUEUE,
  /* 273 0x111     2272B Heap:M */ CN_THREADVEC,
  /* 274 0x112       72B Heap:S */ CN_TIMEMARKER,
  /* 275 0x113       32B Heap:S */ CN_TIMER,
  /* 276 0x114      304B Heap:S */ CN_TOKEN,
  /* 277 0x115    56856B Heap:M */ CN_TOKENISER,
  /* 278 0x116     8232B Heap:M */ CN_TOKENVEC,
  /* 279 0x117      432B Heap:S */ CN_TX,
  /* 280 0x118      240B Heap:S */ CN_TXSET,
  
  /* 281 0x119       21B Heap:I */ CN__U__,
  
  /* 282 0x11A       22B Heap:I */ CN__V__,
  
  /* 283 0x11B       23B Heap:I */ CN__W__,
  /* 284 0x11C       16B Heap:I */ CN_WILDERNESS,
  /* 285 0x11D    64808B Heap:M */ CN_WORD,
  
  /* 286 0x11E       24B Heap:I */ CN__X__,
  
  /* 287 0x11F       25B Heap:I */ CN__Y__,
  
  /* 288 0x120       26B Heap:I */ CN__Z__,
  /* 289 0x121       -1B Heap:I */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /*   0 0x000 */ "NONE"_Stz,                // CN_NONE
  
  /*   1 0x001 */ "_A__"_Stz,                // CN__A__
  /*   2 0x002 */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
  /*   3 0x003 */ "ARG"_Stz,                 // CN_ARG
  /*   4 0x004 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /*   5 0x005 */ "ARTICLE"_Stz,             // CN_ARTICLE
  /*   6 0x006 */ "ATOMIC"_Stz,              // CN_ATOMIC
  /*   7 0x007 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
  
  /*   8 0x008 */ "_B__"_Stz,                // CN__B__
  /*   9 0x009 */ "BIRTH"_Stz,               // CN_BIRTH
  /*  10 0x00A */ "BPA"_Stz,                 // CN_BPA
  /*  11 0x00B */ "BUCKETMAP"_Stz,           // CN_BUCKETMAP
  /*  12 0x00C */ "BUFFERCURSOR"_Stz,        // CN_BUFFERCURSOR
  /*  13 0x00D */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /*  14 0x00E */ "BYTERATE"_Stz,            // CN_BYTERATE
  /*  15 0x00F */ "BYTES_T"_Stz,             // CN_BYTES_T
  
  /*  16 0x010 */ "_C__"_Stz,                // CN__C__
  /*  17 0x011 */ "CALCULATOR"_Stz,          // CN_CALCULATOR
  /*  18 0x012 */ "CANCELMARKER"_Stz,        // CN_CANCELMARKER
  /*  19 0x013 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /*  20 0x014 */ "CATALOG"_Stz,             // CN_CATALOG
  /*  21 0x015 */ "CLAUSE_FROM"_Stz,         // CN_CLAUSE_FROM
  /*  22 0x016 */ "CLAUSE_GROUPBY"_Stz,      // CN_CLAUSE_GROUPBY
  /*  23 0x017 */ "CLAUSE_JOIN"_Stz,         // CN_CLAUSE_JOIN
  /*  24 0x018 */ "CLAUSE_LIMIT"_Stz,        // CN_CLAUSE_LIMIT
  /*  25 0x019 */ "CLAUSE_ORDERBY"_Stz,      // CN_CLAUSE_ORDERBY
  /*  26 0x01A */ "CLAUSE_SELECT"_Stz,       // CN_CLAUSE_SELECT
  /*  27 0x01B */ "CLAUSE_SUBQRY"_Stz,       // CN_CLAUSE_SUBQRY
  /*  28 0x01C */ "CLAUSE_WHERE"_Stz,        // CN_CLAUSE_WHERE
  /*  29 0x01D */ "CMD_SELECT"_Stz,          // CN_CMD_SELECT
  /*  30 0x01E */ "COLUMN"_Stz,              // CN_COLUMN
  /*  31 0x01F */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
  /*  32 0x020 */ "COMMAND"_Stz,             // CN_COMMAND
  /*  33 0x021 */ "COMPILER"_Stz,            // CN_COMPILER
  /*  34 0x022 */ "CRWBUFFER"_Stz,           // CN_CRWBUFFER
  /*  35 0x023 */ "CSVBUFFER"_Stz,           // CN_CSVBUFFER
  
  /*  36 0x024 */ "_D__"_Stz,                // CN__D__
  /*  37 0x025 */ "DECACNT"_Stz,             // CN_DECACNT
  /*  38 0x026 */ "DECIDER"_Stz,             // CN_DECIDER
  /*  39 0x027 */ "DIC"_Stz,                 // CN_DIC
  /*  40 0x028 */ "DICPATH"_Stz,             // CN_DICPATH
  /*  41 0x029 */ "DICSTACK"_Stz,            // CN_DICSTACK
  /*  42 0x02A */ "DIR"_Stz,                 // CN_DIR
  /*  43 0x02B */ "DTMVAL"_Stz,              // CN_DTMVAL
  /*  44 0x02C */ "DYNAMO"_Stz,              // CN_DYNAMO
  
  /*  45 0x02D */ "_E__"_Stz,                // CN__E__
  /*  46 0x02E */ "EARLYMARKER"_Stz,         // CN_EARLYMARKER
  /*  47 0x02F */ "ENCODING"_Stz,            // CN_ENCODING
  /*  48 0x030 */ "ENDMARKER"_Stz,           // CN_ENDMARKER
  /*  49 0x031 */ "ENMSET"_Stz,              // CN_ENMSET
  /*  50 0x032 */ "ENT"_Stz,                 // CN_ENT
  /*  51 0x033 */ "ENTBUFFER"_Stz,           // CN_ENTBUFFER
  /*  52 0x034 */ "ENTWRK"_Stz,              // CN_ENTWRK
  /*  53 0x035 */ "ENUMFLAGS"_Stz,           // CN_ENUMFLAGS
  
  /*  54 0x036 */ "_F__"_Stz,                // CN__F__
  /*  55 0x037 */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /*  56 0x038 */ "FIFO"_Stz,                // CN_FIFO
  /*  57 0x039 */ "FILE"_Stz,                // CN_FILE
  /*  58 0x03A */ "FILTER_FETCH"_Stz,        // CN_FILTER_FETCH
  /*  59 0x03B */ "FILTER_FROM"_Stz,         // CN_FILTER_FROM
  /*  60 0x03C */ "FILTER_GROUPBY"_Stz,      // CN_FILTER_GROUPBY
  /*  61 0x03D */ "FILTER_INFLATE"_Stz,      // CN_FILTER_INFLATE
  /*  62 0x03E */ "FILTER_JOIN"_Stz,         // CN_FILTER_JOIN
  /*  63 0x03F */ "FILTER_LIMIT"_Stz,        // CN_FILTER_LIMIT
  /*  64 0x040 */ "FILTER_ORDERBY"_Stz,      // CN_FILTER_ORDERBY
  /*  65 0x041 */ "FILTER_OUTPUT"_Stz,       // CN_FILTER_OUTPUT
  /*  66 0x042 */ "FILTER_PROJECT"_Stz,      // CN_FILTER_PROJECT
  /*  67 0x043 */ "FILTER_SCAN"_Stz,         // CN_FILTER_SCAN
  /*  68 0x044 */ "FILTER_SELECT"_Stz,       // CN_FILTER_SELECT
  /*  69 0x045 */ "FILTER_SUBQRY"_Stz,       // CN_FILTER_SUBQRY
  /*  70 0x046 */ "FILTER_WHERE"_Stz,        // CN_FILTER_WHERE
  /*  71 0x047 */ "FILTER_ZED"_Stz,          // CN_FILTER_ZED
  /*  72 0x048 */ "FIPVAL"_Stz,              // CN_FIPVAL
  /*  73 0x049 */ "FOCALOBJECTS"_Stz,        // CN_FOCALOBJECTS
  /*  74 0x04A */ "FORTH"_Stz,               // CN_FORTH
  /*  75 0x04B */ "FORMATTERS"_Stz,          // CN_FORMATTERS
  /*  76 0x04C */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /*  77 0x04D */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /*  78 0x04E */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /*  79 0x04F */ "FREEITEM"_Stz,            // CN_FREEITEM
  /*  80 0x050 */ "FREQDIST"_Stz,            // CN_FREQDIST
  
  /*  81 0x051 */ "_G__"_Stz,                // CN__G__
  /*  82 0x052 */ "GENVAR"_Stz,              // CN_GENVAR
  /*  83 0x053 */ "GRID"_Stz,                // CN_GRID
  
  /*  84 0x054 */ "_H__"_Stz,                // CN__H__
  /*  85 0x055 */ "HASHBUFFER"_Stz,          // CN_HASHBUFFER
  /*  86 0x056 */ "HASHMAP"_Stz,             // CN_HASHMAP
  /*  87 0x057 */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  /*  88 0x058 */ "HPROW"_Stz,               // CN_HPROW
  
  /*  89 0x059 */ "_I__"_Stz,                // CN__I__
  /*  90 0x05A */ "INDEX"_Stz,               // CN_INDEX
  /*  91 0x05B */ "INDEXDEF"_Stz,            // CN_INDEXDEF
  /*  92 0x05C */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /*  93 0x05D */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /*  94 0x05E */ "INPUT"_Stz,               // CN_INPUT
  /*  95 0x05F */ "IDXCOLVEC"_Stz,           // CN_IDXCOLVEC
  /*  96 0x060 */ "IDXVEC"_Stz,              // CN_IDXVEC
  /*  97 0x061 */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /*  98 0x062 */ "INTBUFFER"_Stz,           // CN_INTBUFFER
  /*  99 0x063 */ "INTVAL"_Stz,              // CN_INTVAL
  /* 100 0x064 */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /* 101 0x065 */ "IOCOPY"_Stz,              // CN_IOCOPY
  /* 102 0x066 */ "IRWBUFFER"_Stz,           // CN_IRWBUFFER
  
  /* 103 0x067 */ "_J__"_Stz,                // CN__J__
  /* 104 0x068 */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /* 105 0x069 */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
  /* 106 0x06A */ "JOB_COPYBUFFER"_Stz,      // CN_JOB_COPYBUFFER
  /* 107 0x06B */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
  /* 108 0x06C */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
  /* 109 0x06D */ "JOB_INSERTTABLE"_Stz,     // CN_JOB_INSERTTABLE
  /* 110 0x06E */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
  /* 111 0x06F */ "JOB_NODEINFO"_Stz,        // CN_JOB_NODEINFO
  /* 112 0x070 */ "JOB_OPERAND"_Stz,         // CN_JOB_OPERAND
  /* 113 0x071 */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
  /* 114 0x072 */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
  /* 115 0x073 */ "JOB_SELECT"_Stz,          // CN_JOB_SELECT
  /* 116 0x074 */ "JOB_START"_Stz,           // CN_JOB_START
  /* 117 0x075 */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
  /* 118 0x076 */ "JOB_SUBINFLATE"_Stz,      // CN_JOB_SUBINFLATE
  /* 119 0x077 */ "JOB_SUBINGESTER"_Stz,     // CN_JOB_SUBINGESTER
  /* 120 0x078 */ "JOB_SUBINSERTER"_Stz,     // CN_JOB_SUBINSERTER
  /* 121 0x079 */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
  /* 122 0x07A */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
  /* 123 0x07B */ "JOB_UNIVERSAL"_Stz,       // CN_JOB_UNIVERSAL
  /* 124 0x07C */ "JOB_WRITEBUFFER"_Stz,     // CN_JOB_WRITEBUFFER
  /* 125 0x07D */ "JOB_WRITEROW"_Stz,        // CN_JOB_WRITEROW
  /* 126 0x07E */ "JOBLIST"_Stz,             // CN_JOBLIST
  /* 127 0x07F */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
  /* 128 0x080 */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
  
  /* 129 0x081 */ "_K__"_Stz,                // CN__K__
  /* 130 0x082 */ "KEY"_Stz,                 // CN_KEY
  
  /* 131 0x083 */ "_L__"_Stz,                // CN__L__
  /* 132 0x084 */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* 133 0x085 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* 134 0x086 */ "LEXICON"_Stz,             // CN_LEXICON
  /* 135 0x087 */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* 136 0x088 */ "LOADMAP"_Stz,             // CN_LOADMAP
  /* 137 0x089 */ "LOCAL"_Stz,               // CN_LOCAL
  /* 138 0x08A */ "LOCALSCHAIN"_Stz,         // CN_LOCALSCHAIN
  /* 139 0x08B */ "LOCK"_Stz,                // CN_LOCK
  /* 140 0x08C */ "LOG"_Stz,                 // CN_LOG
  /* 141 0x08D */ "LOGENTRY"_Stz,            // CN_LOGENTRY
  
  /* 142 0x08E */ "_M__"_Stz,                // CN__M__
  /* 143 0x08F */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* 144 0x090 */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  /* 145 0x091 */ "MEMTEST"_Stz,             // CN_MEMTEST
  /* 146 0x092 */ "MUTEX"_Stz,               // CN_MUTEX
  
  /* 147 0x093 */ "_N__"_Stz,                // CN__N__
  /* 148 0x094 */ "NEXTID"_Stz,              // CN_NEXTID
  /* 149 0x095 */ "NMSTR"_Stz,               // CN_NMSTR
  /* 150 0x096 */ "NMSTRLIST"_Stz,           // CN_NMSTRLIST
  /* 151 0x097 */ "NMVAL"_Stz,               // CN_NMVAL
  /* 152 0x098 */ "NODE"_Stz,                // CN_NODE
  /* 153 0x099 */ "NOW"_Stz,                 // CN_NOW
  /* 154 0x09A */ "NUMBAT"_Stz,              // CN_NUMBAT
  
  /* 155 0x09B */ "_O__"_Stz,                // CN__O__
  /* 156 0x09C */ "OP"_Stz,                  // CN_OP
  /* 157 0x09D */ "OP_TOCLAUSE"_Stz,         // CN_OP_TOCLAUSE
  /* 158 0x09E */ "OP_TOCOMMAND"_Stz,        // CN_OP_TOCOMMAND
  /* 159 0x09F */ "OP_TODO"_Stz,             // CN_OP_TODO
  /* 160 0x0A0 */ "OP_TOFILTER"_Stz,         // CN_OP_TOFILTER
  /* 161 0x0A1 */ "OPERANDQUEUE"_Stz,        // CN_OPERANDQUEUE
  /* 162 0x0A2 */ "OPSPEC"_Stz,              // CN_OPSPEC
  /* 163 0x0A3 */ "OPSPECVEC"_Stz,           // CN_OPSPECVEC
  /* 164 0x0A4 */ "OPVEC"_Stz,               // CN_OPVEC
  
  /* 165 0x0A5 */ "_P__"_Stz,                // CN__P__
  /* 166 0x0A6 */ "PARSER"_Stz,              // CN_PARSER
  /* 167 0x0A7 */ "PATHVEC"_Stz,             // CN_PATHVEC
  /* 168 0x0A8 */ "PAUSEMARKER"_Stz,         // CN_PAUSEMARKER
  /* 169 0x0A9 */ "PERUSE"_Stz,              // CN_PERUSE
  /* 170 0x0AA */ "PIPE"_Stz,                // CN_PIPE
  /* 171 0x0AB */ "PIPELINE"_Stz,            // CN_PIPELINE
  /* 172 0x0AC */ "PIPELINESPEC"_Stz,        // CN_PIPELINESPEC
  /* 173 0x0AD */ "PLAN"_Stz,                // CN_PLAN
  /* 174 0x0AE */ "PLANNER"_Stz,             // CN_PLANNER
  /* 175 0x0AF */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* 176 0x0B0 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* 177 0x0B1 */ "PSXTHD"_Stz,              // CN_PSXTHD
  /* 178 0x0B2 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  /* 179 0x0B3 */ "PTRVAL"_Stz,              // CN_PTRVAL
  
  /* 180 0x0B4 */ "_Q__"_Stz,                // CN__Q__
  /* 181 0x0B5 */ "QUEUE"_Stz,               // CN_QUEUE
  /* 182 0x0B6 */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* 183 0x0B7 */ "QUEUES"_Stz,              // CN_QUEUES
  
  /* 184 0x0B8 */ "_R__"_Stz,                // CN__R__
  /* 185 0x0B9 */ "RECORD"_Stz,              // CN_RECORD
  /* 186 0x0BA */ "RECORDBUFFER"_Stz,        // CN_RECORDBUFFER
  /* 187 0x0BB */ "REGSET"_Stz,              // CN_REGSET
  /* 188 0x0BC */ "RELOP"_Stz,               // CN_RELOP
  /* 189 0x0BD */ "REQUEST"_Stz,             // CN_REQUEST
  /* 190 0x0BE */ "RESOURCETRACKER"_Stz,     // CN_RESOURCETRACKER
  /* 191 0x0BF */ "RESPONSE"_Stz,            // CN_RESPONSE
  /* 192 0x0C0 */ "RESPONSEVEC"_Stz,         // CN_RESPONSEVEC
  /* 193 0x0C1 */ "ROW"_Stz,                 // CN_ROW
  /* 194 0x0C2 */ "ROWAGG"_Stz,              // CN_ROWAGG
  /* 195 0x0C3 */ "ROWCOMPACTOR"_Stz,        // CN_ROWCOMPACTOR
  /* 196 0x0C4 */ "ROWONCE"_Stz,             // CN_ROWONCE
  /* 197 0x0C5 */ "ROWTRAY"_Stz,             // CN_ROWTRAY
  
  /* 198 0x0C6 */ "_S__"_Stz,                // CN__S__
  /* 199 0x0C7 */ "SCHEMA"_Stz,              // CN_SCHEMA
  /* 200 0x0C8 */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /* 201 0x0C9 */ "SEEME"_Stz,               // CN_SEEME
  /* 202 0x0CA */ "SEMANTIC"_Stz,            // CN_SEMANTIC
  /* 203 0x0CB */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* 204 0x0CC */ "SESSION"_Stz,             // CN_SESSION
  /* 205 0x0CD */ "SESSIONARRAY"_Stz,        // CN_SESSIONARRAY
  /* 206 0x0CE */ "SHORTCUT"_Stz,            // CN_SHORTCUT
  /* 207 0x0CF */ "SHORTCUTVEC"_Stz,         // CN_SHORTCUTVEC
  /* 208 0x0D0 */ "SINCE"_Stz,               // CN_SINCE
  /* 209 0x0D1 */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* 210 0x0D2 */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* 211 0x0D3 */ "SLAB"_Stz,                // CN_SLAB
  /* 212 0x0D4 */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* 213 0x0D5 */ "SLEEP"_Stz,               // CN_SLEEP
  /* 214 0x0D6 */ "SLOTS"_Stz,               // CN_SLOTS
  /* 215 0x0D7 */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* 216 0x0D8 */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* 217 0x0D9 */ "SNBCD"_Stz,               // CN_SNBCD
  /* 218 0x0DA */ "SNBOL"_Stz,               // CN_SNBOL
  /* 219 0x0DB */ "SNDTE"_Stz,               // CN_SNDTE
  /* 220 0x0DC */ "SNFLT"_Stz,               // CN_SNFLT
  /* 221 0x0DD */ "SNINT"_Stz,               // CN_SNINT
  /* 222 0x0DE */ "SNIVL"_Stz,               // CN_SNIVL
  /* 223 0x0DF */ "SNTME"_Stz,               // CN_SNTME
  /* 224 0x0E0 */ "SNTSP"_Stz,               // CN_SNTSP
  /* 225 0x0E1 */ "SNVCH"_Stz,               // CN_SNVCH
  /* 226 0x0E2 */ "SOCKET"_Stz,              // CN_SOCKET
  /* 227 0x0E3 */ "SPBCD"_Stz,               // CN_SPBCD
  /* 228 0x0E4 */ "SPBOL"_Stz,               // CN_SPBOL
  /* 229 0x0E5 */ "SPDTE"_Stz,               // CN_SPDTE
  /* 230 0x0E6 */ "SPELAPSED"_Stz,           // CN_SPELAPSED
  /* 231 0x0E7 */ "SPFLT"_Stz,               // CN_SPFLT
  /* 232 0x0E8 */ "SPID"_Stz,                // CN_SPID
  /* 233 0x0E9 */ "SPINT"_Stz,               // CN_SPINT
  /* 234 0x0EA */ "SPIVL"_Stz,               // CN_SPIVL
  /* 235 0x0EB */ "SPSTRING"_Stz,            // CN_SPSTRING
  /* 236 0x0EC */ "SPTKNVECSTR"_Stz,         // CN_SPTKNVECSTR
  /* 237 0x0ED */ "SPTME"_Stz,               // CN_SPTME
  /* 238 0x0EE */ "SPTSP"_Stz,               // CN_SPTSP
  /* 239 0x0EF */ "SPVCH"_Stz,               // CN_SPVCH
  /* 240 0x0F0 */ "SRTBUFFER"_Stz,           // CN_SRTBUFFER
  /* 241 0x0F1 */ "SRTSET"_Stz,              // CN_SRTSET
  /* 242 0x0F2 */ "STACK"_Stz,               // CN_STACK
  /* 243 0x0F3 */ "STAGE"_Stz,               // CN_STAGE
  /* 244 0x0F4 */ "STAGENDE"_Stz,            // CN_STAGENDE
  /* 245 0x0F5 */ "STAGENDEVEC"_Stz,         // CN_STAGENDEVEC
  /* 246 0x0F6 */ "STAGEPTN"_Stz,            // CN_STAGEPTN
  /* 247 0x0F7 */ "STAGEPTNVEC"_Stz,         // CN_STAGEPTNVEC
  /* 248 0x0F8 */ "STAGESPEC"_Stz,           // CN_STAGESPEC
  /* 249 0x0F9 */ "STAGESPECVEC"_Stz,        // CN_STAGESPECVEC
  /* 250 0x0FA */ "STAGEVEC"_Stz,            // CN_STAGEVEC
  /* 251 0x0FB */ "STARTMARKER"_Stz,         // CN_STARTMARKER
  /* 252 0x0FC */ "STATISTICS"_Stz,          // CN_STATISTICS
  /* 253 0x0FD */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* 254 0x0FE */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /* 255 0x0FF */ "STMT_SELECT"_Stz,         // CN_STMT_SELECT
  /* 256 0x100 */ "STREAMDEF"_Stz,           // CN_STREAMDEF
  /* 257 0x101 */ "STRING_T"_Stz,            // CN_STRING_T
  /* 258 0x102 */ "STZVAL"_Stz,              // CN_STZVAL
  /* 259 0x103 */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* 260 0x104 */ "_T__"_Stz,                // CN__T__
  /* 261 0x105 */ "TABLE"_Stz,               // CN_TABLE
  /* 262 0x106 */ "TABLEDEF"_Stz,            // CN_TABLEDEF
  /* 263 0x107 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* 264 0x108 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* 265 0x109 */ "TASK"_Stz,                // CN_TASK
  /* 266 0x10A */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* 267 0x10B */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /* 268 0x10C */ "TEMPNAME"_Stz,            // CN_TEMPNAME
  /* 269 0x10D */ "TENANT"_Stz,              // CN_TENANT
  /* 270 0x10E */ "TENANTS"_Stz,             // CN_TENANTS
  /* 271 0x10F */ "THREAD"_Stz,              // CN_THREAD
  /* 272 0x110 */ "THREADQUEUE"_Stz,         // CN_THREADQUEUE
  /* 273 0x111 */ "THREADVEC"_Stz,           // CN_THREADVEC
  /* 274 0x112 */ "TIMEMARKER"_Stz,          // CN_TIMEMARKER
  /* 275 0x113 */ "TIMER"_Stz,               // CN_TIMER
  /* 276 0x114 */ "TOKEN"_Stz,               // CN_TOKEN
  /* 277 0x115 */ "TOKENISER"_Stz,           // CN_TOKENISER
  /* 278 0x116 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* 279 0x117 */ "TX"_Stz,                  // CN_TX
  /* 280 0x118 */ "TXSET"_Stz,               // CN_TXSET
  
  /* 281 0x119 */ "_U__"_Stz,                // CN__U__
  
  /* 282 0x11A */ "_V__"_Stz,                // CN__V__
  
  /* 283 0x11B */ "_W__"_Stz,                // CN__W__
  /* 284 0x11C */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* 285 0x11D */ "WORD"_Stz,                // CN_WORD
  
  /* 286 0x11E */ "_X__"_Stz,                // CN__X__
  
  /* 287 0x11F */ "_Y__"_Stz,                // CN__Y__
  
  /* 288 0x120 */ "_Z__"_Stz,                // CN__Z__
  /* 289 0x121 */ "ENUMS"_Stz,               // CN_ENUMS
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
