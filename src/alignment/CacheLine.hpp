/*============================================================================*/
/* CacheLine.hpp -- a cacheline                                               */
/*============================================================================*/
#pragma once

namespace Alignment
{
  class CacheLine : public Line
  {
  public:
    union {
      byte m_yaByte[C_BYTES_PER_CACHE_LINE];
      //SingleWord m_waWord; //! this doesnt work
    };
  
  public:
    CacheLine(void);
   ~CacheLine(void) noexcept(false);
  
    static bool bMultipleCacheLines(i8 nBytes);
    static bool bOnCacheLineBoundary(void* aAddress);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline CacheLine* _aCacheLine(void* aAddr)
  {
    CacheLine* aCacheLine = reinterpret_cast<CacheLine*>(aAddr);
    
    return aCacheLine;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
