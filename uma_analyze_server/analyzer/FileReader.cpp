#include "FileReader.hpp"
bool FileReader::NameCell::analyze(Horse* dest, const std::string& cell) const 
{
	dest->name = cell;
	return !cell.empty();
};
bool FileReader::NoCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->no = stoi(cell);
	return res;
};
bool FileReader::PayCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->pay_rate = stod(cell);
	return res;
};
bool FileReader::PlaceCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else dest->place = stoi(cell);
	return res;
};
bool FileReader::OAIndexCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else if(std::string("---") == cell)	res = false;
	else dest->over_all_index = stoi(cell);
	return res;
};
bool FileReader::CIndexCell::analyze(Horse* dest, const std::string& cell) const 
{
	bool res = true;
	if (cell.empty())	res = false;
	else if(std::string("---") == cell)	res = false;
	else dest->curr_index = stoi(cell);
	return res;
};

void FileReader::read(const std::string& file_name)
{  
	// Open file
	std::ifstream ifs(file_name); 
	if (!ifs){
		std::cout << "Error: cannot open file(" << file_name << ")" << std::endl;
		exit(1);
	}

	// Get one line
	std::string str_line;
	while(getline(ifs, str_line)){
		std::string element;
		std::stringstream stream(str_line);

		// Separated by commas
		std::vector<std::string> devided_line;
		while (getline(stream, element, ',')){
			devided_line.push_back(element);
		}
		data.push_back(devided_line);
	}
};

void FileReader::get_track_info(TrackInfo& ti) const { analyze(ti); };

void FileReader::analyze(TrackInfo& track_info) const 
{
	Horse horse;

	// analyze current line
	str_manip::ManipulatorArray<
		Horse,
		PlaceCell, 
		NoCell, 
		NameCell, 
		OAIndexCell,
		str_manip::Manipulator<Horse>, 
		str_manip::Manipulator<Horse>, 
		CIndexCell, 
		str_manip::Manipulator<Horse>,
		str_manip::Manipulator<Horse>,
		str_manip::Manipulator<Horse>, 
		PayCell >()
		.analyze(&horse, line);
};

