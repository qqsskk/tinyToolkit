/**
 *
 *  作者: hm
 *
 *  说明: 主函数
 *
 */


#include "main.h"


static void Task()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << CURRENT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		class Task : public timer::ITask
		{
		public:
			~Task() override = default;

			void OnKill() override
			{
				std::cout << "OnKill" << std::endl;
			}

			void OnPause() override
			{
				std::cout << "OnPause" << std::endl;
			}

			void OnResume() override
			{
				std::cout << "OnResume" << std::endl;
			}

			void OnFinish() override
			{
				std::cout << "OnFinish" << std::endl;
			}

			void OnTrigger() override
			{
				std::cout << "OnTrigger : " << "[total=" << TotalCount() << ",trigger=" << TriggerCount() << "]" << std::endl;
			}
		};

		auto task = std::make_shared<Task>();

		timer::Timer timer{ };

		timer.AddTask(task, -1, 1000);

		if (timer.TotalTaskCount() != 1)
		{
			return;
		}

		while (task->TriggerCount() != 5)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		timer.Pause(task);

		if (timer.PauseTaskCount() != 1)
		{
			return;
		}

		std::this_thread::sleep_for(std::chrono::seconds(3));

		timer.Resume(task);

		if (timer.WorkTaskCount() != 1)
		{
			return;
		}

		std::this_thread::sleep_for(std::chrono::seconds(3));

		timer.Kill(task);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


static void Function()
{
	std::cout << std::endl;
	std::cout << "**************************************************" << std::endl;
	std::cout << "Ready run function [" << CURRENT_FUNC << "]" << std::endl;
	std::cout << std::endl;

	try
	{
		int32_t count = 3;

		timer::Timer timer;

		timer.AddTask([&](){ std::cout << "count : " << --count << std::endl; }, count, 1000);

		while (count != 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		count = -1;

		timer.AddTask([&](){ std::cout << "count : " << ++count << std::endl; }, count, 1000);

		while (count != 10)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc, char const * argv[])
{
	(void)argc;
	(void)argv;

	Task();
	Function();

	return 0;
}
