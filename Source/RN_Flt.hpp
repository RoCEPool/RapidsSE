/*============================================================================*/
/* RN_Flt.hpp - Flt random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Flt : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Flt(void);
    RN_Flt(Datum_h* pDatum, Column* pCol);
    RN_Flt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Flt(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
