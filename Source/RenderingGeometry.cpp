#include "RenderingGeometry.h"

//1 goes to 2, 2 goes to 3, 3 goes to 1
//Not optimized, but that doesn't really matter for this. It better shows how the algorithm works
void PrismifyByVec(Line line1, Line line2, Line line3, glm::vec4 dirVec, std::vector<Tetrahedron>& outTetra)
{
	Line holdLines[6];
	glm::vec4 holdPoint[3];
	holdLines[0] = line1;
	holdLines[1] = line2;
	holdLines[2] = line3;
	holdLines[3] = holdLines[0];
	holdLines[4] = holdLines[1];
	holdLines[5] = holdLines[2];
	holdPoint[0] = line1.point + dirVec;
	holdPoint[1] = line2.point + dirVec;
	holdPoint[2] = line3.point + dirVec;
	if (outTetra.size() != 3)
	{
		std::cout << "Error in Prismify(): vector is " << outTetra.size() <<'\n';
		abort();
	}
	holdLines[3].direction = (holdPoint[2] - holdLines[0].point);
	holdLines[4].direction = (holdPoint[2] - holdLines[1].point);
	holdLines[5].direction = dirVec;
	holdLines[3].textureDirection = (holdLines[2].textureCoords - holdLines[0].textureCoords);
	holdLines[4].textureDirection = (holdLines[2].textureCoords - holdLines[1].textureCoords);
	holdLines[5].textureDirection = glm::vec4(0);
	outTetra[0] = Tetrahedron(holdLines[0], holdLines[1], holdLines[2], holdLines[3], holdLines[4], holdLines[5]);
	holdLines[1] = holdLines[4];
	holdLines[2] = holdLines[3];
	holdLines[3] = holdLines[0];
	holdLines[4] = holdLines[1];
	holdLines[5] = holdLines[2];

	holdLines[3].direction = (holdPoint[1] - holdLines[0].point);
	holdLines[4].direction = dirVec;
	holdLines[5].direction = (holdPoint[1] - holdLines[2].point);
	holdLines[3].textureDirection = (holdLines[1].textureCoords - holdLines[0].textureCoords);
	holdLines[4].textureDirection = glm::vec4(0);
	holdLines[5].textureDirection = (holdLines[1].textureCoords - holdLines[2].textureCoords);
	outTetra[0] = Tetrahedron(holdLines[0], holdLines[1], holdLines[2], holdLines[3], holdLines[4], holdLines[5]);
	holdLines[0] = holdLines[3];
	holdLines[1] = holdLines[5];
	holdLines[3] = holdLines[0];
	holdLines[4] = holdLines[1];
	holdLines[5] = holdLines[2];

	holdLines[3].direction = dirVec;
	holdLines[4].direction = (holdPoint[0] - holdLines[1].point);
	holdLines[5].direction = (holdPoint[0] - holdLines[2].point);
	holdLines[3].textureDirection = glm::vec4(0);
	holdLines[4].textureDirection = (holdLines[0].textureCoords - holdLines[1].textureCoords);
	holdLines[5].textureDirection = (holdLines[0].textureCoords - holdLines[2].textureCoords);
	outTetra[0] = Tetrahedron(holdLines[0], holdLines[1], holdLines[2], holdLines[3], holdLines[4], holdLines[5]);
}

//Line
Line::Line()
{
	point = glm::vec4(0, 0, 0, 0);
	direction = glm::vec4(0, 0, 0, 0);
	textureCoords = glm::vec4(0, 0, 0, 0);
	textureDirection = glm::vec4(0, 0, 0, 0);
}

Line::Line(glm::vec4 pointVec, glm::vec4 directionVec, glm::vec3 textureCoordsVec, glm::vec3 textureDirectionVec)
{
	point = pointVec;
	direction = directionVec;
	textureCoords = glm::vec4(textureCoordsVec,0);
	textureDirection = glm::vec4(textureDirectionVec,0);
}

Line::Line(float xPos, float yPos, float zPos, float wPos, float xDir, float yDir, float zDir, float wDir, float xTPos, float yTPos, float zTPos, float xTDir, float yTDir, float zTDir)
{
	point = glm::vec4(xPos, yPos, zPos, wPos);
	direction = glm::vec4(xDir, yDir, zDir, wDir);
	textureCoords = glm::vec4(xTPos, yTPos, zTPos,0);
	textureDirection = glm::vec4(xTDir, yTDir, zTDir,0);
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
	direction = direction + point;
	point = matrix * point;
	direction = matrix * direction;
	direction = direction - point;
	point += center;
}
void Line::Transform(glm::mat4x4 matrix)
{
	glm::vec4 center = point + (direction*0.5f);
	point -= center;
	direction = direction + point;
	point = matrix * point;
	direction = matrix * direction;
	direction = direction - point;
	point += center;
}
void Line::TransformAround(glm::mat4x4 matrix, glm::vec4 pivot)
{
	point -= pivot;
	point = matrix * point;
	direction = matrix * direction;
	point += pivot;
}

