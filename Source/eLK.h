/*============================================================================*/
/* eLK.h -- Lock Kind                                                         */
/*============================================================================*/
#pragma once

enum eLK // lock kind
{
  /*  0 */ LK_NONE=0,
  /*  1 */ LK_API_STATE,
  /*  2 */ LK_BIT_ARRAY,
  /*  3 */ LK_BUFFER_POOL,
  /*  4 */ LK_COLLECTION,
  /*  5 */ LK_COMPLETER,
  /*  6 */ LK_CRWBUFFER,
  /*  7 */ LK_DATA,
  /*  8 */ LK_DDL,
  /*  9 */ LK_DIC,
  /* 10 */ LK_ENTRYBUFFER,
  /* 11 */ LK_EXPORT,
  /* 12 */ LK_FOCAL_OBJECTS,
  /* 13 */ LK_HEAP,
  /* 14 */ LK_INDEX_TREE,
  /* 15 */ LK_JOBS,
  /* 16 */ LK_LIST,
  /* 17 */ LK_LOAD_MAP,
  /* 18 */ LK_LOG,
  /* 19 */ LK_OP,
  /* 20 */ LK_PARTITION,
  /* 21 */ LK_PIPE,
  /* 22 */ LK_QUEUE,
  /* 23 */ LK_SOCKET,
  /* 24 */ LK_SUBINSERTER,
  /* 25 */ LK_STEP_ARRAY,
  /* 26 */ LK_TASK,
  /* 27 */ LK_TASK_QUEUE,
  /* 28 */ LK_TASK_SET,
  /* 29 */ LK_TEMP_NAME,
  /* 30 */ LK_THREAD_QUEUE,
  /* 31 */ LK_THREAD_VECTOR,
  /* 32 */ LK_TRACE,
  /* 33 */ LK_TX_SET,
  /* 34 */ LK_UNKNOWN,
  /* 35 */ LK_ENUMS
};

static Stz* aLK[]
{
  /*  0 */ "NONE"_Stz,
  /*  1 */ "API_STATE"_Stz,
  /*  2 */ "BIT_ARRAY"_Stz,
  /*  3 */ "BUFFER_POOL"_Stz,
  /*  4 */ "COLLECTION"_Stz,
  /*  5 */ "COMPLETER"_Stz,
  /*  6 */ "CRWBUFFER"_Stz,
  /*  7 */ "DATA"_Stz,
  /*  8 */ "DDL"_Stz,
  /*  9 */ "DIC"_Stz,
  /* 10 */ "ENTRYBUFFER"_Stz,
  /* 11 */ "EXPORT"_Stz,
  /* 12 */ "FOCAL_OBJECTS"_Stz,
  /* 13 */ "HEAP"_Stz,
  /* 14 */ "INDEX_TREE"_Stz,
  /* 15 */ "JOBS"_Stz,
  /* 16 */ "LIST"_Stz,
  /* 17 */ "LOAD_MAP"_Stz,
  /* 18 */ "ACTION_LOG"_Stz,
  /* 19 */ "OP"_Stz,
  /* 20 */ "PARTITION"_Stz,
  /* 21 */ "PIPE"_Stz,
  /* 22 */ "QUEUE"_Stz,
  /* 23 */ "SOCKET"_Stz,
  /* 24 */ "SUBINSERTER"_Stz,
  /* 25 */ "STEP_ARRAY"_Stz,
  /* 26 */ "TASK"_Stz,
  /* 27 */ "TASK_QUEUE"_Stz,
  /* 28 */ "TASK_SET"_Stz,
  /* 29 */ "TEMP_NAME"_Stz,
  /* 30 */ "THREAD_QUEUE"_Stz,
  /* 31 */ "THREAD_VECTOR"_Stz,
  /* 32 */ "TRACE"_Stz,
  /* 33 */ "TX_SET"_Stz,
  /* 34 */ "UNKNOWN"_Stz,
  /* 35 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
