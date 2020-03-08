/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

namespace InfoData
{
  static const i8 C_INFO_VALUES = 256;
  
  class InfoSet : public Operand
  {
  protected:
    
    i8 m_nInfoRows;
    i8 m_nValuesPerRow;
    i8 m_nInfoValues;
    
    union {
      i8 m_i_Name;
      i8 m_i_Value;
    } m_xInfo[C_INFO_VALUES];
    
    b4K  m_yxStrings;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(i8 nValuesPerRow);
    ~InfoSet(void) noexcept(false);
    
    void vAddValue(i8 iValue);
    void vAddValue(Stz* szValue);
    bool bIncRows(void);
    i8   nInfoRows(void);
    void vPrint(StreamDef* pStreamDef);
    i8   iValue(i8 iRow, i8 iCol);
    Stz* szValue(i8 iRow, i8 iCol);
    i8   nValues(void);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
