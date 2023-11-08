/*============================================================================*/
/* TagConstants.cpp -- Set of Tag Constants                                   */
/*============================================================================*/
#include "BASE.h"
#include "TagConstants.h"

Stz* C_TAGSET_NAME      = "<aTagSetNm>"_Stz;

Stz* C_TAG_BYPROMPT     = "<byPrompt>"_Stz;
Stz* C_TAG_CCODE        = "<ccode>"_Stz;
Stz* C_TAG_CMDFILE      = "<cmdFile>"_Stz;
Stz* C_TAG_CMDLINE      = "<cmdLine>"_Stz;
Stz* C_TAG_COUNT        = "<count>"_Stz;
Stz* C_TAG_DBG_ACKS     = "<dbg_acks>"_Stz;
Stz* C_TAG_DEDICATEDTHD = "<dedicatedThd>"_Stz;
Stz* C_TAG_DIARY        = "<diary>"_Stz;
Stz* C_TAG_DIARYDIR     = "<diaryDir>"_Stz;
Stz* C_TAG_DIVISION     = "<division>"_Stz;
Stz* C_TAG_ENTRYPOINT   = "<entryPoint>"_Stz;
Stz* C_TAG_EXPR         = "<expr>"_Stz;
Stz* C_TAG_FAULTINJECTION = "<faultInjection>"_Stz;
Stz* C_TAG_FAULTPOINT   = "<faultPoint>"_Stz;
Stz* C_TAG_FLOW         = "<flow>"_Stz;
Stz* C_TAG_FORCELOG     = "<forceLog>"_Stz;
Stz* C_TAG_FREEPCT      = "<freePct>"_Stz;
Stz* C_TAG_OPTIMISER_TRACE = "<optimiser_trace>"_Stz;
Stz* C_TAG_GRID         = "<grid>"_Stz;
Stz* C_TAG_HASCODEFILE  = "<hasCodeFile>"_Stz;
Stz* C_TAG_HOST         = "<host>"_Stz;
Stz* C_TAG_INFOSET      = "<infoSet>"_Stz;
Stz* C_TAG_INTERNAL     = "<internal>"_Stz;
Stz* C_TAG_INTERNALERROR  = "<internalError>"_Stz;
Stz* C_TAG_JOBNM        = "<jobNm>"_Stz;
Stz* C_TAG_JOBMETRICS   = "<jobMetrics>"_Stz;
Stz* C_TAG_KEEPCODEFILE = "<keepCodeFile>"_Stz;
Stz* C_TAG_KEYEXPR      = "<keyExpr>"_Stz;
Stz* C_TAG_LASTJOBID    = "<lastJobId>"_Stz;
Stz* C_TAG_LIFE         = "<life>"_Stz;
Stz* C_TAG_LOWMEMORY    = "<lowMemory>"_Stz;
Stz* C_TAG_MEMORYLOG    = "<memoryLog>"_Stz;
Stz* C_TAG_NESTEDSQL    = "<nestedSql>"_Stz;
Stz* C_TAG_NODE         = "<node>"_Stz;
Stz* C_TAG_NOTE         = "<note>"_Stz;
Stz* C_TAG_NOWAIT       = "<noWait>"_Stz;
Stz* C_TAG_OBJTRACKER   = "<objectTracker>"_Stz;
Stz* C_TAG_OPERANDLOG   = "<operandLog>"_Stz;
Stz* C_TAG_OPTIMISE     = "<optimise>"_Stz;
Stz* C_TAG_OPTIMISED    = "<optimised>"_Stz;
Stz* C_TAG_PATH         = "<path>"_Stz;
Stz* C_TAG_PARAM0       = "<param0>"_Stz;
Stz* C_TAG_PARAM1       = "<param1>"_Stz;
Stz* C_TAG_PARAM2       = "<param2>"_Stz;
Stz* C_TAG_PARAM3       = "<param3>"_Stz;
Stz* C_TAG_PARAM4       = "<param4>"_Stz;
Stz* C_TAG_PERSISTENT   = "<persistentTx>"_Stz;
Stz* C_TAG_PIPELINENAME = "<pipelineName>"_Stz;
Stz* C_TAG_REPLICATED   = "<replicated>"_Stz;
Stz* C_TAG_REQNO        = "<reqNo>"_Stz;
Stz* C_TAG_REQOF        = "<reqOf>"_Stz;
Stz* C_TAG_RESOURCELOG  = "<resourceLog>"_Stz;
Stz* C_TAG_SECTION      = "<section>"_Stz;
Stz* C_TAG_SHM          = "<shm>"_Stz;
Stz* C_TAG_SIGNALPARENT = "<signalParent>"_Stz;
Stz* C_TAG_STMT         = "<stmt>"_Stz;
Stz* C_TAG_SUMMARY      = "<summary>"_Stz;
Stz* C_TAG_SYSTEMCLOSER = "<systemCloser>"_Stz;
Stz* C_TAG_TABLENM      = "<tableNm>"_Stz;
Stz* C_TAG_TIME         = "<time>"_Stz;
Stz* C_TAG_TRACEAPI     = "<traceApi>"_Stz;
Stz* C_TAG_TRACEOP      = "<traceOp>"_Stz;
Stz* C_TAG_TRANSLITERATED = "<transliterated>"_Stz;
Stz* C_TAG_USECODEFILE  = "<useCodeFile>"_Stz;
Stz* C_TAG_WHEN         = "<when>"_Stz;
Stz* C_TAG_XFER         = "<xfer>"_Stz;
Stz* C_TAG_SENDKIND     = "<sendKind>"_Stz;
Stz* C_TAG_SENDS        = "<sends>"_Stz;
Stz* C_TAG_WAIT         = "<wait>"_Stz;
Stz* C_TAG_WEAVED       = "<weaved>"_Stz;

