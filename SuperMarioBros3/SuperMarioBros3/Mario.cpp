#include "Mario.h"

#include "AnimationManager.h"
#include "Game.h"
#include "Ultis.h"
#include "MarioConst.h"
#include "Const.h"
#include "Sprite.h"

#include "CollisionBox.h"
#include "Enemy.h"
#include "KoopaShell.h"
#include "KoopaConst.h"

#include <cstdlib>
#include <cctype> 
#include <string>
#include "Koopa.h"
#include "MarioCollisionBox.h"
#include "SceneManager.h"
#include "MarioControllerConst.h"
#include "MarioController.h"
#include "QuestionBlock.h"
#include "Label.h"
#include "Portal.h"
using namespace std;

CMario::CMario()
{
	LoadAnimation();
	InitProperties();
	tag = GameObjectTags::Player;

}

void CMario::SetDamageFlag(bool isDamaged)
{
	this->isDamaged = isDamaged;
}

void CMario::SetChangeSmokeEffectFlag(bool isChangeSmokeEffect)
{
	this->isSmokeEffectAnimation = isChangeSmokeEffect;
}

void CMario::SetChangeLevelFlag(bool flag)
{
	this->isChangeLevel = flag;
}

void CMario::SetPowerUp(bool pU)
{
	this->isPowerUp = pU;
}

void CMario::SetPowerUpItem(PowerupTag powerupItem)
{
	this->powerupItem = powerupItem;
}

void CMario::SetCountChangeAlpha(int count)
{
	countChangeAlpha = count;
}

void CMario::SetCountSmokeEffectActivate(int count)
{
	countSmokeEffectActivate = count;
}

void CMario::SetTimeStartDamaged(DWORD t)
{
	timeStartDamaged = t;
}

void CMario::SetTimeStartSmokeEffect(DWORD t)
{
	timeStartSmokeEffect = t;
}

void CMario::SetTimeStartChangeLevel(DWORD t)
{
	timeStartChangeLevel = t;
}

void CMario::SetTimeStartChangeAlpha(DWORD t)
{
	timeStartChangeAlpha = t;
}

bool CMario::GetDamageFlag()
{
	return isDamaged;
}

bool CMario::GetChangeSmokeEffectFlag()
{
	return isSmokeEffectAnimation;
}

bool CMario::GetChangeLevelFlag()
{
	return isChangeLevel;
}

bool CMario::GetCountChangeAlpha()
{
	return countChangeAlpha;
}

bool CMario::GetCountSmokeEffectActivate()
{
	return countSmokeEffectActivate;
}

bool CMario::IsPowerUp()
{
	return isPowerUp;
}

PowerupTag CMario::GetPowerupItem()
{
	return powerupItem;
}

DWORD CMario::GetTimeStartDamaged()
{
	return timeStartDamaged;
}

DWORD CMario::GetTimeStartSmokeEffect()
{
	return timeStartSmokeEffect;
}

DWORD CMario::GetTimeStartChangeLevel()
{
	return timeStartChangeLevel;
}

DWORD CMario::GetTimeStartChangeAlpha()
{
	return timeStartChangeAlpha;
}

void CMario::Init()
{
	this->SetState(MARIO_STATE_IDLE); 
	CMarioCollisionBox* collisionBox = new CMarioCollisionBox();
	collisionBox->SetSizeBox(SUPER_MARIO_BBOX); // Big
	collisionBox->SetPosition(D3DXVECTOR2(0.0f, 0.0f)); // Local Position
	collisionBox->SetGameObjectAttach(this);
	collisionBox->SetName("Mario");
	collisionBox->SetDistance(D3DXVECTOR2(0.0f, 0.0f));
	this->collisionBoxs->push_back(collisionBox);
}

