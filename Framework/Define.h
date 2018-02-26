#pragma once

// singleton include list
#include "CameraClass.h"

// define list
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_DESTROY(p) { if(p) { (p)->Destroy(); delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }

//#define PIXEL_COLOR_BUFFER

// template delete
template <typename _Data>
inline void safe_delete(_Data& d)
{
    if(d)
    {
      delete (d);
      (d) = nullptr;
    }
}

template <typename _Data>
inline void safe_release(_Data& d)
{
    if(d)
    {
      (d)->Release();
      (d) = nullptr;
    }
}

template <typename _Data>
inline void safe_delete_array(_Data& d)
{
    if(d)
    {
      delete [] (d);
      (d) = nullptr;
    }
}

template <typename _Data>
inline void safe_delete_destroy(_Data& d)
{
    if(d)
    {
      (d)->Destroy();
      safe_delete(d);
    }
}
