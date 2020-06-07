/*============================================================================*/
/* StreamDef.hpp -- Stream Definition                                         */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class StreamDef : public Definition
  {
  public:
    s32    m_sName;         // name
    s256   m_sPath;         // full name
    i8     m_iStreamId;     // unique stream identifier
    i8     m_nDefSets;      // eg. 3
    i8     m_nDefColumns[C_MAX_STREAMDEFS];
    i8     m_i_DefStart[C_MAX_STREAMDEFS];    // 0, 4, 9
    Column m_xaColumn[C_MAX_DEFCOLUMNS];
  
    i8     m_nFixedSized;   // is this a fixed size row?
    
    bool   m_bTemp;         // is this a temporary definition?
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StreamDef(void);
    StreamDef(StreamDef* pStreamDef);
   ~StreamDef(void) noexcept(false);
    
    void vAdd(StreamDef* pStreamDef);
    void vAddColumn(Stz* ColumnNm, eCT eColumnType);
    void vAsString(String* pString);
    i8 nColumns(i8 iSubSet);
    i8 iFindColumn(char* sColumnNm, i8& iColVec);
    Column* pGetColumn(i8 iColumn);
    void vName(Stz* sName);
    Stz* szName(void);
    void vToColPix(ColPix* axColPix, bool bPtnHash);
    void vToStreamPix(StreamPix* aStreamPix, bool bPtnHash);
    void vToMetadata(Stz* sMetaData);
    
    Footer* aFooter(void);
  };
  
  StreamDef* _pStreamDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
