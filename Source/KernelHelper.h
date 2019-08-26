#pragma once
#include <CL/cl.hpp>

#include <iostream>
#include <fstream>
#include <vector>

class GPUProgram
{ //To Do: allow support for multiple kernels, multi-dimensional ND range, and smart range where template determines type
public:
	std::vector<cl::Platform> platforms;
	cl::Platform platform;
	std::vector<cl::Device> devices;
	cl::Device device;
	cl::Program::Sources sources;
	cl::Context context;
	std::ifstream kernelFile;
	std::string src;
	cl::Program program;
	cl::Kernel kernel;
	cl::CommandQueue queue;
	GPUProgram();
	GPUProgram(std::string);

	void SetFunction(std::string);
	void SetVariable(int, cl::Buffer);
	void LaunchKernel(int, int, int);
	void ReadKernel(cl::Buffer, bool, int, int, void*);
};