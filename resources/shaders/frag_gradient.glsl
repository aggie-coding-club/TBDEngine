#version 450 core

const float PI = 3.1415;

out vec4 gl_FragColor;

uniform vec2 _ScreenParams;
uniform vec3 _WorldSpaceCamPos;
uniform vec3 viewParams;
uniform mat4x4 camLocalToWorldMatrix;

struct Ray
{
	vec3 origin;
	vec3 dir;
	vec3 invDir;
};

struct HitInfo
{
	bool didHit;
	float dst;
	vec3 hitPoint;
	vec3 normal;
};

// Hit Info Struct
struct TriangleHitInfo
{
	bool didHit;                        // Did the ray hit?
	float dst;                          // distance the ray had to travel
	vec3 hitPoint;                    // The point that the ray hit the object
	vec3 normal;                      // The normal of the object at that point
	vec2 uv;                     // Location of hit on UV
	int triIndex;
};

struct Triangle
{
	vec3 posA, posB, posC;
	vec3 normalA, normalB, normalC;
	vec2 uvA, uvB, uvC;
};

struct Material
{
	int useTexture;
	int textureIndex;
	int normalIndex;
	vec4 color;
	vec4 emissionColor;
	float emissionStrength;
	vec4 specularColor;
	float specularProbability;
	float smoothness;
	float refractIndx;
	float transparency;
};

struct Model
{
	int nodeOffset;
	int triOffset;
	mat4x4 worldToLocalMatrix;
	mat4x4 localToWorldMatrix;
	Material material;
};

struct BVHNode
{
	vec3 boundsMin;
	vec3 boundsMax;
	int startIndex;
	int triangleCount;
};

struct ModelHitInfo
{
	bool didHit;
	vec3 normal;
	vec3 hitPoint;
	vec2 uv;
	float dst;
	Material material;
};

struct Light
{
	vec4 emissionColor;
	float emissionStrength;
	vec3 modelPosition;
	int modelIndex;
};

//buffer Model modelInfo;
layout(std430, binding = 1) buffer TriangleBuffer {
	Triangle triangles[];
};
//buffer BVHNode Nodes;
//buffer Light lights;

int triangleCount;
int modelCount;
int lightCount;

//float RandomValue(inout uint state)
//{
//	state = state * uint(747796405) + uint(2891336453);
//	uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
//	result = (result >> 22) ^ result;
//	return result / 4294967295.0;
//}
//
//vec2 RandomPointInCircle(inout uint rngState)
//{
//	float angle = RandomValue(rngState) * 2 * PI;
//	float2 pointOnCircle = float2(cos(angle), sin(angle));
//	return pointOnCircle * sqrt(RandomValue(rngState));
//}
//
//float RandomValueNormalDist(inout uint state)
//{
//	float t = 2 * 3.14159263 * RandomValue(state);
//	float r = sqrt(-2 * log(RandomValue(state)));
//	return r * cos(t);
//}
//
//vec3 RandomDirection(inout uint state)
//{
//	float x = RandomValueNormalDist(state);
//	float y = RandomValueNormalDist(state);
//	float z = RandomValueNormalDist(state);
//	return normalize(float3(x,y,z));
//}
//
//vec3 RandomHemiDir(vec3 normal, inout uint rngState)
//{
//	float3 dir = RandomDirection(rngState);
//	return dir * sign(dot(normal, dir));
//}

//ModelHitInfo CalculateRayCollission(Ray worldRay)
//{
//	ModelHitInfo result;
//	result.dst = pos_infinity;
//	Ray localRay;
//
//	for(int i = 0; i < triangleCount; i++)
//	{
//
//	}
//	return result;
//}
//
//vec3 Trace(vec3 rayOrigin, vec3 rayDir, inout uint rngState, inout bool hitObj)
//{
//	vec3 incominglight = vec3(0);
//	vec3 rayColor = vec3(1);
//
//	Ray ray;
//	ray.dir = rayDir;
//	ray.orgin = rayOrigin;
//	ModelHitInfo hitInfo = CalculateRayCollission(ray);
//	return hitInfo.normal;
//}

