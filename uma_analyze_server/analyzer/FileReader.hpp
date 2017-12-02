#ifndef FILE_READER_HPP
#define FILE_READER_HPP
//----------------------------------------------------------
// include
//----------------------------------------------------------
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>

#include "DataStructure.hpp"
#include "StringManipulator.hpp"
//----------------------------------------------------------
// FileReader 
// Read race date on csv
//----------------------------------------------------------
class FileReader
{
public:
	struct NoCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct PlaceCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct NameCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct OAIndexCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct CIndexCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};
	struct PayCell : public str_manip::Manipulator<Horse>
	{
		bool analyze(Horse* dest, const std::string& cell) const;
	};

public:
	std::vector< std::vector<std::string> > data;

public:

	//----------------------------------------------------------
	// Constructor
	//----------------------------------------------------------
	FileReader(){};
	//----------------------------------------------------------
	// Constructor
	// 		Read inputed file and update data.
	// Parameters:
	// 		file name
	//----------------------------------------------------------
	FileReader(const std::string& file_name){ read(file_name); };
	//----------------------------------------------------------
	// read
	// 		Read inputed file and update data.
	// Parameters:
	// 		file name
	//----------------------------------------------------------
	void read(const std::string& file_name);
	//----------------------------------------------------------
	// get_track_info
	// 		convert std::vector< std::vector<std::string> > data
	//		property to TrackInfo
	// Parameters:
	// 		track_info 	return value
	//----------------------------------------------------------
	void get_track_info(TrackInfo& track_info) const;
	
private:
	//----------------------------------------------------------
	// analyze
	// 		called from to_track_info
	//		analyze vectored data and convert to TrackInfo
	// Parameters:
	// 		track_info 	return value
	//----------------------------------------------------------
	void analyze(TrackInfo& track_info) const;

};
#endif