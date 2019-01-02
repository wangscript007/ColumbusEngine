#pragma once

#include <Math/Matrix.h>
#include <Math/Vector3.h>

namespace Columbus
{

	class Camera
	{
	private:
		Vector3 CameraDirection = Vector3(0, 0, -1);
		Vector3 CameraRight = Vector3(1, 0, 0);
		Vector3 CameraUp = Vector3(0, 1, 0);

		Matrix ProjectionMatrix;
		Matrix ViewMatrix;
		Matrix ViewProjection;
		
		bool PreTargeted = false;
	public:
		Vector3 Pos = Vector3(0, 0, 5);
		Vector3 Rot = Vector3(0, 0, 0);
		Vector3 Target = Vector3(0, 0, 4);
	public:
		Camera();
		
		void Update();
		
		Vector3 Direction() const;
		Vector3 Right() const;
		Vector3 Up() const;

		void Perspective(float FOV, float Aspect, float Near, float Far);
		//void ortho();

		const Matrix& GetViewProjection() const;
		const Matrix& GetProjectionMatrix() const;
		const Matrix& GetViewMatrix() const;
		
		~Camera();
	};

}
