# Conway's Game of Life [![C/C++ CI](https://github.com/mrundle/conways-game-of-life/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mrundle/conways-game-of-life/actions/workflows/c-cpp.yml)

This package implement's Conway's Game of Life in C, using `ncurses` to
write the grid to the terminal.

## Dependencies

This tool is targted for Unix-style environments and needs the following
tools/packages installed.

* `gcc`
* `ncurses-libs`
* `ncurses-devel`

## Usage

### Building

To build the `./life` executable, simply run:

```
make
```

### Running

```
./life
```

The output will be sized to your screen when you launch the program,
and will iterate to the next generation every 500 milliseconds.

```
...XXX............XX....................XX.XX.....
.XXX.X......X.....XX.X..............X...XX.XX.....
X..X......XX........XX..............X.............
X..X.....X.X......................................
..XX.....XXX........XX............................
.XXX....XXXX...................XX.......XXXX......
.XXX..XXX.X.X..................XX.......XX.X......
.XXX...XX..X....X.......XX.............X...X.XXX..
.X..X..X...XX...XXX...XXXXX.............XXX...XX..
..X.X..XX.XX.......X..X.....XX..XX......X....X...X
..........XX.......X.X...X...XXX.........XXX.XXX.X
..X.....X.....XX.XX...XX.XXX.XX.............XX...X
..X.....X.X...XX............X............X.X...XX.
.........XX....XX........................X.....XX.
```

## Next?

The following features would be nice to implement:

* Controls, via an overlay window displaying options
  * Start the game
  * Stop the game
  * Step forward one generation
  * Modify the generation delay from the default 500ms
* Saving and loading state
