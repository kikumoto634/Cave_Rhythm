#pragma once
#include "BaseScene.h"

#include "TouchableObject.h"

#include "Player.h"
#include "Exit.h"

#include "JudgeLocation.h"
#include "RNotes.h"
#include "LNotes.h"

#include "CollisionPrimitive.h"
#include "CollisionManager.h"

#include "RhythmManager.h"
#include "GameManager.h"
#include "AreaManager.h"

class BaseBattleScene : public BaseScene
{
protected:
	//シーン遷移
	const float FadeSecond = 1.f;

	//ノーツ数
	const int notesNum= 7;

public:
	
	//コンストラクタ
	BaseBattleScene(DirectXCommon* dxCommon, Window* window, int saveHP = 5, int floorVaule = 1);

	~BaseBattleScene();

	/// <summary>
	/// 起動時
	/// </summary>
	void Application() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize() override;

private:
	//初期化
	void CommonInitialize();
	void Object3DInitialize();
	void Object2DInitialize();

	//更新
	void Object3DUpdate();
	void Object2DUpdate();
	void CommonUpdate();

	//リズム関連
	void RhythmJudgeUpdate();
	void BeatEndUpdate();

	//描画
	void Object3DDraw();
	void ParticleDraw();
	void UIDraw();

	//後処理
	void ObjectFinaize();
	void CommonFinalize();

protected:

	//シーン遷移
	virtual void SceneGameEnd();
	virtual void NextSceneChange() = 0;
	virtual void SceneChange();

	//初期化
	virtual void AddCommonInitialize() = 0;
	virtual void AddObject3DInitialize() = 0;
	virtual void AddObject2DInitialize() = 0;

	//更新
	virtual void AddCommonUpdate() = 0;
	virtual void AddObject3DUpdate() = 0;
	virtual void AddObject2DUpdate() = 0;
	virtual void AddBeatEndUpdate() = 0;

	//描画
	virtual void AddObject3DDraw() = 0;
	virtual void AddParticleDraw() = 0;
	virtual void AddFrontUIDraw() = 0;
	virtual void AddBackUIDraw() = 0;

	//後処理
	virtual void AddObjectFinalize() = 0;
	virtual void AddCommonFinalize() = 0;


protected:
	//共通
	//衝突マネージャー
	CollisionManager* collisionManager = nullptr;

	//持ち越し
	//セーブ体力
	int saveHP = 0;
	//階層
	int floorValue = 1;

	//player
	std::unique_ptr<Player> player;
	//リズムカウント
	std::unique_ptr<RhythmManager> rhythmManager;
	//ゲームマネージャー
	std::unique_ptr<GameManager> gameManager;
	//リズム入力
	bool IsRhythmInput = false;
	//出口
	std::unique_ptr<Exit> exit;
	//エリアマネージャー
	std::unique_ptr<AreaManager> areaManager;

#ifdef _DEBUG

#endif // _DEBUG

	//シーン遷移
	std::unique_ptr<BaseSprites> fade;
	DirectX::XMFLOAT4 fadeColor = {0,0,0,1};
	Vector2 fadeInSize = {};
	float fadeCurrentFrame = 0;
	//Prev
	bool IsPrevSceneChange = true;
	//Next
	bool IsNextSceneChange = false;

	//GameOver
	bool IsGameEnd = false;

	//BGM再生フラグ
	bool IsBGMStart = true;

	//リズム目視確認用SP
	std::unique_ptr<JudgeLocation> judgeLoca;

	/*std::vector<std::unique_ptr<Notes>> lNotes;
	std::vector<std::unique_ptr<Notes>> rNotes;*/

	bool IsNoteInput = false;
};