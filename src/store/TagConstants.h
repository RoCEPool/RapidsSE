/*============================================================================*/
/* TagConstants.h -- Set of Tag Constants                                     */
/*============================================================================*/
#pragma once

static Stz* C_TAGSET_NAME      = "<aTagSetNm>"_Stz;
  
static Stz* C_TAG_BG           = "<bg>"_Stz;
static Stz* C_TAG_BYPROMPT     = "<byPrompt>"_Stz;
static Stz* C_TAG_CMD          = "<cmd>"_Stz;
static Stz* C_TAG_CMDFILE      = "<cmdFile>"_Stz;
static Stz* C_TAG_CMDLINE      = "<cmdLine>"_Stz;
static Stz* C_TAG_DEDICATEDTHD = "<dedicatedThd>"_Stz;
static Stz* C_TAG_DIARY        = "<diary>"_Stz;
static Stz* C_TAG_DIARYDIR     = "<diaryDir>"_Stz;
static Stz* C_TAG_DIVISION     = "<division>"_Stz;
static Stz* C_TAG_FLOW         = "<flow>"_Stz;
static Stz* C_TAG_FORCELOG     = "<forceLog>"_Stz;
static Stz* C_TAG_FREEPCT      = "<freePct>"_Stz;
static Stz* C_TAG_DISTID       = "<distId>"_Stz;
static Stz* C_TAG_GRID         = "<grid>"_Stz;
static Stz* C_TAG_HOST         = "<host>"_Stz;
static Stz* C_TAG_INFOSET      = "<infoSet>"_Stz;
static Stz* C_TAG_INTERNAL     = "<internal>"_Stz;
static Stz* C_TAG_JOBKIND      = "<jobKind>"_Stz;
static Stz* C_TAG_JOBNM        = "<jobNm>"_Stz;
static Stz* C_TAG_LIFE         = "<life>"_Stz;
static Stz* C_TAG_LOWMEMORY    = "<lowMemory>"_Stz;
static Stz* C_TAG_MEMORYLOG    = "<memoryLog>"_Stz;
static Stz* C_TAG_NODE         = "<node>"_Stz;
static Stz* C_TAG_NOTE         = "<note>"_Stz;
static Stz* C_TAG_NOWAIT       = "<noWait>"_Stz;
static Stz* C_TAG_OPERANDLOG   = "<operandLog>"_Stz;
static Stz* C_TAG_ORIGIN       = "<origin>"_Stz;
static Stz* C_TAG_PATH         = "<path>"_Stz;
static Stz* C_TAG_PARAM0       = "<param0>"_Stz;
static Stz* C_TAG_PARAM1       = "<param1>"_Stz;
static Stz* C_TAG_PARAM2       = "<param2>"_Stz;
static Stz* C_TAG_PARAM3       = "<param3>"_Stz;
static Stz* C_TAG_PARAM4       = "<param4>"_Stz;
static Stz* C_TAG_PLPROTOCOL   = "<plProtocol>"_Stz;
static Stz* C_TAG_REPLICATED   = "<replicated>"_Stz;
static Stz* C_TAG_REQUEST      = "<request>"_Stz;
static Stz* C_TAG_REQNO        = "<reqNo>"_Stz;
static Stz* C_TAG_REQOF        = "<reqOf>"_Stz;
static Stz* C_TAG_RESOURCELOG  = "<resourceLog>"_Stz;
static Stz* C_TAG_SECTION      = "<section>"_Stz;
static Stz* C_TAG_SESSION      = "<session>"_Stz;
static Stz* C_TAG_SHM          = "<shm>"_Stz;
static Stz* C_TAG_STMT         = "<stmt>"_Stz;
static Stz* C_TAG_SUMMARY      = "<summary>"_Stz;
static Stz* C_TAG_TABLENM      = "<tableNm>"_Stz;
static Stz* C_TAG_TAGS         = "<tags>"_Stz;
static Stz* C_TAG_TIME         = "<time>"_Stz;
static Stz* C_TAG_TRACEAPI     = "<traceApi>"_Stz;
static Stz* C_TAG_TRACEOP      = "<traceOp>"_Stz;
static Stz* C_TAG_WHEN         = "<when>"_Stz;
static Stz* C_TAG_XFER         = "<xfer>"_Stz;
static Stz* C_TAG_SENDKIND     = "<sendKind>"_Stz;
static Stz* C_TAG_SENDS        = "<sends>"_Stz;
 
