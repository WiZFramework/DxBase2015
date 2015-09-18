#include "stdafx.h"
#include "Fbx2BinDoc.h"
#include "Fbx2BinView.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	�p�r: FBX���b�V���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	FbxMeshObject::FbxMeshObject(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr),
		m_DataDir(L""),
		m_FbxFileName(L""),
		m_FbxResName(L""),
		m_MeshIndex(0),
		m_FbxMeshResName(L"")
	{}
	void FbxMeshObject::Create(){
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);
		//�C�x���g�}�V���̍\�z
		m_EventMachine = make_shared< EventMachine<FbxMeshObject>>(GetThis<FbxMeshObject>());
		//�C�x���g�L�[�ƃC�x���g�X�e�[�g�����ѕt����
		m_EventMachine->AddEventState(L"DataReset", DataResetEvent::Instance());
		m_EventMachine->AddEventState(L"DataSave", DataSaveEvent::Instance());
		m_EventMachine->AddEventState(L"DataSaveSkin", DataSaveSkinEvent::Instance());
		m_EventMachine->AddEventState(L"RunSkin", RunSkinEvent::Instance());

		

	}
	void FbxMeshObject::ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale){
		try{
			if (m_FbxMeshResName != L""){
				App::GetApp()->UnRegisterResource<FbxMeshResource>(m_FbxMeshResName);
				m_FbxMeshResName = L"";
			}
			if (m_FbxResName != L""){
				App::GetApp()->UnRegisterResource<FbxSceneResource>(m_FbxResName);
				m_FbxResName = L"";
			}
			m_DataDir = DirName;
			m_FbxFileName = FbxName;
			m_FbxResName = FbxName;
			m_MeshIndex = MeshIndex;
			m_FbxMeshResName = m_FbxResName + Util::UintToWStr(m_MeshIndex);
			auto PtrFbxScene = App::GetApp()->RegisterFbxScene(m_FbxResName, m_DataDir, m_FbxFileName);
			auto PtrFbxMesh = App::GetApp()->RegisterFbxMesh(m_FbxMeshResName, PtrFbxScene, m_MeshIndex);

			//���f���̑��΃X�P�[��
			//���f�����̂͑傫���̂ŁA�X�e�[�W�ɍ��킹�ď���������
			Matrix4X4 CharaScale;
			CharaScale.DefTransformation(
				Vector3(Scale, Scale, Scale),
				Vector3(0.0f, 0, 0.0f),
				Vector3(0.0f, 0.0f, 0.0f)
				);

			if (PtrFbxMesh->IsSkining()){
				RemoveComponent<BasicFbxPNTDraw>();
				PtrFbxMesh->AddAnimation("start", 0, 1, true, 30);
				auto PtrDraw = AddComponent<BasicFbxPNTBoneDraw>();
				PtrDraw->SetMeshToTransform(CharaScale);
				PtrDraw->SetFbxMeshResource(PtrFbxMesh);
				PtrDraw->SetCurrentAnimation("start");
				PtrDraw->SetTextureOnlyNoLight(true);
				App::GetApp()->GetScene<Scene>()->GetView()->SetSkinedActive(TRUE);
			}
			else{
				RemoveComponent<BasicFbxPNTBoneDraw>();
				auto PtrDraw = AddComponent<BasicFbxPNTDraw>();
				PtrDraw->SetMeshToTransform(CharaScale);
				PtrDraw->SetFbxMeshResource(PtrFbxMesh);
				PtrDraw->SetTextureOnlyNoLight(true);
				App::GetApp()->GetScene<Scene>()->GetView()->SetSkinedActive(FALSE);
			}




		}
		catch (...){
			throw;
		}
	}


	void FbxMeshObject::ReadMesh(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
		float Scale){
		//���\�[�X�����o��
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);

		auto NumVirtex = MeshRes->GetFbxMesh()->GetControlPointsCount();
		vertices.resize(NumVirtex);

		//�|���S�����̎擾
		auto dwNumPolygons = (DWORD)MeshRes->GetFbxMesh()->GetPolygonCount();

		FbxStringList sUVSetNames;
		MeshRes->GetFbxMesh()->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = MeshRes->GetFbxMesh()->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = MeshRes->GetFbxMesh()->GetControlPointAt(iIndex);
				//�@���𓾂�
				MeshRes->GetFbxMesh()->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				MeshRes->GetFbxMesh()->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
					//���_�̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vPos[0] * Scale), static_cast< float >(vPos[1] * Scale), -static_cast< float >(vPos[2] * Scale)),
					//�@���̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV�l�̐ݒ�
					//V�̒l���A1.0����������l�ɂȂ�
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);
			}
		}
		//�}�e���A���̓ǂݍ���
		for (auto& M : MeshRes->GetMaterialVec()){
			//�Q�Ƃ�����
			MaterialEx ToM = {0};
			ToM.m_Ambient = M.m_Ambient;
			ToM.m_Diffuse = M.m_Diffuse;
			ToM.m_Emissive = M.m_Emissive;
			ToM.m_Specular = M.m_Specular;
			ToM.m_Texture = M.m_Texture;
			ToM.m_StartIndex = 0;
			ToM.m_IndexCount = 0;
			materials.push_back(ToM);
		}


		//�}�e���A���̐����擾����(��̃��b�V���ɑ΂��A�����̃}�e���A�������蓖�Ă��Ă��邱�Ƃ�����)
		auto MaterialCount = materials.size();
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = MeshRes->GetFbxMesh()->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(MeshRes->GetFbxMesh()->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}
	}

	void FbxMeshObject::SaveStaticMeshBinFile(const shared_ptr<FbxMeshObjSaveData>& Data){
		try{
			string header;
			Util::WStoMB(Data->m_Header, header);
			if (header.size() < 16){
				header.resize(16, '\0');
			}

			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			ReadMesh(vertices, indices, materials, Data->m_Scale);

			wstring filename = Data->m_DataDir + Data->m_BinFileName;

			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::Vertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTexture);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials){
				wstring TextureFileName = mat.m_Texture->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//���W���[��������A�e�u���b�N�ɕ�����
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));
			ofs.close();

		}
		catch (...){
			throw;
		}
	}

	void FbxMeshObject::ReadSkinMesh(vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices,
		vector<MaterialEx>& materials, vector<Bone>& bones, vector<Matrix4X4>& animematrix, float Scale,
		const shared_ptr<FbxMeshObjSaveData>& Dat){
		//���\�[�X�����o��
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);

		auto NumVirtex = MeshRes->GetFbxMesh()->GetControlPointsCount();
		vertices.resize(NumVirtex);

		//�|���S�����̎擾
		auto dwNumPolygons = (UINT)MeshRes->GetFbxMesh()->GetPolygonCount();

		//���_���쐬���邽�߂̔z��

		FbxStringList sUVSetNames;
		MeshRes->GetFbxMesh()->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = MeshRes->GetFbxMesh()->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = MeshRes->GetFbxMesh()->GetControlPointAt(iIndex);
				//�@���𓾂�
				MeshRes->GetFbxMesh()->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				MeshRes->GetFbxMesh()->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
					//���_�̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vPos[0] * Scale), static_cast< float >(vPos[1] * Scale), -static_cast< float >(vPos[2] * Scale)),
					//�@���̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV�l�̐ݒ�
					//V�̒l���A1.0����������l�ɂȂ�
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
					//�u�����h�C���f�b�N�X�͂Ƃ肠����0
					temp,
					//�u�����h�E�G�C�g�͂Ƃ肠����0
					tempf
					);
			}
		}

		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X�̓ǂݍ���
		const int	iNumCluster = MeshRes->GetFbxSkin()->GetClusterCount();
		// �ϊ����� FbxSkin ���� �N���X�^�[(�{�[��)�̐����擾����

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointIndicesCount();	// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̐����擾����
			int*	piBlendIndices = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointIndices();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̔ԍ���z��Ŏ擾����
			double*	pdBlendWeights = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointWeights();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�ɑΉ������d�݃f�[�^��z��Ŏ擾����

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//���_�ɓo�^����S�̃u�����h�E�F�C�g�̂����A�ŏ��̒l�����v�f����������iDirectX9�̌Œ�V�F�[�_�ł͂S�̃{�[�����炵���u�����h�ł��Ȃ��j
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//���łɓo�^����Ă��钆�ōŏ��̃u�����h�E�F�C�g�����傫���l�����f�[�^��o�^����
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//�E�G�C�g�̃`�F�b�N
		//�e���_���ƂɃE�F�C�g�𑫂���1.0f�ɂȂ�Ȃ��ƃX�L�������܂��ł��Ȃ�
		for (size_t i = 0; i < vertices.size(); i++){
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f){
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//��^�C�}�[�̐ݒ�
		FbxGlobalSettings&	globalTimeSettings = MeshRes->GetFbxMesh()->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		FbxTime timePeriod;
		timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//�}�e���A���̓ǂݍ���
		for (auto& M : MeshRes->GetMaterialVec()){
			//�Q�Ƃ�����
			MaterialEx ToM = { 0 };
			ToM.m_Ambient = M.m_Ambient;
			ToM.m_Diffuse = M.m_Diffuse;
			ToM.m_Emissive = M.m_Emissive;
			ToM.m_Specular = M.m_Specular;
			ToM.m_Texture = M.m_Texture;
			ToM.m_StartIndex = 0;
			ToM.m_IndexCount = 0;
			materials.push_back(ToM);
		}


		//�}�e���A���̐����擾����(��̃��b�V���ɑ΂��A�����̃}�e���A�������蓖�Ă��Ă��邱�Ƃ�����)
		auto MaterialCount = materials.size();
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = MeshRes->GetFbxMesh()->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(MeshRes->GetFbxMesh()->GetPolygonVertex(j, 2 - k)));
						materials[i].m_IndexCount++;
					}
				}
			}
		}
		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}


		//�{�[�����𓾂�
		auto NumBones = (UINT)MeshRes->GetFbxSkin()->GetClusterCount();
		for (UINT i = 0; i < NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			MeshRes->GetFbxSkin()->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = MeshRes->GetFbxSkin()->GetCluster(i)->GetLink()->EvaluateGlobalTransform(timePeriod * 0);

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast< float >(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}

			Matrix4X4	mMirror, mBindInverse;
			mMirror.Identity();
			mMirror(2, 2) = -1.0f;

			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			Vector4 temp4;
			mBindInverse = Matrix4X4EX::Inverse(&temp4, bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;
			bones.push_back(bone);
		}

		//�A�j���f�[�^�𓾂�
		MeshRes->AddAnimation("run", Dat->m_StartFrame, Dat->m_FrameCount, true, Dat->m_FrameParSecond);



		AnimationData animData = MeshRes->GetAnimationData("run");
		//�{�[�������݂̎��ԂɍX�V����
		vector<Bone> AnimeBones;
		AnimeBones.resize(NumBones);
		AnimeBones = bones;

		float start = Dat->m_StartFrame / Dat->m_FrameParSecond;
		float end = Dat->m_StartFrame / Dat->m_FrameParSecond
			+ Dat->m_FrameCount / Dat->m_FrameParSecond;

		for (float f = start; f < end; f += Dat->m_SampleSpan){
			MeshRes->GenerateCurrentPose(AnimeBones, animData, f);
			for (auto b : AnimeBones){
				Matrix4X4 Mat = b.m_ConbinedPose;
				Mat._41 *= Scale;
				Mat._42 *= Scale;
				Mat._43 *= Scale;
				animematrix.push_back(Mat);
			}
		}


	}


	void FbxMeshObject::SaveSkinedMeshBinFile(const shared_ptr<FbxMeshObjSaveData>& Data){
		try{
			string header;
			Util::WStoMB(Data->m_Header, header);
			if (header.size() < 16){
				header.resize(16, '\0');
			}

			vector<VertexPositionNormalTextureSkinning> vertices;
			vector<uint16_t> indices;
			vector<MaterialEx> materials;
			vector<Bone> bones;
			vector<Matrix4X4> animematrix;
			ReadSkinMesh(vertices, indices, materials, bones, animematrix,Data->m_Scale,
				Data);

			wstring filename = Data->m_DataDir + Data->m_BinFileName;

			ofstream ofs(filename, ios::out | ios::binary);
			ofs.write(header.c_str(), 16);
			//���_�̕ۑ�
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::SkinedVertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTextureSkinning);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//�C���f�b�N�X�̕ۑ�
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//�}�e���A���̕ۑ�
			//�}�e���A�����̃w�b�_�̕ۑ�
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//�}�e���A���{�̂̕ۑ�
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];
			BlockHeader MaterialHeader;
			MaterialHeader.m_Type = BlockType::Material;
			for (auto mat : materials){
				wstring TextureFileName = mat.m_Texture->GetTextureFileName();
				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));
				//���W���[��������A�e�u���b�N�ɕ�����
				_wsplitpath_s(TextureFileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);
				TextureFileName = FileNamebuff;
				TextureFileName += Extbuff;

				SaveMaterialEx SaveMat;
				SaveMat.m_StartIndex = mat.m_StartIndex;
				SaveMat.m_IndexCount = mat.m_IndexCount;
				SaveMat.m_Diffuse = mat.m_Diffuse;
				SaveMat.m_Specular = mat.m_Specular;
				SaveMat.m_Ambient = mat.m_Ambient;
				SaveMat.m_Emissive = mat.m_Emissive;
				UINT TextureStrSize = (TextureFileName.size() + 1) * sizeof(wchar_t);
				MaterialHeader.m_Size = sizeof(SaveMaterialEx) + TextureStrSize;
				ofs.write((const char*)&MaterialHeader, sizeof(BlockHeader));
				ofs.write((const char*)&SaveMat, sizeof(SaveMaterialEx));
				ofs.write((const char*)TextureFileName.c_str(), TextureStrSize);

			}
			//�{�[���J�E���g�i�w�b�_�̂݁j
			BlockHeader BoneCountHeader;
			BoneCountHeader.m_Type = BlockType::BoneCount;
			//�{�[���J�E���g�̏ꍇm_Size�̓{�[�����ɂȂ�
			BoneCountHeader.m_Size = (UINT)bones.size();
			ofs.write((const char*)&BoneCountHeader, sizeof(BlockHeader));

			//�A�j���[�V�����s��
			BlockHeader AnimeMatrixHeader;
			AnimeMatrixHeader.m_Type = BlockType::AnimeMatrix;
			AnimeMatrixHeader.m_Size = (UINT)animematrix.size() * sizeof(Matrix4X4);
			ofs.write((const char*)&AnimeMatrixHeader, sizeof(BlockHeader));
			ofs.write((const char*)&animematrix.front(), AnimeMatrixHeader.m_Size);

			//End(�w�b�_�̂�)
			BlockHeader EndHeader;
			EndHeader.m_Type = BlockType::End;
			EndHeader.m_Size = 0;
			ofs.write((const char*)&EndHeader, sizeof(BlockHeader));


			ofs.close();
		}
		catch (...){
			throw;
		}
	}


	void FbxMeshObject::SaveBinFile(const shared_ptr<FbxMeshObjSaveData>& Data){
		SaveStaticMeshBinFile(Data);
	}

	void FbxMeshObject::SaveSkinBinFile(const shared_ptr<FbxMeshObjSaveData>& Data){
		//���\�[�X�����o��
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);
		if (MeshRes->IsSkining()){
			SaveSkinedMeshBinFile(Data);
		}
	}

	void FbxMeshObject::RunSkin(const shared_ptr<FbxMeshObjSaveData>& Data){
		//���\�[�X�����o��
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);
		if (MeshRes->IsSkining()){
			MeshRes->AddAnimation("run", Data->m_StartFrame, Data->m_FrameCount, true, Data->m_FrameParSecond);
			auto PtrDraw = GetComponent<BasicFbxPNTBoneDraw>();
			PtrDraw->SetCurrentAnimation("run");
		}
	}



	//�C�x���g�̃n���h��
	void FbxMeshObject::OnEvent(const shared_ptr<Event>& event){
		//�n���h���֐��Ăяo��
		//����ŃC�x���g���U�蕪������
		m_EventMachine->HandleEvent(event);
	}


	//--------------------------------------------------------------------------------------
	//	class DataResetEvent : public EventState<FbxMeshObject>;
	//	�p�r: �f�[�^�������ւ���ꂽ�C�x���g
	//--------------------------------------------------------------------------------------
	shared_ptr<DataResetEvent> DataResetEvent::Instance(){
		static shared_ptr<DataResetEvent> instance;
		if (!instance){
			instance = shared_ptr<DataResetEvent>(new DataResetEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void DataResetEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjData>(event->m_Info);
		Obj->ResetFbxMesh(Data->m_DataDir,Data->m_FbxFileName,Data->m_MeshIndex,Data->m_Scale);
	}

	//--------------------------------------------------------------------------------------
	//	class DataSaveEvent : public EventState<FbxMeshObject>;
	//	�p�r: �f�[�^�ۑ��̃C�x���g
	//--------------------------------------------------------------------------------------
	shared_ptr<DataSaveEvent> DataSaveEvent::Instance(){
		static shared_ptr<DataSaveEvent> instance;
		if (!instance){
			instance = shared_ptr<DataSaveEvent>(new DataSaveEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void DataSaveEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->SaveBinFile(Data);
	}

	//--------------------------------------------------------------------------------------
	//	class DataSaveSkinEvent : public EventState<FbxMeshObject>;
	//	�p�r: �X�L���f�[�^�ۑ��̃C�x���g
	//--------------------------------------------------------------------------------------
	shared_ptr<DataSaveSkinEvent> DataSaveSkinEvent::Instance(){
		static shared_ptr<DataSaveSkinEvent> instance;
		if (!instance){
			instance = shared_ptr<DataSaveSkinEvent>(new DataSaveSkinEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void DataSaveSkinEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->SaveSkinBinFile(Data);
	}


	//--------------------------------------------------------------------------------------
	//	class RunSkinEvent : public EventState<FbxMeshObject>;
	//	�p�r: �X�L�����s�̃C�x���g
	//--------------------------------------------------------------------------------------
	shared_ptr<RunSkinEvent> RunSkinEvent::Instance(){
		static shared_ptr<RunSkinEvent> instance;
		if (!instance){
			instance = shared_ptr<RunSkinEvent>(new RunSkinEvent);
		}
		return instance;
	}
	//���̃C�x���g�����������Ƃ��ɌĂ΂��
	void RunSkinEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->RunSkin(Data);
	}






}
//endof  basedx11
