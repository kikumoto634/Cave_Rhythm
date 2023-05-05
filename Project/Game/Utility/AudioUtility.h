#pragma once

struct AudioState{
	int number;
	float volume;
};

//ワールド定義
inline AudioState rhythm_audio		= {0, 0.25f};
inline AudioState miss_audio		= {1, 0.7f};
inline AudioState damage_audio		= {2, 0.3f};
inline AudioState exBPM90_audio		= {3, 0.25f};
inline AudioState exBPM120_audio	= {4, 0.25f};
inline AudioState exBPM180_audio	= {5, 0.25f};
inline AudioState openExit_audio	= {6, 0.5f};
inline AudioState coinGet_audio		= {7, 0.5f};
inline AudioState bpm120Game_audio	= {8, 0.5f};
inline AudioState bpm120Home_audio	= {9, 0.25f};
inline AudioState dig_audio			= {10, 1.0f};
inline AudioState gateEnter_audio	= {11, 0.25f};
inline AudioState cutIn_audio		= {12, 0.25f};
inline AudioState reflected_audio	= {13, 0.5f};