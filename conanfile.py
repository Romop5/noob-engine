from conans import ConanFile, CMake


class NoobEngine(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = [
        "opengl/system",
        "glew/2.2.0",
        "glm/0.9.9.8",
        "sdl/2.0.20",
        # required to fix compilation of wayland's client (deps from sdl)
        "libffi/3.4.2",
    ]
    generators = "cmake"
