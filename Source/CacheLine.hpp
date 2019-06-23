/*============================================================================*/
/* CacheLine.hpp -- a cacheline                                               */
/*============================================================================*/
#pragma once

namespace Base
{
  class CacheLine : public Line
  {
  public:
    union {
      byte m_yaByte[C_BYTES_PER_CACHE_LINE];
      //Word m_waWord; //! this doesnt work
    };
  
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
