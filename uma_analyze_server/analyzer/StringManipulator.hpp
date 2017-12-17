#ifndef STRING_MANIPULATOR_HPP
#define STRING_MANIPULATOR_HPP
#include <memory>
#include <vector>
#include <cassert>
namespace str_manip{
	/**
	 * @brief 
	 *  analyze std::String in cell and set result to <dest>. 
	 *  Derive this class and override analyze funcion.
	 * @tparam T class of destination value.
	 */
	template<class T>
	struct Manipulator
	{
		virtual bool analyze(T& dest, const std::string& cell) const 
		{
			return true;
		};
	};

	/**
	 * @brief analyze std::String in cell and set result to <dest>. 
	 *  Derive this class and override analyze funcion. 
	 * @tparam T 
	 */
	template<class T>
	struct LineAnalyzer
	{
		virtual bool analyze(T& dest, const std::vector<std::string> line)
		{
			return true;
		};
	};

	/**
	 * @brief Manipulator allay for line that pharsed and aligned as vector
	 * 
	 * @tparam T destination class 
	 * @tparam T_Manips manipulator class array
	 */
	template<class T, class... T_Manips>
	struct ManipulatorArray : public LineAnalyzer<T>
	{
	private:
		std::vector< std::shared_ptr< Manipulator<T> > > manips;

	public:
		ManipulatorArray() {
			make_maniprator_array(T_Manips() ...);
		}

	public:
		/**
		 * @brief analyze line that is pharsed and represented as vector
		 * 
		 * @param dest return value
		 * @param line 
		 * @return true All manipulator got valid value 
		 * @return false one or more manipulator got invalid value
		 */
		bool analyze(T& dest, const std::vector<std::string>& line)
		{
			bool valid = true;
			
			assert(line.size() >= manips.size());

			for(unsigned int i = 0; i < manips.size(); ++ i){
				bool res = manips[i]->analyze(dest, line[i]);
				valid = valid && res;
			}
			return valid;
		};

	private:

		void make_maniprator_array() {};
		
		template<class Head, class... Tail>
		void make_maniprator_array(Head&& head, Tail&&... tail)
		{
			manips.push_back(std::static_pointer_cast< Manipulator<T> >(std::make_shared< Head >()));
			make_maniprator_array(std::move(tail) ...);
		};

	};


}
#endif