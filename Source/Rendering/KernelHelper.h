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
	std::vector<cl::Program::Sources> sources;
	cl::Context context;
	std::ifstream kernelFile;
	std::vector<std::string> filePaths;
	std::vector<cl::Program> programs;
	std::vector<cl::Kernel> kernels;
	std::vector<cl::CommandQueue> queues;
	GPUProgram();
	GPUProgram(std::string);

	void CreateProgram(std::string);
	void SetFunction(int, std::string);
	void SetVariable(int, int, cl::Buffer);
	void LaunchKernel(int, int, int);
	void ReadKernel(int, cl::Buffer, bool, int, int, void*);
};