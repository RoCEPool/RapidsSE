/*============================================================================*/
/* StackObject.hpp -- On TypedStack-Only Proper Object in system                   */
/*============================================================================*/
#pragma once

namespace BASE
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
