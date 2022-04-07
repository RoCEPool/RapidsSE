/*============================================================================*/
/* NextGid.hpp -- Next Gid_t generator                                          */
/*============================================================================*/
#pragma once

namespace BASE
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
  
    void vNodeId(i8 iNodeId);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
