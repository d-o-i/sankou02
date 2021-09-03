#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Billboard.h"
class Particle :
	public Billboard
{
private:
	bool						use;			// 使用しているかどうか
	int							time;			// 発生時間
	float						decAlpha;		// アルファの減衰値
	static LPDIRECT3DTEXTURE9	D3DTexture;

	HRESULT MakeVertex();
	void SetVertex();
	void SetColor();
	void SetTexture();

public:
	Particle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, D3DXVECTOR3 scl, int time);
	~Particle();

	void Update()override;
	void Draw()override;

	bool GetUse() { return use; };

	static void ReleaseTexture() { SAFE_RELEASE(D3DTexture); };

};

#endif