void CMario::InitProperties()
{
	this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
	this->physiscBody->SetDynamic(true); // có chuyển động
	this->physiscBody->SetGravity(MARIO_GRAVITY);

	currentPhysicsState =
	{
		MoveOnGroundStates::Idle,
		JumpOnAirStates::Stand
	};
	targetVelocity.x = 0.0f;
	targetVelocity.y = 0.0f;

	previousNormal = physiscBody->GetNormal();
	ignoreTimeScale = true;
	isEnabled = true;
	isOnGround = false;
	canHighJump = false;
	isJump = false;	isSkid = false;
	isAttack = false;
	canFly = false;
	isFly = false;
	stopBounce = false;
	isKick = false;
	isDamaged = false;
	isGoToWarpPipe = false;
	canGoToWarpPipe = false;
	isSmokeEffectAnimation = false;
	isChangeLevel = false;
	bounceAfterJumpOnEnemy = false;
	feverTime = MARIO_FEVER_TIME;
	lastFeverTime = 0;
	feverState = 0;
	pMeterCounting = 0.0f;
	beforeJumpPosition = 0.0f;
	timeStartSmokeEffect = 0;
	timeStartChangeLevel = 0;
	timeStartDamaged = 0;
	countSmokeEffectActivate = 0;
	countChangeAlpha = 0;
	timeStartChangeAlpha = 0;
	isPowerUp = false;
	isAutogo = false;
	powerupItem = PowerupTag::None;
	this->SetScale(D3DXVECTOR2(1.0f, 1.0f));
	ventDirection = { 0, 0, 0, 0 };
}

void CMario::LoadAnimation()
{
	auto animationManager = CAnimationManager::GetInstance();
	AddAnimation(MARIO_STATE_DAMAGED, animationManager->Get("ani-mario-damaged"), false);
}

void CMario::EndAnimation()
{
	CGameObject::EndAnimation();
	if (currentState.compare(MARIO_STATE_KICK) == 0)
	{
		isKick = false;
	}
}

