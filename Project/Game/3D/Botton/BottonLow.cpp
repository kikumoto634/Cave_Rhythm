#include "BottonLow.h"


void BottonLow::RhythmChange()
{
	audio_->StopWave(bpm120Game_audio.number);
	audio_->StopWave(bpm150Game_audio.number);
	rhythm_->BPMLowSet();
	audio_->PlayWave(bpm90Game_audio.number, bpm90Game_audio.volume, true);
}
