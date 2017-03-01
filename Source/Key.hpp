/*============================================================================*/
/* Key.hpp -- key part of an index entry                                      */
/*============================================================================*/

#pragma once

#include "DBobjects.h"

namespace DBobjects
{
  class Key : public Wsp
  {
  private:
    // ..dataArea..
    
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
    
    i8    nSize(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
