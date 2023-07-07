#include "BottonHigh.h"

void BottonHigh::RhythmChange()
{
	rhythm_->BPMHighSet();
	audio_->SetRation(bpm120Game_audio.number, 1.25f);
}
