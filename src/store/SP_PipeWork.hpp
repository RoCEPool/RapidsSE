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
    SP_PipeWork(Vertex* pPipeWork);
   ~SP_PipeWork(void) noexcept(false);

    void vPipeWork(Vertex* pPipeWork);
    void vPipeWork(Vertex* pPipeWork, i8 iDepth);
  
    Stz* sz(void);
    Stz* sz(Vertex* pPipeWork);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
