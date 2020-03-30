/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

namespace InfoData
{
  static const i8 C_INFO_VALUES = 256;
  
  class InfoSet : public RowBuffer
  {
  protected:
    s32 m_sInfoSetNm;
    i8  m_nRowSize;
    i8  m_nInfoRows;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    InfoSet(StreamDef* pStreamDef);
    ~InfoSet(void) noexcept(false);
    
    Row* aAllotRow(void);
    Row* aAllotRow(RowSingle* pRowSingle);
    Row* aAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...);
    Row* aFirstRow();
    i8   nInfoRows(void);
    Stz* szInfoSetNm(void);
    void vInfoSetNm(Stz* szInfoSetNm);
    Row* aNextRow(Row* pPrevRow);
    
  private:
    i8   iToOffset(Stz* aItem);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
