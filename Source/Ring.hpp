/*============================================================================*/
/* Ring.hpp - Ring                                                            */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Ring : public Object
  {
    static const i8 C_ASSIGNMENT = 16;

  private:
    i8 m_iAssignment[C_ASSIGNMENT];
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Ring(void);
    ~Ring(void) noexcept(false);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
