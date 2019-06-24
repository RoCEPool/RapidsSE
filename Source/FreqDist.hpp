/*============================================================================*/
/* FreqDist.hpp - Resource Tracker                                     */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class FreqDist : public Object
  {
  public:
  
    i8 m_nBuckets;
    i8 m_nBucket[128];
    i8 m_nDist[128];

    // must be last
    Footer m_xFooter;
    
  public:
    FreqDist(void);
    ~FreqDist(void) noexcept(false);
  
    void vAdd(i8 nBytes);
    i8   nBuckets(void);
    i8   iBucket(i8 i_Index);
    i8   iDist(i8 i_Bucket);
    void vInit(i8 iValue, ...);
    i8   nSum(void);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
