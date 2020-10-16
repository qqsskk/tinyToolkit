#ifndef __OPTION__OPTION__H__
#define __OPTION__OPTION__H__


/**
 *
 *  作者: hm
 *
 *  说明: 解析器
 *
 */


#include "description.h"

#include <unordered_map>


namespace option
{
	class API_TYPE Option
	{
	public:
		/**
		 *
		 * 单例对象
		 *
		 * @return 单例对象
		 *
		 */
		static Option & Instance();

		/**
		 *
		 * 解析
		 *
		 * @param argc 选项个数
		 * @param argv 选项数组
		 *
		 */
		void Parse(int argc, const char * argv[]);

		/**
		 *
		 * 添加描述组
		 *
		 * @param group 描述组
		 *
		 */
		void AddDescriptionGroup(const std::shared_ptr<DescriptionGroup> & group);

		/**
		 *
		 * 是否存在
		 *
		 * @param option 选项
		 *
		 * @return 是否存在
		 *
		 */
		bool Exits(const std::string & option) const;

		/**
		 *
		 * 详细信息
		 *
		 * @return 详细信息
		 *
		 */
		std::string Verbose();

		/**
		 *
		 * 获取数据
		 *
		 * @param option 选项
		 *
		 * @return 获取的数据
		 *
		 */
		template<typename ValueTypeT>
		ValueTypeT Get(const std::string & option)
		{
			auto find = _options.find(option);

			if (find == _options.end())
			{
				throw std::runtime_error("Option invalid : " + option);
			}

			if (!find->second->IsRequired())
			{
				throw std::runtime_error("Option not value : " + option);
			}

			if (!find->second->IsValid())
			{
				throw std::runtime_error("Option value invalid : " + option);
			}

			auto real = dynamic_cast<RealValue<ValueTypeT> *>(find->second->Value().get());

			if (!real->_value.template Is<ValueTypeT>())
			{
				throw std::runtime_error("Option value type invalid : " + option);
			}

			return real->_value.template Get<ValueTypeT>();
		}

	private:
		std::size_t _modeWidth{ 0 };
		std::size_t _optionWidth{ 0 };

		std::vector<std::shared_ptr<DescriptionGroup>> _groups{ };

		std::unordered_map<std::string, DescriptionInfo *> _options{ };
		std::unordered_map<std::string, DescriptionInfo *> _longOptions{ };
		std::unordered_map<std::string, DescriptionInfo *> _shortOptions{ };
	};
}


#endif // __OPTION__OPTION__H__
