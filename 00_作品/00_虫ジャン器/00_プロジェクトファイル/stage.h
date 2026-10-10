//=======================================================================================================
//
//ステージの処理[stage.h]
// Author MurataShinnosuke
//
//=======================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

//ステージ
class Stage
{
public:
	//種類の列挙
	enum TYPE
	{
		IS_DEMO = 0,
	};

	//コンストラクタ
	Stage()
	{
	}

	//デストラクタ
	~Stage()
	{
	}

	void Load(Stage::TYPE type);   //読み込み

private:
	void LoadDemo();
};

#endif // !_STAGE_H_
