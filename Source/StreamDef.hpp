/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class StreamDef : public Operand
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    ~StreamDef(void) noexcept(false);
    
    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
