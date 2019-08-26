#include "RenderObjects.h"

glm::mat4x4 RotateMat(float xy, float yz, float zx, float xw, float yw, float zw)
{
	glm::mat4x4 copyMatrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(xy);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(yz);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(zx);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(xw);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(yw);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(zw);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	copyMatrix *= tempMat;

	return copyMatrix;
}

//Line
Line::Line()
{
	point = glm::vec4(0, 0, 0, 0);
	direction = glm::vec4(0, 0, 0, 0);
}

Line::Line(glm::vec4 pointVec, glm::vec4 directionVec)
{
	point = pointVec;
	direction = directionVec;
}

Line::Line(float xPos, float yPos, float zPos, float wPos, float xDir, float yDir, float zDir, float wDir)
{
	point = glm::vec4(xPos, yPos, zPos, wPos);
	direction = glm::vec4(xDir, yDir, zDir, wDir);
}
void Line::Translate(float x, float y, float z, float w)
{
	point.x += x;
	point.y += y;
	point.z += z;
	point.w += w;
}
void Line::Rotate(float xy, float yz, float zx, float xw, float yw, float zw)
{
	glm::mat4x4 matrix = RotateMat(xy, yz, zx, xw, yw, zw);
	glm::vec4 center = point + (direction*0.5f);
	point -= center;
	direction -= center;
	point = matrix * point;
	direction = matrix * direction;
	point += center;
	direction += center;
}
void Line::Transform(glm::mat4x4 matrix)
{
	glm::vec4 center = point + (direction*0.5f);
	point -= center;
	direction -= center;
	point = matrix * point;
	direction = matrix * direction;
	point += center;
	direction += center;
}
void Line::TransformAround(glm::mat4x4 matrix, glm::vec4 pivot)
{
	point -= pivot;
	direction -= pivot;
	point = matrix * point;
	direction = matrix * direction;
	point += pivot;
	direction += pivot;
}
//Camera
Camera::Camera()
{
	position = glm::vec4(0, 0, 0, 0);
	rotation[0] = 0;
	rotation[1] = 0;
	rotation[2] = 0;
	rotation[3] = 0;
	rotation[4] = 0;
	rotation[5] = 0;
}

Camera::Camera(glm::vec4 positionVec, float xy, float yz, float zx, float xw, float yw, float zw)
{
	position = positionVec;
	rotation[0] = xy;
	rotation[1] = yz;
	rotation[2] = zx;
	rotation[3] = xw;
	rotation[4] = yw;
	rotation[5] = zw;
}

glm::mat4x4 Camera::GetTransform()
{
	copyMatrix = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	glm::mat4x4 tempMat;
	float angle;
	//Inverted and column major

	//X->Y rotation
	angle = glm::radians(rotation[0]);
	tempMat = glm::mat4x4(glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Y->Z rotation
	angle = glm::radians(rotation[1]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//Z->X rotation
	angle = glm::radians(rotation[2]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, glm::sin(angle), 0, 0, 1, 0, 0, -glm::sin(angle), 0, glm::cos(angle), 0, 0, 0, 0, 1);
	copyMatrix *= tempMat;

	//X->W rotation
	angle = glm::radians(rotation[3]);
	tempMat = glm::mat4x4(glm::cos(angle), 0, 0, -glm::sin(angle), 0, 1, 0, 0, 0, 0, 1, 0, glm::sin(angle), 0, 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Y->W rotation
	angle = glm::radians(rotation[4]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, glm::cos(angle), 0, -glm::sin(angle), 0, 0, 1, 0, 0, glm::sin(angle), 0, glm::cos(angle));
	copyMatrix *= tempMat;

	//Z->W rotation
	angle = glm::radians(rotation[5]);
	tempMat = glm::mat4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, glm::cos(angle), -glm::sin(angle), 0, 0, glm::sin(angle), glm::cos(angle));
	copyMatrix *= tempMat;

	return copyMatrix;
}

float* Camera::GetTransformValuePtr()
{
	GetTransform();
	return glm::value_ptr(copyMatrix);
}

void Camera::RotateXY(float degrees)
{
	rotation[0] += degrees;
}

void Camera::RotateYZ(float degrees)
{
	rotation[1] += degrees;
}

void Camera::RotateZX(float degrees)
{
	rotation[2] += degrees;
}

void Camera::RotateXW(float degrees)
{
	rotation[3] += degrees;
}

void Camera::RotateYW(float degrees)
{
	rotation[4] += degrees;
}

void Camera::RotateZW(float degrees)
{
	rotation[5] += degrees;
}
//Triangle
Triangle::Triangle()
{
	lines[0] = Line();
	lines[1] = Line();
	lines[2] = Line();
}

Triangle::Triangle(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3)
{
	lines[0] = Line(point1, point2 - point1);
	lines[1] = Line(point2, point3 - point2);
	lines[2] = Line(point3, point1 - point3);
}

Triangle::Triangle(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3)
{
	lines[0] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x2 - x1, y2 - y1, z2 - z1, w2 - w1));
	lines[1] = Line(glm::vec4(x2, y2, z2, w2), glm::vec4(x3 - x2, y3 - y2, z3 - z2, w3 - w2));
	lines[2] = Line(glm::vec4(x3, y3, z3, w3), glm::vec4(x1 - x3, y1 - y3, z1 - z3, w1 - w3));
}
//Tetrahedron
Tetrahedron::Tetrahedron()
{
	lines[0] = Line();
	lines[1] = Line();
	lines[2] = Line();
	lines[3] = Line();
	lines[4] = Line();
	lines[5] = Line();
}

