/*============================================================================*/
/* TagConstants.h -- Set of Tag Constants                                     */
/*============================================================================*/
#pragma once

extern Stz* C_TAGSET_NAME;

extern Stz* C_TAG_BYPROMPT;
extern Stz* C_TAG_CCODE;
extern Stz* C_TAG_CMDFILE;
extern Stz* C_TAG_CMDLINE;
extern Stz* C_TAG_COUNT;
extern Stz* C_TAG_DBG_ACKS;
extern Stz* C_TAG_DEDICATEDTHD;
extern Stz* C_TAG_DIARY;
extern Stz* C_TAG_DIARYDIR;
extern Stz* C_TAG_DIVISION;
extern Stz* C_TAG_ENTRYPOINT;
extern Stz* C_TAG_EXPR;
extern Stz* C_TAG_FAULTINJECTION;
extern Stz* C_TAG_FAULTPOINT;
extern Stz* C_TAG_FLOW;
extern Stz* C_TAG_FORCELOG;
extern Stz* C_TAG_FREEPCT;
extern Stz* C_TAG_OPTIMISER_TRACE;
extern Stz* C_TAG_GRID;
extern Stz* C_TAG_HASCODEFILE;
extern Stz* C_TAG_HOST;
extern Stz* C_TAG_INFOSET;
extern Stz* C_TAG_INTERNAL;
extern Stz* C_TAG_INTERNALERROR;
extern Stz* C_TAG_JOBNM;
extern Stz* C_TAG_JOBMETRICS;
extern Stz* C_TAG_KEEPCODEFILE;
extern Stz* C_TAG_KEYEXPR;
extern Stz* C_TAG_LASTJOBID;
extern Stz* C_TAG_LIFE;
extern Stz* C_TAG_LOWMEMORY;
extern Stz* C_TAG_MEMORYLOG;
extern Stz* C_TAG_METADATA;
extern Stz* C_TAG_NESTEDSQL;
extern Stz* C_TAG_NODE;
extern Stz* C_TAG_NOTE;
extern Stz* C_TAG_NOWAIT;
extern Stz* C_TAG_OBJTRACKER;
extern Stz* C_TAG_OPERANDLOG;
extern Stz* C_TAG_OPTIMISE;
extern Stz* C_TAG_OPTIMISED;
extern Stz* C_TAG_PATH;
extern Stz* C_TAG_PARAM0;
extern Stz* C_TAG_PARAM1;
extern Stz* C_TAG_PARAM2;
extern Stz* C_TAG_PARAM3;
extern Stz* C_TAG_PARAM4;
extern Stz* C_TAG_PERSISTENT;
extern Stz* C_TAG_PIPELINENAME;
extern Stz* C_TAG_REPLICATED;
extern Stz* C_TAG_REQNO;
extern Stz* C_TAG_REQOF;
extern Stz* C_TAG_RESOURCELOG;
extern Stz* C_TAG_SECTION;
extern Stz* C_TAG_SENDERRORRESPONSEOP;
extern Stz* C_TAG_SHM;
extern Stz* C_TAG_SIGNALPARENT;
extern Stz* C_TAG_STMT;
extern Stz* C_TAG_SUMMARY;
extern Stz* C_TAG_SYSTEMCLOSER;
extern Stz* C_TAG_TABLENM;
extern Stz* C_TAG_TIME;
extern Stz* C_TAG_TRACEAPI;
extern Stz* C_TAG_TRACEOP;
extern Stz* C_TAG_TRANSLITERATED;
extern Stz* C_TAG_USECODEFILE;
extern Stz* C_TAG_WHEN;
extern Stz* C_TAG_XFER;
extern Stz* C_TAG_SENDKIND;
extern Stz* C_TAG_SENDS;
extern Stz* C_TAG_WAIT;
extern Stz* C_TAG_WEAVED;

