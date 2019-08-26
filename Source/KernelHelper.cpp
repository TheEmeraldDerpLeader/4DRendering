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

	kernelFile = std::ifstream(kernelFilePath.c_str());
	src = std::string(std::istreambuf_iterator<char>(kernelFile), (std::istreambuf_iterator<char>()));

	sources = cl::Program::Sources(1, std::make_pair(src.c_str(), src.length() + 1));
	context = cl::Context(device);
	program = cl::Program(context, sources, &error);
	if (error != 0)
	{
		std::cout << error << '\n';
		abort();
	}
	queue = cl::CommandQueue(context, device, 0Ui64, &error);
	if (error != 0)
	{
		std::cout << error << '\n';
		abort();
	}

	if (program.build("-cl-std=CL1.2") != CL_SUCCESS)
	{
		std::cout << "Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device) << std::endl;
		std::cin.get();
		abort();
	}
}

void GPUProgram::SetFunction(std::string functionName)
{
	cl_int error = 0;
	kernel = cl::Kernel(program, functionName.c_str(), &error);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::SetVariable(int position, cl::Buffer data)
{
	cl_int error = kernel.setArg(position, data);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::LaunchKernel(int kernelNumber, int offset, int size) ///size in elements
{
	cl_int error = queue.enqueueNDRangeKernel(kernel, cl::NDRange(offset), cl::NDRange(size));
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

void GPUProgram::ReadKernel(cl::Buffer buffer, bool shouldWait, int offset, int count, void* dataToCopyTo) ///offset and count in bytes
{
	cl_int error = queue.enqueueReadBuffer(buffer, shouldWait, offset, count, dataToCopyTo);
	if (error != 0)
	{
		std::cout << error;
		abort();
	}
}

