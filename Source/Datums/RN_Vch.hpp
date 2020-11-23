/*============================================================================*/
/* RN_Vch.hpp - Vch random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Vch : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Vch(void);
    RN_Vch(Datum_h* pDatum, Column* pCol);
    RN_Vch(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Vch(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
