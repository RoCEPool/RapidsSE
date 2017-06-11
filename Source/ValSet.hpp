/*============================================================================*/
/* ValSet.hpp - set of values                                                 */
/*============================================================================*/
#pragma once

namespace DBobjects {
  class EntryBuffer;
}

namespace Values
{
  class ValSet : public Value
  {
    static const i8 C_VALSET_DATAAREA = C_1K * 63; // has to fit inside ValWrk
    
  private:
    i8  m_iBytes; // bytes used in work space
    DBobjects::EntryBuffer* m_pRowBuffer;
    i8  m_iRowBufferBytes;
    char m_wWorkspace[C_VALSET_DATAAREA];
    
  public:
    void* operator new(size_t);
    ValSet(void);

    void vAppend(Str*& pCursor, Value* pValue);
    bool bExtractValue(Str*& pCursor, Value*& pValue);
    bool bExtractValue(Str*& pCursor, eCN& eClassNo, Value*& pValue);
    DBobjects::EntryBuffer* pRowBuffer(void);
    void vRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    
    DBobjects::EntryBuffer* pGetRowBuffer();
    bool bHasRowBuffer();
    bool bSetRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    
    i8   nSize(void);
    
    void vToString(Str* aWorkspace);

  private:
    Str* aFirstByte(void);
    Str* aLastByte(void);
    ValWrk* pToValWrk(Str* pCursor);
  };
  
  ValSet* _pValSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
