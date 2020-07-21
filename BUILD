load("@rules_cc//cc:defs.bzl", "cc_library", "objc_library")

objc_library(
    name = "sdl2_mac",
    deps = ["@sdl2//:SDL2_lib"],
)

cc_library(
    name = "quez_lib",
    srcs = glob([
        "*.cc",
        "quez/*.cpp",
    ]),
    hdrs = glob([
        "quez/*.h",
    ]),
    deps = [
        "//glad:lib",
        "@glm//:glm_hdrs",
        "@sdl2//:SDL2_hdrs",
        "@soil//:soil_lib",
    ],
)

apple_binary(
    name = "quez_mac_build",
    minimum_os_version = "10.14",
    platform_type = "macos",
    deps = [
        "//:quez_lib",
        "//:sdl2_mac",
    ],
)

genrule(
    name = "quez_mac_bin",
    srcs = ["//:quez_mac_build"],
    outs = ["quez_mac_bin"],
    cmd = "cp $(location quez_mac_build) $(location quez_mac_bin)",
    executable = True,
)
