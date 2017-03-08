/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Key
  {
  private:
    Wsp m_xWsp;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
    
    Key(KeyWsp& xKeyWsp);
    ~Key(void);
  
    bool operator <(Key* pKeyArg);
    bool operator <=(Key* pKeyArg);
    
    void  vShow(void);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWsp& xKeyWsp);
  
    char* sWspArea(void);
    Str*  aWspArea(void);
    
    i8    nSize(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
