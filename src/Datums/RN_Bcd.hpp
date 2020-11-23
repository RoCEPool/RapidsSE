/*============================================================================*/
/* RN_Bcd.hpp - Bcd random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Bcd : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Bcd(void);
    RN_Bcd(Datum_h* pDatum, Column* pCol);
    RN_Bcd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Bcd(void) noexcept(false);
 
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