Tetrahedron::Tetrahedron(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3, glm::vec4 point4)
{
	lines[0] = Line(point1, point2 - point1);
	lines[1] = Line(point1, point3 - point1);
	lines[2] = Line(point1, point4 - point1);
	lines[3] = Line(point2, point3 - point2);
	lines[4] = Line(point2, point4 - point2);
	lines[5] = Line(point3, point4 - point3);
}

Tetrahedron::Tetrahedron(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4)
{
	lines[0] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x2 - x1, y2 - y1, z2 - z1, w2 - w1));
	lines[1] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x3 - x1, y3 - y1, z3 - z1, w3 - w1));
	lines[2] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x4 - x1, y4 - y1, z4 - z1, w4 - w1));
	lines[3] = Line(glm::vec4(x2, y2, z2, w2), glm::vec4(x3 - x2, y3 - y2, z3 - z2, w3 - w2));
	lines[4] = Line(glm::vec4(x2, y2, z2, w2), glm::vec4(x4 - x2, y4 - y2, z4 - z2, w4 - w2));
	lines[5] = Line(glm::vec4(x3, y3, z3, w3), glm::vec4(x4 - x3, y4 - y3, z4 - z3, w4 - w3));
}
void Tetrahedron::Translate(float x, float y, float z, float w)
{
	lines[0].Translate(x, y, z, w);
	lines[1].Translate(x, y, z, w);
	lines[2].Translate(x, y, z, w);
	lines[3].Translate(x, y, z, w);
	lines[4].Translate(x, y, z, w);
	lines[5].Translate(x, y, z, w);
}
//Cube
Cube::Cube()
{
	lines[0] = Line(-0.5f, -0.5f, -0.5f, 0, 1, 0, 0, 0);
	lines[1] = Line(0.5f, -0.5f, -0.5f, 0, 0, 1, 0, 0);
	lines[2] = Line(0.5f, 0.5f, -0.5f, 0, -1, 0, 0, 0);
	lines[3] = Line(-0.5f, 0.5f, -0.5f, 0, 0, -1, 0, 0);
	lines[4] = Line(-0.5f, -0.5f, 0.5f, 0, 1, 0, 0, 0);
	lines[5] = Line(0.5f, -0.5f, 0.5f, 0, 0, 1, 0, 0);
	lines[6] = Line(0.5f, 0.5f, 0.5f, 0, -1, 0, 0, 0);
	lines[7] = Line(-0.5f, 0.5f, 0.5f, 0, 0, -1, 0, 0);
	lines[8] = Line(-0.5f, -0.5f, -0.5f, 0, 0, 0, 1, 0);
	lines[9] = Line(0.5f, -0.5f, -0.5f, 0, 0, 0, 1, 0);
	lines[10] = Line(0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	lines[11] = Line(-0.5f, 0.5f, -0.5f, 0, 0, 0, 1, 0);
	center = glm::vec4(0, 0, 0, 0);
}

Cube::Cube(glm::vec4 position, float scale)
{
	lines[0] = Line(glm::vec4(position.x - scale, position.y - scale, position.z - scale, 0), glm::vec4(scale * 2, 0, 0, 0));
	lines[1] = Line(glm::vec4(position.x + scale, position.y - scale, position.z - scale, 0), glm::vec4(0, scale * 2, 0, 0));
	lines[2] = Line(glm::vec4(position.x + scale, position.y + scale, position.z - scale, 0), glm::vec4(-scale * 2, 0, 0, 0));
	lines[3] = Line(glm::vec4(position.x - scale, position.y + scale, position.z - scale, 0), glm::vec4(0, -scale * 2, 0, 0));
	lines[4] = Line(glm::vec4(position.x - scale, position.y - scale, position.z + scale, 0), glm::vec4(scale * 2, 0, 0, 0));
	lines[5] = Line(glm::vec4(position.x + scale, position.y - scale, position.z + scale, 0), glm::vec4(0, scale * 2, 0, 0));
	lines[6] = Line(glm::vec4(position.x + scale, position.y + scale, position.z + scale, 0), glm::vec4(-scale * 2, 0, 0, 0));
	lines[7] = Line(glm::vec4(position.x - scale, position.y + scale, position.z + scale, 0), glm::vec4(0, -scale * 2, 0, 0));
	lines[8] = Line(glm::vec4(position.x - scale, position.y - scale, position.z - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[9] = Line(glm::vec4(position.x + scale, position.y - scale, position.z - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[10] = Line(glm::vec4(position.x + scale, position.y + scale, position.z - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[11] = Line(glm::vec4(position.x - scale, position.y + scale, position.z - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	center = position;
}

Cube::Cube(float px, float py, float pz, float pw, float scale)
{
	lines[0] = Line(glm::vec4(px - scale, py - scale, pz - scale, 0), glm::vec4(scale * 2, 0, 0, 0));
	lines[1] = Line(glm::vec4(px + scale, py - scale, pz - scale, 0), glm::vec4(0, scale * 2, 0, 0));
	lines[2] = Line(glm::vec4(px + scale, py + scale, pz - scale, 0), glm::vec4(-scale * 2, 0, 0, 0));
	lines[3] = Line(glm::vec4(px - scale, py + scale, pz - scale, 0), glm::vec4(0, -scale * 2, 0, 0));
	lines[4] = Line(glm::vec4(px - scale, py - scale, pz + scale, 0), glm::vec4(scale * 2, 0, 0, 0));
	lines[5] = Line(glm::vec4(px + scale, py - scale, pz + scale, 0), glm::vec4(0, scale * 2, 0, 0));
	lines[6] = Line(glm::vec4(px + scale, py + scale, pz + scale, 0), glm::vec4(-scale * 2, 0, 0, 0));
	lines[7] = Line(glm::vec4(px - scale, py + scale, pz + scale, 0), glm::vec4(0, -scale * 2, 0, 0));
	lines[8] = Line(glm::vec4(px - scale, py - scale, pz - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[9] = Line(glm::vec4(px + scale, py - scale, pz - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[10] = Line(glm::vec4(px + scale, py + scale, pz - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	lines[11] = Line(glm::vec4(px - scale, py + scale, pz - scale, 0), glm::vec4(0, 0, scale * 2, 0));
	center = glm::vec4(px, py, pz, pw);
}

void Cube::Translate(float x, float y, float z, float w)
{
	lines[0].Translate(x, y, z, w);
	lines[1].Translate(x, y, z, w);
	lines[2].Translate(x, y, z, w);
	lines[3].Translate(x, y, z, w);
	lines[4].Translate(x, y, z, w);
	lines[5].Translate(x, y, z, w);
	lines[6].Translate(x, y, z, w);
	lines[7].Translate(x, y, z, w);
	lines[8].Translate(x, y, z, w);
	lines[9].Translate(x, y, z, w);
	lines[10].Translate(x, y, z, w);
	lines[11].Translate(x, y, z, w);
	center += glm::vec4(x, y, z, w);
}
void Cube::Rotate(float xy, float yz, float zx, float xw, float yw, float zw)
{
	glm::mat4x4 matrix = RotateMat(xy, yz, zx, xw, yw, zw);
	lines[0].TransformAround(matrix, center);
	lines[1].TransformAround(matrix, center);
	lines[2].TransformAround(matrix, center);
	lines[3].TransformAround(matrix, center);
	lines[4].TransformAround(matrix, center);
	lines[5].TransformAround(matrix, center);
	lines[6].TransformAround(matrix, center);
	lines[7].TransformAround(matrix, center);
	lines[8].TransformAround(matrix, center);
	lines[9].TransformAround(matrix, center);
	lines[10].TransformAround(matrix, center);
	lines[11].TransformAround(matrix, center);
}
void Cube::Transform(glm::mat4x4 matrix)
{
	lines[0].TransformAround(matrix, center);
	lines[1].TransformAround(matrix, center);
	lines[2].TransformAround(matrix, center);
	lines[3].TransformAround(matrix, center);
	lines[4].TransformAround(matrix, center);
	lines[5].TransformAround(matrix, center);
	lines[6].TransformAround(matrix, center);
	lines[7].TransformAround(matrix, center);
	lines[8].TransformAround(matrix, center);
	lines[9].TransformAround(matrix, center);
	lines[10].TransformAround(matrix, center);
	lines[11].TransformAround(matrix, center);
}
void Cube::TransformAround(glm::mat4x4 matrix, glm::vec4 pivot)
{
	lines[0].TransformAround(matrix, pivot);
	lines[1].TransformAround(matrix, pivot);
	lines[2].TransformAround(matrix, pivot);
	lines[3].TransformAround(matrix, pivot);
	lines[4].TransformAround(matrix, pivot);
	lines[5].TransformAround(matrix, pivot);
	lines[6].TransformAround(matrix, pivot);
	lines[7].TransformAround(matrix, pivot);
	lines[8].TransformAround(matrix, pivot);
	lines[9].TransformAround(matrix, pivot);
	lines[10].TransformAround(matrix, pivot);
	lines[11].TransformAround(matrix, pivot);
}
//Pentachoron
Pentachoron::Pentachoron()
{
	tetrahedra[0] = Tetrahedron();
	tetrahedra[1] = Tetrahedron();
	tetrahedra[2] = Tetrahedron();
	tetrahedra[3] = Tetrahedron();
	tetrahedra[4] = Tetrahedron();
}

Pentachoron::Pentachoron(glm::vec4 point1, glm::vec4 point2, glm::vec4 point3, glm::vec4 point4, glm::vec4 point5)
{
	tetrahedra[0] = Tetrahedron(point1, point2, point3, point4);
	tetrahedra[1] = Tetrahedron(point1, point2, point3, point5);
	tetrahedra[2] = Tetrahedron(point1, point2, point5, point4);
	tetrahedra[3] = Tetrahedron(point1, point5, point3, point4);
	tetrahedra[4] = Tetrahedron(point5, point2, point3, point4);
}

Pentachoron::Pentachoron(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4, float x5, float y5, float z5, float w5)
{
	tetrahedra[0] = Tetrahedron(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);
	tetrahedra[1] = Tetrahedron(x1, y1, z1, w1, x2, y2, z2, w2, x3, y3, z3, w3, x5, y5, z5, w5);
	tetrahedra[2] = Tetrahedron(x1, y1, z1, w1, x2, y2, z2, w2, x5, y5, z5, w5, x4, y4, z4, w4);
	tetrahedra[3] = Tetrahedron(x1, y1, z1, w1, x5, y5, z5, w5, x3, y3, z3, w3, x4, y4, z4, w4);
	tetrahedra[4] = Tetrahedron(x5, y5, z5, w5, x2, y2, z2, w2, x3, y3, z3, w3, x4, y4, z4, w4);
}

void Pentachoron::Translate(float x, float y, float z, float w)
{
	tetrahedra[0].Translate(x, y, z, w);
	tetrahedra[1].Translate(x, y, z, w);
	tetrahedra[2].Translate(x, y, z, w);
	tetrahedra[3].Translate(x, y, z, w);
	tetrahedra[4].Translate(x, y, z, w);
}
//Tesseract
Tesseract::Tesseract()
{
	cubes[0] = Cube();
	cubes[1] = Cube();
	cubes[2] = Cube();
	cubes[3] = Cube();
	cubes[4] = Cube();
	cubes[5] = Cube();
	cubes[6] = Cube();
	cubes[7] = Cube();
	center = glm::vec4(0, 0, 0, 0);
	cubes[2].Rotate(0, 0, 0, 90, 0, 0);
	cubes[3].Rotate(0, 0, 0, 90, 0, 0);
	cubes[4].Rotate(0, 0, 0, 0, 90, 0);
	cubes[5].Rotate(0, 0, 0, 0, 90, 0);
	cubes[6].Rotate(0, 0, 0, 0, 0, 90);
	cubes[7].Rotate(0, 0, 0, 0, 0, 90);
	cubes[0].Translate(0, 0, 0, -0.5f);
	cubes[1].Translate(0, 0, 0, 0.5f);
	cubes[2].Translate(0.5f, 0, 0, 0);
	cubes[3].Translate(-0.5f, 0, 0, 0);
	cubes[4].Translate(0, 0.5f, 0, 0);
	cubes[5].Translate(0, -0.5f, 0, 0);
	cubes[6].Translate(0, 0, -0.5f, 0);
	cubes[7].Translate(0, 0, 0.5f, 0);
}

Tesseract::Tesseract(glm::vec4 position, float scale)
{
	cubes[0] = Cube(position, scale);
	cubes[1] = Cube(position, scale);
	cubes[2] = Cube(position, scale);
	cubes[3] = Cube(position, scale);
	cubes[4] = Cube(position, scale);
	cubes[5] = Cube(position, scale);
	cubes[6] = Cube(position, scale);
	cubes[7] = Cube(position, scale);
	center = position;
	cubes[2].Rotate(0, 0, 0, 90, 0, 0);
	cubes[3].Rotate(0, 0, 0, 90, 0, 0);
	cubes[4].Rotate(0, 0, 0, 0, 90, 0);
	cubes[5].Rotate(0, 0, 0, 0, 90, 0);
	cubes[6].Rotate(0, 0, 0, 0, 0, 90);
	cubes[7].Rotate(0, 0, 0, 0, 0, 90);
	cubes[0].Translate(0, 0, 0, -0.5f*scale);
	cubes[1].Translate(0, 0, 0, 0.5f*scale);
	cubes[2].Translate(0.5f*scale, 0, 0, 0);
	cubes[3].Translate(-0.5f*scale, 0, 0, 0);
	cubes[4].Translate(0, 0.5f*scale, 0, 0);
	cubes[5].Translate(0, -0.5f*scale, 0, 0);
	cubes[6].Translate(0, 0, -0.5f*scale, 0);
	cubes[7].Translate(0, 0, 0.5f*scale, 0);
}

Tesseract::Tesseract(float px, float py, float pz, float pw, float scale)
{
	cubes[0] = Cube(px, py, pz, pw, scale);
	cubes[1] = Cube(px, py, pz, pw, scale);
	cubes[2] = Cube(px, py, pz, pw, scale);
	cubes[3] = Cube(px, py, pz, pw, scale);
	cubes[4] = Cube(px, py, pz, pw, scale);
	cubes[5] = Cube(px, py, pz, pw, scale);
	cubes[6] = Cube(px, py, pz, pw, scale);
	cubes[7] = Cube(px, py, pz, pw, scale);
	center = glm::vec4(px, py, pz, pw);
	cubes[2].Rotate(0, 0, 0, 90, 0, 0);
	cubes[3].Rotate(0, 0, 0, 90, 0, 0);
	cubes[4].Rotate(0, 0, 0, 0, 90, 0);
	cubes[5].Rotate(0, 0, 0, 0, 90, 0);
	cubes[6].Rotate(0, 0, 0, 0, 0, 90);
	cubes[7].Rotate(0, 0, 0, 0, 0, 90);
	cubes[0].Translate(0, 0, 0, -scale);
	cubes[1].Translate(0, 0, 0, scale);
	cubes[2].Translate(scale, 0, 0, 0);
	cubes[3].Translate(-scale, 0, 0, 0);
	cubes[4].Translate(0, scale, 0, 0);
	cubes[5].Translate(0, -scale, 0, 0);
	cubes[6].Translate(0, 0, -scale, 0);
	cubes[7].Translate(0, 0, scale, 0);
}

void Tesseract::Translate(float x, float y, float z, float w)
{
	cubes[0].Translate(x, y, z, w);
	cubes[1].Translate(x, y, z, w);
	cubes[2].Translate(x, y, z, w);
	cubes[3].Translate(x, y, z, w);
	cubes[4].Translate(x, y, z, w);
	cubes[5].Translate(x, y, z, w);
	cubes[6].Translate(x, y, z, w);
	cubes[7].Translate(x, y, z, w);
	center += glm::vec4(x, y, z, w);
}
void Tesseract::Rotate(float xy, float yz, float zx, float xw, float yw, float zw)
{
	glm::mat4x4 matrix = RotateMat(xy, yz, zx, xw, yw, zw);
	cubes[0].TransformAround(matrix, center);
	cubes[1].TransformAround(matrix, center);
	cubes[2].TransformAround(matrix, center);
	cubes[3].TransformAround(matrix, center);
	cubes[4].TransformAround(matrix, center);
	cubes[5].TransformAround(matrix, center);
	cubes[6].TransformAround(matrix, center);
	cubes[7].TransformAround(matrix, center);
}
void Tesseract::Transform(glm::mat4x4 matrix)
{
	cubes[0].TransformAround(matrix, center);
	cubes[1].TransformAround(matrix, center);
	cubes[2].TransformAround(matrix, center);
	cubes[3].TransformAround(matrix, center);
	cubes[4].TransformAround(matrix, center);
	cubes[5].TransformAround(matrix, center);
	cubes[6].TransformAround(matrix, center);
	cubes[7].TransformAround(matrix, center);
}
//RenderManager
RenderManager::RenderManager()
{
	crossSection = GPUProgram("Assets\\Kernels\\CrossSection.cl");
}

RenderManager::RenderManager(Tetrahedron* tetrahedronsToPass, int arraySize)
{
	crossSection = GPUProgram("Assets\\Kernels\\CrossSection.cl");
	for (int i = 0; i < arraySize; i++)
	{
		tetrahedra.push_back(tetrahedronsToPass[i]);
	}
}

unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	if (tetrahedra.size() == 0 && cubes.size() == 0)
		return 0;
#pragma region Kernel setup and launch
	crossSection.SetFunction("CrossSection");

	glm::vec4* points = new glm::vec4[(tetrahedra.size() * 6) + (cubes.size() * 12)];
	glm::vec4* directions = new glm::vec4[(tetrahedra.size() * 6) + (cubes.size() * 12)];
	char* states = new char[(tetrahedra.size() * 6) + (cubes.size() * 12)];

	std::vector<Line> lines;
	lines.reserve((tetrahedra.size() * 6) + (cubes.size() * 12));
	{
		int iLimit = tetrahedra.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(tetrahedra[i].lines[0]);
			lines.push_back(tetrahedra[i].lines[1]);
			lines.push_back(tetrahedra[i].lines[2]);
			lines.push_back(tetrahedra[i].lines[3]);
			lines.push_back(tetrahedra[i].lines[4]);
			lines.push_back(tetrahedra[i].lines[5]);
		}
		iLimit = cubes.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(cubes[i].lines[0]);
			lines.push_back(cubes[i].lines[1]);
			lines.push_back(cubes[i].lines[2]);
			lines.push_back(cubes[i].lines[3]);
			lines.push_back(cubes[i].lines[4]);
			lines.push_back(cubes[i].lines[5]);
			lines.push_back(cubes[i].lines[6]);
			lines.push_back(cubes[i].lines[7]);
			lines.push_back(cubes[i].lines[8]);
			lines.push_back(cubes[i].lines[9]);
			lines.push_back(cubes[i].lines[10]);
			lines.push_back(cubes[i].lines[11]);
		}
	}
	{
		int iLimit = lines.size();
		for (int i = 0; i < iLimit; i++)
		{
			points[i] = lines[i].point;
			directions[i] = lines[i].direction;
		}
	}
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), points);
	cl::Buffer directionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), directions);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lines.size());
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer positionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, pointBuffer);
	crossSection.SetVariable(1, directionBuffer);
	crossSection.SetVariable(2, stateBuffer);
	crossSection.SetVariable(3, matrixBuffer);
	crossSection.SetVariable(4, positionBuffer);
	crossSection.LaunchKernel(0, 0, lines.size());
#pragma endregion
	crossSection.ReadKernel(pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lines.size(), points);
	crossSection.ReadKernel(stateBuffer, GL_TRUE, 0, sizeof(char) * lines.size(), states);

	vertexPos.clear();
	vertexCol.clear();

	//Kernel tetrahedron return handler
	{
		faceColor = 0;
		int tLimit = tetrahedra.size();
		for (int t = 0; t < tLimit; t++)
		{
			//Make a Deque to hold the results. Sort results to Right and left and add to the lists. Sort the lists then combine them. Create triangles from the combinations
			std::deque<glm::vec4> vertexResults;

			//Which lines crossected
			if (states[(t * 6)] == 0)
			{
				vertexResults.push_back(points[(t * 6)]);
			}
			if (states[(t * 6) + 1] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 1]);
			}
			if (states[(t * 6) + 2] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 2]);
			}
			if (states[(t * 6) + 3] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 3]);
			}
			if (states[(t * 6) + 4] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 4]);
			}
			if (states[(t * 6) + 5] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 5]);
			}

			int vectorSize = vertexResults.size();
			if (vectorSize < 3)
				continue;

			std::vector<glm::vec4> planedVertices;
			planedVertices.reserve(vectorSize);
