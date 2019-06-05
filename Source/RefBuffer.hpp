/*============================================================================*/
/* RefBuffer.hpp -- Reference Buffer                                          */
/*============================================================================*/
#pragma once

namespace Data
{
  class RefBuffer : public Object
  {
  protected:
  
    // must be last
    Footer m_xFooter;
  
  public:
    RefBuffer(void);
    ~RefBuffer(void) noexcept(false);

  private:
    Footer* aFooter(void);

  };
  
  RefBuffer* _pRefBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
