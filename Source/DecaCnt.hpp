/*============================================================================*/
/* DecaCnt.hpp - deca-counter                                                 */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class DecaCnt : public Object
  {
  public:
    s32 m_sName;
    i8 m_iCount;
  
    // must be last
    Footer m_xFooter;
    
  public:
    DecaCnt(Stz* szName);
    ~DecaCnt() noexcept(false);
  
    void vInc(Stz* szFormat,...);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
