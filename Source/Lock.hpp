/*============================================================================*/
/* Lock.hpp -- Shared Read / Exclusive Write Lock                             */
/*============================================================================*/
#pragma once

namespace System
{
  const u8 C_WRITELOCK = 0x8000000000000000;
  
  static const i8 C_HAVE_READ_LOCK  = 1;
  static const i8 C_HAVE_WRITE_LOCK = 2;
  
  enum eLK // lock kind
  {
  /*  0 */ LK_NONE=0,
  /*  1 */ LK_ACTIVITY_MAP,
  /*  2 */ LK_API_STATE,
  /*  3 */ LK_BIT_ARRAY,
  /*  4 */ LK_BUFFER_POOL,
  /*  5 */ LK_COLLECTION,
  /*  6 */ LK_DATA,
  /*  7 */ LK_DDL,
  /*  8 */ LK_DIC,
  /*  9 */ LK_EB_STRAND,
  /* 10 */ LK_ENTRYBUFFER,
  /* 11 */ LK_HEAP,
  /* 12 */ LK_INDEX_TREE,
  /* 13 */ LK_JOBS,
  /* 14 */ LK_LIST,
  /* 15 */ LK_LOAD_MAP,
  /* 16 */ LK_PARTITION,
  /* 17 */ LK_PIPE,
  /* 18 */ LK_QUEUE,
  /* 19 */ LK_SOCKET,
  /* 20 */ LK_SUBINSERTER,
  /* 21 */ LK_STEP_ARRAY,
  /* 22 */ LK_TASK,
  /* 23 */ LK_TASK_QUEUE,
  /* 24 */ LK_TASK_SET,
  /* 25 */ LK_THREAD,
  /* 26 */ LK_THREAD_VECTOR,
  /* 27 */ LK_TRACE,
  /* 28 */ LK_TX_SET,
  /* 29 */ LK_UNKNOWN,
  /* 30 */ LK_ENUMS
  };
  
  static Stz* aLK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "ACTIVITY_MAP"_Stz,
  /*  2 */ "API_STATE"_Stz,
  /*  3 */ "BIT_ARRAY"_Stz,
  /*  4 */ "BUFFER_POOL"_Stz,
  /*  5 */ "COLLECTION"_Stz,
  /*  6 */ "DATA"_Stz,
  /*  7 */ "DDL"_Stz,
  /*  8 */ "DIC"_Stz,
  /*  9 */ "EB_STRAND"_Stz,
  /* 10 */ "ENTRYBUFFER"_Stz,
  /* 11 */ "HEAP"_Stz,
  /* 12 */ "INDEX_TREE"_Stz,
  /* 13 */ "JOBS"_Stz,
  /* 14 */ "LIST"_Stz,
  /* 15 */ "LOAD_MAP"_Stz,
  /* 16 */ "PARTITION"_Stz,
  /* 17 */ "PIPE"_Stz,
  /* 18 */ "QUEUE"_Stz,
  /* 19 */ "SOCKET"_Stz,
  /* 20 */ "SUBINSERTER"_Stz,
  /* 21 */ "STEP_ARRAY"_Stz,
  /* 22 */ "TASK"_Stz,
  /* 23 */ "TASK_QUEUE"_Stz,
  /* 24 */ "TASK_SET"_Stz,
  /* 25 */ "THREAD"_Stz,
  /* 26 */ "THREAD_VECTOR"_Stz,
  /* 27 */ "TRACE"_Stz,
  /* 28 */ "TX_SET"_Stz,
  /* 29 */ "UNKNOWN"_Stz,
  /* 30 */ "ENUMS"_Stz
  };

  class Lock : public Object
  {
  private:
    u8     m_uLockWord;        // lock word
  
    eLK    m_eLockKind;        // lock kind
  
    Atomic m_ziReadGrabs;       // number of read grabs
    Atomic m_ziReadCollisions;  // number of read collisions
    
    Atomic m_ziWriteGrabs;      // number of write grabs
    Atomic m_ziWriteCollisions; // number of write collisions
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(void);
    Lock(eLK eLockKind);
    ~Lock(void) noexcept(false);
  
    void vInitLock(eLK eLockKind);
  
    bool bIsLocked(void);
    u8   uLockWord(void);
    bool bGrabRead(void);
    void vGrabRead(void);
    void vCheckRead(void);
    void vDropRead(void);
    i8   iReadGrabs(void);
    i8   iReadCollisions(void);
    
    bool bGrabWrite(void);
    void vGrabWrite(void);
    void vCheckWrite(void);
    void vDropWrite(void);
    i8   iWriteGrabs(void);
    i8   iWriteCollisions(void);
    
    Footer* aFooter(void);
  };
  
  Lock* _pLock(void* aAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
