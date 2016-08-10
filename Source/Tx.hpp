/*============================================================================*/
/* Tx.hpp - Transaction                                                       */
/*============================================================================*/

#pragma once

#include "Transactions.h"

namespace Transactions {
    
  enum eTP // Transaction Phase
  {
    /* 0 */ TP_FREE=0,
    /* 1 */ TP_ALLOCATED,
    /* 2 */ TP_EXECUTING,
    /* 3 */ TP_COMMITTING,
    /* 4 */ TP_COMMITTED,
    /* 5 */ TP_PREPARING,
    /* 6 */ TP_PREPARED,
    /* 7 */ TP_ROLLINGBACK,
    /* 8 */ TP_ROLLEDBACK,
    /* 9 */ TP_ENUMS
  };
  
  static const char *pTP[]
  {
    /* 0 */ "FREE",
    /* 1 */ "ALLOCATED",
    /* 2 */ "EXECUTING",
    /* 3 */ "COMMITTING",
    /* 4 */ "COMMITTED",
    /* 5 */ "PREPARING",
    /* 6 */ "PREPARED",
    /* 7 */ "ROLLINGBACK",
    /* 8 */ "ROLLEDBACK",
    /* 9 */ "ENUMS"
  };
  
  class Tx : public Object,
             public CollectionNode_t<Tx>
  {
    
  private:
    i8    m_iTxId;      // transaction id
    eTP   m_ePhase;     // phase (executing, committing, ...)

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Tx(void);
    
    void vDump(void);
  };
    
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/