# shmupwarz in v

Do I never tire of this? Nah, same old shmup, new language - [v](https://vlang.io/)

* ctrl-b - builds the v program using vc
* cmake  - builds the genereted c into the build folder using clang
* ./emcc - builds the generated c into the docs folder using emcc

uses V 0.1.11

* Hello Window 

        dektop - works
        emcc - works


* Hello Triangle - next

struct Texture2D {
pub mut:
    id  u32
    path string
}

pub fn test()
{
    s := []Texture2D
    t := map[string]f32{}
    u := map[string]Texture2D{}

}

map init unknown type "Texture2D"


using https://github.com/recp/cglm  (Highly Optimized Graphics Math (glm) for C)
rather than builtin glm. Why re-invent the wheel?
