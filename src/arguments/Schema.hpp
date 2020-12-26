/*============================================================================*/
/* Schema.hpp -- Schema                                                       */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class Schema : public Folder
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Schema(void);
   ~Schema(void) noexcept(false);
    
   
    Footer* aFooter(void);
  };
  
  Schema* _pSchema(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