void CMario::Update(DWORD dt, CCamera* cam, CCamera* uiCam)
{
	CGameObject::Update(dt, cam, uiCam);
	auto keyboard = CKeyboardManager::GetInstance();
	auto velocity = physiscBody->GetVelocity();
	auto normal = physiscBody->GetNormal();
	previousVelocity = velocity;
	float acceleration = 0.0f;

	D3DXVECTOR2 drag = physiscBody->GetDragForce();
	if (isAutogo == false)
	{
		// Horizontal Movement: Walk, Run, Idle
		if (keyboard->GetKeyStateDown(DIK_RIGHT) || keyboard->GetKeyStateDown(DIK_LEFT))
		{
			// Nhấn nút A chạy ! RUN
			if (keyboard->GetKeyStateDown(DIK_A))
			{
				currentPhysicsState.move = MoveOnGroundStates::Run;
				acceleration = MARIO_RUNNING_ACCELERATION;
				targetVelocity.x = MARIO_RUNNING_SPEED;
			}
			else
			{
				// Không chạy -> Đi bộ bình thường: WALK
				currentPhysicsState.move = MoveOnGroundStates::Walk;
				acceleration = MARIO_WALKING_ACCELERATION;
				targetVelocity.x = MARIO_WALKING_SPEED;
			}

			normal.x = (keyboard->GetKeyStateDown(DIK_RIGHT)) ? 1 : -1;
			physiscBody->SetNormal(normal);
			physiscBody->SetAcceleration(acceleration * normal.x);

			if (abs(velocity.x) < targetVelocity.x)
				velocity.x += physiscBody->GetAcceleration() * dt;

			FrictionProcess(velocity.x, dt); // Kèm lực ma sát kéo lại vận tốc
			physiscBody->SetVelocity(velocity);

			if (abs(velocity.x) > MARIO_RUNNING_SPEED * 0.95f)
				currentPhysicsState.move = MoveOnGroundStates::HighSpeed;

			SkidProcess(velocity);
			if (previousVelocity.x * velocity.x <= 0)
				isSkid = false;
			if (isSkid == true)
				currentPhysicsState.move = MoveOnGroundStates::Skid;
		}
		else
		{
			FrictionProcess(velocity.x, dt);
			physiscBody->SetVelocity(velocity);

			if (velocity.x == 0)
				if (currentPhysicsState.move != MoveOnGroundStates::Idle)
					currentPhysicsState.move = MoveOnGroundStates::Idle;
			isSkid = false;
		}

#pragma region P-METER
		if (currentPhysicsState.move == MoveOnGroundStates::Run
			&& abs(velocity.x) > MARIO_RUNNING_SPEED * 0.15f
			&& pMeterCounting < PMETER_MAX + 1
			&& currentPhysicsState.jump == JumpOnAirStates::Stand
			&& feverState != 2
			&& isHold == false)
		{
			if (feverState != -1)
				feverState = 1;
			pMeterCounting += PMETER_STEP * dt;
			if (pMeterCounting > PMETER_MAX + 1)
			{
				pMeterCounting = PMETER_MAX + 1;
			}
		}
		else if (feverState != 2 && feverState != -1) // nếu feverState đang = 1 mà k thỏa những điều kiện trên thì reset lại
			feverState = 0;

#pragma region FEVER STATE
		if (pMeterCounting >= PMETER_MAX && feverState == 0)
		{
			feverState = 2;
			lastFeverTime = GetTickCount();
		}
		else if (pMeterCounting > 0 && feverState == -1) // feverState = -1 là con raccoon
		{
			if (pMeterCounting >= PMETER_MAX)
			{
				canFly = true;
			}
		}
		if (feverState == 2)
		{
			pMeterCounting = PMETER_MAX; // giữ giá trị max 1 thời gian
			if (GetTickCount() - lastFeverTime > MARIO_FEVER_TIME)
			{
				feverState = 0;
				pMeterCounting = 0.0f;
			}
		}
#pragma endregion

#pragma endregion
		// Vertical Movement: Jump, High Jump, Super Jump

#pragma region STATE JUMP

// Điều kiện để có high-jump => Đã phải Jump (ấn 1 lần nút S) + giữ lâu tới target
// Để bé Mario k làm ninja thì phải kiểm tra OnCollisionEnter để coi ẻm có onGround hay chưa
// Điều kiện để có super-jump => Đã phải run tới mức cao nhất của p-meter => và nhấn S
// Xử lý super-jump tương đương high-jump nhưng tăng giới hạn khoảng cách lên cao hơn high-jump (và chuyển qua animation giơ 2 tay và ưỡn bụng)

// Đối với ấn nút X giữ lâu => Mario sẽ nhảy liên tục
// Ấn S giữ lâu thì chỉ nhảy cao hơn thôi
		if (keyboard->GetKeyStateDown(DIK_X) && isOnGround == true)
		{
			// Nhảy liên tục: Chỉ cần cung cấp dy < 0 và có gravity thì ta tạo cảm giác nó nhảy liên tục chuyển động đều

			if (canLowJumpContinous == true)
			{
				velocity.y -= MARIO_JUMP_SPEED_Y;
				isOnGround = false;
			}
			currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		else if (currentPhysicsState.jump == JumpOnAirStates::LowJump)
		{
			if (velocity.y > 0) // Hướng xuống 
			{
				currentPhysicsState.jump = JumpOnAirStates::Fall;
			}
			else
				currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Jump && canLowJumpContinous == false && canHighJump == true)
		{
			if ((keyboard->GetKeyStateDown(DIK_S) && isFly == false) || bounceAfterJumpOnEnemy == true)
			{
				float jumpMaxHeight;
				if ((feverState == 2 && abs(velocity.x) > MARIO_RUNNING_SPEED * 0.85f) || bounceAfterJumpOnEnemy == true)
				{
					// SUPER JUMP
					jumpMaxHeight = MARIO_SUPER_JUMP_HEIGHT;
				}
				else
				{
					// HIGH - JUMP
					jumpMaxHeight = MARIO_HIGH_JUMP_HEIGHT;

				}
				if (abs(beforeJumpPosition) - abs(transform.position.y) <= jumpMaxHeight)
				{
					velocity.y = -MARIO_PUSH_FORCE;
				}
				else
				{
					// EndJump
					velocity.y = -MARIO_PUSH_FORCE / 2;
					canHighJump = false;
				}
				if (bounceAfterJumpOnEnemy == true)
				{
					bounceAfterJumpOnEnemy = false;
					canHighJump = false;
					stopBounce = true; // ****
				}

			}
		}
		if (velocity.y > 0)
		{
			currentPhysicsState.jump = JumpOnAirStates::Fall;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fly) // Có thể lỗi ở đây
		{
			if (isOnGround == true)
				currentPhysicsState.jump = JumpOnAirStates::Stand;
			feverState = 0;
			pMeterCounting = 0;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Fall)
		{
			if (isOnGround == true)
				currentPhysicsState.jump = JumpOnAirStates::Stand;
			isJump = false;
		}


#pragma endregion

		physiscBody->SetVelocity(velocity);
		physiscBody->SetNormal(normal);


		if (canCrouch == true) // Small Mario k thể crouch
			CrouchProcess(keyboard);
		HoldProcess();
		GoToWarpPipeProcess();

		DamageProcess();
		if (isKick == true)
		{
			previousPhysicsState.move = currentPhysicsState.move;
			currentPhysicsState.move = MoveOnGroundStates::Kick;
		}
		if (currentPhysicsState.move == MoveOnGroundStates::Kick && isKick == false)
			currentPhysicsState.move = previousPhysicsState.move;
	}
	else
	{
		WarpPipeProcess(cam);
	}	
}

void CMario::Render(CCamera* cam, int alpha)
{
	SetScale(D3DXVECTOR2(physiscBody->GetNormal().x, 1.0f));

#pragma region Update State
	if (isSmokeEffectAnimation == false && isGoToWarpPipe == false)
	{
#pragma region Move On Ground
		switch (currentPhysicsState.move)
		{
		case MoveOnGroundStates::Idle:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_IDLE);
			else
				SetState(MARIO_STATE_IDLE);
			break;
		}
		case MoveOnGroundStates::Walk:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_MOVE);
			else
				SetState(MARIO_STATE_WALKING);
			break;
		}
		case MoveOnGroundStates::Run:
		{
			if (isHold == true)
				SetState(MARIO_STATE_HOLD_MOVE);
			else
				SetState(MARIO_STATE_RUNNING);
			break;
		}
		case MoveOnGroundStates::Skid:
		{
			if (isHold == false)
			{
				auto normal = physiscBody->GetNormal();
				SetScale(D3DXVECTOR2(-normal.x, 1.0f));
				SetState(MARIO_STATE_SKID);
				break;
			}

		}
		case MoveOnGroundStates::Crouch:
		{
			if (isHold == false)
				SetState(MARIO_STATE_CROUCH);
			break;
		}
		case MoveOnGroundStates::HighSpeed:
		{
			if (isHold == false)
				SetState(MARIO_STATE_HIGH_SPEED);
			break;
		}
		case MoveOnGroundStates::Attack:
		{
			if (isHold == false) // Tạm thời
				SetState(MARIO_STATE_ATTACK);
			break;
		}
		case MoveOnGroundStates::JumpAttack:
		{
			if (isHold == false)
				SetState(MARIO_STATE_JUMP_ATTACK);
			break;
		}
		case MoveOnGroundStates::Kick:
		{
			//DebugOut(L"KICK \n");
			if (isHold == false)
				SetState(MARIO_STATE_KICK);
			break;
		}
		case MoveOnGroundStates::Damaged:
		{
			SetState(MARIO_STATE_DAMAGED);
		}
		}
