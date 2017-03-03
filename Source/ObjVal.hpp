/*============================================================================*/
/* ObjVal.hpp - object value                                                  */
/*============================================================================*/
#pragma once

namespace Values
{
  class ObjVal : public Value
  {
  private:
    i8   m_iSize;
    
  public:
    void* operator new(size_t);
    
    ObjVal(i8 iSize);
    i8 nSize(void) const;
  };
  
  ObjVal* _pObjVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
