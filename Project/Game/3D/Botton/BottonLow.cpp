#include "BottonLow.h"


void BottonLow::RhythmChange()
{
	rhythm_->BPMLowSet();
	audio_->SetRation(bpm120Game_audio.number, 0.75f);
}
