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
  /*  9 */ LK_ENTRYBUFFER,
  /* 10 */ LK_HEAP,
  /* 11 */ LK_INDEX_TREE,
  /* 12 */ LK_JOBS,
  /* 13 */ LK_LIST,
  /* 14 */ LK_LOAD_MAP,
  /* 15 */ LK_PARTITION,
  /* 16 */ LK_QUEUE,
  /* 17 */ LK_SOCKET,
  /* 18 */ LK_STEP_ARRAY,
  /* 19 */ LK_TASK,
  /* 20 */ LK_TASK_QUEUE,
  /* 21 */ LK_TASK_SET,
  /* 22 */ LK_THREAD,
  /* 23 */ LK_THREAD_VECTOR,
  /* 24 */ LK_TRACE,
  /* 25 */ LK_TX_SET,
  /* 26 */ LK_UNKNOWN,
  /* 27 */ LK_ENUMS
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
  /*  9 */ "ENTRYBUFFER"_Stz,
  /* 10 */ "HEAP"_Stz,
  /* 11 */ "INDEX_TREE"_Stz,
  /* 12 */ "JOBS"_Stz,
  /* 13 */ "LIST"_Stz,
  /* 14 */ "LOAD_MAP"_Stz,
  /* 15 */ "PARTITION"_Stz,
  /* 16 */ "QUEUE"_Stz,
  /* 17 */ "SOCKET"_Stz,
  /* 18 */ "STEP_ARRAY"_Stz,
  /* 19 */ "TASK"_Stz,
  /* 20 */ "TASK_QUEUE"_Stz,
  /* 21 */ "TASK_SET"_Stz,
  /* 22 */ "THREAD"_Stz,
  /* 23 */ "THREAD_VECTOR"_Stz,
  /* 24 */ "TRACE"_Stz,
  /* 25 */ "TX_SET"_Stz,
  /* 26 */ "UNKNOWN"_Stz,
  /* 27 */ "ENUMS"_Stz
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
