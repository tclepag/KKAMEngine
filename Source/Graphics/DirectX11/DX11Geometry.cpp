#include "Graphics/DirectX11/DX11Geometry.h"
#include "Core/KEngine.h"

namespace Graphics {
	DX11Geometry::DX11Geometry(KEngine* KEngine, ID3D11Device* Device)
		: Device_(Device), IGeometry(KEngine) {
		VertexBuffer_ = std::make_unique<DX11VertexBuffer>(Device_);
		IndexBuffer_ = std::make_unique<DX11IndexBuffer>(Device_);
		Shader_ = std::make_unique<DX11Shader>(KEngine, Device_);
		Device_->GetImmediateContext(DeviceContext_.GetAddressOf());
	}
	DX11Geometry::~DX11Geometry() {
		Release();
	}
	void DX11Geometry::Create() {
		Shader_->Create();
		VertexBuffer_->Create();
		IndexBuffer_->Create();
	}
	void DX11Geometry::Rebake() {
		Create();
	}
	void DX11Geometry::Draw() {
		if (Shader_) {
			Shader_->SetTransformationMatrices(WorldMatrix_, ViewMatrix_, ProjectionMatrix_);
			Shader_->Bind(DeviceContext_.Get());
		}
		VertexBuffer_->Bind(DeviceContext_.Get());
		IndexBuffer_->Bind(DeviceContext_.Get());
		DeviceContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		DeviceContext_->DrawIndexed(IndexBuffer_->GetIndexCount(), 0, 0);
		IndexBuffer_->Unbind(DeviceContext_.Get());
		VertexBuffer_->Unbind(DeviceContext_.Get());
	}
	void DX11Geometry::Release() {
		VertexBuffer_->Release();
		IndexBuffer_->Release();
	}
	void DX11Geometry::SetData(VertexArray Vertices, Indices Indices) {
		VertexBuffer_->SetData(Vertices);
		IndexBuffer_->SetData(Indices);
	}

	void DX11Geometry::SetVertexPath(const std::string& path) {
		VertexPath_ = path;
		if (Shader_) {
			Shader_->SetVertexPath(path);
		}
	}

	void DX11Geometry::SetFragmentPath(const std::string& path) {
		FragmentPath_ = path;
		if (Shader_) {
			Shader_->SetFragmentPath(path);
		}
	}

	void DX11Geometry::SetTransformMatrices(const glm::mat4& world,
		const glm::mat4& view,
		const glm::mat4& projection) {
		WorldMatrix_ = world;
		ViewMatrix_ = view;
		ProjectionMatrix_ = projection;
	}

	void DX11Geometry::SetWorldMatrix(const glm::mat4& world) {
		WorldMatrix_ = world;
	}

	void DX11Geometry::SetViewMatrix(const glm::mat4& view) {
		ViewMatrix_ = view;
	}

	void DX11Geometry::SetProjectionMatrix(const glm::mat4& projection) {
		ProjectionMatrix_ = projection;
	}

}