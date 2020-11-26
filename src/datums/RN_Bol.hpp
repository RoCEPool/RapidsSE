/*============================================================================*/
/* RN_Bol.hpp - Bol random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Bol : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Bol(void);
    RN_Bol(Datum_h* pDatum, Column* pCol);
    RN_Bol(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Bol(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
