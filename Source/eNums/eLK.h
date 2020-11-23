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
  /* 19 */ LK_NAME,
  /* 20 */ LK_NMSTR_LIST,
  /* 21 */ LK_OP,
  /* 22 */ LK_PARTITION,
  /* 23 */ LK_PIPE,
  /* 24 */ LK_QUEUE,
  /* 25 */ LK_QUEUES,
  /* 26 */ LK_SOCKET,
  /* 27 */ LK_SUBINSERTER,
  /* 28 */ LK_STEP_ARRAY,
  /* 29 */ LK_TASK,
  /* 30 */ LK_TASK_QUEUE,
  /* 31 */ LK_TASK_SET,
  /* 32 */ LK_TCC,
  /* 33 */ LK_TEMP_NAME,
  /* 34 */ LK_THREAD_QUEUE,
  /* 35 */ LK_THREAD_VECTOR,
  /* 36 */ LK_TRACE,
  /* 37 */ LK_TX_SET,
  /* 38 */ LK_UNKNOWN,
  /* 39 */ LK_VECTOR,
  /* 40 */ LK_ENUMS
};

static Stz* aLK[]
{
  /*  0 */ "NONE"_Stz,
  
  /*  1 */ "LK_API_STATE"_Stz,
  /*  2 */ "LK_BIT_ARRAY"_Stz,
  /*  3 */ "LK_BUFFER_POOL"_Stz,
  /*  4 */ "LK_COLLECTION"_Stz,
  /*  5 */ "LK_COMPLETER"_Stz,
  /*  6 */ "LK_CRWBUFFER"_Stz,
  /*  7 */ "LK_DATA"_Stz,
  /*  8 */ "LK_DDL"_Stz,
  /*  9 */ "LK_DIC"_Stz,
  /* 10 */ "LK_ENTRYBUFFER"_Stz,
  /* 11 */ "LK_EXPORT"_Stz,
  /* 12 */ "LK_FOCAL_OBJECTS"_Stz,
  /* 13 */ "LK_HEAP"_Stz,
  /* 14 */ "LK_INDEX_TREE"_Stz,
  /* 15 */ "LK_JOBS"_Stz,
  /* 16 */ "LK_LIST"_Stz,
  /* 17 */ "LK_LOAD_MAP"_Stz,
  /* 18 */ "LK_LOG"_Stz,
  /* 19 */ "LK_NAME"_Stz,
  /* 20 */ "LK_NMSTR_LIST"_Stz,
  /* 21 */ "LK_OP"_Stz,
  /* 22 */ "LK_PARTITION"_Stz,
  /* 23 */ "LK_PIPE"_Stz,
  /* 24 */ "LK_QUEUE"_Stz,
  /* 25 */ "LK_QUEUES"_Stz,
  /* 26 */ "LK_SOCKET"_Stz,
  /* 27 */ "LK_SUBINSERTER"_Stz,
  /* 28 */ "LK_STEP_ARRAY"_Stz,
  /* 29 */ "LK_TASK"_Stz,
  /* 30 */ "LK_TASK_QUEUE"_Stz,
  /* 31 */ "LK_TASK_SET"_Stz,
  /* 32 */ "LK_TCC"_Stz,
  /* 33 */ "LK_TEMP_NAME"_Stz,
  /* 34 */ "LK_THREAD_QUEUE"_Stz,
  /* 35 */ "LK_THREAD_VECTOR"_Stz,
  /* 36 */ "LK_TRACE"_Stz,
  /* 37 */ "LK_TX_SET"_Stz,
  /* 38 */ "LK_UNKNOWN"_Stz,
  /* 39 */ "LK_VECTOR"_Stz,
  
  /* 40 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