Line Line::operator+(glm::vec4 vector)
{
	return Line(this->point + vector, this->direction, this->textureCoords, this->textureDirection);
}
Line Line::operator-(glm::vec4 vector)
{
	return Line(this->point - vector, this->direction, this->textureCoords, this->textureDirection);
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

Tetrahedron::Tetrahedron(Line line1, Line line2, Line line3, Line line4, Line line5, Line line6)
{
	lines[0] = line1;
	lines[1] = line2;
	lines[2] = line3;
	lines[3] = line4;
	lines[4] = line5;
	lines[5] = line6;
}

Tetrahedron::Tetrahedron(glm::vec4 point1, glm::vec3 point1Tex, glm::vec4 point2, glm::vec3 point2Tex, glm::vec4 point3, glm::vec3 point3Tex, glm::vec4 point4, glm::vec3 point4Tex)
{
	lines[0] = Line(point1, point2 - point1, point1Tex, point2Tex - point1Tex);
	lines[1] = Line(point1, point3 - point1, point1Tex, point3Tex - point1Tex);
	lines[2] = Line(point1, point4 - point1, point1Tex, point4Tex - point1Tex);
	lines[3] = Line(point2, point3 - point2, point2Tex, point3Tex - point2Tex);
	lines[4] = Line(point2, point4 - point2, point2Tex, point4Tex - point2Tex);
	lines[5] = Line(point3, point4 - point3, point3Tex, point4Tex - point3Tex);
}

Tetrahedron::Tetrahedron(float x1, float y1, float z1, float w1, float xt1, float yt1, float zt1, float x2, float y2, float z2, float w2, float xt2, float yt2, float zt2, float x3, float y3, float z3, float w3, float xt3, float yt3, float zt3, float x4, float y4, float z4, float w4, float xt4, float yt4, float zt4) 
{
	lines[0] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x2 - x1, y2 - y1, z2 - z1, w2 - w1), glm::vec3(xt1, yt1, zt1), glm::vec3(xt2 - xt1, yt2 - yt1, zt2 - zt1));
	lines[1] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x3 - x1, y3 - y1, z3 - z1, w3 - w1), glm::vec3(xt1, yt1, zt1), glm::vec3(xt3 - xt1, yt3 - yt1, zt3 - zt1));
	lines[2] = Line(glm::vec4(x1, y1, z1, w1), glm::vec4(x4 - x1, y4 - y1, z4 - z1, w4 - w1), glm::vec3(xt1, yt1, zt1), glm::vec3(xt4 - xt1, yt4 - yt1, zt4 - zt1));
	lines[3] = Line(glm::vec4(x2, y2, z2, w2), glm::vec4(x3 - x2, y3 - y2, z3 - z2, w3 - w2), glm::vec3(xt2, yt2, zt2), glm::vec3(xt3 - xt2, yt3 - yt2, zt3 - zt2));
	lines[4] = Line(glm::vec4(x2, y2, z2, w2), glm::vec4(x4 - x2, y4 - y2, z4 - z2, w4 - w2), glm::vec3(xt2, yt2, zt2), glm::vec3(xt4 - xt2, yt4 - yt2, zt4 - zt2));
	lines[5] = Line(glm::vec4(x3, y3, z3, w3), glm::vec4(x4 - x3, y4 - y3, z4 - z3, w4 - w3), glm::vec3(xt3, yt3, zt3), glm::vec3(xt4 - xt3, yt4 - yt3, zt4 - zt3));
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