#	pragma endregion

#pragma region Jump On Air
		if (currentPhysicsState.move != MoveOnGroundStates::JumpAttack && isHold == false)
		{
			switch (currentPhysicsState.jump)
			{
			case JumpOnAirStates::Jump: case JumpOnAirStates::HighJump:
			case JumpOnAirStates::LowJump:
			{
				SetState(MARIO_STATE_JUMP);
				break;
			}
			case JumpOnAirStates::Fall:
			{
				SetState(MARIO_STATE_FALL);
				break;
			}
			case JumpOnAirStates::Fly: //Riêng raccoon
			{
				SetState(MARIO_STATE_FLY);
				break;
			}
			case JumpOnAirStates::Float:
			{
				SetState(MARIO_STATE_FLOAT);
				break;
			}
			}
			if (feverState == 2 && isOnGround == false) // Các mario khác
			{
				SetState(MARIO_STATE_FLY);

			}

		}

#pragma endregion
	}
	else if (isGoToWarpPipe == false)
	{
		SetState(MARIO_STATE_DAMAGED);
	}
	else
	{
		SetState(MARIO_STATE_IDLE_FRONT);
	}

#pragma endregion
	//if (currentState != "IDLE")
		//OutputDebugString(ToLPCWSTR("Current State " + currentState + "\n"));

	SetRelativePositionOnScreen(collisionBoxs->at(0)->GetPosition());
	countChangeAlpha++;

	if (isDamaged == true && isSmokeEffectAnimation == false && isPowerUp == false)
	{
		if ((GetTickCount64() - timeStartChangeAlpha > TIME_TO_CHANGE_ALPHA && timeStartChangeAlpha != 0) || timeStartChangeAlpha == 0)
		{
			alpha = (countChangeAlpha % 2 == 0) ? 0 : 255;
		}
	}
	CGameObject::Render(cam, alpha);
		
}

