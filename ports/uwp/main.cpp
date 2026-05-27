#include "pk2_entry.hpp"

#include <SDL_hints.h>
#include <SDL_main.h>
#include <SDL_system.h>

#include <filesystem>
#include <string>

#include <Windows.h>

namespace
{

std::filesystem::path getSdlPath(SDL_WinRT_Path pathId)
{
    const char* path = SDL_WinRTGetFSPathUTF8(pathId);
    return path ? std::filesystem::u8path(path) : std::filesystem::path{};
}

int runPk2Uwp(int, char**)
{
    SDL_SetHint(SDL_HINT_WINRT_HANDLE_BACK_BUTTON, "1");

    const auto installPath = getSdlPath(SDL_WINRT_PATH_INSTALLED_LOCATION);
    const auto dataPath = getSdlPath(SDL_WINRT_PATH_LOCAL_FOLDER);
    std::filesystem::create_directories(dataPath);

    pk2_set_paths((installPath / "res").u8string(), dataPath.u8string());
    pk2_init();
    pk2_main(false, false, false, std::string{});

    return 0;
}

}

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    return SDL_WinRTRunApp(runPk2Uwp, nullptr);
}
