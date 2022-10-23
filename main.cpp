#include <Novice.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

/*================================
	コピペ用↓
=================================*/

/*********************************
	大見出しコピペ
*********************************/

/******** 小見出しコピペ用 **********/

/*================================
	コピペ用↑
=================================*/

/******** ウィンドウ名の指定 **********/
const char kWindowTitle[] = "LC1A_16_トヨダユウキ_TD1_課題";


/******** ウィンドウサイズの指定 **********/
const int kWindowWidth = 1280; //x
const int kWindowHeight = 720; //y

/*********************************
	定数の宣言ここまで
*********************************/

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*********************************
		構造体宣言ここから
	*********************************/
#pragma region 構造体

#pragma region 二次元ベクトル

	/******** 二次元ベクトル **********/

	typedef struct vector2 {

		float x; //x
		float y; //y

	};

#pragma endregion
#pragma region 距離

	/******** 距離 **********/

	typedef struct Distance {

		vector2 distance; //距離xy
		float length; //実際距離

	};

#pragma endregion
#pragma region 画像

	/******** 画像 **********/

	typedef struct graph {

		vector2 translate;
		vector2 radius;
		vector2 imgRadius;
		vector2 drawStartArea;
		vector2 drawEndArea;
		int name;

	};

#pragma endregion
#pragma region キャラクター

	/******** キャラクター **********/
	 //位置
	 //動作方向
	 //ベクトル長さ
	 //当たり判定半径
	 //進行方向
	 //スピード
	 //スピードデフォルト値
	 //生存しているか
	 //キャラクタHP
	 //与えるダメージ
	 //画像半径
	 //色
	typedef struct chara {

		vector2 translate; //位置
		vector2 moveDirection; //動作方向
		float vectorLength; //ベクトル長さ

		float radius; //当たり判定半径
		float theta; //進行方向

		float speed; //スピード
		float defSpeed; //スピードデフォルト/

		int isAlive;
		int HP; //キャラクタHP

		int damage; //与えるダメージ

		int graph; //キャラ画像
		int graphRadius; //画像半径

		unsigned int color; //色

	};

#pragma endregion

#pragma region エフェクト
	/******** エフェクト(残像) **********/

	typedef struct trajectory {
		vector2 translate; //位置
		float radius;
		unsigned int carentAlpha;
		int isActive;
		int graph;
		int graphRadius; //画像半径
	};

	/******** エフェクト(チャージ) **********/
	typedef struct effect {
		vector2 translate; //位置
		vector2 moveDirection; //動作方向
		float vectorLength; //ベクトル長さ
		float radius;
		unsigned int carentAlpha;
		int isActive;
		int graph;
		int graphRadius; //画像半径
	};

	/******** 死亡エフェクト **********/
	typedef struct deathEffect {
		vector2 translate; //位置
		vector2 moveDirection; //動作方向
		float vectorLength; //ベクトル長さ
		float radius;
		unsigned int carentAlpha;
		int isActive;
		int graph;
		int graphRadius; //画像半径
	};

#pragma endregion

#pragma endregion
	/*********************************
		構造体宣言ここまで
	*********************************/

	/*********************************
		変数宣言ここから
	*********************************/

#pragma region 変数

#pragma region ワールド座標関係

	/******** ワールド座標原点 **********/
	vector2 worldPosOrigin{

		0, //x
		kWindowHeight //y

	};

#pragma endregion
#pragma region シーン

	enum Scenes
	{

		TITLE,
		GAME,
		RESULT

	};

	int scene = TITLE;

#pragma endregion
#pragma region スコア

	int score = 0;

	int addScore = 100;
	int defAddScore = 100;
	float scoreMagnification = 1.0f;

	int combo = 0;
	int tenCombo = 0;
	int startCombo = 10;

	float comboMagnification = 1.0f;

	float comboReceptionTime = 120.0f;
	float defComboReceptionTime = 120.0f;


#pragma endregion

#pragma region 乱数 

	unsigned int currentTime = time(nullptr);

	currentTime = time(nullptr);
	srand(currentTime);

	unsigned int effectTime = time(nullptr);
	srand(effectTime);

#pragma endregion

#pragma region スクロール関係

	/******** スクロール **********/

	//開始座標
	vector2 scroolStartPos{

		kWindowWidth - kWindowWidth / 2, //x
		worldPosOrigin.y - kWindowHeight + kWindowHeight / 2 //y

	};

	//スクロール値
	vector2 scrool{

		0.0f, //x
		0.0f //y

	};

#pragma endregion
#pragma region 背景

	/******** 背景 **********/

	int white1x1Png = Novice::LoadTexture("white1x1.png");

	int bgGraph[6];

	bgGraph[0] = Novice::LoadTexture("./resources/graph/map/BG_1.png");
	bgGraph[1] = Novice::LoadTexture("./resources/graph/map/BG_2.png");
	bgGraph[2] = Novice::LoadTexture("./resources/graph/map/BG_3.png");
	bgGraph[3] = Novice::LoadTexture("./resources/graph/map/BG_4.png");
	bgGraph[4] = Novice::LoadTexture("./resources/graph/map/BG_5.png");
	bgGraph[5] = Novice::LoadTexture("./resources/graph/map/BG_6.png");

	graph bg[6];

	for (int i = 0; i < 6; i++) {

		bg[i] = {

			kWindowWidth / 2, kWindowHeight / 2,
			kWindowWidth, kWindowHeight,
			kWindowWidth, kWindowHeight,
			0.0f, 0.0f,
			1920.0f, 1080.0f,
			bgGraph[i]

		};

	}

#pragma endregion

#pragma region プレイヤー

	/******** プレイヤー **********/
	chara player{

		kWindowWidth * 1.5f, kWindowHeight - 160.0f, //translate x, y
		0.0f, 0.0f, //moveDirection x, y
		0.0f, //vectorLength

		128.0f, //radius
		0.0f,

		5.0f, //speed
		5.0f, //defSpeed

		true,
		1, //HP

		10, //damage

		Novice::LoadTexture("./resources/graph/player/player.png"), //graph
		128, //graphRadius

		WHITE

	};

	vector2 playerPreTranslate = {

		0.0f,0.0f

	};

	vector2 playerAttackRange = {

		0.0f, 0.0f

	};

	float theta = 0.0f;

	//スペースキートリガー用
	int isPressSpace = false;

	//チャージできるか
	int canCharge = true;
	//現在チャージしているか
	int isCharging = false;
	//チャージが完了しているか
	int compCharge = false;

	//攻撃中か
	int isAttacking = false;

	//チャージされているパワー
	float chargePower = 0.0f;
	//パワーの最大値
	float maxPower = 60.0f;

	//チャージ可能までのクールタイム
	float chargeCoolTime = 0.0f;
	//チャージ可能までのクールタイムのデフォルト値
	float defChargeCoolTime = 120.0f;

	int canEnhancementAttack = false;

