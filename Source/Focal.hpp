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
    i8 m_i_Log;     // log entry subscript
  
    i8 m_iUniqueId; // AM next id of this focal
    i8 m_iForId;    // for which focal owner
  
  private:
    i8 m_iActNo;    // last action sequence number
    i8 m_iActTk;    // time point that occurred

  public:
    Focal(eCN eClassNo);
    Focal(eCN eClassNo, i8 iUniqueId, i8 iForId);
   ~Focal(void) noexcept(false);
  
    i8   iActNo(void);
    void vIncActNo(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