#pragma region To plane space
			{ //This stuff is explained is some graph paper I worked on. You would need to see it to understand this.
				glm::vec4 origin = vertexResults[0]; //i, j, k
				glm::vec4 planeVec1 = vertexResults[1] - origin; // f, g, h
				glm::vec4 planeVec2 = vertexResults[2] - origin; // m, n, o

				float w = (planeVec2.x * planeVec1.y) - (planeVec2.y * planeVec1.x);
				float gdivw = planeVec1.y / w;
				float fdivw = planeVec1.x / w;
				float kdivh = -(origin.z / planeVec1.z);
				float odivh = planeVec2.z / planeVec1.z;
				//vectorSize = vertexResults.size();
				for (int i = 0; i < vectorSize; i++)
				{
					glm::vec4& vertex = vertexResults[i];
					float b = (gdivw*(vertex.x - origin.x)) - (fdivw*(vertex.y - origin.y));
					planedVertices.push_back(glm::vec4((vertex.z / planeVec1.z) + kdivh - (odivh*b), b, 0, 0)); //The x coordinate is called "a"	
				}
			}
#pragma endregion

#pragma region Rotation ordering stuff
			glm::vec4 centroid(0);
			std::vector<SortItem> rightVertices;
			std::vector<SortItem> leftVertices;
			//vectorSize = vertexResults.size();
			for (int i = 0; i < vectorSize; i++)
			{
				centroid += planedVertices[i];
			}
			centroid /= vectorSize;
			for (int i = 0; i < vectorSize; i++)
			{
				glm::vec4& vertex = planedVertices[i];
				vertex -= centroid;
				if (vertex.x > 0)
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					rightVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag)); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x| 
				}
				else
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					leftVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag));
				}
				//Add to sides with values and ID
			}