void CMario::OnCollisionEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> collisionEvents)
{
	for (auto collisionEvent : collisionEvents)
	{
		auto collisionBox = collisionEvent->obj;
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Solid || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::GhostPlatform 
			|| collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::QuestionBlock || collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Brick)
		{
			if (collisionEvent->ny < 0 && isOnGround == false)
			{
				isOnGround = true;
				DebugOut(L"OnGround\n");
			}
			if (collisionEvent->nx != 0) // vừa ấn nhảy vừa ấn qua trái phải
			{
				auto v = physiscBody->GetVelocity();
				physiscBody->SetVelocity(D3DXVECTOR2(0, v.y));
				pMeterCounting = 0;
			}
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::QuestionBlock && collisionEvent->ny > 0)
		{
			// Mario cụng đầu lên question block
			FallProcess();
			auto questionBlock = dynamic_cast<CQuestionBlock*>(collisionBox->GetGameObjectAttach());
			questionBlock->Bounce(this);
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Brick && collisionEvent->ny > 0)
		{
			FallProcess();
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Coin)
		{
			// Đụng trúng tiền là tăng tiền và disbale tiền
			collisionBox->GetGameObjectAttach()->Enable(false);
		}
		if (collisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Portal)
		{
			isGoToWarpPipe = false;
			canGoToWarpPipe = false;
			auto portal = static_cast<CPortal*>(collisionBox->GetGameObjectAttach());
			auto sceneID = portal->GetSceneID();
			auto cameraID = portal->GetCameraID();
			if (cameraID != -1)
			{
				// Đổi camera
				DebugOut(L"Cameraaaa %d\n", cameraID);
				auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
				activeScene->SetCamera(cameraID);
			}
			if (sceneID != -1)
			{
				// Đổi scene
			}
		}

	}
}

void CMario::OnTriggerEnter(CCollisionBox* selfCollisionBox, std::vector<CollisionEvent*> otherCollisions)
{
}

void CMario::OnOverlappedEnter(CCollisionBox* selfCollisionBox, CCollisionBox* otherCollisionBox)
{
	//if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Enemy)
	//{
	//	// Nếu overlap thì không xét hướng được, chỉ khi theo chiều ngang hoặc quái trên đầu mario mario mới bị damage thôi
	//	OnDamaged();
	//}
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::VenusFireBall)
	{
		OnDamaged();
		otherCollisionBox->GetGameObjectAttach()->Enable(false);
	}
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Label)
	{
		// Khi mario đụng trúng object label
		// Nó sẽ lấy cái push direction ra và di chuyển theo hướng đó 
		// Autogo => Disable trạng thái vật lý update, chỉ render và thay đổi relative position
		auto label = static_cast<CLabel*>(otherCollisionBox->GetGameObjectAttach());
		auto direction = label->GetPushDirection();
		ventDirection = direction;
		OnGoToWarpPipe();
	}
	if (otherCollisionBox->GetGameObjectAttach()->GetTag() == GameObjectTags::Portal)
	{
		isGoToWarpPipe = false;
		canGoToWarpPipe = false;
		auto portal = static_cast<CPortal*>(otherCollisionBox->GetGameObjectAttach());
		auto sceneID = portal->GetSceneID();
		auto cameraID = portal->GetCameraID();
		if (cameraID != -1)
		{
			// Đổi camera
			auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
			activeScene->SetCamera(cameraID);
		}
		if (sceneID != -1)
		{
			// Đổi scene
		}
			
	}
}

