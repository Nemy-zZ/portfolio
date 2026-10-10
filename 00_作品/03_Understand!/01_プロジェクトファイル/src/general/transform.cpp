//------------------------------------------------------------
// @file		transform.cpp
// @brief	トランスフォーム
// @detail	位置や回転などの情報を持つクラス
//------------------------------------------------------------
#include "transform.h"

std::vector<TransformMonitor*> TransformMonitor::m_monitors;

//=============================================================
// ゲームオブジェクト付きコンストラクタ
//=============================================================
Transform::Transform(GameObject* attach)
{
	// 初期化
	position = { 0.0f, 0.0f, 0.0f };
	D3DXQuaternionIdentity(&rotation);
	scale = { 1.0f, 1.0f, 1.0f };
	size = { 1.0f, 1.0f };
	m_parent = nullptr;

	// ゲームオブジェクトをアタッチする
	m_attachObject = attach;
}

//=============================================================
// デストラクタ
//=============================================================
Transform::~Transform()
{
	TransformMonitor::Detatch(this);
}

//=============================================================
// 親を設定する
//=============================================================
void Transform::SetParent(Transform* parent)
{
	bool isSuccess = true;
	Transform* par = parent;
	while (par != nullptr)
	{
		// 親が自分に設定されていないかを確認する
		if (par->m_parent == this)
		{
			isSuccess = false;
			break;
		}

		// 次の親
		par = par->m_parent;
	}

	// 成功時
	if (isSuccess)
	{
		m_parent = parent;
	}
}

//=============================================================
// 回転する
//=============================================================
void Transform::Rotate(float x, float y, float z)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, y, x, z);
	D3DXQuaternionMultiply(&rotation, &rotation, &q);
}

//=============================================================
// オイラー角で回転を取得する
//=============================================================
D3DXVECTOR3 Transform::GetEulerAngle()
{
	return QuaternionToEulerAngle(rotation);
}

//=============================================================
// オイラー角で回転を設定する
//=============================================================
void Transform::SetEulerAngle(float x, float y, float z)
{
	D3DXQUATERNION q;
	D3DXQuaternionRotationYawPitchRoll(&q, y, x, z);
	rotation = q;
}

//=============================================================
// 指定位置の方向に回転する
//=============================================================
void Transform::LookAt(float x, float y, float z)
{
	rotation = Benlib::LookAt(position, D3DXVECTOR3(x, y, z));
}

//=============================================================
// ワールド基準の位置を取得する
//=============================================================
D3DXVECTOR3 Transform::GetWorldPosition()
{
	D3DXMATRIX mtx = GetMatrix();
	return D3DXVECTOR3(mtx._41, mtx._42, mtx._43);
}

//=============================================================
// ワールド基準の回転を取得する
//=============================================================
D3DXQUATERNION Transform::GetWorldRotation()
{
	D3DXQUATERNION worldQuaternion;
	D3DXQuaternionIdentity(&worldQuaternion);

	Transform* worldTransform = this;
	do
	{
		D3DXQUATERNION q = worldTransform->rotation;
		D3DXQuaternionMultiply(&worldQuaternion, &worldQuaternion, &q);

		// 次の親に進む
		worldTransform = worldTransform->GetParent();
	} while (worldTransform != nullptr);

	// 正規化
	D3DXQuaternionNormalize(&worldQuaternion, &worldQuaternion);

	return worldQuaternion;
}

//=============================================================
// ワールド基準のオイラー角を取得する
//=============================================================
D3DXVECTOR3 Transform::GetWorldEulerAngle()
{
	return QuaternionToEulerAngle(GetWorldRotation());
}

//=============================================================
// ワールド基準のスケールを取得する
//=============================================================
D3DXVECTOR3 Transform::GetWorldScale()
{
	// 親のワールド座標をローカル座標に掛ける
	D3DXVECTOR3 worldScale = { 1.0f, 1.0f, 1.0f };
	Transform* worldTransform = this;
	do
	{
		// 座標を掛ける
		worldScale.x *= worldTransform->scale.x;
		worldScale.y *= worldTransform->scale.y;
		worldScale.z *= worldTransform->scale.z;

		// 次の親に進む
		worldTransform = worldTransform->GetParent();
	} while (worldTransform != nullptr);

	return worldScale;
}

//=============================================================
// マトリックスを取得する
//=============================================================
D3DXMATRIX& Transform::GetMatrix()
{
	// 変数
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_matrix);

	// スケールを反映
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationQuaternion(&mtxRot, &rotation);
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &mtxRot);
	//31 32 33で前方方向を取得できる

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_matrix, &m_matrix, &mtxTrans);

	if (m_parent != nullptr)
	{
		D3DXMATRIX mtxParent;
		mtxParent = m_parent->GetMatrix();
		D3DXMatrixMultiply(&m_matrix, &m_matrix, &mtxParent);
	}

	return m_matrix;
}

//=============================================================
// 位置情報マトリックスを取得する
//=============================================================
D3DXMATRIX Transform::GetTranslationMatrix()
{
	D3DXMATRIX mtx;
	D3DXVECTOR3 wPos = GetWorldPosition();
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixTranslation(&mtx, wPos.x, wPos.y, wPos.z);
	return mtx;
}

