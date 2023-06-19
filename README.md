# Cave_Rhythm
3Dリズムアクションゲーム

「[Crypt Of The necroDancer](https://store.steampowered.com/app/247080/Crypt_of_the_NecroDancer/?l=japanese)」を参考に作成

流れる音楽、リズムに合わせて移動、攻撃！  
最下層のボス討伐を目指す！！

- 入力対応
  - キーボード　(マウス不要)
  - ゲームパッド　(Xbox 360)
- その他
  - 制作期間
    - 2022/12/26
  - 使用アルゴリズム
    - A*アルゴリズム
    - 区域分割法
    - ラジアルブラー
    - BPM計測 (#include "chrono" を使用)
    - Rayを使用した衝突判定

# 環境
| 種類 | 条件 |
|:-----:|:-----:|
| API | DirectX12 |
| OS | Window11 |

# 使用言語
| 言語名 | 用途 |
|:-----:|:-----:|
| C++ | エンジン、アプリ|
| HLSL | シェーダー|