#pragma endregion
#pragma region 味方
	/******** 味方 **********/
	chara ally{

		kWindowWidth * 1.5f, kWindowHeight, //translate x, y
		0.0f, 0.0f, //moveDirection x, y
		0.0f, //vectorLength

		128.0f, //radius
		0.0f,

		20.0f, //speed
		5.0f, //defSpeed

		true,
		100, //HP

		5, //damage

		Novice::LoadTexture("./resources/graph/decoy/decoy.png"), //graph
		128, //graphRadius

		WHITE

	};

	int putDecoy = false;
	int canCollectDecoy = false;

	Distance p2d = {

		0.0f, 0.0f,
		0.0f

	};

	Distance e2d = {

		0.0f, 0.0f,
		0.0f

	};

#pragma endregion

#pragma region 敵

	const int kMaxEnemy = 100;

	chara enemy[kMaxEnemy];
	Distance e2e[kMaxEnemy];
	Distance ef2e[kMaxEnemy];

	enum EnemyType
	{

		FOLLOW,

	};

	int isNockBacking[kMaxEnemy];
	float nockBackSpeed[kMaxEnemy];

	int enemyType[kMaxEnemy];

	int enemyNockBack[kMaxEnemy];

	int deadEnemy[kMaxEnemy];

	for (int i = 0; i < kMaxEnemy; i++) {

		enemy[i] = {

			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f,

			64.0f,
			0.0f,

			3.0f,
			3.0f,

			false,
			1,
			1,

			Novice::LoadTexture("./resources/graph/enemy/enemy.png"),
			128,

			WHITE

		};

		enemyType[i] = FOLLOW;

		e2e[i] = {

			0.0f, 0.0f,
			0.0f

		};

		enemyNockBack[i] = 0.0f;

		isNockBacking[i] = false;
		nockBackSpeed[i] = 0.0f;

		deadEnemy[i] = false;

		ef2e[i] = {

			0.0f,0.0f,
			0.0f

		};

	}

	Distance e2p{

		0.0f, 0.0f,
		0.0f

	};

#pragma endregion

#pragma region 機械

	chara Machine{

		kWindowWidth * 1.5f, kWindowHeight, //translate x, y
		0.0f, 0.0f, //moveDirection x, y
		0.0f, //vectorLength

		256.0f, //radius
		0.0f,

		0.0f, //speed
		50.0f, //defSpeed

		true,
		1000, //HP

		1, //damage

		Novice::LoadTexture("./resources/graph/machine/Machine.png"), //graph
		256, //graphRadius

		WHITE

	};


	Distance m2p = {

		0.0f, 0.0f,
		0.0f

	};


	Distance m2d = {

		0.0f, 0.0f,
		0.0f

	};

	int tempMachineEnergy = 0;

	int isRepairing = false;
	int repairComplete = false;

	unsigned int circleColor = RED;
	int collectEnergyRange = Machine.radius * 1.5f;

#pragma endregion
#pragma region エネルギー

	const int kMaxEnergy = kMaxEnemy;
	chara energy[kMaxEnergy];

	Distance en2p[kMaxEnergy];

	float collectRange[kMaxEnergy];

	int followPlayer[kMaxEnergy];

	int colletCount[kMaxEnergy];


	for (int i = 0; i < kMaxEnergy; i++) {

		energy[i] = {

			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f,

			32.0f,
			0.0f,

			3.0f,
			3.0f,

			false,
			1,
			1,

			Novice::LoadTexture("./resources/graph/energy/Energy.png"),
			128,

			WHITE

		};

		collectRange[i] = energy[i].radius * 25.0f;

		followPlayer[i] = false;

		colletCount[i] = 0;

		en2p[i] = {

			0.0f, 0.0f,
			0.0f

		};

	}

	int energyAmount = 0;

#pragma endregion

#pragma region スポーン

	vector2 spawnPoint[4];

	spawnPoint[0] = { 0.0f, 0.0f };
	spawnPoint[1] = { 0.0f, 0.0f };
	spawnPoint[2] = { 0.0f, 0.0f };
	spawnPoint[3] = { 0.0f, 0.0f };

	float spawnTimer = 60.0f;
	float defSpawnTimer = 30.0f;

	int canSpawn = false;
	int enemySpawnPoint = 0;

	int spawnCounter = 0;
	int nowSpawnCounter = 0;
	int maxOnceSpawn = 10;

#pragma endregion
#pragma region エフェクト
#pragma region チャージ
	/******** チャージエフェクト **********/
	const int chargeEffectMax = 16;
	effect chargeEffect[chargeEffectMax];

	for (int i = 0; i < chargeEffectMax; i++)
	{
		chargeEffect[i].isActive = false;
		chargeEffect[i].vectorLength = player.radius * 2;
		chargeEffect[i].moveDirection.x = 0;
		chargeEffect[i].moveDirection.y = 0;
		chargeEffect[i].translate.x = chargeEffect[i].moveDirection.x * chargeEffect[i].vectorLength;
		chargeEffect[i].translate.y = chargeEffect[i].moveDirection.y * chargeEffect[i].vectorLength;
		chargeEffect[i].carentAlpha = 0xBA;
		chargeEffect[i].radius = 96;
		chargeEffect[i].graph = Novice::LoadTexture("./resources/graph/effect/effect.png");
		chargeEffect[i].graphRadius = 512;
	};
#pragma endregion

#pragma region 残像

	/******** エフェクト(残像) **********/
	const int afterimageMax = 30;

	trajectory afterimage[afterimageMax];

	Distance ef2en[afterimageMax];

	for (int i = 0; i < afterimageMax; i++)
	{
		afterimage[i].isActive = false;
		afterimage[i].translate.x = enemy[i].translate.x;
		afterimage[i].translate.y = enemy[i].translate.y;
		afterimage[i].carentAlpha = 0xFF;
		afterimage[i].radius = 128 / 2;
		afterimage[i].graph = Novice::LoadTexture("./resources/graph/player/player.png");
		afterimage[i].graphRadius = 128;

		ef2en[i] = {

			0.0f,0.0f,
			0.0f

		};
	};