TriangleHitInfo RayTriangle(Ray ray, Triangle tri) {
	vec3 edgeAB = tri.posB - tri.posA;
	vec3 edgeAC = tri.posC - tri.posA;
	vec3 normalVector = cross(edgeAB, edgeAC);
	vec3 ao = ray.origin - tri.posA;
	vec3 dao = cross(ao, ray.dir);

	float determinant = -dot(ray.dir, normalVector);
	float invDet = 1 / determinant;

	// Calculate dst to Triangle and barycentric coordinates of intersection point
	float dst = dot(ao, normalVector) * invDet;
	float u = dot(edgeAC, dao) * invDet;
	float v = -dot(edgeAB, dao) * invDet;
	float w = 1 - u - v;

	// Initialize hit info
	TriangleHitInfo hitInfo;
	hitInfo.didHit = determinant >= 1E-6 && dst >= -1E-6 && u >= -1E-6 && v >= -1E-6 && w >= -1E-6;
	if (!hitInfo.didHit) return hitInfo;

	hitInfo.hitPoint = ray.origin + ray.dir * dst;
	hitInfo.dst = dst;

	hitInfo.normal = normalize(tri.normalA * w + tri.normalB * u + tri.normalC * v);

	return hitInfo;
}

HitInfo RaySphere(Ray ray, vec3 sphereCenter, float sphereRadius)
{
	HitInfo hitInfo = HitInfo(false, 0, vec3(0), vec3(0));
	vec3 offsetRayOrigin = ray.origin - sphereCenter;

	// Quadratic equation coefficients for ray-sphere intersection
	float a = dot(ray.dir, ray.dir);
	float b = 2.0 * dot(offsetRayOrigin, ray.dir);
	float c = dot(offsetRayOrigin, offsetRayOrigin) - sphereRadius * sphereRadius;

	float discriminant = b * b - 4.0 * a * c;

	if(discriminant < 0)
	{
		return hitInfo;
	}

	float sqrtD = sqrt(discriminant);
	float dst1 = (-b - sqrtD) / (2.0 * a);  // First root (closest intersection)
	float dst2 = (-b + sqrtD) / (2.0 * a);  // Second root (farther intersection)

	// Select the correct intersection point
	float epsilon = 1e-4; // Small bias to avoid floating-point precision issues

	if (dst1 >= epsilon) {
		hitInfo.didHit = true;
		hitInfo.dst = dst1;
	} else if (dst2 >= epsilon) { // Check second root in case ray starts inside the sphere
	  	hitInfo.didHit = true;
	  	hitInfo.dst = dst2;
	} else {
		return hitInfo; // No valid hit
	}

	// Compute hit point and normal
	hitInfo.hitPoint = ray.origin + ray.dir * hitInfo.dst;
	hitInfo.normal = normalize(hitInfo.hitPoint - sphereCenter);

	return hitInfo;
}


void main()
{
	vec2 ndc = vec2(
	(gl_FragCoord.x / _ScreenParams.x) * 2.0 - 1.0,
	-(gl_FragCoord.y / _ScreenParams.y) * 2.0 + 1.0 // Flip Y
	);

	vec3 viewPointLocal = vec3(ndc * viewParams.xy, 1.0);
	vec3 viewPoint = (camLocalToWorldMatrix * vec4(viewPointLocal, 1.0)).xyz;

	vec3 rayOrigin = _WorldSpaceCamPos;
	vec3 rayDir = normalize(viewPoint - rayOrigin);


	Ray ray;
	ray.dir = rayDir;
	ray.origin = rayOrigin;
	ray.invDir = 1.0 / ray.dir;

	Triangle tri;
	tri.posA = vec3(0,3,4);
	tri.posB = vec3(4,0,3);
	tri.posC = vec3(3,4,0);

	float sq3 = 1/sqrt(3);
	tri.normalA = vec3(sq3);
	tri.normalB = vec3(sq3);
	tri.normalC = vec3(sq3);


	gl_FragColor = vec4(RayTriangle(ray, tri).normal, 1);
}

