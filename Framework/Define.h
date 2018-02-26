#pragma once

// singleton include list
#include "CameraClass.h"

// define list
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#define SAFE_DELETE_DESTROY(p) { if(p) { (p)->Destroy(); delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p) = NULL; } }

//#define PIXEL_COLOR_BUFFER