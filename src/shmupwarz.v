import vex
import emscripten

fn main() {

    println('Hello Sirius IV')

    game := vex.create_game('Hello World', 640, 480) 

    if emscripten.is_enabled() == true {
        emscripten.set_main_loop_arg(main_loop, game, 60, 0)
    } else {
        for game.is_running() {
                    game.render()
        }
    }


}


fn main_loop(game &vex.Game) {
    game.render()
}