/*============================================================================*/
/* SP_Ent.hpp - Ent sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Ent : public Sprint
  {
  public:
    char  m_caEnt[C_4KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ent(void);
    SP_Ent(Ent* pEnt, IdxColVec* pIdxColVec);
   ~SP_Ent(void) noexcept(false);
  
    char* azKey(void);

    Stz* sz(void);
    Stz* sz(Ent* aEnt, IdxColVec* pIdxColVec);
  
    void vReset(void);
    void vSprintEnt(Ent* aEnt, IdxColVec* pIdxColVec);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