#pragma endregion

			//Sort each side
			std::vector<glm::vec4> orderedVertices;
			orderedVertices.reserve(vertexResults.size());
			InsertSort(rightVertices);
			InsertSort(leftVertices);
			vectorSize = rightVertices.size();
			for (int i = 0; i < vectorSize; i++)
			{
				orderedVertices.push_back(vertexResults[rightVertices[i].ID]);
			}
			vectorSize = leftVertices.size();
			for (int i = vectorSize - 1; i >= 0; i--)
			{
				orderedVertices.push_back(vertexResults[leftVertices[i].ID]);
			}
			vectorSize = orderedVertices.size() - 2;
			for (int i = 0; i < vectorSize; i++)
			{
				switch (faceColor)
				{
				case 0:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 1:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					break;
				case 2:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					break;
				case 3:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				case 4:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					break;
				}
			}
			faceColor = Modulo(faceColor + 1, 5);
		}
	}
	//Kernel cube return handler
	{
		faceColor = 0;
		int stateOffset = tetrahedra.size() * 6;
		int tLimit = cubes.size();
		for (int t = 0; t < tLimit; t++)
		{
			//Make a Deque to hold the results. Sort results to Right and left and add to the lists. Sort the lists then combine them. Create triangles from the combinations
			std::deque<glm::vec4> vertexResults;

			//Which lines crossected
			if (states[(t * 12) + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + stateOffset]);
			}
			if (states[(t * 12) + 1 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 1 + stateOffset]);
			}
			if (states[(t * 12) + 2 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 2 + stateOffset]);
			}
			if (states[(t * 12) + 3 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 3 + stateOffset]);
			}
			if (states[(t * 12) + 4 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 4 + stateOffset]);
			}
			if (states[(t * 12) + 5 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 5 + stateOffset]);
			}
			if (states[(t * 12) + 6 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 6 + stateOffset]);
			}
			if (states[(t * 12) + 7 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 7 + stateOffset]);
			}
			if (states[(t * 12) + 8 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 8 + stateOffset]);
			}
			if (states[(t * 12) + 9 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 9 + stateOffset]);
			}
			if (states[(t * 12) + 10 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 10 + stateOffset]);
			}
			if (states[(t * 12) + 11 + stateOffset] == 0)
			{
				vertexResults.push_back(points[(t * 12) + 11 + stateOffset]);
			}
			char* debug[100];
			for (int m = 0; m < 100; m++)
			{
				debug[m] = states + m;
			}
			int vectorSize = vertexResults.size();
			if (vectorSize < 3)
				continue;

			std::vector<glm::vec4> planedVertices;
			planedVertices.reserve(vectorSize);
