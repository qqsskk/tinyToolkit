#ifndef __LOGGER__SIMPLE_LAYOUT__H__
#define __LOGGER__SIMPLE_LAYOUT__H__


/**
 *
 *  作者: hm
 *
 *  说明: 简单布局
 *
 */


#include "layout.h"


namespace tinyToolkit
{
	namespace logger
	{
		class API_TYPE SimpleLayout : public ILayout
		{
		public:
			/**
			 *
			 * 格式化
			 *
			 * @param context 上下文
			 *
			 * @return 格式化后内容
			 *
			 */
			std::string Format(const Context & context) override;
		};
	}
}


#endif // __LOGGER__SIMPLE_LAYOUT__H__
