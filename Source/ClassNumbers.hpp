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
  /*   3 0x003     2896B Heap:S */ CN_AGGOP,
  /*   4 0x004      304B Heap:S */ CN_ARG,
  /*   5 0x005    16584B Heap:I */ CN_ARGUMENTS,
  /*   6 0x006      384B Heap:S */ CN_ARTICLE,
  /*   7 0x007       24B Heap:I */ CN_ATOMIC,
  /*   8 0x008    28664B Heap:I */ CN_ATTACHMAP,
  
  /*   9 0x009        2B Heap:I */ CN__B__,
  /*  10 0x00A       32B Heap:I */ CN_BIRTH,
  /*  11 0x00B      480B Heap:S */ CN_BOTHOP,
  /*  12 0x00C        1B Heap:I */ CN_BPA,
  /*  13 0x00D     8400B Heap:L */ CN_BUCKETMAP,
  /*  14 0x00E       32B Heap:L */ CN_BUFFERCURSOR,
  /*  15 0x00F      384B Heap:I */ CN_BUFFERPOOL,
  /*  16 0x010      128B Heap:S */ CN_BYTERATE,
  /*  17 0x011       56B Heap:I */ CN_BYTES_T,
  
  /*  18 0x012        3B Heap:I */ CN__C__,
  /*  19 0x013    36880B Heap:M */ CN_CALCULATOR,
  /*  20 0x014       80B Heap:S */ CN_CANCELMARKER,
  /*  21 0x015       40B Heap:I */ CN_CARRYOVER,
  /*  22 0x016       16B Heap:I */ CN_CATALOG,
  /*  23 0x017      480B Heap:I */ CN_CHUNKFILEOP,
  /*  24 0x018      136B Heap:M */ CN_CLAUSE_FROM,
  /*  25 0x019      136B Heap:M */ CN_CLAUSE_GROUPBY,
  /*  26 0x01A      136B Heap:M */ CN_CLAUSE_JOIN,
  /*  27 0x01B      136B Heap:M */ CN_CLAUSE_LIMIT,
  /*  28 0x01C      136B Heap:M */ CN_CLAUSE_ORDERBY,
  /*  29 0x01D      136B Heap:M */ CN_CLAUSE_SELECT,
  /*  30 0x01E      136B Heap:M */ CN_CLAUSE_SUBQRY,
  /*  31 0x01F      136B Heap:M */ CN_CLAUSE_WHERE,
  /*  32 0x020     4096B Heap:M */ CN_CMD_SELECT,
  /*  33 0x021      120B Heap:S */ CN_COLUMN,
  /*  34 0x022     8216B Heap:M */ CN_COLUMNVEC,
  /*  35 0x023      480B Heap:S */ CN_COMBINEOP,
  /*  36 0x024      176B Heap:M */ CN_COMMAND,
  /*  37 0x025       64B Heap:M */ CN_COMPILER,
  /*  40 0x028  1044008B Heap:L */ CN_CRWBUFFER,
  /*  41 0x029  1040784B Heap:L */ CN_CSVBUFFER,
  
  /*  42 0x02A        4B Heap:I */ CN__D__,
  /*  43 0x02B       80B Heap:S */ CN_DECACNT,
  /*  44 0x02C       32B Heap:S */ CN_DECIDER,
  /*  45 0x02D      488B Heap:S */ CN_DELAYOP,
  /*  46 0x02E       88B Heap:M */ CN_DIC,
  /*  47 0x02F      920B Heap:S */ CN_DICPATH,
  /*  48 0x030      152B Heap:S */ CN_DICSTACK,
  /*  49 0x031      680B Heap:S */ CN_DIR,
  /*  50 0x032      480B Heap:S */ CN_DISTINCTOP,
  /*  51 0x033     4072B Heap:M */ CN_DTMVAL,
  /*  52 0x034       56B Heap:S */ CN_DYNAMO,
  
  /*  53 0x035        5B Heap:I */ CN__E__,
  /*  54 0x036       72B Heap:S */ CN_EARLYMARKER,
  /*  55 0x037    27264B Heap:L */ CN_ENCODING,
  /*  56 0x038       80B Heap:S */ CN_ENDMARKER,
  /*  57 0x039     5584B Heap:M */ CN_ENMSET,
  /*  58 0x03A       32B Heap:I */ CN_ENT,
  /*  59 0x03B  1043952B Heap:L */ CN_ENTBUFFER,
  /*  60 0x03C      304B Heap:I */ CN_ENTWRK,
  /*  61 0x03D      352B Heap:S */ CN_ENUMFLAGS,
  /*  62 0x03E     4112B Heap:M */ CN_EXPROP,
  
  /*  63 0x03F        6B Heap:I */ CN__F__,
  /*  64 0x040  1040440B Heap:L */ CN_FETCHBUFFER,
  /*  65 0x041     1088B Heap:S */ CN_FETCHOP,
  /*  66 0x042      384B Heap:S */ CN_FIFO,
  /*  67 0x043      448B Heap:S */ CN_FILE,
  /*  68 0x044      520B Heap:S */ CN_FILTEROP,
  /*  69 0x045      136B Heap:M */ CN_FILTER_FETCH,
  /*  70 0x046      136B Heap:M */ CN_FILTER_FROM,
  /*  71 0x047      136B Heap:M */ CN_FILTER_GROUPBY,
  /*  72 0x048      136B Heap:M */ CN_FILTER_INFLATE,
  /*  73 0x049      136B Heap:M */ CN_FILTER_JOIN,
  /*  74 0x04A      136B Heap:M */ CN_FILTER_LIMIT,
  /*  75 0x04B      136B Heap:M */ CN_FILTER_ORDERBY,
  /*  76 0x04C      136B Heap:M */ CN_FILTER_OUTPUT,
  /*  77 0x04D      136B Heap:M */ CN_FILTER_PROJECT,
  /*  78 0x04E      136B Heap:M */ CN_FILTER_SCAN,
  /*  79 0x04F      136B Heap:M */ CN_FILTER_SELECT,
  /*  80 0x050      136B Heap:M */ CN_FILTER_SUBQRY,
  /*  81 0x051      136B Heap:M */ CN_FILTER_WHERE,
  /*  82 0x052      136B Heap:M */ CN_FILTER_ZED,
  /*  83 0x053       16B Heap:I */ CN_FIPVAL,
  /*  84 0x054   524520B Heap:L */ CN_FOCALOBJECTS,
  /*  85 0x055      256B Heap:S */ CN_FORTH,
  /*  86 0x056       56B Heap:S */ CN_FORMATTERS,
  /*  87 0x057       32B Heap:I */ CN_FREEBUFFER,
  /*  88 0x058       24B Heap:I */ CN_FREECHUNK,
  /*  89 0x059        0B Heap:I */ CN_FREEDNOT,
  /*  90 0x05A        0B Heap:I */ CN_FREEITEM,
  /*  91 0x05B     2072B Heap:S */ CN_FREQDIST,
  
  /*  92 0x05C        7B Heap:I */ CN__G__,
  /*  93 0x05D      496B Heap:S */ CN_GENERATEOP,
  /*  94 0x05E    49168B Heap:I */ CN_GENVAR,
  /*  95 0x05F      784B Heap:S */ CN_GETKEYOP,
  /*  96 0x060     2184B Heap:L */ CN_GRID,
  
  /*  97 0x061        8B Heap:I */ CN__H__,
  /*  98 0x062  1044336B Heap:L */ CN_HASHBUFFER,
  /*  99 0x063       96B Heap:S */ CN_HASHMAP,
  /* 100 0x064      480B Heap:S */ CN_HASHOP,
  /* 101 0x065       64B Heap:I */ CN_HEAPBUFFER,
  /* 102 0x066    20448B Heap:I */ CN_HPROW,
  
  /* 103 0x067        9B Heap:I */ CN__I__,
  /* 104 0x068      184B Heap:I */ CN_INDEX,
  /* 105 0x069     4168B Heap:M */ CN_INDEXDEF,
  /* 106 0x06A      232B Heap:S */ CN_INDEXTREE,
  /* 107 0x06B     8224B Heap:M */ CN_INDEXTREEVEC,
  /* 108 0x06C      480B Heap:M */ CN_INFLATEOP,
  /* 109 0x06D       24B Heap:M */ CN_INPUT,
  /* 110 0x06E     8216B Heap:M */ CN_IDXCOLVEC,
  /* 111 0x06F     8224B Heap:M */ CN_IDXVEC,
  /* 112 0x070  1000088B Heap:L */ CN_INGESTCHUNK,
  /* 113 0x071      784B Heap:L */ CN_INSERTOP,
  /* 114 0x072     1192B Heap:L */ CN_INTBUFFER,
  /* 115 0x073      480B Heap:S */ CN_INTERSECTOP,
  /* 116 0x074       16B Heap:I */ CN_INTVAL,
  /* 117 0x075  1000512B Heap:L */ CN_IOCHUNK,
  /* 118 0x076       32B Heap:S */ CN_IOCOPY,
  /* 119 0x077  1043864B Heap:L */ CN_IRWBUFFER,
  
  /* 120 0x078       10B Heap:I */ CN__J__,
  /* 121 0x079    14224B Heap:L */ CN_JOB__FIND,
  /* 122 0x07A    14224B Heap:L */ CN_JOB_BOOT,
  /* 123 0x07B   145384B Heap:L */ CN_JOB_COPYBUFFER,
  /* 124 0x07C    14888B Heap:L */ CN_JOB_FETCHROWS,
  /* 125 0x07D    14248B Heap:L */ CN_JOB_INSERTROW,
  /* 126 0x07E    14240B Heap:L */ CN_JOB_INSERTTABLE,
  /* 127 0x07F    14576B Heap:L */ CN_JOB_LOADTABLE,
  /* 128 0x080    14280B Heap:L */ CN_JOB_NODEINFO,
  /* 129 0x081    14264B Heap:L */ CN_JOB_OPERAND,
  /* 130 0x082    14240B Heap:L */ CN_JOB_RECEIVER,
  /* 131 0x083    14240B Heap:L */ CN_JOB_SANITY,
  /* 132 0x084    14280B Heap:L */ CN_JOB_SELECT,
  /* 133 0x085    14224B Heap:L */ CN_JOB_START,
  /* 134 0x086    14256B Heap:L */ CN_JOB_SUBFETCH,
  /* 135 0x087    14256B Heap:L */ CN_JOB_SUBINFLATE,
  /* 136 0x088    14256B Heap:L */ CN_JOB_SUBINGESTER,
  /* 137 0x089   407784B Heap:L */ CN_JOB_SUBINSERTER,
  /* 138 0x08A   407544B Heap:L */ CN_JOB_SUBLOADER,
  /* 139 0x08B    14232B Heap:L */ CN_JOB_SUBTRUNCATE,
  /* 140 0x08C    14224B Heap:L */ CN_JOB_UNIVERSAL,
  /* 141 0x08D    14280B Heap:L */ CN_JOB_WRITEBUFFER,
  /* 142 0x08E   145392B Heap:L */ CN_JOB_WRITEROW,
  /* 143 0x08F      224B Heap:S */ CN_JOBLIST,
  /* 144 0x090      544B Heap:S */ CN_JOINOP,
  /* 145 0x091      296B Heap:I */ CN_JUMBOPERSISTENT,
  /* 146 0x092      296B Heap:I */ CN_JUMBOTRANSIENT,
  
  /* 147 0x093       11B Heap:I */ CN__K__,
  /* 148 0x094        8B Heap:I */ CN_KEY,
  
  /* 149 0x095       12B Heap:I */ CN__L__,
  /* 150 0x096      296B Heap:I */ CN_LARGEPERSISTENT,
  /* 151 0x097      296B Heap:I */ CN_LARGETRANSIENT,
  /* 152 0x098       80B Heap:S */ CN_LEXICON,
  /* 153 0x099      496B Heap:S */ CN_LIMITOP,
  /* 154 0x09A  1000512B Heap:L */ CN_LOADCHUNK,
  /* 155 0x09B  9389360B Heap:J */ CN_LOADMAP,
  /* 156 0x09C     8216B Heap:I */ CN_LOCAL,
  /* 157 0x09D   262944B Heap:L */ CN_LOCALSCHAIN,
  /* 158 0x09E      192B Heap:I */ CN_LOCK,
  /* 159 0x09F 325632272B Heap:I */ CN_LOG,
  /* 160 0x0A0     1272B Heap:I */ CN_LOGENTRY,
  
  /* 161 0x0A1       13B Heap:I */ CN__M__,
  /* 162 0x0A2      480B Heap:S */ CN_MARK2OP,
  /* 163 0x0A3      296B Heap:I */ CN_MEDIUMPERSISTENT,
  /* 164 0x0A4      296B Heap:I */ CN_MEDIUMTRANSIENT,
  /* 165 0x0A5      136B Heap:S */ CN_MEMTEST,
  /* 166 0x0A6      744B Heap:S */ CN_MERGEOP,
  /* 167 0x0A7      480B Heap:S */ CN_MUTEX,
  
  /* 168 0x0A8       14B Heap:I */ CN__N__,
  /* 169 0x0A9       40B Heap:I */ CN_NEXTID,
  /* 170 0x0AA     4112B Heap:M */ CN_NMSTR,
  /* 171 0x0AB      248B Heap:S */ CN_NMSTRLIST,
  /* 172 0x0AC       80B Heap:S */ CN_NMVAL,
  /* 173 0x0AD      216B Heap:S */ CN_NODE,
  /* 174 0x0AE       56B Heap:S */ CN_NOW,
  /* 175 0x0AF   118856B Heap:L */ CN_NUMBAT,
  
  /* 176 0x0B0       15B Heap:I */ CN__O__,
  /*                 Heap:S     */ CN_OP,
  /* 177 0x0B1      536B Heap:M */ CN_OP_TOCLAUSE,
  /* 178 0x0B2     4504B Heap:M */ CN_OP_TOCOMMAND,
  /* 179 0x0B3      480B Heap:M */ CN_OP_TODO,
  /* 180 0x0B4      576B Heap:M */ CN_OP_TOFILTER,
  /* 181 0x0B5       56B Heap:S */ CN_OPERANDQUEUE,
  /* 182 0x0B6     5696B Heap:M */ CN_OPSPEC,
  /* 183 0x0B7     2080B Heap:S */ CN_OPSPECVEC,
  /* 184 0x0B8     2080B Heap:S */ CN_OPVEC,
  
  /* 186 0x0BA       16B Heap:I */ CN__P__,
  /* 187 0x0BB    48840B Heap:M */ CN_PARSER,
  /* 188 0x0BC     1048B Heap:M */ CN_PATHVEC,
  /* 189 0x0BD       72B Heap:M */ CN_PAUSEMARKER,
  /* 190 0x0BE      232B Heap:M */ CN_PERUSE,
  /* 191 0x0BF      384B Heap:S */ CN_PIPE,
  /* 192 0x0C0     2136B Heap:M */ CN_PIPELINE,
  /* 193 0x0C1       80B Heap:S */ CN_PIPELINESPEC,
  /* 194 0x0C2    48224B Heap:M */ CN_PLAN,
  /* 195 0x0C3     3112B Heap:M */ CN_PLANNER,
  /* 196 0x0C4       48B Heap:S */ CN_PREDICATE,
  /* 197 0x0C5      184B Heap:M */ CN_PREDICATEVEC,
  /* 198 0x0C6     2896B Heap:S */ CN_PROJECTOP,
  /* 199 0x0C7      104B Heap:S */ CN_PSXTHD,
  /* 200 0x0C8     8216B Heap:M */ CN_PTNCOLVEC,
  /* 201 0x0C9       16B Heap:I */ CN_PTRVAL,
  
  /* 202 0x0CA       17B Heap:I */ CN__Q__,
  /* 203 0x0CB      232B Heap:S */ CN_QUEUE,
  /* 204 0x0CC       64B Heap:I */ CN_QUEUEBUFFER,
  /* 205 0x0CD      160B Heap:S */ CN_QUEUES,
  
  /* 206 0x0CE       18B Heap:I */ CN__R__,
  /* 207 0x0CF    16440B Heap:M */ CN_RECORD,
  /* 208 0x0D0  1043496B Heap:L */ CN_RECORDBUFFER,
  /* 209 0x0D1      480B Heap:L */ CN_REDUCEOP,
  /* 210 0x0D2  1043872B Heap:L */ CN_REFBUFFER,
  /* 211 0x0D3       24B Heap:I */ CN_REFSET,
  /* 212 0x0D4      176B Heap:S */ CN_REGSET,
  /* 213 0x0D5       24B Heap:S */ CN_RELOP,
  /* 214 0x0D6      176B Heap:M */ CN_REQUEST,
  /* 215 0x0D7      184B Heap:S */ CN_RESOURCETRACKER,
  /* 216 0x0D8     4128B Heap:M */ CN_RESPONSE,
  /* 217 0x0D9     1048B Heap:M */ CN_RESPONSEVEC,
  /* 218 0x0DA    17800B Heap:L */ CN_ROW,
  /* 219 0x0DB      848B Heap:M */ CN_ROWAGG,
  /* 220 0x0DC     2184B Heap:L */ CN_ROWCOMPACTOR,
  /* 221 0x0DD      216B Heap:S */ CN_ROWONCE,
  /* 222 0x0DE  1043608B Heap:L */ CN_ROWTRAY,
  
  /* 223 0x0DF       19B Heap:I */ CN__S__,
  /* 225 0x0E1       16B Heap:S */ CN_SCHEMA,
  /* 226 0x0E2    50336B Heap:L */ CN_SCHEMATEST,
  /* 227 0x0E3        8B Heap:I */ CN_SEEME,
  /* 228 0x0E4     5600B Heap:M */ CN_SEMANTIC,
  /* 229 0x0E5     6280B Heap:L */ CN_SEPROMPT,
  /* 230 0x0E6    52520B Heap:M */ CN_SESSION,
  /* 231 0x0E7     1248B Heap:L */ CN_SESSIONARRAY,
  /* 232 0x0E8     4096B Heap:M */ CN_SHORTCUT,
  /* 233 0x0E9      536B Heap:M */ CN_SHORTCUTVEC,
  /* 234 0x0EA       32B Heap:I */ CN_SINCE,
  /* 235 0x0EB       64B Heap:S */ CN_SKIPLIST,
  /* 236 0x0EC       32B Heap:S */ CN_SKIPNODE,
  /* 237 0x0ED  1040416B Heap:L */ CN_SLAB,
  /* 238 0x0EE       24B Heap:S */ CN_SLABALLOCATOR,
  /* 239 0x0EF       48B Heap:I */ CN_SLEEP,
  /* 240 0x0F0      128B Heap:I */ CN_SLOTS,
  /* 241 0x0F1      296B Heap:I */ CN_SMALLPERSISTENT,
  /* 242 0x0F2      296B Heap:I */ CN_SMALLTRANSIENT,
  /* 243 0x0F3       32B Heap:I */ CN_SNBCD,
  /* 244 0x0F4       32B Heap:I */ CN_SNBOL,
  /* 245 0x0F5       32B Heap:I */ CN_SNDTE,
  /* 246 0x0F6       32B Heap:I */ CN_SNFLT,
  /* 247 0x0F7       32B Heap:I */ CN_SNINT,
  /* 248 0x0F8       32B Heap:I */ CN_SNIVL,
  /* 249 0x0F9       32B Heap:I */ CN_SNTME,
  /* 250 0x0FA       32B Heap:I */ CN_SNTSP,
  /* 251 0x0FB     4128B Heap:I */ CN_SNVCH,
  /* 252 0x0FC      304B Heap:S */ CN_SOCKET,
  /* 253 0x0FD      480B Heap:S */ CN_SORTOP,
  /* 254 0x0FE     4064B Heap:I */ CN_SPBCD,
  /* 255 0x0FF     4064B Heap:I */ CN_SPBOL,
  /* 256 0x100     4064B Heap:I */ CN_SPDTE,
  /* 257 0x101     4056B Heap:I */ CN_SPELAPSED,
  /* 258 0x102     4064B Heap:I */ CN_SPFLT,
  /* 259 0x103     4056B Heap:I */ CN_SPID,
  /* 260 0x104     4064B Heap:I */ CN_SPINT,
  /* 261 0x105     4064B Heap:I */ CN_SPIVL,
  /* 262 0x106     4056B Heap:I */ CN_SPSTRING,
  /* 263 0x107     4056B Heap:I */ CN_SPTKNVECSTR,
  /* 264 0x108     4064B Heap:I */ CN_SPTME,
  /* 265 0x109     4064B Heap:I */ CN_SPTSP,
  /* 266 0x10A     8160B Heap:I */ CN_SPVCH,
  /* 267 0x10B  1043888B Heap:L */ CN_SRTBUFFER,
  /* 268 0x10C       24B Heap:I */ CN_SRTSET,
  /* 269 0x10D  1043528B Heap:L */ CN_STACK,
  /* 270 0x10E     6264B Heap:M */ CN_STAGE,
  /* 271 0x10F     2112B Heap:S */ CN_STAGENDE,
  /* 272 0x110     2080B Heap:S */ CN_STAGENDEVEC,
  /* 273 0x111     2112B Heap:S */ CN_STAGEPTN,
  /* 274 0x112     2080B Heap:S */ CN_STAGEPTNVEC,
  /* 275 0x113      144B Heap:S */ CN_STAGESPEC,
  /* 276 0x114     2080B Heap:S */ CN_STAGESPECVEC,
  /* 277 0x115     2080B Heap:S */ CN_STAGEVEC,
  /* 278 0x116       72B Heap:S */ CN_STARTMARKER,
  /* 280 0x118    19136B Heap:I */ CN_STATISTICS,
  /* 281 0x119       64B Heap:I */ CN_STEPACCUM,
  /* 282 0x11A   131288B Heap:L */ CN_STEPTIMING,
  /* 283 0x11B       80B Heap:L */ CN_STMT_SELECT,
  /* 284 0x11C    31408B Heap:L */ CN_STREAMDEF,
  /* 285 0x11D       56B Heap:I */ CN_STRING_T,
  /* 286 0x11E       24B Heap:I */ CN_STZVAL,
  /* 287 0x11F      488B Heap:S */ CN_SUPERAGGOP,
  /* 289 0x121     1520B Heap:S */ CN_SUPERMERGEOP,
  /* 290 0x122       32B Heap:I */ CN_SYSCALL,
  
  /* 291 0x123       20B Heap:I */ CN__T__,
  /* 292 0x124     3416B Heap:I */ CN_TABLE,
  /* 293 0x125     4120B Heap:M */ CN_TABLEDEF,
  /* 294 0x126      296B Heap:S */ CN_TABLEPTN,
  /* 295 0x127     1048B Heap:M */ CN_TABLEPTNVEC,
  /* 296 0x128      480B Heap:S */ CN_TAKEAWAYOP,
  /* 297 0x129      432B Heap:M */ CN_TASK,
  /* 298 0x12A      256B Heap:S */ CN_TASKLIST,
  /* 299 0x12B      224B Heap:S */ CN_TASKQUEUE,
  /* 300 0x12C      296B Heap:S */ CN_TEMPNAME,
  /* 301 0x12D       16B Heap:S */ CN_TENANT,
  /* 302 0x12E       16B Heap:S */ CN_TENANTS,
  /* 303 0x12F      120B Heap:S */ CN_THREAD,
  /* 304 0x130      248B Heap:S */ CN_THREADQUEUE,
  /* 305 0x131     2272B Heap:M */ CN_THREADVEC,
  /* 306 0x132       72B Heap:S */ CN_TIMEMARKER,
  /* 307 0x133       32B Heap:S */ CN_TIMER,
  /* 309 0x135      928B Heap:S */ CN_TOFILEOP,
  /* 310 0x136      304B Heap:S */ CN_TOKEN,
  /* 311 0x137    56856B Heap:M */ CN_TOKENISER,
  /* 312 0x138     8232B Heap:M */ CN_TOKENVEC,
  /* 313 0x139      432B Heap:S */ CN_TX,
  /* 314 0x13A      240B Heap:S */ CN_TXSET,
  
  /* 315 0x13B       21B Heap:I */ CN__U__,
  
  /* 316 0x13C       22B Heap:I */ CN__V__,
  
  /* 317 0x13D       23B Heap:I */ CN__W__,
  /* 318 0x13E      480B Heap:S */ CN_WHEREOP,
  /* 319 0x13F       16B Heap:I */ CN_WILDERNESS,
  /* 320 0x140    64808B Heap:M */ CN_WORD,
  
  /* 321 0x141       24B Heap:I */ CN__X__,
  
  /* 323 0x143       25B Heap:I */ CN__Y__,
  
  /* 324 0x144       26B Heap:I */ CN__Z__,
  /* 325 0x145      480B Heap:S */ CN_ZEDOP,

  /* 330 0x14A       -1B Heap:I */ CN_ENUMS,
};
  
