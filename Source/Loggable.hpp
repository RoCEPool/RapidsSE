/*============================================================================*/
/* Loggable.hpp -- Loggable                                                   */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class _Loggable : public Object
  {
  public:
    i8 m_i_Focal;   // focal subscript
    i8 m_iUniqueId; // id
    i8 m_i_LogPrev; // previous log entry
  
  public:
    _Loggable(eCN eClassNo, i8 iUniqueId);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
