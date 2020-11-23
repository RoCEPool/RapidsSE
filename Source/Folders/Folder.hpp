/*============================================================================*/
/* Folder.hpp -- Folder                                                       */
/*============================================================================*/
#pragma once

namespace Folders
{
  class Folder : public Object
  {
  private:
      
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Folder(eCN eClassNo);
   ~Folder(void) noexcept(false);
   
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
