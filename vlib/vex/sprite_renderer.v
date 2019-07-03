module vex

#flag linux -lSDL2
#flag linux -lSDL2_image
#flag linux -lGLEW
#flag linux -lGL
#flag linux  -I @VROOT/thirdparty/vex

#include "vex.h"

pub fn create_sprite_renderer() *SpriteRenderer
{

    s := &SpriteRenderer{ }
    return s
}
