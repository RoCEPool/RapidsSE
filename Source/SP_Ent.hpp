/*============================================================================*/
/* SP_Ent.hpp - Ent sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Ent : public SPsmall
  {
  public:
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Ent(void);
    SP_Ent(Ent* pEnt, IndexDef* pIndexDef);
   ~SP_Ent(void) noexcept(false);
  
    char* azKey(void);

    Stz* sz(void);
    Stz* sz(Ent* aEnt, IndexDef* pIndexDef);
  
    void vReset(void);
    void vSprintEnt(Ent* aEnt, IndexDef* pIndexDef);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