bool CMario::CanCollisionWithThisObject(LPGameObject gO, GameObjectTags tag)
{
	if (MarioTag(tag) || tag == GameObjectTags::MarioFireBall || tag == GameObjectTags::Label)
		return false;
	if (tag == GameObjectTags::Label)
		return false;
	if (GiftTag(tag) == true && tag != GameObjectTags::Coin)
		return false;
	if (isGoToWarpPipe == true && StaticTag(tag))
		return false;
	return true;
}

void CMario::CrouchProcess(CKeyboardManager* keyboard)
{
	// Khi chuyển qua Crouch, chiều cao của Crouch nhỏ lại => Ta phải thay dổi lại boxsize và localposition

	// Một số ràng buộc khi crouch:
	// Khi vx = 0 và nhấn xuống => Crouch. Khi đang nhấn xuống mà vx != 0 (Đi / chạy qua lại) là chuyển qua ani Đi / chạy
	// Còn đang nhảy (vy != 0) và ấn xuống thì vẫn crouch. Còn 1 lúc bắt k kịp trạng thái bấm qua lại khi đang nhảy (vy != 0 && vx != 0) thì nó có thể vẫn crouch. 

	bool changeAniState = false;
	if (keyboard->GetKeyStateDown(DIK_LEFT) || keyboard->GetKeyStateDown(DIK_RIGHT) || currentState == MARIO_STATE_ATTACK || currentState == MARIO_STATE_IDLE_FRONT) // thiếu xét trường hợp nhảy ******
	{
		// KHÔNG HỤP
		changeAniState = true;
	}
	if ((changeAniState == false && keyboard->GetKeyStateDown(DIK_DOWN)))
	{
		// HỤP
		collisionBoxs->at(0)->SetSizeBox(SUPER_MARIO_CROUCH_BBOX);
		float transformY = SUPER_MARIO_BBOX.y - SUPER_MARIO_CROUCH_BBOX.y;
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, transformY*0.5f));
		currentPhysicsState.move = MoveOnGroundStates::Crouch;
	}
	else
	{
		// KHÔNG HỤP
		collisionBoxs->at(0)->SetSizeBox(SUPER_MARIO_BBOX);
		collisionBoxs->at(0)->SetPosition(D3DXVECTOR2(0.0f, 0.0f));
	}
}

void CMario::SkidProcess(D3DXVECTOR2 velocity)
{
	auto keyboard = CKeyboardManager::GetInstance();
	int sign = keyboard->GetKeyStateDown(DIK_RIGHT) ? 1 : -1;
	if (isOnGround == true && velocity.x * sign < 0) 
	{
		isSkid = true;
	}
}

void CMario::HoldProcess()
{
	auto keyboard = CKeyboardManager::GetInstance();
	auto normal = physiscBody->GetNormal();
	if (isHold == true && objectHolding != NULL)
	{
		if (keyboard->GetKeyStateDown(DIK_A) == true) // Vẫn đang ấn giữ phím A
		{
			D3DXVECTOR2 posHoldable = transform.position;
			// Chưa xét trường hợp small mario
			posHoldable.x += (SUPER_MARIO_BBOX.x + objectHolding->GetHoldableCollisionBox().x) * normal.x*0.4f;
			objectHolding->SetHoldablePosition(posHoldable);
			objectHolding->SetHoldableNormal(normal);
		}
		else
		{
			// thả ra => vẫn còn nút A => tưởng bị A
			objectHolding->Release();
			objectHolding = NULL;
			isHold = false;
			isKick = true;
		}
	}
}

void CMario::JumpProcess(float jumpForce, bool bounceAfterJumpOnEnemy)
{
	physiscBody->SetVelocity(D3DXVECTOR2(physiscBody->GetVelocity().x, jumpForce));
	isJump = true;
	isOnGround = false;
	this->bounceAfterJumpOnEnemy = bounceAfterJumpOnEnemy;
	if (bounceAfterJumpOnEnemy == true)
		currentPhysicsState.jump = JumpOnAirStates::Jump;
}

