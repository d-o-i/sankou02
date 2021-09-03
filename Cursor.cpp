//=============================================================================
//
// �J�[�\���\�� [Cursor.cpp]
// Author : 
//
//=============================================================================
#include "Main.h"
#include "Cursor.h"
#include "Camera.h"
#include "Input.h"
#include "Collision.h"
#include "MyLibrary.h"
#include "DebugWindow.h"
#include "CharacterAI.h"
#include "ResourceManager.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define CURSOR_SIZE D3DXVECTOR3(75.0f, 75.0f, 0.0f)				// �T�C�Y
#define CURSOR_DIVIDE_X	(4)										// ������
#define CURSOR_DIVIDE_Y	(2)										// �c����
#define CURSOR_FIRST_POS	D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f)
#define CURSOR_SPEED	(30.0f)									// �����X�s�[�h

//=============================================================================
// �R���X�g���N�^
//=============================================================================
Cursor::Cursor(int PlayerNo, bool AIUse, CharacterAI *AIptr)
{
	ResourceManager::Instance()->GetTexture("Cursor", &D3DTexture);

	use = true;
	pos = CURSOR_FIRST_POS;
	PatternAnim = ctrlNum = PlayerNo;
	// AI�|�C���^�̐ݒ�
	if (AIUse)
	{
		this->AIUse = true;
		this->AIptr = AIptr;
	}
	else
	{
		this->AIUse = false;
		this->AIptr = nullptr;
	}

	// ���_���̍쐬
	MakeVertex();
}

