/*============================================================================*/
/* SP_Key.hpp - Key sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Entries
{
  class SP_Key : public SPsmall
  {
  public:
   
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Key(void);
    SP_Key(Key* pKey, IndexDef* pIndexDef);
   ~SP_Key(void) noexcept(false);
  
    char* azKey(void);

    Stz* sz(void);
    Stz* sz(Key* pKey, IndexDef* pIndexDef);
  
    void vSprintKey(Key* pKey, IndexDef* pIndexDef);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