Stz* C_TAG_LD_DATE_FORMAT          = "<date_format>"_Stz;
Stz* C_TAG_LD_ENCLOSED_BY_LEADING  = "<enclosed_by_leading>"_Stz;
Stz* C_TAG_LD_ENCLOSED_BY_TRAILING = "<enclosed_by_trailing>"_Stz;
Stz* C_TAG_LD_ESCAPE_CHAR          = "<escape_char>"_Stz;
Stz* C_TAG_LD_FIELD_DELIMITER      = "<field_delimiter>"_Stz;
Stz* C_TAG_LD_MAX_ERRORS           = "<max_errors>"_Stz;
Stz* C_TAG_LD_RECORD_TERMINATOR    = "<record_terminator>"_Stz;
Stz* C_TAG_LD_RECORD_ENDS          = "<record_ends>"_Stz;

// ASM Tags
Stz* C_TAG_QUERY_NO = "<queryNo>"_Stz;
Stz* C_TAG_SUBQUERY_NO = "<subQueryNo>"_Stz;

Stz* C_TAG_JOINROW_ROUTINE       = "<joinRowRoutine>"_Stz;

Stz* C_TAG_LOGICUNIT_HANDLER     = "<logicUnitHandler>"_Stz;

Stz* C_TAG_CRW_PROCESS_ROUTINE   = "<crwProcessRoutine>"_Stz;
Stz* C_TAG_DATAPULSE_PROCESS_ROUTINE = "<dataPulseProcessRoutine>"_Stz;
Stz* C_TAG_EOS_PROCESS_ROUTINE   = "<eosProcessRoutine>"_Stz;
Stz* C_TAG_INDEX_PROCESS_ROUTINE = "<indexProcessRoutine>"_Stz;
Stz* C_TAG_ROW_PROCESS_ROUTINE   = "<rowProcessRoutine>"_Stz;

Stz* C_TAG_LIMIT_ROWS            = "<limitRows>"_Stz;

Stz* C_TAG_PAYLOAD_BYTES         = "<payLoadBytes>"_Stz;
Stz* C_TAG_PAYLOAD_ROUTINE       = "<payLoadRoutine>"_Stz;
Stz* C_TAG_PAYLOAD_KEY           = "<payLoadKey>"_Stz;
Stz* C_TAG_PAYLOAD_TO_BUFFER     = "<payLoadToBuffer>"_Stz;

