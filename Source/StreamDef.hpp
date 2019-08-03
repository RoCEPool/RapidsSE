/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class StreamDef : public Definition
  {
  private:
    s32    m_sName;   // name
    Column m_xaColumn[C_MAX_COLUMNS];

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
