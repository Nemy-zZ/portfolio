//========================================================================================================
//
//•Ö—˜ŠÖ”ˆ—[benlib.h]
// Author Murata Shinnosuke
//
//========================================================================================================
#ifndef _BENLIB_H_
#define _BENLIB_H_

#include "main.h"

//•Ö—˜ŠÖ”
class Benlib
{
public:
	static D3DXVECTOR3 Distance(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget);   //‘ÎÛ‚Æ‚Ì‹——£‚ÌZoˆ—
	static D3DXVECTOR3 Direction(D3DXVECTOR3 pos, D3DXVECTOR3 posTarget);   //‘ÎÛ‚Ì•ûŒü‚ÌZoˆ—
	static int CreateRand(int nMax, int nMin);   //—”‚Ì¶¬ˆ—
};

#endif // !_BENLIB_H_