Stz* C_TAG_PROJECT_ITEMS         = "<projectItems>"_Stz;
Stz* C_TAG_PROJECT_SRC_COLUMN    = "<projectSrcColumn>"_Stz;
Stz* C_TAG_PROJECT_SRC_COLUMNS   = "<projectSrcColumns>"_Stz;
Stz* C_TAG_PROJECT_ROUTINE       = "<projectRoutine>"_Stz;

Stz* C_TAG_ROW_OFFSET       = "<rowOffset>"_Stz;

Stz* C_TAG_RELATIONAL_UPPER = "<relationalUpper>"_Stz;

Stz* C_TAG_LBOUND_TYPE     = "<lboundType>"_Stz;
Stz* C_TAG_UBOUND_TYPE     = "<uboundType>"_Stz;

Stz* C_TAG_LBOUND_LITERAL = "<lboundLiteral>"_Stz;
Stz* C_TAG_UBOUND_LITERAL = "<uboundLiteral>"_Stz;

Stz* C_VB_API             = "<vb_api>"_Stz;
Stz* C_VB_BOOT            = "<vb_boot>"_Stz;
Stz* C_VB_OP              = "<vb_op>"_Stz;
Stz* C_VB_ACK             = "<vb_ack>"_Stz;
Stz* C_VB_NODE            = "<vb_node>"_Stz;
Stz* C_VB_PIPEIN_CRWBUF   = "<vb_pipein_crwbuf>"_Stz;
Stz* C_VB_PIPEIN_CSVBUF   = "<vb_pipein_csvbuf>"_Stz;
Stz* C_VB_PIPEIN_ROWBUF   = "<vb_pipein_rowbuf>"_Stz;
Stz* C_VB_PIPELINE        = "<vb_pipeline>"_Stz;
Stz* C_VB_PIPESPEC        = "<vb_pipeSpec>"_Stz;
Stz* C_VB_PIPEOUT_CRWBUF  = "<vb_pipeout_crwbuf>"_Stz;
Stz* C_VB_PIPEOUT_CSVBUF  = "<vb_pipeout_csvbuf>"_Stz;
Stz* C_VB_PIPEOUT_ROWBUF  = "<vb_pipeout_rowbuf>"_Stz;
Stz* C_VB_PROCEDURE       = "<vb_procedure>"_Stz;
Stz* C_VB_SEN             = "<vb_sen>"_Stz;
Stz* C_VB_SEND_CRWBUF     = "<vb_send_crwbuf>"_Stz;
Stz* C_VB_SOCKET          = "<vb_socket>"_Stz;
Stz* C_VB_THREAD          = "<vb_thread>"_Stz;
Stz* C_VB_WORD            = "<vb_word>"_Stz;

Stz* C_SEE_JOB            = "<see_job>"_Stz;
Stz* C_SEE_SHM            = "<see_shm>"_Stz;

Stz* C_SHOW_BOOT           = "<show_boot>"_Stz;
Stz* C_SHOW_CMD_TAGS       = "<show_cmd_tags>"_Stz;
Stz* C_SHOW_DIR            = "<show_dir>"_Stz;
Stz* C_SHOW_DIR_FAILURE    = "<show_dir_failure>"_Stz;
Stz* C_SHOW_CONNECTION     = "<show_connection>"_Stz;
Stz* C_SHOW_EXCEPTION      = "<show_exception>"_Stz;
Stz* C_SHOW_FILE           = "<show_file>"_Stz;
Stz* C_SHOW_FILE_FAILURE   = "<show_file_failure>"_Stz;
Stz* C_SHOW_LOWMEM         = "<show_lowmem>"_Stz;
Stz* C_SHOW_LOWMEM_RECV    = "<show_lowmem_recv>"_Stz;
Stz* C_SHOW_RECURRING      = "<show_recurring>"_Stz;
Stz* C_SHOW_SOCKET         = "<show_socket>"_Stz;
Stz* C_SHOW_SOCKET_FAILURE = "<show_socket_failure>"_Stz;
//                                   0123456789012345678901234567890
//                                   0         1         2         3
Stz* C_NO_FREE             = "<no_free>"_Stz;
Stz* C_NOSHOW_RECURRING    = "<noshow_recurring>"_Stz;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
