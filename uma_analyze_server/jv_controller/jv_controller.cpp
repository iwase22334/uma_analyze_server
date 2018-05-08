#include "jv_controller.hpp"

#include <cassert>
#include <string>
#include <stdexcept>

namespace jvlink {

    DownloadController::DownloadController(msclr::gcroot<AxJVDTLabLib::AxJVLink^> jv_link) :
        jv_link_(jv_link),
        thread_disabled_(false),
        download_maximum_(0),
        download_current_(0),
        thread_(this->run);
    {};

    DownloadController::~DownloadController()
    {
        if (thread_.joinable()) {
            this->cancel();
            thread_.join();
        }
    };

    int DownloadController::get_status()
    {
        assert(download_maximum_ >= download_current_);
        assert(download_maximum_ >= 0);
        assert(download_current_ >= 0);

        return download_maximum_ - download_current_;
    };

    void DownloadController::cancel()
    {
        thread_disabled_ = true;
        
        // lock guard block
        {
            boost::lock_guard<boost::mutex> lock(jv_link_mutex_);
            jv_link_->JVCancel();
        }
    };

    void DownloadController::run()
    {
        while (download_maximum_ - download_current_ > 0 && !thread_disabled_) {
            int st;

            // lock guard block
            {
                boost::lock_guard<boost::mutex> lock(jv_link_mutex_);
                st = jv_link_->JVStatus();
            }
            
            if (st < 0) {
                throw std::runtime_error(
                      std::string("DownloadCOntroller: jvstatus returned errorcode - (")
                    + std::to_string(download_current_)
                    + std::string(")"));
            }

            download_current_ = st;
        }
    };

};
