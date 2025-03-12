#version 450 core

const float PI = 3.1415;

layout(location = 0) out vec4 fragColor;


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
	int textureIndex;
	int normalIndex;
	float emissionStrength;
	float specularProbability;
	float smoothness;
	float refractIndx;
	float transparency;
	vec4 color;
	vec4 emissionColor;
	vec4 specularColor;
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

layout(std430, binding = 0) buffer ModelBuffer {
	Model models[];
};

layout(std430, binding = 1) buffer TriangleBuffer {
	Triangle triangles[];
};

layout(std430, binding = 2) buffer NodeBuffer {
	BVHNode Nodes[];
};

layout(std430, binding = 3) buffer LightBuffer {
	Light lights[];
};

int triangleCount;
int modelCount;
int lightCount;

float RandomValue(inout uint state)
{
	state = state * uint(747796405) + uint(2891336453);
	uint result = ((state >> ((state >> 28) + 4)) ^ state) * 277803737;
	result = (result >> 22) ^ result;
	return result / 4294967295.0;
}

vec2 RandomPointInCircle(inout uint rngState)
{
	float angle = RandomValue(rngState) * 2 * PI;
	vec2 pointOnCircle = vec2(cos(angle), sin(angle));
	return pointOnCircle * sqrt(RandomValue(rngState));
}

float RandomValueNormalDist(inout uint state)
{
	float t = 2 * 3.14159263 * RandomValue(state);
	float r = sqrt(-2 * log(RandomValue(state)));
	return r * cos(t);
}

vec3 RandomDirection(inout uint state)
{
	float x = RandomValueNormalDist(state);
	float y = RandomValueNormalDist(state);
	float z = RandomValueNormalDist(state);
	return normalize(vec3(x,y,z));
}

vec3 RandomHemiDir(vec3 normal, inout uint rngState)
{
	vec3 dir = RandomDirection(rngState);
	return dir * sign(dot(normal, dir));
}

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

float RayBoundingBoxDst(Ray ray, vec3 boxMin, vec3 boxMax)
{
	vec3 tMin = (boxMin - ray.origin) * ray.invDir;
	vec3 tMax = (boxMax - ray.origin) * ray.invDir;
	vec3 t1 = min(tMin, tMax);
	vec3 t2 = max(tMin, tMax);
	float tNear = max(max(t1.x, t1.y), t1.z);
	float tFar = min(min(t2.x, t2.y), t2.z);

	bool hit = bool(tFar >= tNear) && bool(tFar > 0);
	float dst = hit ? (tNear > 0 ? tNear : 0) : float(uintBitsToFloat(0x7F800000));
return dst;
}

ModelHitInfo CalculateRayCollision(Ray worldRay)
{
	ModelHitInfo result;
	result.dst = uintBitsToFloat(0x7F800000);
	Ray localRay;

	for(int i = 0; i < modelCount; i++)
	{
		Model model = models[i];
		localRay.origin = (model.worldToLocalMatrix * vec4(worldRay.origin, 1)).xyz;
		localRay.dir = (model.worldToLocalMatrix * vec4(worldRay.dir, 1)).xyz;
		localRay.invDir = 1 / localRay.dir;

		TriangleHitInfo hit = RayTriangle(localRay, triangles[model.triOffset]);

		if(hit.dst < result.dst)
		{
			result.didHit = true;
			result.dst = hit.dst;
			result.normal = normalize((model.localToWorldMatrix * vec4(hit.normal, 0)).xyz);
			result.uv = hit.uv;
			result.hitPoint = worldRay.origin + worldRay.dir * hit.dst;
			result.material = model.material;
		}
	}

	return result;
}

ModelHitInfo Trace(vec3 rayOrgin, vec3 rayDir)
{
	Ray ray;
	ray.origin = rayOrgin;
	ray.dir = rayDir;

	return CalculateRayCollision(ray);
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

	fragColor = vec4(triangles[0].normalC, 1);
}