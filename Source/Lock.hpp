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
  /*  1 */ LK_ACTION_JOURNAL,
  /*  2 */ LK_ACTIVITY_MAP,
  /*  3 */ LK_API_STATE,
  /*  4 */ LK_BIT_ARRAY,
  /*  5 */ LK_BUFFER_POOL,
  /*  6 */ LK_COLLECTION,
  /*  7 */ LK_COMPLETER,
  /*  8 */ LK_CRWBUFFER,
  /*  9 */ LK_DATA,
  /* 10 */ LK_DDL,
  /* 11 */ LK_DIC,
  /* 12 */ LK_ENTRYBUFFER,
  /* 13 */ LK_EXPORT,
  /* 14 */ LK_HEAP,
  /* 15 */ LK_INDEX_TREE,
  /* 16 */ LK_JOBS,
  /* 17 */ LK_LIST,
  /* 18 */ LK_LOAD_MAP,
  /* 19 */ LK_PARTITION,
  /* 20 */ LK_PIPE,
  /* 21 */ LK_QUEUE,
  /* 22 */ LK_SCH,
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
  /*  1 */ "ACTION_JOURNAL"_Stz,
  /*  2 */ "ACTIVITY_MAP"_Stz,
  /*  3 */ "API_STATE"_Stz,
  /*  4 */ "BIT_ARRAY"_Stz,
  /*  5 */ "BUFFER_POOL"_Stz,
  /*  6 */ "COLLECTION"_Stz,
  /*  7 */ "COMPLETER"_Stz,
  /*  8 */ "CRWBUFFER"_Stz,
  /*  9 */ "DATA"_Stz,
  /* 10 */ "DDL"_Stz,
  /* 11 */ "DIC"_Stz,
  /* 12 */ "ENTRYBUFFER"_Stz,
  /* 13 */ "EXPORT"_Stz,
  /* 14 */ "HEAP"_Stz,
  /* 15 */ "INDEX_TREE"_Stz,
  /* 16 */ "JOBS"_Stz,
  /* 17 */ "LIST"_Stz,
  /* 18 */ "LOAD_MAP"_Stz,
  /* 19 */ "PARTITION"_Stz,
  /* 20 */ "PIPE"_Stz,
  /* 21 */ "QUEUE"_Stz,
  /* 22 */ "SCH"_Stz,
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

  class Lock : public Object
  {
  private:
    u8     m_uLockWord;        // lock word
    eLK    m_eLockKind;        // lock kind
  
    Atomic m_ixReadGrabs;       // number of read grabs
    Atomic m_ixReadCollisions;  // number of read collisions
    
    Atomic m_ixWriteGrabs;      // number of write grabs
    Atomic m_ixWriteCollisions; // number of write collisions
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(void);
    Lock(eLK eLockKind);
    ~Lock(void) noexcept(false);
  
    static u8 iLockDropRead(u8*);
    static u8 iLockGrabRead(u8*);
    static u8 iLockDropWrite(u8*);
    static u8 iLockGrabWrite(u8*);
    static u8 iLockIsLocked(u8*);

    void vInitLock(eLK eLockKind);
    bool bIsLocked(void);
    u8   uLockWord(void);
  
    void vAssertRead(void);
    void vDropRead(void);
    bool bGrabRead(void);
    void vGrabRead(void);
    i8   iReadGrabs(void);
    i8   iReadCollisions(void);
    
    void vAssertWrite(void);
    void vDropWrite(void);
    bool bGrabWrite(void);
    void vGrabWrite(void);
    bool bIsWriteLocked(void);
    i8   iWriteGrabs(void);
    i8   iWriteCollisions(void);
    
    Footer* aFooter(void);
  };
  
  Lock* _pLock(void* aAddr);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