#pragma endregion
/*****************************************************************************************************************************
										ここから
*****************************************************************************************************************************/
#pragma region 死亡

	/******** エフェクト(敵死亡) **********/

	const int deathMaxEffect = 8;
	const int playMaxDeathEffect = kMaxEnemy;
	effect deathEffect[playMaxDeathEffect][deathMaxEffect];
	int playDeathEffect[playMaxDeathEffect];

	for (int i = 0; i < playMaxDeathEffect; i++)
	{
		playDeathEffect[i] = false;
		for (int j = 0; j < deathMaxEffect; j++)
		{
			deathEffect[i][j].isActive = false;
			deathEffect[i][j].vectorLength = 0;
			deathEffect[i][j].moveDirection.x = 0;
			deathEffect[i][j].moveDirection.y = 0;
			deathEffect[i][j].translate = enemy[i].translate;
			deathEffect[i][j].carentAlpha = 0xFF;
			deathEffect[i][j].radius = 16;
			deathEffect[i][j].graph = Novice::LoadTexture("./resources/graph/effect/korosuzo.png");
			deathEffect[i][j].graphRadius = 128;
		}

	};

#pragma endregion
	/*****************************************************************************************************************************
											ここまで
	*****************************************************************************************************************************/
#pragma endregion

#pragma endregion
	/*********************************
		変数宣言ここまで
	*********************************/

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*********************************
			更新処理ここから
		*********************************/

		/*********************************
			スクリーン関係ここから
		*********************************/

		/******** フルスクリーン **********/
		//Novice::SetWindowMode(kFullscreen);

		/*********************************
			スクリーン関係ここまで
		*********************************/

		switch (scene)
		{
		case TITLE:

#pragma region シーン遷移

			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {

				scene++;

			}

#pragma endregion

			break;
		case GAME:

#pragma region プレイヤー

#pragma region 壁貫通防止
			//左右
			if (player.translate.x <= 0 + player.radius / 2) {

				player.translate.x = player.radius / 2;

			}
			else if (player.translate.x >= kWindowWidth * 3 - player.radius / 2) {

				player.translate.x = kWindowWidth * 3 - player.radius / 2;

			}

			//上下
			if (player.translate.y <= 0 + player.radius / 2) {

				player.translate.y = player.radius / 2;

			}
			else if (player.translate.y >= kWindowHeight * 2 - player.radius / 2) {

				player.translate.y = kWindowHeight * 2 - player.radius / 2;

			}
#pragma endregion
#pragma region チャージ
			/******** チャージ関係の処理 **********/
			//スペースキー長押し
			if (keys[DIK_SPACE]) {

				//チャージ状態true
				isCharging = true;

			}
			else {

				//チャージ状態false
				isCharging = false;

			}

			if (keys[DIK_RIGHT] || keys[DIK_D]) {

				player.theta += 0.1f;

			}

			else if (keys[DIK_LEFT] || keys[DIK_A]) {

				player.theta -= 0.1f;

			}

			//チャージ状態trueの時
			if (isCharging == true && isAttacking == false) {

				if (chargePower < maxPower) {

					//チャージ
					chargePower += 5.0f;

				}
				else {

					//一定の値を超えたら固定
					chargePower = maxPower;

				}
				/******** チャージエフェクト **********/
				for (int i = 0; i < chargeEffectMax; i++)
				{
					//エフェクトが出ていない
					if (!chargeEffect[i].isActive)
					{

						chargeEffect[i].translate.x = player.translate.x;
						chargeEffect[i].translate.y = player.translate.y;
						chargeEffect[i].isActive = true;
						chargeEffect[i].carentAlpha = 0xAA;
						chargeEffect[i].vectorLength = player.radius;

						if (rand() % 8 == 0)
						{
							chargeEffect[i].moveDirection.x = 0;
							chargeEffect[i].moveDirection.y = -1;
						}
						else if (rand() % 8 == 1) {
							chargeEffect[i].moveDirection.x = -1;
							chargeEffect[i].moveDirection.y = -1;
						}
						else if (rand() % 8 == 2) {
							chargeEffect[i].moveDirection.x = 1;
							chargeEffect[i].moveDirection.y = 0;
						}
						else if (rand() % 8 == 3) {
							chargeEffect[i].moveDirection.x = -1;
							chargeEffect[i].moveDirection.y = 1;
						}
						else if (rand() % 8 == 4) {
							chargeEffect[i].moveDirection.x = 1;
							chargeEffect[i].moveDirection.y = 1;
						}
						else if (rand() % 8 == 5) {
							chargeEffect[i].moveDirection.x = 1;
							chargeEffect[i].moveDirection.y = -1;
						}
						else if (rand() % 8 == 6) {
							chargeEffect[i].moveDirection.x = -1;
							chargeEffect[i].moveDirection.y = 0;
						}
						else if (rand() % 8 == 7) {
							chargeEffect[i].moveDirection.x = 0;
							chargeEffect[i].moveDirection.y = 1;
						}
						break;
					}
					//エフェクトが出ている
					else {
						chargeEffect[i].vectorLength -= 4;
						if (chargeEffect[i].vectorLength <= 0) {
							chargeEffect[i].isActive = false;
						}
					}
				}
			}
			else {

				if (chargePower > 0) {

					//スペースキーを離したらパワーが減る
					chargePower -= 4.0f;

				}
				else {

					//0以下になったら値を0にリセット
					chargePower = 0;

					isAttacking = false;

				}

				//動いた時にエフェクトを薄くしていく
				for (int i = 0; i < chargeEffectMax; i++)
				{
					if (chargeEffect[i].isActive)
					{
						chargeEffect[i].carentAlpha -= 0x11;

						if (chargeEffect[i].carentAlpha <= 0) {

							chargeEffect[i].isActive = false;

						}
					}
				}
			}

			if (theta >= 6.0f) {

				player.theta = 0.0f;

			}
			else if (theta < 0.0f) {

				player.theta = 5.9f;

			}

			if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {

				if (isAttacking == false) {

					isAttacking = true;

				}

			}

			if (isAttacking == true) {

				if (canEnhancementAttack == true) {

					player.translate.x += (cosf(player.theta) * player.speed * 1.25f * chargePower / 3.0f);
					player.translate.y += -(sinf(player.theta) * player.speed * 1.25f * chargePower / 3.0f);

				}
				else {

					player.translate.x += (cosf(player.theta) * player.speed * chargePower / 3.0f);
					player.translate.y += -(sinf(player.theta) * player.speed * chargePower / 3.0f);

				}

				chargePower--;

				//残像
				for (int i = 0; i < afterimageMax; i++)
				{
					//残像が出ていない
					if (!afterimage[i].isActive)
					{
						afterimage[i].isActive = true;
						afterimage[i].translate.x = player.translate.x;
						afterimage[i].translate.y = player.translate.y;
						afterimage[i].carentAlpha = 0xA0;
						afterimage[i].radius = 128;
						break;
					}
					//出てる
					else
					{
						afterimage[i].carentAlpha -= 0x0A;
						if (afterimage[i].carentAlpha <= 0x00) {
							afterimage[i].isActive = false;
						}
					}
				}
			}
			//動いてないとき
			else
			{
				for (int i = 0; i < afterimageMax; i++)
				{
					if (afterimage[i].isActive)
					{
						afterimage[i].carentAlpha -= 0x0A;
						if (afterimage[i].carentAlpha <= 0x00) {
							afterimage[i].isActive = false;
						}
					}
				}
			}

#pragma endregion
#pragma region コンボ

			if (comboReceptionTime < 0) {

				combo = 0;
				tenCombo = 0;
				comboMagnification = 1.0f;
				comboReceptionTime = defComboReceptionTime;

			}

			if (combo >= 1) {

				comboReceptionTime--;

			}
			else {

				comboReceptionTime = defComboReceptionTime;

			}

			if (tenCombo >= startCombo) {

				comboMagnification += 0.1f;
				tenCombo = 0;

			}

#pragma endregion
#pragma region スクロール

			/******** スクロール処理 **********/

			if (player.translate.x >= scroolStartPos.x || player.translate.x <= scroolStartPos.x) {

				scrool.x = player.translate.x - scroolStartPos.x;

			}
			else {

				scrool.x = 0.0f;

			}

			if (player.translate.y >= scroolStartPos.y || player.translate.y <= scroolStartPos.y) {

				scrool.y = player.translate.y - scroolStartPos.y;

			}
			else {

				scrool.y = 0.0f;

			}

#pragma endregion

#pragma endregion
#pragma region 味方

#pragma region デコイ設置
			if (keys[DIK_S] && !preKeys[DIK_S] || keys[DIK_DOWNARROW] && !preKeys[DIK_DOWNARROW]) {

				ally.HP -= 5;

				putDecoy = true;
				canEnhancementAttack = true;

			}
#pragma endregion

#pragma region プレイヤーが強化攻撃をできるか
			if (putDecoy == false && isAttacking == false) {

				canEnhancementAttack = false;

			}
#pragma endregion

#pragma region プレイヤー追従

			if (putDecoy == false) {

				ally.theta = atan2(player.translate.y - ally.translate.y, player.translate.x - ally.translate.x);

				if (ally.theta <= 0) {

					ally.theta *= -1;

				}
				else {

					float def = M_PI - ally.theta;

					ally.theta = def + M_PI;

				}

				ally.translate.x += (cosf(ally.theta) * ally.speed);
				ally.translate.y += -(sinf(ally.theta) * ally.speed);

			}

#pragma endregion
#pragma region プレイヤーとの衝突判定

			p2d.distance.x = player.translate.x - ally.translate.x;
			p2d.distance.y = player.translate.y - ally.translate.y;

			p2d.length = sqrt(pow(p2d.distance.x, 2) + pow(p2d.distance.y, 2));

			if (p2d.length <= player.radius / 2 + ally.radius / 2) {

				if (putDecoy == true) {

					if (canCollectDecoy == true) {

						putDecoy = false;
						repairComplete = false;

					}

				}
				else {

					canCollectDecoy = false;
					ally.translate.x += -(cosf(player.theta) * ally.speed);
					ally.translate.y += (sinf(player.theta) * ally.speed);

				}

			}
			else {

				canCollectDecoy = true;

			}

#pragma endregion

#pragma region 設置されている時HP減少

			if (putDecoy == true) {

				ally.HP--;

			}

#pragma endregion


#pragma endregion


#pragma region 敵

#pragma region スポーン地点設定

			//上
			spawnPoint[0].x = player.translate.x;
			spawnPoint[0].y = player.translate.y + kWindowHeight / 2 + player.radius;

			//下
			spawnPoint[1].x = player.translate.x;
			spawnPoint[1].y = player.translate.y - kWindowHeight / 2 - player.radius;

			//右
			spawnPoint[2].x = player.translate.x + kWindowWidth / 2 + player.radius;
			spawnPoint[2].y = player.translate.y;

			//左
			spawnPoint[3].x = player.translate.x - kWindowWidth / 2 - player.radius;
			spawnPoint[3].y = player.translate.y;

#pragma endregion
#pragma region スポーンタイマー

			if (nowSpawnCounter <= kMaxEnemy) {

				if (spawnTimer <= 0.0f) {

					canSpawn = true;

				}
				else {

					canSpawn = false;
					spawnTimer--;

				}

			}

#pragma endregion
#pragma region 敵メイン処理

			for (int i = 0; i < kMaxEnemy; i++) {

#pragma region デバック

				if (keys[DIK_E]) {

					spawnCounter = 0;
					nowSpawnCounter = 0;
					nowSpawnCounter--;
					isNockBacking[i] = false;
					enemy[i].translate.x = 0.0f;
					enemy[i].translate.y = 0.0f;
					enemy[i].isAlive = false;

				}

#pragma endregion
#pragma region 敵生存時処理
				if (enemy[i].isAlive == true) {

#pragma region 敵タイプ
					if (isNockBacking[i] == false) {

						switch (enemyType[i])
						{
						case FOLLOW:

							if (putDecoy == true) {

								enemy[i].theta = atan2(ally.translate.y - enemy[i].translate.y, ally.translate.x - enemy[i].translate.x);

							}
							else {

								enemy[i].theta = atan2(player.translate.y - enemy[i].translate.y, player.translate.x - enemy[i].translate.x);

							}

							if (enemy[i].theta <= 0) {

								enemy[i].theta *= -1;

							}
							else {

								float def = M_PI - enemy[i].theta;

								enemy[i].theta = def + M_PI;

							}

							enemy[i].translate.x += (cosf(enemy[i].theta) * enemy[i].speed);
							enemy[i].translate.y += -(sinf(enemy[i].theta) * enemy[i].speed);

							break;

						}

					}
					else {

						enemy[i].translate.x += (cosf(enemyNockBack[i]) * nockBackSpeed[i]);
						enemy[i].translate.y += -(sinf(enemyNockBack[i]) * nockBackSpeed[i]);
						nockBackSpeed[i]--;

					}

					if (nockBackSpeed[i] <= 0) {

						isNockBacking[i] = false;

					}

#pragma endregion

#pragma region プレイヤーとの衝突判定
					if (isAttacking == false) {

						if (isNockBacking[i] == false) {

							e2p.distance.x = enemy[i].translate.x - player.translate.x;
							e2p.distance.y = enemy[i].translate.y - player.translate.y;

							e2p.length = sqrt(pow(e2p.distance.x, 2) + pow(e2p.distance.y, 2));

							if (e2p.length <= enemy[i].radius / 2 + player.radius / 2) {

								enemyNockBack[i] = rand();
								enemyNockBack[i] = enemyNockBack[i] % 360;

								enemyNockBack[i] = enemyNockBack[i] * (M_PI / 180.0f);

								isNockBacking[i] = true;
								nockBackSpeed[i] = 20.0f;

							}

						}

					}
					else {

						e2p.distance.x = enemy[i].translate.x - player.translate.x;
						e2p.distance.y = enemy[i].translate.y - player.translate.y;

						e2p.length = sqrt(pow(e2p.distance.x, 2) + pow(e2p.distance.y, 2));

						if (e2p.length <= enemy[i].radius / 2 + player.radius / 2) {

							enemy[i].HP -= player.damage;

						}

					}
#pragma endregion
#pragma region デコイとの衝突判定

					e2d.distance.x = enemy[i].translate.x - ally.translate.x;
					e2d.distance.y = enemy[i].translate.y - ally.translate.y;

					e2d.length = sqrt(pow(e2d.distance.x, 2) + pow(e2d.distance.y, 2));

					if (putDecoy == true) {

						if (e2d.length <= enemy[i].radius / 2 + ally.radius / 2) {

							enemyNockBack[i] = rand();
							enemyNockBack[i] = enemyNockBack[i] % 360;

							enemyNockBack[i] = enemyNockBack[i] * (M_PI / 180.0f);

							isNockBacking[i] = true;
							nockBackSpeed[i] = 25.0f;

						}

					}

#pragma endregion
#pragma region エフェクトとの衝突判定
					for (int j = 0; j < afterimageMax; j++) {

						if (afterimage[j].carentAlpha <= 0xA0 && afterimage[j].isActive == true && isAttacking == true) {

							ef2e[j].distance.x = afterimage[j].translate.x - enemy[i].translate.x;
							ef2e[j].distance.y = afterimage[j].translate.y - enemy[i].translate.y;

							ef2e[j].length = sqrt(pow(ef2e[j].distance.x, 2) + pow(ef2e[j].distance.y, 2));

							if (ef2e[j].length <= afterimage[j].radius / 2 + enemy[i].radius / 2) {
								enemy[i].HP -= player.damage;


							}

						}

					}
#pragma endregion

#pragma region 死亡判定

					if (enemy[i].HP <= 0) {

						deadEnemy[i] = true;
						enemy[i].HP = 1;

					}

					if (deadEnemy[i] == true) {

						if (energy[i].isAlive == false) {

							energy[i].translate = enemy[i].translate;
							energy[i].isAlive = true;
						}
/*****************************************************************************************************************************
										ここから
*****************************************************************************************************************************/
						if (playDeathEffect[i] == false) {

							deathEffect[i][0].translate = enemy[i].translate;
							deathEffect[i][1].translate = enemy[i].translate;
							deathEffect[i][2].translate = enemy[i].translate;
							deathEffect[i][3].translate = enemy[i].translate;
							deathEffect[i][4].translate = enemy[i].translate;
							deathEffect[i][5].translate = enemy[i].translate;
							deathEffect[i][6].translate = enemy[i].translate;
							deathEffect[i][7].translate = enemy[i].translate;
							playDeathEffect[i] = true;
						}
/*****************************************************************************************************************************
										ここまで
*****************************************************************************************************************************/

						enemy[i].translate.x = 0.0f;
						enemy[i].translate.y = 0.0f;

						combo++;
						tenCombo++;
						comboReceptionTime = defComboReceptionTime;

						score += addScore * comboMagnification * scoreMagnification;

						nowSpawnCounter--;

						isNockBacking[i] = false;
						enemy[i].isAlive = false;
						deadEnemy[i] = false;

					}
#pragma endregion

				}
#pragma endregion
#pragma region スポーン処理
				else {
					if (canSpawn == true) {

						if (spawnCounter <= maxOnceSpawn) {

							enemySpawnPoint = rand();
							enemySpawnPoint = enemySpawnPoint % 4;

							enemy[i].translate = spawnPoint[enemySpawnPoint];

							enemy[i].speed = rand();
							enemy[i].speed = (int)enemy[i].speed % 3 + 1;

							enemy[i].isAlive = true;

							nowSpawnCounter++;
							spawnCounter++;

						}
						else {

							canSpawn = false;
							spawnTimer = defSpawnTimer;
							spawnCounter = 0;

						}
					}
				}
#pragma endregion

			}
#pragma endregion



#pragma endregion

#pragma region 機械

#pragma region エネルギー回収処理

#pragma region プレイヤーが近づいて回復する処理
			//m2p.distance.x = Machine.translate.x - player.translate.x;
			//m2p.distance.y = Machine.translate.y - player.translate.y;

			//m2p.length = sqrt(pow(m2p.distance.x, 2) + pow(m2p.distance.y, 2));

			//if (m2p.length <= collectEnergyRange / 2 + player.radius / 2) {

			//	if (Machine.HP < 1000) {

			//		/*if (energyAmount >= 10) {

			//			Machine.HP += 10;
			//			energyAmount -= 10;

			//		}
			//		else if (energyAmount < 10 && energyAmount > 0) {

			//			Machine.HP += 10 - energyAmount;
			//			energyAmount = 0;

			//		}*/

			//		if (energyAmount > 0) {

			//			Machine.HP += 5;
			//			energyAmount--;

			//		}

			//	}
			//	else {

			//		Machine.HP = 1000;

			//	}

			//}
#pragma endregion

#pragma region デコイがエネルギー回収範囲内に設置されているときに回復する処理
			m2d.distance.x = Machine.translate.x - ally.translate.x;
			m2d.distance.y = Machine.translate.y - ally.translate.y;

			m2d.length = sqrt(pow(m2d.distance.x, 2) + pow(m2d.distance.y, 2));

			if (putDecoy == true) {

				if (m2d.length <= collectEnergyRange / 2 + ally.radius / 2) {

					if (repairComplete == true) {

						canCollectDecoy = true;

					}
					else {

						canCollectDecoy = false;

					}

					if (Machine.HP < 1000) {

						if (energyAmount >= 0) {

							Machine.HP += 5;
							energyAmount--;

						}

					}
					else {

						Machine.HP = 1000;
						repairComplete = true;

					}

				}

			}

#pragma endregion


#pragma endregion
#pragma region 時間がたつごとにエネルギー減少

			if (Machine.HP <= 0) {

				Machine.HP = 0;

			}
			else {

				Machine.HP--;

			}

#pragma endregion


#pragma endregion


#pragma region エネルギー

			for (int i = 0; i < kMaxEnergy; i++) {

				if (energy[i].isAlive == true) {

#pragma region プレイヤーとの衝突判定

					en2p[i].distance.x = energy[i].translate.x - player.translate.x;
					en2p[i].distance.y = energy[i].translate.y - player.translate.y;

					en2p[i].length = sqrt(pow(en2p[i].distance.x, 2) + pow(en2p[i].distance.y, 2));

					if (en2p[i].length <= collectRange[i] / 2 + player.radius / 2) {

						colletCount[i]++;
						if (colletCount[i] >= 10)
						{
							followPlayer[i] = true;

						}

					}
					if (colletCount[i] >= 10)
					{
						if (en2p[i].length <= energy[i].radius / 2 + player.radius / 2) {

							energyAmount++;
							followPlayer[i] = false;
							energy[i].speed = energy[i].defSpeed;
							energy[i].isAlive = false;
							colletCount[i] = 0;
						}
					}


#pragma endregion
#pragma region エフェクトとの衝突判定

					/*for (int j = 0; j < afterimageMax; j++) {
						if (afterimage[j].isActive == true && isAttacking == true) {

							ef2en[j].distance.x = energy[i].translate.x - afterimage[j].translate.x;
							ef2en[j].distance.y = energy[i].translate.y - afterimage[j].translate.y;

							ef2en[j].length = sqrt(pow(ef2en[j].distance.x, 2) + pow(ef2en[j].distance.y, 2));

							if (ef2en[j].length <= collectRange[i] / 2 + afterimage[j].radius / 2) {

								followPlayer[i] = true;

							}

						}

					}*/

#pragma endregion

#pragma region プレイヤーへの追従判定

					if (followPlayer[i] == true) {

						energy[i].theta = atan2(player.translate.y - energy[i].translate.y, player.translate.x - energy[i].translate.x);

						if (energy[i].theta <= 0) {

							energy[i].theta *= -1;

						}
						else {

							float def = M_PI - energy[i].theta;

							energy[i].theta = def + M_PI;

						}

						energy[i].translate.x += (cosf(energy[i].theta) * energy[i].speed);
						energy[i].translate.y += -(sinf(energy[i].theta) * energy[i].speed);

						energy[i].speed += 0.5f;

					}

#pragma endregion

				}

#pragma region 画面外に生成されたとき自動で追従するようにする

				//左右
				if (energy[i].translate.x <= 0 + energy[i].radius / 2) {

					followPlayer[i] = true;

				}
				else if (energy[i].translate.x >= kWindowWidth * 3 - energy[i].radius / 2) {

					followPlayer[i] = true;

				}

				//上下
				if (energy[i].translate.y <= 0 + energy[i].radius / 2) {

					followPlayer[i] = true;

				}
				else if (player.translate.y >= kWindowHeight * 2 - player.radius / 2) {

					followPlayer[i] = true;

				}
#pragma endregion

			}

#pragma endregion
/*****************************************************************************************************************************
                                        ここから
*****************************************************************************************************************************/
#pragma region 死亡エフェクト
			/*********** 死亡エフェクト ***********/
			for (int i = 0; i < playMaxDeathEffect; i++) {
				if (playDeathEffect[i])
				{
					for (int j = 0; j < deathMaxEffect; j++)
					{
						if (!deathEffect[i][j].isActive)
						{
							deathEffect[i][j].isActive = true;
							deathEffect[i][j].carentAlpha = 0xFF;
							deathEffect[i][j].vectorLength = 0;
							if (j == 0)
							{
								deathEffect[i][j].moveDirection.x = 0;
								deathEffect[i][j].moveDirection.y = -1;
							}
							else if (j == 1) {
								deathEffect[i][j].moveDirection.x = -1;
								deathEffect[i][j].moveDirection.y = -1;
							}
							else if (j == 2) {
								deathEffect[i][j].moveDirection.x = 1;
								deathEffect[i][j].moveDirection.y = 0;
							}
							else if (j == 3) {
								deathEffect[i][j].moveDirection.x = -1;
								deathEffect[i][j].moveDirection.y = 1;
							}
							else if (j == 4) {
								deathEffect[i][j].moveDirection.x = 1;
								deathEffect[i][j].moveDirection.y = 1;
							}
							else if (j == 5) {
								deathEffect[i][j].moveDirection.x = 1;
								deathEffect[i][j].moveDirection.y = -1;
							}
							else if (j == 6) {
								deathEffect[i][j].moveDirection.x = -1;
								deathEffect[i][j].moveDirection.y = 0;
							}
							else{
								deathEffect[i][j].moveDirection.x = 0;
								deathEffect[i][j].moveDirection.y = 1;
							}
						}
						else {
							deathEffect[i][j].vectorLength += 5;
							deathEffect[i][j].carentAlpha -= 0x08;
							if (deathEffect[i][j].carentAlpha <= 0x0C) 
							{
								playDeathEffect[i] = false;
							}
						}
					}
				}else
				{
					deathEffect[i][0].isActive = false;
					deathEffect[i][1].isActive = false;
					deathEffect[i][2].isActive = false;
					deathEffect[i][3].isActive = false;
					deathEffect[i][4].isActive = false;
					deathEffect[i][5].isActive = false;
					deathEffect[i][6].isActive = false;
					deathEffect[i][7].isActive = false;
				}
			}
#pragma endregion
	/*****************************************************************************************************************************
											ここまで
	*****************************************************************************************************************************/
#pragma endregion	

			break;
		case RESULT:



			break;
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		switch (scene)
		{
		case TITLE:



			break;
		case GAME:

#pragma region 背景描画

			Novice::DrawQuad(

				worldPosOrigin.x + player.translate.x - kWindowWidth / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y - kWindowHeight / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x + kWindowWidth / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y - kWindowHeight / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x - kWindowWidth / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y + kWindowHeight / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x + kWindowWidth / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y + kWindowHeight / 2 + scrool.y,

				0,
				0,

				1,
				1,

				white1x1Png,
				BLACK

			);

			/******** 背景描画 **********/

			for (int i = 0; i < 3; i++) {


				Novice::DrawQuad(

					worldPosOrigin.x + bg[i].translate.x * (i + 1) + (kWindowWidth / 2 * i) - bg[i].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i].translate.y - bg[i].radius.y / 2 - (kWindowHeight)+scrool.y,

					worldPosOrigin.x + bg[i].translate.x * (i + 1) + (kWindowWidth / 2 * i) + bg[i].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i].translate.y - bg[i].radius.y / 2 - (kWindowHeight)+scrool.y,

					worldPosOrigin.x + bg[i].translate.x * (i + 1) + (kWindowWidth / 2 * i) - bg[i].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i].translate.y + bg[i].radius.y / 2 - (kWindowHeight)+scrool.y,

					worldPosOrigin.x + bg[i].translate.x * (i + 1) + (kWindowWidth / 2 * i) + bg[i].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i].translate.y + bg[i].radius.y / 2 - (kWindowHeight)+scrool.y,

					bg[i].drawStartArea.x,
					bg[i].drawStartArea.y,

					bg[i].drawEndArea.x,
					bg[i].drawEndArea.y,

					bg[i].name,
					WHITE

				);


			}

			for (int i = 0; i < 3; i++) {


				Novice::DrawQuad(

					worldPosOrigin.x + bg[i + 3].translate.x * (i + 1) + (kWindowWidth / 2 * i) - bg[i + 3].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i + 3].translate.y - bg[i + 3].radius.y / 2 + scrool.y,

					worldPosOrigin.x + bg[i + 3].translate.x * (i + 1) + (kWindowWidth / 2 * i) + bg[i + 3].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i + 3].translate.y - bg[i].radius.y / 2 + scrool.y,

					worldPosOrigin.x + bg[i + 3].translate.x * (i + 1) + (kWindowWidth / 2 * i) - bg[i + 3].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i + 3].translate.y + bg[i + 3].radius.y / 2 + scrool.y,

					worldPosOrigin.x + bg[i + 3].translate.x * (i + 1) + (kWindowWidth / 2 * i) + bg[i + 3].radius.x / 2 - scrool.x,
					worldPosOrigin.y - bg[i + 3].translate.y + bg[i + 3].radius.y / 2 + scrool.y,

					bg[i + 3].drawStartArea.x,
					bg[i + 3].drawStartArea.y,

					bg[i + 3].drawEndArea.x,
					bg[i + 3].drawEndArea.y,

					bg[i + 3].name,
					WHITE

				);


			}

