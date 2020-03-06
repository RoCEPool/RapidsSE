/*============================================================================*/
/* InfoSet.hpp -- Information Set                                             */
/*============================================================================*/
#pragma once

namespace InfoData
{
  class InfoSet : public Operand
  {
  protected:
    EnameSet m_oEnameSet;

  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    InfoSet(void);
    ~InfoSet(void) noexcept(false);
    
    void vAdd(Stz* szName, i8 iValue);
    void vAdd(Stz* szName, Stz* szValue);
    void vPrintRows(StreamDef* pStreamDef);
    i8   nSets(void);
    i8   iValue(Stz* szName);
    Stz* szValue(Stz* szName);

  };

  InfoSet* _pInfoSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
