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
  /* 18 */ LK_PARTITION,
  /* 19 */ LK_PIPE,
  /* 20 */ LK_QUEUE,
  /* 21 */ LK_SOCKET,
  /* 22 */ LK_SUBINSERTER,
  /* 23 */ LK_STEP_ARRAY,
  /* 24 */ LK_TASK,
  /* 25 */ LK_TASK_QUEUE,
  /* 26 */ LK_TASK_SET,
  /*    */ LK_TEMP_NAME,
  /* 27 */ LK_THREAD_VECTOR,
  /* 28 */ LK_TRACE,
  /* 29 */ LK_TX_SET,
  /* 30 */ LK_UNKNOWN,
  /* 31 */ LK_ENUMS
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
  /* 18 */ "PARTITION"_Stz,
  /* 19 */ "PIPE"_Stz,
  /* 20 */ "QUEUE"_Stz,
  /* 21 */ "SOCKET"_Stz,
  /* 22 */ "SUBINSERTER"_Stz,
  /* 23 */ "STEP_ARRAY"_Stz,
  /* 24 */ "TASK"_Stz,
  /* 25 */ "TASK_QUEUE"_Stz,
  /* 26 */ "TASK_SET"_Stz,
  /*    */ "TEMP_NAME"_Stz,
  /* 27 */ "THREAD_VECTOR"_Stz,
  /* 28 */ "TRACE"_Stz,
  /* 29 */ "TX_SET"_Stz,
  /* 30 */ "UNKNOWN"_Stz,
  /* 31 */ "ENUMS"_Stz
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
