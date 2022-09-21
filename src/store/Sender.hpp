/*============================================================================*/
/* Sender.hpp --                                                           */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Sender : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Sender(Bench* pBench);
   ~Sender(void) noexcept(false);

    void vDelete_Sends(void);
    void vReadd_Sends(void);

  private:

    Footer* aFooter(void);
  };

  Sender* _pSender(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
