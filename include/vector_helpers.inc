#if defined __vh_included_
 #endinput
#endif
#define __vh_included_

#include <vector>

stock bool VectorsEqual(const float vec1[3], const float vec2[3], float tolerance = 0.001)
{
	return (FloatAbs(vec1[0] - vec2[0]) <= tolerance 
			&& FloatAbs(vec1[1] - vec2[1]) <= tolerance 
			&& FloatAbs(vec1[2] - vec2[2]) <= tolerance);
}

stock bool VectorIsZero(const float vec[3], float tolerance = 0.01)
{
	return (vec[0] > -tolerance && vec[0] < tolerance &&
			vec[1] > -tolerance && vec[1] < tolerance &&
			vec[2] > -tolerance && vec[2] < tolerance);
}

stock void VectorCopy(const float vIn[3], float vOut[3])
{
	vOut[0] = vIn[0];
	vOut[1] = vIn[1];
	vOut[2] = vIn[2];
}

// Figure out a middle point between source and destination in the given time
stock void VectorLerp(const float vec[3], const float dest[3], float time, float res[3])
{
	res[0] = vec[0] + (dest[0] - vec[0]) * time;
	res[1] = vec[1] + (dest[1] - vec[1]) * time;
	res[2] = vec[2] + (dest[2] - vec[2]) * time;
}

// Basically just gives a point in the length of scale towards the given direction from the source
stock void VectorMulAdd(float vec[3], const float dir[3], float scale, float result[3])
{
	result[0] = vec[0] + scale * dir[0];
	result[1] = vec[1] + scale * dir[1];
	result[2] = vec[2] + scale * dir[2];
}

// Fills result with values that are the smallest between source and desired
stock void VectorMin(const float src[3], const float dest[3], float res[3])
{
	res[0] = (src[0] < dest[0]) ? dest[0] : src[0];
	res[1] = (src[1] < dest[1]) ? dest[1] : src[1];
	res[2] = (src[2] < dest[2]) ? dest[2] : src[2];
}

// Same as Min but in the opposite direction
stock void VectorMax(const float src[3], const float dest[3], float res[3])
{
	res[0] = (src[0] > dest[0]) ? dest[0] : src[0];
	res[1] = (src[1] > dest[1]) ? dest[1] : src[1];
	res[2] = (src[2] > dest[2]) ? dest[2] : src[2];
}

stock void VectorRand(float vec[3], float flMin, float flMax)
{
	vec[0] = flMin + GetURandomFloat() * (flMax - flMin);
	vec[1] = flMin + GetURandomFloat() * (flMax - flMin);
	vec[2] = flMin + GetURandomFloat() * (flMax - flMin);
}

// Needs a rotation matrix, see below
stock void VectorRotate(float vec[3], const float matrix[3][3])
{
	vec[0] = GetVectorDotProduct(vec, matrix[0]);
	vec[1] = GetVectorDotProduct(vec, matrix[1]);
	vec[2] = GetVectorDotProduct(vec, matrix[2]);
}

// Rotate a vector about an axis relative to the direction(forward, right, up) by n degrees
stock void VectorRotateOnAxis(float vec[3], float dir[3], float degrees)
{
	float st, ct;
	SinCos(DegToRad(degrees), st, ct);
	
	NormalizeVector(dir, dir);
	
	float f = dir[0];
	float r = dir[1];
	float u = dir[2];
	
	float x = vec[0];
	float y = vec[1];
	float z = vec[2];
	
	vec[0] += (ct + (1 - ct) * f*f) * x;
	vec[0] += ((1 - ct) * f*r - u*st) * y;
	vec[0] += ((1 - ct) * f*z + y*st) * z;
	
	vec[1] += ((1 - ct) * f*r + u*st) * x;
	vec[1] += (ct + (1 - ct) * r*r) * y;
	vec[1] += ((1 - ct) * r*u - f*st) * z;

	vec[2] += ((1 - ct) * f*u - r*st) * x;
	vec[2] += ((1 - ct) * r*u + f*st) * y;
	vec[2] += (ct + (1 - ct) * u*u) * z;
}

stock void VectorMatrix(const float fwd[3], float matrix[3][3])
{
	float right[3], up[3];
	GetVectorVectors(fwd, right, up);
	
	NegateVector(right);
	
	for(int x=0; x<3; ++x)
	{
		matrix[0][x] = fwd[x];
		matrix[1][x] = right[x];
		matrix[2][x] = up[x];
	}
}

// Given an angle, creates a rotation matrix
stock void AngleMatrix(const float ang[3], float matrix[3][3])
{
	float sy, sp, sr, cy, cp, cr;
	SinCos(DegToRad(ang[0]), sy, cy);
	SinCos(DegToRad(ang[1]), sp, cp);
	SinCos(DegToRad(ang[2]), sr, cr);
	
	matrix[0][0] = cp*cy;
	matrix[1][0] = cp*sy;
	matrix[2][0] = sp*-1.0;
	
	matrix[0][1] = sp*(sr*cy-cr*sy);
	matrix[1][1] = sp*(sr*sy+cr*cy);
	matrix[2][1] = sr*cp;

	matrix[0][2] = sp*(cr*cy+sr*sy);
	matrix[1][2] = sp*(cr*sy-sr*cy);
	matrix[2][2] = cr*cp;
}

stock void AnglesNormalize(float ang[3])
{
	while(ang[0] >  89.0) ang[0]-=180.0;
	while(ang[0] < -89.0) ang[0]+=180.0;
	while(ang[1] > 180.0) ang[1]-=360.0;
	while(ang[1] <-180.0) ang[1]+=360.0;
}

stock void AngleNormalize(float &flAng)
{
	if(flAng > 180.0) flAng-=360.0;
	if(flAng <-180.0) flAng+=360.0;
}

stock void MatrixVectors(const float matrix[3][3], float fwd[3], float right[3], float up[3])
{
	for(int x=0; x<3; x++)
	{
		fwd[x] = matrix[0][x];
		right[x] = matrix[1][x];
		up[x] = matrix[2][x];
	}
	
	NegateVector(right);
}

stock void MatrixAngles(const float matrix[3][3], float ang[3])
{
	float fwd[3], right[3];
	float up[3] = {0.0, 0.0, 1.0};
	
	for(int x=0; x<3; x++)
	{
		fwd[x] = matrix[0][x];
		right[x] = matrix[1][x];
	}
	
	up[2] = matrix[2][2];
	
	float xyD = SquareRoot(fwd[0] * fwd[0] + fwd[1] * fwd[1]);
	if(xyD > 0.001)
	{
		ang[0] = Rad2Deg(ArcTangent2(fwd[1], fwd[0]));
		ang[1] = Rad2Deg(ArcTangent2(-fwd[2], xyD));
		ang[2] = Rad2Deg(ArcTangent2(right[2], up[2]));
	}
	else
	{
		ang[0] = Rad2Deg(ArcTangent2(-right[0], right[1]));
		ang[1] = Rad2Deg(ArcTangent2(-fwd[2], xyD));
		ang[2] = 0.0;
	}
}

// Helper function to look nicer
stock void SinCos(float radian, float &sine, float &cosine)
{
	sine = Sine(radian);
	cosine = Cosine(radian);
}
