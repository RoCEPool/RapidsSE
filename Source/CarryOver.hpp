/*============================================================================*/
/* CarryOver.hpp - set of pointers to be restored on restart                  */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class CarryOver : public Buffer
  {
  public:
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CarryOver(void);
    ~CarryOver(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
