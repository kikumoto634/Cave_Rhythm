#include "BottonHigh.h"

void BottonHigh::RhythmChange()
{
	audio_->StopWave(bpm120Game_audio.number);
	audio_->StopWave(bpm90Game_audio.number);
	rhythm_->BPMHighSet();
	audio_->PlayWave(bpm150Game_audio.number, bpm150Game_audio.volume, true);
}
