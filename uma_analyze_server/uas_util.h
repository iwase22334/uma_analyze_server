/**
 * @internal
 * @brief Define Utility functions
 * that provides type conversion, output and so on.
 */

#include <msclr/marshal_cppstd.h>

namespace {

System::String^ to_system_string(const std::string& str) {
    return gcnew System::String(str.c_str());
}

std::string to_string(System::String^ str) {
    assert(str != nullptr);
    msclr::interop::marshal_context context;
    return context.marshal_as<std::string>(str);
}

template<int N>
std::string to_string(const char(&str)[N]) {
    assert(memchr(str, '\0', N) != NULL);
    return std::string(str);
}

/**
 * @brief to string function for not null terminated string 
 * 
 * @tparam N
 * @return std::string 
 */
template<int N> 
std::string memory_to_string(const char (&data)[N]) {
    return std::string(data, N);
}

void uas_print_message(System::Windows::Forms::TextBox^ tb, System::String^ str) {
    tb->AppendText(str);
}

void uas_print_message(System::Windows::Forms::TextBox^ tb, const std::string str) {
    tb->AppendText(gcnew System::String(str.c_str()));
}

template<int N>
void uas_print_message(System::Windows::Forms::TextBox^ tb, const char(&str)[N]) {
	uas_print_message(std::string(str));
}

template<class T>
void uas_print_messag(System::Windows::Forms::TextBox^ tb, const std::vector<T>& vec) {
	uas_print_message(tb, "[ ");
	for (const auto& a : vec) {
		uas_print_message(tb, std::to_string(a));
		uas_print_message(tb, " ");
	}
	uas_print_message(tb, "]");
}

void uas_print_message(System::Windows::Forms::TextBox^ tb, System::DateTime^ date) {
	uas_print_message(tb, date->ToString("yyyymmdd"));
}

void uas_print_message(System::Windows::Forms::TextBox^ tb, System::DateTime^ from, System::DateTime^ to) {
	uas_print_message(tb, "From: ");
	uas_print_message(tb, from);
	uas_print_message(tb, ", To: ");
	uas_print_message(tb, to);
	uas_print_message(tb, "\n");
}

void uas_print_message(System::Windows::Forms::TextBox^ tb, const jvdata::id_type& id) {
    uas_print_message(tb, 
        memory_to_string(id.JyoCD)      + std::string("\n") +
        memory_to_string(id.Kaiji)      + std::string("\n") +
        memory_to_string(id.Year)       + std::string("\n") +
        memory_to_string(id.MonthDay)   + std::string("\n") +
        memory_to_string(id.Nichiji)    + std::string("\n") +
        memory_to_string(id.RaceNum)    + std::string("\n")
    );
}

System::String^ uas_generate_message(const jvdata::id_type& id) {
    return to_system_string(
        memory_to_string(id.JyoCD) + std::string("\n") +
        memory_to_string(id.Kaiji) + std::string("\n") +
        memory_to_string(id.Year) + std::string("\n") +
        memory_to_string(id.MonthDay) + std::string("\n") +
        memory_to_string(id.Nichiji) + std::string("\n") +
        memory_to_string(id.RaceNum) + std::string("\n")
    );
}

void uas_print_error(System::Windows::Forms::TextBox^ tb, const std::string str) {
	tb->AppendText(gcnew System::String(str.c_str()));
}

template<class T>
std::ostream& operator<<(std::ostream& ost, const std::vector<T>& vec) {
    ost << "[ ";
    for (const auto& val : vec) {
        ost << val << " ";
    }
    ost << "]" ;
    return ost;
}

}
