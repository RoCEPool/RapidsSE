/*============================================================================*/
/* Licence.hpp - software licence                                             */
/*============================================================================*/
#pragma once

namespace Arguments
{
  class Licence : public Object
  {
  private:
      
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Licence(void);
   ~Licence(void) noexcept(false);
    
  
    Footer* aFooter(void);
  };

  Licence* _pLicence(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
