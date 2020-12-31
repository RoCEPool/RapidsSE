/*============================================================================*/
/* TagConstants.h -- Set of Tag Constants                                     */
/*============================================================================*/
#pragma once

static Stz* C_TAGSET_NAME    = "<aTagSetNm>"_Stz;
 
static Stz* C_TAG_BG         = "<bg>"_Stz;
static Stz* C_TAG_BYPROMPT   = "<byPrompt>"_Stz;
static Stz* C_TAG_CMD        = "<cmd>"_Stz;
static Stz* C_TAG_CMDFILE    = "<cmdFile>"_Stz;
static Stz* C_TAG_CMDLINE    = "<cmdLine>"_Stz;
static Stz* C_TAG_DIARY      = "<diary>"_Stz;
static Stz* C_TAG_DIARYDIR   = "<diaryDir>"_Stz;
static Stz* C_TAG_DIVISION   = "<division>"_Stz;
static Stz* C_TAG_FLOW       = "<flow>"_Stz;
static Stz* C_TAG_FORCELOG   = "<forceLog>"_Stz;
static Stz* C_TAG_FREEPCT    = "<freePct>"_Stz;
static Stz* C_TAG_DISTID     = "<distId>"_Stz;
static Stz* C_TAG_GRID       = "<grid>"_Stz;
static Stz* C_TAG_HOST       = "<host>"_Stz;
static Stz* C_TAG_JOBKIND    = "<jobKind>"_Stz;
static Stz* C_TAG_JOBNM      = "<jobNm>"_Stz;
static Stz* C_TAG_LIFE       = "<life>"_Stz;
static Stz* C_TAG_LOWMEMORY  = "<lowMemory>"_Stz;
static Stz* C_TAG_NEXTOP     = "<nextOp>"_Stz;
static Stz* C_TAG_NEXTSTAGE  = "<nextStage>"_Stz;
static Stz* C_TAG_NODE       = "<node>"_Stz;
static Stz* C_TAG_NOTE       = "<note>"_Stz;
static Stz* C_TAG_ORIGIN     = "<origin>"_Stz;
static Stz* C_TAG_PATH       = "<path>"_Stz;
static Stz* C_TAG_PARAM0     = "<param0>"_Stz;
static Stz* C_TAG_PARAM1     = "<param1>"_Stz;
static Stz* C_TAG_PARAM2     = "<param2>"_Stz;
static Stz* C_TAG_PARAM3     = "<param3>"_Stz;
static Stz* C_TAG_PARAM4     = "<param4>"_Stz;
static Stz* C_TAG_REQUEST    = "<request>"_Stz;
static Stz* C_TAG_REQNO      = "<reqNo>"_Stz;
static Stz* C_TAG_REQOF      = "<reqOf>"_Stz;
static Stz* C_TAG_NOWAIT     = "<noWait>"_Stz;
static Stz* C_TAG_PLPROTOCOL = "<plProtocol>"_Stz;
static Stz* C_TAG_SECTION    = "<section>"_Stz;
static Stz* C_TAG_SESSION    = "<session>"_Stz;
static Stz* C_TAG_SHM        = "<shm>"_Stz;
static Stz* C_TAG_STMT       = "<stmt>"_Stz;
static Stz* C_TAG_SUMMARY    = "<summary>"_Stz;
static Stz* C_TAG_TABLENM    = "<tableNm>"_Stz;
static Stz* C_TAG_TAGS       = "<tags>"_Stz;
static Stz* C_TAG_TIME       = "<time>"_Stz;
static Stz* C_TAG_TOPOLOGY   = "<topology>"_Stz;
static Stz* C_TAG_TRACEOP    = "<traceOp>"_Stz;
static Stz* C_TAG_WHEN       = "<when>"_Stz;
static Stz* C_TAG_XFER       = "<xfer>"_Stz;
static Stz* C_TAG_XMITKIND   = "<xmitKind>"_Stz;
static Stz* C_TAG_XMITS      = "<xmits>"_Stz;
 
static Stz* C_TAG_DBG_ACKS   = "<dbg_acks>"_Stz;

