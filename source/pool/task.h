#ifndef __POOL__TASK__H__
#define __POOL__TASK__H__


/**
 *
 *  作者: hm
 *
 *  说明: 任务池
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"

#include <queue>
#include <future>
#include <functional>


namespace pool
{
	class API_TYPE TaskPool
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param count 线程个数
		 *
		 */
		explicit TaskPool(std::size_t count = std::thread::hardware_concurrency());

		/**
		 *
		 * 析构函数
		 *
		 */
		~TaskPool();

		/**
		 *
		 * 等待
		 *
		 */
		void Wait() const;

		/**
		 *
		 * 关闭
		 *
		 */
		void Close();

		/**
		 *
		 * 暂停
		 *
		 */
		void Pause();

		/**
		 *
		 * 恢复
		 *
		 */
		void Resume();

		/**
		 *
		 * 是否为空闲状态
		 *
		 * @return 是否为空闲状态
		 *
		 */
		bool IsIdle() const;

		/**
		 *
		 * 是否为关闭状态
		 *
		 * @return 是否为关闭状态
		 *
		 */
		bool IsClose() const;

		/**
		 *
		 * 是否为暂停状态
		 *
		 * @return 是否为暂停状态
		 *
		 */
		bool IsPause() const;

		/**
		 *
		 * 是否有效
		 *
		 * @return 是否有效
		 *
		 */
		bool IsValid() const;

		/**
		 *
		 * 任务个数
		 *
		 * @return 任务个数
		 *
		 */
		std::size_t TaskSize() const;

		/**
		 *
		 * 线程个数
		 *
		 * @return 线程个数
		 *
		 */
		std::size_t ThreadSize() const;

		/**
		 *
		 * 添加任务
		 *
		 * @param function 函数
		 * @param args 参数
		 *
		 * @return 操作结果对象
		 *
		 */
		template<typename FunctionTypeT, typename ... Args>
		std::future<typename std::result_of<FunctionTypeT(Args...)>::type> AddTask(FunctionTypeT && function, Args &&... args)
		{
			using ResultType = typename std::result_of<FunctionTypeT(Args...)>::type;

			auto task = std::make_shared<std::packaged_task<ResultType()>>
			(
				std::bind(std::forward<FunctionTypeT>(function), std::forward<Args>(args)...)
			);

			std::future<ResultType> future = task->get_future();

			{
				std::lock_guard<std::mutex> lock(_mutex);

				if (_isClose)
				{
					throw std::runtime_error("Task Pool already close");
				}

				_tasks.emplace
				(
					[task]()
					{
						(*task)();
					}
				);
			}

			_condition.notify_one();

			return future;
		};

	private:
		bool _isValid{ false };
		bool _isClose{ false };
		bool _isPause{ false };

		mutable std::mutex _mutex{ };

		std::condition_variable _condition{ };

		std::atomic<std::size_t> _idles{ 0 };

		std::vector<std::thread> _threads{ };

		std::queue<std::function<void()>> _tasks{ };
	};
}


#endif // __POOL__TASK__H__
