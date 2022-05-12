/*============================================================================*/
/* SP_Ent.hpp - Ent sprint helper                                             */
/*============================================================================*/
#pragma once

#include "IndexDef.hpp"

#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"

namespace STORE
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
  
    char* aEVey(void);

    Stz* sz(void);
    Stz* sz(Ent* aEnt, IndexDef* pIndexDef);
  
    void vSprintEnt(Ent* aEnt, IndexDef* pIndexDef);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
