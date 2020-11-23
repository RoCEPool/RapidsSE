/*============================================================================*/
/* InfoSetBase.hpp -- Information Set Base                                    */
/*============================================================================*/
#pragma once

namespace InfoData
{
  static const i8 C_INFO_VALUES = 256;
  
  class InfoSetBase : public RowBuffer
  {
  protected:
    s32 m_sInfoSetNm;
    i8  m_nRowSize;
    i8  m_nInfoRows;
    
  public:
  
    InfoSetBase(eCN eClassNo);
    InfoSetBase(eCN eClassNo, StreamDef* pStreamDef);
    ~InfoSetBase(void) noexcept(false);
    
    Row* aAllotRow(void);
    Row* aAllotRow(RowSingle* pRowSingle);
    Row* aAllotRow(StreamDef* pStreamDef, i8 iVargsBegin, ...);
    Row* aFirstRow();
    i8   nInfoRows(void);
    Stz* szInfoSetNm(void);
    void vInfoSetNm(Stz* szInfoSetNm);
    Row* aNextRow(Row* pPrevRow);
    
  //private:
    i8   iToOffset(Stz* aItem);
  };

  InfoSetBase* _pInfoSetBase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
