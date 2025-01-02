#include "CoreEngine.h"
#include <iostream>
#include "Camera.h"
#include <fstream>
#include "Util.h"
void CoreEngine::processInput(GLFWwindow* window)
{
	glm::vec3 cam_pos = m_renderer.GetActiveCamera()->GetPosition();
	glm::vec3 cam_forward = m_renderer.GetActiveCamera()->GetForward();
	glm::vec3 cam_up = m_renderer.GetActiveCamera()->GetUp();
	glm::vec3 cam_left = glm::normalize(glm::cross(cam_up, cam_forward));
	glm::vec3 deltaPos = glm::vec3(0.0f);
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_forward;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_forward;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_left;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_left;
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		deltaPos += move_speed * cam_up;
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		deltaPos -= move_speed * cam_up;
	}
	
	// heading and azimuth
	double d_theta = 0.0f;
	double d_phi = 0.0f;

	// performing updates
	cam_pos += deltaPos;
	glfwGetCursorPos(m_window.GetWindowHandle(), &d_theta, &d_phi);
	d_theta *= 0.005;
	d_phi *= 0.005;

	// enforcing invariants
	cam_forward = glm::normalize(cam_forward - float(d_phi)*cam_up - float(d_theta)*cam_left);
	cam_left = glm::normalize(cam_left - float(d_theta) * cam_forward);
	cam_up = glm::vec3(0, 1, 0);

	glfwSetCursorPos(m_window.GetWindowHandle(), 0, 0);

	// Finalizing to global camera state
	m_renderer.GetActiveCamera()->SetPosition(cam_pos);
	m_renderer.GetActiveCamera()->SetForward(cam_forward);
	m_renderer.GetActiveCamera()->SetUp(cam_up);
	
}


void CoreEngine::initCL()
{
	std::vector<cl::Platform> platforms;
	cl::Platform::get(&platforms);
	auto platform = platforms.front();
	std::vector<cl::Device> devices;
	platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);

	cl::Device* selectedDevice = nullptr;
	if (!devices.empty())
	{
		selectedDevice = &devices[0];
		for (auto dev : devices)
		{
			if (dev.getInfo<CL_DEVICE_TYPE>() == CL_DEVICE_TYPE_GPU)
			{
				selectedDevice = &dev;
			}
		}
	}

	if (selectedDevice)
	{
		std::cout << "Selected Device: " << selectedDevice->getInfo<CL_DEVICE_VENDOR>() << std::endl;

		std::string src = read_file("kernels/test.cl");
		cl::Context context(*selectedDevice);
		cl::Program program(context, { src });
		auto err = program.build();

		size_t log_size;
		clGetProgramBuildInfo(program.get(), selectedDevice->get(), CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		char* log = (char*)malloc(log_size);
		clGetProgramBuildInfo(program.get(), selectedDevice->get(), CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
		printf("Build Log: \n%s\n", log);
		free(log);

		std::vector<int> values(1024);
		for (int i = 0; i < 1024; i++)
		{
			values[i] = 1;
		}

		std::vector<int> filter(15);
		for (int i = -7; i < 8; i++)
		{
			filter[i + 7] = 1;
		}

		cl::Kernel kernel(program, "testLocalId");
		size_t workGroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(*selectedDevice);
		size_t numWorkGroups = values.size() / workGroupSize;

		cl::Buffer buffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS| CL_MEM_COPY_HOST_PTR, sizeof(int) * values.size(), values.data());
		cl::Buffer filterBuffer(context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, sizeof(int) * filter.size(), filter.data());
		cl::Buffer outBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, sizeof(int) * values.size());

		/*
		kernel.setArg(0, buffer);
		kernel.setArg(1, sizeof(int) * (workGroupSize + 14), nullptr);
		kernel.setArg(2, filterBuffer);
		kernel.setArg(3, (unsigned int)15);
		kernel.setArg(4, outBuffer);
		*/

		kernel.setArg(0, buffer);
		kernel.setArg(1, sizeof(int) * (workGroupSize), nullptr);
		kernel.setArg(2, outBuffer);

		cl::NDRange offset(0);
		cl::NDRange globalSize(values.size());
		cl::NDRange localSize(workGroupSize);

		std::vector<int> outVals(1024);

		cl::CommandQueue queue(context, *selectedDevice);
		err = queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize);

		err = queue.enqueueReadBuffer(outBuffer, CL_TRUE, 0, sizeof(int) * outVals.size(), outVals.data());
		if (err)
		{
			std::cout << "CLERR : " << err << std::endl;
		}
		
		for (int i : outVals)
		{
			std::cout << i << " ";
		}
		std::cout << "\n";
	}
	
}

void CoreEngine::loop()
{
	m_renderer.Init();
	while (m_isRunning && !glfwWindowShouldClose(m_window.GetWindowHandle()))
	{
		// Process Inputs
		processInput(m_window.GetWindowHandle());
		m_renderer.Update(m_time, DT);
		// Render Here 
		m_renderer.Render();

		// Check events
		glfwSwapBuffers(m_window.GetWindowHandle());
		glfwPollEvents();
		m_time += DT;
		if (m_window.ShouldClose())
		{
			m_isRunning = false;
		}
		std::chrono::time_point<std::chrono::high_resolution_clock> currTime = std::chrono::high_resolution_clock::now();
		std::cout << "Frame duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(currTime - m_lastFrameTime).count() << "\n";
		m_lastFrameTime = currTime;
	}
}

void CoreEngine::Start()
{
	std::cout << "Starting Main Loop" << std::endl;
	m_isRunning = true;
	glfwSetInputMode(m_window.GetWindowHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	m_lastFrameTime = std::chrono::high_resolution_clock::now();
	//initCL();
	loop();
}
