/*============================================================================*/
/* JobId.hpp -- Node.Job identifier                                           */
/*============================================================================*/
#pragma once

#include "SEtype.hpp"

namespace BASE
{
  class JobId : public SEtype
  {
    public:
    union {
      struct {
        u8 m_uCmd  : 48 ; // 0..256G
        u8 m_uNode : 16 ; // 0..64K
        //          ---
        //           64
        //          ===
      };
      i8 m_iAsI8;
    };

    JobId(void);
    JobId(i8 iAsI8);
    
    void operator =(i8 iAsI8);
    void operator =(JobId tJobId);

    bool operator ==(JobId tArg);
    bool operator >(JobId tArg);

    i8   iAsI8(void);
  
    bool bIsUnknown(void);

    i8   nJobBits(void);
    u8   uJobMask(void);
    u8   uJobNo(void);
    void vJobNo(u8 uJobNo);
    
    void vNextJobId(void);
  
   u8 uNodeId(void);

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
