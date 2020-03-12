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
    i8  m_nRowSize;
    i8  m_nInfoRows;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    InfoSet(i8 nRowSize);
    ~InfoSet(void) noexcept(false);
    
    Row2* aAllotRow(void);
    Row2* aFirstRow();
    i8    nInfoRows(void);
    Row2* aNextRow(Row2* pPrevRow);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
