/*============================================================================*/
/* RN_Tsp.hpp - Tsp random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Tsp : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Tsp(void);
    RN_Tsp(Datum_h* pDatum, Column* pCol);
    RN_Tsp(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Tsp(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
   Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
