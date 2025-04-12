#pragma once

namespace Mathf
{
	constexpr float EPSILON = glm::epsilon<float>(); // Same as glm::epsilon<float>()
	constexpr float EPSILON2 = EPSILON * EPSILON; // Epsilon Squared
	constexpr float PI = glm::pi<float>(); // Same as glm::pi<float>()
	constexpr float PRECISION = 0.04f;

	// Compares two float values for equality using epsilon
	// Returns TRUE if lhs == rhs
	bool IsEqual(float lhs, float rhs);

	// Loops the value t, so that it is never larger than length and never smaller than 0.
	float Repeat(float t, float length);

	// Wraps the value t, so that it returns the minimum when overflow or maximum if underflow
	int Wrap(int t, int min, int max);

	// Ping pongs the value t, so that it is never larger than length and never smaller than 0.
	float PingPong(float t, float length);

	// Clamps the given value within a minimum and maximum value
	float Clamp(float val, float min, float max);

	// Computes the Gaussian Kernel used for Blur
	// Taken from: https://www.gamedev.net/forums/topic/694503-gaussian-based-weights/
	std::vector<double> ComputeGaussianKernel(double sigma, int n);

	// Returns TRUE if any of the vector's component is near-zero or zero
	bool IsComponentZero(const glm::vec3& val);

	// Converts a given Quaternion to Euler Angle in Degrees
	// https://gamedev.stackexchange.com/a/13441/102948
	glm::vec3 QuaternionToEuler(glm::quat quat);

	// Extracts and returns the Scale of a model matrix
	// * Scale does not return the correct sign bit
	glm::vec3 ExtractScale(const glm::mat4& matrix);

	// Extracts and returns the Rotation of a model matrix
	glm::quat ExtractRotation(const glm::mat4& matrix);

	// Extracts and outputs the Scale and Rotation of a model matrix
	// * Scale does not return the correct sign bit
	void ExtractScaleRotation(const glm::mat4& matrix, glm::vec3* scale, glm::quat* rotation);

	// Computes a Model Matrix with a given translation, rotation, scale
	glm::mat4 ComputeModelMatrix(glm::vec3 pos, glm::vec3 rotDegrees, glm::vec3 scale);

	// Viewport-space to NDC-space
	glm::vec3 ViewPortToNDCSpace(glm::vec2 monitorMousePos, glm::vec2 sceneWindowSize);

	// NDC-space to Clip-space
	glm::vec4 NDCToClipSpace(glm::vec3 ndc);

	// Clip-space to Eye-space
	glm::vec4 ClipToEyeSpace(glm::mat4 projectionMatrix, glm::vec4 clipSpaceCoords);

	// Eye-space to World-space
	glm::vec3 EyeToWorldSpace(glm::mat4 viewMatrix, glm::vec4 eyeCoords);

	// returns square of Euclidean distance between two points
	float sqrDist(glm::vec2 a, glm::vec2 b);

	// returns -1 if a -> b -> c forms a counter-clockwise turn,
	// +1 for a clockwise turn, 0 if they are collinear
	int ccw(glm::vec2 a, glm::vec2 b, glm::vec2 c);

	bool SameFloat(float a, float b, float precision = PRECISION);

	bool isVectorApproximate(const glm::vec2& t1, const glm::vec2& t2, float precision = PRECISION);

	bool isVectorApproximate(const glm::vec3& t1, const glm::vec3& t2, float precision = PRECISION);

	bool AreLinesCollinear(const glm::vec2& A, const glm::vec2& B, const glm::vec2& C, const glm::vec2& D);
};