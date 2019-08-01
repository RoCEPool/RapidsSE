/*============================================================================*/
/* eLK.h -- lock kind                                                         */
/*============================================================================*/
#pragma once

enum eLK // lock kind
{
  /*  0 */ LK_NONE=0,
  /*  1 */ LK_ACTION_LOG,
  /*  2 */ LK_API_STATE,
  /*  3 */ LK_BIT_ARRAY,
  /*  4 */ LK_BUFFER_POOL,
  /*  5 */ LK_COLLECTION,
  /*  6 */ LK_COMPLETER,
  /*  7 */ LK_CRWBUFFER,
  /*  8 */ LK_DATA,
  /*  9 */ LK_DDL,
  /* 10 */ LK_DIC,
  /* 11 */ LK_ENTRYBUFFER,
  /* 12 */ LK_EXPORT,
  /* 13 */ LK_HEAP,
  /* 14 */ LK_INDEX_TREE,
  /* 15 */ LK_JOBS,
  /* 16 */ LK_LIST,
  /* 17 */ LK_LOAD_MAP,
  /* 18 */ LK_OP,
  /* 19 */ LK_PARTITION,
  /* 20 */ LK_PIPE,
  /* 21 */ LK_QUEUE,
  /* 22 */ LK_SOCKET,
  /* 23 */ LK_SUBINSERTER,
  /* 24 */ LK_STEP_ARRAY,
  /* 25 */ LK_TASK,
  /* 26 */ LK_TASK_QUEUE,
  /* 27 */ LK_TASK_SET,
  /* 28 */ LK_TEMP_NAME,
  /* 29 */ LK_THREAD_QUEUE,
  /* 30 */ LK_THREAD_VECTOR,
  /* 31 */ LK_TRACE,
  /* 32 */ LK_TX_SET,
  /* 33 */ LK_UNKNOWN,
  /* 34 */ LK_ENUMS
};

static Stz* aLK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ACTION_LOG"_Stz,
  /*  2 */ "API_STATE"_Stz,
  /*  3 */ "BIT_ARRAY"_Stz,
  /*  4 */ "BUFFER_POOL"_Stz,
  /*  5 */ "COLLECTION"_Stz,
  /*  6 */ "COMPLETER"_Stz,
  /*  7 */ "CRWBUFFER"_Stz,
  /*  8 */ "DATA"_Stz,
  /*  9 */ "DDL"_Stz,
  /* 10 */ "DIC"_Stz,
  /* 11 */ "ENTRYBUFFER"_Stz,
  /* 12 */ "EXPORT"_Stz,
  /* 13 */ "HEAP"_Stz,
  /* 14 */ "INDEX_TREE"_Stz,
  /* 15 */ "JOBS"_Stz,
  /* 16 */ "LIST"_Stz,
  /* 17 */ "LOAD_MAP"_Stz,
  /* 18 */ "OP"_Stz,
  /* 19 */ "PARTITION"_Stz,
  /* 20 */ "PIPE"_Stz,
  /* 21 */ "QUEUE"_Stz,
  /* 22 */ "SOCKET"_Stz,
  /* 23 */ "SUBINSERTER"_Stz,
  /* 24 */ "STEP_ARRAY"_Stz,
  /* 25 */ "TASK"_Stz,
  /* 26 */ "TASK_QUEUE"_Stz,
  /* 27 */ "TASK_SET"_Stz,
  /* 28 */ "TEMP_NAME"_Stz,
  /* 29 */ "THREAD_QUEUE"_Stz,
  /* 30 */ "THREAD_VECTOR"_Stz,
  /* 31 */ "TRACE"_Stz,
  /* 32 */ "TX_SET"_Stz,
  /* 33 */ "UNKNOWN"_Stz,
  /* 34 */ "ENUMS"_Stz
};


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
