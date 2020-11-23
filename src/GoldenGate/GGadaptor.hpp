/*============================================================================*/
/* GGadaptor.hpp - GGadaptor                                                  */
/*============================================================================*/
#pragma once

namespace GoldenGate
{
  class GGadaptor : public Object
  {
  public:
 
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    GGadaptor(void);
   ~GGadaptor(void) noexcept(false);
      
    Footer* aFooter(void);
  };

  GGadaptor* _pGGadaptor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
