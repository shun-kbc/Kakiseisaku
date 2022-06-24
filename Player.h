#pragma once
#define MRIO_IMAGE_MAX 9
#include "Input.h"

class Player : Input{
	static int mImagePlayer[MRIO_IMAGE_MAX];    //�摜�n���h���i�[�p�ϐ�
	int change_walk[17] = {8,7,7,6,2,3,4,5,3,5,4,3,5,4,3,5,4}; //�����̉摜�ύX�t���[��
	int change_run[10] = {8,7,4,3,4,4,4,3,4,2}; //�����̉摜�ύX�t���[�� 
	int array_num;
	int player_num;
	int cnt;
	int cnt_limit;
	bool turn = false; // true:���Ɍ��� false:�E�Ɍ���

	int time;
	int stop_time;

	float speed;
	float moveX;
	float p_posX; //�v���C���[�̃|�W�V����
	float now_posX;
	int N;
	bool dash;
	bool idle;

	void Turn(); //�U���������
	void Walk(); //����
	void Dash(); //����
	void Stop();
	void ChangeImage(); //�摜�؂�ւ�����
	void ShowDebug(); //�f�o�b�O�\������
public:
	void Player_Initialize();//������
	void Player_Finalize();//�I������
	void Player_Update();//�X�V
	void Player_Draw();//�`��
};

