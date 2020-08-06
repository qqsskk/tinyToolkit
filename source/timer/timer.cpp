/**
 *
 *  作者: hm
 *
 *  说明: 定时器
 *
 */


#include "timer.h"


#if PLATFORM_TYPE == PLATFORM_WINDOWS
#
#  include <string>
#
#elif PLATFORM_TYPE == PLATFORM_APPLE
#
#
#
#elif PLATFORM_TYPE == PLATFORM_LINUX
#
#
#
#endif


namespace timer
{
	class API_TYPE Task : public ITask
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param function 函数
		 *
		 */
		explicit Task(std::function<void()> function) : _function(std::move(function))
		{

		}

		/**
		 *
		 * 析构函数
		 *
		 */
		~Task() override = default;

		/**
		 *
		 * 触发事件
		 *
		 */
		void OnTrigger() override
		{
			if (_function)
			{
				_function();
			}
		}

	private:
		std::function<void()> _function{ };
	};


	////////////////////////////////////////////////////////////////////////////////////////////////////


	const std::size_t Timer::NEAR_BITS;
	const std::size_t Timer::WHEEL_NUM;
	const std::size_t Timer::WHEEL_BITS;

	const std::size_t Timer::NEAR_SIZE;
	const std::size_t Timer::NEAR_MASK;

	const std::size_t Timer::WHEEL_SIZE;
	const std::size_t Timer::WHEEL_MASK;


	/**
	 *
	 * 构造函数
	 *
	 */
	Timer::Timer()
	{
		_tickTime.store(0);
		_lastTime.store(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());

		_thread = std::thread
		(
			[this]()
			{
				while (_status)
				{
					Update();

					std::this_thread::sleep_for(std::chrono::milliseconds(2));
				}
			}
		);
	}

	/**
	 *
	 * 析构函数
	 *
	 */
	Timer::~Timer()
	{
		Close();

		auto ClearSpokes = [](std::vector<Event *> & spokesList)
		{
			for (Event * event : spokesList)
			{
				delete event;
			}

			spokesList.clear();
			spokesList.shrink_to_fit();
		};

		for (auto && spokes : _nearList)
		{
			ClearSpokes(spokes);
		}

		for (auto && wheel : _wheelList)
		{
			for (auto && spokes : wheel)
			{
				ClearSpokes(spokes);
			}
		}
	}

	/**
	 *
	 * 单例对象
	 *
	 * @return 单例对象
	 *
	 */
	Timer & Timer::Instance()
	{
		static Timer instance{ };

		return instance;
	}

	/**
	 *
	 * 关闭
	 *
	 */
	void Timer::Close()
	{
		_status = false;

		if (_thread.joinable())
		{
			_thread.join();
		}
	}

	/**
	 *
	 * 关闭事件
	 *
	 * @param task 任务
	 *
	 * @return 是否关闭成功
	 *
	 */
	bool Timer::Kill(const std::shared_ptr<ITask> & task)
	{
		std::lock_guard<std::mutex> lock(_taskMutex);

		auto find = _taskList.find(task);

		if (find == _taskList.end())
		{
			return false;
		}

		auto event = find->second;

		if (event == nullptr)
		{
			return false;
		}

		event->Kill();

		_taskList.erase(event->Task());
		_workList.erase(event->Task());
		_pauseList.erase(event->Task());

		return true;
	}

	/**
	 *
	 * 是否存在
	 *
	 * @param task 任务
	 *
	 * @return 是否存在
	 *
	 */
	bool Timer::Exist(const std::shared_ptr<ITask> & task)
	{
		return _taskList.find(task) != _taskList.end();
	}

	/**
	 *
	 * 暂停事件
	 *
	 * @param task 任务
	 *
	 * @return 是否暂停成功
	 *
	 */
	bool Timer::Pause(const std::shared_ptr<ITask> & task)
	{
		std::lock_guard<std::mutex> lock(_taskMutex);

		auto find = _workList.find(task);

		if (find == _workList.end())
		{
			return false;
		}

		auto event = find->second;

		if (event == nullptr)
		{
			return false;
		}

		event->Pause(_tickTime);

		_workList.erase(find);

		_pauseList.insert(std::make_pair(task, event));

		return true;
	}

	/**
	 *
	 * 恢复事件
	 *
	 * @param task 任务
	 *
	 * @return 是否恢复成功
	 *
	 */
	bool Timer::Resume(const std::shared_ptr<ITask> & task)
	{
		std::lock_guard<std::mutex> lock(_taskMutex);

		auto find = _pauseList.find(task);

		if (find == _pauseList.end())
		{
			return false;
		}

		auto event = find->second;

		if (event == nullptr)
		{
			return false;
		}

		event->Resume(_tickTime);

		AddEvent(event);

		_pauseList.erase(find);

		_workList.insert(std::make_pair(task, event));

		return true;
	}

	/**
	 *
	 * 添加任务
	 *
	 * @param task 任务
	 * @param count 次数
	 * @param interval 间隔(毫秒)
	 *
	 * @return 是否启动成功
	 *
	 */
	bool Timer::AddTask(const std::function<void()> & task, int64_t count, std::time_t interval)
	{
		if (count == 0)
		{
			return false;
		}

		return AddTask(std::make_shared<Task>(task), count, interval);
	}

	/**
	 *
	 * 添加任务
	 *
	 * @param task 任务
	 * @param count 次数
	 * @param interval 间隔(毫秒)
	 *
	 * @return 是否启动成功
	 *
	 */
	bool Timer::AddTask(const std::shared_ptr<ITask> & task, int64_t count, std::time_t interval)
	{
		if (count == 0)
		{
			return false;
		}

		std::lock_guard<std::mutex> lock(_taskMutex);

		if (Exist(task))
		{
			return false;
		}

		auto event = new Event(task, count, interval + _tickTime, interval);

		AddEvent(event);

		_taskList.insert(std::make_pair(task, event));
		_workList.insert(std::make_pair(task, event));

		return true;
	}

	/**
	 *
	 * 工作任务个数
	 *
	 * @return 工作任务个数
	 *
	 */
	std::size_t Timer::WorkTaskCount()
	{
		return _workList.size();
	}

	/**
	 *
	 * 暂停任务个数
	 *
	 * @return 暂停任务个数
	 *
	 */
	std::size_t Timer::PauseTaskCount()
	{
		return _pauseList.size();
	}

	/**
	 *
	 * 定时任务个数
	 *
	 * @return 定时任务个数
	 *
	 */
	std::size_t Timer::TotalTaskCount()
	{
		return _taskList.size();
	}

	/**
	 *
	 * 更新定时器
	 *
	 */
	void Timer::Update()
	{
		/// 这里需要用稳定时间, 防止系统时间变化
		std::time_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

		if (now < _lastTime.load())
		{
			throw std::runtime_error("Time go backwards : " + std::to_string(now) + " < " + std::to_string(_lastTime.load()));
		}

		std::time_t ticks = now - _lastTime.load();

		if (ticks > 0)
		{
			_lastTime.store(now);

			for (int i = 0; i < ticks; i++)
			{
				Executes();
			}
		}
	}

	/**
	 *
	 * 执行定时器
	 *
	 */
	void Timer::Executes()
	{
		auto index = static_cast<std::size_t>(_tickTime.load()) & NEAR_MASK;

		if (index == 0)
		{
			if (Cascade(0, static_cast<std::size_t>(_tickTime >> (NEAR_BITS + (0 * WHEEL_BITS))) & WHEEL_MASK) &&
			    Cascade(1, static_cast<std::size_t>(_tickTime >> (NEAR_BITS + (1 * WHEEL_BITS))) & WHEEL_MASK) &&
			    Cascade(2, static_cast<std::size_t>(_tickTime >> (NEAR_BITS + (2 * WHEEL_BITS))) & WHEEL_MASK))
			{
				Cascade(3, static_cast<std::size_t>(_tickTime >> (NEAR_BITS + (3 * WHEEL_BITS))) & WHEEL_MASK);
			}
		}

		++_tickTime;

		std::vector<Event *> spokesList{ };

		_nearList[index].swap(spokesList);

		for (Event * event : spokesList)
		{
			if (event == nullptr)
			{
				continue;
			}

			if (event->IsValid())
			{
				if (event->IsPause())
				{
					continue;
				}

				event->Trigger();

				if (event->Task()->TriggerCount() != event->Task()->TotalCount())
				{
					event->RevisedExpire(_tickTime);

					AddEvent(event);

					continue;
				}
			}

			DelEvent(event);
		}
	}

	/**
	 *
	 * 添加事件
	 *
	 * @param event 事件
	 *
	 */
	void Timer::AddEvent(Event * event)
	{
		std::lock_guard<std::mutex> lock(_eventMutex);

		std::time_t expire = event->Expire();

		auto offset = static_cast<uint64_t>(expire - _tickTime);

		if (offset < NEAR_SIZE) /// [0, 0x100)
		{
			auto index = static_cast<std::size_t>(expire) & NEAR_MASK;

			_nearList[index].push_back(event);
		}
		else if (offset < (1 << (NEAR_BITS + WHEEL_BITS)))  /// [0x100, 0x4000)
		{
			auto index = static_cast<std::size_t>(expire >> NEAR_BITS) & WHEEL_MASK;

			_wheelList[0][index].push_back(event);
		}
		else if (offset < (1 << (NEAR_BITS + (2 * WHEEL_BITS)))) /// [0x4000, 0x100000)
		{
			auto index = static_cast<std::size_t>(expire >> (NEAR_BITS + WHEEL_BITS)) & WHEEL_MASK;

			_wheelList[1][index].push_back(event);
		}
		else if (offset < (1 << (NEAR_BITS + (3 * WHEEL_BITS)))) /// [0x100000, 0x4000000)
		{
			auto index = static_cast<std::size_t>(expire >> (NEAR_BITS + (2 * WHEEL_BITS))) & WHEEL_MASK;

			_wheelList[2][index].push_back(event);
		}
		else if (static_cast<int64_t>(offset) < 0)
		{
			auto index = static_cast<std::size_t>(_tickTime) & NEAR_MASK;

			_nearList[index].push_back(event);
		}
		else if (offset <= 0xffffffffUL)
		{
			auto index = static_cast<std::size_t>(expire >> (NEAR_BITS + (3 * WHEEL_BITS))) & WHEEL_MASK;

			_wheelList[3][index].push_back(event);
		}
		else
		{
			throw std::runtime_error("The timing interval exceeds the maximum limit");
		}
	}

	/**
	 *
	 * 删除事件
	 *
	 * @param event 删除事件
	 *
	 */
	void Timer::DelEvent(Event * event)
	{
		std::lock_guard<std::mutex> lock(_taskMutex);

		_taskList.erase(event->Task());
		_workList.erase(event->Task());
		_pauseList.erase(event->Task());

		delete event;
	}

	/**
	 *
	 * 转动时间轮
	 *
	 * @param wheel 第几个时间轮
	 * @param index 时间轮索引
	 *
	 * @return 索引是否是起始位置
	 *
	 */
	bool Timer::Cascade(std::size_t wheel, std::size_t index)
	{
		std::vector<Event *> spokesList{ };

		_wheelList[wheel][index].swap(spokesList);

		for (Event * event : spokesList)
		{
			if (event)
			{
				AddEvent(event);
			}
		}

		return index == 0;
	}
}
