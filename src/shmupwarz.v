import vex
import emscripten

fn main() {

    println('Hello Sirius IV')

    game := vex.create_game(vex.Config {
        title: 'Shmupwarz'
        width: 640
        height: 480
        major: 3
        minor: 0
        images: vex.IMG_INIT_PNG
    })

    if emscripten.is_enabled() == true {
        emscripten.set_main_loop_arg(main_loop, game, 60, 0)
    } else {
        for game.is_running() {
            game.update()
            game.render()
        }
    }
}


fn main_loop(game &vex.Game) {
    game.update()
    game.render()
}