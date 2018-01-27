#include "StringManipulator.hpp"
#include "DataStructure.hpp"
#include <string>
/**
 * @brief string manipulators
 * 
 */
namespace smanip 
{

    /**
     * @brief namespace for manipulator of data::Race record
     * 
     */
    namespace race 
    {

        struct Non : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                return true;
            } 
        };

        struct Year : str_manip::Manipulator<data::Race> 
        {
            bool analyze(data::Race& dest, const std::string cell) 
            {
                dest.race_id.year = cell;
                return true;
            };
        };

        struct Date : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_id.date = cell;
                return true;
            }
        };

        struct TrackCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_id.track_code = cell;
            };
        };

        struct YearRaceNo : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_id.year_race_no = cell;
            };
        };

        struct DailyRaceNo : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_id.daily_race_no = cell;
            };
        };
        
        struct GradeCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.grade_code = cell;
                return true;
            }
        };

        struct RaceTypeCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_type_code = cell;
                return true;
            }
        };

        struct RaceSymbolCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.race_symbol_code = cell;
                return true;
            }
        };

        struct WeightTypeCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.weight_type_code = cell;
                return true;
            }
        };

        struct TrackTypeCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.track_type_code = cell;
                return true;
            }
        };

        struct WeatherCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.weather_code = cell;
                return true;
            }
        };
 
        struct BabaStateCode : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.baba_state_code = cell;
                return true;
            }
        };

        struct length : str_manip::Manipulator<data::Race>
        {
            bool analyze(data::Race& dest, const std::string cell)
            {
                dest.length = std::stoi(cell);
                return true;
            }
        };

    };

    /**
     * @brief namespace for manipulator of horce record.
     * 
     */
    namespace horce 
    {

        struct Non : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                return true;
            } 
        };
       
        struct Year : str_manip::Manipulator<data::Horse> 
        {
            bool analyze(data::Horse& dest, const std::string cell) 
            {
                dest.race_id.year = cell;
                return true;
            };
        };

        struct Date : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.race_id.date = cell;
                return true;
            }
        };

        struct TrackCode : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.race_id.track_code = cell;
            };
        };

        struct YearRaceNo: str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.race_id.year_race_no = cell;
            };
        };

        struct DailyRaceNo : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.race_id.daily_race_no = cell;
            };
        };
        
        struct Name : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.name = cell; 
                return true;
            }
        };
        
        struct Pedigree : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.pedigree_id = cell; 
                return true;
            }
        };

        struct Jockey : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.jockey = cell; 
                return true;
            }
        };

        struct No : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.no = std::stoi(cell); 
                return true;
            }
        };

        struct Age : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.age = std::stoi(cell); 
                return true;
            }
        };

        struct Rank : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.rank = std::stoi(cell); 
                return true;
            }
        };

        struct ForecastedPlace : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.forecasted_place = std::stoi(cell); 
                return true;
            }
        };
        
        struct Weight : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.weight = std::stoi(cell); 
                if (dest.weight > 998 || dest.weight < 2) {
                    return false;
                }
                return true;
            }
        };
        
        struct BurdenWeight: str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.burden_weight = std::stoi(cell) * 0.1; 
                return true;
            }
        };


        struct WeightDiffSign : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                if (cell != "+" && cell != "-") {
                    return false;
                }

                dest.weight_diff_sign = cell == "+" ? 1 : -1;
                return true;
            }
        };

        struct WeightDiff : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                dest.weight_diff = std::stoi(cell);
                if (dest.weight_diff == 999){
                    return false;
                }
                return true;
            }
        };


        struct Record : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                auto ctoi = [](const char c) -> int { return c - '0'; };
                dest.record = ctoi(cell.c_str()[0]) * 60
                            + ctoi(cell.c_str()[1]) * 10
                            + ctoi(cell.c_str()[2])
                            + (ctoi(cell.c_str()[3]) + 5) * 0.1;
                
                if(dest.record <= 0 || cell.c_str()[1] > 6) {
                    return false;
                }

                return true;
            }
        };

        struct MiningData_Time : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                auto ctoi = [](const char c) -> int { return c - '0'; };
                dest.mining_data.record = 
                            ctoi(cell.c_str()[0]) * 60
                            + ctoi(cell.c_str()[1]) * 10
                            + ctoi(cell.c_str()[2])
                            + (ctoi(cell.c_str()[3]) + 5) * 0.1;
                
                if(dest.mining_data.record <= 0 || cell.c_str()[1] > 6) {
                    return false;
                }

                return true;
            }
        };

        struct MiningData_ErrP : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                auto ctoi = [](const char c) -> int { return c - '0'; };
                dest.mining_data.error_plus = 
                            ctoi(cell.c_str()[0]) * 60
                            + ctoi(cell.c_str()[1]) * 10
                            + ctoi(cell.c_str()[2])
                            + (ctoi(cell.c_str()[3]) + 5) * 0.1;
                
                if(dest.mining_data.error_plus <= 0 || cell.c_str()[1] > 6) {
                    return false;
                }

                return true;
            }
        };

        struct MiningData_ErrM : str_manip::Manipulator<data::Horse>
        {
            bool analyze(data::Horse& dest, const std::string cell)
            {
                auto ctoi = [](const char c) -> int { return c - '0'; };
                dest.mining_data.error_minus = 
                            ctoi(cell.c_str()[0]) * 60
                            + ctoi(cell.c_str()[1]) * 10
                            + ctoi(cell.c_str()[2])
                            + (ctoi(cell.c_str()[3]) + 5) * 0.1;
                
                if(dest.mining_data.error_minus <= 0 || cell.c_str()[1] > 6) {
                    return false;
                }

                return true;
            }
        };

    };

    /**
     * @brief namespace for manipulator of payout.
     * 
     */
    namespace payout 
    {

        struct Non : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                return true;
            } 
        };

        struct Year : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell) 
            {
                dest.race_id.year = cell;
                return true;
            };
        };

        struct Date : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                dest.race_id.date = cell;
                return true;
            }
        };

        struct TrackCode : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                dest.race_id.track_code = cell;
            };
        };

        struct YearRaceNo: str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                dest.race_id.year_race_no = cell;
            };
        };

        struct DailyRaceNo : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                dest.race_id.daily_race_no = cell;
            };
        };

        struct Tanshou : str_manip::Manipulator<data::PayoutManager>
        {
            bool analyze(data::PayoutManager& dest, const std::string cell)
            {
                dest.race_id.daily_race_no = cell;
            };
        };

    };
    
};
