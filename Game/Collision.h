#pragma once
#include<SimpleMath.h>
class Sphere
{
public:
	DirectX::SimpleMath::Vector3 center;
	float radius;
	Sphere() { radius = 1.0f; }
};

class Segment
{
public:
	DirectX::SimpleMath::Vector3 start;
	DirectX::SimpleMath::Vector3 end;
};
class Capsule
{
public:
	Segment segment;
	float radius;
	Capsule()
	{
		segment.start = DirectX::SimpleMath::Vector3(0, 0, 0);
		segment.end = DirectX::SimpleMath::Vector3(0, 1, 0);
		radius = 1.0f;
	}
};
// 法線付き三角形（反時計回りが表面）
class Triangle
{
public:
	DirectX::SimpleMath::Vector3	P0;
	DirectX::SimpleMath::Vector3	P1;
	DirectX::SimpleMath::Vector3	P2;
	DirectX::SimpleMath::Vector3	Normal;	// 法線ベクトル
};

void ComputeTriangle(const DirectX::SimpleMath::Vector3& _p0, const DirectX::SimpleMath::Vector3& _p1, const DirectX::SimpleMath::Vector3& _p2, Triangle* _triangle);
bool CheckPoint2Triangle(const DirectX::SimpleMath::Vector3& _point, const Triangle& _triangle);
bool CheckSphere2Triangle(const Sphere& _sphere, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);
bool CheckSphere2Sphere(const Sphere&sphereA, const Sphere&sphereB, DirectX::SimpleMath::Vector3*inter );
bool CheckCapsule2Capsule(const Capsule&CapsuleA, const Capsule&CapsuleB, DirectX::SimpleMath::Vector3*inter );
bool CheckSphere2Capsule(const Sphere&sphereA, const Capsule&CapsuleB, DirectX::SimpleMath::Vector3*inter );
bool CheckSegment2Triangle(const Segment& _segment, const Triangle& _triangle, DirectX::SimpleMath::Vector3 *_inter);