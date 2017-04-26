/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Key
  {
  private:
  //i8  m_nSize;
  Wsp m_xWsp;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Key*);
  
    Key(KeyWsp& xKeyWsp);
    ~Key(void);
  
    bool operator <(Key* pKeyArg);
    bool operator <=(Key* pKeyArg);
    
    void  vShow(Column* pColumn);
  
    void  vKey(Key& xKey);
    void  vKey(KeyWsp& xKeyWsp);
  
    char* aElement(Column* pColumn, i8& nBytes, char** aaCursor);
  
    char* aWspArea(void);
    Str*  aWspStr(void);
  };
  
  Key* _aKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
