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
  /*  1 */ LK_BUFFER_POOL,
  /*  2 */ LK_COLLECTION,
  /*  3 */ LK_DATA,
  /*  4 */ LK_DDL,
  /*  5 */ LK_DIC,
  /*  6 */ LK_EB_STRAND,
  /*  7 */ LK_HEAP,
  /*  8 */ LK_INDEX_DATA,
  /*  9 */ LK_LIST,
  /* 10 */ LK_LOAD_MAP,
  /* 11 */ LK_QUEUE,
  /* 12 */ LK_STEP_ARRAY,
  /* 13 */ LK_TASK,
  /* 14 */ LK_TASK_SET,
  /* 15 */ LK_TX_MAP,
  /* 16 */ LK_UNKNOWN,
  /* 17 */ LK_ENUMS
  };
  
  static Str* aLK[]
  {
  /*  0 */ "NONE"_Str,
  /*  1 */ "BUFFER_POOL"_Str,
  /*  2 */ "COLLECTION"_Str,
  /*  3 */ "DATA"_Str,
  /*  4 */ "DDL"_Str,
  /*  5 */ "DIC"_Str,
  /*  6 */ "EB_STRAND"_Str,
  /*  7 */ "HEAP"_Str,
  /*  8 */ "INDEX_DATA"_Str,
  /*  9 */ "LIST"_Str,
  /* 10 */ "LOAD_MAP"_Str,
  /* 11 */ "QUEUE"_Str,
  /* 12 */ "STEP_ARRAY"_Str,
  /* 13 */ "TASK"_Str,
  /* 14 */ "TASK_SET"_Str,
  /* 15 */ "TX_MAP"_Str,
  /* 16 */ "UNKNOWN"_Str,
  /* 17 */ "ENUMS"_Str
  };

  LockTrack gxLockTrack[LK_ENUMS];
  
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
