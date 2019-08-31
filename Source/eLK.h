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
  /* 19 */ LK_NMSTR_LIST,
  /* 20 */ LK_OP,
  /* 21 */ LK_PARTITION,
  /* 22 */ LK_PIPE,
  /* 23 */ LK_QUEUE,
  /* 24 */ LK_SOCKET,
  /* 25 */ LK_SUBINSERTER,
  /* 26 */ LK_STEP_ARRAY,
  /* 27 */ LK_TASK,
  /* 28 */ LK_TASK_QUEUE,
  /* 29 */ LK_TASK_SET,
  /* 30 */ LK_TEMP_NAME,
  /* 31 */ LK_THREAD_QUEUE,
  /* 32 */ LK_THREAD_VECTOR,
  /* 33 */ LK_TRACE,
  /* 34 */ LK_TX_SET,
  /* 35 */ LK_UNKNOWN,
  /* 36 */ LK_ENUMS
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
  /* 19 */ "LK_NMSTR_LIST"_Stz,
  /* 20 */ "LK_OP"_Stz,
  /* 21 */ "LK_PARTITION"_Stz,
  /* 22 */ "LK_PIPE"_Stz,
  /* 23 */ "LK_QUEUE"_Stz,
  /* 24 */ "LK_SOCKET"_Stz,
  /* 25 */ "LK_SUBINSERTER"_Stz,
  /* 26 */ "LK_STEP_ARRAY"_Stz,
  /* 27 */ "LK_TASK"_Stz,
  /* 28 */ "LK_TASK_QUEUE"_Stz,
  /* 29 */ "LK_TASK_SET"_Stz,
  /* 30 */ "LK_TEMP_NAME"_Stz,
  /* 31 */ "LK_THREAD_QUEUE"_Stz,
  /* 32 */ "LK_THREAD_VECTOR"_Stz,
  /* 33 */ "LK_TRACE"_Stz,
  /* 34 */ "LK_TX_SET"_Stz,
  /* 35 */ "LK_UNKNOWN"_Stz,
  
  /* 36 */ "ENUMS"_Stz
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
