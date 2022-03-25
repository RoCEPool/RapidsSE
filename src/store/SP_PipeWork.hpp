/*============================================================================*/
/* SP_PipeWork.hpp - PipeWork sprint helper                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_PipeWork : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_PipeWork(void);
    SP_PipeWork(PipeWork* pPipeWork, bool bDescend=false);
   ~SP_PipeWork(void) noexcept(false);

    void vPipeWork(PipeWork* pPipeWork, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(PipeWork* pPipeWork);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
