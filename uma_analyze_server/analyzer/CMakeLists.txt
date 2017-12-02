cmake_minimum_required(VERSION 2.8.3)
###########
## Build ##
###########
add_definitions("-Wall -std=c++11")
add_library(ModAnalyzer STATIC
	FileReader.cpp
	DataStructure.cpp
	UmaDataAnalyzer.cpp
)

#target_link_libraries(
#)


