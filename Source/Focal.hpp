/*============================================================================*/
/* Focal.hpp -- Focal Object                                                  */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Focal : public Object
  {
  public:
    i8 m_i_Focal;   // focal lattice subscript
    i8 m_iUniqueId; // AM next id of this focal
    i8 m_i_Log;     // log entry subscript
    i8 m_iTick_;    //
  
    i8 m_iActNo;
    i8 m_iForId;
  
  public:
    Focal(eCN eClassNo);
    Focal(eCN eClassNo, i8 iUniqueId);
   ~Focal(void) noexcept(false);
  
    void vIncActNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
