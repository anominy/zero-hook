#pragma once

#include <windows.h>

#include <mutex>

#include <spdlog/sinks/base_sink.h>
#include <spdlog/details/null_mutex.h>

namespace zero
{
    namespace dbg
    {
        void Msg(HMODULE, const char*, ...);

        template<typename Mutex>
        class msg_sink : public spdlog::sinks::base_sink<Mutex>
        {
        private:
            HMODULE m_hModule;

        public:
            msg_sink(HMODULE hModule) : m_hModule(hModule)
            {
            }

        protected:
            void sink_it_(const spdlog::details::log_msg& msg) override
            {
                spdlog::memory_buf_t formatted;
                spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);

                Msg(m_hModule, fmt::to_string(formatted).c_str());
            }

            void flush_() override
            {
            }
        };

        using msg_sink_mt = msg_sink<std::mutex>;
        using msg_sink_st = msg_sink<spdlog::details::null_mutex>;
    } // namespace dbg
} // namespace zero
