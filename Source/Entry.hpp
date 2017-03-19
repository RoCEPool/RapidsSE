/*============================================================================*/
/* Entry.hpp -- index entry                                                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Entry
  {
  private:
    RowId  m_xRowId;  // rowid
    i8     m_nLevels; // no of levels
    WspVbl m_xWsp;    // workspace
  
  public:
    static i8 iBytesRequired(i8 iDataBytes);
  
    void* operator new(size_t);
    void* operator new(size_t, Entry*);
    
    Entry(i8 iDataBytes);
    Entry(EntWsp& xEntWsp);
    ~Entry(void);
    
    void  vContentDump(void);

    RowId& xRowId(void);
    void   vRowId(RowId& xRowId);

    void   vEntry(EntWsp& xEntWsp);

    Key&   xKey(void);
    Key*   pKey(void);
    void   vKey(Key& xKey);
    void   vKey(KeyWsp& xKeyWsp);
  
    i8     nLevels(void);
    void   vLevels(i8 nLevels);
  
    //------
  
    i8    nSize(void);
  
    Footer* pFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
