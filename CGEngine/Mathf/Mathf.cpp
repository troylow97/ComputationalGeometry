#include "pch.h"
#include "Mathf.h"

bool Mathf::IsEqual(float lhs, float rhs) {
	return glm::epsilonEqual<float>(lhs, rhs, EPSILON);
}

float Mathf::Repeat(float t, float length) {
	return std::clamp<float>(t - floorf(t / length) * length, 0, length);
}

int Mathf::Wrap(int t, int min, int max) {
	if (t < min)
		return max;
	if (t > max)
		return min;
	
	return t;
}

float Mathf::PingPong(float t, float length) {
	t = Repeat(t, length * 2.0f);
	return length - abs(t - length);
}

float Mathf::Clamp(float val, float min, float max) {
	return std::clamp<float>(val, min, max);
}

std::vector<double> Mathf::ComputeGaussianKernel(double sigma, int N) {
	std::vector<double> results;
	results.reserve(N);

	const double total = std::erf((N + 0.5) / (std::sqrt(2.0) * sigma)) - std::erf((-N - 0.5) / (std::sqrt(2.0) * sigma));
	for (int i = 0; i < N; ++i)
		results.push_back((std::erf((i + 0.5) / (std::sqrt(2.0) * sigma)) - std::erf((i - 0.5) / (std::sqrt(2.0) * sigma))) / total);

	return results;
}

bool Mathf::IsComponentZero(const glm::vec3& val) {
	constexpr float smallVal = 0.001f;
	return !((glm::abs(val.x) > smallVal) && (glm::abs(val.y) > smallVal) && (glm::abs(val.x) > smallVal));
}

glm::vec3 Mathf::QuaternionToEuler(glm::quat quat) {
	glm::vec3 euler;

	// if the input quaternion is normalized, this is exactly one. Otherwise, this acts as a correction factor for the quaternion's not-normalizedness
	float unit = (quat.x * quat.x) + (quat.y * quat.y) + (quat.z * quat.z) + (quat.w * quat.w);

	// this will have a magnitude of 0.5 or greater if and only if this is a singularity case
	float test = quat.x * quat.w - quat.y * quat.z;

	if (test > 0.4995f * unit) // singularity at north pole
	{
		euler.x = PI / 2;
		euler.y = 2.f * atan2(quat.y, quat.x);
		euler.z = 0;
	}
	else if (test < -0.4995f * unit) // singularity at south pole
	{
		euler.x = -PI / 2;
		euler.y = -2.f * atan2(quat.y, quat.x);
		euler.z = 0;
	}
	else // no singularity - this is the majority of cases
	{
		euler.x = asin(2.f * (quat.w * quat.x - quat.y * quat.z));
		euler.y = atan2(2.f * quat.w * quat.y + 2.f * quat.z * quat.x, 1.f - 2.f * (quat.x * quat.x + quat.y * quat.y));
		euler.z = atan2(2.f * quat.w * quat.z + 2.f * quat.x * quat.y, 1.f - 2.f * (quat.z * quat.z + quat.x * quat.x));
	}

	// all the math so far has been done in radians. Before returning, we convert to degrees...
	euler = glm::degrees(euler);

	//...and then ensure the degree values are between 0 and 360
	euler.x = fmodf(euler.x, 360);
	euler.y = fmodf(euler.y, 360);
	euler.z = fmodf(euler.z, 360);

	return euler;
}

glm::vec3 Mathf::ExtractScale(const glm::mat4& matrix) {
	glm::vec3 scale;
	scale.x = glm::length(glm::vec3{ matrix[0][0] , matrix[0][1] , matrix[0][2] });
	scale.y = glm::length(glm::vec3{ matrix[1][0] , matrix[1][1] , matrix[1][2] });
	scale.z = glm::length(glm::vec3{ matrix[2][0] , matrix[2][1] , matrix[2][2] });
	return scale;
}

