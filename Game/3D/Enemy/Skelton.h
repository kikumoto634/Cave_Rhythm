#pragma once
#include "../BaseObjObject.h"
#include "../ParticleObject.h"
#include "../../Collision/SphereCollider.h"

#include <vector>
#include <queue>

//struct Node{
//	int x, y;
//	int g, h;
//	Node* parent;
//
//	Node(int x,int y, int g,int h, Node* parent) : 
//		x(x),y(y), g(g),h(h), parent(parent)
//	{}
//};
//
//bool operator<(const Node& a, const Node& b) {
//	return a.g + a.h > b.g + b.h;
//}
//
//int Heuristic(int x1,int y1, int x2,int y2){
//	return abs(x1-x2) + abs(y1-y2);
//}

//vector<Node*> find_path(vector<vector<int>>& grid, int start_x, int start_y, int end_x, int end_y) {
//    //パス
//    vector<Node*> path;
//    //未探索ノード
//    priority_queue<Node*> frontier;
//    //調べたノードをマーク
//    vector<vector<bool>> visited(grid.size(), vector<bool>(grid[0].size(), false));
//
//    //スタートノード設定
//    Node* start_node = new Node(start_x, start_y, 0, Heuristic(start_x, start_y, end_x, end_y), nullptr);
//    //未探索エリアの追加
//    frontier.push(start_node);
//
//    //未探索エリアが空で解除
//    while (!frontier.empty()) {
//        //現在地のノードを設定(priority_queueを使用しているので降順で大きい値が取得される)
//        Node* current_node = frontier.top();
//        //現在地で探索されたので、削除
//        frontier.pop();
//
//        //現在地がゴールなら終了
//        if (current_node->x == end_x && current_node->y == end_y) {
//            while (current_node != nullptr) {
//                path.push_back(current_node);
//                current_node = current_node->parent;
//            }
//            //取得した順番を逆にする
//            reverse(path.begin(), path.end());
//            break;
//        }
//
//        //現在地に調べたマークをつける
//        visited[current_node->y][current_node->x] = true;
//
//        //上下左右のルート先
//        int dx[4] = {-1, 1, 0, 0};
//        int dy[4] = {0, 0, -1, 1};
//
//        //四方向をforで調べる
//        for (int i = 0; i < 4; i++) {
//            //dx,dy方向のルート移動
//            int next_x = current_node->x + dx[i];
//            int next_y = current_node->y + dy[i];
//
//            //マップ範囲外
//            if (next_x < 0 || next_y < 0 || next_x >= grid.size() || next_y >= grid[0].size()) {
//                continue;
//            }
//
//            //常に調べ済み
//            if (visited[next_y][next_x]) {
//                continue;
//            }
//
//            //障害物
//            if (grid[next_y][next_x] == 1) {
//                continue;
//            }
//
//            //開始ノードからのコスト
//            int new_g = current_node->g + 1;
//            //ヒューリスティック推定値
//            int new_h = Heuristic(next_x, next_y, end_x, end_y);
//
//            //次のノードの取得、未探索エリアに追加
//            Node* next_node = new Node(next_x, next_y, new_g, new_h, current_node);
//            frontier.push(next_node);
//        }
//    }
//
//    return path;
//}


class Skelton : public BaseObjObject
{
//定数
	//ビートx回数終了時にPOP
	const int POP_COUNT = 6;
	//死亡後のリセット時間
	const int AppearanceResetFrame = 50;
	//移動待機
	const int WaitCount = 2;

	//描画範囲(非表示)
	const int DrawingRange_Not = 11;
	//描画範囲(暗めの表示)
	const int DrawingRange_Half = 7;

public:
	~Skelton();
	void Initialize(std::string filePath, bool IsSmoothing = false) override;
	void Update(Camera* camera, Vector3 playerPos);
	void Draw() override;
	void ParticleDraw();
	void Finalize() override;
	void OnCollision(const CollisionInfo& info) override;

	void Pop(Vector3 pos);

	inline void CaveLightOn()	{IsCaveLight = true;}
	inline void CaveLightOff()	{IsCaveLight = false;}

	//Getter
	inline bool GetIsNotApp()	{return IsNotApp;}
	inline bool GetIsDeadAudio()	{return IsDeadOnceAudio;}
	inline Vector3 GetDeadParticlepos()	{return DeadParticlePos;}
	inline bool GetIsInvisible() {return IsInvisible;}

private:
	void Reset();

	void Movement();

	//出現予定エフェクト
	void PopParticleApp();
	//死亡エフェクト
	void DeadParticleApp();

private:
	//待機時間
	int waitCount = 0;

	//コライダー
	SphereCollider* sphereCollider = nullptr;
	bool IsCollision = true;

	//未出現
	bool IsNotApp = false;

	Vector3 NotAlivePos = {50,50,50};
	//死亡
	bool IsDead = false;
	bool IsDeadOnceAudio = false;
	bool IsDeadOnceParticle = false;
	Vector3 DeadParticlePos = {};

	//復活
	int appearancePopFrame = 0;

	//ポップ
	bool IsPop = false;
	int popCount = 0;
	Vector3 PopParticlePos = {};

	//距離に応じた非表示
	bool IsInvisible = false;
	float distance = 0.f;

	//スケール
	bool IsScaleEasing = false;

	//パーティクル
	ParticleObject* PopParticle = nullptr;
	ParticleObject* DeadParticle = nullptr;
	//ライティング
	bool IsCaveLight = false;
};