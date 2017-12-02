#ifndef STRING_MANIPULATOR_HPP
#define STRING_MANIPULATOR_HPP
#include <memory>
#include <vector>
#include <cassert>
namespace str_manip{
	//----------------------------------------------------------
	// Manipulator
	// 		analyze std::String in cell and set result to <dest>. 
	//		Derive this class and override analyze funcion.
	// Template parameters:
	// 		T         output class
	//----------------------------------------------------------
	template<class T>
	struct Manipulator
	{
		virtual bool analyze(T* const dest, const std::string& cell) const 
		{
			return true;
		};
	};

	//----------------------------------------------------------
	// LineAnalyzer
	// Template parameters:
	// 		T         output class
	//----------------------------------------------------------
	template<class T>
	struct LineAnalyzer
	{
		virtual bool analyze(T* const dest, const std::vector<std::string> line)
		{
			return true;
		};
	};

	//----------------------------------------------------------
	// ManipulatorArray
	// Template parameters:
	// 		T         output class
	//		T_Manips  Manipulators
	//----------------------------------------------------------
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
		//----------------------------------------------------------
		// analyze
		//		Analyze inputed text line.
		// Parameters:
		// 		dest      destination structure
		// 		line   	  input text line
		//----------------------------------------------------------
		bool analyze(T* const dest, const std::vector<std::string>& line)
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
		//----------------------------------------------------------
		// make_maniprator_array
		// 		Extract T_Manips instance to std::vector
		//		Called from analyze(...) function only.
		// Template Parameters:
		//		Head 	Head in Template parameter pack
		//		Tail 	Other parameters in template parameter pack
		//----------------------------------------------------------
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