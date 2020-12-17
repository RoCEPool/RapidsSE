/*============================================================================*/
/* TempName.hpp -- Temporary Name generator                                   */
/*============================================================================*/
#pragma once

namespace Resources
{
  class TempName : public Object
  {
  public:
    Lock  m_okLock;
    Birth m_oBirth;
    s32   m_sClicker;
  
    // must be last
    Footer m_xFooter;
    
  public:
    TempName(void);
    ~TempName() noexcept(false);
  
    void vNextName(s32& sName);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
