#include "stdafx.h"
#include "Fbx2BinDoc.h"
#include "Fbx2BinView.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	用途: FBXメッシュのオブジェクト
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
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.0f, 0);
		//イベントマシンの構築
		m_EventMachine = make_shared< EventMachine<FbxMeshObject>>(GetThis<FbxMeshObject>());
		//イベントキーとイベントステートを結び付ける
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

			//モデルの相対スケール
			//モデル自体は大きいので、ステージに合わせて小さくする
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
		//リソースを取り出す
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);

		auto NumVirtex = MeshRes->GetFbxMesh()->GetControlPointsCount();
		vertices.resize(NumVirtex);

		//ポリゴン数の取得
		auto dwNumPolygons = (DWORD)MeshRes->GetFbxMesh()->GetPolygonCount();

		FbxStringList sUVSetNames;
		MeshRes->GetFbxMesh()->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = MeshRes->GetFbxMesh()->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = MeshRes->GetFbxMesh()->GetControlPointAt(iIndex);
				//法線を得る
				MeshRes->GetFbxMesh()->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				MeshRes->GetFbxMesh()->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
					//頂点の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vPos[0] * Scale), static_cast< float >(vPos[1] * Scale), -static_cast< float >(vPos[2] * Scale)),
					//法線の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV値の設定
					//Vの値が、1.0から引いた値になる
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);
			}
		}
		//マテリアルの読み込み
		for (auto& M : MeshRes->GetMaterialVec()){
			//参照から代入
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


		//マテリアルの数を取得する(一つのメッシュに対し、複数のマテリアルが割り当てられていることがある)
		auto MaterialCount = materials.size();
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = MeshRes->GetFbxMesh()->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
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
		//マテリアル配列にスタート地点を設定
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
			//頂点の保存
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::Vertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTexture);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//インデックスの保存
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//マテリアルの保存
			//マテリアル数のヘッダの保存
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//マテリアル本体の保存
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
				//モジュール名から、各ブロックに分ける
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
			//End(ヘッダのみ)
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
		//リソースを取り出す
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);

		auto NumVirtex = MeshRes->GetFbxMesh()->GetControlPointsCount();
		vertices.resize(NumVirtex);

		//ポリゴン数の取得
		auto dwNumPolygons = (UINT)MeshRes->GetFbxMesh()->GetPolygonCount();

		//頂点を作成するための配列

		FbxStringList sUVSetNames;
		MeshRes->GetFbxMesh()->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = MeshRes->GetFbxMesh()->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = MeshRes->GetFbxMesh()->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = MeshRes->GetFbxMesh()->GetControlPointAt(iIndex);
				//法線を得る
				MeshRes->GetFbxMesh()->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				MeshRes->GetFbxMesh()->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
					//頂点の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vPos[0] * Scale), static_cast< float >(vPos[1] * Scale), -static_cast< float >(vPos[2] * Scale)),
					//法線の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV値の設定
					//Vの値が、1.0から引いた値になる
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
					//ブレンドインデックスはとりあえず0
					temp,
					//ブレンドウエイトはとりあえず0
					tempf
					);
			}
		}

		//ブレンドウェイトとブレンドインデックスの読み込み
		const int	iNumCluster = MeshRes->GetFbxSkin()->GetClusterCount();
		// 変換した FbxSkin から クラスター(ボーン)の数を取得する

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointIndicesCount();	// i 番目のクラスターに影響を受ける頂点の数を取得する
			int*	piBlendIndices = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointIndices();		// i 番目のクラスターに影響を受ける頂点の番号を配列で取得する
			double*	pdBlendWeights = MeshRes->GetFbxSkin()->GetCluster(i)->GetControlPointWeights();		// i 番目のクラスターに影響を受ける頂点に対応した重みデータを配列で取得する

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//頂点に登録する４つのブレンドウェイトのうち、最少の値をもつ要素を検索する（DirectX9の固定シェーダでは４つのボーンからしかブレンドできない）
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
				//すでに登録されている中で最小のブレンドウェイトよりも大きい値を持つデータを登録する
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//ウエイトのチェック
		//各頂点ごとにウェイトを足して1.0fにならないとスキンがうまくできない
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

		//基準タイマーの設定
		FbxGlobalSettings&	globalTimeSettings = MeshRes->GetFbxMesh()->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		FbxTime timePeriod;
		timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//マテリアルの読み込み
		for (auto& M : MeshRes->GetMaterialVec()){
			//参照から代入
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


		//マテリアルの数を取得する(一つのメッシュに対し、複数のマテリアルが割り当てられていることがある)
		auto MaterialCount = materials.size();
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = MeshRes->GetFbxMesh()->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
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
		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < MaterialCount; i++) {
			materials[i].m_StartIndex = StarIndex;
			StarIndex += materials[i].m_IndexCount;
		}


		//ボーン数を得る
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

		//アニメデータを得る
		MeshRes->AddAnimation("run", Dat->m_StartFrame, Dat->m_FrameCount, true, Dat->m_FrameParSecond);



		AnimationData animData = MeshRes->GetAnimationData("run");
		//ボーンを現在の時間に更新する
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
			//頂点の保存
			BlockHeader VerTexHeader;
			VerTexHeader.m_Type = BlockType::SkinedVertex;
			VerTexHeader.m_Size = (UINT)vertices.size() * sizeof(VertexPositionNormalTextureSkinning);
			ofs.write((const char*)&VerTexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&vertices.front(), VerTexHeader.m_Size);
			//インデックスの保存
			BlockHeader IndexHeader;
			IndexHeader.m_Type = BlockType::Index;
			IndexHeader.m_Size = (UINT)indices.size() * sizeof(uint16_t);
			ofs.write((const char*)&IndexHeader, sizeof(BlockHeader));
			ofs.write((const char*)&indices.front(), IndexHeader.m_Size);
			//マテリアルの保存
			//マテリアル数のヘッダの保存
			BlockHeader MaterialCountHeader;
			MaterialCountHeader.m_Type = BlockType::MaterialCount;
			MaterialCountHeader.m_Size = (UINT)materials.size();
			ofs.write((const char*)&MaterialCountHeader, sizeof(BlockHeader));
			//マテリアル本体の保存
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
				//モジュール名から、各ブロックに分ける
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
			//ボーンカウント（ヘッダのみ）
			BlockHeader BoneCountHeader;
			BoneCountHeader.m_Type = BlockType::BoneCount;
			//ボーンカウントの場合m_Sizeはボーン数になる
			BoneCountHeader.m_Size = (UINT)bones.size();
			ofs.write((const char*)&BoneCountHeader, sizeof(BlockHeader));

			//アニメーション行列
			BlockHeader AnimeMatrixHeader;
			AnimeMatrixHeader.m_Type = BlockType::AnimeMatrix;
			AnimeMatrixHeader.m_Size = (UINT)animematrix.size() * sizeof(Matrix4X4);
			ofs.write((const char*)&AnimeMatrixHeader, sizeof(BlockHeader));
			ofs.write((const char*)&animematrix.front(), AnimeMatrixHeader.m_Size);

			//End(ヘッダのみ)
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
		//リソースを取り出す
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);
		if (MeshRes->IsSkining()){
			SaveSkinedMeshBinFile(Data);
		}
	}

	void FbxMeshObject::RunSkin(const shared_ptr<FbxMeshObjSaveData>& Data){
		//リソースを取り出す
		auto MeshRes = App::GetApp()->GetResource<FbxMeshResource>(m_FbxMeshResName);
		if (MeshRes->IsSkining()){
			MeshRes->AddAnimation("run", Data->m_StartFrame, Data->m_FrameCount, true, Data->m_FrameParSecond);
			auto PtrDraw = GetComponent<BasicFbxPNTBoneDraw>();
			PtrDraw->SetCurrentAnimation("run");
		}
	}



	//イベントのハンドラ
	void FbxMeshObject::OnEvent(const shared_ptr<Event>& event){
		//ハンドラ関数呼び出し
		//これでイベントが振り分けられる
		m_EventMachine->HandleEvent(event);
	}


	//--------------------------------------------------------------------------------------
	//	class DataResetEvent : public EventState<FbxMeshObject>;
	//	用途: データが差し替えられたイベント
	//--------------------------------------------------------------------------------------
	shared_ptr<DataResetEvent> DataResetEvent::Instance(){
		static shared_ptr<DataResetEvent> instance;
		if (!instance){
			instance = shared_ptr<DataResetEvent>(new DataResetEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void DataResetEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjData>(event->m_Info);
		Obj->ResetFbxMesh(Data->m_DataDir,Data->m_FbxFileName,Data->m_MeshIndex,Data->m_Scale);
	}

	//--------------------------------------------------------------------------------------
	//	class DataSaveEvent : public EventState<FbxMeshObject>;
	//	用途: データ保存のイベント
	//--------------------------------------------------------------------------------------
	shared_ptr<DataSaveEvent> DataSaveEvent::Instance(){
		static shared_ptr<DataSaveEvent> instance;
		if (!instance){
			instance = shared_ptr<DataSaveEvent>(new DataSaveEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void DataSaveEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->SaveBinFile(Data);
	}

	//--------------------------------------------------------------------------------------
	//	class DataSaveSkinEvent : public EventState<FbxMeshObject>;
	//	用途: スキンデータ保存のイベント
	//--------------------------------------------------------------------------------------
	shared_ptr<DataSaveSkinEvent> DataSaveSkinEvent::Instance(){
		static shared_ptr<DataSaveSkinEvent> instance;
		if (!instance){
			instance = shared_ptr<DataSaveSkinEvent>(new DataSaveSkinEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void DataSaveSkinEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->SaveSkinBinFile(Data);
	}


	//--------------------------------------------------------------------------------------
	//	class RunSkinEvent : public EventState<FbxMeshObject>;
	//	用途: スキン実行のイベント
	//--------------------------------------------------------------------------------------
	shared_ptr<RunSkinEvent> RunSkinEvent::Instance(){
		static shared_ptr<RunSkinEvent> instance;
		if (!instance){
			instance = shared_ptr<RunSkinEvent>(new RunSkinEvent);
		}
		return instance;
	}
	//このイベントが発生したときに呼ばれる
	void RunSkinEvent::Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event){
		auto Data = VoidToShared<FbxMeshObjSaveData>(event->m_Info);
		Obj->RunSkin(Data);
	}






}
//endof  basedx11
