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
  /*  1 */ LK_API_STATE,
  /*  2 */ LK_BIT_ARRAY,
  /*  3 */ LK_BUFFER_POOL,
  /*  4 */ LK_COLLECTION,
  /*  5 */ LK_DATA,
  /*  6 */ LK_DDL,
  /*  7 */ LK_DIC,
  /*  8 */ LK_EB_STRAND,
  /*  9 */ LK_HEAP,
  /* 10 */ LK_INDEX_TREE,
  /* 11 */ LK_JOBS,
  /* 12 */ LK_LIST,
  /* 13 */ LK_LOAD_MAP,
  /* 14 */ LK_PARTITION,
  /* 15 */ LK_QUEUE,
  /* 16 */ LK_SOCKET,
  /* 17 */ LK_STEP_ARRAY,
  /* 18 */ LK_TASK,
  /* 19 */ LK_TASK_QUEUE,
  /* 20 */ LK_TASK_SET,
  /* 21 */ LK_THREAD,
  /* 22 */ LK_THREAD_VECTOR,
  /* 23 */ LK_TRACE,
  /* 24 */ LK_TX_SET,
  /* 25 */ LK_UNKNOWN,
  /* 26 */ LK_ENUMS
  };
  
  static Stz* aLK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "API_STATE"_Stz,
  /*  2 */ "BIT_ARRAY"_Stz,
  /*  3 */ "BUFFER_POOL"_Stz,
  /*  4 */ "COLLECTION"_Stz,
  /*  5 */ "DATA"_Stz,
  /*  6 */ "DDL"_Stz,
  /*  7 */ "DIC"_Stz,
  /*  8 */ "EB_STRAND"_Stz,
  /*  9 */ "HEAP"_Stz,
  /* 10 */ "INDEX_TREE"_Stz,
  /* 11 */ "JOBS"_Stz,
  /* 12 */ "LIST"_Stz,
  /* 13 */ "LOAD_MAP"_Stz,
  /* 14 */ "PARTITION"_Stz,
  /* 15 */ "QUEUE"_Stz,
  /* 16 */ "SOCKET"_Stz,
  /* 17 */ "STEP_ARRAY"_Stz,
  /* 18 */ "TASK"_Stz,
  /* 19 */ "TASK_QUEUE"_Stz,
  /* 20 */ "TASK_SET"_Stz,
  /* 21 */ "THREAD"_Stz,
  /* 22 */ "THREAD_VECTOR"_Stz,
  /* 23 */ "TRACE"_Stz,
  /* 24 */ "TX_SET"_Stz,
  /* 25 */ "UNKNOWN"_Stz,
  /* 26 */ "ENUMS"_Stz
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
