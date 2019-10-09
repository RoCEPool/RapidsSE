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

    ArrayNL_t<ColumnVec,C_MAX_COLUMNVECS> m_oColVecAry;

    bool m_bTemp;   // is this a temporary definition?
/*
    i8     m_nDefSets;       // eg. 3
    i8     m_i_Start[16];    // 0, 4, 9
    Column m_xaColumn[256];  // colTa,ColTb,...colJa,colJb,...colKa,..

    //s32    m_saName[16];     // "T", "J", "K"
*/
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);
    
    void vAdd(StreamDef* pStreamDef);
    ColumnVec* pColumnVec(i8 iIndex);
    i8 iFindColumn(char* sColumnNm, i8& iColVec);
    void vToMetadata(Stz* sMetaData);
    
    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