//=============================================================
// 回転情報マトリックスを取得する
//=============================================================
D3DXMATRIX Transform::GetRotationMatrix()
{
	D3DXMATRIX mtx;
	D3DXQUATERNION wQuaternion = GetWorldRotation();
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationQuaternion(&mtx, &wQuaternion);
	return mtx;
}

//=============================================================
// 演算子 ==
//=============================================================
bool Transform::operator==(Transform value)
{
	return (position == value.position &&
		rotation == value.rotation &&
		scale == value.scale &&
		size == value.size &&
		m_parent == value.m_parent);
}

//=============================================================
// 演算子 !=
//=============================================================
bool Transform::operator!=(Transform value)
{
	return !(position == value.position &&
		rotation == value.rotation &&
		scale == value.scale &&
		size == value.size &&
		m_parent == value.m_parent);
}

//=============================================================
// 演算子 =
//=============================================================
Transform& Transform::operator=(Transform value)
{
	position = value.position;
	rotation = value.rotation;
	scale = value.scale;
	size = value.size;
	m_parent = value.m_parent;
	return *this;
}

//=============================================================
// クォータニオンをオイラー角に変換する
//=============================================================
D3DXVECTOR3 Transform::QuaternionToEulerAngle(D3DXQUATERNION q)
{
	auto sx = -(2 * q.y * q.z - 2 * q.x * q.w);
	auto unlocked = std::abs(sx) < 0.99999f;
	return D3DXVECTOR3(
		std::asin(sx),
		unlocked ? std::atan2(2 * q.x * q.z + 2 * q.y * q.w, 2 * q.w * q.w + 2 * q.z * q.z - 1)
		: std::atan2(-(2 * q.x * q.z - 2 * q.y * q.w), 2 * q.w * q.w + 2 * q.x * q.x - 1),
		unlocked ? std::atan2(2 * q.x * q.y + 2 * q.z * q.w, 2 * q.w * q.w + 2 * q.y * q.y - 1) : 0
	);
}


//=============================================================
// コンストラクタ
//=============================================================
TransformMonitor::TransformMonitor() :
	m_target(nullptr),
	m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f)),
	m_scale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR2(0.0f, 0.0f)),
	m_changedPosition(false),
	m_changedRotation(false),
	m_changedScale(false),
	m_changedSize(false)
{
	// リストに追加する
	m_monitors.emplace_back(this);
}

//=============================================================
// コンストラクタ
//=============================================================
TransformMonitor::TransformMonitor(Transform* target) :
	m_target(nullptr),
	m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_rotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f)),
	m_scale(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_size(D3DXVECTOR2(0.0f, 0.0f)),
	m_changedPosition(false),
	m_changedRotation(false),
	m_changedScale(false),
	m_changedSize(false)
{
	// 監視対象を設定する
	SetTarget(target);

	// リストに追加する
	m_monitors.emplace_back(this);
}

//=============================================================
// デストラクタ
//=============================================================
TransformMonitor::~TransformMonitor()
{
	auto itr = std::find(m_monitors.begin(), m_monitors.end(), this);
	if (itr != m_monitors.end())
	{
		(*itr)->m_target = nullptr;
		m_monitors.erase(itr);
	}
}

//=============================================================
// 監視変数を更新する
//=============================================================
void TransformMonitor::UpdateMonitorValue()
{
	m_position = m_target->GetWorldPosition();
	m_rotation = m_target->GetWorldRotation();
	m_scale = m_target->GetWorldScale();
	m_size = m_target->size;
}

//=============================================================
// 更新する
//=============================================================
void TransformMonitor::Update()
{
	// 変更フラグをリセットする
	m_changedPosition = false;
	m_changedRotation = false;
	m_changedScale = false;
	m_changedSize = false;

	if (m_target != nullptr)
	{
		// 変更があればフラグを立てる
		m_changedPosition = (m_target->GetWorldPosition() != m_position);
		m_changedRotation = (m_target->GetWorldRotation() != m_rotation);
		m_changedScale = (m_target->GetWorldScale() != m_scale);
		m_changedSize = (m_target->size != m_size);

		// 最新状態に更新する
		UpdateMonitorValue();
	}
}

//=============================================================
// 監視対象を設定する
//=============================================================
void TransformMonitor::SetTarget(Transform* target)
{
	// 監視対象を設定する
	m_target = target;

	// 監視変数を設定する
	UpdateMonitorValue();
}

//=============================================================
// 変更されたか
//=============================================================
bool TransformMonitor::HasChanged()
{
	return (m_changedPosition || m_changedRotation || m_changedScale || m_changedSize);
}

//=============================================================
// デタッチする
//=============================================================
void TransformMonitor::Detatch(Transform* target)
{
	for (auto itr = m_monitors.begin(); itr != m_monitors.end(); itr++)
	{
		if ((*itr)->m_target == target)
		{
			delete* itr;
			itr = m_monitors.begin();
			if (itr == m_monitors.end())
			{
				return;
			}
		}
	}
}

//=============================================================
// すべて更新する
//=============================================================
void TransformMonitor::AllUpdate()
{
	for (auto itr = m_monitors.begin(); itr != m_monitors.end(); itr++)
	{
		(*itr)->Update();
	}
}
