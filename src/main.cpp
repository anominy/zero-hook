#include <windows.h>

#include <thread>
#include <chrono>
#include <memory>
#include <iterator>

#include <cstdint>
#include <cstddef>

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>
#include <spdlog/sinks/basic_file_sink.h>

#include "./debug.hpp"

[[noreturn]]
DWORD WINAPI ThreadMain(LPVOID lpParam)
{
    HMODULE hClient;
    do
    {
        hClient = GetModuleHandleA("client.dll");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    while (hClient == NULL);

    HMODULE hTier0;
    do
    {
        hTier0 = GetModuleHandleA("tier0.dll");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    while (hTier0 == NULL);

    HMODULE hD3d9;
    do
    {
        hD3d9 = GetModuleHandleA("d3d9.dll");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    while (hD3d9 == NULL);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("zero-hook\\logs.txt", true);
    auto msg_sink = std::make_shared<zero::dbg::msg_sink_mt>(hTier0);
    std::vector<spdlog::sink_ptr> sinks{file_sink, msg_sink};

    auto logger = std::make_shared<spdlog::logger>("zero-hook", std::begin(sinks), std::end(sinks));
    spdlog::set_default_logger(logger);

    spdlog::info("hClient <-> {:#X}", reinterpret_cast<std::uintptr_t>(hClient));
    spdlog::info("hTier0 <-> {:#X}", reinterpret_cast<std::uintptr_t>(hTier0));
    spdlog::info("hD3d9 <-> {:#X}", reinterpret_cast<std::uintptr_t>(hD3d9));

    FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(reinterpret_cast<HMODULE>(hInstance));
        auto hThread = CreateThread(NULL, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ThreadMain), reinterpret_cast<LPVOID>(hInstance), 0, NULL);
        if (hThread != NULL)
        {
            CloseHandle(hThread);
        }
    }
    return TRUE;
}