#pragma endregion

#pragma region 機械描画

			Novice::DrawQuad(

				worldPosOrigin.x + Machine.translate.x - Machine.radius / 2 - scrool.x,
				worldPosOrigin.y - Machine.translate.y - Machine.radius / 2 + scrool.y,

				worldPosOrigin.x + Machine.translate.x + Machine.radius / 2 - scrool.x,
				worldPosOrigin.y - Machine.translate.y - Machine.radius / 2 + scrool.y,

				worldPosOrigin.x + Machine.translate.x - Machine.radius / 2 - scrool.x,
				worldPosOrigin.y - Machine.translate.y + Machine.radius / 2 + scrool.y,

				worldPosOrigin.x + Machine.translate.x + Machine.radius / 2 - scrool.x,
				worldPosOrigin.y - Machine.translate.y + Machine.radius / 2 + scrool.y,

				0,
				0,

				Machine.graphRadius,
				Machine.graphRadius,

				Machine.graph,
				Machine.color

			);

			Novice::DrawEllipse(

				worldPosOrigin.x + Machine.translate.x - scrool.x,
				worldPosOrigin.y - Machine.translate.y + scrool.y,
				collectEnergyRange,
				collectEnergyRange,
				0.0f,
				circleColor,
				kFillModeWireFrame

			);