Cursor::Cursor(int PlayerNo)
{
	ResourceManager::Instance()->GetTexture("Cursor", &D3DTexture);

	use = true;
	pos = CURSOR_FIRST_POS;
	PatternAnim = ctrlNum = PlayerNo;
	this->AIUse = false;
	this->AIptr = nullptr;

	// ���_���̍쐬
	MakeVertex();
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
Cursor::~Cursor()
{
	D3DTexture = NULL;
}

//=============================================================================
// �X�V����
//=============================================================================
void Cursor::Update()
{
	if (use)
	{
		// ����
		Move();

		// �X�N���[�����W���烏�[���h���W�ɕϊ�
		CalWorldPos();

		// ���_���W�̐ݒ�
		SetVertex();
	}

#if 0
#if _DEBUG
	ImGui::SetNextWindowPos(ImVec2(5, 300), ImGuiSetCond_Once);

	BeginDebugWindow("Cursor");

	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Cursor Postion"))
	{
		if (ImGui::TreeNode("ScreenPos"))
		{
			DebugText("Pos X:%.2f\nPos Y:%.2f\n", pos.x, pos.y);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("WorldPos"))
		{
			DebugText("X:%.2f\nY:%.2f\nZ:%.2f\n", WorldPos.x, WorldPos.y, WorldPos.z);
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	EndDebugWindow("Cursor");

#endif
#endif
}

//=============================================================================
// �`�揈��
//=============================================================================
void Cursor::Draw()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���e�X�g��L����
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	Device->SetRenderState(D3DRS_ALPHAREF, TRUE);
	Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// ���_�t�H�[�}�b�g�̐ݒ�
	Device->SetFVF(FVF_VERTEX_2D);

	if (use == true)
	{
		// �e�N�X�`���̐ݒ�(�|���S���̕`��O�ɓǂݍ��񂾃e�N�X�`���̃Z�b�g���s��)
		// �e�N�X�`���̃Z�b�g�����Ȃ��ƑO�ɃZ�b�g���ꂽ�e�N�X�`�����\���遨�����͂�Ȃ����Ƃ��w�肷��pDevide->SetTexture(0, NULL);
		Device->SetTexture(0, D3DTexture);

		// �|���S���̕`��
		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(Vertex2D));
	}

	// ���e�X�g�𖳌���
	Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT Cursor::MakeVertex()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();

	// ���_���W�̐ݒ�
	SetVertex();

	// rhw�̐ݒ�
	vertexWk[0].rhw = 1.0f;
	vertexWk[1].rhw = 1.0f;
	vertexWk[2].rhw = 1.0f;
	vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	SetTexture();

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�̐ݒ�
//=============================================================================
void Cursor::SetTexture()
{
	int x = PatternAnim % CURSOR_DIVIDE_X;
	int y = PatternAnim / CURSOR_DIVIDE_X;
	float sizeX = 1.0f / CURSOR_DIVIDE_X;
	float sizeY = 1.0f / CURSOR_DIVIDE_Y;

	// �e�N�X�`�����W�̐ݒ�
	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void Cursor::SetVertex()
{
	// ���_���W�̐ݒ�
	vertexWk[0].vtx = D3DXVECTOR3(pos.x, pos.y - CURSOR_SIZE.y, 0.0f);
	vertexWk[1].vtx = D3DXVECTOR3(pos.x + CURSOR_SIZE.x, pos.y - CURSOR_SIZE.y, 0.0f);
	vertexWk[2].vtx = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	vertexWk[3].vtx = D3DXVECTOR3(pos.x + CURSOR_SIZE.x, pos.y, 0.0f);
}

//=============================================================================
// ����
//=============================================================================
void Cursor::Move()
{
	if (!AIUse)
	{
		KeyMove();	// �L�[�{�[�h����
		PadMove();	// �R���g���[������
	}
	else
	{
		// �y�C���g���[�g��`��
		if (AIptr->GetCursorState() == ePaintPath)
		{
			PaintPath();
		}
		// ���̃v���C���[�̃��[�g������
		else if (AIptr->GetCursorState() == eUseBlackPaint)
		{
			DeletePath();
		}
		// �I�u�W�F�N�g�`�b�v�𔽓]������
		else if (AIptr->GetCursorState() == ePaintObjChip)
		{
			PaintObjChip();
		}

#if _DEBUG
		//KeyMove();	// �L�[�{�[�h����
#endif
	}
}

//=============================================================================
// �J�[�\���̐؂�ւ�
//=============================================================================
void Cursor::ChangeInk()
{
	// �e�N�X�`���ƃv���C���[�̃C���N�̐؂�ւ�
	// �J���[����
	if (PatternAnim == ctrlNum)
	{
		PatternAnim = ctrlNum + CURSOR_DIVIDE_X;
	}
	// �����J���[
	else
	{
		PatternAnim = ctrlNum;
	}

	//�e�N�X�`�����W���Z�b�g
	SetTexture();
}

//=============================================================================
// �L�[�{�[�h����
//=============================================================================
void Cursor::KeyMove()
{
	// �㉺
	if (GetKeyboardPress(DIK_W))
	{
		pos.y -= CURSOR_SPEED;

		// ��ʊO����
		pos.y = max(pos.y, 0.0f + CURSOR_SIZE.y);
	}
	else if (GetKeyboardPress(DIK_S))
	{
		pos.y += CURSOR_SPEED;

		// ��ʊO����
		pos.y = min(pos.y, SCREEN_HEIGHT);
	}

	// ���E
	if (GetKeyboardPress(DIK_D))
	{
		pos.x += CURSOR_SPEED;

		// ��ʊO����
		pos.x = min(pos.x, SCREEN_WIDTH - CURSOR_SIZE.x);
	}
	else if (GetKeyboardPress(DIK_A))
	{
		pos.x -= CURSOR_SPEED;

		// ��ʊO����
		pos.x = max(pos.x, 0.0f);
	}
}

//=============================================================================
// �R���g���[������
//=============================================================================
void Cursor::PadMove()
{
	vec = GetJoyStickVec(ctrlNum) / 1000.f;
	moveX = (float)GetJoyStickLeftX(ctrlNum) / 1000.0f;
	moveY = (float)GetJoyStickLeftY(ctrlNum) / 1000.0f;

	pos.x += vec * moveX * CURSOR_SPEED;
	pos.y += vec * moveY * CURSOR_SPEED;

	// ��ʊO�ɏo���ꍇ�A�Â����W�ɖ߂�
	//pos.x = clamp(pos.x, 0.0f, SCREEN_WIDTH - CURSOR_SIZE.x);
	//pos.y = clamp(pos.y, 0.0f, SCREEN_HEIGHT - CURSOR_SIZE.y);
	pos.x = clamp(pos.x, 0.0f, SCREEN_WIDTH - CURSOR_SIZE.x);
	pos.y = clamp(pos.y, CURSOR_SIZE.y, (float)SCREEN_HEIGHT);
}

//=============================================================================
// ���[���h���W���v�Z����
//=============================================================================
void Cursor::CalWorldPos()
{
	LPDIRECT3DDEVICE9 Device = GetDevice();
	CAMERA *camerawk = GetCamera();
	D3DXMATRIX ViewMtx, ProjMtx;

	Device->GetTransform(D3DTS_VIEW, &ViewMtx);
	Device->GetTransform(D3DTS_PROJECTION, &ProjMtx);

	// �J�[�\���̃X�N���[�����W�����[���h���W�֕ϊ����č��W���Z�b�g
	// �X�N���[�����W��XZ���ʂ̃��[���h���W��_�Z�o
	D3DXVECTOR3 OutPos1, OutPos2, SetPos;
	CalcScreenToWorld(&OutPos1, (int)pos.x, (int)pos.y, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &ViewMtx, &ProjMtx);
	CalcScreenToWorld(&OutPos2, (int)pos.x, (int)pos.y, 1.0f, SCREEN_WIDTH, SCREEN_HEIGHT, &ViewMtx, &ProjMtx);

	// ����p�O�p�`�|���S��
	TriangleStr triPos1, triPos2;
	triPos1 = { camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f) };

	triPos2 = { camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(-SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f),
		camerawk->at + D3DXVECTOR3(SCREEN_WIDTH, -SCREEN_HEIGHT, 0.0f) };

	// 2�_���g���ē��������ꏊ���Z�b�g����ꏊ�Ƃ���
	if (!hitCheck(&this->WorldPos, triPos1, OutPos1, OutPos2))
	{
		hitCheck(&this->WorldPos, triPos2, OutPos1, OutPos2);
	}
}

//=============================================================================
// �C���N�Œʂ�铹�����
//=============================================================================
void Cursor::PaintPath()
{
	float Distance = 0.0f;
	D3DXMATRIX WorldMtx, TransMtx;
	D3DXVECTOR3 DestPos_World;
	D3DXVECTOR2 DestPos_Screen;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// �J�[�\�����n�_�܂łɈړ����Ă��Ȃ�
	if (AIptr->GetPaintState() == eNoAction)
	{
		DestPos_World = AIptr->GetPaintStartPos();
	}
	// �y�C���g���Ă���
	else
	{
		DestPos_World = AIptr->GetPaintEndPos();
	}

	// �ڕW�̃X�N���[�����W���v�Z����
	D3DXMatrixTranslation(&TransMtx, DestPos_World.x, DestPos_World.y, DestPos_World.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// �J�[�\���ƖڕW�̍��W���v�Z����
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (Distance > pow(20.0f, 2))
	{
		// �J�[�\�����ړI�n�Ɉړ�����
		float Angle = atan2f(Vec.y, Vec.x);
		pos.x += cosf(Angle) * CURSOR_SPEED;
		pos.y += sinf(Angle) * CURSOR_SPEED;
	}
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// �y�C���g���n�܂�
		if (AIptr->GetPaintState() == eNoAction)
		{
			AIptr->SetPaintState(ePaintStart);
		}
		// �y�C���g�I��
		else
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
		}
	}
}

//=============================================================================
// ���̃v���C���[�̃y�C���g���폜
//=============================================================================
void Cursor::DeletePath(void)
{
	float Distance = 0.0f;
	D3DXVECTOR2 DestPos_Screen;
	D3DXMATRIX WorldMtx, TransMtx;
	GroupStruct *EnemyPaint = AIptr->GetEnemyPaint();

	// �폜���Ԃɍ���Ȃ�
	if (EnemyPaint->Count <= 5 || EnemyPaint->PaintPath.empty())
	{
		AIptr->SetCursorState(eNoAction);
		AIptr->SetPaintState(ePaintEnd);
		AIptr->SetFindEnemyPaint(false);
		return;
	}

	// �ڕW�̃X�N���[�����W���v�Z����
	D3DXMatrixTranslation(&TransMtx, EnemyPaint->PaintPath.at(0).x, EnemyPaint->PaintPath.at(0).y, EnemyPaint->PaintPath.at(0).z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// �J�[�\���ƖڕW�̍��W���v�Z����
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (AIptr->GetPaintState() == eNoAction)
	{
		if (Distance > pow(20.0f, 2))
		{
			// �J�[�\�����ڕW�Ɉړ�����
			float Angle = atan2f(Vec.y, Vec.x);
			pos.x += cosf(Angle) * CURSOR_SPEED;
			pos.y += sinf(Angle) * CURSOR_SPEED;
		}
		else
		{
			// �y�C���g���n�܂�
			AIptr->SetPaintState(ePaintStart);
		}
	}
	// �y�C���g���Ă���
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// Vector�̈�Ԗڂ̍��W������
		EnemyPaint->PaintPath.erase(EnemyPaint->PaintPath.begin());

		// �y�C���g�I��
		if (EnemyPaint->PaintPath.empty())
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
			AIptr->SetFindEnemyPaint(false);
			EnemyPaint->Count = 0;
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�`�b�v���y�C���g����
//=============================================================================
void Cursor::PaintObjChip(void)
{
	float Distance = 0.0f;
	D3DXMATRIX WorldMtx, TransMtx;
	D3DXVECTOR3 DestPos_World;
	D3DXVECTOR2 DestPos_Screen;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&WorldMtx);

	// �J�[�\�����n�_�܂łɈړ����Ă��Ȃ�
	if (AIptr->GetPaintState() == eNoAction)
	{
		DestPos_World = AIptr->GetPaintStartPos();
	}
	// �y�C���g���Ă���
	else
	{
		DestPos_World = AIptr->GetPaintEndPos();
	}

	// �ڕW�̃X�N���[�����W���v�Z����
	D3DXMatrixTranslation(&TransMtx, DestPos_World.x, DestPos_World.y, DestPos_World.z);
	D3DXMatrixMultiply(&WorldMtx, &WorldMtx, &TransMtx);
	DestPos_Screen = WorldToScreenPos(WorldMtx);

	// �J�[�\���ƖڕW�̍��W���v�Z����
	D3DXVECTOR2 Vec = D3DXVECTOR2(DestPos_Screen - (D3DXVECTOR2)pos);
	Distance = D3DXVec2LengthSq(&Vec);

	if (Distance > pow(20.0f, 2))
	{
		// �J�[�\�����ڕW�Ɉړ�����
		float Angle = atan2f(Vec.y, Vec.x);
		pos.x += cosf(Angle) * CURSOR_SPEED;
		pos.y += sinf(Angle) * CURSOR_SPEED;
	}
	else
	{
		pos.x = DestPos_Screen.x;
		pos.y = DestPos_Screen.y;

		// �y�C���g���n�܂�
		if (AIptr->GetPaintState() == eNoAction)
		{
			AIptr->SetPaintState(ePaintStart);
		}
		// �y�C���g�I��
		else
		{
			AIptr->SetCursorState(eNoAction);
			AIptr->SetPaintState(ePaintEnd);
		}
	}
}