void CMario::KickProcess(bool isKick)
{
	this->isKick = isKick;
}

void CMario::DamageProcess()
{
	if (timeStartDamaged != 0)
	{
		if (GetTickCount64() - timeStartDamaged > TIME_TO_BE_DAMAGED)
		{
			isDamaged = false;
			timeStartDamaged = 0;
			timeStartChangeLevel = 0;
			timeStartChangeAlpha = 0;
			countSmokeEffectActivate = 0;
			countChangeAlpha = 0;
		}
	}

	if (isSmokeEffectAnimation == true && GetTickCount64() - timeStartSmokeEffect > TIME_TO_SMOKE_EFFECT)
	{
		isSmokeEffectAnimation = false;
		timeStartSmokeEffect = 0;
		timeStartChangeLevel = GetTickCount64();

		timeStartChangeAlpha = GetTickCount64();
		countChangeAlpha++;
	}
	
	if (isDamaged == true && isSmokeEffectAnimation == false)
	{
		countSmokeEffectActivate++;
		if (countSmokeEffectActivate == 1)
		{
			// CHANGE SMOKE EFFECT
			timeStartSmokeEffect = GetTickCount64();
			CGame::SetTimeScale(0.0f);
			isSmokeEffectAnimation = true;
		}
	}
	
	// CHANGE LEVEL
	if (isChangeLevel == true && GetTickCount64() - timeStartChangeLevel > TIME_TO_CHANGE_LEVEL)
	{

		timeStartChangeLevel = 0;
		isChangeLevel = false;
		return;
	}
	
	if (isDamaged == true && timeStartChangeLevel != 0 && isChangeLevel == false)
	{
		ChangeLevelProcess();
	}
}

void CMario::ChangeLevelProcess()
{
	auto activeScene = CSceneManager::GetInstance()->GetActiveScene();
	auto objs = activeScene->GetObjects();
	CMarioController* marioController = NULL;

	for (auto obj : objs)
	{
		if (obj->GetTag() == GameObjectTags::PlayerController)
		{
			marioController = dynamic_cast<CMarioController*>(obj);
			break;
		}
	}
	if (marioController != NULL)
	{
		CGame::SetTimeScale(1.0f);
		DebugOut(L"Time Scale In Mario Controller %f \n", CGame::GetTimeScale());

		isChangeLevel = true;
		switch (powerupItem)
		{
			case PowerupTag::None:
			{
				// Damaged
				if (marioStateTag == MarioStates::RacoonMario || marioStateTag == MarioStates::FireMario)
					marioController->SwitchToState(SUPER_MARIO_STATE);
				else if (marioStateTag == MarioStates::SuperMario)
					marioController->SwitchToState(SMALL_MARIO_STATE);
				break;
			}
			case PowerupTag::SuperLeaf:
			{
				if (marioStateTag == MarioStates::SuperMario)
					marioController->SwitchToState(RACOON_MARIO_STATE);
				break;
			}
			case PowerupTag::SuperMushroom:
			{
				if (marioStateTag == MarioStates::SmallMario)
					marioController->SwitchToState(SUPER_MARIO_STATE);
				break;
			}
		}
	}

}

void CMario::FallProcess()
{
	auto vel = physiscBody->GetVelocity();
	vel.y += MARIO_DEFLECT_BLOCK;
	physiscBody->SetVelocity(vel);
	currentPhysicsState.jump = JumpOnAirStates::Fall;
	isJump = false;
	canHighJump = false;
	isHighJump = false;
}

void CMario::GoToWarpPipeProcess()
{
	auto keyboard = CKeyboardManager::GetInstance();
	if (canGoToWarpPipe == true)
	{
		if (keyboard->GetKeyStateDown(DIK_DOWN) && ventDirection.bottom == 1)
		{
			isGoToWarpPipe = true;
			isAutogo = true;
		}
		if (keyboard->GetKeyStateDown(DIK_UP) && ventDirection.top == 1)
		{
			isGoToWarpPipe = true;
			isAutogo = true;
		}
	}
	
}

