#include "KernelHelper.h"
GPUProgram::GPUProgram()
{

}
GPUProgram::GPUProgram(std::string kernelFilePath)
{
	int error = 0;
	cl::Platform::get(&platforms);

	platform = platforms.front();
	error = platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	if (error != 0)
	{
		std::cout << error << '\n';
		abort();
	}
	device = devices.front();
	context = cl::Context(device);

	CreateProgram(kernelFilePath);
}

void GPUProgram::CreateProgram(std::string kernelFilePath)
{
	cl_int error = 0;
	kernelFile = std::ifstream(kernelFilePath.c_str());
	filePaths.push_back(std::string(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>())));
	sources.push_back(cl::Program::Sources(1, std::make_pair(filePaths.back().c_str(), filePaths.back().length() + 1)));
	programs.push_back(cl::Program(context, sources.back(), &error));
	kernelFile.close();

	if (programs.back().build("-cl-std=CL1.2") != CL_SUCCESS)
	{
		std::cout << "Error building: " << programs.back().getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		std::cin.get();
		abort();
	}

	queues.push_back(cl::CommandQueue(context, device, 0Ui64, &error));
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
	kernels.push_back(cl::Kernel());
}

void GPUProgram::SetFunction(int kernelNumber, std::string functionName)
{
	cl_int error = 0;
	kernels[kernelNumber] = cl::Kernel(programs[kernelNumber], functionName.c_str(), &error);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::SetVariable(int kernelNumber, int position, cl::Buffer data)
{
	cl_int error = kernels[kernelNumber].setArg(position, data);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::LaunchKernel(int kernelNumber, int offset, int size) ///size in elements
{
	cl_int error = queues[kernelNumber].enqueueNDRangeKernel(kernels[kernelNumber], cl::NDRange(offset), cl::NDRange(size));
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::ReadKernel(int kernelNumber, cl::Buffer buffer, bool shouldWait, int offset, int count, void* dataToCopyTo) ///offset and count in bytes
{
	cl_int error = queues[kernelNumber].enqueueReadBuffer(buffer, shouldWait, offset, count, dataToCopyTo);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

