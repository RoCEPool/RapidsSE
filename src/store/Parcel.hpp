//
// Created on 4/20/21.

#pragma once

namespace STORE {

  static const i8 C_MAX_PARCEL_OBJ = 16;

  class Parcel : public Operand {
  private:

    i8 m_nObjects;                        // number of objects in the parcel
    Object* m_pObjects[C_MAX_PARCEL_OBJ];  // let's for now, a fixed size array of C_MAX_PARCEL_OBJ

    // I NEED TO PAD SOME BYTES OR ELSE the cache line will complain in debug mode

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Parcel(void);
    Parcel(i8 nObs);
    ~Parcel(void) noexcept(false);

    void vAddObject(Object *pObj);
    i8 nObjects(void) { return m_nObjects;};
    Object* pGetObject(i8 iIdx);

    Footer* aFooter(void);

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Parcel *_pParcel(void *aAddr);
}

