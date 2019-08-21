/*============================================================================*/
/* Focal.hpp -- Focal Object                                                  */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class Focal : public Object
  {
  public:
    i8 m_i_Focal;   // focal subscript
    i8 m_iUniqueId; // id
    i8 m_i_LogPrev; // previous log entry
  
  public:
    Focal(eCN eClassNo, i8 iUniqueId);
   ~Focal(void) noexcept(false);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