#pragma region To plane space
			{ //This stuff is explained is some graph paper I worked on. You would need to see it to understand this.
				glm::vec4 origin = vertexResults[0]; //i, j, k
				glm::vec4 planeVec1 = vertexResults[1] - origin; // f, g, h
				glm::vec4 planeVec2 = vertexResults[2] - origin; // m, n, o

				float w = (planeVec2.x * planeVec1.y) - (planeVec2.y * planeVec1.x);
				float gdivw = planeVec1.y / w;
				float fdivw = planeVec1.x / w;
				float kdivh = -(origin.z / planeVec1.z);
				float odivh = planeVec2.z / planeVec1.z;
				//vectorSize = vertexResults.size();
				for (int i = 0; i < vectorSize; i++)
				{
					glm::vec4& vertex = vertexResults[i];
					float b = (gdivw*(vertex.x - origin.x)) - (fdivw*(vertex.y - origin.y));
					planedVertices.push_back(glm::vec4((vertex.z / planeVec1.z) + kdivh - (odivh*b), b, 0, 0)); //The x coordinate is called "a"	
				}
			}
#pragma endregion

#pragma region Rotation ordering stuff
			glm::vec4 centroid(0);
			std::vector<SortItem> rightVertices;
			std::vector<SortItem> leftVertices;
			//vectorSize = vertexResults.size();
			for (int i = 0; i < vectorSize; i++)
			{
				centroid += planedVertices[i];
			}
			centroid /= vectorSize;
			for (int i = 0; i < vectorSize; i++)
			{
				glm::vec4& vertex = planedVertices[i];
				vertex -= centroid;
				if (vertex.x > 0)
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					rightVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag)); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x| 
				}
				else
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					leftVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag));
				}
				//Add to sides with values and ID
			}
