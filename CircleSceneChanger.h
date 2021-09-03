//=============================================================================
//
// �~�`�V�[���؂�ւ� [CircleSceneChanger.h]
// Author :  
// 
//=============================================================================
#ifndef _CIRCLESCENECHANGER_H_
#define _CIRCLESCENECHANGER_H_

#include "SceneChanger.h"
#include "BaseSingleton.h"
//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CircleSceneChanger :
	public SceneChanger,
	public BaseSingleton<CircleSceneChanger>
{
	friend class BaseSingleton<CircleSceneChanger>;

private:
	int cntFrame;			// �V�[���`�F���W�ɂ����鎞��

public:
	CircleSceneChanger() { cntFrame = 0; }
	void Update()override;
	void SetChanger(bool isStart, std::function<void(void)> func = NULL);

};

#endif