/*
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
	lines[0] = Line(glm::vec4(position.x - scale, position.y - scale, position.z - scale, position.w), glm::vec4(scale * 2, 0, 0, 0));
	lines[1] = Line(glm::vec4(position.x + scale, position.y - scale, position.z - scale, position.w), glm::vec4(0, scale * 2, 0, 0));
	lines[2] = Line(glm::vec4(position.x + scale, position.y + scale, position.z - scale, position.w), glm::vec4(-scale * 2, 0, 0, 0));
	lines[3] = Line(glm::vec4(position.x - scale, position.y + scale, position.z - scale, position.w), glm::vec4(0, -scale * 2, 0, 0));
	lines[4] = Line(glm::vec4(position.x - scale, position.y - scale, position.z + scale, position.w), glm::vec4(scale * 2, 0, 0, 0));
	lines[5] = Line(glm::vec4(position.x + scale, position.y - scale, position.z + scale, position.w), glm::vec4(0, scale * 2, 0, 0));
	lines[6] = Line(glm::vec4(position.x + scale, position.y + scale, position.z + scale, position.w), glm::vec4(-scale * 2, 0, 0, 0));
	lines[7] = Line(glm::vec4(position.x - scale, position.y + scale, position.z + scale, position.w), glm::vec4(0, -scale * 2, 0, 0));
	lines[8] = Line(glm::vec4(position.x - scale, position.y - scale, position.z - scale, position.w), glm::vec4(0, 0, scale * 2, 0));
	lines[9] = Line(glm::vec4(position.x + scale, position.y - scale, position.z - scale, position.w), glm::vec4(0, 0, scale * 2, 0));
	lines[10] = Line(glm::vec4(position.x + scale, position.y + scale, position.z - scale, position.w), glm::vec4(0, 0, scale * 2, 0));
	lines[11] = Line(glm::vec4(position.x - scale, position.y + scale, position.z - scale, position.w), glm::vec4(0, 0, scale * 2, 0));
	center = position;
}

Cube::Cube(float px, float py, float pz, float pw, float scale)
{
	lines[0] = Line(glm::vec4(px - scale, py - scale, pz - scale, pw), glm::vec4(scale * 2, 0, 0, 0));
	lines[1] = Line(glm::vec4(px + scale, py - scale, pz - scale, pw), glm::vec4(0, scale * 2, 0, 0));
	lines[2] = Line(glm::vec4(px + scale, py + scale, pz - scale, pw), glm::vec4(-scale * 2, 0, 0, 0));
	lines[3] = Line(glm::vec4(px - scale, py + scale, pz - scale, pw), glm::vec4(0, -scale * 2, 0, 0));
	lines[4] = Line(glm::vec4(px - scale, py - scale, pz + scale, pw), glm::vec4(scale * 2, 0, 0, 0));
	lines[5] = Line(glm::vec4(px + scale, py - scale, pz + scale, pw), glm::vec4(0, scale * 2, 0, 0));
	lines[6] = Line(glm::vec4(px + scale, py + scale, pz + scale, pw), glm::vec4(-scale * 2, 0, 0, 0));
	lines[7] = Line(glm::vec4(px - scale, py + scale, pz + scale, pw), glm::vec4(0, -scale * 2, 0, 0));
	lines[8] = Line(glm::vec4(px - scale, py - scale, pz - scale, pw), glm::vec4(0, 0, scale * 2, 0));
	lines[9] = Line(glm::vec4(px + scale, py - scale, pz - scale, pw), glm::vec4(0, 0, scale * 2, 0));
	lines[10] = Line(glm::vec4(px + scale, py + scale, pz - scale, pw), glm::vec4(0, 0, scale * 2, 0));
	lines[11] = Line(glm::vec4(px - scale, py + scale, pz - scale, pw), glm::vec4(0, 0, scale * 2, 0));
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
//Pentachoron have multiple cells! Change to a tetrahedra dependent renderer.
Pentachoron::Pentachoron(glm::vec4 point1, glm::vec3 point1Tex, glm::vec4 point2, glm::vec3 point2Tex, glm::vec4 point3, glm::vec3 point3Tex, glm::vec4 point4, glm::vec3 point4Tex, glm::vec4 point5, glm::vec3 point5Tex)
{
	tetrahedra[0] = Tetrahedron(point1, point1Tex, point2, point2Tex, point3, point3Tex, point4, point4Tex);
	tetrahedra[1] = Tetrahedron(point1, point1Tex, point2, point2Tex, point3, point3Tex, point5, point5Tex);
	tetrahedra[2] = Tetrahedron(point1, point1Tex, point2, point2Tex, point5, point5Tex, point4, point4Tex);
	tetrahedra[3] = Tetrahedron(point1, point1Tex, point5, point5Tex, point3, point3Tex, point4, point4Tex);
	tetrahedra[4] = Tetrahedron(point5, point5Tex, point2, point2Tex, point3, point3Tex, point4, point4Tex);
}

Pentachoron::Pentachoron(float x1, float y1, float z1, float w1, float xt1, float yt1, float zt1, float x2, float y2, float z2, float w2, float xt2, float yt2, float zt2, float x3, float y3, float z3, float w3, float xt3, float yt3, float zt3, float x4, float y4, float z4, float w4, float xt4, float yt4, float zt4, float x5, float y5, float z5, float w5, float xt5, float yt5, float zt5)
{
	tetrahedra[0] = Tetrahedron(x1, y1, z1, w1, xt1, yt1, zt1, x2, y2, z2, w2, xt2, yt2, zt2, x3, y3, z3, w3, xt3, yt3, zt3, x4, y4, z4, w4, xt4, yt4, zt4);
	tetrahedra[1] = Tetrahedron(x1, y1, z1, w1, xt1, yt1, zt1, x2, y2, z2, w2, xt2, yt2, zt2, x3, y3, z3, w3, xt3, yt3, zt3, x5, y5, z5, w5, xt5, yt5, zt5);
	tetrahedra[2] = Tetrahedron(x1, y1, z1, w1, xt1, yt1, zt1, x2, y2, z2, w2, xt2, yt2, zt2, x5, y5, z5, w5, xt5, yt5, zt5, x4, y4, z4, w4, xt4, yt4, zt4);
	tetrahedra[3] = Tetrahedron(x1, y1, z1, w1, xt1, yt1, zt1, x5, y5, z5, w5, xt5, yt5, zt5, x3, y3, z3, w3, xt3, yt3, zt3, x4, y4, z4, w4, xt4, yt4, zt4);
	tetrahedra[4] = Tetrahedron(x5, y5, z5, w5, xt5, yt5, zt5, x2, y2, z2, w2, xt2, yt2, zt2, x3, y3, z3, w3, xt3, yt3, zt3, x4, y4, z4, w4, xt4, yt4, zt4);
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

*/