static Stz* C_TAG_LD_DATE_FORMAT          = "<date_format>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_LEADING  = "<enclosed_by_leading>"_Stz;
static Stz* C_TAG_LD_ENCLOSED_BY_TRAILING = "<enclosed_by_trailing>"_Stz;
static Stz* C_TAG_LD_ESCAPE_CHAR          = "<escape_char>"_Stz;
static Stz* C_TAG_LD_FIELD_DELIMITER      = "<field_delimiter>"_Stz;
static Stz* C_TAG_LD_MAX_ERRORS           = "<max_errors>"_Stz;
static Stz* C_TAG_LD_RECORD_TERMINATOR    = "<record_terminator>"_Stz;
static Stz* C_TAG_LD_RECORD_ENDS          = "<record_ends>"_Stz;

// job info tags
static Stz* C_TAG_SEEDCAN  = "<seedCan>"_Stz;
static Stz* C_TAG_SEEDEND  = "<seedEnd>"_Stz;
static Stz* C_TAG_SEEDEXE  = "<seedExe>"_Stz;
static Stz* C_TAG_SEEDRCV  = "<seedRcv>"_Stz;
static Stz* C_TAG_SEEDREQ  = "<seedReq>"_Stz;
static Stz* C_TAG_SEEDRUN  = "<seedRun>"_Stz;

// ASM Tags
static Stz* C_TAG_QUERY_NO = "<queryNo>"_Stz;
static Stz* C_TAG_SUBQUERY_NO = "<subQueryNo>"_Stz;

static Stz* C_TAG_SRC_TABLENM  = "<srcTableNm>"_Stz;
static Stz* C_TAG_SRC_INDEXNM  = "<srcIndexNm>"_Stz;
static Stz* C_TAG_ALT_TABLENM  = "<altTableNm>"_Stz;
static Stz* C_TAG_ALT_INDEXNM  = "<altIndexNm>"_Stz;

static Stz* C_TAG_COLUMN_SCALE  = "<columnScale>"_Stz;
static Stz* C_TAG_COLUMN_LENGTH = "<columnLength>"_Stz;
static Stz* C_TAG_COLUMN_TYPE   = "<columnType>"_Stz;
static Stz* C_TAG_COLUMN_NAME   = "<columnName>"_Stz;

static Stz* C_TAG_LOGICUNIT_HANDLER     = "<logicUnitHandler>"_Stz;
static Stz* C_TAG_CRW_PROCESS_ROUTINE   = "<crwProcessRoutine>"_Stz;
static Stz* C_TAG_CRW_FILTER_ROUTINE    = "<crwFilterRoutine>"_Stz;
static Stz* C_TAG_DATAPULSE_PROCESS_ROUTINE = "<dataPulseProcessRoutine>"_Stz;
static Stz* C_TAG_ROW_PROCESS_ROUTINE   = "<rowProcessRoutine>"_Stz;

static Stz* C_TAG_INDEX_RANGE_ROUTINE = "<indexRangeRoutine>"_Stz;

static Stz* C_TAG_SRC_EXISTS_ALTIDX     = "<srcExistsAltIdx>"_Stz;

static Stz* C_TAG_ADD_PTN_HASH          = "<addPtnHash>"_Stz;

static Stz* C_TAG_PROJECT_ITEMS         = "<projectItems>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMN    = "<projectSrcColumn>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMNS   = "<projectSrcColumns>"_Stz;

static Stz* C_TAG_LOCALS_COLUMN = "<localsColumn>"_Stz;
static Stz* C_TAG_SUM_COLUMN    = "<sumColumn>"_Stz;

static Stz* C_TAG_DST_COLUMN       = "<dstColumn>"_Stz;
static Stz* C_TAG_SRC_COLUMN       = "<srcColumn>"_Stz;

static Stz* C_TAG_PREDICATE_COLUMNS     = "<predicateColumns>"_Stz;
static Stz* C_TAG_PREDICATE_SRC_COLUMN  = "<predicateSrcColumn>"_Stz;
static Stz* C_TAG_FILTERNO  = "<filterNo>"_Stz;

static Stz* C_TAG_LITERAL_TYPE     = "<literalType>"_Stz;
static Stz* C_TAG_LITERAL_VALUE    = "<literalValue>"_Stz;
static Stz* C_TAG_RELATIONAL_UPPER = "<relationalUpper>"_Stz;
static Stz* C_TAG_LBOUND_VALUE     = "<lboundValue>"_Stz;
static Stz* C_TAG_UBOUND_VALUE     = "<uboundValue>"_Stz;

static Stz* C_TAG_LOWER_BOUND = "<lowerBound>"_Stz;
static Stz* C_TAG_UPPER_BOUND = "<upperBound>"_Stz;


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
