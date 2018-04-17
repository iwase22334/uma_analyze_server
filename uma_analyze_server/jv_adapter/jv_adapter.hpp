#ifndef JV_ADAPTER
#define JV_ADAPTER

namespace jvlink 
{
    class Adapter
    {
    private: 
        AxJVDTLabLib::AxJVLink^  jv_link_;

    public:
        Adapter()= delete;
        Adapter(AxJVDTLabLib::AxJVLink^  jv_link): jv_link_(jv_link) {};
    
    public:
        long set_ui_properies()
        {
		    return jv_link_->JVSetUIProperties();
        }
        
	};

};

#endif