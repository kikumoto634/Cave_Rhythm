#pragma once


/// <summary>
/// �����蔻�葮��
/// </summary>

const unsigned short COLLISION_ATTR_LANDSHAPE = 0b1 << 0; //�n�`
const unsigned short COLLISION_ATTR_ALLIES = 0b1 << 1; //�����w�c
const unsigned short COLLISION_ATTR_ENEMYS = 0b1 << 2; //�G�w�c
const unsigned short COLLISION_ATTR_WEAPONS = 0b11 << 1; //����w�c
const unsigned short COLLISION_ATTR_ITEMS = 0b11 << 2;	//�A�C�e��
const unsigned short COLLISION_ATTR_DUMMY = 0b11 << 3;	//�_�~�[