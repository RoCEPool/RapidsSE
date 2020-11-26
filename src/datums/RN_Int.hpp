/*============================================================================*/
/* RN_Int.hpp - Int random helper                                             */
/*============================================================================*/
#pragma once

namespace Datums
{
  class RN_Int : public Rand
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    RN_Int(void);
    RN_Int(Datum_h* pDatum, Column* pCol);
    RN_Int(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
   ~RN_Int(void) noexcept(false);
  
    void vRnd(Datum_h* pDatum, Column* pCol);
    void vRnd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
