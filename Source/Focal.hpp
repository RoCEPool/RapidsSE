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
    i8 m_iUniqueId; // id
    i8 m_i_Log;     // log entry subscript
  
  public:
    Focal(eCN eClassNo, i8 iUniqueId);
   ~Focal(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
