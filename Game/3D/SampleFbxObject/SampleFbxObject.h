#pragma once
#include "../BaseFbxObjects.h"
#include "../../../Engine/input/Input.h"

class SampleFbxObject : public BaseFbxObjects
{
//ƒƒ“ƒoŠÖ”
public:
	~SampleFbxObject();

	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(std::string filePath) override;

	/// <summary>
	/// XV
	/// </summary>
	void Update(Camera* camera);

	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw() override;

	/// <summary>
	/// Œãˆ—
	/// </summary>
	void Finalize() override;

};

