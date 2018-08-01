#ifndef JV_DATA_POOL_HPP
#define JV_DATA_POOL_HPP

#include <jv_reader/JVDataConstants.hpp>
#include <jv_reader/JVDataHandling.hpp>
#include <jv_reader/JVRecordReader.hpp>

#include <boost/optional.hpp>

#include <list>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace jvdata
{

    template<class T> 
    class JVDataPool
    {
    public:
        using farray_type = T;
		using umap_type = std::unordered_map<std::string, T>;

    private:
        umap_type farray_map_;

    public:
        /**
         * @brief filter stiring by using filter array
         * 
         * @param str input record string
         * @return FilterResult 
         */
		boost::optional<const id_type&> operator()(const std::string& str);

        /**
         * @brief return data_list
         * 
         * @return const std::list< T >& get_data 
         */
        const farray_type& get(const std::string& id) const;
		const farray_type& get(const jvdata::id_type id) const { return get(to_string(id)); };

		const umap_type& data() const { return farray_map_; };

        /**
         * @brief initialize
         * 
         */
        void reset();

    };
    
    template<class T>
    boost::optional<const id_type&> JVDataPool<T>::operator()(const std::string& str)
    {
		T farray{};
		
		// filter array returns optional, so id has invalid or valid.
		if (auto id = farray(str)) {
			
			auto it = this->farray_map_.find(to_string(id.get()));

			// same id exists in map
			if (it != farray_map_.end()) {
				it->second.operator()(str);
			}

			// id not exists in map
			else {	
				std::string id_str = to_string(id.get());
				this->farray_map_.insert({ id_str, std::move(farray) });
			}

			return id;
		}

		return boost::none;
    };

    template<class T>
    const typename JVDataPool<T>::farray_type& JVDataPool<T>::get(const std::string& id) const
    {	
		assert(this->farray_map_.find(id) != farray_map_.end());
        return this->farray_map_.at(id);
    };

    template<class T>
    void JVDataPool<T>::reset()
    {
        this->farray_map_.clear();
    };

	namespace datapool {

		using race = JVDataPool<filterarray::race>;
		using ming = JVDataPool<filterarray::ming>;

	}

};




#endif