extern Stz* C_TAG_LD_DATE_FORMAT;
extern Stz* C_TAG_LD_ENCLOSED_BY_LEADING;
extern Stz* C_TAG_LD_ENCLOSED_BY_TRAILING;
extern Stz* C_TAG_LD_ESCAPE_CHAR;
extern Stz* C_TAG_LD_FIELD_DELIMITER;
extern Stz* C_TAG_LD_MAX_ERRORS;
extern Stz* C_TAG_LD_RECORD_TERMINATOR;
extern Stz* C_TAG_LD_RECORD_ENDS;

// ASM Tags
extern Stz* C_TAG_QUERY_NO;
extern Stz* C_TAG_SUBQUERY_NO;

extern Stz* C_TAG_JOINROW_ROUTINE;

extern Stz* C_TAG_LOGICUNIT_HANDLER;

extern Stz* C_TAG_CRW_PROCESS_ROUTINE;
extern Stz* C_TAG_DATAPULSE_PROCESS_ROUTINE;
extern Stz* C_TAG_EOS_PROCESS_ROUTINE;
extern Stz* C_TAG_INDEX_PROCESS_ROUTINE;
extern Stz* C_TAG_ROW_PROCESS_ROUTINE;

extern Stz* C_TAG_LIMIT_ROWS;

extern Stz* C_TAG_PAYLOAD_BYTES;
extern Stz* C_TAG_PAYLOAD_ROUTINE;
extern Stz* C_TAG_PAYLOAD_KEY;
extern Stz* C_TAG_PAYLOAD_TO_BUFFER;

extern Stz* C_TAG_PROJECT_ITEMS;
extern Stz* C_TAG_PROJECT_SRC_COLUMN;
extern Stz* C_TAG_PROJECT_SRC_COLUMNS;
extern Stz* C_TAG_PROJECT_ROUTINE;

extern Stz* C_TAG_ROW_OFFSET;

extern Stz* C_TAG_RELATIONAL_UPPER;

extern Stz* C_TAG_LBOUND_TYPE;
extern Stz* C_TAG_UBOUND_TYPE;

extern Stz* C_TAG_LBOUND_LITERAL;
extern Stz* C_TAG_UBOUND_LITERAL;

extern Stz* C_VB_API;
extern Stz* C_VB_BOOT;
extern Stz* C_VB_OP;
extern Stz* C_VB_ACK;
extern Stz* C_VB_NODE;
extern Stz* C_VB_PIPEIN_CRWBUF;
extern Stz* C_VB_PIPEIN_CSVBUF;
extern Stz* C_VB_PIPEIN_ROWBUF;
extern Stz* C_VB_PIPELINE;
extern Stz* C_VB_PIPESPEC;
extern Stz* C_VB_PIPEOUT_CRWBUF;
extern Stz* C_VB_PIPEOUT_CSVBUF;
extern Stz* C_VB_PIPEOUT_ROWBUF;
extern Stz* C_VB_PROCEDURE;
extern Stz* C_VB_SEN;
extern Stz* C_VB_SEND_CRWBUF;
extern Stz* C_VB_SOCKET;
extern Stz* C_VB_THREAD;
extern Stz* C_VB_WORD;

extern Stz* C_SEE_JOB;
extern Stz* C_SEE_SHM;

extern Stz* C_SHOW_BOOT;
extern Stz* C_SHOW_CMD_TAGS;
extern Stz* C_SHOW_DIR;
extern Stz* C_SHOW_DIR_FAILURE;
extern Stz* C_SHOW_CONNECTION;
extern Stz* C_SHOW_EXCEPTION;
extern Stz* C_SHOW_FILE;
extern Stz* C_SHOW_FILE_FAILURE;
extern Stz* C_SHOW_LOWMEM;
extern Stz* C_SHOW_LOWMEM_RECV;
extern Stz* C_SHOW_RECURRING;
extern Stz* C_SHOW_SOCKET;
extern Stz* C_SHOW_SOCKET_FAILURE;

extern Stz* C_NO_FREE;
extern Stz* C_NOSHOW_RECURRING;

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
