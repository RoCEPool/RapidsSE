/*============================================================================*/
/* PipeIn.hpp - Pipe In                                                       */
/*============================================================================*/
#pragma once

#include "BASE.h"

namespace STORE
{
  class PipeIn : public Object
  {
  public:
  
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueLeft;   // left input queue
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueRight;  // right input queue
    SMA<OperandQueue,CN_OPERANDQUEUE> m_pQueueOoO;    // out-of-order queue

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    PipeIn(void);
   ~PipeIn(void) noexcept(false);
  
    OperandQueue* pQueueLeft(void);
    OperandQueue* pQueueRight(void);
    OperandQueue* pQueueOoO(void);
      
    Footer* aFooter(void);
  };

  PipeIn* _pPipeIn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
