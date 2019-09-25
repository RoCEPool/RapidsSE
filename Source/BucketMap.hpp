/*============================================================================*/
/* BucketMap.hpp -- Bucket (to partition) Map                                 */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class BucketMap : public Object
  {
  private:
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    BucketMap(void);
   ~BucketMap(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };
  
  BucketMap* _pBucketMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
