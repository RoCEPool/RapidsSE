/*============================================================================*/
/* ClassPlaceHolder.hpp -- Place Holder for a new Class                       */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ClassPlaceHolder : public Object
  {
  private:
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ClassPlaceHolder(void);
   ~ClassPlaceHolder(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  ClassPlaceHolder* _pClassPlaceHolder(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
