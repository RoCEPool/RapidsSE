/*============================================================================*/
/* InfoSetBase.hpp -- Information Set Base                                    */
/*============================================================================*/
#pragma once

#include "RowBuffer.hpp"

namespace STORE
{
  static const i8 C_INFO_VALUES = 256;
  
  class InfoSetBase : public RowBuffer
  {
  protected:
    s32 m_sInfoSetNm;
    i8  m_nRowSize;
    i8  m_nInfoRows;
    
    // must be last
    Footer m_xFooter;

  public:
  
    InfoSetBase(eCN eClassNo);
    InfoSetBase(eCN eClassNo, StreamDef* pStreamDef);
    ~InfoSetBase(void) noexcept(false);
    
    Row* pAllotRow(void);
    Row* pAllotRow(i8 iVargsBegin, ...);
    Row* pAllotRow(StreamDef* pStreamDef, i8 iVargsBegin, ...);
    Row* pFirstRow();
    i8   nInfoRows(void);
    Stz* szInfoSetNm(void);
    void vInfoSetNm(Stz* szInfoSetNm);
    Row* pNextRow(Row* pPrevRow);
    
  //private:
    i8   iToOffset(Stz* aItem);
  
    Footer* aFooter(void);
  };

  InfoSetBase* _pInfoSetBase(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
