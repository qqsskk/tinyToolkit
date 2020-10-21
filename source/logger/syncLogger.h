#ifndef __LOGGER__SYNC_LOGGER__H__
#define __LOGGER__SYNC_LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 同步日志
 *
 */


#include "logger.h"

#include <ctime>


namespace tinyToolkit
{
	namespace logger
	{
		class API_TYPE SyncLogger : public ILogger
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 */
			SyncLogger();

			/**
			 *
			 * 构造函数
			 *
			 * @param name 名称
			 *
			 */
			explicit SyncLogger(std::string name);

			/**
			 *
			 * 析构函数
			 *
			 */
			~SyncLogger() override = default;

			/**
			 *
			 * 单例对象
			 *
			 * @return 单例对象
			 *
			 */
			static SyncLogger & Instance();

			/**
			 *
			 * 等待
			 *
			 */
			void Wait() override;

			/**
			 *
			 * 关闭
			 *
			 */
			void Close() override;

			/**
			 *
			 * 刷新
			 *
			 */
			void Flush() override;

			/**
			 *
			 * 写入
			 *
			 * @param context 上下文
			 *
			 */
			void Write(Context & context) override;

		private:
			std::tm _tm{ };

			int64_t _second{ 0 };
			int64_t _minutes{ 0 };
		};
	}
}


#endif // __LOGGER__SYNC_LOGGER__H__
