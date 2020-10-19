#ifndef __OPTION__ANY__H__
#define __OPTION__ANY__H__


/**
 *
 *  作者: hm
 *
 *  说明: 任意
 *
 */


#include "../common/macro.h"
#include "../common/symbol.h"

#include <memory>
#include <stdexcept>


namespace option
{
	class API_TYPE Any
	{
		class API_TYPE Base
		{
		public:
			/**
			 *
			 * 析构函数
			 *
			 */
			virtual ~Base() = default;

			/**
			 *
			 * 克隆
			 *
			 * @return 克隆对象
			 *
			 */
			virtual std::unique_ptr<Base> Clone() = 0;

			/**
			 *
			 * 类型
			 *
			 * @return 类型
			 *
			 */
			virtual const std::type_info & Type() const = 0;
		};

		template<typename ValueTypeT>
		class API_TYPE Derived : public Base
		{
		public:
			/**
			 *
			 * 构造函数
			 *
			 * @param value 数据
			 *
			 */
			template<typename TypeT>
			explicit Derived(TypeT && value) : _value(std::forward<TypeT>(value))
			{

			}

			/**
			 *
			 * 析构函数
			 *
			 */
			~Derived() override = default;

			/**
			 *
			 * 克隆
			 *
			 * @return 克隆对象
			 *
			 */
			std::unique_ptr<Base> Clone() override
			{
				return std::unique_ptr<Base>(new Derived<ValueTypeT>(_value));
			}

			/**
			 *
			 * 类型
			 *
			 * @return 类型
			 *
			 */
			const std::type_info & Type() const override
			{
				return typeid(ValueTypeT);
			}

		public:
			ValueTypeT _value;
		};

	public:
		/**
		 *
		 * 构造函数
		 *
		 */
		Any() = default;

		/**
		 *
		 * 移动构造函数
		 *
		 * @param rhs 右值对象
		 *
		 */
		Any(Any && rhs) noexcept;

		/**
		 *
		 * 拷贝构造函数
		 *
		 * @param lhs 左值对象
		 *
		 */
		Any(const Any & lhs);

		/**
		 *
		 * 构造函数
		 *
		 * @param value 内容
		 *
		 */
		template<typename ValueTypeT>
		explicit Any(ValueTypeT && value) : _content(new Derived<typename std::decay<ValueTypeT>::type>(std::forward<ValueTypeT>(value)))
		{

		}

		/**
		 *
		 * 清理
		 *
		 */
		void Clear();

		/**
		 *
		 * 类型判断
		 *
		 * @return 是否符合类型
		 *
		 */
		template<typename ValueTypeT>
		bool Is() const
		{
			return _content->Type() == typeid(ValueTypeT);
		}

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
		 * 获取内容
		 *
		 * @return 内容
		 *
		 */
		template<class ValueTypeT>
		ValueTypeT Get()
		{
			if (!_content)
			{
				throw std::logic_error("Can not cast nullptr");
			}

			if (!Is<ValueTypeT>())
			{
				throw std::logic_error("Can not cast difference type");
			}

			auto derived = dynamic_cast<Derived<ValueTypeT> *>(_content.get());

			if (derived == nullptr)
			{
				throw std::logic_error("cast result nullptr");
			}

			return derived->_value;
		}

		/**
		 *
		 * 类型
		 *
		 * @return 类型
		 *
		 */
		const std::type_info & Type() const;

		/**
		 *
		 * 重载=操作
		 *
		 * @param rhs 右值对象
		 *
		 * @return 对象
		 *
		 */
		Any & operator=(Any && rhs) noexcept;

		/**
		 *
		 * 重载=操作
		 *
		 * @param lhs 左值对象
		 *
		 * @return 对象
		 *
		 */
		Any & operator=(const Any & lhs);

		/**
		 *
		 * 重载=操作
		 *
		 * @param value 数据
		 *
		 * @return 对象
		 *
		 */
		template<typename ValueTypeT>
		Any & operator=(ValueTypeT && value)
		{
			_content.reset(new Derived<typename std::decay<ValueTypeT>::type>(std::forward<ValueTypeT>(value)));

			return *this;
		}

	private:
		std::unique_ptr<Base> _content{ };
	};
}


#endif // __OPTION__ANY__H__
