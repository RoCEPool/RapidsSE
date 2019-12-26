/*============================================================================*/
/* NextId.hpp -- Next Id generator                                            */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class NextId : public Object
  {
  private:
    Atomic m_ixId;
    
    // must be last
    Footer m_xFooter;
    
  public:
    NextId(void);
   ~NextId(void) noexcept(false);
  
    i8 operator ++(void); // increment (prefix form)
  
    void vNodeNo(i8 iNodeNo);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
