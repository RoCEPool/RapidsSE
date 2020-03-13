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
    
    Row* aAllotRow(void);
    Row* aAllotRow(RowSingle* pRowSingle);
    Row* aFirstRow();
    i8   nInfoRows(void);
    Row* aNextRow(Row* pPrevRow);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
