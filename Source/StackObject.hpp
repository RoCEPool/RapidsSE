/*============================================================================*/
/* StackObject.hpp -- On Stack-Only Proper Object in system                   */
/*============================================================================*/
#pragma once

namespace Correctness
{
  class StackObject : public Object
  {
  public:
    void* operator new(size_t);

    StackObject(eCN eClassNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
