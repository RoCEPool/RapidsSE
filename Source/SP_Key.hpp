/*============================================================================*/
/* SP_Key.hpp - Key sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Key : public Sprint
  {
  public:
    char  m_caKey[C_4KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Key(void);
    SP_Key(Key* pKey, IdxColVec* pIdxColVec);
   ~SP_Key(void) noexcept(false);
  
    char* azKey(void);

    Stz* sz(void);
    Stz* sz(Key* pKey, IdxColVec* pIdxColVec);
  
    void vReset(void);
    void vSprintKey(Key* pKey, IdxColVec* pIdxColVec);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
