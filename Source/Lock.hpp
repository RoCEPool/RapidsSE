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
  /*  1 */ LK_BIT_ARRAY,
  /*  2 */ LK_BUFFER_POOL,
  /*  3 */ LK_COLLECTION,
  /*  4 */ LK_DATA,
  /*  5 */ LK_DDL,
  /*  6 */ LK_DIC,
  /*  7 */ LK_EB_STRAND,
  /*  8 */ LK_HEAP,
  /*  9 */ LK_INDEX_TREE,
  /* 10 */ LK_LIST,
  /* 11 */ LK_LOAD_MAP,
  /* 12 */ LK_PARTITION,
  /* 13 */ LK_QUEUE,
  /* 14 */ LK_STEP_ARRAY,
  /* 15 */ LK_TASK,
  /* 16 */ LK_TASK_QUEUE,
  /* 17 */ LK_TASK_SET,
  /* 18 */ LK_THREAD,
  /* 19 */ LK_THREAD_VECTOR,
  /* 20 */ LK_TX_SET,
  /* 21 */ LK_UNKNOWN,
  /* 22 */ LK_ENUMS
  };
  
  static Stz* aLK[]
  {
  /*  0 */ "NONE"_Stz,
  /*  1 */ "BIT_ARRAY"_Stz,
  /*  2 */ "BUFFER_POOL"_Stz,
  /*  3 */ "COLLECTION"_Stz,
  /*  4 */ "DATA"_Stz,
  /*  5 */ "DDL"_Stz,
  /*  6 */ "DIC"_Stz,
  /*  7 */ "EB_STRAND"_Stz,
  /*  8 */ "HEAP"_Stz,
  /*  9 */ "INDEX_TREE"_Stz,
  /* 10 */ "LIST"_Stz,
  /* 11 */ "LOAD_MAP"_Stz,
  /* 12 */ "PARTITION"_Stz,
  /* 13 */ "QUEUE"_Stz,
  /* 14 */ "STEP_ARRAY"_Stz,
  /* 15 */ "TASK"_Stz,
  /* 16 */ "TASK_QUEUE"_Stz,
  /* 17 */ "TASK_SET"_Stz,
  /* 18 */ "THREAD"_Stz,
  /* 19 */ "THREAD_VECTOR"_Stz,
  /* 20 */ "TX_SET"_Stz,
  /* 21 */ "UNKNOWN"_Stz,
  /* 22 */ "ENUMS"_Stz
  };

  class Lock : public Object
  {
  private:
    u8     m_uLockWord;        // lock word
  
    eLK    m_eLockKind;        // lock kind
  
    Atomic m_zReadGrabs;       // number of read grabs
    Atomic m_zReadCollisions;  // number of read collisions
    
    Atomic m_zWriteGrabs;      // number of write grabs
    Atomic m_zWriteCollisions; // number of write collisions
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Lock* pLock);
    void  operator delete(void* pLockAddress);
    
    Lock(void);
    ~Lock(void);
  
    void vInitLock(eLK eLockKind);
  
    bool bIsLocked(void);
    
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
