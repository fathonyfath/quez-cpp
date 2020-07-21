load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "khr",
    hdrs = ["include/KHR/khrplatform.h"],
    strip_include_prefix = "include",
)

cc_library(
    name = "lib",
    srcs = ["src/glad.c"],
    hdrs = ["include/glad/glad.h"],
    strip_include_prefix = "include",
    deps = ["//glad:khr"],
    visibility = ["//visibility:public"],
)
