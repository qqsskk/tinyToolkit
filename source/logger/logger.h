#ifndef __LOGGER__LOGGER__H__
#define __LOGGER__LOGGER__H__


/**
 *
 *  作者: hm
 *
 *  说明: 日志
 *
 */


#include "sink.h"

#include <mutex>
#include <unordered_map>

#include <fmt/format.h>


namespace logger
{
	class API_TYPE ILogger
	{
	public:
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 *
		 */
		explicit ILogger(std::string name);

		/**
		 *
		 * 析构函数
		 *
		 */
		virtual ~ILogger() = default;

		/**
		 *
		 * 等待
		 *
		 */
		virtual void Wait() = 0;

		/**
		 *
		 * 关闭
		 *
		 */
		virtual void Close() = 0;

		/**
		 *
		 * 刷新
		 *
		 */
		virtual void Flush() = 0;

		/**
		 *
		 * 写入
		 *
		 * @param context 上下文
		 *
		 */
		virtual void Write(Context & context) = 0;

		/**
		 *
		 * 写入
		 *
		 * @param option 操作
		 *
		 */
		void Write(LOG_OPTION_TYPE option);

		/**
		 *
		 * 写入
		 *
		 * @param priority 优先级
		 * @param content 内容
		 *
		 */
		void Write(LOG_PRIORITY_TYPE priority, const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param priority 优先级
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Write(LOG_PRIORITY_TYPE priority, const char * fmt, Args &&... args)
		{
			Write(priority, fmt::format(fmt, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Debug(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Debug(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::DEBUGS, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Info(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Info(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::INFO, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Notice(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Notice(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::NOTICE, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Warning(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Warning(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::WARNING, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Error(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Error(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::ERRORS, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Critical(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Critical(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::CRITICAL, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Alert(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Alert(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::ALERT, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 写入
		 *
		 * @param content 内容
		 *
		 */
		void Fatal(const std::string & content);

		/**
		 *
		 * 写入
		 *
		 * @param fmt 格式化
		 * @param args 参数
		 *
		 */
		template<typename... Args>
		void Fatal(const char * fmt, Args &&... args)
		{
			Write(LOG_PRIORITY_TYPE::FATAL, fmt, std::forward<Args>(args)...);
		}

		/**
		 *
		 * 添加节点
		 *
		 * @param sink 节点
		 *
		 */
		void AddSink(const std::shared_ptr<ISink> & sink);

		/**
		 *
		 * 添加节点
		 *
		 * @param name 名称
		 * @param args 参数
		 *
		 */
		template<class SinkTypeT, typename... Args>
		void AddSink(const std::string & name, Args &&... args)
		{
			AddSink(std::make_shared<SinkTypeT>(name, std::forward<Args>(args)...));
		}

		/**
		 *
		 * 移除节点
		 *
		 * @param name 节点名称
		 *
		 */
		void RemoveSink(const std::string & name);

		/**
		 *
		 * 移除节点
		 *
		 * @param sink 节点对象
		 *
		 */
		void RemoveSink(const std::shared_ptr<ISink> & sink);

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
		 * 名称
		 *
		 * @return 名称
		 *
		 */
		const std::string & Name() const;

	protected:
	    bool _isValid{ true };
		
		std::mutex _mutex{ };

		std::string _name{ };

		std::unordered_map<std::string, std::shared_ptr<ISink>> _sinks{ };
	};
}


#endif // __LOGGER__LOGGER__H__
