/*============================================================================*/
/* RN_Tme.hpp - Tme random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Tme : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Tme(void);
    RN_Tme(Datum_h* pDatum, Column* pCol);
    RN_Tme(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Tme(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
