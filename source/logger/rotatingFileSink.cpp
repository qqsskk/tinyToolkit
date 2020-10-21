/**
 *
 *  作者: hm
 *
 *  说明: 旋转文件节点
 *
 */


#include "rotatingFileSink.h"


namespace tinyToolkit
{
	namespace logger
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param name 名称
		 * @param path 路径
		 * @param maxSize 大小
		 * @param maxCount 个数
		 *
		 */
		RotatingFileSink::RotatingFileSink(std::string name, std::string path, std::size_t maxSize, std::size_t maxCount) : ISink(std::move(name)),
		                                                                                                                    _maxSize(maxSize),
		                                                                                                                    _maxCount(maxCount),
		                                                                                                                    _path(std::move(path))
		{
			if (!_file.Open(_path))
			{
				throw std::logic_error("Open rotating log file failed : " + _file.Path());
			}
		}

		/**
		 *
		 * 析构函数
		 *
		 */
		RotatingFileSink::~RotatingFileSink()
		{
			_file.Close();
		}

		/**
		 *
		 * 关闭
		 *
		 */
		void RotatingFileSink::Close()
		{
			_file.Flush();
			_file.Close();
		}

		/**
		 *
		 * 刷新
		 *
		 */
		void RotatingFileSink::Flush()
		{
			_file.Flush();
		}

		/**
		 *
		 * 写入
		 *
		 * @param context 上下文
		 *
		 */
		void RotatingFileSink::Write(const Context & context)
		{
			if (Filter() && Filter()->Decide(context))
			{
				return;
			}

			if (_file.Size() > _maxSize)
			{
				_file.Close();

				Rotating();

				if (!_file.Open(_path))
				{
					throw std::logic_error("Open rotating log file failed : " + _file.Path());
				}
			}

			_file.Write(Layout() ? Layout()->Format(context) : context.content);

			if (IsAutoFlush())
			{
				Flush();
			}
		}

		/**
		 *
		 * 旋转
		 *
		 */
		void RotatingFileSink::Rotating()
		{
			for (std::size_t i = _maxCount; i > 0; --i)
			{
				std::string src = FormatPath(i - 1);
				std::string dst = FormatPath(i);

				std::remove(dst.c_str());
				std::rename(src.c_str(), dst.c_str());
			}
		}

		/**
		 *
		 * 格式化路径
		 *
		 * @param index 索引
		 *
		 * @return 路径
		 *
		 */
		std::string RotatingFileSink::FormatPath(std::size_t index)
		{
			if (index == 0)
			{
				return _path;
			}

			std::size_t pos = _path.rfind('.');

			if (pos == std::string::npos)  /// name_1
			{
				return _path + "_" + std::to_string(index);
			}
			else if (pos == 0)  /// 1.log
			{
				return std::to_string(index) + _path;
			}
			else  /// name_1.log
			{
				return _path.substr(0, pos) + "_" + std::to_string(index) + _path.substr(pos);
			}
		}
	}
}