static Stz* C_TAG_DBG_ACKS     = "<dbg_acks>"_Stz;

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

static Stz* C_TAG_JOINROW_ROUTINE       = "<joinRowRoutine>"_Stz;

static Stz* C_TAG_LOGICUNIT_HANDLER     = "<logicUnitHandler>"_Stz;

static Stz* C_TAG_CRW_PROCESS_ROUTINE   = "<crwProcessRoutine>"_Stz;
static Stz* C_TAG_DATAPULSE_PROCESS_ROUTINE = "<dataPulseProcessRoutine>"_Stz;
static Stz* C_TAG_EOS_PROCESS_ROUTINE   = "<eosProcessRoutine>"_Stz;
static Stz* C_TAG_INDEX_PROCESS_ROUTINE = "<indexProcessRoutine>"_Stz;
static Stz* C_TAG_ROW_PROCESS_ROUTINE   = "<rowProcessRoutine>"_Stz;

static Stz* C_TAG_LIMIT_ROWS            = "<limitRows>"_Stz;

static Stz* C_TAG_PAYLOAD_BYTES         = "<payLoadBytes>"_Stz;
static Stz* C_TAG_PAYLOAD_ROUTINE       = "<payLoadRoutine>"_Stz;
static Stz* C_TAG_PAYLOAD_KEY           = "<payLoadKey>"_Stz;
static Stz* C_TAG_PAYLOAD_TO_BUFFER     = "<payLoadToBuffer>"_Stz;

static Stz* C_TAG_PROJECT_ITEMS         = "<projectItems>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMN    = "<projectSrcColumn>"_Stz;
static Stz* C_TAG_PROJECT_SRC_COLUMNS   = "<projectSrcColumns>"_Stz;

static Stz* C_TAG_ROW_OFFSET       = "<rowOffset>"_Stz;

static Stz* C_TAG_RELATIONAL_UPPER = "<relationalUpper>"_Stz;

static Stz* C_TAG_LBOUND_TYPE     = "<lboundType>"_Stz;
static Stz* C_TAG_UBOUND_TYPE     = "<uboundType>"_Stz;

static Stz* C_TAG_LBOUND_LITERAL = "<lboundLiteral>"_Stz;
static Stz* C_TAG_UBOUND_LITERAL = "<uboundLiteral>"_Stz;

static Stz* C_VB_API             = "<vb_api>"_Stz;
static Stz* C_VB_BOOT            = "<vb_boot>"_Stz;
static Stz* C_VB_LOGICCOMPILER   = "<vb_logicCompiler>"_Stz;
static Stz* C_VB_OP              = "<vb_op>"_Stz;
static Stz* C_VB_ACK             = "<vb_ack>"_Stz;
static Stz* C_VB_NODE            = "<vb_node>"_Stz;
static Stz* C_VB_PIPEIN_CRWBUF   = "<vb_pipein_crwbuf>"_Stz;
static Stz* C_VB_PIPEIN_CSVBUF   = "<vb_pipein_csvbuf>"_Stz;
static Stz* C_VB_PIPEIN_ROWBUF   = "<vb_pipein_rowbuf>"_Stz;
static Stz* C_VB_PIPELINE        = "<vb_pipeline>"_Stz;
static Stz* C_VB_PIPELINESPEC    = "<vb_pipelineSpec>"_Stz;
static Stz* C_VB_PIPEOUT_CRWBUF  = "<vb_pipeout_crwbuf>"_Stz;
static Stz* C_VB_PIPEOUT_CSVBUF  = "<vb_pipeout_csvbuf>"_Stz;
static Stz* C_VB_PIPEOUT_ROWBUF  = "<vb_pipeout_rowbuf>"_Stz;
static Stz* C_VB_PROCEDURE       = "<vb_procedure>"_Stz;
static Stz* C_VB_SEN             = "<vb_sen>"_Stz;
static Stz* C_VB_SEND_CRWBUF     = "<vb_send_crwbuf>"_Stz;
static Stz* C_VB_SOCKET          = "<vb_socket>"_Stz;
static Stz* C_VB_THREAD          = "<vb_thread>"_Stz;
static Stz* C_VB_WORD            = "<vb_word>"_Stz;

static Stz* C_SEE_SHM            = "<see_shm>"_Stz;

static Stz* C_SEE_JOB            = "<see_job>"_Stz;

static Stz* C_SHOW_BOOT          = "<show_boot>"_Stz;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
