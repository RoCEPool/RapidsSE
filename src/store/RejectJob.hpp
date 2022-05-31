/*============================================================================*/
/* RejectJob.hpp -- Reject Job                                                */
/*============================================================================*/
#pragma once

#include "eOM.h"
#include "eMG.h"

namespace STORE
{
  class RejectJob : public Object
  {
  protected:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    RejectJob(void);
   ~RejectJob(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };

  RejectJob* _pRejectJob(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
