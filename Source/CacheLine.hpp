/*============================================================================*/
/* CacheLine.hpp -- a cacheline                                               */
/*============================================================================*/
#pragma once

namespace Base
{
  class CacheLine : public Line
  {
  public:
    byte m_yaByte[C_BYTES_PER_CACHE_LINE];
  
  public:
    CacheLine(void);
    ~CacheLine(void) noexcept(false);
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
