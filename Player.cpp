#include "DxLib.h"
#include "Player.h"
#include "Input.h"
#include <cstdlib>

#define PI    3.1415926535897932384626433832795f
#define WALK_MAX 16
#define RUN_MAX 9
#define CNT_LIMIT_MAX 8

int Player::mImagePlayer[MRIO_IMAGE_MAX] = {}; //static変数は呼び出すときは必ず特殊な初期化が必要(グローバル変数の様に書く)

void Player::Player_Initialize()//初期化
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

void Player::Player_Finalize()//終了処理
{
	for (int i = 0; i < MRIO_IMAGE_MAX; i++) {
		DeleteGraph(mImagePlayer[i]);
	}
}

void Player::Player_Update()//更新
{
	/* ダッシュフラグの切り替え */
	if (iNowKey & PAD_INPUT_A && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		dash = true;
	}
	else {
		dash = false;
	}

	Turn(); //振り向き処理

	/* 移動処理 */
	if (dash) { //走る時
		Dash();
	}
	else { //歩く時    
		Walk();
	}
}

void Player::Player_Draw()//描画
{
	if (!idle) { //移動している時
		if (dash) {	//ダッシュフラグがtrueの時
			if (time >= 4) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
		else { //ダッシュフラグがfalseの時
			if (time >= 7) {
				DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
				now_posX = p_posX;
			}
			else {
				DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
			}
		}
	}
	else { //止まる時
		if (speed > 0.1) {
			DrawRotaGraph(p_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
		else {
			DrawRotaGraph(now_posX, 240, 1.0f, 0, mImagePlayer[player_num], true, turn); //プレイヤーの表示位置
		}
	}

	ShowDebug();
}

void Player::ChangeImage() {
	if (!dash) { //歩き
		if (++cnt >= cnt_limit) {
			cnt = 0;
			if (++player_num > 3) {
				player_num = 1;
			}
			if (++array_num > WALK_MAX) array_num = WALK_MAX;
			cnt_limit = change_walk[array_num];
		}
	}
	else { //走り
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

void Player::Walk() { //歩く処理
	if (dash == false && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
			speed = 0.1f;
		}
		
		ChangeImage();//画像切り替え処理

		time++;

		/* 加速処理 */
		if (abs(speed) < 3) {
			if (time == 11) {
				N = 11;
			}
			else if (time >= 40) {
				DrawFormatString(0, 240, GetColor(255, 255, 255), "timeが40以上です");
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
				if (time % 10) { //10カウント毎には例外的に進める
					N = 1;
				}
				else {
					N = 2;
				}
			}
		}

		if (time % N == 0) {
			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
		}
	}
	else {
		Stop();

		///* Dash.Walkしよう変数 */
		//player_num = 0;
		//speed = 0.1f;
		//time = 0;
		//N = 7;

		///* ChangeImage使用変数 */
		//array_num = 0;
		//cnt_limit = CNT_LIMIT_MAX;
		

		/*DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいません");
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

void Player::Dash() { //走る処理
	if (dash == true && (iNowKey & PAD_INPUT_LEFT || iNowKey & PAD_INPUT_RIGHT)) {
		if (idle == true) {
			idle = false;
			stop_time = 0;
		}
		ChangeImage();//画像切り替え処理

		time++;

		/* 加速処理 */
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
			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
		}
		else {
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいません");
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

void Player::Stop() { //止まる時の処理
	DrawFormatString(0, 210, GetColor(255, 255, 255), "Stop処理に入っています");
	idle = true; //止まる時のフラグをtrueにする

	if (speed > 0.1f) {
		++stop_time;
		time = 0;

		//DrawFormatString(0, 190, GetColor(255, 255, 255), "Stop処理経過 : %d",i);

		/* 減速処理 */
		if (abs(speed) > 0.1f) {
			if (stop_time < 2) {
				N = 2;
			}
			else if (stop_time > 16) {
				speed = 1.0f;
				stop_time = 16;
				player_num = 0; //プレイヤーの画像を止まっている時の画像にする
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
			p_posX = p_posX + (speed * moveX); //プレイヤーの移動
			now_posX = p_posX;
			DrawFormatString(0, 170, GetColor(255, 255, 255), "現在進んでいます");
		}
	}
	

	
	
	
}

void Player::ShowDebug() { //デバッグ表示
	DrawFormatString(0, 30, GetColor(255, 255, 255), "入力時間:%d", time);
	DrawFormatString(0, 50, GetColor(255, 255, 255), "プレイヤーの画像番号:%d", player_num);
	DrawFormatString(0, 70, GetColor(255, 255, 255), "現在の入力[左:2,右:4]:%d", iNowKey);
	DrawFormatString(0, 90, GetColor(255, 255, 255), "画像の向き[0:右,1:左]%d", turn);
	DrawFormatString(0, 110, GetColor(255, 255, 255), "スピード%f", abs(speed));
	DrawFormatString(0, 130, GetColor(255, 255, 255), "speed計算%f = %f + (%f * %f)", p_posX,p_posX,speed,moveX);
	DrawFormatString(0, 150, GetColor(255, 255, 255), "Nの値 : %d",N);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "stop_time : %d",stop_time);
}