#pragma endregion

			//Sort each side
			std::vector<glm::vec4> orderedVertices;
			orderedVertices.reserve(vertexResults.size());
			InsertSort(rightVertices);
			InsertSort(leftVertices);
			vectorSize = rightVertices.size();
			for (int i = 0; i < vectorSize; i++)
			{
				orderedVertices.push_back(vertexResults[rightVertices[i].ID]);
			}
			vectorSize = leftVertices.size();
			for (int i = vectorSize - 1; i >= 0; i--)
			{
				orderedVertices.push_back(vertexResults[leftVertices[i].ID]);
			}
			vectorSize = orderedVertices.size() - 2;
			for (int i = 0; i < vectorSize; i++)
			{
				switch (faceColor)
				{
				case 0:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 1:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
					break;
				case 2:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
					break;
				case 3:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
					break;
				case 4:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
					break;
				case 5:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(1.0f, 0.0f, 1.0f, 1.0f));
					break;
				case 6:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));
					break;
				case 7:
					vertexPos.push_back(orderedVertices[0]);
					vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 1]);
					vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
					vertexPos.push_back(orderedVertices[i + 2]);
					vertexCol.push_back(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
					break;
				}
			}
			faceColor = Modulo(faceColor + 1, 8);
		}
	}

#pragma region Buffer setup
	float* vboData = new float[vertexPos.size() * 7];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 7)] = vertexPos[i].x;
		vboData[1 + (i * 7)] = vertexPos[i].y;
		vboData[2 + (i * 7)] = vertexPos[i].z;
		vboData[3 + (i * 7)] = 1;
		vboData[4 + (i * 7)] = vertexCol[i].x;
		vboData[5 + (i * 7)] = vertexCol[i].y;
		vboData[6 + (i * 7)] = vertexCol[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);
