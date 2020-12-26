/*============================================================================*/
/* NextGid.hpp -- Next Gid generator                                          */
/*============================================================================*/
#pragma once

namespace Resources
{
  class NextGid : public Object
  {
  private:
    Atomic m_ixId;
    
    // must be last
    Footer m_xFooter;
    
  public:
    NextGid(void);
   ~NextGid(void) noexcept(false);
  
    i8 operator ++(void); // increment (prefix form)
  
    void vNodeNo(i8 iNodeNo);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
