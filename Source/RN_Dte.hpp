/*============================================================================*/
/* RN_Dte.hpp - Dte random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Dte : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Dte(void);
    RN_Dte(Datum_h* pDatum, Column* pCol);
    RN_Dte(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Dte(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
