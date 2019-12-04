# shmupwarz in v

Do I never tire of this? Nah, same old shmup, new language - [v](https://vlang.io/)

use cmake to build:

    based on autovala cmake script
    compiles src/*.v to build/src/*.c
    compiles resulting *.c to executable 

    why?
        v builds too fast?
        to use clang 10.0
        to use pkg-config
        to target other platforms, such as emscripten