/*============================================================================*/
/* RN_Ivl.hpp - Ivl random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Ivl : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Ivl(void);
    RN_Ivl(Datum_h* pDatum, Column* pCol);
    RN_Ivl(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Ivl(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
