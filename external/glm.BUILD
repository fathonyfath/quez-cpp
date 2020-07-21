load("@rules_cc//cc:defs.bzl", "cc_library")

cc_library(
    name = "glm_hdrs",
    hdrs = glob([
        "glm/*.hpp",
        "glm/*.h",
        "glm/*.inl",
        "glm/**/*.hpp",
        "glm/**/*.h",
        "glm/**/*.inl",
    ]),
    visibility = ["//visibility:public"],
)
