/*============================================================================*/
/* SP_Stage.hpp - Stage sprint helper                                         */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Stage : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Stage(void);
    SP_Stage(Stage* pStage, bool bDescend=false);
   ~SP_Stage(void) noexcept(false);

    void vStage(Stage* pStage, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(Stage* pStage);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
