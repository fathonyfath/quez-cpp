load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "soil_lib",
    srcs = glob([
        "src/*.c",
    ]),
    hdrs = glob([
        "src/*.h",
    ]),
    strip_include_prefix = "src",
    visibility = ["//visibility:public"],
)
