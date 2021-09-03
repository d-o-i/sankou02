//=============================================================================
//
// 使いそうな色まとめ [MyColor.h]
// Author : 
//
//=============================================================================
#ifndef _MYCOLOR_H_
#define _MYCOLOR_H_

// 下の配列の番号とリンク
enum MyColorNum{
	nWhite,
	nRed,
	nVermilion,
	nOrange,
	nPurple,
	nBlue,
	nDeepSkyBlue,
	nCyan,
	nYellow,
	nGreen,

	nColorMax
};

static D3DXCOLOR MyColor[nColorMax] = {
	D3DXCOLOR(1.00f, 1.00f, 1.00f, 1.00f),	// white
	D3DXCOLOR(1.00f, 0.00f, 0.00f, 1.00f),	// red
	D3DXCOLOR(1.00f, 0.20f, 0.00f, 1.00f),	// vermilion
	D3DXCOLOR(1.00f, 0.40f, 0.00f, 1.00f),	// orange
	D3DXCOLOR(0.40f, 0.00f, 0.20f, 1.00f),	// purple
	D3DXCOLOR(0.00f, 0.00f, 1.00f, 1.00f),	// blue
	D3DXCOLOR(0.00f, 0.75f, 1.00f, 1.00f),	// deepskyblue
	D3DXCOLOR(0.00f, 1.00f, 1.00f, 1.00f),	// cyan
	D3DXCOLOR(1.00f, 0.94f, 0.00f, 1.00f),	// yellow
	D3DXCOLOR(0.00f, 1.00f, 0.00f, 1.00f),	// green
};

#endif