void CMario::WarpPipeProcess(CCamera* cam)
{
	if (isGoToWarpPipe == true)
	{
		this->physiscBody->SetGravity(0.0f);
		this->physiscBody->SetVelocity(D3DXVECTOR2(0.0f, 0.0f));
		if (ventDirection.bottom == 1)
			transform.position.y += MARIO_VENT_SPEED * CGame::GetInstance()->GetDeltaTime();
		if (ventDirection.top == 1)
			transform.position.y -= MARIO_VENT_SPEED * CGame::GetInstance()->GetDeltaTime();
	}
	else
	{
		isAutogo = false;
		//this->transform.position = D3DXVECTOR2(6237, 1366);
		auto camPos = cam->GetPositionCam();
		camPos.x += cam->GetWidthCam() / 2;
		SetPosition(camPos);
		this->physiscBody->SetGravity(MARIO_GRAVITY);
	}
}

void CMario::StopBounce(bool stopBounce)
{
	this->stopBounce = stopBounce;
}

bool CMario::StopBounce()
{
	return stopBounce;
}

void CMario::KeyState()
{
	
}

void CMario::OnKeyDown(int KeyCode)
{
	// EVENT
	if ((KeyCode == DIK_S || KeyCode == DIK_X) && isOnGround == true && currentPhysicsState.jump == JumpOnAirStates::Stand)
	{
		// JUMP
		JumpProcess(-MARIO_JUMP_FORCE, false);
		if (KeyCode == DIK_S)
		{
			canHighJump = true;
			canLowJumpContinous = false;
			beforeJumpPosition = transform.position.y;

		}
		if (KeyCode == DIK_X)
		{
			canLowJumpContinous = true;
			if (currentPhysicsState.jump == JumpOnAirStates::Stand
				|| currentPhysicsState.jump == JumpOnAirStates::Jump)
				currentPhysicsState.jump = JumpOnAirStates::LowJump;
		}
		if (currentPhysicsState.jump == JumpOnAirStates::Stand && KeyCode == DIK_S)
			currentPhysicsState.jump = JumpOnAirStates::Jump;
	}
	if ((KeyCode == DIK_Z || KeyCode == DIK_A)&& canAttack == true && isAttack == false && currentPhysicsState.move != MoveOnGroundStates::Attack)
	{
		beginAttackTime = GetTickCount64();
		isAttack = true;
	}
}

void CMario::OnKeyUp(int KeyCode)
{
	if (KeyCode == DIK_S)
	{
		canHighJump = false;
	}
}

void CMario::OnDamaged()
{
	isDamaged = true;
	timeStartDamaged = GetTickCount64();
}

void CMario::OnGoToWarpPipe()
{
	canGoToWarpPipe = true;
}

void CMario::HoldObject(CHoldable* holdableObj)
{
	this->objectHolding = holdableObj;
	isHold = true;
	holdableObj->SetHolder(this);
}

void CMario::SetMarioStateTag(MarioStates tag)
{
	marioStateTag = tag;
}

MarioStates CMario::GettMarioStateTag()
{
	return marioStateTag;
}

bool CMario::CanRun()
{
	return currentPhysicsState.move == MoveOnGroundStates::Run;
}

void CMario::Access()
{
	this->isEnabled = true;
	//this->collisionBoxs->at(0)->GetGameObjectAttach()->Enable(true);
	//DebugOut(L"Access: Collision box enable: %d \n", this->collisionBoxs->at(0)->GetGameObjectAttach()->IsEnabled());

}

void CMario::Process()
{

}

void CMario::Exit()
{
	this->isEnabled = false;
	//this->collisionBoxs->at(0)->GetGameObjectAttach()->Enable(false); // solid box . Nếu solid box bị disable thì sao? thì không update
	//DebugOut(L"Exit: Collision box enable: %d \n", this->collisionBoxs->at(0)->GetGameObjectAttach()->IsEnabled());
}

CMario::~CMario()
{
	CGameObject::~CGameObject();

}
