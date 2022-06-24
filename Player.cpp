#include "DxLib.h"
#include "Player.h"
#include "Input.h"
#include <cstdlib>

#define PI    3.1415926535897932384626433832795f
#define WALK_MAX 16
#define RUN_MAX 9
#define CNT_LIMIT_MAX 8

int Player::mImagePlayer[MRIO_IMAGE_MAX] = {}; //static�ϐ��͌Ăяo���Ƃ��͕K������ȏ��������K�v(�O���[�o���ϐ��̗l�ɏ���)

void Player::Player_Initialize()//������
{
	LoadDivGraph("images/char/mario.png", MRIO_IMAGE_MAX, MRIO_IMAGE_MAX, 1, 32, 32, mImagePlayer);
	player_num = 0;
	cnt = 0;
	cnt_limit = CNT_LIMIT_MAX;
	array_num = 0;

	speed = 0.1f;
	moveX = 1.0f;
	p_posX = 320;
	now_posX = p_posX;
	N = 7;
	dash = false;
	idle = true;

	time = 0;
	stop_time = 0;
}

void Player::Player_Finalize()//�I������
{
	for (int i = 0; i < MRIO_IMAGE_MAX; i++) {
		DeleteGraph(mImagePlayer[i]);
	}
}

void Player::Player_Update()//�X�V
{
	/* �_�b�V���t���O�̐؂�ւ� */
	if (iNowKey & PAD_INPUT_A && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		dash = true;
	}
	else {
		dash = false;
	}

	Turn(); //�U���������

	/* �ړ����� */
	if (dash) { //���鎞
		Dash();
	}
	else { //������    
		Walk();
	}
}

void Player::Player_Draw()//�`��
{
	if (!idle) { //�ړ����Ă��鎞
		if (dash) {	//�_�b�V���t���O��true�̎�
			if (time >= 4) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
		}
		else { //�_�b�V���t���O��false�̎�
			if (time >= 7) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
			}
		}
	}
	else { //�~�܂鎞
		if (speed > 0.1) {
			DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
		}
		else {
			DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //�v���C���[�̕\���ʒu
		}
	}

	ShowDebug();
}

void Player::ChangeImage() {
	if (!dash) { //����
		if (++cnt >= cnt_limit) {
			cnt = 0;
			if (++player_num > 3) {
				player_num = 1;
			}
			if (++array_num > WALK_MAX) array_num = WALK_MAX;
			cnt_limit = change_walk[array_num];
		}
	}
	else { //����
		if (++cnt >= cnt_limit) {
			cnt = 0;
			if (++player_num > 3) {
				player_num = 1;
			}
			if (++array_num > RUN_MAX) array_num = RUN_MAX;
			cnt_limit = change_run[array_num];
		}
	}

	
	/*if (++cnt > 4) {
		cnt = 0;
		if (++player_num > 3) player_num = 1;
	}*/
}

void Player::Turn() {
	if (iNowKey & PAD_INPUT_LEFT) {
		turn = true;
		moveX = -1.0f;
	}
	else if (iNowKey & PAD_INPUT_RIGHT) {
		turn = false;
		moveX = 1.0f;
	}
}

void Player::Walk() { //��������
	if (dash == false && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
			speed = 0.1f;
		}
		
		ChangeImage();//�摜�؂�ւ�����

		time++;

		/* �������� */
		if (abs(speed) < 3) {
			if (time == 11) {
				N = 11;
			}
			else if (time >= 40) {
				DrawFormatString(0, 240, GetColor(255, 255, 255), "time��40�ȏ�ł�");
				N = 1;
				speed = 3;
			}
			else if (time >= 23) {
				speed = 2;
				if (time % 10 == 8) {
					N = 2;
				}
				else {
					N = 1;
				}
			}
			else if (time > 11) {
				speed = 1;
				if (time % 10) { //10�J�E���g���ɂ͗�O�I�ɐi�߂�
					N = 1;
				}
				else {
					N = 2;
				}
			}
		}

		if (time % N == 0) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
		}
	}
	else {
		Stop();

		///* Dash.Walk���悤�ϐ� */
		//player_num = 0;
		//speed = 0.1f;
		//time = 0;
		//N = 7;

		///* ChangeImage�g�p�ϐ� */
		//array_num = 0;
		//cnt_limit = CNT_LIMIT_MAX;
		

		/*DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂���");
		Stop(speed);*/
		/*if (speed == 0.1f) {
			player_num = 0;
			time = 0;
			N = 7;
			array_num = 0;
			cnt_limit = CNT_LIMIT_MAX;
		}
		else {
			Stop(speed);
		}*/
	}
}

