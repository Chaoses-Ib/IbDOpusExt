cmake_minimum_required(VERSION 3.14)

project(external)

include(FetchContent)
FetchContent_Declare(IbWinCpp
    GIT_REPOSITORY https://github.com/Chaoses-Ib/IbWinCppLib.git
    GIT_TAG        a29ac95537f403ac5ae221cb744d3e82076efbf7
)
FetchContent_Declare(IbDllHijack
    GIT_REPOSITORY https://github.com/Chaoses-Ib/IbDllHijackLib.git
    GIT_TAG        a82533aff8a73129aee2a3757069f7aa89e7dd6a
)
FetchContent_Declare(IbEverything
    GIT_REPOSITORY https://github.com/Chaoses-Ib/IbEverythingLib.git
    GIT_TAG        0fda1b6a2db79112b08ebdfb1443a84959ddc042
)
FetchContent_Declare(sigmatch
    GIT_REPOSITORY https://github.com/SpriteOvO/sigmatch.git
    GIT_TAG        2ca983dc0cb78fe4f4406b67655e7ee132ecf5da
)
FetchContent_MakeAvailable(IbWinCpp IbDllHijack IbEverything sigmatch)