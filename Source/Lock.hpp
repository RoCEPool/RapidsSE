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
  /*  7 */ LK_DATA,
  /*  8 */ LK_DDL,
  /*  9 */ LK_DIC,
  /* 10 */ LK_EB_STRAND,
  /* 11 */ LK_ENTRYBUFFER,
  /* 12 */ LK_HEAP,
  /* 13 */ LK_INDEX_TREE,
  /* 14 */ LK_JOBS,
  /* 15 */ LK_LIST,
  /* 16 */ LK_LOAD_MAP,
  /* 17 */ LK_PARTITION,
  /* 18 */ LK_PIPE,
  /* 19 */ LK_QUEUE,
  /* 20 */ LK_SOCKET,
  /* 21 */ LK_SUBINSERTER,
  /* 22 */ LK_STEP_ARRAY,
  /* 23 */ LK_TASK,
  /* 24 */ LK_TASK_QUEUE,
  /* 25 */ LK_TASK_SET,
  /* 26 */ LK_THREAD_VECTOR,
  /* 27 */ LK_TRACE,
  /* 28 */ LK_TX_SET,
  /* 29 */ LK_UNKNOWN,
  /* 30 */ LK_ENUMS
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
  /*  7 */ "DATA"_Stz,
  /*  8 */ "DDL"_Stz,
  /*  9 */ "DIC"_Stz,
  /* 10 */ "EB_STRAND"_Stz,
  /* 11 */ "ENTRYBUFFER"_Stz,
  /* 12 */ "HEAP"_Stz,
  /* 13 */ "INDEX_TREE"_Stz,
  /* 14 */ "JOBS"_Stz,
  /* 15 */ "LIST"_Stz,
  /* 16 */ "LOAD_MAP"_Stz,
  /* 17 */ "PARTITION"_Stz,
  /* 18 */ "PIPE"_Stz,
  /* 19 */ "QUEUE"_Stz,
  /* 20 */ "SOCKET"_Stz,
  /* 21 */ "SUBINSERTER"_Stz,
  /* 22 */ "STEP_ARRAY"_Stz,
  /* 23 */ "TASK"_Stz,
  /* 24 */ "TASK_QUEUE"_Stz,
  /* 25 */ "TASK_SET"_Stz,
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