void Player::Dash() { //���鏈��
	if (dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
		}
		ChangeImage();//�摜�؂�ւ�����

		time++;

		/* �������� */
		if (abs(speed) < 5) {
			if (time < 8) {
				N = 4;
			}
			else if (time >= 33) {
				N = 1;
				speed = 5;
			}
			else if (time >= 23) {
				speed = 4;
				if (time <= 25) {
					N = 2;
				}
				else {
					N = 1;
				}
			}
			else if (time >= 17) {
				speed = 3;
				if (time <= 19) {
					N = 2;
				}
				else {
					N = 1;
				}
			}
			else if (time >= 14) {
				speed = 2;
				N = 1;
			}
			else if (time > 8) {
				speed = 1;
				N = 2;
			}
		}

		if (time % N == 0) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
		}
		else {
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂���");
		}
	}
	else {
		Stop();
		/*player_num = 0;
		speed = 0.1f;
		time = 0;
		N = 7;
		array_num = 0;
		cnt_limit = CNT_LIMIT_MAX;*/
	}

}

void Player::Stop() { //�~�܂鎞�̏���
	DrawFormatString(0, 210, GetColor(255, 255, 255), "Stop�����ɓ����Ă��܂�");
	idle = true; //�~�܂鎞�̃t���O��true�ɂ���

	if (speed > 0.1f) {
		++stop_time;
		time = 0;

		//DrawFormatString(0, 190, GetColor(255, 255, 255), "Stop�����o�� : %d",i);

		/* �������� */
		if (abs(speed) > 0.1f) {
			if (stop_time < 2) {
				N = 2;
			}
			else if (stop_time > 16) {
				speed = 1.0f;
				stop_time = 16;
				player_num = 0; //�v���C���[�̉摜���~�܂��Ă��鎞�̉摜�ɂ���
				time = 0;
				N = 7;
				array_num = 0;
				cnt_limit = CNT_LIMIT_MAX;
			}

			else if (stop_time > 14) {
				speed = 0.15f;
			}
			else if (stop_time > 12) {
				N = 4;
				speed = 0.25f;
			}
			else if (stop_time > 8) {
				N = 2;
				speed = 0.5f;
			}
			else if (stop_time > 2) {

				if (stop_time > 4 && stop_time < 7) {
					N = 3;
				}
				else {
					N = 1;
				}
			}
		}

		if (speed > 0.1f && stop_time % N == 0) {
			p_posX = p_posX + (speed * moveX); //�v���C���[�̈ړ�
			now_posX = p_posX;
			DrawFormatString(0, 170, GetColor(255, 255, 255), "���ݐi��ł��܂�");
		}
	}
	

	
	
	
}

void Player::ShowDebug() { //�f�o�b�O�\��
	DrawFormatString(0, 30, GetColor(255, 255, 255), "���͎���:%d", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "�v���C���[�̉摜�ԍ�:%d", player_num);
	DrawFormatString(0, 70, GetColor(255, 255, 255), "���݂̓���[��:2,�E:4]:%d", iNowKey);
	DrawFormatString(0, 90, GetColor(255, 255, 255), "�摜�̌���[0:�E,1:��]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "�X�s�[�h%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed�v�Z%f = %f + (%f * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "N�̒l : %d",N);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
}