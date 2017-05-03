/*============================================================================*/
/* ValSet.hpp - set of values                                                 */
/*============================================================================*/
#pragma once

namespace DBobjects {
  class RowBuffer;
}

//#define OLD_VALSET

namespace Values
{
  class ValSet : public Value
  {
    static const i8 C_VALSET_DATAAREA = C_1K * 63; // has to fit inside valwsp
    
  private:
    i8  m_iBytes; // bytes used in work space
    DBobjects::RowBuffer* m_pRowBuffer;
    i8  m_iRowBufferBytes;
    char m_wWorkspace[C_VALSET_DATAAREA];
    
  public:
    void* operator new(size_t);
    ValSet(void);

    void vAppend(Str*& pCursor, Value* pValue);
#ifdef OLD_VALSET
    bool bExtractValWsp(Str*& pCursor, ValWsp*& pValWsp);
    bool bExtractValWsp(Str*& pCursor, eCN& eClassNo, ValWsp*& pVal);
#else
    bool bExtractValue(Str*& pCursor, Value*& pValue);
    bool bExtractValue(Str*& pCursor, eCN& eClassNo, Value*& pValue);
#endif
    DBobjects::RowBuffer* pRowBuffer(void);
    void vRowBuffer(DBobjects::RowBuffer* pRowBuffer);
    
    DBobjects::RowBuffer* pGetRowBuffer();
    bool bHasRowBuffer();
    bool bSetRowBuffer(DBobjects::RowBuffer* pRowBuffer);
    
    i8   nSize(void);
    
    void vToString(Str* aWorkspace);

  private:
    Str* aFirstByte(void);
    Str* aLastByte(void);
    ValWsp* pToValWsp(Str* pCursor);
  };
  
  ValSet* _pValSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
