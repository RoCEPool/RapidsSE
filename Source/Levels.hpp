/*============================================================================*/
/* levels.hpp -- array of level pointers/references/offsets                   */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class Levels
  {
  private:
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, Levels*);
    
    Levels(void);
    ~Levels(void);
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
