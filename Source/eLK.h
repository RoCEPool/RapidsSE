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
  /* 19 */ "LK_OP"_Stz,
  /* 20 */ "LK_PARTITION"_Stz,
  /* 21 */ "LK_PIPE"_Stz,
  /* 22 */ "LK_QUEUE"_Stz,
  /* 23 */ "LK_SOCKET"_Stz,
  /* 24 */ "LK_SUBINSERTER"_Stz,
  /* 25 */ "LK_STEP_ARRAY"_Stz,
  /* 26 */ "LK_TASK"_Stz,
  /* 27 */ "LK_TASK_QUEUE"_Stz,
  /* 28 */ "LK_TASK_SET"_Stz,
  /* 29 */ "LK_TEMP_NAME"_Stz,
  /* 30 */ "LK_THREAD_QUEUE"_Stz,
  /* 31 */ "LK_THREAD_VECTOR"_Stz,
  /* 32 */ "LK_TRACE"_Stz,
  /* 33 */ "LK_TX_SET"_Stz,
  /* 34 */ "LK_UNKNOWN"_Stz,
  
  /* 35 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
