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
    SP_PipeWork(PipeWork* pPipeWork);
   ~SP_PipeWork(void) noexcept(false);

    void vPipeWork(PipeWork* pPipeWork);
    void vPipeWork(PipeWork* pPipeWork, i8 iDepth);
  
    Stz* sz(void);
    Stz* sz(PipeWork* pPipeWork);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