glm::quat Mathf::ExtractRotation(const glm::mat4& matrix) {
	glm::vec3 scale{};
	glm::quat rotation{};
	ExtractScaleRotation(matrix, &scale, &rotation);
	return rotation;
}

void Mathf::ExtractScaleRotation(const glm::mat4& matrix, glm::vec3* scale, glm::quat* rotation) {
	const glm::vec3 extractedScale = ExtractScale(matrix);

	if (scale)
		*scale = extractedScale;

	if (rotation)
	{
		glm::mat4 rot{ 1.0f };
		rot[0][0] = matrix[0][0] / extractedScale.x;
		rot[1][0] = matrix[1][0] / extractedScale.y;
		rot[2][0] = matrix[2][0] / extractedScale.z;

		rot[0][1] = matrix[0][1] / extractedScale.x;
		rot[1][1] = matrix[1][1] / extractedScale.y;
		rot[2][1] = matrix[2][1] / extractedScale.z;

		rot[0][2] = matrix[0][2] / extractedScale.x;
		rot[1][2] = matrix[1][2] / extractedScale.y;
		rot[2][2] = matrix[2][2] / extractedScale.z;

		*rotation = glm::quat_cast(rot);
	}
}

glm::mat4 Mathf::ComputeModelMatrix(glm::vec3 pos, glm::vec3 rotDegrees, glm::vec3 scale) {
	const glm::mat4 translation = glm::translate(glm::mat4{ 1.0f }, pos);
	const glm::mat4 rotation = translation * glm::toMat4(glm::quat(glm::radians(rotDegrees)));
	return glm::scale(rotation, scale);
}

glm::vec3 Mathf::ViewPortToNDCSpace(glm::vec2 monitorMousePos, glm::vec2 sceneWindowSize) {
	float x = (2.f * monitorMousePos.x) / sceneWindowSize.x - 1.f;
	float y = 1.f - (2.f * monitorMousePos.y) / sceneWindowSize.y;

	return { x, y, 1.0f };
}

glm::vec4 Mathf::NDCToClipSpace(glm::vec3 ndc) {
	return { ndc.x, ndc.y,-1.f, 1.f };
}

glm::vec4 Mathf::ClipToEyeSpace(glm::mat4 projectionMatrix, glm::vec4 clipSpaceCoords) {
	glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
	glm::vec4 eyeSpaceCoords = inverseProjectionMatrix * clipSpaceCoords;

	return glm::vec4(eyeSpaceCoords.x, eyeSpaceCoords.y, -1.f, 0.f);
}

glm::vec3 Mathf::EyeToWorldSpace(glm::mat4 viewMatrix, glm::vec4 eyeCoords) {
	glm::vec3 ray_world = glm::inverse(viewMatrix) * eyeCoords;

	return glm::normalize(ray_world);
}

// returns square of Euclidean distance between two points
float Mathf::sqrDist(glm::vec2 a, glm::vec2 b) {
	float dx = a.x - b.x, dy = a.y - b.y;
	return dx * dx + dy * dy;
}

// returns -1 if a -> b -> c forms a counter-clockwise turn,
// +1 for a clockwise turn, 0 if they are collinear
int Mathf::ccw(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
	float area = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	if (area > 0)
		return -1;
	else if (area < 0)
		return 1;
	return 0;
}

bool Mathf::SameFloat(float a, float b, float precision) {
	return fabs(a - b) < precision;
}

bool Mathf::isVectorApproximate(const glm::vec2& t1, const glm::vec2& t2, float precision) {
	bool SameX = SameFloat(t1.x, t2.x, precision);
	bool SameY = SameFloat(t1.y, t2.y, precision);

	return SameX && SameY;
}

bool Mathf::isVectorApproximate(const glm::vec3& t1, const glm::vec3& t2, float precision) {
	bool SameX = SameFloat(t1.x, t2.x, precision);
	bool SameY = SameFloat(t1.y, t2.y, precision);
	bool SameZ = SameFloat(t1.z, t2.z, precision);

	return SameX && SameY && SameZ;
}