#pragma endregion

#pragma region 敵描画

			for (int i = 0; i < kMaxEnemy; i++) {

				if (enemy[i].isAlive == true) {

					Novice::DrawQuad(

						worldPosOrigin.x + enemy[i].translate.x - enemy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - enemy[i].translate.y - enemy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + enemy[i].translate.x + enemy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - enemy[i].translate.y - enemy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + enemy[i].translate.x - enemy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - enemy[i].translate.y + enemy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + enemy[i].translate.x + enemy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - enemy[i].translate.y + enemy[i].radius / 2 + scrool.y,

						0,
						0,

						enemy[i].graphRadius,
						enemy[i].graphRadius,

						enemy[i].graph,
						enemy[i].color

					);

				}

			}
/*****************************************************************************************************************************
										ここから
*****************************************************************************************************************************/
			/******** 死亡 **********/
			for (int i = 0; i < playMaxDeathEffect; i++)
			{
				for (int j = 0; j < deathMaxEffect; j++)
				{
					if (deathEffect[i][j].isActive)
					{
						Novice::DrawQuad(

							worldPosOrigin.x + deathEffect[i][j].translate.x - deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.x - scrool.x,
							worldPosOrigin.y - deathEffect[i][j].translate.y - deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.y + scrool.y,

							worldPosOrigin.x + deathEffect[i][j].translate.x + deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.x - scrool.x,
							worldPosOrigin.y - deathEffect[i][j].translate.y - deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.y + scrool.y,

							worldPosOrigin.x + deathEffect[i][j].translate.x - deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.x - scrool.x,
							worldPosOrigin.y - deathEffect[i][j].translate.y + deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.y + scrool.y,

							worldPosOrigin.x + deathEffect[i][j].translate.x + deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.x - scrool.x,
							worldPosOrigin.y - deathEffect[i][j].translate.y + deathEffect[i][j].radius + deathEffect[i][j].vectorLength * deathEffect[i][j].moveDirection.y + scrool.y,

							0,
							0,

							deathEffect[i][j].graphRadius,
							deathEffect[i][j].graphRadius,

							deathEffect[i][j].graph,
							0xFFFFFF00 + deathEffect[i][j].carentAlpha

						);
					}
				}
			}

			/*****************************************************************************************************************************
													ここまで
			*****************************************************************************************************************************/
