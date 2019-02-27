/*============================================================================*/
/* TempName.hpp - temporary name generator                                    */
/*============================================================================*/
#pragma once

namespace System
{
  class TempName : public Object
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    TempName();
    ~TempName() noexcept(false);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
