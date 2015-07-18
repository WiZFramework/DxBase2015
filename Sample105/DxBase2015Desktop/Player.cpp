
#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr){}

	//������
	void Player::Create(){
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);

		//���ǌn�̃R���|�[�l���g������ꍇ��Rigidbody������
		auto PtrRedit = AddComponent<Rigidbody>();
		//Seek����
		AddComponent<SeekSteering>();
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//�Փ˔��������
		auto PtrCol = AddComponent<CollisionSphere>();

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");
		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<BasicPNTDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		//�`�悷��e�N�X�`����ݒ�
		PtrDraw->SetTextureResource(L"TRACE_TX");

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		//��������
		SetAlphaActive(true);
		//0�Ԗڂ̃r���[�̃J�����𓾂�
		//LookAtCamera�ł���
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetCamera(0));
		if (PtrCamera){
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
	}

	//�ړ��̌����𓾂�
	Vector3 Player::GetAngle(){
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 && CntlVec[0].fThumbLY != 0){
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetCamera(0);
				//�i�s�����̌������v�Z
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K��
				Angle.Normalize();
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}

	void Player::StartShellBall(){
		//�C�e�̒ǉ�
		auto PtrTrans = GetComponent<Transform>();
		//�v���C���[�̌����𓾂�
		auto PlayerAngle = PtrTrans->GetRotation();
		Vector3 ShellSpeed(sin(PlayerAngle.y), 0, cos(PlayerAngle.y));
		ShellSpeed *= 10.0f;
		//�v���C���[�̈ړ��X�s�[�h�𓾂�
		auto Velo = GetComponent<Rigidbody>()->GetVelocity();
		Velo.y = 0;
		//�ړ��X�s�[�h�����Z
		ShellSpeed += Velo;
		//�ł��グ�̏�����̏����x��ǉ��i�l�͌Œ�j
		ShellSpeed += Vector3(0.0f,8.0f, 0);
		//�O���[�v���ɋ󂫂�����΂��̃I�u�W�F�N�g���ė��p
		//�����łȂ���ΐV�K�ɍ쐬
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		for (auto Ptr : ShellVec){
			//Ptr��weak_ptr�Ȃ̂ŗL�����`�F�b�N���K�v
			if (!Ptr.expired()){
				auto ShellPtr = dynamic_pointer_cast<ShellBall>(Ptr.lock());
				if (ShellPtr){
					if ((!ShellPtr->IsUpdateActive()) && (!ShellPtr->IsDrawActive())){
						ShellPtr->Refresh(PtrTrans->GetPosition(), ShellSpeed);
						return;
					}
				}
			}
		}
		//�����܂ŗ�����󂫂��Ȃ��������ƂɂȂ�
		//�C�e�̒ǉ�
		auto Sh = GetStage()->AddGameObject<ShellBall>(PtrTrans->GetPosition(), ShellSpeed);
		//�O���[�v�ɒǉ�
		Group->IntoGroup(Sh);
	}

	//�X�V
	void Player::Update(){
		//�����̎擾
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//���݈ʒu�����o��
		auto Pos = PtrTransform->GetPosition();
		//�ړ����������Z�B
		//�ړ������������킩��΂����̂ŁA
		//Angle�͐��K������Ăėǂ�
		Pos += Angle;



		//Seek����
		auto PtrSeek = GetComponent<SeekSteering>();
		//���Z���ꂽ�����ɒǂ�������
		PtrSeek->SetTargetPosition(Pos);
		//��]�̌v�Z
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f){
			//�x�N�g����Y����]�ɕϊ�
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else{
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);

		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected){
			//A�{�^���������ꂽ�u�ԂȂ�C�e����
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				//�C�e�̒ǉ�
				StartShellBall();
			}
		}

		//��������Ƃ肾��
		auto PtrString = GetComponent<StringSprite>();
		auto Group = GetStage()->GetSharedObjectGroup(L"ShellBallGroup");
		auto ShellVec = Group->GetGroupVector();
		wstring str(L"�C�e�O���[�v�̖C�e��: ");
		str += Util::UintToWStr(ShellVec.size());
		PtrString->SetText(str);


	}



}
//endof  basedx11