#pragma endregion
#pragma region エフェクト
			/******** 残像 **********/
			for (int i = 0; i < afterimageMax; i++)
			{
				if (afterimage[i].isActive)
				{
					Novice::DrawQuad(

						worldPosOrigin.x + afterimage[i].translate.x - afterimage[i].radius-- / 2 - scrool.x,
						worldPosOrigin.y - afterimage[i].translate.y - afterimage[i].radius / 2 + scrool.y,

						worldPosOrigin.x + afterimage[i].translate.x + afterimage[i].radius / 2 - scrool.x,
						worldPosOrigin.y - afterimage[i].translate.y - afterimage[i].radius / 2 + scrool.y,

						worldPosOrigin.x + afterimage[i].translate.x - afterimage[i].radius / 2 - scrool.x,
						worldPosOrigin.y - afterimage[i].translate.y + afterimage[i].radius / 2 + scrool.y,

						worldPosOrigin.x + afterimage[i].translate.x + afterimage[i].radius / 2 - scrool.x,
						worldPosOrigin.y - afterimage[i].translate.y + afterimage[i].radius / 2 + scrool.y,

						0,
						0,

						afterimage[i].graphRadius,
						afterimage[i].graphRadius,

						afterimage[i].graph,
						0xFFFFFF00 + afterimage[i].carentAlpha

					);
				}
			}

			/******** チャージエフェクト **********/
			for (int i = 0; i < chargeEffectMax; i++)
			{
				if (chargeEffect[i].isActive)
				{
					Novice::DrawQuad(

						worldPosOrigin.x + chargeEffect[i].translate.x - chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.x - scrool.x,
						worldPosOrigin.y - chargeEffect[i].translate.y - chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.y + scrool.y,

						worldPosOrigin.x + chargeEffect[i].translate.x + chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.x - scrool.x,
						worldPosOrigin.y - chargeEffect[i].translate.y - chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.y + scrool.y,

						worldPosOrigin.x + chargeEffect[i].translate.x - chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.x - scrool.x,
						worldPosOrigin.y - chargeEffect[i].translate.y + chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.y + scrool.y,

						worldPosOrigin.x + chargeEffect[i].translate.x + chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.x - scrool.x,
						worldPosOrigin.y - chargeEffect[i].translate.y + chargeEffect[i].radius * 0.4f + chargeEffect[i].vectorLength * chargeEffect[i].moveDirection.y + scrool.y,

						0,
						0,

						chargeEffect[i].graphRadius,
						chargeEffect[i].graphRadius,

						chargeEffect[i].graph,
						0xFFFFFF00 + chargeEffect[i].carentAlpha

					);
				}
			}
