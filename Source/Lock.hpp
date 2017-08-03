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
  /*    */ LK_BIT_ARRAY,
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
  
  static Stz* aLK[]
  {
  /*  0 */ "NONE"_Stz,
  /*    */ "BIT_ARRAY"_Stz,
  /*  1 */ "BUFFER_POOL"_Stz,
  /*  2 */ "COLLECTION"_Stz,
  /*  3 */ "DATA"_Stz,
  /*  4 */ "DDL"_Stz,
  /*  5 */ "DIC"_Stz,
  /*  6 */ "EB_STRAND"_Stz,
  /*  7 */ "HEAP"_Stz,
  /*  8 */ "INDEX_DATA"_Stz,
  /*  9 */ "LIST"_Stz,
  /* 10 */ "LOAD_MAP"_Stz,
  /* 11 */ "QUEUE"_Stz,
  /* 12 */ "STEP_ARRAY"_Stz,
  /* 13 */ "TASK"_Stz,
  /* 14 */ "TASK_SET"_Stz,
  /* 15 */ "TX_MAP"_Stz,
  /* 16 */ "UNKNOWN"_Stz,
  /* 17 */ "ENUMS"_Stz
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
