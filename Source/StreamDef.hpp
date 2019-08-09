/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class StreamDef : public Definition
  {
  public:
    s32  m_sName;   // name
    s256 m_sPath;   // full name
    i8   m_iStreamId; // unique stream identifier

    ArrayNL_t<Column,C_MAX_COLUMNS> m_oColAry;
  
    bool m_bTemp;   // is this a temporary definition?

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);
    
    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
