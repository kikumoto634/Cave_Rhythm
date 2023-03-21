#include "Combo.h"

#include "../../Engine/math/Easing/Easing.h"

using namespace std;

void Combo::Initialize()
{
	//�R���{�e�L�X�g
	comboSp = make_unique<BaseSprites>();
	comboSp->Initialize(2);
	comboSp->SetPosition({50,300});
	comboSp->SetSize({150,75});
	comboSp->SetColor({comboSpColor.x,comboSpColor.y,comboSpColor.z,1});

	//����
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i] = make_unique<BaseSprites>();
		numberSp_combo[i]->Initialize(TexNumberBegin + 0);
		numberSp_combo[i]->SetPosition({float(50+(i*50)),375});
		numberSp_combo[i]->SetSize({50,75});
	}
}

void Combo::Update()
{
	comboSp->SetColor({comboSpColor.x,comboSpColor.y,comboSpColor.z,1});
	comboSp->Update();
	//����
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Update();
	}

	//���Z�b�g�F�ω�
	if(IsReset){
		comboSpColor.x = Easing_Linear_Point2(0,1,Time_OneWay(curColorChangeFrame,ComboResetColorSecond));
		comboSpColor.y = comboSpColor.x;

		if(comboSpColor.x >= 1){
			IsReset = false;
			curColorChangeFrame = 0;
		}
	}
}

void Combo::Draw()
{
	comboSp->Draw();
	//����
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Draw();
	}
}

void Combo::Finalize()
{
	//����
	for(int i = 0;i < NumberSpSize; i++){
		numberSp_combo[i]->Finalize();
	}
	comboSp->Finalize();
}

void Combo::Increment()
{
	comboNum += 1;

	//�X�v���C�g�X�V
	int hundred = comboNum/100;
	int ten = (comboNum - (hundred*100))/10;
	int one = (comboNum - (hundred*100) - (ten*10))/1;
	numberSp_combo[0]->SetTexNumber(hundred + TexNumberBegin);
	numberSp_combo[1]->SetTexNumber(ten + TexNumberBegin);
	numberSp_combo[2]->SetTexNumber(one + TexNumberBegin);
}

void Combo::Reset()
{
	comboNum = 0;
	//�X�v���C�g�X�V
	for(int i = 0; i <NumberSpSize; i++){
		numberSp_combo[i]->SetTexNumber(TexNumberBegin);
	}

	//�F�ύX
	if(IsReset) return;
	IsReset = true;
	comboSpColor = {0,0,1};
}
