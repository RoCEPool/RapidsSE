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
    
    i8 m_nInfoSets;
    i8 m_nInfoValues;
    
    union {
      i8 m_i_Name;
      i8 m_i_Value;
    } m_xInfo[C_INFO_VALUES];
    
    b4K  m_yxStrings;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    ~InfoSet(void) noexcept(false);
    
    void vAddValue(i8 iValue);
    void vAddValue(Stz* szValue);
    void vIncSet(void);
    void vPrint(StreamDef* pStreamDef);
    i8   nValues(void);
    i8   nSets(void);
  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
