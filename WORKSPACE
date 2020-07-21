workspace(name = "quez")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "sdl2",
    urls = ["https://github.com/bazelregistry/sdl2/archive/4199cadf3342666c49760541c738dc7db9aaa8b3.zip"],
    strip_prefix = "sdl2-4199cadf3342666c49760541c738dc7db9aaa8b3",
)

http_archive(
    name = "glm",
    urls = ["https://github.com/g-truc/glm/archive/2929ad5a663597139276c10ef905d91e568fdc48.zip"],
    strip_prefix = "glm-2929ad5a663597139276c10ef905d91e568fdc48",
    build_file = "glm.BUILD", 
)

http_archive(
    name = "soil",
    urls = ["https://github.com/kbranigan/Simple-OpenGL-Image-Library/archive/4fff135429c4c7996791c019e6cc20a1e9fde568.zip"],
    strip_prefix = "Simple-OpenGL-Image-Library-4fff135429c4c7996791c019e6cc20a1e9fde568",
    build_file = "soil.BUILD", 
)
