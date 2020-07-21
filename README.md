# Quez

Some exercise project to learn about [Bazel](https://bazel.build/). Use the chance to recover my old OpenGL games. Look at [README-original.md](README-original.md) for more information.

Some changes & stuffs that not done yet are: 

* Change GLEW to GLAD GL Loader for easier integration with current code
* Since the original audio implementation is using irrKlang, I haven't found and try any open source alternative for that library. So no sounds for the game.
* Somehow sometime Texture fails to render? Still cannot pinpoint what causing this, probably because of the shitty code I wrote years ago.

## Building

Install Bazel, then follow the specific platform that you have.

### Mac

* `bazel build //:quez_mac_bin` to build the binary
* `bazel run //:quez_mac_bin` to run the binary

## License

While the codes inside `quez` directory is under MIT license, other dependency may have different license. Please look at each of its dependency to get clear information around this.