#pragma endregion

#pragma region 味方描画
			/******** 味方描画 **********/
			Novice::DrawQuad(

				worldPosOrigin.x + ally.translate.x - ally.radius / 2 - scrool.x,
				worldPosOrigin.y - ally.translate.y - ally.radius / 2 + scrool.y,

				worldPosOrigin.x + ally.translate.x + ally.radius / 2 - scrool.x,
				worldPosOrigin.y - ally.translate.y - ally.radius / 2 + scrool.y,

				worldPosOrigin.x + ally.translate.x - ally.radius / 2 - scrool.x,
				worldPosOrigin.y - ally.translate.y + ally.radius / 2 + scrool.y,

				worldPosOrigin.x + ally.translate.x + ally.radius / 2 - scrool.x,
				worldPosOrigin.y - ally.translate.y + ally.radius / 2 + scrool.y,

				0,
				0,

				ally.graphRadius,
				ally.graphRadius,

				ally.graph,
				ally.color

			);
#pragma endregion
#pragma region プレイヤー描画

			/******** プレイヤー描画 **********/
			Novice::DrawQuad(

				worldPosOrigin.x + player.translate.x - player.radius / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y - player.radius / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x + player.radius / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y - player.radius / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x - player.radius / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y + player.radius / 2 + scrool.y,

				worldPosOrigin.x + player.translate.x + player.radius / 2 - scrool.x,
				worldPosOrigin.y - player.translate.y + player.radius / 2 + scrool.y,

				0,
				0,

				player.graphRadius,
				player.graphRadius,

				player.graph,
				player.color

			);