#pragma endregion
	delete[] vboData;
	delete[] points;
	delete[] directions;
	delete[] states;
	return vertexPos.size();
}
//2D then old 3D
/*
unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	faceColor = 0;
	crossSection.SetFunction("CrossSection");

	if (points != nullptr)
		delete[] points;
	if (directions != nullptr)
		delete[] directions;
	if (states != nullptr)
		delete[] states;
	points = new glm::vec4[tetrahedrons.size() * 6];
	directions = new glm::vec4[tetrahedrons.size() * 6];
	states = new char[tetrahedrons.size() * 6];

	std::vector<Line> lines;
	lines.reserve(tetrahedrons.size() * 6);
	{
		int iLimit = tetrahedrons.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(tetrahedrons[i].lines[0]);
			lines.push_back(tetrahedrons[i].lines[1]);
			lines.push_back(tetrahedrons[i].lines[2]);
			lines.push_back(tetrahedrons[i].lines[3]);
			lines.push_back(tetrahedrons[i].lines[4]);
			lines.push_back(tetrahedrons[i].lines[5]);
		}
	}
	{
		int iLimit = lines.size();
		for (int i = 0; i < iLimit; i++)
		{
			points[i] = lines[i].point;
			directions[i] = lines[i].direction;
		}
	}
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), points);
	cl::Buffer directionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), directions);
	cl::Buffer stateBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(char) * lines.size());
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer positionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, pointBuffer);
	crossSection.SetVariable(1, directionBuffer);
	crossSection.SetVariable(2, stateBuffer);
	crossSection.SetVariable(3, matrixBuffer);
	crossSection.SetVariable(4, positionBuffer);
	crossSection.LaunchKernel(0, 0, lines.size());
	crossSection.ReadKernel(pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lines.size(), points);
	crossSection.ReadKernel(stateBuffer, GL_TRUE, 0, sizeof(char) * lines.size(), states);

	vertexPos.clear();
	vertexCol.clear();

	{
		int tLimit = tetrahedrons.size();
		for (int t = 0; t < tLimit; t++)
		{
			//Make a Deque to hold the results. Sort results to Right and left and add to the lists. Sort the lists then combine them. Create triangles from the combinations
			std::deque<glm::vec4> vertexResults;

			if (states[(t * 6)] == 0)
			{
				vertexResults.push_back(points[(t * 6)]);
			}
			if (states[(t * 6) + 1] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 1]);
			}
			if (states[(t * 6) + 2] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 2]);
			}
			if (states[(t * 6) + 3] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 3]);
			}
			if (states[(t * 6) + 4] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 4]);
			}
			if (states[(t * 6) + 5] == 0)
			{
				vertexResults.push_back(points[(t * 6) + 5]);
			}

			glm::vec4 centroid(0);
			std::vector<SortItem> rightVertices;
			std::vector<SortItem> leftVertices;
			int vectorSize = vertexResults.size();
			for (int i = 0; i < vectorSize; i++)
			{
				centroid += vertexResults[i];
			}
			centroid /= vectorSize;
			for (int i = 0; i < vectorSize; i++)
			{
				glm::vec4& vertex = vertexResults[i];
				vertex -= centroid;
				if (vertex.x > 0)
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					rightVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag)); //Sin(theta)^2 = cross/magnitude. Cross of point and (1,0). Sin is invertable between -90 and 90 deg. Cross is x * |x|
				}
				else
				{
					float mag = (vertex.x*vertex.x) + (vertex.y*vertex.y);
					leftVertices.push_back(SortItem(i, (vertex.y * std::abs(vertex.y)) / mag));
				}
				//Add to sides with values and ID
			}

			//Sort each side
			std::vector<glm::vec4> orderedVertices;
			orderedVertices.reserve(vertexResults.size());
			InsertSort(rightVertices);
			InsertSort(leftVertices);
			vectorSize = rightVertices.size();
			for (int i = 0; i < vectorSize; i++)
			{
				orderedVertices.push_back(vertexResults[rightVertices[i].ID] + centroid);
			}
			vectorSize = leftVertices.size();
			for (int i = vectorSize-1; i >= 0; i--)
			{
				orderedVertices.push_back(vertexResults[leftVertices[i].ID] + centroid);
			}
			vectorSize = orderedVertices.size() - 2;
			for (int i = 0; i < vectorSize; i++)
			{
				vertexPos.push_back(orderedVertices[0]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f,1.0f));
				vertexPos.push_back(orderedVertices[i+1]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
				vertexPos.push_back(orderedVertices[i+2]);
				vertexCol.push_back(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
	float* vboData = new float[vertexPos.size() * 7];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 7)] = vertexPos[i].x;
		vboData[1 + (i * 7)] = vertexPos[i].y;
		vboData[2 + (i * 7)] = vertexPos[i].z;
		vboData[3 + (i * 7)] = 1;
		vboData[4 + (i * 7)] = vertexCol[i].x;
		vboData[5 + (i * 7)] = vertexCol[i].y;
		vboData[6 + (i * 7)] = vertexCol[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);

	delete[] vboData;
	return vertexPos.size();
}
*/
/*
unsigned int RenderManager::SetBuffer(Camera& camera, unsigned int VBOaddress)
{
	faceColor = 0;
	crossSection.SetFunction("CrossSection");

	if (points != nullptr)
		delete[] points;
	if (directions != nullptr)
		delete[] directions;
	if (magnitude != nullptr)
		delete[] magnitude;
	points = new glm::vec4[triangles.size() * 3];
	directions = new glm::vec4[triangles.size() * 3];
	magnitude = new float[triangles.size() * 3];

	std::vector<Line> lines;
	lines.reserve(triangles.size() * 3);
	{
		int iLimit = triangles.size();
		for (int i = 0; i < iLimit; i++)
		{
			lines.push_back(triangles[i].lines[0]);
			lines.push_back(triangles[i].lines[1]);
			lines.push_back(triangles[i].lines[2]);
		}
	}
	{
		int iLimit = lines.size();
		for (int i = 0; i < iLimit; i++)
		{
			points[i] = lines[i].point;
			directions[i] = lines[i].direction;
		}
	}
	//perhaps perspect here?
	cl::Buffer pointBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), points);
	cl::Buffer directionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4) * lines.size(), directions);
	cl::Buffer magnitudeBuffer(crossSection.context, CL_MEM_HOST_READ_ONLY, sizeof(float) * lines.size());
	cl::Buffer matrixBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::mat4), camera.GetTransformValuePtr());
	cl::Buffer positionBuffer(crossSection.context, CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(glm::vec4), glm::value_ptr(camera.position));

	crossSection.SetVariable(0, pointBuffer);
	crossSection.SetVariable(1, directionBuffer);
	crossSection.SetVariable(2, magnitudeBuffer);
	crossSection.SetVariable(3, matrixBuffer);
	crossSection.SetVariable(4, positionBuffer);
	crossSection.LaunchKernel(0, 0, lines.size());
	crossSection.ReadKernel(pointBuffer, GL_TRUE, 0, sizeof(glm::vec4) * lines.size(), points);
	crossSection.ReadKernel(magnitudeBuffer, GL_TRUE, 0, sizeof(float) * lines.size(), magnitude);

	vertexPos.clear();
	vertexCol.clear();

	{
		int tLimit = triangles.size();
		for (int t = 0; t < tLimit; t++)
		{
			if ((magnitude[(t * 3)] >= 0 && magnitude[(t * 3)] <= 1)\
				&& (magnitude[(t * 3) + 1] >= 0 && magnitude[(t * 3) + 1] <= 1) && (magnitude[(t * 3) + 1] >= 0 && magnitude[(t * 3) + 1] <= 1))
			{
				vertexPos.push_back(points[(t * 3)]);
				vertexPos.push_back(points[(t * 3) + 1]);
				vertexPos.push_back(points[(t * 3) + 2]);
				switch (faceColor)
				{
				case 0:
				case 1:
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
					break;
				case 2:
				case 3:
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
					break;
				case 4:
				case 5:
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
					break;
				case 6:
				case 7:
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
					break;
				case 8:
				case 9:
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					vertexCol.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
					break;
				case 10:
				case 11:
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					vertexCol.push_back(glm::vec3(0.0f, 1.0f, 1.0f));
					break;
				}
			}
			faceColor = Modulo(faceColor + 2, 12);
		}
	}
	float* vboData = new float[vertexPos.size() * 7];

	for (int i = 0; i < vertexPos.size(); i++)
	{
		vboData[0 + (i * 7)] = vertexPos[i].x;
		vboData[1 + (i * 7)] = vertexPos[i].y;
		vboData[2 + (i * 7)] = vertexPos[i].z;
		vboData[3 + (i * 7)] = 1;
		vboData[4 + (i * 7)] = vertexCol[i].x;
		vboData[5 + (i * 7)] = vertexCol[i].y;
		vboData[6 + (i * 7)] = vertexCol[i].z;
	}
	glBindBuffer(GL_ARRAY_BUFFER, VBOaddress);
	glBufferData(GL_ARRAY_BUFFER, 7 * sizeof(float) * vertexPos.size(), vboData, GL_DYNAMIC_DRAW);

	delete[] vboData;
	return vertexPos.size();
}
*/
