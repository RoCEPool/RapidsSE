/*============================================================================*/
/* TempName.hpp - temporary name generator                                    */
/*============================================================================*/
#pragma once

namespace System
{
  class TempName : public Object
  {
  public:
    Lock  m_okLock;
    Since m_oSince;
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