/*----------------------------------------------------------------------------*/
/* class names                                                                */
/*----------------------------------------------------------------------------*/
static Stz* aCN[]
{
  /*   0 0x000 */ "NONE"_Stz,                // CN_NONE
  
  /*   1 0x001 */ "_A__"_Stz,                // CN__A__
  /*   2 0x002 */ "ACCESSPATH"_Stz,          // CN_ACCESSPATH
  /*   3 0x003 */ "AGGOP"_Stz,               // CN_AGGOP
  /*   4 0x004 */ "ARG"_Stz,                 // CN_ARG
  /*   5 0x005 */ "ARGUMENTS"_Stz,           // CN_ARGUMENTS
  /*   6 0x006 */ "ARTICLE"_Stz,             // CN_ARTICLE
  /*   7 0x007 */ "ATOMIC"_Stz,              // CN_ATOMIC
  /*   8 0x008 */ "ATTACHMAP"_Stz,           // CN_ATTACHMAP
  
  /*   9 0x009 */ "_B__"_Stz,                // CN__B__
  /*  10 0x00A */ "BIRTH"_Stz,               // CN_BIRTH
  /*  11 0x00B */ "BOTHOP"_Stz,              // CN_BOTHOP
  /*  12 0x00C */ "BPA"_Stz,                 // CN_BPA
  /*  13 0x00D */ "BUCKETMAP"_Stz,           // CN_BUCKETMAP
  /*  14 0x00E */ "BUFFERCURSOR"_Stz,        // CN_BUFFERCURSOR
  /*  15 0x00F */ "BUFFERPOOL"_Stz,          // CN_BUFFERPOOL
  /*  16 0x010 */ "BYTERATE"_Stz,            // CN_BYTERATE
  /*  17 0x011 */ "BYTES_T"_Stz,             // CN_BYTES_T
  
  /*  18 0x012 */ "_C__"_Stz,                // CN__C__
  /*  19 0x013 */ "CALCULATOR"_Stz,          // CN_CALCULATOR
  /*  20 0x014 */ "CANCELMARKER"_Stz,        // CN_CANCELMARKER
  /*  21 0x015 */ "CARRYOVER"_Stz,           // CN_CARRYOVER
  /*  22 0x016 */ "CATALOG"_Stz,             // CN_CATALOG
  /*  23 0x017 */ "CHUNKFILEOP"_Stz,         // CN_CHUNKFILEOP
  /*  24 0x018 */ "CLAUSE_FROM"_Stz,         // CN_CLAUSE_FROM
  /*  25 0x019 */ "CLAUSE_GROUPBY"_Stz,      // CN_CLAUSE_GROUPBY
  /*  26 0x01A */ "CLAUSE_JOIN"_Stz,         // CN_CLAUSE_JOIN
  /*  27 0x01B */ "CLAUSE_LIMIT"_Stz,        // CN_CLAUSE_LIMIT
  /*  28 0x01C */ "CLAUSE_ORDERBY"_Stz,      // CN_CLAUSE_ORDERBY
  /*  29 0x01D */ "CLAUSE_SELECT"_Stz,       // CN_CLAUSE_SELECT
  /*  30 0x01E */ "CLAUSE_SUBQRY"_Stz,       // CN_CLAUSE_SUBQRY
  /*  31 0x01F */ "CLAUSE_WHERE"_Stz,        // CN_CLAUSE_WHERE
  /*  32 0x020 */ "CMD_SELECT"_Stz,          // CN_CMD_SELECT
  /*  33 0x021 */ "COLUMN"_Stz,              // CN_COLUMN
  /*  34 0x022 */ "COLUMNVEC"_Stz,           // CN_COLUMNVEC
  /*  35 0x023 */ "COMBINEOP"_Stz,           // CN_COMBINEOP
  /*  36 0x024 */ "COMMAND"_Stz,             // CN_COMMAND
  /*  37 0x025 */ "COMPILER"_Stz,            // CN_COMPILER
  /*  40 0x028 */ "CRWBUFFER"_Stz,           // CN_CRWBUFFER
  /*  41 0x029 */ "CSVBUFFER"_Stz,           // CN_CSVBUFFER
  
  /*  42 0x02A */ "_D__"_Stz,                // CN__D__
  /*  43 0x02B */ "DECACNT"_Stz,             // CN_DECACNT
  /*  44 0x02C */ "DECIDER"_Stz,             // CN_DECIDER
  /*  45 0x02D */ "DELAYOP"_Stz,             // CN_DELAYOP
  /*  46 0x02E */ "DIC"_Stz,                 // CN_DIC
  /*  47 0x02F */ "DICPATH"_Stz,             // CN_DICPATH
  /*  48 0x030 */ "DICSTACK"_Stz,            // CN_DICSTACK
  /*  49 0x031 */ "DIR"_Stz,                 // CN_DIR
  /*  50 0x032 */ "DISTINCTOP"_Stz,          // CN_DISTINCTOP
  /*  51 0x033 */ "DTMVAL"_Stz,              // CN_DTMVAL
  /*  52 0x034 */ "DYNAMO"_Stz,              // CN_DYNAMO
  
  /*  53 0x035 */ "_E__"_Stz,                // CN__E__
  /*  54 0x036 */ "EARLYMARKER"_Stz,         // CN_EARLYMARKER
  /*  55 0x037 */ "ENCODING"_Stz,            // CN_ENCODING
  /*  56 0x038 */ "ENDMARKER"_Stz,           // CN_ENDMARKER
  /*  57 0x039 */ "ENMSET"_Stz,              // CN_ENMSET
  /*  58 0x03A */ "ENT"_Stz,                 // CN_ENT
  /*  59 0x03B */ "ENTBUFFER"_Stz,           // CN_ENTBUFFER
  /*  60 0x03C */ "ENTWRK"_Stz,              // CN_ENTWRK
  /*  61 0x03D */ "ENUMFLAGS"_Stz,           // CN_ENUMFLAGS
  /*  62 0x03E */ "EXPROP"_Stz,              // CN_EXPROP
  
  /*  63 0x03F */ "_F__"_Stz,                // CN__F__
  /*  64 0x040 */ "FETCHBUFFER"_Stz,         // CN_FETCHBUFFER
  /*  65 0x041 */ "FETCHOP"_Stz,             // CN_FETCHOP
  /*  66 0x042 */ "FIFO"_Stz,                // CN_FIFO
  /*  67 0x043 */ "FILE"_Stz,                // CN_FILE
  /*  68 0x044 */ "FILTEROP"_Stz,            // CN_FILTEROP
  /*  69 0x045 */ "FILTER_FETCH"_Stz,        // CN_FILTER_FETCH
  /*  70 0x046 */ "FILTER_FROM"_Stz,         // CN_FILTER_FROM
  /*  71 0x047 */ "FILTER_GROUPBY"_Stz,      // CN_FILTER_GROUPBY
  /*  72 0x048 */ "FILTER_INFLATE"_Stz,      // CN_FILTER_INFLATE
  /*  73 0x049 */ "FILTER_JOIN"_Stz,         // CN_FILTER_JOIN
  /*  74 0x04A */ "FILTER_LIMIT"_Stz,        // CN_FILTER_LIMIT
  /*  75 0x04B */ "FILTER_ORDERBY"_Stz,      // CN_FILTER_ORDERBY
  /*  76 0x04C */ "FILTER_OUTPUT"_Stz,       // CN_FILTER_OUTPUT
  /*  77 0x04D */ "FILTER_PROJECT"_Stz,      // CN_FILTER_PROJECT
  /*  78 0x04E */ "FILTER_SCAN"_Stz,         // CN_FILTER_SCAN
  /*  79 0x04F */ "FILTER_SELECT"_Stz,       // CN_FILTER_SELECT
  /*  80 0x050 */ "FILTER_SUBQRY"_Stz,       // CN_FILTER_SUBQRY
  /*  81 0x051 */ "FILTER_WHERE"_Stz,        // CN_FILTER_WHERE
  /*  82 0x052 */ "FILTER_ZED"_Stz,          // CN_FILTER_ZED
  /*  83 0x053 */ "FIPVAL"_Stz,              // CN_FIPVAL
  /*  84 0x054 */ "FOCALOBJECTS"_Stz,        // CN_FOCALOBJECTS
  /*  85 0x055 */ "FORTH"_Stz,               // CN_FORTH
  /*  86 0x056 */ "FORMATTERS"_Stz,          // CN_FORMATTERS
  /*  87 0x057 */ "FREEBUFFER"_Stz,          // CN_FREEBUFFER
  /*  88 0x058 */ "FREECHUNK"_Stz,           // CN_FREECHUNK
  /*  89 0x059 */ "FREEDNOT"_Stz,            // CN_FREEDNOT
  /*  90 0x05A */ "FREEITEM"_Stz,            // CN_FREEITEM
  /*  91 0x05B */ "FREQDIST"_Stz,            // CN_FREQDIST
  
  /*  92 0x05C */ "_G__"_Stz,                // CN__G__
  /*  93 0x05D */ "GENERATEOP"_Stz,          // CN_GENERATEOP
  /*  94 0x05E */ "GENVAR"_Stz,              // CN_GENVAR
  /*  95 0x05F */ "GETKEYOP"_Stz,            // CN_GETKEYOP
  /*  96 0x060 */ "GRID"_Stz,                // CN_GRID
  
  /*  97 0x061 */ "_H__"_Stz,                // CN__H__
  /*  98 0x062 */ "HASHBUFFER"_Stz,          // CN_HASHBUFFER
  /*  99 0x063 */ "HASHMAP"_Stz,             // CN_HASHMAP
  /* 100 0x064 */ "HASHOP"_Stz,              // CN_HASHOP
  /* 101 0x065 */ "HEAPBUFFER"_Stz,          // CN_HEAPBUFFER
  /* 102 0x066 */ "HPROW"_Stz,               // CN_HPROW
  
  /* 103 0x067 */ "_I__"_Stz,                // CN__I__
  /* 104 0x068 */ "INDEX"_Stz,               // CN_INDEX
  /* 105 0x069 */ "INDEXDEF"_Stz,            // CN_INDEXDEF
  /* 106 0x06A */ "INDEXTREE"_Stz,           // CN_INDEXTREE
  /* 107 0x06B */ "INDEXTREEVEC"_Stz,        // CN_INDEXTREEVEC
  /* 108 0x06C */ "INFLATEOP"_Stz,           // CN_INFLATEOP
  /* 109 0x06D */ "INPUT"_Stz,               // CN_INPUT
  /* 110 0x06E */ "IDXCOLVEC"_Stz,           // CN_IDXCOLVEC
  /* 111 0x06F */ "IDXVEC"_Stz,              // CN_IDXVEC
  /* 112 0x070 */ "INGESTCHUNK"_Stz,         // CN_INGESTCHUNK
  /* 113 0x071 */ "INSERTOP"_Stz,            // CN_INSERTOP
  /* 114 0x072 */ "INTBUFFER"_Stz,           // CN_INTBUFFER
  /* 115 0x073 */ "INTERSECTOP"_Stz,         // CN_INTERSECTOP
  /* 116 0x074 */ "INTVAL"_Stz,              // CN_INTVAL
  /* 117 0x075 */ "IOCHUNK"_Stz,             // CN_IOCHUNK
  /* 118 0x076 */ "IOCOPY"_Stz,              // CN_IOCOPY
  /* 119 0x077 */ "IRWBUFFER"_Stz,           // CN_IRWBUFFER
  
  /* 120 0x078 */ "_J__"_Stz,                // CN__J__
  /* 121 0x079 */ "JOB__FIND"_Stz,           // CN_JOB__FIND
  /* 122 0x07A */ "JOB_BOOT"_Stz,            // CN_JOB_BOOT
  /* 123 0x07B */ "JOB_COPYBUFFER"_Stz,      // CN_JOB_COPYBUFFER
  /* 124 0x07C */ "JOB_FETCHROWS"_Stz,       // CN_JOB_FETCHROWS
  /* 125 0x07D */ "JOB_INSERTROW"_Stz,       // CN_JOB_INSERTROW
  /* 126 0x07E */ "JOB_INSERTTABLE"_Stz,     // CN_JOB_INSERTTABLE
  /* 127 0x07F */ "JOB_LOADTABLE"_Stz,       // CN_JOB_LOADTABLE
  /* 128 0x080 */ "JOB_NODEINFO"_Stz,        // CN_JOB_NODEINFO
  /* 129 0x081 */ "JOB_OPERAND"_Stz,         // CN_JOB_OPERAND
  /* 130 0x082 */ "JOB_RECEIVER"_Stz,        // CN_JOB_RECEIVER
  /* 131 0x083 */ "JOB_SANITY"_Stz,          // CN_JOB_SANITY
  /* 132 0x084 */ "JOB_SELECT"_Stz,          // CN_JOB_SELECT
  /* 133 0x085 */ "JOB_START"_Stz,           // CN_JOB_START
  /* 134 0x086 */ "JOB_SUBFETCH"_Stz,        // CN_JOB_SUBFETCH
  /* 135 0x087 */ "JOB_SUBINFLATE"_Stz,      // CN_JOB_SUBINFLATE
  /* 136 0x088 */ "JOB_SUBINGESTER"_Stz,     // CN_JOB_SUBINGESTER
  /* 137 0x089 */ "JOB_SUBINSERTER"_Stz,     // CN_JOB_SUBINSERTER
  /* 138 0x08A */ "JOB_SUBLOADER"_Stz,       // CN_JOB_SUBLOADER
  /* 139 0x08B */ "JOB_SUBTRUNCATE"_Stz,     // CN_JOB_SUBTRUNCATE
  /* 140 0x08C */ "JOB_UNIVERSAL"_Stz,       // CN_JOB_UNIVERSAL
  /* 141 0x08D */ "JOB_WRITEBUFFER"_Stz,     // CN_JOB_WRITEBUFFER
  /* 142 0x08E */ "JOB_WRITEROW"_Stz,        // CN_JOB_WRITEROW
  /* 143 0x08F */ "JOBLIST"_Stz,             // CN_JOBLIST
  /* 144 0x090 */ "JOINOP"_Stz,              // CN_JOINOP
  /* 145 0x091 */ "JUMBOPERSISTENT"_Stz,     // CN_JUMBOPERSISTENT
  /* 146 0x092 */ "JUMBOTRANSIENT"_Stz,      // CN_JUMBOTRANSIENT
  
  /* 147 0x093 */ "_K__"_Stz,                // CN__K__
  /* 148 0x094 */ "KEY"_Stz,                 // CN_KEY
  
  /* 149 0x095 */ "_L__"_Stz,                // CN__L__
  /* 150 0x096 */ "LARGEPERSISTENT"_Stz,     // CN_LARGEPERSISTENT
  /* 151 0x097 */ "LARGETRANSIENT"_Stz,      // CN_LARGETRANSIENT
  /* 152 0x098 */ "LEXICON"_Stz,             // CN_LEXICON
  /* 153 0x099 */ "LIMITOP"_Stz,             // CN_LIMITOP
  /* 154 0x09A */ "LOADCHUNK"_Stz,           // CN_LOADCHUNK
  /* 155 0x09B */ "LOADMAP"_Stz,             // CN_LOADMAP
  /* 156 0x09C */ "LOCAL"_Stz,               // CN_LOCAL
  /* 157 0x09D */ "LOCALSCHAIN"_Stz,         // CN_LOCALSCHAIN
  /* 158 0x09E */ "LOCK"_Stz,                // CN_LOCK
  /* 159 0x09F */ "LOG"_Stz,                 // CN_LOG
  /* 160 0x0A0 */ "LOGENTRY"_Stz,            // CN_LOGENTRY
  
  /* 161 0x0A1 */ "_M__"_Stz,                // CN__M__
  /* 162 0x0A2 */ "MARK2OP"_Stz,             // CN_MARK2OP
  /* 163 0x0A3 */ "MEDIUMPERSISTENT"_Stz,    // CN_MEDIUMPERSISTENT
  /* 164 0x0A4 */ "MEDIUMTRANSIENT"_Stz,     // CN_MEDIUMTRANSIENT
  /* 165 0x0A5 */ "MEMTEST"_Stz,             // CN_MEMTEST
  /* 166 0x0A6 */ "MERGEOP"_Stz,             // CN_MERGEOP
  /* 167 0x0A7 */ "MUTEX"_Stz,               // CN_MUTEX
  
  /* 168 0x0A8 */ "_N__"_Stz,                // CN__N__
  /* 169 0x0A9 */ "NEXTID"_Stz,              // CN_NEXTID
  /* 170 0x0AA */ "NMSTR"_Stz,               // CN_NMSTR
  /* 171 0x0AB */ "NMSTRLIST"_Stz,           // CN_NMSTRLIST
  /* 172 0x0AC */ "NMVAL"_Stz,               // CN_NMVAL
  /* 173 0x0AD */ "NODE"_Stz,                // CN_NODE
  /* 174 0x0AE */ "NOW"_Stz,                 // CN_NOW
  /* 175 0x0AF */ "NUMBAT"_Stz,              // CN_NUMBAT
  
  /* 176 0x0B0 */ "_O__"_Stz,                // CN__O__
  /*           */ "OP"_Stz,                  // CN_OP
  /* 177 0x0B1 */ "OP_TOCLAUSE"_Stz,         // CN_OP_TOCLAUSE
  /* 178 0x0B2 */ "OP_TOCOMMAND"_Stz,        // CN_OP_TOCOMMAND
  /* 179 0x0B3 */ "OP_TODO"_Stz,             // CN_OP_TODO
  /* 180 0x0B4 */ "OP_TOFILTER"_Stz,         // CN_OP_TOFILTER
  /* 181 0x0B5 */ "OPERANDQUEUE"_Stz,        // CN_OPERANDQUEUE
  /* 182 0x0B6 */ "OPSPEC"_Stz,              // CN_OPSPEC
  /* 183 0x0B7 */ "OPSPECVEC"_Stz,           // CN_OPSPECVEC
  /* 184 0x0B8 */ "OPVEC"_Stz,               // CN_OPVEC
  
  /* 186 0x0BA */ "_P__"_Stz,                // CN__P__
  /* 187 0x0BB */ "PARSER"_Stz,              // CN_PARSER
  /* 188 0x0BC */ "PATHVEC"_Stz,             // CN_PATHVEC
  /* 189 0x0BD */ "PAUSEMARKER"_Stz,         // CN_PAUSEMARKER
  /* 190 0x0BE */ "PERUSE"_Stz,              // CN_PERUSE
  /* 191 0x0BF */ "PIPE"_Stz,                // CN_PIPE
  /* 192 0x0C0 */ "PIPELINE"_Stz,            // CN_PIPELINE
  /* 193 0x0C1 */ "PIPELINESPEC"_Stz,        // CN_PIPELINESPEC
  /* 194 0x0C2 */ "PLAN"_Stz,                // CN_PLAN
  /* 195 0x0C3 */ "PLANNER"_Stz,             // CN_PLANNER
  /* 196 0x0C4 */ "PREDICATE"_Stz,           // CN_PREDICATE
  /* 197 0x0C5 */ "PREDICATEVEC"_Stz,        // CN_PREDICATEVEC
  /* 198 0x0C6 */ "PROJECTOP"_Stz,           // CN_PROJECTOP
  /* 199 0x0C7 */ "PSXTHD"_Stz,              // CN_PSXTHD
  /* 200 0x0C8 */ "PTNCOLVEC"_Stz,           // CN_PTNCOLVEC
  /* 201 0x0C9 */ "PTRVAL"_Stz,              // CN_PTRVAL
  
  /* 202 0x0CA */ "_Q__"_Stz,                // CN__Q__
  /* 203 0x0CB */ "QUEUE"_Stz,               // CN_QUEUE
  /* 204 0x0CC */ "QUEUEBUFFER"_Stz,         // CN_QUEUEBUFFER
  /* 205 0x0CD */ "QUEUES"_Stz,              // CN_QUEUES
  
  /* 206 0x0CE */ "_R__"_Stz,                // CN__R__
  /* 207 0x0CF */ "RECORD"_Stz,              // CN_RECORD
  /* 208 0x0D0 */ "RECORDBUFFER"_Stz,        // CN_RECORDBUFFER
  /* 209 0x0D1 */ "REDUCEOP"_Stz,            // CN_REDUCEOP
  /* 210 0x0D2 */ "REFBUFFER"_Stz,           // CN_REFBUFFER
  /* 211 0x0D3 */ "REFSET"_Stz,              // CN_REFSET
  /* 212 0x0D4 */ "REGSET"_Stz,              // CN_REGSET
  /* 213 0x0D5 */ "RELOP"_Stz,               // CN_RELOP
  /* 214 0x0D6 */ "REQUEST"_Stz,             // CN_REQUEST
  /* 215 0x0D7 */ "RESOURCETRACKER"_Stz,     // CN_RESOURCETRACKER
  /* 216 0x0D8 */ "RESPONSE"_Stz,            // CN_RESPONSE
  /* 217 0x0D9 */ "RESPONSEVEC"_Stz,         // CN_RESPONSEVEC
  /* 218 0x0DA */ "ROW"_Stz,                 // CN_ROW
  /* 219 0x0DB */ "ROWAGG"_Stz,              // CN_ROWAGG
  /* 220 0x0DC */ "ROWCOMPACTOR"_Stz,        // CN_ROWCOMPACTOR
  /* 221 0x0DD */ "ROWONCE"_Stz,             // CN_ROWONCE
  /* 222 0x0DE */ "ROWTRAY"_Stz,             // CN_ROWTRAY
  
  /* 223 0x0DF */ "_S__"_Stz,                // CN__S__
  /* 225 0x0E1 */ "SCHEMA"_Stz,              // CN_SCHEMA
  /* 226 0x0E2 */ "SCHEMATEST"_Stz,          // CN_SCHEMATEST
  /* 227 0x0E3 */ "SEEME"_Stz,               // CN_SEEME
  /* 228 0x0E4 */ "SEMANTIC"_Stz,            // CN_SEMANTIC
  /* 229 0x0E5 */ "SEPROMPT"_Stz,            // CN_SEPROMPT
  /* 230 0x0E6 */ "SESSION"_Stz,             // CN_SESSION
  /* 231 0x0E7 */ "SESSIONARRAY"_Stz,        // CN_SESSIONARRAY
  /* 232 0x0E8 */ "SHORTCUT"_Stz,            // CN_SHORTCUT
  /* 233 0x0E9 */ "SHORTCUTVEC"_Stz,         // CN_SHORTCUTVEC
  /* 234 0x0EA */ "SINCE"_Stz,               // CN_SINCE
  /* 235 0x0EB */ "SKIPLIST"_Stz,            // CN_SKIPLIST
  /* 236 0x0EC */ "SKIPNODE"_Stz,            // CN_SKIPNODE
  /* 237 0x0ED */ "SLAB"_Stz,                // CN_SLAB
  /* 238 0x0EE */ "SLABALLOCATOR"_Stz,       // CN_SLABALLOCATOR
  /* 239 0x0EF */ "SLEEP"_Stz,               // CN_SLEEP
  /* 240 0x0F0 */ "SLOTS"_Stz,               // CN_SLOTS
  /* 241 0x0F1 */ "SMALLPERSISTENT"_Stz,     // CN_SMALLPERSISTENT
  /* 242 0x0F2 */ "SMALLTRANSIENT"_Stz,      // CN_SMALLTRANSIENT
  /* 243 0x0F3 */ "SNBCD"_Stz,               // CN_SNBCD
  /* 244 0x0F4 */ "SNBOL"_Stz,               // CN_SNBOL
  /* 245 0x0F5 */ "SNDTE"_Stz,               // CN_SNDTE
  /* 246 0x0F6 */ "SNFLT"_Stz,               // CN_SNFLT
  /* 247 0x0F7 */ "SNINT"_Stz,               // CN_SNINT
  /* 248 0x0F8 */ "SNIVL"_Stz,               // CN_SNIVL
  /* 249 0x0F9 */ "SNTME"_Stz,               // CN_SNTME
  /* 250 0x0FA */ "SNTSP"_Stz,               // CN_SNTSP
  /* 251 0x0FB */ "SNVCH"_Stz,               // CN_SNVCH
  /* 252 0x0FC */ "SOCKET"_Stz,              // CN_SOCKET
  /* 253 0x0FD */ "SORTOP"_Stz,              // CN_SORTOP
  /* 254 0x0FE */ "SPBCD"_Stz,               // CN_SPBCD
  /* 255 0x0FF */ "SPBOL"_Stz,               // CN_SPBOL
  /* 256 0x100 */ "SPDTE"_Stz,               // CN_SPDTE
  /* 257 0x101 */ "SPELAPSED"_Stz,           // CN_SPELAPSED
  /* 258 0x102 */ "SPFLT"_Stz,               // CN_SPFLT
  /* 259 0x103 */ "SPID"_Stz,                // CN_SPID
  /* 260 0x104 */ "SPINT"_Stz,               // CN_SPINT
  /* 261 0x105 */ "SPIVL"_Stz,               // CN_SPIVL
  /* 262 0x106 */ "SPSTRING"_Stz,            // CN_SPSTRING
  /* 263 0x107 */ "SPTKNVECSTR"_Stz,         // CN_SPTKNVECSTR
  /* 264 0x108 */ "SPTME"_Stz,               // CN_SPTME
  /* 265 0x109 */ "SPTSP"_Stz,               // CN_SPTSP
  /* 266 0x10A */ "SPVCH"_Stz,               // CN_SPVCH
  /* 267 0x10B */ "SRTBUFFER"_Stz,           // CN_SRTBUFFER
  /* 268 0x10C */ "SRTSET"_Stz,              // CN_SRTSET
  /* 269 0x10D */ "STACK"_Stz,               // CN_STACK
  /* 270 0x10E */ "STAGE"_Stz,               // CN_STAGE
  /* 271 0x10F */ "STAGENDE"_Stz,            // CN_STAGENDE
  /* 272 0x110 */ "STAGENDEVEC"_Stz,         // CN_STAGENDEVEC
  /* 273 0x111 */ "STAGEPTN"_Stz,            // CN_STAGEPTN
  /* 274 0x112 */ "STAGEPTNVEC"_Stz,         // CN_STAGEPTNVEC
  /* 275 0x113 */ "STAGESPEC"_Stz,           // CN_STAGESPEC
  /* 276 0x114 */ "STAGESPECVEC"_Stz,        // CN_STAGESPECVEC
  /* 277 0x115 */ "STAGEVEC"_Stz,            // CN_STAGEVEC
  /* 278 0x116 */ "STARTMARKER"_Stz,         // CN_STARTMARKER
  /* 280 0x118 */ "STATISTICS"_Stz,          // CN_STATISTICS
  /* 281 0x119 */ "STEPACCUM"_Stz,           // CN_STEPACCUM
  /* 282 0x11A */ "STEPTIMING"_Stz,          // CN_STEPTIMING
  /* 283 0x11B */ "STMT_SELECT"_Stz,         // CN_STMT_SELECT
  /* 284 0x11C */ "STREAMDEF"_Stz,           // CN_STREAMDEF
  /* 285 0x11D */ "STRING_T"_Stz,            // CN_STRING_T
  /* 286 0x11E */ "STZVAL"_Stz,              // CN_STZVAL
  /* 287 0x11F */ "SUPERAGGOP"_Stz,          // CN_SUPERAGGOP
  /* 289 0x121 */ "SUPERMERGEOP"_Stz,        // CN_SUPERMERGEOP
  /* 290 0x122 */ "SYSCALL"_Stz,             // CN_SYSCALL
  
  /* 291 0x123 */ "_T__"_Stz,                // CN__T__
  /* 292 0x124 */ "TABLE"_Stz,               // CN_TABLE
  /* 293 0x125 */ "TABLEDEF"_Stz,            // CN_TABLEDEF
  /* 294 0x126 */ "TABLEPTN"_Stz,            // CN_TABLEPTN
  /* 295 0x127 */ "TABLEPTNVEC"_Stz,         // CN_TABLEPTNVEC
  /* 296 0x128 */ "TAKEAWAYOP"_Stz,          // CN_TAKEAWAYOP
  /* 297 0x129 */ "TASK"_Stz,                // CN_TASK
  /* 298 0x12A */ "TASKLIST"_Stz,            // CN_TASKLIST
  /* 299 0x12B */ "TASKQUEUE"_Stz,           // CN_TASKQUEUE
  /* 300 0x12C */ "TEMPNAME"_Stz,            // CN_TEMPNAME
  /* 301 0x12D */ "TENANT"_Stz,              // CN_TENANT
  /* 302 0x12E */ "TENANTS"_Stz,             // CN_TENANTS
  /* 303 0x12F */ "THREAD"_Stz,              // CN_THREAD
  /* 304 0x130 */ "THREADQUEUE"_Stz,         // CN_THREADQUEUE
  /* 305 0x131 */ "THREADVEC"_Stz,           // CN_THREADVEC
  /* 306 0x132 */ "TIMEMARKER"_Stz,          // CN_TIMEMARKER
  /* 307 0x133 */ "TIMER"_Stz,               // CN_TIMER
  /* 309 0x135 */ "TOFILEOP"_Stz,            // CN_TOFILEOP
  /* 310 0x136 */ "TOKEN"_Stz,               // CN_TOKEN
  /* 311 0x137 */ "TOKENISER"_Stz,           // CN_TOKENISER
  /* 312 0x138 */ "TOKENVEC"_Stz,            // CN_TOKENVEC
  /* 313 0x139 */ "TX"_Stz,                  // CN_TX
  /* 314 0x13A */ "TXSET"_Stz,               // CN_TXSET
  
  /* 315 0x13B */ "_U__"_Stz,                // CN__U__
  
  /* 316 0x13C */ "_V__"_Stz,                // CN__V__
  
  /* 317 0x13D */ "_W__"_Stz,                // CN__W__
  /* 318 0x13E */ "WHEREOP"_Stz,             // CN_WHEREOP
  /* 319 0x13F */ "WILDERNESS"_Stz,          // CN_WILDERNESS
  /* 320 0x140 */ "WORD"_Stz,                // CN_WORD
  
  /* 321 0x141 */ "_X__"_Stz,                // CN__X__
  
  /* 323 0x143 */ "_Y__"_Stz,                // CN__Y__
  
  /* 324 0x144 */ "_Z__"_Stz,                // CN__Z__
  /* 325 0x145 */ "ZEDOP"_Stz,               // CN_ZEDOP

  /* 330 0x14A */ "ENUMS"_Stz,               // CN_ENUMS
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
