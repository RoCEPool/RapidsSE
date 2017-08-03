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
    void  operator delete(void*);
    ValSet(void);
    ~ValSet(void);

    void vAppend(Stz*& pCursor, Value* pValue);
    bool bExtractValue(Stz*& pCursor, Value*& pValue);
    bool bExtractValue(Stz*& pCursor, eCN& eClassNo, Value*& pValue);
    DBobjects::EntryBuffer* pRowBuffer(void);
    void vRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    
    DBobjects::EntryBuffer* pGetRowBuffer();
    bool bHasRowBuffer();
    bool bSetRowBuffer(DBobjects::EntryBuffer* pRowBuffer);
    
    i8   nSize(void);
    
    void vToString(Stz* aWorkspace);

  private:
    Stz* aFirstByte(void);
    Stz* aLastByte(void);
    ValWrk* pToValWrk(Stz* pCursor);
  };
  
  ValSet* _pValSet(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
