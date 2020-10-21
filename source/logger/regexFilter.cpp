/**
 *
 *  作者: hm
 *
 *  说明: 正则过滤器
 *
 */


#include "regexFilter.h"


namespace tinyToolkit
{
	namespace logger
	{
		/**
		 *
		 * 构造函数
		 *
		 * @param rule 规则
		 *
		 */
		RegexFilter::RegexFilter(std::regex rule) : _rule(std::move(rule))
		{

		}

		/**
		 *
		 * 构造函数
		 *
		 * @param rule 规则
		 *
		 */
		RegexFilter::RegexFilter(const std::string & rule) : _rule(rule)
		{

		}

		/**
		 *
		 * 过滤处理
		 *
		 * @param context 上下文
		 *
		 * @return 是否过滤
		 *
		 */
		bool RegexFilter::Filter(const Context & context)
		{
			return std::regex_match(context.content, _rule);
		}
	}
}