#pragma endregion

#pragma region エネルギー描画

			for (int i = 0; i < kMaxEnergy; i++) {

				if (energy[i].isAlive == true) {

					Novice::DrawQuad(

						worldPosOrigin.x + energy[i].translate.x - energy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - energy[i].translate.y - energy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + energy[i].translate.x + energy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - energy[i].translate.y - energy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + energy[i].translate.x - energy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - energy[i].translate.y + energy[i].radius / 2 + scrool.y,

						worldPosOrigin.x + energy[i].translate.x + energy[i].radius / 2 - scrool.x,
						worldPosOrigin.y - energy[i].translate.y + energy[i].radius / 2 + scrool.y,

						0,
						0,

						energy[i].graphRadius,
						energy[i].graphRadius,

						energy[i].graph,
						energy[i].color

					);

				}

				/*Novice::DrawEllipse(

					energy[i].translate.x,
					energy[i].translate.y,
					collectRange[i],
					collectRange[i],
					0.0f,
					RED,
					kFillModeWireFrame

				);*/

			}

#pragma endregion

			break;
		case RESULT:



			break;
		}

#pragma region Debug描画

		/******** プレイヤーデバック描画 **********/

		//座標
		Novice::ScreenPrintf(0, 10, "Px : %4.2f Py : %4.2f", player.translate.x, player.translate.y);
		Novice::ScreenPrintf(0, 30, "Enemy : %d", nowSpawnCounter);
		Novice::ScreenPrintf(0, 50, "SCORE : %d", score);
		Novice::ScreenPrintf(0, 70, "Combo : %d", combo);
		Novice::ScreenPrintf(0, 90, "ScoreMag : %4.2f", comboMagnification);
		Novice::ScreenPrintf(0, 110, "energy : %d", energyAmount);
		Novice::ScreenPrintf(0, 130, "MHP : %d", Machine.HP);
		Novice::ScreenPrintf(0, 150, "AHP : %d", ally.HP);
		Novice::ScreenPrintf(0, 170, "efffect : %d", playDeathEffect[99]);


		//発射方向
		Novice::DrawLine(
			worldPosOrigin.x + player.translate.x - scrool.x,
			worldPosOrigin.y - player.translate.y + scrool.y,
			worldPosOrigin.x + player.translate.x + (cosf(player.theta) * player.speed * 2 * (chargePower + 10) / 3) - scrool.x,
			worldPosOrigin.y - player.translate.y + (sinf(player.theta) * player.speed * 2 * (chargePower + 10) / 3) + scrool.y,
			WHITE
		);

#pragma endregion

		/*********************************
			描画処理ここまで
		*********************************/

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {

			break;

		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
