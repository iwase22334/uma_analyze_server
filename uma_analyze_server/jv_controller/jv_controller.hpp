#ifndef JV_CONTROLLER_HPP
#define JV_CONTROLLER_HPP

#include <msclr/gcroot.h>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

namespace jvlink 
{

    class DownloadController 
    {
    private:
        boost::mutex jv_link_mutex_;
        msclr::gcroot<AxJVDTLabLib::AxJVLink^> jv_link_;

    private:
        boost::thread thread_;
        boost::atomic_bool thread_disabled_;

    private:
        boost::atomic_int download_maximum_; 
        boost::atomic_int download_current_; 

    public:
        DownloadController() = delete;
        DownloadController(const DownloadController&) = delete;
        DownloadController(msclr::gcroot<AxJVDTLabLib::AxJVLink^> jv_link);

    public:
        ~DownloadController();

    public:
        int get_status();
        void cancel();

    private:
        void run();
    };

};

#endif
