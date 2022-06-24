#pragma once
#define MRIO_IMAGE_MAX 9
#include "Input.h"

class Player : Input{
	static int mImagePlayer[MRIO_IMAGE_MAX];    //画像ハンドル格納用変数
	int change_walk[17] = {8,7,7,6,2,3,4,5,3,5,4,3,5,4,3,5,4}; //歩きの画像変更フレーム
	int change_run[10] = {8,7,4,3,4,4,4,3,4,2}; //歩きの画像変更フレーム 
	int array_num;
	int player_num;
	int cnt;
	int cnt_limit;
	bool turn = false; // true:左に向く false:右に向く

	int time;
	int stop_time;

	float speed;
	float moveX;
	float p_posX; //プレイヤーのポジション
	float now_posX;
	int N;
	bool dash;
	bool idle;

	void Turn(); //振り向き処理
	void Walk(); //歩き
	void Dash(); //走り
	void Stop();
	void ChangeImage(); //画像切り替え処理
	void ShowDebug(); //デバッグ表示処理
public:
	void Player_Initialize();//初期化
	void Player_Finalize();//終了処理
	void Player_Update();//更新
	void Player_Draw();//描画
};

